# qe6502-benchmark — Session 4 core integration report

This report is **not** a normal handover for continuing development.  It is an assessment report from **ChatGPT session 4** describing how the integration work felt in practice: how each core fit the benchmark project, how easy it was to port and test, what API limitations appeared, what had to be patched or worked around, and which lessons matter when producing the final project conclusions.

The purpose is to provide raw material for a later final summary that compares emulator cores on integration suitability, portability, flexibility, correctness visibility, performance benchmarking usefulness, and test compatibility.

The latest full-tree artifact produced in this session is:

```text
qe6502-benchmark_v51.zip
```

The latest major state after this session:

- NMOS SingleStep coverage exists for all current NMOS candidates.
- `all_6502_tests --singlestep-only` can run the SingleStep matrix without running smoke/Klaus/performance tests first.
- SingleStep reporting is informational for external candidates: mismatches are recorded and logged, but do not fail the aggregate runner.
- Patch artifacts from v42 onward were generated to apply with `patch -p2`; this must remain the expected format for future `qe6502-benchmark` patch files.

---

## Scope of session 4 work

Session 4 covered the second half of the NMOS SingleStep phase and some supporting cleanup.  The main artifacts produced were v40 through v51.

High-level work done:

1. Added `vrEmu6502` NMOS SingleStep.
2. Added `all_6502_tests` CLI flags to skip smoke/Klaus/performance and jump directly to SingleStep.
3. Added `O2` NMOS SingleStep.
4. Added `Peddle` NMOS SingleStep.
5. Added `fake6502` NMOS SingleStep.
6. Added `gianlucag/mos6502` NMOS SingleStep.
7. Added `sflib6502` NMOS SingleStep.
8. Corrected `sflib6502` policy so illegal/undocumented opcode slots are executed blindly instead of skipped.
9. Suppressed `sflib6502` stderr spam without skipping opcodes.
10. Optimized that stderr suppression so it is run-wide rather than per test case.
11. Added `olcNES` NMOS SingleStep.
12. Added `chris-pikul/mos6502` NMOS SingleStep.
13. Produced a normal development handoff through v51.
14. Produced this separate session-4 integration assessment report.

This session also relied on earlier work from the same project line: `vrEmu6502`, `O2`, and `Peddle` had already been integrated as Klaus/smoke benchmark candidates, and common SingleStep infrastructure already existed for `qe6502` and `floooh/chips`.

---

## Workflow and artifact expectations relevant to this report

This section is included only where it affects the interpretation of the integration work.

### Versioning

The project uses an artifact-based workflow.  Each accepted development step produces:

```text
qe6502-benchmark_vXX_<description>.patch
qe6502-benchmark_vXX.zip
```

The next development step must use the latest accepted full-tree zip as its base.

During this session, the user explicitly requested that future patch files should apply with:

```bash
patch -p2 < qe6502-benchmark_vXX_<description>.patch
```

The already-created v41 patch was not regenerated, but later patches were made `-p2` compatible.  Future sessions should keep this convention.

### No hidden semantic patching of vendored cores

A recurring project policy is to avoid broad vendored-core semantic rewrites.  Tiny fixes can be accepted if explicitly approved, but benchmark integration should primarily expose the actual behavior of each core rather than reshape the core to pass tests.

In this session, no broad vendored semantic fixes were added.  Most changes were harness/toolbox-side adapters.

### “Run the core blindly” principle

A key correction during this session was clarified by the user while discussing `sflib6502`: the benchmark should run the tested core as it is.  The harness should not pre-filter opcodes just because the benchmark knows a core is not an exact NMOS core.

This means:

- run all opcode cases that belong to the selected corpus/model;
- let the core behave as it behaves;
- compare the resulting state/cycle/trace against the oracle;
- report mismatches honestly;
- do not skip illegal/undocumented opcode slots merely because they are expected to fail.

The only special handling that remains acceptable is for cases where a core jams/hangs in a way that would make the harness impossible to continue, or where post-jam synthetic cycles must not be compared as if they were real SingleStep bus cycles.  Existing KIL/JAM trace truncation for cores that support it is not the same as skipping the opcode.

---

## Common SingleStep testing model used in this session

The SingleStep framework reports three independent accuracy levels:

1. **Instruction-level**
   - final architectural CPU state:
     - `A`, `X`, `Y`, `S/SP`, `P/status`, `PC`;
   - plus final RAM cells listed by the test case.
   - It does **not** check cycle count or bus trace.

2. **Cycle-count**
   - total number of CPU cycles for the instruction.
   - If a core only executes whole instructions but returns a reliable cycle count, this level can still be supported.

