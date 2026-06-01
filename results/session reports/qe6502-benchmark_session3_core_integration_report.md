# qe6502-benchmark — Session 3 core integration report

This report summarizes the work done in **ChatGPT session 3** on the `qe6502-benchmark` project.

The purpose of this document is not to hand off the development process. It is a technical integration report intended to help produce a final objective assessment of each emulator core: how easy it was to integrate, how portable/flexible it is, how well it fits the benchmark methodology, what had to be patched, what failed, and what impressions were formed during actual work.

## Scope of this session

This session started from the state where the project already had several 6502 and 65C02 cores integrated. The main work in this session was:

- Review three newly vendored cores:
  - `vrEmu6502`
  - `O2`
  - `Peddle`
- Integrate them into the benchmark project.
- Add or extend aggregate runners where appropriate.
- Investigate the two failing external 65C02 candidates:
  - `fake65c02`
  - `sflib65c02`
- Add a common NMOS opcode metadata helper.
- Start a SingleStepTests framework.
- Add initial SingleStep NMOS runners for:
  - `qe6502`
  - `floooh/chips`
- Add detailed SingleStep per-opcode TSV logs.

The latest generated code artifact in this session was **v38**.

## General workflow and philosophy followed

The project uses versioned patch/full-tree artifacts. In this session, every accepted change produced a versioned patch and a full-tree zip. New work was based on the latest accepted zip.

A major rule throughout the session was to avoid broad, unapproved changes, especially to vendored emulator cores. Vendored patches were only accepted when the scope was clear and justified. In several cases, I deliberately stopped at diagnosis instead of patching, because the change would have represented changing the upstream emulator’s behavior rather than merely adapting it to the benchmark harness.

This distinction mattered most for:

- `fake65c02`: diagnosis showed Klaus extended could be made to pass by changing opcode-table semantics, but this was rejected as too invasive for the benchmark’s purpose.
- `sflib65c02`: diagnosis showed missing opcode-family support, which would amount to implementing substantial new CPU functionality.

The benchmark goal was treated as integration and evaluation, not silently improving every competitor until it passes.

## Important methodology context

The benchmark is not just measuring raw speed. A core is also judged by how well it hands memory/bus control to the embedding application.

The most valuable integration model is one where the host can participate in each CPU bus cycle. This matters for realistic machine emulation, bank switching, memory-mapped I/O, and cycle-accurate behavior.

The benchmark therefore increasingly distinguishes three levels of correctness:

1. **Instruction-level correctness** — final CPU state and final memory effects after one instruction.
2. **Cycle-count correctness** — whether the core reports/performs the expected number of cycles.
3. **Full memory/bus operation correctness** — whether the core performs the exact expected memory operations cycle by cycle.

Not all cores can support all three levels. Unsupported levels should be marked as unsupported, not as failures.

## Newly reviewed and integrated cores

## 1. vrEmu6502

### Integration result

`vrEmu6502` was integrated in version **v28**.

Added:

- `toolbox/vremu6502`
- `vremu6502_smoketest`
- `vremu6502_klaus_benchmark`
- `vremu6502_klaus_65c02_benchmark`
- Entries in `all_6502_tests`
- Entries in `all_65c02_tests`

No vendored core changes were required.

### API and integration experience

`vrEmu6502` was the easiest and most rewarding of the three newly added cores.

Positive points:

- C99 implementation.
- Simple to compile as a static library.
- Has a tick-style API via `vrEmu6502Tick()`.
- Supports read/write callbacks.
- Supports multiple CPU models:
  - `CPU_6502`
  - `CPU_6502U`
  - `CPU_65C02`
  - `CPU_W65C02`
  - `CPU_R65C02`
  - `CPU_6510` alias
- README/upstream claims were broadly consistent with benchmark behavior.

The main integration caveat is that the callback API does not have a clean `user_data` parameter. This required the toolbox bridge to use an active/static context style, similar in spirit to `fake6502`. This is not ideal for multi-instance embedding, but it is manageable inside the benchmark.

### Test results observed

The integrated targets passed:

- NMOS smoke
- NMOS Klaus standard
- WDC 65C02 Klaus standard
- WDC 65C02 Klaus extended
- Rockwell 65C02 Klaus standard
- Rockwell 65C02 Klaus extended

Observed cycle counts from the local run:

