# qe6502-benchmark — Session 5 Core Integration Report

This is **not** a continuation handoff. It is an engineering report for the final project notes.

The goal of this report is to summarize what happened during **Session 5** while integrating and evaluating the 65C02 SingleStepTests matrix, with emphasis on:

- how easy each core was to integrate;
- how well the core API fits realistic emulator integration;
- how portable/flexible each core appears;
- what had to be patched or worked around;
- how well each core fits the benchmark methodology;
- which results should be interpreted with caveats.

Session 5 primarily covered the 65C02 SingleStepTests phase for:

- `vrEmu6502`
- `fake65c02`
- `sflib65c02`
- `qe6502`

It also clarified reporting methodology, 65C02 opcode classification, bus-trace support policy, and `all_65c02_tests` exit-code semantics.

---

## 1. Scope of Session 5

The repository already had the NMOS SingleStep matrix in place before this session. Session 5 moved the project into the 65C02 SingleStep phase.

The work done in this session corresponds to these versions:

| Version | Main purpose |
| --- | --- |
| `v53` | Diagnostic `vrEmu6502 CPU_65C02` runner against WDC/Rockwell/Synertek 65C02 corpora. |
| `v54` | Official `vrEmu6502` 65C02 SingleStep integration in `all_65c02_tests`. |
| `v55` | Add `fake65c02` and `sflib65c02` 65C02 SingleStep runners. |
| `v56` | Add `qe6502` WDC/Rockwell/Synertek-ST 65C02 SingleStep runners. |
| `v57` | Make `all_65c02_tests` return success when candidate cores fail semantically; only harness errors should fail the aggregate process. |

No broad cleanup/refactor of existing NMOS tests was done. That was intentional. The existing NMOS tests were already working and trusted, so Session 5 reused what was safe and copied/adapted where the 65C02 logic needed to differ.

---

## 2. General methodology conclusions from this session

### 2.1 Do not over-refactor working NMOS tests

At first, a common runner abstraction looked tempting because many SingleStep runners share similar logic: initialize memory, initialize CPU state, execute one instruction, compare final state, count cycles, and optionally compare bus trace.

However, the decision was to avoid disturbing the already-working NMOS tests. This was the right call. The 65C02 phase introduced differences that make some copy/paste acceptable:

- different opcode categories;
- different CPU model/corpus mapping;
- WDC/Rockwell/Synertek differences;
- special handling for WDC `WAI`/`STP`;
- different levels of bus visibility per core;
- different API quirks per core.

The practical approach was:

- reuse corpus loading, JSON parsing, in-memory corpus handling, statistics structures, and log writing where already available;
- copy/adapt core-specific execution and comparison code where that kept the implementation clearer and safer;
- do not rewrite existing NMOS runners just for elegance.

### 2.2 65C02 reporting uses exactly two opcode categories

For NMOS the existing split is:

```text
legal/documented
illegal/undocumented
```

That split is not appropriate for 65C02. The agreed 65C02 reporting categories are exactly:

```text
functional
nop
```

Rules:

- `nop` includes the official `$EA` NOP and all defined 65C02 NOP variants, including multi-byte NOPs.
- `functional` includes every non-NOP instruction, including model-specific real instructions.
- There is no separate `model-specific` category.
- Rockwell `RMB/SMB/BBR/BBS`, WDC `WAI/STP`, and any Synertek/ST real operations belong to `functional`.

The reason for the split is interpretability. A failure in `ADC`, `RTI`, `BBR`, or `STZ` is more important than a mismatch in a defined NOP's length or cycle behavior. NOP failures are still real failures, but they should be visible separately.

### 2.3 Zero-case JSON files are valid

Some 65C02 SingleStepTests JSON files may contain zero test cases, or a given corpus may have missing/empty entries for some opcode slots.

This is valid and must not be treated as a harness failure.

The correct behavior is:

- do not fail the loader;
- do not mark the opcode as failed just because it has zero cases;
- do not let zero-case files distort the failed-opcode counters;
- continue normally.

This was important during the 65C02 work because not every corpus has usable cases for every opcode slot.

### 2.4 Bus-trace support must be earned, not inferred

A core only supports the SingleStep `bus-trace` category if the harness can observe a true cycle-by-cycle bus stream.

It is not enough for the core to expose memory callbacks during semantic instruction execution. If the core executes the whole instruction internally and later just counts down cycles, the benchmark can test final state and cycle count, but not real bus trace.

During this session:

