# Interrupt adapter integration notes

This file records practical notes collected while adapting individual 6502 cores to the instruction-level basic interrupt test interface.

The notes are intentionally integration-oriented rather than a formal compatibility matrix. They describe how cleanly each adapter mapped to `benchmark6502::IInstructionCpu`, what interrupt-specific issues were encountered, and what risks remain for future users who want to reuse or extend the adapter.

## Baseline expectations

The basic interrupt tests use only `benchmark6502::IInstructionCpu`.

An adapter should model this contract as well as the underlying core allows:

- `reset()` leaves the CPU at the first opcode boundary reached through the reset vector.
- `step_instruction()` completes exactly one instruction-level operation and stops at the next instruction boundary.
- `assert_irq_for(N)` asserts IRQ immediately, keeps it asserted for the next `N` completed `step_instruction()` calls, then deasserts it automatically.
- `assert_nmi_for(N)` does the same for NMI, adapted to the core's native NMI model.
- `state().pc` should report the visible instruction-boundary PC, not an internal partially advanced PC when the core exposes cycle-level state.
- RAM, stack contents, pushed status bytes, and vectors must be observable through the adapter's `read()`/`write()` methods.

A test failure should not be hidden by broadening skip rules unless the core has a clearly documented limitation, such as legal-opcode-only support or missing native IRQ/NMI support.

## Test harness notes as of v43

The basic interrupt tests now include two explicit `IInstructionCpu` contract guards in group1:

- `assert_nmi_for is not synchronous`;
- `assert_irq_for is not synchronous`.

These guards check that calling `assert_nmi_for(N)` or `assert_irq_for(N)` records the asserted line/event without synchronously changing CPU registers, PC, stack, or handler-visible RAM before the next `step_instruction()` call. They also verify that the deferred assertion is still consumed by later stepping.

The opcode sweep still keeps the same skip policy and still reports real failures, but its output now classifies failure causes instead of mixing all failures into a single bucket. The classes separate synthetic boundary failures, synthetic IRQ-mask failures, testcase bootstrap/setup failures, testcase precondition failures, post-opcode NMI failures, post-opcode IRQ failures, and other unexpected exceptions. The TSV log has a `failure_class` column, and the console summary prints per-class totals.

The synthetic reset-NMI case is now named `first_reset_nmi_reaches_handler` instead of the old over-specific `first_reset_nmi_before_opcode`. That name is intentionally weaker: the test checks that the first reset-time NMI reaches the handler and that the continuation marker did not run, but it should not be described as a strict proof that every target opcode had zero internal side effects before NMI sampling. The prefixed NMI/IRQ cases now record the interrupt stack PC and require the interrupt to occur at the prefix-JMP or target-fetch boundary, so those cases better detect if the target opcode has actually run before the handler.

The precision `nmi_irq_pulse_sweep` keeps its normal `MemoryUnchanged` reuse for successful scenario sequences. When a scenario fails, only the following scenario is re-setup with a clean `MemoryFill` image to prevent failure-cascade contamination; if that recovery scenario passes, the runner returns to the normal unchanged-memory mode. The summary reports how many recovery memory clears were needed.

The precision `interrupt_arbitration` TSV and console labels now mark handler-fetch counters as `expected_left_*`, because those counters are derived from the left/reference side of the lockstep comparison.

## qe6502

Status: reference implementation, clean integration.

`qe6502` is the reference `IInstructionCpu` adapter used to shape the basic interrupt tests. The core exposes cycle-level bus state, opcode-fetch status, IRQ and NMI line assertion, CPU registers, and a restart path that can be driven to the first reset-vector opcode fetch.

Important adapter choices:

- The adapter owns a flat 64K RAM image and services bus reads/writes directly.
- `reset()` clears scheduled interrupt deassertions and both interrupt lines, restarts the CPU, then runs until the reset-vector target appears as an opcode fetch.
- `step_instruction()` is built from cycle ticks: align to opcode fetch if needed, consume the current instruction, then stop at the next opcode fetch.
- `state().pc` uses the bus address while the CPU is at an opcode fetch; otherwise it falls back to the internal PC.
- IRQ and NMI duration bookkeeping is instruction-count based, matching the shared `assert_*_for(N)` contract.