- NMOS Klaus standard: `481206820`
- WDC/Rockwell 65C02 standard: `482806605`
- WDC/Rockwell 65C02 extended: `334525005`

### Fit for benchmark methodology

Very good.

Because `vrEmu6502` exposes tick-style execution, it is a strong candidate for SingleStep testing at least for instruction-level and cycle-count accuracy, and likely for bus/memory trace accuracy depending on how precisely its tick API exposes or allows observation of bus operations.

It is also valuable because it supports both NMOS and multiple 65C02 variants, making it one of the few external cores that can be compared against `qe6502` across several CPU models.

### Opinion

`vrEmu6502` is one of the strongest external cores in this benchmark set. It is portable, relatively clean, compact enough to integrate easily, and supports enough CPU variants to be useful beyond a single NMOS comparison.

The lack of callback `user_data` is a design weakness for embedding, but not a blocker. Overall it is a very good comparison candidate.

## 2. O2

### Integration result

`O2` was integrated in versions **v29** and **v30**, then added to `all_6502_tests` in **v33**.

Added:

- `toolbox/o2`
- `o2_smoketest`
- `o2_klaus_benchmark`
- Entry in `all_6502_tests`

Vendored changes were required.

### API and integration experience

`O2` is small and easy to build.

Positive points:

- Minimal C++ source layout: essentially `O2.hpp` and `O2.cpp`.
- Easy to add to CMake.
- Has `cycle()` and `reset()` style execution.
- Has `setRW(read, write)` using `std::function`, which makes host memory integration straightforward.
- Integrates naturally as an NMOS-only core.

Negative points:

- It is not a 65C02 candidate.
- Initial Klaus NMOS run failed because decimal mode arithmetic was missing.
- It had a macOS build issue due to a symbol collision with `OVERFLOW` from `<math.h>`.

### Required patches

Two vendored issues were fixed.

#### Decimal-mode ADC/SBC

The original `O2` arithmetic handled `SED/CLD` flag changes but did not implement BCD decimal behavior for `ADC` and `SBC`.

Klaus NMOS standard requires decimal arithmetic. A minimal decimal-mode fix was added to `O2.cpp`, after which Klaus NMOS passed.

This was considered acceptable because it was analogous to a missing CPU feature needed for documented NMOS behavior, not a broad rewrite of the emulator.

#### macOS `OVERFLOW` macro collision

On macOS, including system headers can define `OVERFLOW` as a macro. `O2.hpp` had an enum member named `OVERFLOW`, causing a compile failure:

```text
error: expected identifier
note: expanded from macro 'OVERFLOW'
```

The enum member was renamed to `OVERFLOW_FLAG` and all internal uses were updated.

This was a portability fix rather than a semantic CPU fix.

### Test results observed

After the two fixes:

- O2 smoke: PASS
- O2 Klaus NMOS standard: PASS

Observed cycle count:

- NMOS Klaus standard: `481206815`

This is five cycles lower than the trusted `481206820` count used by `qe6502`, `floooh`, and `vrEmu6502`, but Klaus itself passed. This should be kept in mind when using O2 for cycle-level comparisons.

### Fit for benchmark methodology

Good for instruction-level NMOS testing.

Potentially usable for cycle-count testing because it has cycle stepping, but its cycle count behavior needs scrutiny because of the observed Klaus workload cycle-count difference.

Bus/memory trace accuracy may be possible but needs verification. The read/write callback API is convenient, but it must be checked whether every expected memory access is exposed at the right cycle, including dummy/idle accesses.

### Opinion

`O2` is easy to integrate and quite portable after the macOS rename. It is a reasonable lightweight NMOS core, but not a strong architecture reference.

Its need for decimal arithmetic patching means upstream was not fully suitable for Klaus NMOS as-is. Still, the patch was small and clear. I would treat O2 as a practical lightweight competitor, not as a high-confidence cycle/bus accuracy reference until SingleStep results prove it.

## 3. Peddle

### Integration result

`Peddle` was integrated in two steps:

- **v31**: build/link/smoke only
- **v32**: Klaus NMOS benchmark
- **v34**: added to `all_6502_tests`

Added:

- `toolbox/peddle`
- `peddle_smoketest`
- `peddle_klaus_benchmark`
- Entry in `all_6502_tests`

No vendored core changes were required.

### API and integration experience