| Core | 65C02 bus-trace status | Reason |
| --- | --- | --- |
| `qe6502` | supported | Real tick/bus model with host-visible bus operations. |
| `vrEmu6502` | unsupported | `Tick()` does not expose a true per-cycle bus stream. |
| `fake65c02` | unsupported | Instruction-level stepping/cycle accounting only; no full bus trace. |
| `sflib65c02` | unsupported | Memory integration is not a complete host-visible bus path. |

This distinction is important for final evaluation. A core with `bus-trace: unsupported` is not necessarily bad, but it is less useful for cycle-accurate or bus-accurate host integration.

### 2.5 Host-visible memory access matters

A major lesson across the benchmark is that realistic CPU integration requires host-visible memory operations.

For a core to fit well into real emulator integration, the host should be able to observe or control:

- opcode fetches;
- operand reads;
- data reads/writes;
- stack reads/writes;
- vector reads;
- dummy/idle bus accesses when relevant;
- memory-mapped I/O side effects;
- bank switching and device interactions.

A core that hides memory internally may look faster, but it is not paying the same integration cost as a core that routes bus operations through host callbacks, pins, or virtual bus methods. Such a core can still be useful for functional testing, but its speed and integration characteristics are not directly comparable to a fully host-driven bus model.

This is especially relevant to the `sflib6502`/`sflib65c02` family, discussed below.

---

## 3. Core-by-core integration report

## 3.1 `qe6502`

### Work done in Session 5

Session 5 added `qe6502` 65C02 SingleStep coverage for all three 65C02 models:

```text
qe6502 WDC 65C02        -> qe6502_model_wdc -> WDC corpus
qe6502 Rockwell 65C02   -> qe6502_model_rw  -> Rockwell corpus
qe6502 Synertek/ST      -> qe6502_model_st  -> Synertek corpus
```

The runners were added to `all_65c02_tests --singlestep-only`.

`qe6502` supports all three SingleStep validation levels:

```text
instruction
cycle-count
bus-trace
```

### Integration difficulty

`qe6502` was the easiest and cleanest core to integrate for this phase, mainly because it is designed around the benchmark's desired model:

- explicit CPU model selection;
- host-controlled memory/bus behavior;
- tick-level execution;
- observable bus operations;
- controllable CPU state;
- clear separation between CPU core and host memory.

The harness could treat it as a reference-quality integration rather than a workaround-heavy external core.

### Special issue: WDC `WAI`/`STP`

The only notable special handling was for WDC `WAI` and `STP`, especially opcodes `CB` and `DB`.

These instructions do not behave like ordinary instructions that naturally proceed to a normal next opcode fetch. The runner therefore needed to stop based on the expected cycle count rather than waiting for a normal instruction boundary pattern.

This was a small harness-side adjustment, not a vendored-core semantic patch.

### Patches required

No vendored `qe6502` semantic patch was needed in Session 5. The work was harness integration.

### Test fit

`qe6502` fits the tests extremely well:

- exact instruction-level state comparison;
- exact cycle-count comparison;
- exact bus-trace comparison;
- model-specific WDC/Rockwell/Synertek behavior represented directly by explicit CPU models.

Full external-corpus results observed after integration:

| Model | Instruction | Cycle-count | Bus-trace |
| --- | ---: | ---: | ---: |
| WDC | 0/254 | 0/254 | 0/254 |
| Rockwell | 0/256 | 0/256 | 0/256 |
| Synertek/ST | 0/256 | 0/256 | 0/256 |

### Opinion

`qe6502` is the best fit for this benchmark and for realistic host integration. It exposes the right control points, supports exact bus-level testing, and models the important 65C02 variants explicitly.

For final scoring, I would rate it very highly on:

- integration ease;
- portability;
- host flexibility;
- observability;
- model clarity;
- testability;
- cycle/bus accuracy.

The `WAI/STP` handling is not a negative; it is a real CPU behavior that the harness simply had to respect.

---

## 3.2 `vrEmu6502`

### Work done in Session 5

Session 5 first added a diagnostic runner for the unknown `CPU_65C02` mode, then promoted the chosen mapping into the official `all_65c02_tests` runner.

The final 65C02 mapping is:

```text
vrEmu6502 WDC 65C02        -> CPU_W65C02 -> WDC corpus
vrEmu6502 Rockwell 65C02   -> CPU_R65C02 -> Rockwell corpus
vrEmu6502 Synertek/ST      -> CPU_65C02  -> Synertek corpus
```

