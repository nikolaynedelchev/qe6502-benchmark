# qe6502-benchmark Test Methodology

This document describes how the benchmark suite should be read and interpreted: what each test category validates, how failures are counted, why some cores do not report bus-trace accuracy, and why the benchmark tries to drive memory through host-visible bus handlers or callbacks whenever a core exposes them.

The benchmark is not only a speed contest. It is also an integration-quality check: a useful 6502/65C02 emulator core must be usable by a host machine implementation, not merely able to execute a closed internal RAM image. A C64, Apple II, NES, BBC Micro, or custom board emulator needs to observe and service CPU memory accesses. The host often needs to implement RAM/ROM mapping, bank switching, I/O registers, DMA-visible memory, wait states, interrupt-visible state, and device side effects. For that reason, benchmark results should be read together with the integration notes for each core.

## Test categories

The project currently uses three broad kinds of tests:

1. **Smoke tests**
2. **Klaus functional/performance tests**
3. **SingleStepTests**

Each category answers a different question. Passing one category does not imply passing the others.

## Smoke tests

Smoke tests are small sanity tests used to verify that a core is linked correctly, can be reset or initialized, can execute a few basic operations, and can interact with the benchmark wrapper without crashing.

A smoke test is intentionally shallow. It should catch integration breakage, obvious register/state failures, or unusable wrappers. It is not an instruction-set conformance test.

A smoke result is reported as `PASS` or `FAIL`.

## Klaus tests

The Klaus tests run the well-known 6502/65C02 functional test ROMs through each integrated core. They are useful because they exercise long instruction streams and catch many functional implementation errors.

The benchmark reports each Klaus result as:

```text
<core> <test name> PASS/FAIL  runs=<n> cycles=<ticks> seconds=<s> MHz=<mhz>
```

when the run passes. The speed number is computed from the emulator's reported CPU/bus ticks and wall-clock time over measured runs.

### What Klaus validates

Klaus validates that the CPU can execute the test program to its expected success address. In practice this means it is a strong functional test for broad instruction behavior.

### What Klaus does not validate

Klaus does not compare every instruction against an oracle after every instruction. It does not prove:

- exact status register state after every instruction;
- exact decimal-mode edge-case flags/results in all possible cases;
- exact cycle count for each instruction;
- exact bus read/write sequence;
- exact undocumented opcode behavior;
- exact behavior of every 65C02 model variant.

A core can pass Klaus and still fail many SingleStepTests. This is expected and should not be interpreted as a contradiction.

### Klaus failures in aggregate runners

The aggregate test executables may include cores that are known to fail some Klaus variants. In particular, some 65C02 candidates are intentionally kept in the matrix even though they fail extended Klaus coverage.

For `all_65c02_tests`, a core-level Klaus failure is treated as an informative test result, not as a harness failure. The process exit code remains success unless the harness itself encounters an error, such as an invalid corpus path, bad input, a log-writing failure, or another infrastructure problem.

## SingleStepTests

SingleStepTests are the strictest correctness tests in this project. They execute one instruction at a time from a generated corpus. Each test case contains:

- initial CPU state;
- initial RAM cells relevant to the case;
- expected final CPU state;
- expected final RAM cells relevant to the case;
- expected cycle-by-cycle memory/bus operations.

The common corpus loader parses JSON files into an in-memory corpus. Aggregate runners load a given corpus once, then pass the same in-memory corpus to all relevant core runners. This avoids repeatedly loading large external JSON corpora for every core.

Corpus selection order:

1. `--singlestep-dir <path>` or `--path <path>` command-line argument;
2. `QE6502_SINGLESTEP_TESTS_DIR` environment variable;
3. bundled light corpus compiled into the benchmark tree.

If an explicit path is supplied but invalid, this is a harness error. The runner should not silently fall back to the bundled light corpus.

For 65C02 corpora, missing or empty opcode JSON files are valid. An opcode with zero cases contributes no passed or failed opcode to the summary. It is not a loader error and should not be marked failed merely because the corpus contains no cases for that opcode.

## SingleStep accuracy levels

Each SingleStep result can report up to three independent accuracy levels:

1. **instruction**
2. **cycle-count**
3. **bus-trace**

A core can support some levels and not others.

### Instruction accuracy

Instruction accuracy validates the final architectural state after executing one instruction.

The harness compares:

- program counter (`PC`);
- stack pointer (`S` or `SP`);
- accumulator (`A`);
- index register `X`;
- index register `Y`;
- processor status (`P`);
- every expected RAM cell listed in the test case's final state.