3. **Bus-trace**
   - cycle-by-cycle memory/bus operation trace.
   - This requires a real per-cycle model that exposes address, read/write, data, and dummy/idle accesses well enough to compare with SingleStepTests.
   - Whole-instruction cores usually cannot support this honestly.

The headline statistics are **per opcode**, not per case.  One failed case marks that opcode failed at that accuracy level.  This makes the percentages intentionally strict and useful for coverage, but they can look higher than a per-case failure rate.

---

## Important global lesson: Klaus PASS is not SingleStep exactness

A major conclusion from this session is that Klaus PASS should be interpreted as a functional milestone, not proof of cycle-exact or instruction-oracle exact behavior.

The existing Klaus runners generally pass when the test program reaches the success address.  They do not compare every instruction against an oracle and they do not validate bus-trace behavior.

SingleStepTests exposed issues such as:

- status bit policy differences (`B` and unused bit behavior in `P`);
- decimal `ADC/SBC` edge cases;
- missing NMOS `JMP (indirect)` page-wrap bug;
- incorrect cycle tables;
- missing page-cross extra cycles;
- unsupported or different undocumented opcode semantics;
- whole-instruction cores that can count cycles but cannot expose cycle-by-cycle bus accesses.

This is not a contradiction.  It is exactly why both Klaus and SingleStepTests are useful.

---

# Core-by-core integration assessment

The following sections describe the practical integration experience and the resulting suitability impressions.

---

## qe6502

### Role in this session

`qe6502` was already integrated as the project’s main candidate and already had NMOS SingleStep support before the later v40-v51 work.  In this session it served as a reference-quality benchmark participant and as a comparison point for how clean a SingleStep-capable API can be.

### Integration fit

`qe6502` fits the SingleStep model extremely well:

- clean tick/bus API;
- explicit model selection;
- full architectural state control;
- real cycle-by-cycle bus visibility;
- predictable host-driven bus loop;
- accurate enough to pass the NMOS light corpus at instruction, cycle-count, and bus-trace levels.

### Test fit

Current NMOS SingleStep light result:

```text
instruction: 0/256 failed
cycle-count: 0/256 failed
bus-trace:   0/256 failed
```

This is the cleanest result in the matrix.

### Portability and API impression

The API is deliberately shaped for host integration.  It is also suitable for special machine-level adapters such as the future 6510 `$0000/$0001` port layer.  It does not force the benchmark harness to reach into private implementation state.

### Opinion

For the benchmark, `qe6502` is the best balance of correctness visibility, host control, portability, and testability.  It should be reported as a candidate like all other cores, not as the judge.  The oracle is SingleStepTests.  But from an integration perspective, `qe6502` is the most cooperative design in the set.

---

## floooh/chips

### Role in this session

`floooh/chips` already had NMOS SingleStep support before the later candidates were added.  It remained important as one of the few candidates with genuine bus-trace-capable behavior.

### Integration fit

`floooh/chips` has a pin/tick model.  That makes it more complex to drive than a simple “execute instruction” API, but much more valuable for bus-level testing.

The harness can observe per-cycle behavior closely enough to compare with SingleStepTests.  This is the important distinction: it is not merely instruction-level or cycle-count capable; it can participate in full bus-trace comparison.

### Test fit

Current NMOS light result:

```text
instruction: 8/256 failed
cycle-count: 0/256 failed
bus-trace:   7/256 failed
```

The cycle-count result is excellent.  The instruction and bus-trace mismatches are small but real.

### Problems and caveats

The low-level pin API requires care.  It is not as ergonomic as a high-level callback API.  But that same low-level design is what makes full bus-trace testing possible.

KIL/JAM handling needed careful policy: compare real early bus cycles but do not compare synthetic post-jam cycles against SingleStepTests.

### Opinion

`floooh/chips` is one of the most valuable benchmark candidates because it combines reasonable exactness with bus-level observability.  It should be described as a **bus-trace-capable benchmark candidate**, not as an “external comparator” or as secondary to `qe6502`.  In the benchmark report, all cores are candidates; SingleStepTests are the oracle.

---

## vrEmu6502

### Work done

Session 4 added `vrEmu6502` NMOS SingleStep in v40.

Added:

```text
toolbox/vremu6502/src/singlestep.cpp
vremu6502_singlestep_nmos
all_6502_tests integration
singlestep_vremu6502_nmos_detail.tsv
```

### Earlier integration context

`vrEmu6502` had already been integrated as a benchmark candidate before this SingleStep work.  It passed:

- NMOS Klaus;
- WDC 65C02 Klaus standard/extended;
- Rockwell 65C02 Klaus standard/extended.

That makes it highly relevant for later 65C02 SingleStep work.

### Integration fit

The C API is compact and generally easy to use.  It is a practical portable core.  However, the public API is incomplete for SingleStep harness needs: there are getters for registers and a setter for PC, but not a full public setter for all CPU state (`A`, `X`, `Y`, `S`, `P`).

To initialize arbitrary SingleStep cases, the harness needed controlled access to the core’s internal struct layout.  That is not ideal, but it was workable without vendored source changes.

### Cycle and bus model

`vrEmu6502` has cycle-count control but not real bus-trace control.

The important implementation behavior is that `vrEmu6502Tick()` executes the whole instruction when `step == 0`, including memory callbacks, and then later ticks mostly decrement the remaining cycle counter.  This means the core can provide a useful total cycle count, but not a trustworthy cycle-by-cycle bus stream.

Therefore v40 correctly marked:

```text
instruction: supported
cycle-count: supported
bus-trace:   unsupported
```

### Test fit

NMOS light result:

```text
instruction: 52/256 failed
cycle-count: 12/256 failed
bus-trace:   unsupported
```

Legal and illegal breakdown from the run:

```text
instruction legal:   18/151 failed
instruction illegal: 34/105 failed
cycle legal:          0/151 failed
cycle illegal:       12/105 failed
```

### Problems encountered

- No complete public CPU state setter.
- Whole-instruction execution hidden behind a tick-like API can be misleading; it looks cycle-driven but is not bus-cycle driven.
- Full bus-trace support would be dishonest unless the core itself is changed to expose real cycle-by-cycle bus behavior.

### Patches

No vendored `cores/vremu6502` changes were made in this session.

### Opinion

`vrEmu6502` is not a top NMOS exactness core, but it is one of the most important benchmark candidates because it is portable, fast-oriented, and model-rich.  Its 65C02 coverage makes it especially valuable for future work.  It is a strong practical benchmark core, not a bus-level accuracy reference.

---

## O2

### Work done

Session 4 added `O2` NMOS SingleStep in v42.

Added:

```text
toolbox/o2/src/singlestep.cpp
o2_singlestep_nmos
all_6502_tests integration
singlestep_o2_nmos_detail.tsv
```

### Earlier integration context

`O2` had already been integrated as an NMOS candidate before the SingleStep work.  Earlier issues required vendored fixes:

1. Decimal-mode `ADC/SBC` support was missing and had to be added to pass NMOS Klaus.
2. The enum member `OVERFLOW` conflicted with a macOS macro from system headers and was renamed to `OVERFLOW_FLAG`.

These were accepted earlier as small enough fixes.

### Integration fit

`O2` is small and easy to read.  It is one of the more approachable codebases.

For SingleStep it was better than expected because its `cycle()` style API exposed enough per-cycle behavior to attempt all three accuracy levels:

```text
instruction: supported
cycle-count: supported
bus-trace:   supported
```

### Test fit

NMOS light result:

```text
instruction: 66/256 failed
cycle-count: 76/256 failed
bus-trace:   93/256 failed
```

The important nuance is that legal instruction-level was clean in the light corpus:

```text
legal instruction: 0/151 failed
illegal instruction: 66/105 failed
```

This suggests that `O2` is quite decent for documented NMOS instruction semantics after the decimal fix, but weak on undocumented opcode semantics and exact timing/bus behavior.

### Problems encountered

- Required earlier vendored decimal fix.
- Required earlier macOS portability fix for `OVERFLOW` name collision.
- Undocumented opcodes and exact timing are not strong.
- Bus trace can be measured, but many bus-trace mismatches remain.

### Opinion

`O2` is a good compact educational/minimal core and surprisingly usable with SingleStep.  It is not a strong full-NMOS-exact candidate, but it gives useful data because legal instruction behavior is clean in the light corpus.  It is a good example of a small core that can be integrated and tested without heavy adapter complexity once its basic NMOS missing features are fixed.

---

## Peddle

### Work done

Session 4 added `Peddle` NMOS SingleStep in v43.

Added:

```text
toolbox/peddle/src/singlestep.cpp
peddle_singlestep_nmos
all_6502_tests integration
singlestep_peddle_nmos_detail.tsv
```

### Earlier integration context

`Peddle` had already been integrated for smoke and Klaus before this SingleStep work.  It passed NMOS Klaus without vendored source changes.

### Integration fit