The important clarification was that `CPU_W65C02` and `CPU_R65C02` already have direct meanings. They should not be treated as candidates for Synertek/ST best-fit.

Only `CPU_65C02` was unknown. Therefore it was tested against all three corpora:

```text
CPU_65C02 vs WDC corpus
CPU_65C02 vs Rockwell corpus
CPU_65C02 vs Synertek corpus
```

The full external-corpus diagnostic results showed that `CPU_65C02` best fits Synertek/ST by functional instruction behavior.

### Integration difficulty

`vrEmu6502` was moderately easy to integrate.

Positive factors:

- small C-style API;
- explicit runtime model selector;
- supports NMOS, WDC 65C02, Rockwell 65C02, and standard CMOS 65C02 modes;
- already passed Klaus WDC/Rockwell standard and extended tests;
- straightforward to instantiate and run for functional/cycle-level testing.

Difficulties and caveats:

- it does not expose a complete public setter API for all SingleStep initial-state fields;
- the harness has to rely on known internal layout/access strategy for some state setup;
- no convenient callback user-data path, so wrappers may need static/thread-local context patterns;
- memory callbacks do not represent a true cycle-by-cycle bus trace.

### Bus-trace limitation

`vrEmu6502` does not support bus-trace in this benchmark.

The important issue is timing granularity. Its tick path executes the instruction semantically and then counts remaining cycles. That is useful for instruction result and cycle count, but it does not expose the real per-cycle bus stream required by SingleStep bus-trace comparison.

Therefore:

```text
instruction: supported
cycle-count: supported
bus-trace: unsupported
```

This is not a correctness failure by itself. It is an API/timing visibility limitation.

### Patches required

No vendored semantic patch was needed for `vrEmu6502` in Session 5.

The harness work included:

- adding the diagnostic `CPU_65C02` runner;
- adding official WDC/Rockwell/Synertek-ST 65C02 runners;
- printing the exact CPU init model in the output;
- mapping `CPU_65C02` to Synertek/ST based on best-fit functional results;
- adding detail logs for each corpus/model combination.

### Test fit

`vrEmu6502` fits instruction-level and cycle-count testing reasonably well. It does not fit bus-trace testing.

Full external-corpus results observed:

| Mapping | Instruction failed opcodes | Cycle-count failed opcodes | Bus-trace |
| --- | ---: | ---: | --- |
| `CPU_W65C02` vs WDC | 2/254 | 18/254 | unsupported |
| `CPU_R65C02` vs Rockwell | 4/256 | 20/256 | unsupported |
| `CPU_65C02` vs Synertek/ST | 35/256 | 36/256 | unsupported |

For `CPU_65C02` best-fit, the decisive comparison was functional instruction failures:

| `CPU_65C02` diagnostic corpus | Functional instruction failures |
| --- | ---: |
| WDC | 34/209 |
| Rockwell | 36/211 |
| Synertek/ST | 3/179 |

Synertek/ST also has many NOP mismatches:

```text
nop instruction: 32/77 failed
```

But by the agreed interpretation, functional failures dominate best-fit selection. The NOP failures are still recorded, but they are less important than failures in real functional opcodes.

### Opinion

`vrEmu6502` is a useful and fairly portable candidate, especially because it offers multiple 65C02 runtime modes. It is much stronger than the partial 65C02 candidates and is a good comparison core for WDC/Rockwell/Synertek-ST functional behavior.

Its main weakness is not basic functionality but observability:

- no full public state setup API;
- no true bus-trace capability;
- limited callback context ergonomics.

For final scoring, I would rate it:

- good for portability;
- good for functional integration;
- moderate for API ergonomics;
- weak for bus-level/cycle-visible host integration;
- strong enough to be a serious 65C02 comparison candidate, but not a bus-accurate reference.

---

## 3.3 `fake65c02`

### Work done in Session 5

Session 5 added `fake65c02` SingleStep runners against all three 65C02 corpora:

```text
fake65c02 vs WDC corpus
fake65c02 vs Rockwell corpus
fake65c02 vs Synertek/ST corpus
```

`fake65c02` has no WDC/Rockwell/Synertek runtime model selector. The same implementation is run against each corpus.

The output explicitly identifies the CPU init model as:

```text
cpu init model: fake65c02
```

### Integration difficulty

`fake65c02` was relatively easy to integrate mechanically. It is a simple core with a straightforward style. However, simplicity comes with limitations.

