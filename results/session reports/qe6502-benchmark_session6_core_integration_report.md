# qe6502-benchmark — Session 6 core integration report

This document is **not** a continuation handover. It is an integration report for **session 6**, intended as raw material for the final project evaluation.

The purpose is to record what it was like to integrate the CPU cores handled in this session: how easy they were to isolate, how portable/flexible they appear, what had to be wrapped or shimmed, what testing levels they plausibly support, and what limitations should be considered when interpreting future benchmark results.

Session scope covered these cores/features:

- `clk` / Clock Signal
- `ares` MOS6502
- `fceux` X6502
- `altirrasdl` / Altirra `ATCoProc6502`
- `applewin` CPU core
- `mame` m6502-family generated core

The final tree produced in this session was `qe6502-benchmark_v78.zip`.

---

## 1. General integration approach used in this session

The integration strategy was deliberately conservative:

1. Add one core at a time.
2. First prove a minimal CPU-only build.
3. Add a tiny smoke test using a trivial program such as `LDA #$42; TAX; NOP`.
4. Avoid modifying vendored core sources unless absolutely necessary.
5. Prefer benchmark-side wrappers/shims over changes inside third-party cores.
6. Defer Klaus, SingleStep, aggregate runners, and performance benchmarking until the core adapter is known to build and run.
7. Prune vendored trees only after the smoke target is stable.

The working pattern was:

```text
core import -> CPU-only adapter -> smoke target -> optional capability expansion -> optional pruning
```

For large vendored tree pruning, full-tree zip artifacts were sometimes safer than massive deletion patches. This was learned the hard way with the first `ares` prune patch, which applied in the assistant environment but was fragile in the user's working tree. For large delete-heavy pruning, a full cleaned tree is less error-prone than a raw recursive deletion patch.

---

## 2. Test-fit categories used in the evaluation

When judging each core, I used the following practical distinctions.

### Smoke-test fit

Can the core be built in isolation and execute a short program under a minimal host memory model?

### Klaus fit

Can the core realistically run long functional test programs with normal memory, vectors, stack behavior, branches, decimal arithmetic, and enough cycle accounting to drive the runner?

### SingleStep instruction-level fit

Can the adapter set initial CPU state, run exactly one instruction, and compare final registers/status/memory?

### SingleStep cycle-count fit

Can the adapter report a meaningful per-instruction cycle count?

### SingleStep bus-trace fit

Can the core expose true per-cycle bus activity: address, data, read/write, dummy/idle/fetch cycles, and host-provided read data at each CPU tick?

Important: semantic memory callbacks or final memory effects are **not** equivalent to bus trace. Several cores can support instruction and cycle-count testing but should still be marked `bus-trace: unsupported`.

---

## 3. `clk` / Clock Signal

### Work performed

Integrated as:

- `toolbox/clk`
- `clk_toolbox`
- `clk_smoketest`

The CPU-only build used only a very small part of the original CLK tree:

```text
Processors/6502/6502.hpp
Processors/6502/Implementation/6502Storage.cpp
```

Later the `cores/clk` tree was pruned aggressively and safely.

### Integration difficulty

**Very easy.** This was the cleanest integration of the session.

The Clock Signal 6502 code is designed in a way that is naturally embeddable. The CPU is exposed as a template-based processor with an external bus handler. It does not require dragging in the full emulator, UI, storage, display, or platform framework.

### Portability and flexibility

**Excellent.** The core appears highly portable and well isolated. The CPU can be embedded without significant framework baggage.

It supports multiple 6502-family personalities, including NMOS and 65C02 variants. That makes it attractive for future broader testing.

### Bus/memory model

This is one of the strongest aspects of `clk`. The CPU uses a bus handler model that is much closer to a true host-visible bus than the whole-instruction interpreters.

Expected test fit:

```text
Smoke:                 yes
Klaus:                 likely yes
SingleStep instruction: yes
SingleStep cycle-count: yes
SingleStep bus-trace:   likely yes / strong candidate
```