`Peddle` was more complex than `vrEmu6502` and `O2`, but it integrated successfully.

Positive points:

- It has a proper host memory abstraction through virtual `read`/`write` methods.
- It supports cycle-style execution through repeated `execute()` calls.
- It has support for several 6502-family variants:
  - 6502
  - 6507
  - 6510
  - 8502
- It includes 6510/8502-style processor port support, which is interesting for future C64/6510 work.
- It passed Klaus NMOS without vendored changes.

Caveats:

- It is a heavier C++ object-model style core.
- Virtual calls make the integration more flexible but may have performance cost.
- The vendored project looked like it might need C++20 at first, but the benchmark integration compiled with C++17.
- The internal `clock` field did not advance in the simple standalone benchmark usage, so the benchmark counted cycles by counting repeated `execute()` calls rather than relying on the internal clock.

### Test results observed

- Peddle smoke: PASS
- Peddle Klaus NMOS standard: PASS

Observed Klaus result:

- cycles: `481206820`
- local speed around `93.28 MHz` in the container

### Fit for benchmark methodology

Good for instruction-level and cycle-count testing.

Potentially useful for bus/memory trace testing because memory access is routed through virtual methods, but it still needs careful SingleStep validation to determine whether all dummy/idle accesses are externally visible in the same way as the test corpus expects.

Because Peddle is heavier and object-oriented, it may not be ideal as a speed baseline, but it is useful as a flexible integration-style comparison.

### Opinion

Peddle is not as lightweight or straightforward as `vrEmu6502`, but it is a respectable integration candidate. The API is more system-emulation-friendly than very minimal C cores, especially because of its bus abstraction and 6510-related features.

I would not treat it as a raw-speed competitor. I would treat it as a useful example of a more complete C++ embedding model.

## Investigation-only 65C02 work

## 4. fake65c02

### Existing integration status

`fake65c02` had already been integrated before this session. Current status remains:

- smoke: PASS
- Klaus 65C02 extended: FAIL

This session investigated why it fails but deliberately did not produce a patch.

### Diagnosis

The first major failure is caused by 65C02 NOP opcode operand-length semantics.

Example observed around the Klaus extended test:

```asm
0C08: 02 C8   ; 65C02 two-byte NOP immediate
0C0A: CA      ; DEX
0C0B: CA      ; DEX
```

`fake65c02` has opcode `$02` mapped to a NOP handler, but its addressing mode table treats it as implied/one-byte. Therefore the operand byte `$C8` is executed as an instruction (`INY`) instead of being consumed as an operand byte. This corrupts CPU state and causes the later failure.

Further local throwaway experimentation showed that fixing the 65C02 NOP addressing modes and increasing the timeout guard allowed Klaus extended to pass locally.

The NOP-family addressing corrections would include opcodes such as:

- `$02,$22,$42,$62,$82,$C2,$E2` as two-byte NOP/immediate style
- `$44` as zero-page NOP
- `$54,$D4,$F4` as zero-page,X NOPs
- `$5C` as absolute NOP
- `$DC,$FC` as absolute,X NOPs

### Why it was not patched

The user decided not to patch this because it changes opcode-table semantics across multiple opcodes. This is more than a tiny missing-feature fix like adding decimal arithmetic.

The benchmark should therefore mark `fake65c02` as a failing 65C02 extended candidate rather than silently maintaining/fixing its opcode matrix.

### Opinion

`fake65c02` is easy to integrate and useful as a practical 65C02-ish external candidate, but it should not be presented as passing Klaus extended in this project.

The failure is not catastrophic from an engineering standpoint, but fixing it would mean taking responsibility for correcting the vendored emulator. That is outside the desired benchmark methodology for this project.

## 5. sflib65c02

### Existing integration status

`sflib65c02` had already been integrated before this session. Current status remains:

- smoke: PASS
- Klaus 65C02 extended: FAIL

This session investigated why it fails but deliberately did not patch it.

### Diagnosis

The failure happens very early in Klaus extended at opcode `$0F`:

```text
PC=$072A
opcode=$0F
```

For Rockwell/WDC-style 65C02 testing, `$0F` is part of the `BBR` family:

```asm
BBR0 zp,rel
```

In `sflib6502/lib6502.c`, this opcode is marked illegal/undefined:

```c
_(0f, ill, implied, 2)
```

