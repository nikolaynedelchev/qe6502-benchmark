# qe6502-benchmark - Session 2 Core Integration Evaluation Report

**Session:** 2  
**Purpose:** qualitative evaluation notes for the final comparison of 6502/65C02 emulator cores.  
**Focus:** how each core behaved during integration work: ease of integration, portability, flexibility, memory/device handoff, correctness-test fit, patches required, and overall suitability as an embeddable CPU core.

This document is intentionally **not** a handoff and not a process log. It is a practical evaluation report: what it felt like to integrate each core, what problems appeared, what had to be patched, and what should be remembered when drawing conclusions from the benchmark results.

---

## 1. Evaluation criteria

The project is not only measuring raw speed. For the final report, MHz should be interpreted together with several practical criteria:

1. **Ease of integration**
   - How difficult was it to build and link the core?
   - Did it fit the existing `cores/`, `toolbox/`, `apps/` structure?
   - Did it require unusual compile flags, header implementation tricks, global symbol isolation, or C++ wrappers?

2. **Portability**
   - Is the core plain C, C99, C++, header-only, or framework-like?
   - Does it look usable from C, C++, WASM/JS, Python bindings, or other hosts?
   - Does it support multiple CPU instances cleanly?

3. **Host memory/device handoff**
   - Does the CPU give control back to the client for memory reads/writes?
   - Can a real host implement bank switching, ROM/RAM overlays, memory-mapped I/O, external devices, open bus, etc.?
   - Is the handoff complete, or are there internal direct memory accesses?

4. **Correctness-test fit**
   - Does it pass Klaus2m5 NMOS standard?
   - Does it support 65C02, and if so, does it pass Klaus 65C02 tests?
   - Are model differences explicit and testable?

5. **Model clarity**
   - Does the core clearly separate NMOS, NES/2A03, WDC 65C02, Rockwell 65C02, Synertek 65C02, etc.?
   - Or is model behavior hidden behind compile-time defines, separate headers, or unclear mixed opcode tables?

6. **Benchmark fairness**
   - Is the benchmark measuring a realistic integration path?
   - Or is the core being timed mostly against a private flat RAM array that bypasses the host?

7. **Patch burden**
   - How much code needed to be changed to pass Klaus?
   - Were the fixes local and understandable, or broad and invasive?
   - Did the core fail because of small edge cases or broader semantic issues?

8. **Personal confidence**
   - After working with the core, would I treat it as a serious external competitor/reference?
   - Would I recommend it for real emulator integration?

---

## 2. Important workflow note

During this session there was a process correction: I initially produced a patch too aggressively before the user explicitly approved the scope. That must not be repeated.

For future benchmark work:

- Do **not** create a new version just because an issue is found.
- Do **not** add extra fixes “while there”.
- Do **not** include unrequested cleanup or design changes.
- First provide analysis/proposal.
- Only create a patch/zip after the user explicitly says to proceed.
- Every accepted change should produce:
  - a versioned patch file;
  - a full-tree zip;
  - both based on the latest accepted version.
- Rejected/unaccepted patches must not be used as the next base.

This matters for final evaluation because some proposed fixes may be technically useful but methodologically different. Accepted versions and unaccepted experiments must remain separate.

---

## 3. Scope of Session 2 work

Session 2 covered:

- Fixing remaining Klaus NMOS failures in external cores.
- Reviewing whether each core actually gives memory access control back to the client.
- Adding official `sflib6502` callbacks in the benchmark/toolbox path without modifying the vendored core.
- Adding aggregate runners:
  - `all_6502_tests`
  - `all_65c02_tests`
- Adding first 65C02 contenders:
  - `fake65c02`
  - `sflib65c02`
  - `qe6502` WDC/Rockwell Klaus 65C02 targets
- Reviewing compatibility/model support across cores.
- Investigating 6510 support model and comparing `floooh/chips` with what `qe6502` should do.
- Searching for a serious missing external candidate; `vrEmu6502` was identified as the most promising future addition.

---

## 4. Current accepted status at the end of this session

At the end of this session, the latest accepted project version was **v26**.

### 4.1 6502 / NMOS aggregate status

All current 6502 targets passed:

```text
all_6502_tests summary: 14 passed, 0 failed
```

Passing NMOS/Klaus cores:

- `qe6502`
- `floooh_chips`
- `gianlucag_mos6502`
- `fake6502`
- `sflib6502`
- `olcNES`
- `chris_pikul_mos6502`

### 4.2 65C02 aggregate status

The 65C02 aggregate runner had mixed results:

```text
fake65c02                      smoke              PASS
fake65c02                      Klaus 65C02 ext    FAIL
sflib65c02                     smoke              PASS
sflib65c02                     Klaus 65C02 ext    FAIL
qe6502 WDC 65C02               Klaus standard     PASS
qe6502 WDC 65C02               Klaus extended     PASS
qe6502 RW 65C02                Klaus standard     PASS
qe6502 RW 65C02                Klaus extended     PASS

all_65c02_tests summary: 6 passed, 2 failed
```

`qe6502` Synertek 65C02 was intentionally **not** tested with Klaus in this session. The plan is to test it later with SingleStepTests because that corpus includes all memory bus operations.

---

# 5. Core-by-core evaluation

---

## 5.1 `qe6502`

### Integration and test fit

`qe6502` is the home/reference core, but it still deserves evaluation using the same practical criteria as the external cores.

It fits the benchmark methodology best because it naturally exposes bus/tick-level behavior. The benchmark is not forced to fake a callback layer or patch a memory array. The host can observe each relevant bus operation and decide how to service it.

This makes `qe6502` especially suitable for real machine emulation, not just CPU-only benchmarks. A real host often needs:

- ROM/RAM overlays;
- memory banking;
- memory-mapped devices;
- I/O side effects;
- open bus behavior;
- external wait/device logic;
- model-specific behavior.

The `qe6502` design is already aligned with that.

### Model support

`qe6502` is the strongest core in the current project in terms of explicit model support. It has runtime-selectable models such as:

- NMOS 6502
- NES / Ricoh 2A03-style mode
- WDC 65C02
- Rockwell 65C02
- Synertek 65C02

The important distinction is that these are not separate headers or compile-time accidental modes. They are explicit runtime model choices.

This is a major advantage for benchmarking and final reporting. It lets the test suite compare model-specific behavior deliberately.

### 65C02 status

During Session 2, `qe6502` WDC and Rockwell 65C02 targets were added to the aggregate 65C02 runner.

Results:

- WDC 65C02 Klaus standard: PASS
- WDC 65C02 Klaus extended: PASS
- Rockwell 65C02 Klaus standard: PASS
- Rockwell 65C02 Klaus extended: PASS

Synertek was intentionally left for later SingleStepTests.

### Portability and FFI

`qe6502` appears to be the best positioned core for long-term ABI/FFI work. It has a clean direction for:

- C ABI;
- C++ wrapper;
- WASM/JS;
- external host integration.

Compared with C++ class-based cores, it is easier to imagine stable ABI bindings. Compared with old C cores, it avoids problematic global-state design.

### Personal assessment

`qe6502` looks like the most balanced core in the project:

- good performance;
- strong correctness direction;
- explicit model support;
- realistic bus-level integration;
- good FFI potential.

I would not market it as simply “the fastest 6502 emulator”. That is too narrow and fragile. A stronger and more honest position is:

> a fast, embeddable, bus-observable 6502/65C02 core with practical multi-model support and a clean ABI path.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | High |
| Portability | High |
| FFI suitability | High |
| Memory/device handoff | Excellent |
| Model clarity | Excellent |
| Correctness confidence | High and improving |
| Benchmark fairness | Excellent |
| Overall suitability | Excellent |

---

## 5.2 `floooh_chips`

### Integration experience

`floooh/chips` is one of the strongest external reference cores in the project. Integration is somewhat unusual because it uses the common single-header / `CHIPS_IMPL` style, but once that pattern is handled, it is not difficult.

The API is pin/tick-oriented, which fits the spirit of the benchmark very well. It is not a sterile internal-memory emulator. The host services bus pins and can observe read/write behavior.

### Memory/device handoff

This is one of the best external cores for realistic host integration. The benchmark can service memory through the pin-level interface. This makes it a fair and meaningful competitor for `qe6502`.

It is not just fast-code-in-a-loop. It has a real host-visible bus model.

### 6510 support

During Session 2 I checked `floooh/chips` for 6510-specific behavior. It does have genuine 6510 support, not just a comment or a C64 wrapper.

In `m6502.h`, `m6502_desc_t` includes 6510-specific fields:

- `m6510_in_cb`
- `m6510_out_cb`
- `m6510_user_data`
- `m6510_io_pullup`
- `m6510_io_floating`

The CPU state includes 6510 port state such as:

- data direction register;
- input state;
- output latch;
- pull-up/floating behavior;
- drive state.

There is a dedicated `m6510_iorq()` path and an address check for `$0000/$0001`.

In `systems/c64.h`, that generic 6510 CPU-port behavior is connected to C64-specific banking and datasette behavior. This is a good separation:

- CPU core models the generic 6510 port;
- C64 machine layer interprets port bits as system banking/device control.

### Model support

`floooh/chips` is strong for:

- NMOS 6502;
- 6510;
- NES-like mode via decimal/BCD disable;
- many undocumented NMOS-style opcodes.

I did **not** see 65C02 support. It should not be presented as a 65C02 contender.

### Portability and FFI

The core is C-oriented and portable, but the header/macro implementation style is less ideal for a stable library ABI than a deliberately designed C ABI.

It is very easy to embed in C/C++ source form. It is less clean if the goal is a shared-library ABI or multi-language binding surface.

### Personal assessment

`floooh/chips` is the strongest external benchmark reference so far. It is not necessarily the fastest in every run, but it is methodologically valuable because it exposes a real pin/bus model.

For final conclusions, it should be treated as a high-quality competitor/reference, especially on the NMOS/6510 side.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | Medium-high |
| Portability | High as source, medium as ABI |
| FFI suitability | Medium |
| Memory/device handoff | Excellent |
| Model clarity | High for 6502/6510/NES-like, no 65C02 |
| Correctness confidence | High for tested NMOS workload |
| Benchmark fairness | Excellent |
| Overall suitability | Very high |

---

## 5.3 `gianlucag_mos6502`

### Integration experience

`gianlucag_mos6502` was relatively straightforward to integrate. It uses read/write callbacks, which makes it a practical fit for the benchmark methodology.

It is not as bus-explicit as `qe6502` or `floooh/chips`, but it does let the host service memory operations through callbacks.

### Memory/device handoff

The memory handoff is callback-based and usable. This is good enough for typical memory map integration.

It is not a full bus/pin tick model. For some detailed cycle/bus tests it may not be as expressive, but for Klaus-style functional/cycle-count benchmark it is acceptable.

### Correctness and model support

The core passed Klaus NMOS after integration. It appears to be a practical legal 6502 core. I did not find strong 65C02 support, and it should not currently be treated as a 65C02 contender.

There are traces of optional illegal opcode support in source form, but the current benchmark integration does not treat it as an undocumented-opcode reference. Without explicit tests, this should remain a caveat.

### Portability and FFI

It is C++-oriented, so it is less directly ABI-friendly than C cores. A wrapper would be needed for stable C ABI, WASM C ABI, or language bindings.

Still, it is not difficult to embed in a C++ project.

### Personal assessment

This is a good practical competitor. It does not stand out as the most flexible or most model-rich, but it was not painful to integrate and it uses a reasonable callback model.

I would keep it as a useful “normal emulator library” comparison point.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | High |
| Portability | Medium |
| FFI suitability | Medium-low without wrapper |
| Memory/device handoff | Good |
| Model clarity | Medium |
| Correctness confidence | Good for tested NMOS |
| Benchmark fairness | Good |
| Overall suitability | Good |

---

## 5.4 `fake6502`

### Integration experience

`fake6502` was easy to integrate because it is plain C and small. It uses global functions such as `read6502()` and `write6502()` for memory access. That made it simple to hook into the benchmark.

However, the global-state style is also the main weakness. It is easy for one instance and one executable, but not ideal for multi-instance embedding or clean library design.

### Memory/device handoff

The core does call global read/write hooks, so it gives the client a real path for memory operations.

This makes it more methodologically acceptable than a core that only uses a private flat RAM buffer. However, global callbacks are less flexible than per-instance callback/user-data APIs.

### Correctness and model support

The NMOS `fake6502` target passed Klaus NMOS.

The source also includes `fake65c02.h`, which was later integrated as a separate 65C02 toolbox/target because there is no runtime model selector. That is the right approach: keep `fake6502` and `fake65c02` as separate contenders.