### Patches/shims required

No vendored source patches were required.

The only project-side work was a normal wrapper/smoke target and CMake wiring. Because the project was raised to C++20 earlier, no special per-target C++ standard exception was needed.

### Pruning result

The original `clk` tree was reduced from roughly:

```text
6.6 MB / 850 files
```

to roughly:

```text
103 KB / 12 files
```

This was unusually clean. The retained dependency closure was small and obvious.

### Opinion

`clk` is one of the best candidates in the project from an integration-quality perspective. It is portable, flexible, test-friendly, and close to the desired embeddable CPU model. It should score very highly for integration ease and architectural suitability.

---

## 4. `ares` MOS6502

### Work performed

Integrated as:

- `toolbox/ares`
- `ares_toolbox`
- `ares_smoketest`

The useful CPU code lives under:

```text
cores/ares/ares/component/processor/mos6502
```

A benchmark-side shim header was added:

```text
toolbox/ares/include/ares/ares.hpp
```

This shim avoided including the full `ares/ares.hpp`, which pulls in much broader ares/nall infrastructure.

### Integration difficulty

**Moderate.** The MOS6502 component itself is fairly clean, but the umbrella framework include is heavy.

The first obstacle was that the full ares framework pulls in infrastructure such as `sljit`, `libco`, platform/scheduler/node systems, and other emulator-wide dependencies. These are not needed by MOS6502 execution, but they appear if the full umbrella header is used.

The solution was to provide a small local shim for the subset of types and helpers needed by the MOS6502 component.

### JIT/recompiler note

During integration, `sljit` appeared through the full ares framework. This raised the question of whether MOS6502 execution might be JIT-backed.

The conclusion was: **the MOS6502 path used here is not JIT-based.** The JIT/recompiler infrastructure is present in the wider ares/nall tree and used by other components, but the MOS6502 component used in this adapter is an interpreter-style implementation.

### Portability and flexibility

**Good, with caveats.** Once separated from the full framework, the MOS6502 component is reasonably portable. However, it is not as plug-and-play as `clk` because it assumes ares/nall-style types and include structure.

### Bus/memory model

The CPU exposes virtual/read-write style memory integration via subclassing/overriding. This is good for semantic memory access and normal embedding, but it is not obviously a true per-cycle pin/bus model.

Expected test fit:

```text
Smoke:                  yes
Klaus:                  likely yes
SingleStep instruction: yes
SingleStep cycle-count: likely / needs validation
SingleStep bus-trace:   likely unsupported unless deeper timing hooks exist
```

### Patches/shims required

No vendored ares source was changed.

A local benchmark shim header was required to avoid dragging in the full ares framework.

### Pruning result

The original `ares` tree was reduced from roughly:

```text
5.6 MB / 869 files
```

to roughly:

```text
317 KB / 65 files
```

However, the first deletion patch for this pruning was fragile in the user's tree. The cleaned zip was usable, but the patch application experience showed that massive delete patches are risky for vendor-tree pruning.

### Opinion

`ares` is a good CPU implementation candidate, but the integration score should be lower than `clk` because the CPU component is embedded in a broader framework and needed a shim to become truly CPU-only. It remains a strong interpreter core candidate for instruction-level testing.

---

## 5. `fceux` X6502

### Work performed

Integrated as:

- `toolbox/fceux`
- `fceux_toolbox`
- `fceux_smoketest`

The main CPU file used was:

```text
cores/fceux/src/x6502.cpp
```

A benchmark-side stub file was added:

```text
toolbox/fceux/src/stubs.cpp
```

This supplied the FCEUX globals/hooks needed by the CPU file without building the full emulator.

### Integration difficulty

**Moderate to moderately hard.** The source tree is small, but the CPU is tightly coupled to FCEUX emulator globals.

The code expects global memory handler arrays, debug hooks, sound/timing globals, and other emulator-level state. These had to be stubbed or supplied by the wrapper.