This means the core lacks at least the Rockwell bit-branch opcode family.

Likely missing families include:

- `RMB0..RMB7`
- `SMB0..SMB7`
- `BBR0..BBR7`
- `BBS0..BBS7`

That is 32 opcodes, not a small one-line fix.

There were also concerns that existing `TSB/TRB` behavior might need further validation after the first missing opcode is addressed.

### Why it was tested at all

`sflib65c02` was not tested because it was known to be a complete WDC/Rockwell 65C02. It was tested because the source contains enough CMOS/65C02-looking functionality to make it a plausible candidate:

- `BRA`
- `STZ`
- `TSB`
- `TRB`
- `PHX/PLX`
- `PHY/PLY`
- other 65C02-like operations

The smoke test passed, confirming partial 65C02 behavior. Klaus extended then correctly exposed that the implementation is not complete enough for the extended model tests.

### Why it was not patched

Adding the missing Rockwell/WDC opcode families would be a substantial new CPU implementation effort. That is beyond benchmark integration.

### Opinion

`sflib65c02` should be reported as partial 65C02-looking support, not as a complete 65C02 implementation. It is useful to keep in the benchmark because its failure mode is informative, but it should be marked clearly:

```text
smoke PASS, Klaus 65C02 extended FAIL; missing extended Rockwell bit-branch opcode family.
```

## NMOS opcode metadata helper

### Version

Added in **v35**.

### Purpose

A common helper was added to provide metadata for NMOS opcodes, including whether an opcode is legal/documented NMOS.

Added:

- `toolbox/common/include/benchmark6502/opcode_metadata.hpp`
- `toolbox/common/CMakeLists.txt`
- `opcode_metadata_smoke_test.cpp`

### Metadata provided

For each opcode:

- opcode byte
- mnemonic
- addressing mode
- instruction length
- base cycles
- legal NMOS flag
- page-cross extra-cycle marker
- branch extra-cycle marker

### Important use

This helper became important for SingleStep statistics. The project now distinguishes:

- documented/legal NMOS opcodes: 151
- illegal/undocumented NMOS opcodes: 105

SingleStep statistics can report total failures, legal-opcode failures, and illegal-opcode failures separately.

### Opinion

This is a good foundation piece. It makes later reporting more objective, avoids ad hoc opcode classification, and will be useful when later extending the same idea to 65C02 categories.

For 65C02, the planned later distinction is not just legal vs illegal, but also real instructions vs illegal opcodes that became defined NOPs.

## SingleStepTests framework work

## 6. Common SingleStep infrastructure

### Version

Initial infrastructure added in **v36**.

### Purpose

The project started integrating SingleStepTests as a statistical accuracy matrix, not simply a pass/fail test.

The intended design is:

- Load a model corpus once into memory.
- Reuse that in-memory corpus for all candidate cores.
- Avoid loading large 1GB full corpora repeatedly for each core.
- Use the vendored light corpus by default.
- Use an external full corpus path when explicitly provided.

Corpus path priority:

1. `--singlestep-dir <path>`
2. `QE6502_SINGLESTEP_TESTS_DIR`
3. bundled light corpus

If an explicit external path is invalid, the runner should fail rather than silently falling back to the light corpus.

### Per-opcode statistics

A key requirement is that statistics are per opcode, not simply per test case.

If even one test case for a given opcode fails at a given level, then that whole opcode is counted as failed for that level.

Example:

- If `LDA` has 10,000 cases and one case fails instruction-level comparison, then the `LDA` opcode is counted as failed for instruction-level statistics.
- The runner still continues through all remaining cases.
- It only stops on harness errors, crashes, bad JSON, invalid corpus, etc.

The three levels tracked are:

1. instruction-level
2. cycle-count
3. bus-trace / full memory operation trace

### Legal vs illegal NMOS split

For NMOS SingleStep statistics, the output reports:

- total opcode failures
- legal NMOS opcode failures
- illegal/undocumented NMOS opcode failures

This avoids hiding illegal opcode behavior inside a single aggregate percentage.

### First integrated core: qe6502

`qe6502` NMOS was the first SingleStep runner.

It supports all three levels:

- instruction-level
- cycle-count
- full bus trace

With the bundled light corpus, observed result:

```text
qe6502 NMOS SingleStep:
  cases: 2048
  instruction: total 0/256 failed, legal 0/151, illegal 0/105
  cycle-count: total 0/256 failed, legal 0/151, illegal 0/105
  bus-trace:   total 0/256 failed, legal 0/151, illegal 0/105
```

KIL/JAM handling follows the previously agreed policy: compare the real pre-jam cycles and avoid comparing synthetic post-KIL cycles.

### Opinion

This framework direction is important. It turns SingleStepTests from a simple yes/no validation tool into a comparative diagnostic tool. That is much more useful for evaluating external cores, because many of them will be partially correct or only support certain accuracy levels.

## 7. floooh/chips SingleStep NMOS

### Version

Added in **v37**.

### Why floooh was chosen next

`floooh/chips` was chosen after `qe6502` because it is the strongest independent bus/tick-style external reference core.

It has a pin/tick model and should theoretically support all three SingleStep comparison levels.

### Added behavior

- `floooh_chips_singlestep_nmos`
- `floooh_chips_toolbox::run_singlestep_nmos(...)`
- Integration into `all_6502_tests`
- Shared in-memory corpus use together with `qe6502`

Progress logging was also added:

- Before loading corpus:
  - `Loading SingleStep NMOS corpus...`
- Before each core runner:
  - `Starting qe6502 NMOS SingleStep...`
  - `Starting floooh/chips NMOS SingleStep...`

### Observed light-corpus result

```text
floooh/chips NMOS SingleStep:
  cases: 2048
  instruction: total 8/256 failed, legal 4/151, illegal 4/105
  cycle-count: total 0/256 failed, legal 0/151, illegal 0/105
  bus-trace:   total 7/256 failed, legal 2/151, illegal 5/105
```

`all_6502_tests` still reported its normal aggregate test summary as passing, because SingleStep mismatches are statistical findings and not fatal harness failures.

### Interpretation

This is a useful finding. `floooh/chips` remains an excellent competitor and methodology reference, but the SingleStep light corpus already shows differences from the exact expected traces/states for some opcodes.

The cycle-count result was perfect in the light corpus, while instruction-level and bus-trace results showed failures. This implies that the core can match timing counts while still differing in state or per-cycle memory behavior for some opcodes.

### Opinion

`floooh/chips` remains one of the most important external comparison points. It has a clean bus-level philosophy close to `qe6502`. The SingleStep results are not a reason to discard it; rather, they demonstrate why the new three-level statistics are valuable.

## 8. Detailed SingleStep log files

### Version

Added in **v38**.

### Purpose

The console summary is not enough to diagnose exactly which opcodes failed. A detailed TSV output was added.

Generated files include:

- `singlestep_qe6502_nmos_detail.tsv`
- `singlestep_floooh_chips_nmos_detail.tsv`

They are written to the current working directory when the runner is executed. They are not pre-generated inside the source zip.

### Columns

The TSV report includes:

- core
- model
- corpus
- opcode
- mnemonic
- addressing mode
- legal NMOS flag
- cases run
- instruction status
- instruction failed opcode flag
- instruction failed cases
- cycle-count status
- cycle-count failed opcode flag
- cycle-count failed cases
- bus-trace status
- bus-trace failed opcode flag
- bus-trace failed cases

### Important distinction

The console summary counts failed opcodes.

The detail log adds how many test cases failed inside each opcode bucket. This allows later analysis such as:

- opcode failed because one rare case failed
- opcode failed because all cases failed
- instruction-level is fine but bus-trace differs
- illegal opcodes behave differently from legal opcodes

### Opinion

The TSV report is an important bridge between high-level statistics and debugging. It should be kept as machine-readable output so later scripts can summarize or compare sessions.

A future small improvement would be to print the absolute path of the generated log file, because users running through IDEs may not immediately know the current working directory.

## Current core assessment summary from this session