Positive factors:

- small codebase;
- easy to wrap;
- easy to run instruction-level tests;
- simple enough to diagnose when things go wrong.

Limitations:

- no true bus-trace support;
- no 65C02 model variants;
- known extended 65C02 correctness problems;
- global/simple callback style is less flexible than a fully host-driven bus object.

### Correctness and opcode-table caveats

Before this session, `fake65c02` was already known to pass smoke tests but fail Klaus 65C02 extended.

The earlier diagnostic conclusion was that failures are tied to opcode table and operand-length semantics, especially NOP variants. A representative issue is that some 65C02 NOP variants should consume operands, but the core may treat them as shorter implied NOPs, causing the following byte to be executed as an opcode.

This is exactly the kind of issue that SingleStepTests expose well.

### Patches required

No vendored `fake65c02` patch was applied in Session 5.

That was intentional. Fixing the observed problems would mean maintaining or correcting the vendored 65C02 opcode/NOP table, which is a semantic change to the external core, not a small harness fix.

### Bus-trace limitation

`fake65c02` does not expose a true cycle-by-cycle bus stream suitable for SingleStep bus-trace validation.

Therefore:

```text
instruction: supported
cycle-count: supported
bus-trace: unsupported
```

### Test fit

Full external-corpus results observed:

| Corpus | Instruction failed opcodes | Cycle-count failed opcodes | Bus-trace |
| --- | ---: | ---: | --- |
| WDC | 25/254 | 59/254 | unsupported |
| Rockwell | 27/256 | 61/256 | unsupported |
| Synertek/ST | 52/256 | 77/256 | unsupported |

`fake65c02` is not exact, but it is not useless. It is clearly better than `sflib65c02` in functional instruction behavior.

### Opinion

`fake65c02` is easy to integrate but not a high-confidence 65C02 implementation. Its value in the benchmark is as a simple partial/comparison core rather than a reference candidate.

I would rate it:

- good for integration ease;
- acceptable for basic portability;
- weak to moderate for correctness;
- weak for bus/cycle observability;
- useful as a benchmark comparison point because it exposes common failure modes such as NOP length and opcode-table issues.

I would not patch it casually. If the final project needs `fake65c02` to become more correct, that should be treated as maintaining a fork of the core's opcode table, not as a benchmark-harness issue.

---

## 3.4 `sflib65c02`

### Work done in Session 5

Session 5 added `sflib65c02` SingleStep runners against all three 65C02 corpora:

```text
sflib65c02 vs WDC corpus
sflib65c02 vs Rockwell corpus
sflib65c02 vs Synertek/ST corpus
```

Like `fake65c02`, `sflib65c02` has no WDC/Rockwell/Synertek runtime model selector. The same implementation is run against every corpus.

The output explicitly identifies the CPU init model as:

```text
cpu init model: sflib65c02
```

### Integration difficulty

`sflib65c02` was more problematic than `fake65c02`.

Mechanical integration is possible, but the API does not fit the benchmark's ideal integration model. The core does not expose the same full host-controlled memory path as bus-oriented cores.

This makes it harder to treat the benchmark as fully realistic for this library.

### Host memory control caveat

The important caveat for the `sflib6502`/`sflib65c02` family is memory visibility.

The benchmark wants CPU memory operations to pass through a host-controlled path so the host can model:

- RAM;
- ROM;
- stack behavior;
- vectors;
- memory-mapped I/O;
- bank switching;
- side effects;
- possibly dummy accesses.

`sflib6502`/`sflib65c02` expose some memory access hooks, but they do not provide the same complete external bus-control model as cores like `qe6502`, `floooh/chips`, `O2`, or `Peddle`.

The practical result is that some fetch/stack/internal memory behavior is less transparent from the host/client point of view. This makes SingleStep instruction-level testing still useful, but bus-trace is unsupported and speed comparisons are compromised.

In other words, if a core hides relevant memory operations internally, its benchmark speed may partly reflect the fact that it is not doing the same host-callback work as a core that routes all relevant accesses through the host.

This does not make `sflib65c02` invalid as a comparison point, but it must be interpreted carefully.

### Correctness caveats

Before this session, `sflib65c02` was already known to pass smoke tests but fail Klaus 65C02 extended.

The earlier diagnostic conclusion was that it lacks major Rockwell-style bit opcode families:

```text
RMB0..RMB7
SMB0..SMB7
BBR0..BBR7
BBS0..BBS7
```