`fake6502` also has compile-time options such as undocumented opcode support and NES-style decimal disable. These are useful, but they are not as clean as runtime model selection.

### Portability and FFI

Plain C makes it easy to build almost anywhere. It is also fairly FFI-friendly at first glance.

The problem is global state. Multiple CPU instances, reentrancy, and clean host userdata handling are weaker than in a modern library design.

### Personal assessment

`fake6502` is a good speed/reference baseline. It is simple and fast. But architecturally it feels older and less suitable as a clean embeddable multi-instance library.

I would include it in benchmark tables but not treat it as a model integration design to emulate.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | High |
| Portability | High |
| FFI suitability | Medium because of globals |
| Memory/device handoff | Good, but global |
| Model clarity | Medium, compile-time/separate-header based |
| Correctness confidence | Good for tested NMOS |
| Benchmark fairness | Good |
| Overall suitability | Good as baseline, medium as modern library |

---

## 5.5 `fake65c02`

### Integration experience

`fake65c02` was added as a separate toolbox because it is provided as a separate header/source path rather than a runtime model of `fake6502`.

This separation is correct. It avoids pretending that `fake6502` has a runtime-selectable model architecture like `qe6502`.

A small smoke test using real 65C02-only behavior passed, confirming that the target is wired up and executes at least some CMOS instructions.

### Test fit

The new target produced:

- `fake65c02` smoke: PASS
- `fake65c02` Klaus 65C02 extended: FAIL

The extended failure was intentionally left for later. The important result for this session is that the contender is set up and visible in `all_65c02_tests`.

### Portability and FFI

The same comments as `fake6502` apply:

- plain C is portable;
- global state is a limitation;
- separate target/header approach is manageable for benchmark use but weaker than runtime model selection.

### Personal assessment

`fake65c02` is an important candidate because it gives a real external 65C02 contender. However, the Klaus extended failure means it should not yet be considered a passing 65C02 reference.

It is worth investigating later, but for final conclusions the current state is:

> easy to integrate, smoke works, extended correctness not yet established.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | High |
| Portability | High |
| FFI suitability | Medium because of globals |
| Memory/device handoff | Good, but global |
| Model clarity | Medium; separate header, not runtime model |
| Correctness confidence | Low-medium until extended failure is investigated |
| Benchmark fairness | Good |
| Overall suitability | Promising but unproven for 65C02 |

---

## 5.6 `sflib6502`

### Integration experience

`sflib6502` was one of the more subtle cores to evaluate. It initially appeared to have a callback mechanism, but the actual use of that mechanism in the benchmark and inside the core required closer inspection.

The core is C-oriented and easy enough to build, but its memory access model is not as clean as it first appears.

### Patch required for Klaus NMOS

`sflib6502` initially failed Klaus NMOS. The issue was in decimal arithmetic carry handling.

The core already had decimal `ADC`/`SBC` paths, but the carry flag was computed after BCD high-nibble correction. For cases such as `0x80 + 0x99`, the carry condition must be preserved before subtracting the BCD correction value.

The accepted fix was to preserve carry before correction and then assign the carry flag from that preserved condition.

This was a small, understandable semantic fix. After the patch, `sflib6502` passed Klaus NMOS.

### Memory callback issue

This was the most important methodological issue with `sflib6502`.

The library exposes official callback tables, but the benchmark originally instantiated the callbacks as zero/null. That meant even the accesses that could have gone through the callback mechanism were falling back to direct memory array access.

At the user's request, a later accepted patch installed official read/write callbacks across the full 64K address space. The callbacks simply pass through to the backing memory:

- read callback returns `cpu->memory[address]`;
- write callback stores into `cpu->memory[address]`.

This uses what the library officially provides.

However, a caveat remains: the core itself still has internal direct memory accesses in places such as opcode fetch, stack access, and some addressing paths. An earlier experimental patch attempted to route more of those through callbacks, but that patch was not accepted and should not be considered part of the version line.

Therefore the correct final interpretation is:

> `sflib6502` has callbacks installed in the accepted benchmark, but callback coverage is incomplete inside the core.

This is very important for benchmark conclusions. Its speed should not be treated as fully equivalent to a bus/tick core such as `qe6502` or `floooh/chips`.

### 65C02 / CMOS candidate

`sflib6502` appears to have CMOS/65C02-like behavior. It contains instructions such as:

- `STZ`
- `BRA`
- `TSB`
- `TRB`
- `PHX`
- `PHY`
- `PLX`
- `PLY`
- `RMB`
- `SMB`
- `BBR`
- `BBS`

During Session 2, a separate `sflib65c02` toolbox/target was added.

Results:

- `sflib65c02` smoke: PASS
- `sflib65c02` Klaus 65C02 extended: FAIL

The failure was not investigated in this session.

### Portability and FFI

As C code, `sflib6502` is portable and potentially easy to wrap. But the incomplete callback coverage makes it weaker for real machine integration.

If a host needs every CPU memory access to be observable, this core is problematic unless the vendored core is modified.

### Personal assessment

`sflib6502` can be fast and useful, but it is methodologically tricky. It should remain in the benchmark, but always with a caveat.

I would not treat it as a clean bus/device integration reference. It is better described as:

> fast C core with official callback hooks installed in the benchmark, but incomplete internal callback coverage.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | Medium-high |
| Portability | High |
| FFI suitability | Medium-high in language terms, lower in memory-model terms |
| Memory/device handoff | Partial |
| Model clarity | Medium-low; CMOS-like but not cleanly model-selected |
| Correctness confidence | Good for NMOS Klaus after fix; low-medium for 65C02 extended |
| Benchmark fairness | Medium due to incomplete callback coverage |
| Overall suitability | Useful but caveated |

---

## 5.7 `olcNES`

### Integration experience

`olcNES` was not difficult to build, but it required real CPU semantic fixes to pass Klaus NMOS.

It is educational/project-style C++ code. It is readable and approachable, but not originally a polished standalone CPU library intended for strict external compatibility testing.

### Patches required for Klaus NMOS

`olcNES` initially failed Klaus NMOS. The main issues were:

1. **Decimal mode not implemented**
   - The `D` flag existed but was effectively unused.
   - Klaus NMOS standard tests decimal `ADC` and `SBC`.
   - Decimal/BCD paths had to be added for `ADC` and `SBC`.

2. **`BRK` behavior**
   - The core advanced PC incorrectly for Klaus expectations.
   - It pushed the wrong return address/status combination.
   - It needed to push status with `B|U` while preserving the old interrupt flag state before setting `I`.

After these fixes, `olcNES` passed Klaus NMOS.

### Memory/device handoff

`olcNES` uses a `Bus::read()` / `Bus::write()` style. This is not as clean or explicit as a pin-level bus tick, but it is a real host-replaceable memory/device layer.

A client can implement memory mapping and devices inside the bus object.

### Model support

`olcNES` is essentially a 6502/NES-style educational core. It should not be treated as a 65C02 contender. It also should not be treated as a full undocumented NMOS reference.

The fact that decimal mode had to be added reinforces that it was not originally a full NMOS 6502 compatibility core.

### Portability and FFI

As C++ class-based code, it is less ABI/FFI-friendly than C cores. It is fine for C++ embedding and educational use, weaker for stable multi-language bindings.

### Personal assessment

`olcNES` is useful as a representative educational C++ core. It is not a high-confidence accuracy reference without patches.

I would keep it in the benchmark because it is recognizable and easy to understand, but its final evaluation should mention that it required meaningful semantic fixes to pass Klaus NMOS.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | Medium |
| Portability | Medium |
| FFI suitability | Low-medium |
| Memory/device handoff | Good through Bus object |
| Model clarity | Low-medium |
| Correctness confidence | Medium after fixes |
| Benchmark fairness | Good enough |
| Overall suitability | Useful educational comparator, not strong reference |

---

## 5.8 `chris_pikul_mos6502`

### Integration experience

`chris_pikul_mos6502` was the most difficult external NMOS core in this session.

It was buildable and has a C++ bus/device abstraction, but it required many correctness fixes before Klaus NMOS would pass. The integration also encountered noisy/debug behavior that made test execution impractical until addressed earlier in the project.

### Patches required for Klaus NMOS

This core required multiple independent fixes:

1. **Relative branch sign-extension**
   - Backward branches were not handled correctly.

2. **Opcode table error**
   - Opcode `0x70` was incorrectly assigned as `BVC`; it should be `BVS`.

3. **Indexed indirect zero-page wrap**
   - `(zp,X)` high-byte handling used the wrong mask.
   - It needed zero-page wrap semantics.