The comparison is exact. Status flags are not masked by default. If a core uses a different `B`/unused-bit policy in `P`, that difference is still reported as an instruction mismatch.

Instruction accuracy does not validate the number of cycles or the order of memory accesses. A core can have correct final state but wrong timing.

### Cycle-count accuracy

Cycle-count accuracy validates whether the core reports or performs the same number of cycles as the SingleStep case expects.

This level is supported only when the harness can obtain a meaningful per-instruction cycle count from the core. It does not require observing the complete bus sequence. It only checks the total cycle count for the instruction.

A core can pass instruction accuracy and fail cycle-count accuracy. That usually means the instruction eventually produced the right state, but the timing model differs from the corpus.

### Bus-trace accuracy

Bus-trace accuracy validates the full memory/bus operation stream for an instruction.

The harness compares every expected cycle:

- cycle index;
- bus address;
- read/write direction;
- data value.

This is stricter than both instruction and cycle-count accuracy. A core can have correct final state and correct total cycles but still fail bus trace because it performs dummy reads, write timing, stack activity, or operand fetches differently.

Bus-trace support requires a real cycle/bus-facing API. Merely exposing read/write callbacks is not enough if the core executes an entire instruction semantically and then only reports a cycle count.

## Failure counting

SingleStep summary percentages are counted **per opcode**, not per test case.

For each opcode and each supported level:

- if all cases for that opcode pass, the opcode passes that level;
- if one or more cases for that opcode fail, the opcode is counted as one failed opcode for that level;
- the detail TSV also records the number of failed cases for that opcode.

This means the headline percentages can look severe even when only a small fraction of individual cases failed. For detailed diagnosis, read the TSV log and inspect `*_failed_cases` columns.

Opcodes with zero cases are excluded from the denominator. They do not pass or fail.

Unsupported levels are printed as `unsupported` and are not included in failed counts.

## NMOS opcode classification

For NMOS SingleStep summaries, opcodes are split into:

- `legal`
- `illegal`

The legal group contains documented NMOS 6502 opcodes. The illegal group contains undocumented NMOS opcodes.

The harness still executes every opcode byte against the core. It does not skip undocumented opcodes just because a core may not implement them. If a core prints diagnostics for undefined opcodes, the harness may suppress that stderr noise while still executing the opcode.

Special NMOS KIL/JAM handling is not an opcode skip. For cores where KIL/JAM would otherwise produce an infinite or synthetic post-jam stream, the harness compares the meaningful initial cycles and avoids comparing artificial post-KIL cycles.

## 65C02 opcode classification

For 65C02 SingleStep summaries, the project does not use the NMOS `legal/illegal` split. In the 65C02 corpora, the useful distinction is:

- `functional`
- `nop`

`functional` includes every non-NOP operation, including model-specific real operations such as Rockwell bit operations and WDC wait/stop operations.

Examples that are classified as `functional`:

- normal 65C02 instructions such as `ADC`, `SBC`, `BRA`, `STZ`, `TSB`, `TRB`, stack operations, branches, jumps, loads, stores, shifts, and rotates;
- Rockwell `RMB`, `SMB`, `BBR`, and `BBS` families;
- WDC `WAI` and `STP`;
- any Synertek/ST real operation present in the selected corpus.

`nop` includes the official `NOP` and all defined 65C02 NOP variants, including multi-byte NOPs. The point of this category is not to call them illegal; it is to separate lower-importance decode/length/timing exactness issues from failures in operations that perform real work.

A core that fails many `nop` opcodes but few `functional` opcodes may still be a close functional match for a model. This is why the 65C02 reports should be read using the `functional` and `nop` columns, not only the total failure percentage.

## Detail TSV logs

Each SingleStep runner writes a detail TSV file. The exact filename includes the core and model name.

For NMOS, the TSV contains a `legal_nmos` column.

For 65C02, the TSV contains an `opcode_group_65c02` column with values:

- `functional`
- `nop`

Common columns include:

```text
core
model
corpus
opcode
mnemonic
addressing_mode
cases_run
instruction_status
instruction_failed_opcode
instruction_failed_cases
cycle_count_status
cycle_count_failed_opcode
cycle_count_failed_cases
bus_trace_status
bus_trace_failed_opcode
bus_trace_failed_cases
```

The TSV is the preferred place to diagnose which opcodes failed and whether a failure is broad or caused by a small number of cases.

## Host-visible memory access requirement

A CPU benchmark is more realistic when the host program controls memory through the same API style a real machine emulator would use.

The benchmark therefore prefers integrations where reads and writes go through one of the following, depending on the library:

- explicit bus pins;
- read/write callbacks;
- a virtual bus interface;
- host-provided memory read/write functions;
- a wrapper hook that lets the harness observe or serve memory operations.

This matters because many real systems cannot be modeled by a CPU with a private flat RAM array only. A host needs to implement memory maps and side effects. Examples include:

- ROM/RAM banking;
- memory-mapped I/O;
- peripheral registers that change on read or write;
- stack-visible or interrupt-visible memory behavior;
- bus wait states or externally visible dummy reads/writes;
- device side effects triggered by address, direction, and data.

If a core hides fetches, stack accesses, or addressing-mode reads inside a private memory array that the host cannot intercept, its speed may still be measurable, but the result is less representative of using that core in a real emulator.

## Core integration notes

### qe6502

`qe6502` is the reference-style integration in this benchmark. The harness drives the CPU through its bus/tick interface. Each tick exposes the bus address, read/write direction, and bus data path. The host-side memory array supplies read data and records writes.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: supported.

For 65C02, the benchmark uses explicit CPU models:

- WDC corpus: `qe6502_model_wdc`;
- Rockwell corpus: `qe6502_model_rw`;
- Synertek/ST corpus: `qe6502_model_st`.

For WDC `WAI`/`STP`, the harness stops after the expected cycle count rather than waiting for a normal next opcode fetch.

### floooh/chips

The `floooh/chips` 6502 exposes a pin/tick style API. The harness can observe address pins, data pins, and read/write direction on each tick.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: supported.

This makes it one of the strongest independent bus-level comparison cores in the NMOS matrix.

### O2

The O2 integration uses a harness CPU with host-provided memory read/write functions. The read and write functions update a host memory array and append memory operations to the trace.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: supported.

O2 is NMOS-only in the current benchmark.

### Peddle

Peddle uses a C++ bus abstraction. The benchmark implements a bus object whose `read` and `write` methods serve the host memory array and record trace entries.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: supported.

This is a good integration model because the host is naturally part of memory access.

### vrEmu6502

`vrEmu6502` accepts host read and write callbacks. The benchmark uses those callbacks to serve the memory array, so instruction final-state and memory side effects are checked through host-visible memory access.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: unsupported.

Bus trace is unsupported because the public execution model used here does not expose a real cycle-by-cycle bus stream. The core can execute an instruction and report its cycle count, but the callback sequence is not treated as an authoritative per-cycle bus trace with dummy/idle accesses.

65C02 model mapping:

- WDC corpus: CPU initialized as `CPU_W65C02`;
- Rockwell corpus: CPU initialized as `CPU_R65C02`;
- Synertek/ST corpus: CPU initialized as `CPU_65C02`.

`CPU_65C02` was selected as the best fit for the Synertek/ST corpus after testing it against WDC, Rockwell, and Synertek/ST corpora. It had far fewer functional instruction failures against Synertek/ST than against WDC or Rockwell, although it still had many NOP-group mismatches.

### fake6502

The `fake6502` NMOS integration uses harness-side helper functions to load memory, set CPU state, execute one instruction, read final state, and read final memory cells. A callback can collect semantic memory accesses, but the core does not expose a true per-cycle bus trace suitable for the benchmark's bus-trace level.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: unsupported.

### fake65c02

The `fake65c02` integration uses wrapper functions to clear/load memory, set CPU state, execute one instruction, and read back state and memory. It is tested against all three 65C02 corpora: WDC, Rockwell, and Synertek/ST.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: unsupported.

It is kept in the matrix even if it fails extended Klaus coverage, because the benchmark is intended to measure and characterize candidates, not hide imperfect cores.

### gianlucag/mos6502

This core accepts host read and write functions, which is good for functional integration. However, the current harness runs one instruction and receives a cycle count rather than a complete cycle-by-cycle bus stream.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: unsupported.

The benchmark enables the core's optional illegal opcode table for broader NMOS coverage.

### sflib6502

`sflib6502` is an important special case. The library exposes callback tables for memory reads, writes, and control-transfer calls, and the benchmark uses callbacks where practical in Klaus-style runs. However, internally the library does not route all memory activity through host callbacks.

In the upstream implementation, ordinary data reads and writes can use `getMemory`/`putMemory` and therefore invoke callbacks when installed. But several important operations are direct accesses to the internal `memory` array, including:

- opcode fetch through `memory[PC++]`;
- operand fetches in addressing-mode macros, such as absolute, zero-page, indexed, indirect, and relative operand reads;
- stack push/pop through direct `memory[0x0100 + S]` access;
- interrupt/vector helper paths and some call/return handling that directly read or write `memory`;
- cycle accounting through `tick`, while `tickIf` is empty in this version, so some conditional extra cycles are not represented.

