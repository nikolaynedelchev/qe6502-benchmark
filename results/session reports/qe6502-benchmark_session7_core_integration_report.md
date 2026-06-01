# qe6502-benchmark — Session 7 core integration report

This report is not a handover for the patch workflow. It is an integration-experience report for the final project notes. The goal is to capture what it was like to work with each CPU core during this session: how easy it was to integrate, how portable and flexible it felt, how well it fit the available tests, what problems appeared, what had to be patched or adapted, and what subjective conclusions can be drawn for the final evaluation.

Session identifier: **session 7**

Main themes covered in this session:

- adding Klaus tests for the new cores;
- adding NMOS SingleStep runners for the new full-NMOS candidates;
- adding NES Klaus support only where it is genuinely needed;
- exploring 65C02/CMOS model fit;
- adding final 65C02 SingleStep/Klaus mappings for selected cores;
- removing AltirraSDL from SingleStep after repeated integration problems;
- cleaning AppleWin memory access for a flatter benchmark CPU-memory model;
- adding central build/runtime metadata reporting.

## Summary table

| Core | Practical integration impression | Portability / embeddability | Test fit outcome | Final opinion from this session |
|---|---|---|---|---|
| `fceux` | Moderate; small but NES-emulator global style | Low to moderate; not a standalone generic 6502 | Good fit for NES-only Klaus; not full NMOS | Useful only as NES/2A03-style reference, not as full NMOS |
| `clk` | Very good; clean model/personality separation | Excellent | Good NMOS and 65C02 candidate; direct WDC/Rockwell/Synertek personalities | Strongest new core from an integration and flexibility point of view |
| `ares` | Good CPU component, but wrapped in heavier framework assumptions | Moderate to good | Good NMOS Klaus/SingleStep candidate; no 65C02 model found in current integration | Useful full-NMOS candidate; heavier than `clk` but workable |
| `applewin` | Moderate; CPU can be extracted, but memory model is not a clean external hook model | Moderate | Good NMOS candidate; CMOS best-fit selected as WDC for final tests | Useful, fast and practical, but Apple II memory assumptions matter |
| `mame` | Harder; very capable but framework-coupled | Moderate to low standalone portability | Strong model coverage: NMOS, WDC, Rockwell, W65C02S | Very valuable for coverage, less ideal as a tiny embeddable CPU |
| `altirrasdl` | Difficult; required repeated adjustment and still did not behave cleanly in SingleStep | Low to moderate | Klaus works, but SingleStep integration is too noisy/risky | Exclude from SingleStep for now; not a proof upstream is wrong, but integration risk is high |
| `qe6502 C++ wrapper usage` | Internal benchmark cleanup | Good after cleanup | Removed hybrid C/C++ wrapper use where possible | Important API-use cleanup, not a core integration result |
| common test infrastructure | Improved | Improved | Build/runtime metadata helper added; 65C02 classification adjusted | Useful for final reproducibility notes |

## FCEUX

### What was done

FCEUX was the only new core treated as genuinely NES/2A03-only during this session. The important policy decision was that NES tests should **not** be run on cores that also provide a normal full NMOS 6502 mode. That excluded `qe6502` NES mode, `clk` PNES mode, possible MAME RP2A03 mode, and possible Ares BCD-off mode from the main NES testing plan.

For FCEUX, a Klaus NES runner was added and included under the normal `all_6502_tests` path rather than creating a separate NES aggregate. The runner uses the strict NES Klaus ROM:

```text
asm_6502/6502_functional_test_nes_h.hex
```

That ROM disables decimal arithmetic checks but still keeps D-flag behavior checks. The more relaxed ROM that ignores D flag was kept as a fallback/debug option rather than the default.

### Test fit

FCEUX is a natural fit for NES/2A03 behavior. It should not be judged as a failing full NMOS 6502 just because it does not implement BCD decimal arithmetic. In this benchmark, it should be grouped as a NES-only candidate.

### Integration notes

The FCEUX integration remains emulator-global oriented. It is compact and small, but it is not designed as a clean standalone CPU library. That means it is useful for comparison, but it is not a strong portability example.

### Opinion

FCEUX is useful only if the benchmark explicitly wants a NES/2A03-style CPU. It should not be mixed into the full NMOS group. Its value is mostly in showing how a realistic NES emulator CPU behaves, not in demonstrating clean CPU-component embeddability.