### Portability and flexibility

**Fair.** The core can be isolated, but it is not designed as a standalone embeddable CPU library. It assumes the FCEUX emulator environment.

The adapter is therefore more of a controlled extraction than a clean public CPU API integration.

### CPU model caveat

FCEUX is NES-oriented. This matters because the NES 2A03 CPU does not implement decimal mode the way a normal NMOS 6502 does.

This core should not be treated as a full generic NMOS 6502 reference without explicitly considering decimal-mode behavior. It may be better categorized as a NES/2A03-flavored 6502 candidate when that distinction matters.

### Bus/memory model

FCEUX uses memory handler arrays and whole-run style execution. Host-visible memory access can be attached semantically, but this is not a clean per-cycle bus model.

Expected test fit:

```text
Smoke:                  yes
Klaus:                  likely for NES-compatible subset; generic NMOS decimal may be problematic
SingleStep instruction: possible
SingleStep cycle-count: possible / needs validation
SingleStep bus-trace:   unsupported
```

### Patches/shims required

No vendored FCEUX source was changed.

A benchmark-side stub layer was required to supply globals/hooks that the CPU file expects from the emulator.

### Pruning result

The original `fceux` tree was reduced from roughly:

```text
1.8 MB / 142 files
```

to roughly:

```text
104 KB / 22 files
```

The retained files are the CPU source/header dependencies plus basic provenance/license files.

### Opinion

FCEUX is useful, but it is not a clean embeddable CPU component. Its small size helps, but emulator-global coupling reduces portability and integration elegance. It is a good comparison core if classified correctly, especially as a NES-oriented CPU, but it should not score as highly as `clk` or a clean bus-oriented library.

---

## 6. `altirrasdl` / Altirra `ATCoProc6502`

### Work performed

Integrated as:

- `toolbox/altirrasdl`
- `altirrasdl_toolbox`
- `altirrasdl_smoketest`

The chosen path was the coprocessor-style CPU library:

```text
src/ATCPU/...
```

rather than the full Altirra emulator CPU/device layer.

A small scheduler/allocator style stub was needed on the benchmark side to satisfy dependencies without pulling in the full system/SDL/platform layer.

### Integration difficulty

**Hard.** This was the hardest of the first four cores.

Altirra has a sophisticated codebase. The CPU functionality is not simply a tiny standalone file; it depends on Altirra/VirtualDub-style system infrastructure, scheduler concepts, logging, memory maps, and related support classes.

The `ATCoProc6502` path is still much better than trying to instantiate the full emulator CPU, but it required careful selection and stubbing.

### Portability and flexibility

**Mixed.** The underlying CPU functionality is capable and flexible, but the portability story is not as clean because the code expects a substantial supporting environment.

It is portable in the sense that it can be extracted, but not portable in the sense of a small self-contained CPU library.

### Bus/memory model

The coprocessor path uses a memory map view and scheduler-like execution. It is better structured than many whole-emulator globals, but it still required framework adaptation.

Expected test fit:

```text
Smoke:                  yes
Klaus:                  likely possible
SingleStep instruction: likely possible
SingleStep cycle-count: likely possible
SingleStep bus-trace:   uncertain / likely not as clean as true pin/tick cores
```

Bus-trace suitability would need dedicated investigation. I would not assume full SingleStep bus trace support without proving that its scheduler/memory map model exposes the exact cycle-by-cycle bus sequence expected by the corpus.

### Patches/shims required

No vendored Altirra source was changed.

Benchmark-side stubs were needed for system/scheduler/allocation-related expectations.

### Pruning result

The original `altirrasdl` tree was reduced from roughly:

```text
13 MB / 1091 files
```

to roughly:

```text
435 KB / 41 files
```

This pruning was delivered as a full cleaned tree rather than as a deletion patch, which was the safer approach for such a large vendor tree.

### Opinion

Altirra/ATCPU is powerful but not lightweight. It is a serious emulator-grade CPU implementation, but integration cost is high. It should score well for capability but lower for simplicity and immediate portability. It is not a “drop in two files and go” core.