Observed result: all basic groups and opcode sweep pass.

Risk notes:

- This is the cleanest model for other cycle-level adapters to imitate, but other cores may not expose opcode-fetch state or line-level NMI/IRQ in exactly the same way.

## floooh/chips

Status: mostly clean cycle-level integration, with real illegal-opcode differences exposed by the sweep.

`floooh/chips` has native IRQ and NMI line controls through the existing benchmark wrapper and can be stepped at cycle level. The instruction adapter follows the qe6502-shaped cycle-level pattern: reset to start fetch, step until the next opcode fetch, and count interrupt assertion duration in completed instruction steps.

Important adapter choices:

- The adapter uses the existing wrapper memory and CPU-state access rather than inventing a separate test-only execution path.
- `restart_to_start_fetch()` is used to put the core at the post-reset opcode boundary.
- `step_instruction()` advances from one opcode-fetch boundary to the next.
- IRQ and NMI are asserted/deasserted through the wrapper's native line controls.

Observed result:

- Group1, group2, and group3 pass.
- The opcode sweep reports 12 failures, all in `first_reset_nmi_reaches_handler` for specific illegal opcodes: `$03 $13 $23 $33 $43 $53 $63 $73 $C3 $D3 $E3 $F3`.
- Legal opcodes pass in the sweep.

Risk notes:

- The failures are not hidden. They appear to be real illegal-opcode behavior differences under the interrupt-before-first-opcode scenario.
- Users should treat the adapter as good for normal interrupt testing, but should expect the basic sweep to expose illegal-opcode differences rather than silently classifying them away.
- Do not add a broad illegal-opcode skip for this core unless the selected model is explicitly redefined as legal-opcode-only.

## fake6502

Status: clean instruction-level integration, but the interrupt API is immediate-entry rather than true cycle-level pins.

`fake6502` provides native `irq6502()` and `nmi6502()` entrypoints plus instruction-level stepping. These entrypoints are not cycle-level external pins, so the adapter has to model the `IInstructionCpu` assertion-duration contract above the core's immediate interrupt functions.

Important adapter choices:

- `step_instruction()` calls the native instruction step once.
- Held IRQ is represented by adapter state. Before each instruction step, if IRQ is currently asserted, the adapter calls the native IRQ entrypoint and then executes the instruction-level step.
- NMI is represented as an adapter-side pending assertion. `assert_nmi_for(N)` records the event without synchronously changing CPU state; the native NMI entrypoint is consumed at the beginning of the next `step_instruction()`.
- The adapter still uses instruction-count deassert bookkeeping so test code sees the same duration contract as with cycle-level cores.

Observed result: all basic groups and opcode sweep pass after deferring the native NMI entrypoint to `step_instruction()` instead of running it synchronously inside `assert_nmi_for(N)`.

Risk notes:

- This is not a pin-accurate interrupt adapter. It is a best mapping from immediate interrupt entrypoints to the instruction-level interface.
- The current basic tests accept this mapping, including the guard that `assert_nmi_for(N)` and `assert_irq_for(N)` must not synchronously mutate CPU state before the next `step_instruction()`.
- Future tests that depend on true line sampling inside a specific instruction would not be appropriate for this adapter.

## AppleWin

Status: clean instruction-level integration after using the core's native interrupt helpers directly.

The AppleWin integration exposes enough CPU state, memory access, instruction execution, IRQ line control, and NMI triggering to satisfy `IInstructionCpu`. It is less isolated than qe6502 because the wrapped core is more global/system-shaped, but the adapter can still provide the expected instruction-level behavior.

Important adapter choices:

- `reset()` clears scheduled adapter interrupt state, clears interrupt-related core state, and resets from the vector into NMOS mode.
- `step_instruction()` uses the native one-instruction execution path.
- IRQ is modeled as a held line using the native IRQ line helper.
- NMI is modeled through the native NMI pulse/pending mechanism.
- The adapter relies on AppleWin's own CPU state and memory access paths instead of trying to duplicate CPU internals.