## CLK / Clock Signal

### What was done

CLK was added to the real-test path after smoke integration. In this session it received:

- NMOS Klaus runner;
- NMOS SingleStep runner;
- 65C02 SingleStep runners for the supported personalities;
- 65C02 Klaus wiring in the aggregate paths.

The important model mappings are:

| CLK personality | Test model |
|---|---|
| `P6502` | full NMOS 6502 |
| `PWDC65C02` | WDC 65C02 |
| `PRockwell65C02` | Rockwell R65C02 |
| `PSynertek65C02` | Synertek/ST 65C02 |
| `PNES6502` | NES/2A03 style, but not used in this benchmark policy because CLK also has full NMOS |

### Test fit

CLK is the cleanest new candidate for both instruction-level and cycle-level tests. It also remains the strongest candidate for future true bus-trace work because the design naturally exposes cycle bus handling.

### Integration notes

Compared with the other cores, CLK felt the most like a reusable CPU component. The personality selection is explicit and useful. The main integration work was normal harness adaptation rather than fighting framework assumptions.

### Opinion

CLK is the strongest positive portability signal among the new cores. It is flexible, clear, and model-rich. If the final report scores embeddability, API clarity, and test-model coverage, CLK should score very high.

## Ares

### What was done

Ares received:

- NMOS Klaus runner;
- NMOS SingleStep runner;
- inclusion as a full NMOS candidate.

It was not added as a 65C02 candidate because the current extracted `ares::MOS6502` path did not expose a 65C02 instruction model. It can toggle BCD behavior, but that is not the same as supporting a CMOS 65C02 opcode set.

### Test fit

Ares fits full NMOS testing reasonably well. The BCD toggle also makes it tempting to treat it as NES-like, but benchmark policy for this session was to avoid NES tests on cores that can run as a normal NMOS candidate.

### Integration notes

The CPU component itself is usable, but the surrounding Ares framework is heavy. Earlier work required a local shim to avoid pulling in the full framework stack. During this session the core behaved like a good NMOS candidate once that extraction existed.

### Opinion

Ares is a good CPU component hidden inside a larger emulator framework. It is not as clean as CLK as a standalone integration target, but it is still a useful and credible full-NMOS benchmark participant.

## AppleWin

### What was done

AppleWin received significant work in this session:

- NMOS Klaus runner;
- NMOS SingleStep runner;
- CMOS 65C02 fit/discovery runners;
- final CMOS 65C02 mapping to WDC for aggregate tests;
- flat-memory macro cleanup for benchmark testing;
- clarification of the memory-model limitation.

AppleWin exposes a generic `cmos65c02` mode rather than separate WDC/Rockwell/Synertek personalities. Therefore a discovery step was used against all three available 65C02 SingleStep corpora.

### 65C02 model fit

The full external results showed:

```text
AppleWin CMOS 65C02 / WDC:
  instruction failed: 39/254
  functional failed: 39/209
  nop failed: 0/45

AppleWin CMOS 65C02 / Rockwell:
  instruction failed: 41/256
  functional failed: 39/209
  nop failed: 2/47

AppleWin CMOS 65C02 / Synertek-ST:
  instruction failed: 43/256
  functional failed: 9/177
  nop failed: 34/79
```

Synertek/ST had fewer functional mismatches, but many NOP mismatches. The NOP mismatches were mostly PC/operand-length differences: the opcode behaved like a NOP, but not with the same operand length or cycle behavior expected by the ST corpus. That means AppleWin is functionally closer to some parts of ST, but its undefined/NOP opcode map fits WDC better.

Final decision for aggregate tests:

```text
AppleWin CMOS 65C02 -> WDC corpus
```

### Memory model issue

A major lesson from AppleWin is that its CPU memory path is not a true external hook model in this integration. AppleWin’s CPU opcode implementation is included through headers/inl files, and memory access is heavily macro-driven. The original macro path contained Apple II memory-mapped I/O and special page/ROM behavior. For correctness testing, this was flattened so `READ` and `WRITE` go directly to flat 64K RAM.

This cleanup helped make the benchmark more CPU-focused, but it does not make AppleWin a clean callback-based CPU library. A true external memory hook model could probably be built, but it would require changing the core internals. That is risky and would likely hurt performance because the current memory operations are inlined into the CPU core.