---

## 7. `applewin` CPU core

### Work performed

Integrated as:

- `toolbox/applewin`
- `applewin_toolbox`
- `applewin_smoketest`

The initial smoke adapter used AppleWin CPU implementation headers rather than compiling the full AppleWin `CPU.cpp`, because full `CPU.cpp` pulls in a large amount of emulator framework state.

Later work expanded the adapter substantially:

- full CPU state API
- NMOS 6502 mode
- CMOS 65C02 mode via `Cpu65C02()`
- reset/vector helpers
- IRQ line helper
- NMI pending/pulse helper
- jammed state
- cycle accounting
- hot-path/trivial API functions moved inline into the header

### Integration difficulty

**Moderate.** Easier than Altirra and MAME, harder than `clk`.

The implementation is not packaged as a standalone CPU library. It is AppleWin emulator code and expects a particular environment, but the opcode implementation headers could be reused fairly cleanly once the adapter provided the right globals/macros/state.

### Portability and flexibility

**Good after wrapping, but not naturally standalone.** The final adapter gives a reasonably usable CPU component, but this required benchmark-side structuring.

The CPU itself supports both NMOS and 65C02 paths, which is valuable for the benchmark matrix.

### Performance considerations

The user explicitly requested that the wrapper API not harm performance. The adapter was adjusted so trivial/hot-path functions are inline in `applewin_cpu.hpp`.

The hot path avoids:

- virtual dispatch
- callback-based bus abstraction
- extra wrapper indirection around every memory access

The remaining runtime dispatch is essentially the mode choice between `Cpu6502()` and `Cpu65C02()` plus lightweight interrupt state handling.

### Bus/memory model

AppleWin's CPU architecture is whole-instruction/block oriented rather than a pin/tick-level bus model.

This means the core can expose:

- final architectural state
- memory effects
- cycle counts
- semantic reads/writes if instrumented

But it does not naturally expose:

- true per-cycle bus address/data/RW
- dummy/idle cycles as bus events
- pin-level host-driven tick execution

Expected test fit:

```text
Smoke:                  yes
Klaus NMOS:             likely yes
Klaus 65C02:            likely / needs model validation
SingleStep instruction: yes
SingleStep cycle-count: likely yes
SingleStep bus-trace:   unsupported by architecture
```

The bus-trace limitation is not because the adapter failed to expose an obvious API. It is because the AppleWin CPU engine is not architected as a per-cycle external bus device.

### Patches/shims required

No vendored AppleWin source was changed.

A benchmark-side wrapper was built around the CPU implementation headers and expected state/macros.

### Pruning result

The original `applewin` tree was reduced from roughly:

```text
4.2 MB / 220 files
```

to roughly:

```text
113 KB / 9 files
```

The remaining files are the CPU implementation headers plus license/provenance files.

### Opinion

AppleWin is a useful candidate because it includes both 6502 and 65C02 execution paths and can be wrapped into a reasonably efficient adapter. It is not as architecturally clean as a standalone library, and it cannot provide true bus trace, but it should be very useful for instruction/cycle-level comparisons.

It should score well for capability after wrapping, medium for integration ease, and medium for portability.

---

## 8. `mame` m6502-family generated core

### Work performed

Integrated as:

- `toolbox/mame`
- `mame_toolbox`
- `mame_smoketest`

The initial goal was to use MAME's CPU functionality without instantiating the full `m6502_device` inside the full MAME `device_t/cpu_device/emu.h` framework.

The chosen approach was to use MAME's generated opcode/state implementation path and connect it to a small benchmark-side adapter.

Capabilities added over several versions:

- NMOS 6502
- WDC 65C02
- Rockwell R65C02
- WDC W65C02S

### Integration difficulty

**Hard.** MAME is large and framework-oriented.

