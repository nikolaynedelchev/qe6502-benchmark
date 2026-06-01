# qe6502-benchmark Integration Report - Session 1

This report summarizes the practical integration work performed in Session 1 of the `qe6502-benchmark` project. Its purpose is to help a later summarizing session evaluate each 6502 core objectively in terms of integration difficulty, portability, flexibility, correctness-test suitability, benchmark suitability, and general engineering fit.

## Scope of this session

The work in this session started from a new benchmark repository and proceeded iteratively through versioned patches and full-tree ZIP artifacts. The project goal was to vendor selected 6502 CPU cores, build each one independently, add minimal smoke tests, then add Klaus2m5 NMOS standard benchmark runners where practical.

The central design decision was to keep vendored cores under `cores/`, and to keep benchmark integration code in a root-level `toolbox/` tree. Each core receives its own static toolbox library, and standalone executables only link the toolbox and call toolbox functions. This was done to make later unified runners easier without losing the ability to run each core independently.

## Versioning workflow used

The workflow used throughout this session was:

1. The user provides a ZIP of the current project version.
2. The assistant modifies that tree.
3. The assistant produces two artifacts:
   - a patch file for the change;
   - a full ZIP of the complete updated tree.
4. Both sides then treat the new version as the base for the next iteration.

Artifacts were named with the version immediately after the project name, for example:

- `qe6502-benchmark_v09_gianlucag_smoketest.patch`
- `qe6502-benchmark_v09.zip`

The last completed version in this session was `v18`.

## Project shape at the end of Session 1

By the end of the session, the repository had:

- vendored cores under `cores/`;
- per-core static libraries under `toolbox/<core>/`;
- standalone smoke-test executables;
- standalone Klaus benchmark executables for the cores that were attempted;
- common result structs for smoke and Klaus benchmark results;
- a CMake build that builds and links all integrated toolbox libraries and standalone test programs.

The intended direction is still to keep the standalone executables, but make them thin wrappers around toolbox functions. Later, a unified benchmark runner can link all toolboxes and print comparison tables.

## Benchmark policy used in this session

For Klaus2m5 NMOS standard performance tests, the implementation style was intentionally minimal and speed-oriented:

- first run is a cold run and is ignored;
- then 5 measured runs are executed;
- total cycles and total wall time are measured;
- MHz is calculated from cycles / time;
- no instruction counting was added;
- no jammed-CPU checks were added;
- no unnecessary correctness machinery was added in the hot path;
- the goal is to run until the Klaus success address is reached.

The standard/non-extended NMOS Klaus ROM was used, not the extended version.

## Cores integrated

### qe6502

Status at end of session:

- Smoke test: PASS
- Klaus2m5 NMOS standard: PASS
- Toolbox library: present
- Benchmark executable: present

Integration notes:

`qe6502` was the first core integrated. It was vendored and treated like every other participant, rather than being special-cased as the host project. This is a good decision for benchmark reproducibility: every reported run can be tied to the exact vendored source snapshot.

The initial smoke test linked against `qe6502::cpp`, not the C/static API. The smoke test verified that the C++ wrapper could be included, linked, and used from the benchmark repository.

The Klaus benchmark for `qe6502` was straightforward because the project already had a usable bus-tick-oriented design and the required Klaus ROM material was available from the uploaded `klaus2m5` package.

Practical assessment:

- Integration difficulty: low.
- Portability: good.
- Benchmark fit: excellent.
- Cycle-count fit: excellent, because bus ticks are explicit.
- Flexibility: high.

Opinion:

`qe6502` is naturally suited to this benchmark style because its public execution model already exposes the behavior the benchmark cares about: explicit bus ticks and cycle-level execution. It also gives a clean way to measure a correctness workload without inventing artificial counters.

### floooh/chips

Status at end of session:

- Smoke test: PASS
- Klaus2m5 NMOS standard: PASS
- Toolbox library: present
- Benchmark executable: present

Integration notes:

`floooh/chips` was the second core integrated. It was relatively easy to build because `chips` is largely header-style and exposes an `m6502` CPU implementation that can be driven in a bus-tick-like manner.

The integration required defining `CHIPS_IMPL` in exactly one translation unit. The API provided macros/functions that made it possible to read address/data/RW/SYNC state and feed memory data back into the CPU. This made it one of the most convenient non-qe6502 cores to integrate.

The Klaus benchmark produced the same cycle count as `qe6502` for the NMOS standard ROM, which made it a useful reference point. In local runs discussed during the session, `qe6502` was substantially faster on the Klaus workload, but `floooh/chips` also passed cleanly.

Practical assessment:

- Integration difficulty: low.
- Portability: good.
- Benchmark fit: very good.
- Cycle-count fit: very good.
- Flexibility: high for this benchmark's needs.

Opinion:

`floooh/chips` is a strong comparison candidate. It is compact, fast, relatively easy to drive, and exposes enough bus-level behavior to make it useful for cycle-oriented testing. It should remain one of the main baseline competitors.

### gianlucag/mos6502

Status at end of session:

- Smoke test: PASS
- Klaus2m5 NMOS standard: PASS
- Toolbox library: present
- Benchmark executable: present

Integration notes:

This was selected as the easiest new candidate after `qe6502` and `floooh/chips`. Build and link integration was straightforward enough to create a static toolbox library and smoke test quickly.

A Klaus benchmark was added later. It passed the NMOS standard Klaus test. The benchmark used the core's internal cycle counter rather than trying to infer cycles externally.

Performance reported by the assistant's environment was lower than the user's machine, but both runs showed it as a viable, functional benchmark participant. The user reported around 500 MHz on their machine for the Klaus workload.

Practical assessment:

- Integration difficulty: low to moderate.
- Portability: appears good from this integration.
- Benchmark fit: good.
- Cycle-count fit: good, because an internal cycle counter was available.
- Flexibility: acceptable.

Opinion:

This core is a good benchmark participant. It passed Klaus cleanly and was not painful to integrate. It is probably one of the better candidates for future comparative reporting, after `qe6502` and `floooh/chips`.

### fake6502

Status at end of session:

- Smoke test: PASS
- Klaus2m5 NMOS standard: PASS
- Toolbox library: present
- Benchmark executable: present

Integration notes:

`fake6502` required a small C bridge because the benchmark/toolbox code is C++ while the core is C-style. Once bridged, it built and linked cleanly enough.

The Klaus benchmark passed. The code path used the cycle count returned by `step6502()`.

A key correctness detail checked during the session was decimal mode. This vendored copy did not have `NES_CPU` defined, and BCD support was present. That made a Klaus pass expected and the benchmark confirmed it.

There was one artifact issue in `v15`: the patch/local tree initially missed `toolbox/fake6502/src/klaus_benchmark.cpp`, causing a CMake error. The user fixed the tree locally. This was an artifact/sync problem, not a core integration problem.

Practical assessment:

- Integration difficulty: low to moderate.
- Portability: good enough for current purposes.
- Benchmark fit: good.
- Cycle-count fit: good enough, because `step6502()` returns cycles.
- Flexibility: moderate.

Opinion:

`fake6502` is useful as a compact C baseline. It is not as elegant to integrate as `floooh/chips`, but it is small, practical, and passed Klaus when built with decimal mode enabled.

### sflib6502

Status at end of session:

- Smoke test: PASS
- Klaus2m5 NMOS standard: FAIL / investigate later
- Toolbox library: present
- Benchmark executable: present

Integration notes:

`sflib6502` built and linked successfully. A smoke test was added and passed. The Klaus benchmark was added and used `cpu->ticks` after `M6502_step()` to obtain cycle counts.

The Klaus benchmark failed. During investigation, it appeared to reach or get stuck around the decimal-related part of Klaus. The user noted that lack of correct decimal arithmetic would prevent Klaus from passing, and this suspicion appears relevant here.

No deeper fix was attempted in this session. The result was explicitly left for later investigation.

Practical assessment:

- Integration difficulty: moderate.
- Portability: acceptable.
- Benchmark fit: partial.
- Cycle-count fit: acceptable, because tick data is exposed.
- Flexibility: moderate.
- Correctness risk: likely decimal/BCD related, but not proven conclusively in this session.

Opinion:

`sflib6502` is integratable and can be benchmarked mechanically, but its Klaus failure means it should not be treated as a full NMOS correctness peer until investigated. It may still be useful as a performance participant with caveats, or as a case study in missing/incorrect decimal-mode behavior.

### chris_pikul/mos6502

Status at end of session:

- Smoke test: PASS
- Klaus2m5 NMOS standard: FAIL / investigate later
- Toolbox library: present
- Benchmark executable: present

Integration notes:

This core was integrated after `sflib6502`. A static toolbox library and smoke test were added successfully.

The Klaus benchmark was added, using the core's internal cycle counter. The benchmark failed.

One practical issue encountered was debug `std::cout` spam inside `cpu.cpp`. This had to be removed or disabled because it made a Klaus run practically unusable. That is an important integration-quality signal: the core was not immediately benchmark-friendly in its vendored form.

No full root-cause investigation was completed in this session.

Practical assessment:

- Integration difficulty: moderate.
- Portability: acceptable after adjustment.
- Benchmark fit: partial.
- Cycle-count fit: acceptable, because an internal counter exists.
- Flexibility: moderate to low.
- Correctness risk: unknown from this session.

Opinion:

This core can be integrated, but it felt less benchmark-ready than the earlier passing cores. The debug output in core code is a negative integration signal. Since Klaus fails, it needs correctness investigation before it can be used as a clean comparison participant.

### olcNES

Status at end of session:

- Smoke test: PASS
- Klaus2m5 NMOS standard: FAIL / expected
- Toolbox library: present
- Benchmark executable: present

Integration notes:

`olcNES` was integrated last among the smoke-test candidates. The user had warned that the vendored tree might not contain everything. The available files were sufficient for the integration performed.

The CPU is not a standalone library-style core in the same way as `qe6502` or `floooh/chips`; it is tied to the tutorial project's `Bus` structure. The integration therefore had to build and link `Bus.cpp` and `olc6502.cpp` together.

A minimal source fix was needed: `olc6502.h` required `<cstdint>` to compile cleanly in this benchmark project.

The smoke test passed. Klaus NMOS standard failed. This was expected: the `olc6502.h` flag comment marks decimal mode as unused in this implementation, and ADC/SBC do not appear to have a BCD decimal path. Since Klaus tests decimal arithmetic, a clean Klaus pass should not be expected without implementing decimal mode.

Practical assessment:

- Integration difficulty: moderate to high compared with the other small cores.
- Portability: acceptable after minor include fix.
- Benchmark fit: limited.
- Cycle-count fit: likely limited compared with bus-oriented cores.
- Flexibility: low to moderate because the CPU is coupled to the tutorial Bus design.
- Correctness risk: expected decimal/BCD failure.

Opinion:

`olcNES` is useful as a recognizable educational/NES-style baseline, but it should not be treated as a full NMOS 6502 competitor without caveats. The missing decimal mode is expected for an NES-oriented/tutorial implementation, but it prevents Klaus NMOS standard from passing. It is probably better categorized as a partial/NES-style participant rather than a general NMOS 6502 core.

## Summary table

| Core | Smoke | Klaus NMOS standard | Integration difficulty | Notes |
|---|---:|---:|---|---|
| qe6502 | PASS | PASS | Low | Natural fit; explicit bus-tick model. |
| floooh/chips | PASS | PASS | Low | Strong baseline; convenient bus-level API. |
| gianlucag/mos6502 | PASS | PASS | Low/Moderate | Clean participant; internal cycle counter. |
| fake6502 | PASS | PASS | Low/Moderate | Compact C core; decimal enabled in current vendored build. |
| sflib6502 | PASS | FAIL | Moderate | Suspected decimal/BCD issue; investigate later. |
| chris_pikul/mos6502 | PASS | FAIL | Moderate | Debug output had to be removed/disabled; correctness issue unknown. |
| olcNES | PASS | FAIL | Moderate/High | Expected decimal-mode limitation; tied to Bus/tutorial design. |

## Important caveats for later sessions

1. The vendoring was intentionally not cleaned up in this session. The user explicitly asked to defer proper vendoring/documentation cleanup and focus on getting cores to build, link, smoke-test, and run Klaus.
2. Some local source changes were made to make vendored cores build or be benchmark-usable, such as adding `<cstdint>` to `olc6502.h` and removing/debug-suppressing `std::cout` spam in `chris_pikul`.
3. Not all Klaus failures were deeply debugged. They were recorded and left for later.
4. Decimal/BCD behavior is a key expected failure mode for NMOS Klaus. `olcNES` very likely fails for this reason. `sflib6502` is suspected to fail around decimal behavior, but this needs confirmation.
5. The benchmark is intentionally `qe6502`-centered. The goal is not to create a universal neutral ranking, but integrations should still represent each participant in good faith and use a reasonably fast execution path.
6. Performance numbers vary by host machine. The user's local machine showed much higher absolute MHz than the assistant's environment, but the relative result of `qe6502` being fast on the Klaus workload remained favorable in the observed runs.

## Recommended next steps

1. Build a unified Klaus runner that links all toolbox libraries and reports one table.
2. Keep failed Klaus cores in the table as FAIL, not hidden.
3. Add a failure-reason field for each core once investigated.
4. Investigate `sflib6502` decimal behavior.
5. Investigate `chris_pikul/mos6502` Klaus failure.
6. Decide whether to patch missing decimal support in NES-oriented cores such as `olcNES`, or classify them as partial/NES-style participants.
7. Later, clean up vendoring metadata and document any local source changes.
8. Add SingleStepTests/cycle-accuracy tests after the Klaus runner is unified.
9. Add opcode microbenchmarks later, using deterministic pre-generated opcode sequences rather than runtime randomness.

## Session 1 overall opinion

The toolbox-per-core architecture is the right direction. It keeps each core's quirks localized, lets standalone tests remain simple, and will make a unified report generator straightforward.

Among the external cores integrated in this session, `floooh/chips`, `gianlucag/mos6502`, and `fake6502` currently look like the best practical benchmark participants because they build, link, smoke-test, and pass Klaus NMOS standard. `sflib6502`, `chris_pikul/mos6502`, and `olcNES` are integrated enough to be included in build and smoke-test coverage, but they should be presented with Klaus failure caveats until investigated.