Observed result: all basic groups and opcode sweep pass.

Risk notes:

- AppleWin is a larger machine-oriented core, not a small standalone CPU library. Future changes should be careful not to accidentally depend on unrelated machine state.
- NMI is pulse/pending based rather than a simple exposed boolean line, so duration bookkeeping is necessarily adapter-level.

## MAME

Status: clean integration after adding explicit instruction-boundary handling in the wrapper.

The MAME wrapper has native IRQ and NMI line controls and internal cycle accounting, but it is not an externally bus-cycle-visible precision-test core in this benchmark tree. The integration is instruction-level: a naive `execute(1)` mapping would spend one MAME scheduler/counting unit, not necessarily one complete 6502 instruction, and would not satisfy the `IInstructionCpu` contract.

Important adapter choices:

- The wrapper was extended with an instruction-boundary execution path.
- `reset()` clears IRQ/NMI lines, resets from the vector, then places the visible state at the reset opcode boundary expected by the basic tests.
- `step_instruction()` executes one full instruction from the synthetic/visible opcode boundary.
- IRQ uses the native IRQ line helper.
- NMI uses the native NMI line helper and adapter scheduling to match `assert_nmi_for(N)`.

Observed result: all basic groups and opcode sweep pass.

Risk notes:

- The adapter must not regress to a raw `execute(1)` implementation; that would violate the instruction-level interface.
- The visible opcode-boundary state is an adapter concern and should be preserved when modifying the MAME wrapper.
- MAME has multiple 6502-family variants; the current notes apply to the NMOS target that was wired to the basic tests.


## CLK P6502

Status: clean cycle-level integration, pending the same illegal-opcode scrutiny as other full NMOS sweep targets.

CLK exposes a cycle-run API, opcode-read bus operation visibility, CPU registers, RAM access through the benchmark wrapper, and native IRQ/NMI line controls. This makes it a good cycle-level candidate for an `IInstructionCpu` adapter even though the basic tests still see only the instruction-level interface.

Important adapter choices:

- The adapter uses the existing `cpu6502_bridge::ICpu` wrapper and does not bypass the core's native bus handler.
- `reset()` uses the wrapper's reset-to-start-fetch path, then clears IRQ and NMI lines.
- `step_instruction()` follows the qe6502/floooh pattern: align to opcode fetch, consume the current instruction, and stop at the next opcode fetch.
- `state().pc` reports the opcode-fetch bus address when visible, because the internal PC may already be partially advanced around instruction boundaries.
- IRQ and NMI assertion duration is tracked in completed instruction steps and mapped onto CLK's native line setters.

Observed result:

- Smoke passes.
- Group1: 16/17 passed. The remaining failure is `IRQ RTI resumes`, where the IRQ is accepted but mainline resume after RTI does not match the generic expectation.
- Group2: 13/13 passed.
- Group3: 4/4 passed.
- Opcode sweep: 1185 passed, 114 skipped, 237 failed; legal 755 passed / 151 failed, illegal 430 passed / 86 failed.

Risk notes:

- CLK has strong native line support, but its bus handler reports opcode reads after the opcode bus operation has already been performed. The adapter therefore owns visible instruction-boundary modeling and deasserts scheduled interrupt lines from the opcode-read bus callback to avoid making short IRQ assertions sticky.
- A small IRQ-line visibility allowance is needed when IRQ is asserted while `I` is already clear; without it, one-instruction IRQ assertions are sampled too late by the core. This is adapter compensation for the visible boundary model, not a test skip.
- The remaining group1 failure and opcode-sweep failures are not hidden. They should be treated as observed behavioral differences or remaining adapter-risk areas until investigated further.
- Future changes should preserve the opcode-fetch visible-PC rule rather than returning the raw internal PC unconditionally.

## Peddle

Status: clean but slightly more synthetic cycle-level integration due to wrapper-provided reset/fetch boundary handling.

Peddle has native IRQ and NMI line controls and can execute at a small-step level through the existing benchmark wrapper. The wrapper already provides a reset-to-start-fetch helper because the core's native reset path is not naturally exposed as the same bus-cycle stream used by the strict precision runners.