The actual MAME device architecture is not intended to be embedded as a tiny standalone CPU library. The real `m6502_device` expects the broader MAME device framework, memory interfaces, scheduler, configuration system, and `emu.h` environment.

Directly using the full device layer would have pulled in too much infrastructure for this benchmark. The adapter therefore reused the generated opcode implementation while avoiding the full framework.

### Portability and flexibility

**High capability, low natural portability.** MAME has excellent CPU family coverage, but extracting it cleanly requires careful adapter work.

The generated/inherited CPU variant structure is powerful but not trivial to embed outside MAME. Some variants reuse or inherit layers in ways that made standalone integration heavier than expected.

### 65C02 work

The user requested that 65C02 support be prioritized. The following were integrated:

```text
NMOS 6502
WDC 65C02
Rockwell R65C02
WDC W65C02S
```

A first attempt to pull more variant layers together revealed that the generated/inherited implementation can become large and slow to compile as a single standalone translation unit. The work was split into stable incremental steps instead.

Rockwell was important because it provides real bit operations such as `RMB/SMB/BBR/BBS`, which matter for the 65C02 corpus and should not be treated as minor NOP differences.

W65C02S was added as a WDC-specific variant layer.

### Bus/memory model

The adapter is not a true MAME device and does not expose a clean per-cycle pin bus.

Expected test fit:

```text
Smoke:                  yes
Klaus NMOS:             likely yes
Klaus 65C02:            likely for supported variants
SingleStep instruction: likely yes
SingleStep cycle-count: likely yes / needs validation
SingleStep bus-trace:   unsupported in current adapter
```

MAME may internally have sophisticated timing and memory systems, but the standalone adapter path used here should not be treated as bus-trace capable unless a true per-cycle trace mechanism is proven.

### Patches/shims required

No vendored MAME source was changed.

A benchmark-side adapter was required to avoid the full MAME device framework and expose the generated CPU execution path as a small CPU object.

### Pruning result

The original `mame` tree was reduced from roughly:

```text
7.3 MB / 623 files
```

to roughly:

```text
606 KB / 126 files
```

The pruning was intentionally conservative. The entire MAME `src/devices/cpu/m6502` subtree was kept, along with README/provenance. This preserves nearby variants for possible future use and avoids over-pruning a complex generated CPU family.

### Opinion

MAME is valuable because of its breadth and mature CPU-family coverage, especially across NMOS and multiple 65C02 variants. However, it is not a lightweight embeddable CPU core. Integration required bypassing the normal device framework and building a custom adapter around generated internals.

It should score high for capability and coverage, but lower for integration simplicity and standalone portability. It is a strong benchmark candidate if the evaluation explicitly values CPU-family breadth, but not if the primary criterion is clean embeddability.

---

## 9. Relative integration impressions

The following ranking reflects this session's practical experience, not final correctness results.

### Easiest to integrate as CPU-only

```text
1. clk
2. applewin / ares
3. fceux
4. altirrasdl
5. mame
```

`clk` was clearly the cleanest. MAME and Altirra were the most framework-heavy.

### Most portable / embeddable feeling

```text
1. clk
2. ares after shim
3. applewin after wrapper
4. fceux with stubs
5. altirrasdl with framework stubs
6. mame generated adapter path
```

MAME has excellent coverage, but the extraction path is not naturally portable.

### Best expected test coverage

```text
clk:        best chance for instruction + cycle + bus-trace
mame:       strong instruction/cycle, broad 65C02 family coverage
applewin:   strong instruction/cycle for 6502+65C02, no bus-trace
ares:       good instruction candidate, cycle likely, bus uncertain/unsupported
altirrasdl: capable but needs deeper validation
fceux:      useful NES/2A03-style candidate, generic NMOS caveats
```

### Cores that required no vendored source changes

All cores in this session were integrated without modifying vendored source files. All changes were benchmark-side wrappers, CMake additions, stubs, or pruning.

This is important for final evaluation: the integration work avoided semantic patching of third-party CPU implementations.

---

## 10. Important lessons from this session