`Peddle` has a heavier C++ object model with a virtual bus abstraction.  It is not as minimal as `O2` or `fake6502`, but it is structured and understandable.

It was a pleasant surprise for SingleStep.  It could support all three levels:

```text
instruction: supported
cycle-count: supported
bus-trace:   supported
```

### Test fit

NMOS light result:

```text
instruction: 7/256 failed
cycle-count: 12/256 failed
bus-trace:   19/256 failed
```

Legal breakdown:

```text
instruction legal: 2/151 failed
cycle legal:       0/151 failed
bus legal:         2/151 failed
```

This makes `Peddle` one of the better non-`qe6502` candidates in the NMOS SingleStep matrix.

### Problems encountered

Earlier Klaus benchmark integration had one methodological oddity: Peddle’s internal clock did not advance in the standalone integration path, so cycles were counted manually through repeated `execute()` calls.

For SingleStep, the bus abstraction was usable enough to collect trace data, but not exact enough to pass all bus-trace cases.

### Patches

No vendored `cores/peddle` changes were made.

### Opinion

`Peddle` is one of the most interesting candidates.  It is not the fastest/simple baseline type of core, but it scores well on integration quality and test visibility.  It belongs high in any ranking that values correctness introspection and bus-level testability.

---

## fake6502

### Work done

Session 4 added `fake6502` NMOS SingleStep in v44.

Added:

```text
toolbox/fake6502/src/singlestep.cpp
fake6502_singlestep_nmos
all_6502_tests integration
singlestep_fake6502_nmos_detail.tsv
```

### Integration fit

`fake6502` is simple and easy to integrate, but its style is global/static and callback-based.  This makes it straightforward for functional tests but less clean for isolated repeated SingleStep cases.

### Cycle and bus model

The core executes whole instructions via `step6502()` and provides total cycle count.  It does not expose a reliable per-cycle bus operation stream.

Therefore:

```text
instruction: supported
cycle-count: supported
bus-trace:   unsupported
```

### Test fit

NMOS light result:

```text
instruction: 30/256 failed
cycle-count: 12/256 failed
bus-trace:   unsupported
```

Legal breakdown:

```text
instruction legal: 10/151 failed
cycle legal:        0/151 failed
```

### Problems encountered

- Global callback style is less flexible than context/user-data APIs.
- No reliable bus-trace support.
- It is a useful functional/speed baseline, not a full exactness candidate.

### Patches

No vendored changes were made in this session.

### Opinion

`fake6502` is valuable as a simple, likely fast baseline.  It should not be judged only by exactness.  For benchmark conclusions, it is useful because it represents the “small whole-instruction functional core” category.  It is less useful for bus/cycle-level emulator fidelity analysis.

---

## gianlucag/mos6502

### Work done

Session 4 added `gianlucag/mos6502` NMOS SingleStep in v45.

Added:

```text
toolbox/gianlucag_mos6502/src/singlestep.cpp
gianlucag_mos6502_singlestep_nmos
all_6502_tests integration
singlestep_gianlucag_mos6502_nmos_detail.tsv
```

### Integration fit

This core has a practical API and was reasonably easy to integrate.  It has read/write callbacks and a clean enough execution model for instruction-level and cycle-count testing.

For NMOS SingleStep the integration enabled its optional illegal opcode support using a CMake definition, because the upstream/test style appears to expect that mode for fuller coverage.

### Cycle and bus model

The core can be driven for instruction-level and cycle-count checks, but it does not expose a reliable SingleStepTests-compatible per-cycle bus trace.

Therefore:

```text
instruction: supported
cycle-count: supported
bus-trace:   unsupported
```

### Test fit

NMOS light result:

```text
instruction: 4/256 failed
cycle-count: 12/256 failed
bus-trace:   unsupported
```

Legal breakdown:

```text
instruction legal: 0/151 failed
cycle legal:       0/151 failed
```

This is an excellent result for instruction-level behavior.

### Problems encountered

- Bus trace unsupported.
- Needs the illegal-opcode compile configuration for better NMOS test fit.
- No 65C02 relevance.

### Patches

No vendored source changes were made.

### Opinion

This is one of the cleanest practical functional cores.  It is not bus-level exact, but its instruction/cycle results in the light corpus are strong.  It should rank high for ease of integration and functional correctness, especially among whole-instruction or callback-style cores.

---

## sflib6502

### Work done

Session 4 added and then corrected `sflib6502` NMOS SingleStep across v46-v49.

Added in v46:

```text
toolbox/sflib6502/src/singlestep.cpp
sflib6502_singlestep_nmos
all_6502_tests integration
singlestep_sflib6502_nmos_detail.tsv
```

Corrected in v47:

- Removed the initial harness policy that skipped NMOS undocumented/illegal opcode slots.
- Now all 256 opcode cases are executed blindly against the core.

Fixed in v48/v49:

- Suppressed huge `stderr` spam from undefined opcodes.
- Changed suppression from per-case to run-wide to avoid severe overhead.

### Integration fit

`sflib6502` is small and fast-looking, but methodologically tricky.

The core has an official callback API, but callback/bus coverage is incomplete for full SingleStep bus trace.  It is usable for instruction-level and cycle-count checks, not for full bus trace.

### Cycle and bus model

```text
instruction: supported
cycle-count: supported
bus-trace:   unsupported
```

### Important policy correction

The initial v46 approach pre-marked NMOS undocumented/illegal opcodes as failed without executing them, partly to avoid `undefined instruction XX` spam.

The user correctly pointed out that this is wrong for the benchmark philosophy.  A real program does not ask the test harness whether an opcode belongs to NMOS, 65C02, or a hybrid table.  It runs the opcode and the core does whatever it does.

The corrected policy from v47 onward:

- execute all opcode cases blindly;
- do not skip formerly-illegal slots;
- record mismatches against the selected NMOS corpus;
- suppress noise separately if needed.

This lesson should be carried forward to 65C02 work and any future core.

### stderr problem

`sflib6502` prints messages like:

```text
undefined instruction XX
```

for undefined opcode slots.

The first suppression attempt in v48 redirected `stderr` around every individual `M6502_step(cpu)`.  That stopped visible output but was slow because each case paid for operations like:

```text
dup / fopen / dup2 / fflush / close / fclose
```

v49 moved the suppression to the entire `sflib6502` SingleStep run.  That keeps output clean and removes per-case file-descriptor overhead.

This was done harness-side only.  The vendored core was not changed.

### Test fit

Final v49/v51 NMOS light result:

```text
instruction: 125/256 failed
cycle-count: 134/256 failed
bus-trace:   unsupported
```

Legal breakdown:

```text
instruction legal: 20/151 failed
cycle legal:       49/151 failed
```

### Why legal failures are high despite Klaus PASS

This core was the clearest example of Klaus PASS not implying SingleStep exactness.

Legal instruction failures are concentrated, not random:

- `BRK`, `PLP`, `RTI` status-bit policy;
- decimal-mode `ADC/SBC` exactness;
- `JMP (indirect)` page-wrap bug.

Observed mismatch pattern for legal instruction failures:

```text
94 failed legal instruction cases
93 had P/status mismatch
20 had A mismatch
1 had PC mismatch
```

Specific issues found:

1. **BRK/PLP/RTI status bit policy**
   - `B` and unused status bit behavior differs from the SingleStep oracle.
   - The core appears to keep `B` as part of real `P` state in ways SingleStepTests do not expect.

2. **Decimal ADC/SBC edge cases**
   - The decimal code contains a comment indicating that only carry is valid on NMOS.
   - SingleStepTests are stricter and check exact state after individual decimal cases.

3. **JMP indirect NMOS page-wrap bug**
   - The core reads high byte from `tmp + 1`, whereas real NMOS `JMP ($xxFF)` wraps high-byte read to `$xx00`.

4. **Cycle-count problems**
   - Some opcode table cycle constants differ from NMOS timing.
   - `tickIf(p)` is empty, so page-cross extra cycles are not added.

### Earlier context

`sflib6502` had been made Klaus-pass earlier in the broader project line after decimal carry-related work.  In this session it was not semantically patched further.

### Opinion

`sflib6502` is important for the final report precisely because it is probably a good speed baseline but not an exact NMOS core.  It should not be dismissed as useless.  It represents a very important tradeoff category: small/fast/functional enough for Klaus, but weak on strict SingleStep exactness and timing.

For final conclusions, it is valuable evidence that performance and exactness need separate axes.

---

## olcNES

### Work done

Session 4 added `olcNES` NMOS SingleStep in v50.

Added:

```text
toolbox/olcnes/src/singlestep.cpp
olcNES_singlestep_nmos
all_6502_tests integration
singlestep_olcnes_nmos_detail.tsv
```

### Earlier integration context

`olcNES` had previously required project-side fixes to behave as a full NMOS candidate, including decimal/BRK-related behavior.  It is originally NES-oriented/educational, so treating it as full NMOS required previous adaptation.

### Integration fit

`olcNES` is understandable and popular in style, but it is not designed as a clean standalone CPU library with full external bus control.