Important adapter choices:

- The adapter uses the existing `cpu6502_bridge::ICpu` wrapper, including its synthetic `restart_to_start_fetch()` implementation.
- IRQ and NMI are mapped to Peddle's native pull-down/release interrupt-line API.
- `step_instruction()` is built from wrapper steps and opcode-fetch observation.
- `state().pc` uses the visible opcode-fetch address when available, matching the other cycle-level instruction adapters.
- Interrupt assertion duration remains adapter-level instruction-count bookkeeping.

Observed result:

- Smoke passes after moving the instruction adapter away from the synthetic cycle-wrapper boundary and using Peddle's native `executeInstruction()` path directly.
- Group1: 8/17 passed.
- Group2: 1/13 passed.
- Group3: 1/4 passed.
- Opcode sweep: 240 passed, 114 skipped, 1182 failed; legal 154 passed / 752 failed, illegal 86 passed / 430 failed.

Risk notes:

- The native line model is present, but the first direct instruction adapter still does not make Peddle's IRQ/NMI detector timing line up with the generic `IInstructionCpu` assertions. This is not hidden by skips.
- The older `cpu6502_bridge::ICpu` wrapper exposes a synthetic reset/start-fetch boundary; that was not sufficient for an instruction adapter because the first smoke test did not execute the first instruction correctly. The current adapter uses Peddle directly for instruction execution instead.
- The broad interrupt failures mean this adapter should be treated as experimental/incomplete for basic interrupt testing until the Peddle interrupt polling/detector timing is mapped more precisely.
- The adapter is useful as a recorded integration attempt and as evidence that Peddle needs more careful interrupt-specific work, not as a clean passing target yet.

## O2

Status: native IRQ/NMI support is present, but the first instruction-level interrupt integration is not yet clean.

O2 exposes native IRQ and NMI inputs through its core API, and it was already usable as a cycle-level precision-test candidate. The instruction-level adapter is built over the existing cycle-level bridge: it resets directly to the first opcode fetch, implements `step_instruction()` by running from one opcode-fetch boundary to the next, and tracks IRQ/NMI assertion duration in completed instruction steps.

Important adapter choices:

- The adapter uses the existing `cpu6502_bridge::ICpu` wrapper rather than touching the O2 core directly.
- `reset()` uses the wrapper's reset-to-start-fetch path and clears interrupt state.
- `state().pc` reports the visible opcode-fetch address when available.
- O2's IRQ request is cleared internally when accepted, so the adapter keeps its own held-IRQ state and re-raises IRQ before each instruction while the `assert_irq_for(N)` interval is still active. This is necessary to model the generic level-sensitive IRQ contract.
- NMI is left as a single request-style assertion; held NMI should not retrigger merely because the line remains asserted for multiple instruction steps.

Observed result:

- Smoke passes.
- Group1: 15/17 passed. Remaining failures are `BRK vector and RTI` and `NMI pushed B=0 and live I=1`.
- Group2: 13/13 passed after compensating held IRQ as a level.
- Group3: 3/4 passed. The remaining failure is the short IRQ assertion expiry case.
- Opcode sweep: 1149 passed, 114 skipped, 273 failed; legal 756 passed / 150 failed, illegal 393 passed / 123 failed.

Risk notes:

- The core has native interrupt inputs, but the adapter still has observable mismatches around BRK/RTI, live `I` during NMI, short IRQ expiry, and many opcode-sweep interrupt probes.
- O2 appears to clear accepted IRQ requests internally, which is fine for the core but requires adapter-side held-line bookkeeping for `IInstructionCpu`.
- The remaining failures are not skipped or hidden. They should be treated as either core behavioral differences or areas where the instruction-boundary adapter still needs deeper timing analysis.
- Illegal-opcode testcase failures are especially expected to need careful review, because O2's undocumented-opcode behavior may not match the qe6502 NMOS testcase collection.

## vrEmu6502

Status: clean instruction-level interrupt integration with one visible opcode-testcase mismatch.