This means the host cannot fully interpose on every CPU memory operation. Fetches, many operand reads, and stack accesses are hidden inside the core's memory buffer. As a result, `sflib6502` is not a fully realistic bus-integration benchmark in the same sense as `qe6502`, `floooh/chips`, O2, or Peddle.

The speed benchmark for `sflib6502` is therefore somewhat compromised: it measures the library's internal execution speed with a partly host-visible memory model, not the cost of a fully host-mediated machine bus. This does not make the result useless, but it should not be compared naively against cores that expose every bus access to the host.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: unsupported.

The SingleStep runner suppresses stderr during the run because the core prints diagnostics for undefined instructions. Opcodes are still executed; only the noise is suppressed.

### sflib65c02

`sflib65c02` has the same broad integration caveats as `sflib6502`, because it is based on the same library style. It is tested against all three 65C02 corpora.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: unsupported.

It is kept in the 65C02 matrix even though it is known to fail extended Klaus coverage. The failures are part of the measured characterization.

### olcNES

The olcNES-style core can be driven instruction-by-instruction and can report a cycle count, but it executes instruction semantics internally and counts cycles afterward. The current benchmark does not treat it as exposing a true bus trace.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: unsupported.

### chris-pikul/mos6502

This core uses a C++ bus/device abstraction, but the current execution path performs instruction semantics in a way that does not provide a reliable per-cycle trace for the benchmark.

SingleStep support:

- instruction: supported;
- cycle-count: supported;
- bus-trace: unsupported.

The harness suppresses noisy diagnostics for illegal instructions during SingleStep execution. Opcodes are still executed and counted.

## Reading aggregate results

### `all_6502_tests`

`all_6502_tests` covers the NMOS candidates. It supports several development flags, including `--singlestep-only`, `--skip-klaus`, `--skip-smoke`, and `--skip-singlestep`.

For NMOS SingleStep, the aggregate runner loads the NMOS corpus once and runs all NMOS SingleStep candidates against it.

### `all_65c02_tests`

`all_65c02_tests` covers the 65C02 candidates. It intentionally keeps the command-line surface small:

```text
all_65c02_tests --singlestep-only
```

With `--singlestep-only`, smoke and Klaus tests are skipped and only the 65C02 SingleStep matrix runs.

The current 65C02 SingleStep matrix tests:

- `qe6502` against WDC, Rockwell, and Synertek/ST corpora;
- `vrEmu6502` against WDC, Rockwell, and Synertek/ST corpora using the model mapping described above;
- `fake65c02` against WDC, Rockwell, and Synertek/ST corpora;
- `sflib65c02` against WDC, Rockwell, and Synertek/ST corpora.

Core-level failures are reported but do not make `all_65c02_tests` return a failing process exit code. Harness errors still do.

## Practical interpretation guide

When reading a result, prefer this order:

1. Check whether the level is supported. Do not compare `unsupported` bus-trace results as failures.
2. For SingleStep, read opcode-level failure counts first, then use failed case counts in TSV for diagnosis.
3. For 65C02, read `functional` before `nop`. A high NOP failure rate is less serious than a high functional failure rate.
4. Do not treat Klaus PASS as proof of SingleStep exactness.
5. Do not treat speed numbers as equivalent unless the memory integration model is comparable.
6. Be careful with cores that hide memory inside the CPU. Their speed may be excellent, but the host-integration realism is lower.
7. Treat external-core mismatches as characterization data unless the benchmark explicitly defines that core as a strict reference.

## Summary of supported SingleStep levels

| Core | Model group | Instruction | Cycle count | Bus trace |
| --- | --- | --- | --- | --- |
| qe6502 | NMOS, WDC, Rockwell, Synertek/ST | yes | yes | yes |
| floooh/chips | NMOS | yes | yes | yes |
| O2 | NMOS | yes | yes | yes |
| Peddle | NMOS | yes | yes | yes |
| vrEmu6502 | NMOS, WDC, Rockwell, Synertek/ST best-fit | yes | yes | no |
| fake6502 | NMOS | yes | yes | no |
| fake65c02 | WDC, Rockwell, Synertek/ST corpora | yes | yes | no |
| gianlucag/mos6502 | NMOS | yes | yes | no |
| sflib6502 | NMOS | yes | yes | no |
| sflib65c02 | WDC, Rockwell, Synertek/ST corpora | yes | yes | no |
| olcNES | NMOS | yes | yes | no |
| chris-pikul/mos6502 | NMOS | yes | yes | no |