4. **`PHP` and `BRK` status push**
   - Pushed status needed proper `B|U` behavior.
   - `BRK` needed to set `I` after pushing the expected status.

5. **`ROR` carry-in**
   - Carry-in was shifted into the wrong bit position.

6. **Memory `ASL` / `LSR`**
   - Memory addressing forms did not correctly write the result back to memory.

7. **`ADC` / `SBC`**
   - Binary/decimal carry/overflow/BCD behavior needed replacement/fixing to pass Klaus.

After these fixes, the core passed Klaus NMOS.

### Memory/device handoff

The design has a `Bus` / `IODevice` abstraction. This is conceptually good for host integration because the CPU accesses memory through a device/bus layer.

However, the design is heavier than simple callbacks or a pin-level C API. It likely introduces virtual/interface overhead and is less convenient for FFI.

### Model support

This core should be treated as a legal NMOS 6502 implementation after patches. It does not appear to support 65C02, NES model selection, or full undocumented opcode behavior.

### Portability and FFI

The C++ object/interface style is weaker for ABI/FFI. It may be acceptable in C++ projects, but for general embedding it is not as clean as C or a deliberate C ABI.

### Personal assessment

I have the least confidence in this core among the integrated NMOS cores. Not because it cannot now pass Klaus, but because it needed many independent semantic fixes. That suggests the original core was not in a strong correctness state.

It is still useful in the benchmark as an example of a heavier C++ bus/device design and as a real-world comparator. But I would not use it as a correctness reference.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | Low-medium |
| Portability | Medium |
| FFI suitability | Low |
| Memory/device handoff | Good conceptually |
| Model clarity | Low-medium |
| Correctness confidence | Medium-low despite Klaus pass, due to patch burden |
| Benchmark fairness | Good enough |
| Overall suitability | Low-medium |

---

## 5.9 `sflib65c02`

### Integration experience

`sflib65c02` was added as a separate 65C02-oriented toolbox/target around the existing `sflib6502` core, because the source appears CMOS/65C02-like but does not provide a clean runtime NMOS/CMOS model selector.

This separation is useful for benchmark reporting. It avoids saying that the existing `sflib6502` target is both NMOS and 65C02 in a clean model sense.

### Test fit

The smoke test passed, confirming that at least some 65C02-style instructions are supported and wired correctly.

The Klaus 65C02 extended test failed. This was expected to be possible, and the failure was intentionally not deeply investigated in this session.

There was also a practical issue: when unsupported opcodes are hit, the library can spam undefined-instruction output. A fail-fast guard was added around the benchmark path to avoid excessive noise.

### Memory model caveat

All the `sflib6502` callback caveats also apply to `sflib65c02`.

Even if the 65C02 target becomes functionally correct later, its memory handoff model remains less clean than `qe6502` or `floooh/chips`.

### Personal assessment

`sflib65c02` is worth keeping as a contender because it is one of the few available 65C02-ish external candidates already in the tree. But it should be treated as experimental until Klaus extended or SingleStepTests results are understood.

### Suitability rating

| Criterion | Assessment |
|---|---|
| Integration ease | Medium |
| Portability | High |
| FFI suitability | Medium, limited by memory-model caveat |
| Memory/device handoff | Partial |
| Model clarity | Low-medium |
| Correctness confidence | Low-medium until extended failure is investigated |
| Benchmark fairness | Medium |
| Overall suitability | Experimental but useful |

---

# 6. Cross-core observations

## 6.1 Memory handoff quality ranking

From strongest to weakest:

1. **`qe6502`**
   - Clean bus/tick model.
   - Best fit for realistic host/device integration.

2. **`floooh_chips`**
   - Pin/tick model.
   - Very strong external reference.

3. **`fake6502` / `fake65c02`**
   - Real read/write hooks.
   - Weaker because hooks/global state are not per-instance clean.

4. **`gianlucag_mos6502`**
   - Real callbacks.
   - Good but not as bus-explicit.

5. **`olcNES`**
   - Bus object.
   - Good for C++ integration, not a clean ABI.

6. **`chris_pikul_mos6502`**
   - Bus/IODevice abstraction.
   - Conceptually good but heavy.

7. **`sflib6502` / `sflib65c02`**
   - Official callbacks installed in benchmark.
   - Incomplete internal callback coverage.

## 6.2 Portability / FFI ranking