vrEmu6502 has native IRQ and NMI signal pointers (`vrEmu6502Int()` and `vrEmu6502Nmi()`), plus an instruction-level execution entrypoint (`vrEmu6502InstCycle()`). This makes it a good instruction-level adapter candidate rather than a precision/cycle-level candidate: it accounts for cycles internally but does not expose a complete external bus-cycle trace suitable for the precision interrupt tests.

Important adapter choices:

- The adapter uses the undocumented NMOS model (`CPU_6502U`) so the existing NMOS illegal-opcode sweep can run without a legal-only capability mode.
- The public reset API leaves several registers dependent on existing internal state, so the adapter initializes the reset-visible state explicitly to match the basic instruction-level contract.
- IRQ is modeled as a held level by keeping `vrEmu6502Int()` requested until the scheduled instruction-count deassert point.
- NMI is naturally request/clear style inside the core and is asserted according to the shared `assert_nmi_for(N)` contract.

Observed result:

- Smoke passes.
- Group1: 17/17 passed.
- Group2: 13/13 passed.
- Group3: 4/4 passed.
- Opcode sweep: 1420 passed, 114 skipped, 2 failed; legal 904 passed / 2 failed, illegal 516 passed / 0 failed.
- The two failures are both `$6C JMP (abs)` NMOS indirect-wrap testcase probes: one before NMI and one before IRQ.

Risk notes:

- The interrupt model itself looks clean for the basic instruction-level groups.
- The visible `$6C` testcase failure should not be hidden. It likely points to a JMP-indirect page-wrap behavior mismatch in this core/model or in the way the adapter drives the testcase flow.
- Because vrEmu6502 is not externally bus-cycle visible, it remains inappropriate for the precision interrupt tests even though it has cycle counts internally.

## chris_pikul_mos6502

Status: interrupt groups mostly clean after adapter compensation, but illegal-opcode coverage is weak.

chris_pikul_mos6502 exposes native `IRQ()` and `NMI()` entry functions and an instruction-shaped `Tick()` model that executes the opcode semantics immediately, then counts down remaining cycles. It does not expose true external interrupt pins; the adapter therefore maps `assert_irq_for(N)` and `assert_nmi_for(N)` to pending adapter-side assertions that are consumed at the beginning of `step_instruction()`.

Important adapter choices:

- The adapter owns a normal chris_pikul memory/bus pair and exposes it through `IInstructionCpu::read()`/`write()`.
- Reset is adapted to the basic-test contract by setting the reset-visible interrupt-disable and unused status bits after the core reset.
- IRQ and NMI entry are implemented through a small adapter subclass rather than using the raw `CPU::IRQ()`/`CPU::NMI()` methods directly, because the native methods push status after modifying live flags. The adapter preserves the pre-interrupt pushed status bits expected by the basic tests.
- `step_instruction()` treats interrupt entry as the completed instruction-level operation when an adapter-side NMI or acceptable held IRQ is pending.

Observed result:

- Smoke passes.
- Group1: 17/17 passed.
- Group2: 13/13 passed after pushed-status compensation.
- Group3: 4/4 passed.
- Opcode sweep: 1262 passed, 114 skipped, 160 failed; legal 906 passed / 0 failed, illegal 356 passed / 160 failed.

Risk notes:

- Interrupt entry behavior can be mapped cleanly enough for the grouped basic tests, but this is not a pin-level model.
- The opcode sweep failures are all illegal-opcode testcase-flow failures. They are not hidden; the core prints illegal/unknown instruction diagnostics and does not execute many undocumented NMOS opcodes like the qe6502 testcase collection expects.
- This adapter is suitable for legal-opcode basic interrupt behavior, but users should not expect full undocumented NMOS opcode compatibility.

## olcNES

Status: clean grouped interrupt behavior after adapter compensation, but legal-only/NES-style opcode behavior leaves illegal sweep failures visible.

olcNES exposes native `irq()` and `nmi()` entry functions plus a clock/complete instruction boundary helper. Like several instruction-level cores, its IRQ/NMI functions are immediate interrupt-entry helpers rather than externally sampled pins. The adapter models held assertions above those native entrypoints.