| Core | Integration ease | Vendored patch needed | Test fit | Overall impression |
|---|---:|---:|---|---|
| `vrEmu6502` | Easy | No | Strong NMOS + 65C02 candidate | One of the best external cores added this session |
| `O2` | Easy | Yes: decimal ADC/SBC + macro rename | Good NMOS instruction-level; cycle/bus needs validation | Lightweight and portable after small fixes |
| `Peddle` | Medium | No | Good NMOS; bus trace needs validation | Flexible C++ integration model, heavier but useful |
| `fake65c02` | Already integrated | Not patched in this session | Smoke OK, Klaus extended FAIL | Failure is fixable but patch would alter opcode-table semantics too much |
| `sflib65c02` | Already integrated | Not patched in this session | Smoke OK, Klaus extended FAIL | Partial 65C02-like support; missing major extended opcode families |
| `qe6502` SingleStep | Existing core | No | Full three-level SingleStep exactness | Anchor/reference core |
| `floooh/chips` SingleStep | Existing core | No | Strong bus-level candidate; light corpus shows some mismatches | Best independent bus-level comparison point |

## Notes on cores not newly integrated but relevant to this session

### qe6502

`qe6502` served as the SingleStep framework anchor. It supports the full bus/tick model and passed the bundled light NMOS SingleStep corpus at all three levels with 0 failed opcodes.

This reinforces the core’s role as the reference point for the project.

### floooh/chips

`floooh/chips` is still the strongest independent reference competitor because of its pin/tick model. Its SingleStep mismatches are exactly the type of finding this benchmark should expose: not merely whether a Klaus program passes, but where instruction state, cycle count, and memory trace behavior diverge.

## Important future TODO: 6510 adapter/tutorial

A recurring design conclusion is that `qe6502` probably does **not** need a separate `qe6502_model_6510` at this stage.

The 6510 is essentially an NMOS 6502 core plus a memory-mapped 6-bit I/O port at addresses `$0000` and `$0001`. For C64-style systems, those port bits control memory banking and external lines. That belongs naturally in the host/system layer, not necessarily inside the CPU core.

A future documentation/tutorial task should be created:

```text
Create a detailed qe6502 6510 adapter/tutorial showing how to implement 6510 support as qe6502_model_nmos plus a host-side $0000/$0001 I/O port adapter.
```

The guide should cover:

- `$0000` as data direction register
- `$0001` as data port / output latch / input readback
- read behavior:

```c
read_0001 = (port_latch & port_ddr) | (external_inputs & ~port_ddr);
```

- optional pull-up/floating input modeling
- writes to `$0000` and `$0001`
- updating the machine memory map after port changes
- C64-style banking as system-layer behavior
- comparison with `floooh/chips`, which includes 6510 port state in the CPU-side model
- why this demonstrates `qe6502` bus/tick flexibility

This task should be treated as a portability/integration example, not as a new CPU model unless the project later explicitly decides otherwise.

## Recommendations for later sessions

### Continue SingleStep integration gradually

The next best SingleStep candidates after `qe6502` and `floooh/chips` are:

1. `vrEmu6502` NMOS — important because it is strong and tick-style.
2. `Peddle` NMOS — useful to validate virtual bus behavior.
3. `O2` NMOS — useful to see whether cycle/bus behavior is strong after the decimal patch.
4. Other NMOS cores, likely with reduced capability levels depending on APIs.

### Keep loading corpus once

For full SingleStepTests, a model corpus can be very large. The aggregate runner should load all JSON files for a given model once and then pass the in-memory corpus to all candidate cores.

Do not make each core load the corpus from disk independently.

### Preserve statistical reporting

Do not collapse SingleStep results into simple pass/fail. Keep reporting:

- per-opcode failure count
- total/legal/illegal NMOS split
- instruction/cycle-count/bus-trace levels separately
- unsupported capability separately from failure

### Be careful with vendored patches

Continue distinguishing between:

- small portability fixes
- small missing documented CPU feature fixes
- broad semantic rewrites of a third-party core

The project’s credibility depends on reporting external cores honestly rather than silently turning them into different emulators.

## Final session 3 opinion

The most positive integration outcome of this session was `vrEmu6502`: it was easy to integrate, did not need vendored patches, supports multiple CPU models, and passed both NMOS and 65C02 Klaus tests.

The most useful diagnostic outcome was the start of the SingleStep framework. It already showed that Klaus pass/fail and cycle-count agreement are not enough to fully characterize a core. The `floooh/chips` SingleStep results are especially valuable because they show a strong external tick-based core can have perfect cycle counts while still differing in instruction state or bus trace for selected opcodes.

The main caution from this session is that some cores look like plausible 65C02 candidates because they contain partial 65C02-like instructions, but Klaus extended and SingleStep-style model testing are necessary to determine whether they really cover WDC/Rockwell/ST behavior.