1. **`qe6502`**
   - Best long-term ABI/FFI direction.

2. **`fake6502` / `fake65c02`**
   - Plain C, but global state hurts multi-instance and clean embedding.

3. **`sflib6502` / `sflib65c02`**
   - Plain C, but memory handoff caveat hurts real host integration.

4. **`floooh_chips`**
   - Very portable as source/header; less ideal as shared ABI.

5. **`gianlucag_mos6502`**
   - C++ wrapper needed for non-C++ hosts.

6. **`olcNES`**
   - C++ educational style; not ABI-first.

7. **`chris_pikul_mos6502`**
   - C++ object/interface style; heaviest for FFI.

## 6.3 Correctness confidence after Session 2

High confidence for tested paths:

- `qe6502` NMOS/WDC/Rockwell Klaus paths
- `floooh_chips` NMOS Klaus
- `fake6502` NMOS Klaus
- `gianlucag_mos6502` NMOS Klaus

Medium confidence:

- `olcNES` after fixes
- `sflib6502` after decimal carry fix
- `chris_pikul_mos6502` after many fixes

Lower confidence / needs investigation:

- `fake65c02` extended
- `sflib65c02` extended
- `qe6502` Synertek until SingleStepTests pass

## 6.4 Patch burden ranking

Lowest burden:

- `qe6502`
- `floooh_chips`
- `fake6502`
- `gianlucag_mos6502`

Moderate:

- `sflib6502`
  - one important decimal carry fix;
  - callback setup needed in benchmark.

High:

- `olcNES`
  - missing decimal ADC/SBC;
  - BRK semantics fix.

Very high:

- `chris_pikul_mos6502`
  - many independent semantic fixes required.

## 6.5 Model support clarity ranking

Best:

1. `qe6502`
   - explicit runtime models.

Strong but narrower:

2. `floooh_chips`
   - strong 6502/6510/NES-like behavior, no 65C02.

Medium:

3. `fake6502` / `fake65c02`
   - separate headers and compile-time behavior.

Unclear/mixed:

4. `sflib6502` / `sflib65c02`
   - CMOS-like instructions exist, but no clean model selector.

Basic-only:

5. `gianlucag_mos6502`
6. `olcNES`
7. `chris_pikul_mos6502`

---

# 7. 6510 note

During Session 2, the 6510 question was discussed.

The conclusion was that `qe6502` probably does **not** need a separate `qe6502_model_6510` CPU model for now. A 6510 is essentially a NMOS 6502 execution core plus a special integrated I/O port at `$0000/$0001`.

The better design for `qe6502` is likely:

```text
qe6502_model_nmos + host-side 6510 I/O-port adapter
```

The host should implement:

- `$0000` data direction register;
- `$0001` port latch/readback;
- external input bits;
- optional pull-up/floating behavior;
- C64-style memory banking if relevant.

## Future task: create a 6510 adapter/tutorial

Create a guide or example, possibly named one of:

- `docs/integration/6510_adapter.md`
- `docs/integration/c6510_port_adapter.md`
- `examples/qe6502_6510_port_adapter.c`
- `examples/qe6502_c64_banking_minimal.c`

The purpose should be to show that `qe6502` can support 6510-style systems through the host bus layer rather than by hardcoding C64-specific behavior in the CPU core.

This could also serve as a portability demonstration:

> `qe6502` stays generic, while the host implements machine-specific devices through the bus/tick interface.

Important comparison point: `floooh/chips` has integrated generic 6510 port state in the CPU core plus C64-specific interpretation in its system layer. That is a good design, but `qe6502` can choose a more externalized adapter approach.

---

# 8. Missing serious candidate found online

A web search during Session 2 identified **`vrEmu6502`** as the most serious missing candidate to consider next.

Why it looks promising:

- C99 library;
- claims 6502/65C02 support;
- appears to include 6510, WDC65C02, R65C02-style models;
- supports unofficial opcodes;
- supports user-supplied I/O callbacks;
- appears more serious and model-rich than many small hobby cores.

Recommended future integration plan:

```text
cores/vrEmu6502/
toolbox/vremu6502/
apps/vremu6502_smoketest.cpp
apps/vremu6502_klaus_benchmark.cpp
apps/vremu65c02_smoketest.cpp
apps/vremu65c02_klaus_extended_benchmark.cpp
```