### Cycle and bus model

`olc6502::clock()` executes the instruction semantics when its internal cycle counter reaches zero, then later clocks decrement remaining cycles.  This provides a usable total cycle count, but not a reliable per-cycle bus stream.

Therefore:

```text
instruction: supported
cycle-count: supported
bus-trace:   unsupported
```

### Test fit

NMOS light result:

```text
instruction: 88/256 failed
cycle-count: 21/256 failed
bus-trace:   unsupported
```

Legal breakdown:

```text
instruction legal: 1/151 failed
cycle legal:       0/151 failed
```

This means the documented legal instruction behavior is almost clean in the light corpus, but undocumented opcode behavior is not NMOS-exact.

### Problems encountered

- Whole-instruction execution behind a clock-style API can be misleading.
- No reliable full bus trace.
- Educational/NES origin means NMOS benchmarking must be interpreted carefully.

### Patches

No new vendored changes were made in this session.

### Opinion

`olcNES` is useful as an educational/popular comparison point.  It is not a strong integration target for exact hardware-style CPU validation.  It gives good context: a familiar implementation can be functionally close on documented opcodes while still lacking bus-level/test-oracle fidelity.

---

## chris-pikul/mos6502

### Work done

Session 4 added `chris-pikul/mos6502` NMOS SingleStep in v51.

Added:

```text
toolbox/chris_pikul_mos6502/src/singlestep.cpp
chris_pikul_mos6502_singlestep_nmos
all_6502_tests integration
singlestep_chris-pikul_mos6502_nmos_detail.tsv
```

### Integration fit

This core has a heavier C++ bus/device abstraction.  It was integrable, but the adapter work felt heavier than for the smaller C cores.  The abstraction is more elaborate than needed for the benchmark harness and not particularly friendly for SingleStep oracle comparison.

### Cycle and bus model

The core executes instruction semantics when ticked and then counts remaining cycles.  It does not expose a SingleStepTests-compatible per-cycle bus trace.

Therefore:

```text
instruction: supported
cycle-count: supported
bus-trace:   unsupported
```

### stderr suppression

Illegal opcode execution produced messages like:

```text
attempting to execute an illegal instruction...
```

The harness suppresses stderr around the run, similar in spirit to the final `sflib6502` solution.  The opcodes are still executed blindly; only the spam is suppressed.

### Test fit

NMOS light result:

```text
instruction: 89/256 failed
cycle-count: 250/256 failed
bus-trace:   unsupported
```

Legal breakdown:

```text
instruction legal: 2/151 failed
cycle legal:       145/151 failed
```

The instruction-level result for legal opcodes is not terrible, but cycle-count accuracy is extremely poor in the benchmark’s terms.

### Problems encountered

- Heavy abstraction for benchmark use.
- Almost all cycle-count checks fail.
- Bus trace unsupported.
- Stderr noise needed suppression.

### Patches

No vendored source changes were made in this session.

### Opinion

This was the least compelling NMOS candidate from a benchmark integration/result perspective.  It is still useful as a comparison point, especially because it demonstrates that a core can pass Klaus and still be a poor timing candidate.  But it is not a strong recommendation for projects needing easy integration, portability, speed, or exactness.

---

# Supporting infrastructure work from session 4

## all_6502_tests skip flags

### Work done

v41 added CLI flags to `all_6502_tests` so developers can jump directly to SingleStep:

```bash
./all_6502_tests --singlestep-only
```

Other related flags:

```text
--skip-klaus
--skip-smoke
--skip-singlestep
--skip-performance
--no-klaus
--no-smoke
--no-singlestep
--help
```

### Why this matters

As the SingleStep matrix grew, running all smoke/Klaus/performance tests first became inconvenient and slow.  The shortcut made iteration much easier.

This should remain part of the development workflow, especially when using larger external SingleStep corpora.

---

## Detail logs

SingleStep detail logs already existed before session 4, and the new runners each write their own log:

```text
singlestep_vremu6502_nmos_detail.tsv
singlestep_o2_nmos_detail.tsv
singlestep_peddle_nmos_detail.tsv
singlestep_fake6502_nmos_detail.tsv
singlestep_gianlucag_mos6502_nmos_detail.tsv
singlestep_sflib6502_nmos_detail.tsv
singlestep_olcnes_nmos_detail.tsv
singlestep_chris-pikul_mos6502_nmos_detail.tsv
```

The logs are useful, but more diagnostic columns would help future analysis.  Useful future additions:

- first mismatching register/state field;
- first expected/got `P` value;
- first expected/got `PC` value;
- first expected/got cycle count;
- separate flags for `P`-only mismatch, RAM mismatch, register mismatch;
- optional normalized/relaxed status-bit analysis for diagnostics only, without changing pass/fail.

Do not make the default test less strict.  Add diagnostics instead.

---

# Cross-core observations

## Categories that emerged

A single ranking is misleading because different cores optimize for different things.  The final report should probably classify cores across multiple axes:

1. **Instruction correctness**
2. **Cycle-count accuracy**
3. **Bus-trace capability**
4. **Performance / throughput**
5. **Ease of integration**
6. **API flexibility**
7. **Portability / build friction**
8. **Model coverage**
9. **Need for vendored patches**

### Bus-trace-capable candidates

These candidates can participate in full bus-trace comparison, at least to a meaningful degree:

```text
qe6502
floooh/chips
O2
Peddle
```

Among them, `qe6502`, `floooh/chips`, and `Peddle` are the most interesting bus/cycle modeling candidates.  `O2` can be traced but has many undocumented/timing mismatches.

### Cycle-count capable but not bus-trace capable

These candidates can be tested for instruction and total cycle count, but should not be presented as full bus-trace candidates:

```text
vrEmu6502
fake6502
gianlucag/mos6502
sflib6502
olcNES
chris-pikul/mos6502
```

Many of these execute whole instructions and then return or count total cycles.  That is not enough for SingleStep bus trace.

### Functional/speed baseline candidates

Some cores are especially useful as performance baselines even when not exact:

```text
vrEmu6502
sflib6502
fake6502
O2
gianlucag/mos6502
```

The final report should not over-penalize these for lacking bus trace.  A benchmark project should explicitly show the tradeoff: speed versus fidelity.

---

## Corrected terminology

During discussion, I used the phrase “external bus-level comparator.”  The user corrected the framing: for the benchmark project, **all cores are benchmark candidates**, including `qe6502`.  The report should not imply a “our core vs external cores” hierarchy.

Better terminology:

```text
bus-trace-capable benchmark candidate
cycle-count-capable benchmark candidate
instruction-level benchmark candidate
functional/speed baseline candidate
```

SingleStepTests are the oracle, not `qe6502`.

---

## Speed should be weighted explicitly

An earlier informal ranking over-emphasized exactness.  The user correctly noted that speed is not secondary in this benchmark project.

The final evaluation should not use a single correctness-only ranking.  It should show multiple dimensions.  A fast but less exact core such as `sflib6502` or `fake6502` can be valuable because it defines an important throughput/fidelity tradeoff.

Suggested final-report framing:

```text
Core              Speed  Instruction  Cycle-count  Bus-trace  API fit  Portability
qe6502            ...    ...          ...          ...        ...      ...
floooh/chips      ...    ...          ...          ...        ...      ...
...
```

Then discuss categories rather than only first/last place.

---

# Session 4 rough suitability impressions

These are qualitative notes, not final objective scores.

## Most integration-friendly / test-friendly

```text
qe6502
gianlucag/mos6502
fake6502
O2
```

`qe6502` is the most complete from a host-control perspective.  `gianlucag` and `fake6502` are straightforward for functional/cycle tests.  `O2` is small and readable, though it needed earlier patches.

## Best bus/cycle visibility

```text
qe6502
floooh/chips
Peddle
O2
```

`floooh/chips` and `Peddle` are particularly useful because they provide meaningful bus-level behavior among non-trivial candidates.

## Most valuable for future 65C02 phase

```text
qe6502
vrEmu6502
fake65c02
sflib65c02
```

Only `qe6502` and `vrEmu6502` are currently strong 65C02 Klaus-pass candidates for WDC/Rockwell extended tests.  `fake65c02` and `sflib65c02` should still be included in future SingleStep statistics despite Klaus extended failures, because the purpose is measurement, not pre-filtering.

## Most important speed baselines

```text
vrEmu6502
sflib6502
fake6502
O2
gianlucag/mos6502
```

These should not be dismissed merely because bus-trace is unsupported or SingleStep exactness is imperfect.

## Weakest overall integration/result candidates

```text
chris-pikul/mos6502
olcNES
```

This is not because they are useless.  They provide useful comparison data.  But they are less compelling as flexible, portable, exact, or benchmark-friendly cores.

---

# Important pitfalls for future sessions

## Do not skip opcodes based on expected model category

The `sflib6502` correction is important.  Future harnesses should execute the core as-is.  Do not pre-filter based on legal/illegal category.  Record the mismatch instead.