### Test fit

AppleWin is a useful full NMOS and CMOS candidate. It is fast and practical. However, it carries Apple II emulator assumptions and has a less clean CPU/memory boundary than a library-like CPU core.

### Opinion

AppleWin is a good practical emulator CPU once extracted carefully, but it is not a clean standalone CPU component. Its performance and usefulness are strong, but portability/flexibility should be scored lower than CLK because the memory model is not naturally externalized.

## MAME

### What was done

MAME received:

- NMOS Klaus runner;
- NMOS SingleStep runner;
- 65C02 SingleStep runners for specific adapter modes;
- 65C02 Klaus/extended test wiring.

The supported MAME adapter modes used in this session are:

| MAME adapter mode | Test mapping |
|---|---|
| `nmos6502` | full NMOS 6502 |
| `wdc65c02` | WDC 65C02 |
| `rockwell65c02` | Rockwell R65C02 |
| `wdc65c02s` | WDC/W65C02S-style tests |

### Test fit

MAME is valuable because it provides several specific 6502-family variants. That made it one of the most important candidates for 65C02 coverage. Unlike AppleWin, it does not require a best-fit decision for Rockwell or WDC because the adapter exposes those modes directly.

### Integration notes

MAME is powerful but not designed as a tiny standalone CPU library. The real MAME CPU classes are tied to the MAME device framework. The benchmark adapter uses generated opcode/state paths rather than instantiating a normal MAME device. This makes it usable, but the integration is less natural than CLK and more dependent on careful extraction.

### Opinion

MAME is a high-value correctness reference because of its broad CPU-family support. It should score high for model coverage and low-to-moderate for standalone embeddability. It is very capable, but framework-coupled.

## AltirraSDL

### What was done

AltirraSDL initially received Klaus and SingleStep work:

- NMOS Klaus runner;
- attempted NMOS SingleStep runner;
- attempted 65C02 fit/discovery runner.

However, repeated attempts showed that the SingleStep integration remained noisy and difficult.

### Problems encountered

The first SingleStep implementation blocked because the scheduler tick did not advance as expected. A fix was attempted by adjusting stop-time handling and adding guards. Then the initial detail logs showed 100% instruction failure, which was clearly a harness/state extraction problem rather than a realistic CPU result. Another fix was made by comparing against the internal CPU address rather than the exported instruction PC, and masking a status representation detail.

Even after those fixes, the results remained poor and the integration felt fragile. This matched the earlier experience that AltirraSDL required many stubs and adaptations just to run as a CPU-only component.

### Final decision

AltirraSDL should be excluded from SingleStep tests for now, both NMOS and 65C02. It may remain useful for smoke/Klaus-level checks, but the SingleStep harness adds too much integration noise.

This exclusion should not be written as proof that upstream AltirraSDL is incorrect. The better conclusion is:

```text
AltirraSDL is difficult to integrate as an isolated CPU component in this benchmark. The SingleStep adapter is too noisy and risky, so it is excluded from SingleStep-based scoring for now.
```

### Opinion

AltirraSDL is powerful but framework-heavy. It is a negative portability signal for this benchmark’s “drop-in CPU component” goal. It might be excellent inside its original emulator architecture, but it is not easy to isolate cleanly.

## qe6502 C++ wrapper usage cleanup

### What was done

The benchmark had some hybrid usage of the `qe6502` C++ wrapper: code used the C++ wrapper syntax but then extracted `raw_cpu()` / `raw_tick()` and called the C API directly. This was cleaned up where the C++ wrapper already provided the needed functionality.

The only intentional exception was destructive internal-state poisoning in save/load tests, where the wrapper does not expose a normal public API for that operation.

### Opinion

This was not a third-party-core integration task, but it was important for keeping the benchmark honest. If the C++ wrapper is being benchmarked or demonstrated, the test code should use the wrapper interface rather than bypassing it through the C API.

## 65C02 classification and model-fit lessons

### NOP vs functional classification

The 65C02 result reporting uses two opcode categories:

```text
functional
nop
```

During this session, classification was compared against `qe6502` tables and available JSON corpus behavior. There was discussion around opcodes such as `0xCB` and `0xDB`, which differ across model expectations and may relate to WAI/STP or NOP behavior depending on corpus/model assumptions. The final practical choice was to leave them classified as NOP across the relevant models for reporting purposes.