If it has runtime model selection, use one toolbox and multiple runner configurations. If not, create separate targets per model as done for `fake6502` / `fake65c02`.

---

# 9. Overall impressions by category

## 9.1 Best overall core

`qe6502`

Reason: best combination of speed, bus visibility, model clarity, ABI/FFI direction, and 65C02 support.

## 9.2 Best external reference

`floooh_chips`

Reason: high-quality pin/tick model, strong 6502/6510 support, realistic host integration.

## 9.3 Best simple speed baseline

`fake6502`

Reason: small, plain C, easy to integrate, fast, real hooks. Caveat: global state.

## 9.4 Most caveated fast core

`sflib6502`

Reason: can be fast and useful, but internal callback coverage is incomplete.

## 9.5 Most patch-heavy core

`chris_pikul_mos6502`

Reason: required many independent CPU semantic fixes to pass Klaus.

## 9.6 Most educational / least library-like core

`olcNES`

Reason: readable and useful, but originally not complete enough for strict NMOS Klaus without patches.

## 9.7 Most important future external candidate

`vrEmu6502`

Reason: likely the most serious missing C/C++-friendly competitor with 65C02 model breadth.

---

# 10. Recommended final-report caveats

The final combined report should not reduce everything to a single MHz table.

Important caveats to preserve:

1. **Bus/tick cores and private-memory cores are not measuring the same thing.**
   - `qe6502` and `floooh_chips` perform realistic bus handoff.
   - Some cores have lighter callback or internal-memory models.

2. **`sflib6502` has incomplete memory callback coverage.**
   - Official callbacks are installed in the accepted benchmark.
   - But the core still bypasses callbacks internally in some places.

3. **Patches matter.**
   - `olcNES`, `sflib6502`, and `chris_pikul_mos6502` passed Klaus only after semantic fixes.
   - The final report should mention that these are benchmark-tree patched versions, not necessarily pristine upstream behavior.

4. **65C02 is not one model.**
   - WDC, Rockwell, Synertek, and generic CMOS variants differ.
   - `qe6502` models this explicitly.
   - Most external cores do not.

5. **Klaus pass is not the end.**
   - SingleStepTests are needed for cycle/bus-level validation, especially for 65C02 and model-specific behavior.

6. **Do not claim “fastest”.**
   - A better claim for `qe6502` is that it is fast, embeddable, bus-observable, multi-model, and ABI-friendly.

---

# 11. Concise per-core conclusion table

| Core | Integration | Portability | Memory handoff | Correctness status | Model breadth | Personal confidence |
|---|---|---|---|---|---|---|
| `qe6502` | Easy/clean | High | Excellent | Strong | Excellent | Very high |
| `floooh_chips` | Medium-easy | High as source | Excellent | Strong NMOS | Strong 6502/6510 | Very high |
| `gianlucag_mos6502` | Easy | Medium | Good callbacks | Good NMOS | Limited | Good |
| `fake6502` | Easy | High | Good but global | Good NMOS | Medium via defines/headers | Good |
| `fake65c02` | Easy | High | Good but global | Smoke PASS, Klaus ext FAIL | 65C02 separate header | Promising but unproven |
| `sflib6502` | Medium | High | Partial | NMOS PASS after fix | Unclear/CMOS-like | Caveated |
| `sflib65c02` | Medium | High | Partial | Smoke PASS, Klaus ext FAIL | Unclear/CMOS-like | Experimental |
| `olcNES` | Medium | Medium | Good Bus object | PASS after fixes | Limited | Medium |
| `chris_pikul_mos6502` | Harder | Medium-low | Good but heavy | PASS after many fixes | Limited | Low-medium |

---

## 12. Final opinion from Session 2

After working with the cores, my opinion is that `qe6502` is worth continuing.

The most convincing reason is not raw speed alone. It is the combination of:

```text
realistic bus/tick host integration
explicit model support
good performance
65C02 breadth
clean ABI/FFI direction
testability
```

Several external cores are strong in one or two areas, but few combine all of these. `floooh/chips` is the strongest external reference, while `fake6502` and `sflib6502` are useful speed baselines with caveats. The 65C02 results make `qe6502` look especially promising because the external 65C02 contenders currently fail extended Klaus while `qe6502` WDC/Rockwell pass.

For final conclusions, the project should emphasize quality of integration and test realism, not only benchmark speed.