## Suppress noisy stderr carefully

If a core prints on illegal opcode execution, suppressing stderr can be acceptable, but do it run-wide rather than per case.  Per-case file descriptor redirection is slow.

Preferred pattern:

```text
redirect stderr once
run all cases for that noisy core
restore stderr
```

Avoid vendored changes unless explicitly approved.

## Do not mistake cycle-count capability for bus-trace capability

Several cores have a function named `clock()` or `tick()` but execute the full instruction at the first tick and then decrement an internal cycle counter.  That supports cycle-count testing, not full bus-trace testing.

Check the implementation before declaring bus-trace support.

## Detail logs need richer diagnostics

The current TSV logs are useful but not enough for quick root-cause analysis.  Adding mismatch-field diagnostics would make future reports much easier.

## Klaus pass must be described carefully

Klaus PASS should be described as a functional benchmark result.  Do not imply it proves exact NMOS timing, decimal flags, status bits, undocumented opcode behavior, or bus trace.

---

# Current NMOS SingleStep matrix after session 4

Final NMOS light-corpus results through v51:

```text
qe6502:
  instruction: 0/256
  cycle-count: 0/256
  bus-trace:   0/256

floooh/chips:
  instruction: 8/256
  cycle-count: 0/256
  bus-trace:   7/256

vrEmu6502:
  instruction: 52/256
  cycle-count: 12/256
  bus-trace:   unsupported

O2:
  instruction: 66/256
  cycle-count: 76/256
  bus-trace:   93/256

Peddle:
  instruction: 7/256
  cycle-count: 12/256
  bus-trace:   19/256

fake6502:
  instruction: 30/256
  cycle-count: 12/256
  bus-trace:   unsupported

gianlucag/mos6502:
  instruction: 4/256
  cycle-count: 12/256
  bus-trace:   unsupported

sflib6502:
  instruction: 125/256
  cycle-count: 134/256
  bus-trace:   unsupported

olcNES:
  instruction: 88/256
  cycle-count: 21/256
  bus-trace:   unsupported

chris-pikul/mos6502:
  instruction: 89/256
  cycle-count: 250/256
  bus-trace:   unsupported
```

Remember: these are per-opcode failure counts, not per-case failure counts.

---

# Future TODO: 6510 adapter/tutorial

This is a project TODO that should not be forgotten, but it should only be implemented when explicitly approved.

The idea is to document how to use `qe6502_model_nmos` as a 6510-like CPU by implementing the 6510-specific `$0000/$0001` I/O port behavior in the host/machine layer.

Do **not** add a `qe6502_model_6510` yet.

Preferred framing:

```text
6510 support in qe6502 is implemented as qe6502_model_nmos plus a host-side $0000/$0001 I/O port adapter.  This demonstrates qe6502 portability and the flexibility of its bus/tick API.
```

Important behavior to document:

```text
$0000 = data direction register
$0001 = data port / output latch / input readback
```

Read behavior:

```c
read_0001 = (port_latch & port_ddr) | (external_inputs & ~port_ddr);
```

Write behavior:

```text
write $0000 -> update DDR
write $0001 -> update output latch
then update machine memory map if relevant
```

Example host-side integration sketch:

```c
uint8_t host_read(uint16_t address) {
    if (address == 0x0000) return c6510.ddr;
    if (address == 0x0001) return c6510_read_port(&c6510);
    return memory_map_read(address);
}

void host_write(uint16_t address, uint8_t data) {
    if (address == 0x0000) {
        c6510.ddr = data;
        c64_update_mapping();
        return;
    }
    if (address == 0x0001) {
        c6510.latch = data;
        c64_update_mapping();
        return;
    }
    memory_map_write(address, data);
}
```

The tutorial should compare this with `floooh/chips`, which has built-in 6510 port support in the CPU layer.

Possible document paths:

```text
docs/integration/6510_adapter.md
docs/integration/6510_port_adapter.md
docs/integration/host_side_6510_adapter.md
docs/tutorials/using_qe6502_as_a_6510.md
```

---

# Suggested use of this report

A later final ChatGPT session should combine this report with reports from other sessions and produce an objective multi-axis evaluation.

Do not compress everything into one ranking.  The final assessment should distinguish at least:

- raw performance;
- documented instruction correctness;
- undocumented opcode behavior;
- cycle-count exactness;
- full bus-trace capability;
- portability/build friction;
- API ergonomics;
- ease of state setup for tests;
- need for vendored patches;
- suitability for 65C02 extension;
- usefulness as a benchmark baseline even when not exact.