Important adapter choices:

- The adapter uses the core's public `Bus` RAM and public CPU registers.
- Reset drains the native reset-cycle countdown and then sets the reset-visible `I` and `U` bits expected by the basic interface.
- IRQ/NMI entry uses the native helper to establish the core's internal timing state, then patches the pushed status byte to preserve the pre-interrupt `I`/`B` pattern expected for external IRQ/NMI. This compensates for the native helper setting `I` before pushing `P`.
- `step_instruction()` treats a pending NMI or acceptable held IRQ as the instruction-level operation, then drains the native interrupt countdown to the next instruction boundary.

Observed result:

- Smoke passes.
- Group1: 17/17 passed.
- Group2: 13/13 passed after pushed-status compensation.
- Group3: 4/4 passed.
- Opcode sweep: 1262 passed, 114 skipped, 160 failed; legal 906 passed / 0 failed, illegal 356 passed / 160 failed.

Risk notes:

- The core's external interrupt helpers are not pin-level inputs, but the adapter maps them well enough for the grouped instruction-level interrupt tests.
- The core is NES-oriented and implements many undocumented opcodes as `XXX`/NOP-like placeholders rather than full NMOS illegal behavior. The sweep failures are therefore expected and left visible.
- Decimal arithmetic is also NES-oriented/unused in the upstream core; current grouped interrupt tests do not expose a failure after adapter compensation, but users should be cautious about treating this as a full NMOS decimal model.

## ares

Status: clean instruction-level integration.

ares `MOS6502` exposes an instruction-level `instruction()` entrypoint, native interrupt machinery through virtual hooks (`nmi()`, `irqPending()`, `cancelNmi()`), full CPU state, and overrideable memory access. This made it the cleanest of this batch.

Important adapter choices:

- The adapter derives from `ares::MOS6502`, overrides memory access, and stores a flat 64K RAM image.
- Reset uses the native `power()` and `reset()` flow with BCD enabled, so the selected model is documented as `MOS6502 NMOS/BCD`.
- Held IRQ is implemented by adapter state plus the native `irqPending()` hook.
- NMI is implemented by adapter state plus the native `nmi(vector)`/`cancelNmi()` hooks.
- `step_instruction()` dispatches either native interrupt entry or exactly one native instruction-level operation.

Observed result:

- Smoke passes.
- Group1: 17/17 passed.
- Group2: 13/13 passed.
- Group3: 4/4 passed.
- Opcode sweep: 1422 passed, 114 skipped, 0 failed; legal 906 passed / 0 failed, illegal 516 passed / 0 failed.

Risk notes:

- This is a strong adapter candidate for instruction-level interrupt testing.
- It is not wired as a precision/cycle-level target here because the current wrapper uses instruction-level execution, even though the core internally accounts bus accesses through its read/write hooks.
- Future users should keep the selected BCD-enabled model explicit, because ares can be configured differently.

## sflib6502

Status: grouped interrupt behavior is mostly clean after adapter compensation, but opcode coverage is partial and the sweep exposes it.

sflib6502 exposes native `M6502_irq()` and `M6502_nmi()` entrypoints and a one-instruction `M6502_step()` helper. The native interrupt helpers are immediate entry functions, not sampled external lines. They also push status in a way that does not fully match the external IRQ/NMI expectations of the basic tests, so the adapter implements interrupt entry itself while still using the native CPU state and instruction execution.

Important adapter choices:

- The adapter owns `M6502_Registers`, `M6502_Memory`, and `M6502_Callbacks`, then creates a native `M6502` instance around them.
- Reset initializes stack/status explicitly and then uses the native reset vector behavior.
- `step_instruction()` executes either adapter-modeled interrupt entry or one native `M6502_step()`.
- Adapter-modeled IRQ/NMI entry clears `B` in the pushed status, preserves `U`, sets live `I`, and vectors through the standard NMI/IRQ locations.

Observed result:

- Smoke passes.
- Group1: 17/17 passed.
- Group2: 13/13 passed after pushed-status compensation.
- Group3: 4/4 passed.
- Opcode sweep: 1282 passed, 114 skipped, 140 failed; legal 904 passed / 2 failed, illegal 378 passed / 138 failed.

Risk notes:

- The core has usable native IRQ/NMI entry support, but the adapter must compensate pushed status semantics for the generic external-interrupt contract.
- The sweep failures are mostly undocumented-opcode testcase-flow failures, with two legal `$6C JMP (abs)` NMOS indirect-wrap failures also visible.
- Do not hide these failures with a broad skip rule. If this core is used as legal-only or non-NMOS-indirect-wrap-compatible, that should be a separate explicit model/capability decision.

## sflib65c02

Status: grouped interrupt behavior matches the existing sflib-style adapter model, but the current NMOS opcode sweep is not a natural fit for this 65C02-labelled target.

sflib65c02 is integrated through the same lib6502-style CPU API used by sflib6502: `M6502_step()` executes one instruction, and `M6502_irq()` / `M6502_nmi()` exist as native immediate interrupt-entry helpers. Those helpers are not true sampled external pins, so the adapter follows the sflib6502 approach and models IRQ/NMI entry itself while reusing the native register and memory structures.

Important adapter choices:

- The adapter owns `M6502_Registers`, `M6502_Memory`, and `M6502_Callbacks`, then creates a native `M6502` instance around them.
- Reset initializes the stack/status state explicitly and then uses the native reset-vector behavior.
- `step_instruction()` executes either adapter-modeled interrupt entry or one native `M6502_step()`.
- Adapter-modeled IRQ/NMI entry preserves the pre-interrupt pushed status pattern expected by the basic tests, clears `B`, preserves `U`, sets live `I`, and vectors through the standard NMI/IRQ locations.

Observed result:

- Smoke passes.
- Group1: 17/17 passed.
- Group2: 13/13 passed.
- Group3: 4/4 passed.
- Opcode sweep: 1282 passed, 114 skipped, 140 failed; legal 904 passed / 2 failed, illegal 378 passed / 138 failed.

Risk notes:

- The interrupt behavior exposed through the grouped tests is clean after adapter-side interrupt-entry modeling.
- The opcode sweep is still the NMOS basic sweep. It is useful as a diagnostic, but it should not be mistaken for a proper 65C02 opcode-compatibility statement.
- The visible failures mirror the sflib6502 pattern: mostly undocumented NMOS opcode testcase-flow failures, plus two legal `$6C JMP (abs)` NMOS indirect-wrap failures.
- A future 65C02-specific basic sweep would be a better long-term fit for this core than broadening NMOS skip rules.

## fceux

Status: clean instruction-level integration for the current NMOS/NES basic interrupt collection.

FCEUX exposes native interrupt support through `X6502_IRQBegin()` / `X6502_IRQEnd()` for IRQ and `TriggerNMI()` for NMI. It is not a precision/cycle-level target here: `X6502_Run(N)` uses the emulator scheduler/cycle budget and can execute multiple CPU instructions. The adapter therefore drives exactly one instruction-level operation by setting the internal run count to a tiny positive value and calling `X6502_Run(0)`.

Important adapter choices:

- The adapter installs flat 64K read/write handlers into FCEUX's global `ARead` and `BWrite` tables and points `RAM` at the adapter memory.
- Reset is adapted manually to the basic-test boundary contract: after native init/power, the adapter clears pending IRQ state, loads the reset vector into `X.PC`, sets `I`, initializes `S`, and stops before the first instruction.
- IRQ is modeled as a held native line with `X6502_IRQBegin(FCEU_IQEXT)` / `X6502_IRQEnd(FCEU_IQEXT)` according to the `assert_irq_for(N)` instruction-count contract.
- NMI is modeled as an assertion event via `TriggerNMI()` at the start of the next `step_instruction()`.
- The adapter updates FCEUX's previous-status latch (`X.mooPI`) at instruction boundaries so that the generic `IInstructionCpu` boundary model sees IRQ acceptance after a completed `CLI`, while still keeping the grouped CLI-delay test behavior correct.

Observed result:

- Smoke passes.
- Group1: 17/17 passed.
- Group2: 13/13 passed.
- Group3: 4/4 passed.
- Opcode sweep: 1422 passed, 114 skipped, 0 failed; legal 906 passed / 0 failed, illegal 516 passed / 0 failed.

Risk notes:

- This is a clean basic interrupt adapter, but it depends on FCEUX global CPU/memory state. Multiple adapter instances should not be used concurrently.
- FCEUX is NES/Ricoh-oriented. The current tests pass, including the decimal-flag preservation check, but users should not infer full NMOS decimal arithmetic behavior from this interrupt-focused result.
- The adapter intentionally does not claim cycle-level precision. It uses instruction-boundary control over a scheduler-budgeted emulator core.
- The `X.mooPI` boundary synchronization is important. Removing it makes IRQs asserted after a completed `CLI` appear one instruction too late for the generic basic interface.

## gianlucag_mos6502

- Status: integrated into the instruction-level basic interrupt tests in v41.
- Native interrupt support: the core exposes explicit IRQ and NMI input-line methods. IRQ is active-low level-sensitive and NMI is high-to-low edge-triggered; both line states survive reset in the upstream core, so the adapter explicitly returns them to inactive state on every adapter reset.
- Integration shape: instruction-level. The adapter uses the core's `Run(1, ..., INST_COUNT)` entrypoint and keeps the basic-suite assertion-duration bookkeeping outside the core.
- Cleanliness: relatively clean, because the core gives direct register setters/getters, direct memory callbacks, and native IRQ/NMI line APIs. The main risk is that interrupt servicing is folded into the core's instruction-count execution model rather than being externally visible as a separate bus/cycle sequence.
- Expected behavior: grouped basic interrupt tests are the important signal. Opcode-sweep differences, if any, should be interpreted as opcode-semantic differences rather than as adapter-side hidden skips.

## altirrasdl

- Status: not integrated into the instruction-level basic interrupt tests in v41.
- Native interrupt support: the trimmed Altirra co-processor wrapper currently used in this benchmark tree does not expose a public, adapter-usable IRQ/NMI input API. The internal decode/state machine contains interrupt states, but they are not reachable as clean external line controls through the available wrapper surface.
- Integration shape: excluded for now rather than adapted by patching internal core state or synthesizing interrupts manually. That keeps the basic interrupt suite focused on cores with native IRQ/NMI controls.
- Cleanliness/risk: high risk. Previous Altirra SDL integration work was already noisy, and forcing interrupt entry by reaching into private machine internals would make results hard to trust and hard to compare with other adapters.
- Expected behavior: treat altirrasdl as not applicable to the generic instruction-level basic interrupt collection until a clean public IRQ/NMI control surface is added to its wrapper.

## all_instruction_interrupt_basic

- Status: added in v41.
- Purpose: runs the complete instruction-level basic interrupt collection for every core that currently has a real `benchmark6502::IInstructionCpu` adapter.
- Inclusion policy: it includes integrated cores only. Cores documented as excluded because they lack a clean native IRQ/NMI adapter surface are intentionally not included.

## v44 precision interrupt comparison note

v44 changes the precision interrupt suites to separate primary public-bus identity from secondary trace metadata differences.

Primary precision pass/fail now treats the externally visible bus tuple as the important lockstep signal:

- read/write direction;
- address;
- data.

Opcode-fetch classification and visible IRQ/NMI line annotation differences are still collected, but they are reported as secondary metadata counters rather than being mixed into the primary bus mismatch count. The detail TSV files now include:

- `secondary_mismatches`;
- `secondary_fetch_tag`;
- `secondary_irq_line`;
- `secondary_nmi_line`.

This is especially useful for cycle-level adapters such as CLK, where a row can have the same public bus read/write/address/data but disagree on whether that row is annotated as `FETCH`. Those cases are now visible as secondary metadata differences. They should not be described as vector/address/data failures unless a primary public-bus mismatch also occurs.

The strict precision suites still fail on real bus divergence. Secondary metadata differences are diagnostic context and do not by themselves make a scenario fail.