### 10.1 CPU-only isolation is a useful evaluation metric

Some cores are naturally designed as embeddable CPU components (`clk`). Others are emulator-internal engines that can be extracted but require shims (`fceux`, `applewin`, `altirrasdl`, `mame`).

This distinction should be part of the final scoring.

### 10.2 Bus trace must be judged architecturally, not by memory callbacks

Several cores allow logging memory reads/writes. That does not mean they can support SingleStep bus-trace comparisons.

A proper bus-trace core needs a per-cycle bus/pin/tick interface. Otherwise, it should be marked as instruction/cycle-only even if semantic memory access logging is possible.

### 10.3 Framework-heavy cores can still be useful

MAME and Altirra were not easy to extract, but they may still provide high-quality CPU behavior. Integration cost and CPU correctness are separate axes.

### 10.4 Pruning should be done carefully

Large vendored-tree deletion patches are fragile. For heavy pruning, full cleaned zip artifacts are safer than massive delete diffs.

### 10.5 65C02 model distinctions matter

For MAME and AppleWin, simply saying “65C02” is not enough. WDC, Rockwell, W65C02S, and Synertek/ST-style behavior can differ in meaningful ways, especially for bit operations, WAI/STP, and NOP variants.

---

## 11. Suggested final-evaluation dimensions

For the final project summary, I recommend rating each core on separate axes rather than one overall score:

1. **Integration ease** — how much work was required to get a CPU-only build.
2. **Vendored-source invasiveness** — whether third-party code had to be modified.
3. **Portability** — how independent the CPU is from the original emulator framework.
4. **Host memory control** — whether memory accesses can be fully controlled by the host.
5. **Cycle visibility** — whether the core reports meaningful cycles.
6. **Bus-trace visibility** — whether true per-cycle bus data is available.
7. **CPU-family coverage** — NMOS, 65C02, Rockwell, WDC, NES/2A03, etc.
8. **Compile/build weight** — how much code must be built to use the CPU.
9. **Adapter overhead risk** — whether the wrapper introduces runtime overhead.
10. **Suitability for benchmark fairness** — whether performance numbers are apples-to-apples with host-visible bus cores.

---

## 12. Session 6 summary table

| Core | Integration ease | Portability | 65C02 coverage | Bus-trace potential | Main issue |
| --- | --- | --- | --- | --- | --- |
| `clk` | Excellent | Excellent | Yes, multiple personalities | Strong | Requires C++20, otherwise very clean |
| `ares` | Medium | Good after shim | MOS6502 path only in this session | Unclear/likely unsupported | Full framework include pulls heavy dependencies |
| `fceux` | Medium-hard | Fair | NES/2A03-oriented, not generic 65C02 | Unsupported | Emulator globals/stubs; decimal-mode caveat |
| `altirrasdl` | Hard | Mixed | 6502/65C02 via ATCPU path likely | Unproven | Heavy supporting framework concepts |
| `applewin` | Medium | Good after wrapper | NMOS + 65C02 | Unsupported | Whole-instruction/block CPU architecture |
| `mame` | Hard | Low naturally, usable via adapter | NMOS, WDC, Rockwell, W65C02S | Unsupported in current adapter | MAME device framework too heavy; generated variant complexity |

---

## 13. Personal conclusion from session 6

From an integration and portability standpoint, `clk` was the standout core. It behaves like a component intended to be embedded.

`applewin` and `ares` are good practical candidates after wrapping. They required some adaptation, but the resulting CPU-only adapters are manageable.

`fceux` is useful, but should be classified carefully because it is NES-oriented and emulator-global-heavy.

`altirrasdl` and `mame` are high-capability but high-friction. They are valuable because they represent serious emulator codebases, but they are not lightweight CPU libraries. Their final score should distinguish capability from integration simplicity.

The biggest methodological warning is that instruction/cycle-level testability and bus-trace testability are different. Several of these cores can be good CPU correctness candidates without being fair bus-level integration candidates.