### Important lesson about NOP failures

A NOP instruction-level failure does not necessarily mean the core performed a visible register or memory side effect. It can also mean:

- PC advanced by a different amount;
- the core treated the opcode as a one-byte NOP while the corpus expected a two- or three-byte NOP;
- cycle behavior differed;
- status representation bits differed.

This mattered especially for AppleWin versus Synertek/ST. AppleWin had far fewer functional mismatches against ST, but many NOP mismatches due to NOP encoding/length behavior. For final aggregate mapping, WDC was chosen because its overall opcode map, including NOP space, fit better.

## Extended 65C02 tests

The session also added handling for multiple extended Klaus variants:

- generic 65C02 extended;
- W65C02 extended;
- R65C02 extended.

The key policy is that if a core exposes a specific model, the matching extended ROM should be run in that specific mode. For example:

- Rockwell test -> Rockwell/R65C02 mode when available;
- WDC/W65C02 test -> WDC/W65C02 mode when available.

There was also a bug around success-address handling for one extended variant. The W65C02 extended ROM success loop used a different address than the Rockwell extended ROM, so treating them as identical caused false failures. That was corrected.

## Common build/runtime metadata helper

A central cross-platform helper was added to report information useful for benchmark reproducibility:

- compiler and compiler version;
- configured C++ flags;
- build type;
- operating system;
- CPU model best-effort.

This is important because performance results depend heavily on compiler, flags, operating system and processor model. It should be included in final result logs when comparing core speed.

## Overall conclusions from session 7

### Best integration experience

`clk` was the best integration experience. It is clean, flexible, and model-rich. It should be the positive example for portability and embeddability.

### Best broad model coverage

`mame` provides the richest CPU-family coverage among the new cores, especially for WDC/Rockwell/W65C02S. It is less clean as a standalone library, but it is very valuable for model coverage.

### Good practical but not clean standalone core

`applewin` is useful and performant, but the CPU/memory boundary is not a clean external hook model. Its Apple II-specific memory assumptions had to be flattened for CPU testing. It is a practical emulator extraction, not a clean reusable CPU library.

### NES-only special case

`fceux` should be treated as NES/2A03-only. It is not a full NMOS candidate and should not be punished for missing decimal arithmetic.

### Excluded from SingleStep

`altirrasdl` should be excluded from SingleStep scoring for now. The result should be documented as an integration-risk decision, not a correctness judgment against the upstream core.

### Ares position

`ares` is a reasonable full NMOS candidate. The CPU component is usable, but the surrounding framework makes it less straightforward than CLK.

## Suggested scoring dimensions for final evaluation

The final project summary could score each core on:

1. **CPU-only extraction difficulty** — how hard it was to isolate the CPU from the emulator.
2. **Build dependency size** — how many framework headers/stubs were needed.
3. **Memory interface quality** — flat memory, external callbacks, or emulator-internal globals/macros.
4. **State access quality** — clean register/state get/set support.
5. **Model coverage** — NMOS, NES, WDC 65C02, Rockwell, Synertek/ST, W65C02S.
6. **Cycle-count reliability** — whether cycle counts are meaningful and exposed cleanly.
7. **Bus-trace potential** — true per-cycle bus visibility versus semantic memory logging only.
8. **Correctness-test fit** — Klaus/SingleStep pass profile and whether mismatches are model-fit or adapter noise.
9. **Performance realism** — whether the benchmark path resembles realistic integration or an over-inlined special case.
10. **Adapter invasiveness** — how much source patching or benchmark-side trickery was needed.

From this session alone, my subjective ranking for integration suitability is:

```text
1. clk        — best overall portability/flexibility
2. ares       — good CPU component, heavier framework
3. applewin   — practical and fast, but memory boundary is not clean
4. mame       — excellent coverage, harder standalone portability
5. fceux      — useful NES-only special case, not generic
6. altirrasdl — too difficult/noisy for SingleStep integration
```

MAME could rank higher if model coverage is weighted more heavily than standalone embeddability. AppleWin could rank higher if performance is weighted more heavily than clean memory-hook design. CLK remains the clearest winner for a reusable CPU-core integration.