That is not a small harness bug. It is a real opcode coverage gap.

### Patches required

No vendored `sflib65c02` patch was applied in Session 5.

That was intentional. Adding missing opcode families or correcting deep behavior would be a substantive change to the external core.

### Bus-trace limitation

`sflib65c02` does not support bus-trace in this benchmark.

Therefore:

```text
instruction: supported
cycle-count: supported where observable/derivable
bus-trace: unsupported
```

### Test fit

Full external-corpus results observed:

| Corpus | Instruction failed opcodes | Cycle-count failed opcodes | Bus-trace |
| --- | ---: | ---: | --- |
| WDC | 131/254 | 133/254 | unsupported |
| Rockwell | 131/256 | 134/256 | unsupported |
| Synertek/ST | 130/256 | 134/256 | unsupported |

These results are weak compared with the other 65C02 candidates.

### Opinion

`sflib65c02` is useful mainly as a negative or weak comparison case. It demonstrates what happens when a library has some 65C02-like functionality but lacks important extended opcode coverage and does not expose the kind of bus/memory control that the benchmark considers ideal.

I would rate it:

- moderate for basic integration effort;
- weak for host memory flexibility;
- weak for bus-level testability;
- weak for 65C02 completeness;
- useful as a comparison point, but not a strong integration candidate.

For final conclusions, I would not exclude it just because it fails. The failures are valuable evidence. But I would clearly mark it as a weak/partial 65C02 candidate.

---

## 3.5 Cross-note: `sflib6502` from the same family

Although Session 5's new implementation work focused on 65C02, the same integration caveat applies to `sflib6502`.

Earlier NMOS work showed:

- `sflib6502` can pass Klaus NMOS;
- SingleStep exposes many exactness problems;
- bus-trace is unsupported;
- undefined opcode diagnostic spam had to be suppressed in the harness;
- speed numbers should be interpreted with caution because the memory path is not fully host-controlled.

The important point for final evaluation is that `sflib6502`/`sflib65c02` are not apples-to-apples with cores that call into the host for every relevant bus access.

A CPU core that embeds or hides parts of memory behavior is easier to run in isolation, but less flexible as an embeddable emulator component.

---

## 4. Aggregate runner and exit-code interpretation

Session 5 changed `all_65c02_tests` semantics.

Before `v57`, the aggregate runner could return a nonzero exit code because some candidate cores failed their tests. This was misleading for the benchmark's purpose.

The benchmark is supposed to measure candidates. A candidate failing a Klaus or SingleStep test is a valid result, not necessarily a harness failure.

The final behavior is:

```text
candidate/core mismatches -> reported in summary, but process exits 0
harness/internal errors   -> process exits 1
```

Examples of valid candidate failures:

- `fake65c02` failing Klaus extended;
- `sflib65c02` failing Klaus extended;
- SingleStep mismatch counts for any external core.

Examples of real harness failures:

- invalid explicit corpus path;
- bad JSON parsing;
- missing required test data in a non-zero-case file;
- crash;
- internal runner error.

This is important for final automation. A completed aggregate test run with weak cores should still be considered a successful benchmark run.

---

## 5. Full 65C02 SingleStep results observed after Session 5

The following full external-corpus results were supplied after the v56/v57 work. They should be treated as observed benchmark data, not as compile-time assumptions.

| Core/model | Instruction failed opcodes | Cycle-count failed opcodes | Bus-trace |
| --- | ---: | ---: | --- |
| `qe6502` WDC | 0/254 | 0/254 | 0/254 |
| `qe6502` Rockwell | 0/256 | 0/256 | 0/256 |
| `qe6502` Synertek/ST | 0/256 | 0/256 | 0/256 |
| `vrEmu6502` WDC / `CPU_W65C02` | 2/254 | 18/254 | unsupported |
| `vrEmu6502` Rockwell / `CPU_R65C02` | 4/256 | 20/256 | unsupported |
| `vrEmu6502` Synertek/ST / `CPU_65C02` | 35/256 | 36/256 | unsupported |
| `fake65c02` WDC | 25/254 | 59/254 | unsupported |
| `fake65c02` Rockwell | 27/256 | 61/256 | unsupported |
| `fake65c02` Synertek/ST | 52/256 | 77/256 | unsupported |
| `sflib65c02` WDC | 131/254 | 133/254 | unsupported |
| `sflib65c02` Rockwell | 131/256 | 134/256 | unsupported |
| `sflib65c02` Synertek/ST | 130/256 | 134/256 | unsupported |

Important interpretation:

- `qe6502` is exact across all three 65C02 models and all validation levels.
- `vrEmu6502` is strong at instruction level for direct WDC/Rockwell modes.
- `vrEmu6502 CPU_65C02` best fits Synertek/ST by functional instruction behavior, despite many NOP mismatches.
- `fake65c02` is imperfect but meaningfully better than `sflib65c02` for functional behavior.
- `sflib65c02` is weak and incomplete, but still useful as a comparison point.

---

## 6. What was patched vs. not patched

### Patched in Session 5

Only benchmark harness / aggregate logic was patched:

- added diagnostic `vrEmu6502 CPU_65C02` SingleStep runner;
- added official `vrEmu6502` WDC/Rockwell/Synertek-ST SingleStep runners;
- added `fake65c02` SingleStep runners;
- added `sflib65c02` SingleStep runners;
- added `qe6502` WDC/Rockwell/Synertek-ST SingleStep runners;
- added/used `--singlestep-only` for 65C02 aggregate testing;
- printed explicit CPU init model in output;
- adjusted aggregate exit code semantics for `all_65c02_tests`.

### Not patched in Session 5

No vendored CPU core semantics were patched in this session.

Specifically, no attempt was made to fix:

- `fake65c02` NOP/opcode table behavior;
- `sflib65c02` missing Rockwell bit opcode families;
- `sflib65c02` memory integration limitations;
- `vrEmu6502` public setter/callback-userdata limitations;
- bus-trace support for cores that do not expose true per-cycle bus activity.

This was deliberate. The benchmark should measure the cores as candidates. Semantic patches to vendored cores should only happen with explicit approval and should be considered core maintenance, not harness integration.

---

## 7. Final Session 5 opinions for candidate scoring

### Best integration fit

`qe6502` is clearly the best fit. It exposes the right API shape, supports exact bus-level testing, and models all relevant 65C02 variants directly.

### Strong external 65C02 candidate

`vrEmu6502` is the strongest external 65C02 candidate worked on in this session. Its functional results are good, and its model selection is useful. Its main downside is lack of bus-trace visibility and some API ergonomics limitations.

### Simple but incomplete candidate

`fake65c02` is easy to integrate and useful as a comparison point, but its 65C02 exactness is limited, especially around opcode/NOP semantics.

### Weak/compromised candidate

`sflib65c02` is weak as a 65C02 candidate and compromised as a realistic integration target because of incomplete opcode coverage and limited host-visible memory control. It should remain in the results as evidence, but not be considered a strong candidate.

---

## 8. Suggested scoring dimensions for the final report

Based on Session 5, the final evaluation should not score cores only by pass/fail counts. It should also score integration quality.

Suggested dimensions:

| Dimension | What to consider |
| --- | --- |
| Functional correctness | Instruction-level SingleStep and Klaus behavior. |
| Cycle accuracy | Cycle-count SingleStep results. |
| Bus visibility | Whether true bus-trace can be validated. |
| Host memory flexibility | Whether fetch/read/write/stack/vector/I/O can pass through host-controlled logic. |
| Model coverage | WDC/Rockwell/Synertek/ST model support. |
| API ergonomics | State setters/getters, callback user data, clean reset/init path. |
| Portability | C/C++ simplicity, build friction, platform concerns. |
| Patch burden | Whether vendored semantic fixes were needed. |
| Interpretability | Whether failures are easy to diagnose from logs/output. |
| Benchmark fairness | Whether speed tests exercise comparable host integration work. |

This is important because a fast but closed-memory core may score well on speed while being less suitable for real emulator integration.

---

## 9. Session 5 bottom line

The 65C02 SingleStep matrix became usable and interpretable during this session.

The most important conclusions are:

1. `qe6502` is exact and integrates cleanly.
2. `vrEmu6502` is a strong external functional candidate but lacks bus-trace support.
3. `vrEmu6502 CPU_65C02` should be treated as Synertek/ST best-fit.
4. `fake65c02` is easy to integrate but incomplete, especially around 65C02 opcode/NOP behavior.
5. `sflib65c02` is weak and has both correctness and integration-model caveats.
6. Cores that cannot expose host-visible memory operations should be interpreted carefully, especially for speed and bus-level conclusions.
7. Candidate test failures should be benchmark results, not aggregate harness failures.
8. No vendored semantic patches were applied in Session 5; the results reflect the cores as integrated candidates.

