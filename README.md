# qe6502-benchmark CPU Core Evaluation Report

This report summarizes the CPU-core benchmark data present in this repository snapshot. It is intended to be read from the project root on GitHub and to give a practical overview of what the provided test outputs, detail logs, methodology notes, and benchmark integration code show about each integrated 6502-family core.

The report is divided into two parts:

1. **Objective data and statistics** — test results, failure counts, measured speeds, supported validation levels, and factual integration notes.
2. **Interpretation and overall ranking** — a reasoned reading of the data and integration tradeoffs by core.


## Source material used

The report is based on the repository contents in this snapshot, especially:

| Source | Used for |
| --- | --- |
| `results/nmos_results/tests_6502_output.txt` | Printed NMOS/NES smoke, Klaus, and SingleStep summary output. |
| `results/cmos_results/tests_65c02_output.txt` | Printed 65C02 smoke, Klaus, and SingleStep summary output. |
| `results/nmos_results/*.tsv` | Per-opcode NMOS SingleStep detail logs. |
| `results/cmos_results/*.tsv` | Per-opcode 65C02 SingleStep detail logs. |
| `qe6502-benchmark_test_methodology.md` | Methodology and interpretation rules for smoke, Klaus, and SingleStep results. |
| `toolbox/*` and top-level aggregate runners | Current benchmark integration shape, supported runners, and adoption/integration implications. |

All performance numbers below are from the printed result files. They should be read as measurements for this exact build/run environment, not as universal hardware-independent speeds. Integration notes are based on the wrappers and vendored core APIs visible in this repository snapshot.


## How to read these results

This benchmark suite was created during `qe6502` development, but it was not written to make `qe6502` look correct. It was written to find mismatches.

The project tests several layers of behavior: functional ROM execution, instruction results, cycle counts, bus traces, CPU-model differences, and later detailed interrupt behavior. These tests exposed real bugs and corner cases during `qe6502` development, and they are included here so other emulator authors can use the same scenarios to investigate their own cores.

When possible, the expected behavior comes from external sources such as SingleStep testcase corpora and `perfect6502`-style netlist lockstep comparisons, not from `qe6502` itself. A clean `qe6502` result is therefore a validation result against those tested scenarios, not a self-referential pass.

During integration, each external core was given individual attention and the adapters were written to use the core as fairly and correctly as possible for these tests. Even so, the benchmark cannot guarantee that every adapter or usage pattern is 100% optimal or equivalent to how the original author would integrate the core. Some mismatches may therefore reflect adapter limitations, API mismatches, or harness usage issues rather than defects in the core itself.

# Part I — Objective data and statistics

## Build/runtime environment for the supplied result files

Both aggregate result files print the same environment:

| Field | Value |
| --- | --- |
| Compiler | AppleClang 21.0.0.21000101 |
| Build type | Release |
| C++ flags | `-O3 -DNDEBUG` |
| Operating system | macOS |
| CPU model | Apple M5 |

## Test categories and counting rules

| Category | What the result says | Important counting rule |
| --- | --- | --- |
| Smoke | Basic link/reset/execution sanity for a wrapper. | Reported as PASS/FAIL. |
| Klaus | Functional ROM reaches the expected success address; also used for measured throughput. | PASS does not imply SingleStep exactness. |
| SingleStep instruction | Final CPU state and expected final RAM cells after one instruction. | Headline failures are counted per opcode, not per case. |
| SingleStep cycle-count | Total cycles for an opcode match the corpus expectation. | Unsupported is a capability limit, not a failed result. |
| SingleStep bus-trace | Full per-cycle address/read-write/data trace matches the corpus. | Requires a real per-cycle bus-facing API. |

For NMOS SingleStep, the output splits opcodes into **legal** and **illegal/undocumented** groups. For 65C02 SingleStep, the output splits opcodes into **functional** and **NOP** groups.

## Smoke and aggregate status

| Aggregate | Printed summary |
| --- | --- |
| `all_6502_tests` | `32 passed, 0 failed` |
| `all_65c02_tests` | `21 passed, 4 failed` |

The 65C02 aggregate includes candidate/core failures as measured results. In the supplied output the printed failures are: `fake65c02` Klaus extended, `sflib65c02` Klaus extended, AppleWin WDC 65C02 Klaus extended, and MAME WDC 65C02 Klaus extended.

## Klaus NMOS / NES performance table

| Rank by MHz | Core | Test | Status | Cycles | Seconds | MHz | Factual note |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | sflib6502 | Klaus NMOS | PASS | 492,149,365 | 0.450108 | 1,093.40 | Highest measured MHz; direct-memory/callback-coverage caveat in methodology and code |
| 2 | applewin | Klaus NMOS | PASS | 481,206,820 | 0.563016 | 854.70 | Direct flat-memory Apple II extraction; similar high-speed integration caveat |
| 3 | fake6502 | Klaus NMOS | PASS | 481,206,820 | 0.574722 | 837.29 |  |
| 4 | qe6502 | Klaus NMOS | PASS | 481,206,820 | 0.708310 | 679.37 |  |
| 5 | vrEmu6502 | Klaus NMOS | PASS | 481,206,820 | 0.792116 | 607.50 |  |
| 6 | ares | Klaus NMOS | PASS | 482,806,835 | 0.794999 | 607.30 | Reported slightly different cycle total |
| 7 | mame | Klaus NMOS | PASS | 481,250,000 | 0.854248 | 563.36 | MAME adapter reports rounded/different cycle total |
| 8 | altirrasdl | Klaus NMOS | PASS | 481,206,825 | 0.901467 | 533.80 |  |
| 9 | gianlucag/mos6502 | Klaus NMOS | PASS | 481,206,820 | 0.913712 | 526.65 |  |
| 10 | olcNES | Klaus NMOS | PASS | 481,206,805 | 1.124893 | 427.78 |  |
| 11 | floooh/chips | Klaus NMOS | PASS | 481,206,820 | 1.338296 | 359.57 |  |
| 12 | clk | Klaus NMOS | PASS | 481,206,820 | 1.345292 | 357.70 |  |
| 13 | chris-pikul/mos6502 | Klaus NMOS | PASS | 635,779,875 | 1.894789 | 335.54 | Different reported cycle total than the common Klaus-NMOS count |
| 14 | O2 | Klaus NMOS | PASS | 481,206,815 | 1.700880 | 282.92 | Reported five fewer cycles than common Klaus-NMOS count |
| 15 | Peddle | Klaus NMOS | PASS | 481,206,820 | 1.702652 | 282.62 |  |
| 16 | fceux | Klaus NES | PASS | 420,152,240 | 2.426736 | 173.13 | NES Klaus ROM, not full generic NMOS |

Factual note: `fceux` is intentionally listed as a NES/2A03-style result because the printed test is `Klaus NES`, not the generic full-NMOS Klaus path. `sflib6502` and `AppleWin` are the two highest MHz rows and both have memory-model caveats relevant to speed interpretation. `sflib6502` installs callbacks in the benchmark path, but the core still performs important fetch/operand/stack work through internal memory paths. AppleWin's adapter attaches a flat 64K RAM array and stubs/flattens Apple II page/IO structures while the opcode core still fetches operands and stack data directly from `mem`. These are valid benchmark results for those integration shapes, but they are not the same integration model as a fully host-visible per-cycle bus core.

## Klaus 65C02 performance and failures

| Core/model | Test | Status | Cycles | Seconds | MHz |
| --- | --- | --- | --- | --- | --- |
| fake65c02 | Klaus 65C02 ext | FAIL |  |  |  |
| vrEmu6502 WDC 65C02 | Klaus standard | PASS | 482,806,605 | 0.767009 | 629.47 |
| vrEmu6502 WDC 65C02 | Klaus extended | PASS | 334,525,005 | 0.505187 | 662.18 |
| vrEmu6502 RW 65C02 | Klaus standard | PASS | 482,806,605 | 0.763318 | 632.51 |
| vrEmu6502 RW 65C02 | Klaus extended | PASS | 334,525,005 | 0.515308 | 649.17 |
| sflib65c02 | Klaus 65C02 ext | FAIL |  |  |  |
| qe6502 WDC 65C02 | Klaus standard | PASS | 482,806,605 | 0.715223 | 675.04 |
| qe6502 WDC 65C02 | Klaus extended | PASS | 334,535,345 | 0.440143 | 760.06 |
| qe6502 RW 65C02 | Klaus standard | PASS | 482,806,605 | 0.716390 | 673.94 |
| qe6502 RW 65C02 | Klaus extended | PASS | 334,535,345 | 0.441943 | 756.96 |
| clk WDC 65C02 | Klaus standard | PASS | 482,806,605 | 1.383056 | 349.09 |
| clk WDC 65C02 | Klaus extended | PASS | 334,535,345 | 0.940285 | 355.78 |
| clk RW 65C02 | Klaus standard | PASS | 482,806,605 | 1.331417 | 362.63 |
| clk RW 65C02 | Klaus extended | PASS | 334,535,345 | 0.909891 | 367.67 |
| AppleWin WDC 65C02 | Klaus standard | PASS | 482,806,605 | 0.552077 | 874.53 |
| AppleWin WDC 65C02 | Klaus extended | FAIL |  |  |  |
| MAME WDC 65C02 | Klaus standard | PASS | 482,850,000 | 0.851886 | 566.80 |
| MAME WDC 65C02 | Klaus extended | FAIL |  |  |  |
| MAME RW 65C02 | Klaus standard | PASS | 482,850,000 | 0.854423 | 565.12 |
| MAME RW 65C02 | Klaus extended | PASS | 334,550,000 | 0.618970 | 540.49 |
| MAME W65C02S | Klaus standard | PASS | 482,850,000 | 0.867403 | 556.66 |
| MAME W65C02S | Klaus extended | PASS | 334,550,000 | 0.622427 | 537.49 |

Factual note: `vrEmu6502`, `qe6502`, and `clk` have passing WDC and Rockwell standard/extended Klaus entries in the supplied output. AppleWin and MAME both pass WDC standard but fail the WDC extended entry in the printed aggregate; MAME passes Rockwell and W65C02S standard/extended entries. AppleWin's high WDC-standard speed should be read with the same flat-memory extraction caveat noted in the NMOS Klaus table.

## NMOS SingleStep headline results

The NMOS corpus contains `2,560,000` cases: `256` opcode slots with `10,000` cases each. The table below reports failed opcodes, not failed cases.

| Core/model | Cases | Instruction failures | Cycle-count failures | Bus-trace failures |
| --- | --- | --- | --- | --- |
| qe6502 NMOS | 2,560,000 | 0/256 (0.00%); legal 0/151, illegal 0/105 | 0/256 (0.00%); legal 0/151, illegal 0/105 | 0/256 (0.00%); legal 0/151, illegal 0/105 |
| clk P6502 NMOS | 2,560,000 | 0/256 (0.00%); legal 0/151, illegal 0/105 | 0/256 (0.00%); legal 0/151, illegal 0/105 | 0/256 (0.00%); legal 0/151, illegal 0/105 |
| floooh/chips NMOS | 2,560,000 | 9/256 (3.52%); legal 4/151, illegal 5/105 | 0/256 (0.00%); legal 0/151, illegal 0/105 | 7/256 (2.73%); legal 2/151, illegal 5/105 |
| Peddle NMOS | 2,560,000 | 7/256 (2.73%); legal 2/151, illegal 5/105 | 12/256 (4.69%); legal 0/151, illegal 12/105 | 19/256 (7.42%); legal 2/151, illegal 17/105 |
| gianlucag_mos6502 NMOS | 2,560,000 | 6/256 (2.34%); legal 0/151, illegal 6/105 | 12/256 (4.69%); legal 0/151, illegal 12/105 | unsupported |
| mame NMOS 6502 | 2,560,000 | 21/256 (8.20%); legal 6/151, illegal 15/105 | 0/256 (0.00%); legal 0/151, illegal 0/105 | unsupported |
| fake6502 NMOS | 2,560,000 | 31/256 (12.11%); legal 11/151, illegal 20/105 | 12/256 (4.69%); legal 0/151, illegal 12/105 | unsupported |
| applewin NMOS 6502 | 2,560,000 | 30/256 (11.72%); legal 6/151, illegal 24/105 | 12/256 (4.69%); legal 0/151, illegal 12/105 | unsupported |
| vrEmu6502 NMOS | 2,560,000 | 54/256 (21.09%); legal 20/151, illegal 34/105 | 12/256 (4.69%); legal 0/151, illegal 12/105 | unsupported |
| O2 NMOS | 2,560,000 | 66/256 (25.78%); legal 0/151, illegal 66/105 | 76/256 (29.69%); legal 0/151, illegal 76/105 | 93/256 (36.33%); legal 0/151, illegal 93/105 |
| olcNES NMOS | 2,560,000 | 89/256 (34.77%); legal 2/151, illegal 87/105 | 21/256 (8.20%); legal 0/151, illegal 21/105 | unsupported |
| chris-pikul/mos6502 NMOS | 2,560,000 | 90/256 (35.16%); legal 3/151, illegal 87/105 | 250/256 (97.66%); legal 145/151, illegal 105/105 | unsupported |
| sflib6502 NMOS | 2,560,000 | 148/256 (57.81%); legal 43/151, illegal 105/105 | 138/256 (53.91%); legal 53/151, illegal 85/105 | unsupported |
| ares MOS6502 NMOS/BCD | 2,560,000 | 256/256 (100.00%); legal 151/151, illegal 105/105 | 43/256 (16.80%); legal 16/151, illegal 27/105 | unsupported |

Factual note: AltirraSDL is not in this table because the aggregate output explicitly skips its NMOS SingleStep integration: the current adapter was considered too noisy for reliable SingleStep classification. FCEUX is also not in the NMOS SingleStep matrix in the supplied output.

## NMOS SingleStep failed-case totals from detail TSV logs

The headline table above counts failed opcodes. The TSV logs also record failed individual cases within those opcode slots.

| Core | Model | Cases total | Instruction failed opcodes / cases | Cycle failed opcodes / cases | Bus failed opcodes / cases |
| --- | --- | --- | --- | --- | --- |
| clk | P6502 NMOS | 2,560,000 | 0 opcodes / 0 cases | 0 opcodes / 0 cases | 0 opcodes / 0 cases |
| qe6502 | NMOS | 2,560,000 | 0 opcodes / 0 cases | 0 opcodes / 0 cases | 0 opcodes / 0 cases |
| gianlucag_mos6502 | NMOS | 2,560,000 | 6 opcodes / 21,013 cases | 12 opcodes / 120,000 cases | unsupported |
| Peddle | NMOS | 2,560,000 | 7 opcodes / 57,641 cases | 12 opcodes / 120,000 cases | 19 opcodes / 177,641 cases |
| floooh/chips | NMOS | 2,560,000 | 9 opcodes / 58,377 cases | 0 opcodes / 0 cases | 7 opcodes / 44,287 cases |
| mame | NMOS 6502 | 2,560,000 | 21 opcodes / 189,574 cases | 0 opcodes / 0 cases | unsupported |
| applewin | NMOS 6502 | 2,560,000 | 30 opcodes / 125,828 cases | 12 opcodes / 120,000 cases | unsupported |
| fake6502 | NMOS | 2,560,000 | 31 opcodes / 185,384 cases | 12 opcodes / 120,000 cases | unsupported |
| vrEmu6502 | NMOS | 2,560,000 | 54 opcodes / 284,547 cases | 12 opcodes / 120,000 cases | unsupported |
| O2 | NMOS | 2,560,000 | 66 opcodes / 648,723 cases | 76 opcodes / 749,966 cases | 93 opcodes / 924,812 cases |
| olcNES | NMOS | 2,560,000 | 89 opcodes / 875,031 cases | 21 opcodes / 165,013 cases | unsupported |
| chris-pikul/mos6502 | NMOS | 2,560,000 | 90 opcodes / 880,009 cases | 250 opcodes / 2,455,047 cases | unsupported |
| sflib6502 | NMOS | 2,560,000 | 148 opcodes / 1,137,871 cases | 138 opcodes / 1,144,599 cases | unsupported |
| ares | MOS6502 NMOS/BCD | 2,560,000 | 256 opcodes / 2,560,000 cases | 43 opcodes / 274,975 cases | unsupported |

## 65C02 SingleStep headline results

For 65C02, the output uses `functional` and `nop` opcode groups. WDC has `254` populated opcode slots in the supplied run; Rockwell and Synertek/ST have `256` populated opcode slots.

### WDC 65C02

| Core/model | CPU init model | Cases | Instruction failures | Cycle-count failures | Bus-trace failures |
| --- | --- | --- | --- | --- | --- |
| qe6502 WDC 65C02 | qe6502_model_wdc | 2,540,000 | 0/254 (0.00%); functional 0/209, nop 0/45 | 0/254 (0.00%); functional 0/209, nop 0/45 | 0/254 (0.00%); functional 0/209, nop 0/45 |
| clk WDC 65C02 | CPU::MOS6502::PWDC65C02 | 2,540,000 | 3/254 (1.18%); functional 0/209, nop 3/45 | 0/254 (0.00%); functional 0/209, nop 0/45 | 11/254 (4.33%); functional 8/209, nop 3/45 |
| vrEmu6502 WDC 65C02 | CPU_W65C02 | 2,540,000 | 2/254 (0.79%); functional 2/209, nop 0/45 | 18/254 (7.09%); functional 17/209, nop 1/45 | unsupported |
| mame WDC 65C02 | CpuMode::wdc65c02 | 2,540,000 | 38/254 (14.96%); functional 38/209, nop 0/45 | 0/254 (0.00%); functional 0/209, nop 0/45 | unsupported |
| applewin CMOS 65C02/WDC corpus | CpuMode::cmos65c02 | 2,540,000 | 39/254 (15.35%); functional 39/209, nop 0/45 | 34/254 (13.39%); functional 33/209, nop 1/45 | unsupported |
| fake65c02 WDC 65C02 | fake65c02 | 2,540,000 | 25/254 (9.84%); functional 11/209, nop 14/45 | 59/254 (23.23%); functional 22/209, nop 37/45 | unsupported |
| sflib65c02 WDC 65C02 | sflib65c02 | 2,540,000 | 131/254 (51.57%); functional 87/209, nop 44/45 | 133/254 (52.36%); functional 96/209, nop 37/45 | unsupported |

### Rockwell 65C02

| Core/model | CPU init model | Cases | Instruction failures | Cycle-count failures | Bus-trace failures |
| --- | --- | --- | --- | --- | --- |
| qe6502 Rockwell 65C02 | qe6502_model_rw | 2,560,000 | 0/256 (0.00%); functional 0/209, nop 0/47 | 0/256 (0.00%); functional 0/209, nop 0/47 | 0/256 (0.00%); functional 0/209, nop 0/47 |
| clk Rockwell 65C02 | CPU::MOS6502::PRockwell65C02 | 2,560,000 | 3/256 (1.17%); functional 0/209, nop 3/47 | 0/256 (0.00%); functional 0/209, nop 0/47 | 11/256 (4.30%); functional 8/209, nop 3/47 |
| vrEmu6502 Rockwell 65C02 | CPU_R65C02 | 2,560,000 | 4/256 (1.56%); functional 3/209, nop 1/47 | 20/256 (7.81%); functional 17/209, nop 3/47 | unsupported |
| mame Rockwell R65C02 | CpuMode::rockwell65c02 | 2,560,000 | 7/256 (2.73%); functional 6/209, nop 1/47 | 0/256 (0.00%); functional 0/209, nop 0/47 | unsupported |
| fake65c02 Rockwell 65C02 | fake65c02 | 2,560,000 | 27/256 (10.55%); functional 12/209, nop 15/47 | 61/256 (23.83%); functional 22/209, nop 39/47 | unsupported |
| sflib65c02 Rockwell 65C02 | sflib65c02 | 2,560,000 | 131/256 (51.17%); functional 85/209, nop 46/47 | 134/256 (52.34%); functional 96/209, nop 38/47 | unsupported |

### Synertek/ST 65C02

| Core/model | CPU init model | Cases | Instruction failures | Cycle-count failures | Bus-trace failures |
| --- | --- | --- | --- | --- | --- |
| qe6502 Synertek/ST 65C02 | qe6502_model_st | 2,560,000 | 0/256 (0.00%); functional 0/177, nop 0/79 | 0/256 (0.00%); functional 0/177, nop 0/79 | 0/256 (0.00%); functional 0/177, nop 0/79 |
| clk Synertek/ST 65C02 | CPU::MOS6502::PSynertek65C02 | 2,560,000 | 19/256 (7.42%); functional 0/177, nop 19/79 | 0/256 (0.00%); functional 0/177, nop 0/79 | 19/256 (7.42%); functional 8/177, nop 11/79 |
| vrEmu6502 Synertek/ST 65C02 | CPU_65C02 | 2,560,000 | 35/256 (13.67%); functional 2/177, nop 33/79 | 36/256 (14.06%); functional 1/177, nop 35/79 | unsupported |
| fake65c02 Synertek/ST 65C02 | fake65c02 | 2,560,000 | 52/256 (20.31%); functional 11/177, nop 41/79 | 77/256 (30.08%); functional 6/177, nop 71/79 | unsupported |
| sflib65c02 Synertek/ST 65C02 | sflib65c02 | 2,560,000 | 130/256 (50.78%); functional 52/177, nop 78/79 | 134/256 (52.34%); functional 64/177, nop 70/79 | unsupported |

## 65C02 SingleStep failed-case totals from detail TSV logs

| Core | Model | Cases total | Instruction failed opcodes / cases | Cycle failed opcodes / cases | Bus failed opcodes / cases |
| --- | --- | --- | --- | --- | --- |
| applewin | CMOS 65C02/WDC corpus | 2,540,000 | 39 / 314,941 | 34 / 254,640 | unsupported |
| qe6502 | Rockwell 65C02 | 2,560,000 | 0 / 0 | 0 / 0 | 0 / 0 |
| clk | Rockwell 65C02 | 2,560,000 | 3 / 14,839 | 0 / 0 | 11 / 55,339 |
| vrEmu6502 | Rockwell 65C02 | 2,560,000 | 4 / 30,001 | 20 / 114,677 | unsupported |
| fake65c02 | Rockwell 65C02 | 2,560,000 | 27 / 191,123 | 61 / 585,008 | unsupported |
| sflib65c02 | Rockwell 65C02 | 2,560,000 | 131 / 898,953 | 134 / 1,121,917 | unsupported |
| mame | Rockwell R65C02 | 2,560,000 | 7 / 59,754 | 0 / 0 | unsupported |
| qe6502 | Synertek/ST 65C02 | 2,560,000 | 0 / 0 | 0 / 0 | 0 / 0 |
| clk | Synertek/ST 65C02 | 2,560,000 | 19 / 134,965 | 0 / 0 | 19 / 95,319 |
| vrEmu6502 | Synertek/ST 65C02 | 2,560,000 | 35 / 350,000 | 36 / 355,044 | unsupported |
| fake65c02 | Synertek/ST 65C02 | 2,560,000 | 52 / 350,839 | 77 / 705,075 | unsupported |
| sflib65c02 | Synertek/ST 65C02 | 2,560,000 | 130 / 899,297 | 134 / 1,122,295 | unsupported |
| qe6502 | WDC 65C02 | 2,540,000 | 0 / 0 | 0 / 0 | 0 / 0 |
| vrEmu6502 | WDC 65C02 | 2,540,000 | 2 / 20,000 | 18 / 95,055 | unsupported |
| clk | WDC 65C02 | 2,540,000 | 3 / 14,841 | 0 / 0 | 11 / 54,980 |
| fake65c02 | WDC 65C02 | 2,540,000 | 25 / 181,160 | 59 / 565,151 | unsupported |
| mame | WDC 65C02 | 2,540,000 | 38 / 357,660 | 0 / 0 | unsupported |
| sflib65c02 | WDC 65C02 | 2,540,000 | 131 / 879,434 | 133 / 1,111,804 | unsupported |
| mame | WDC W65C02S | 2,540,000 | 8 / 59,822 | 0 / 0 | unsupported |

## Supported validation levels and integration facts

This table is factual, based on the current benchmark runners, methodology file, wrappers, and observed output.

| Core/family | Models represented in this benchmark snapshot | Instruction | Cycle count | Bus trace | Integration fact |
| --- | --- | --- | --- | --- | --- |
| qe6502 | NMOS, WDC 65C02, Rockwell 65C02, Synertek/ST 65C02; NES/2A03-style mode exists in project context | yes | yes | yes | Small explicit C API plus C++ wrapper; host-driven tick/bus state and model selection are exposed. |
| clk / Clock Signal | NMOS P6502, WDC 65C02, Rockwell 65C02, Synertek/ST 65C02; PNES6502 exists but not used in main NES policy | yes | yes | yes | C++ template/personality CPU with host bus handler; reusable CPU-component extraction. |
| floooh/chips | NMOS tested; vendored core also indicates 6510 and NES-like capability | yes | yes | yes | Pin/tick API exposes address/data/RW pins closely enough for bus-trace comparison. |
| Peddle | NMOS tested; vendored project also indicates 6502/6507/6510/8502-style support | yes | yes | yes | C++ bus abstraction; host serves memory through read/write methods. |
| O2 | NMOS only in this benchmark | yes | yes | yes | Host read/write functions; bus-trace attempted and measured, with many illegal-opcode/timing mismatches. |
| vrEmu6502 | NMOS, WDC 65C02, Rockwell 65C02, generic CPU_65C02 mapped to Synertek/ST best-fit | yes | yes | no | C API with callbacks and runtime model selection; tick path does not expose an authoritative per-cycle bus stream in this harness. |
| MAME adapter | NMOS, WDC 65C02, Rockwell R65C02, WDC W65C02S | yes | yes | no | Generated opcode/state implementation adapted outside full MAME device framework; the adapter owns its test memory and does not expose a host bus callback stream. |
| AppleWin adapter | NMOS, generic CMOS 65C02 mapped to WDC for aggregate tests | yes | yes | no | Opcode implementation headers wrapped; `attach_memory()` points AppleWin memory globals at flat 64K RAM. Fetch/operand/stack paths still use direct `mem[...]`/page-macro access, while Apple II page/IO structures are stubbed or flattened for tests. |
| fake6502 | NMOS | yes | yes | no | Small C core; global/static-style state and no full bus trace. |
| fake65c02 | WDC/Rockwell/Synertek corpora tested as fake65c02 | yes | yes | no | Small C-style 65C02 candidate; extended Klaus fails in current data. |
| gianlucag/mos6502 | NMOS | yes | yes | no | Host callbacks and optional illegal opcode table; no real per-cycle bus stream. |
| sflib6502 | NMOS | yes | yes | no | Callbacks are installed in the benchmark paths, but fetch/operand/stack code still uses internal `memory[...]` paths; no authoritative bus event stream is exposed. |
| sflib65c02 | WDC/Rockwell/Synertek corpora tested as sflib65c02 | yes | yes | no | Same host-memory caveats as sflib6502; extended 65C02 coverage is weak. |
| olcNES | NMOS in current aggregate | yes | yes | no | Educational/tutorial-style CPU with Bus object; whole-instruction style for this harness. |
| chris-pikul/mos6502 | NMOS | yes | yes | no | C++ bus/device abstraction; current path gives poor cycle-count fit. |
| Ares | NMOS; BCD toggle present, no extracted 65C02 model in current integration | yes | yes | no | MOS6502 component extracted through shim; framework assumptions remain. |
| AltirraSDL / ATCPU | NMOS Klaus; SingleStep excluded in final matrix | not scored | not scored | not scored | Coprocessor/scheduler style integration; the supplied output marks SingleStep as intentionally excluded because the adapter was too noisy. |
| FCEUX | NES/2A03-style Klaus only | not in matrix | not in matrix | not in matrix | NES emulator CPU with globals/stubs; listed separately from full generic NMOS candidates. |

## Integration/adoption implications visible in the current code

This section is still factual: it describes the integration surface visible in the benchmark wrappers and vendored core APIs. It does not score whether an integration style is good or bad by itself. The practical implication is that two cores with similar Klaus speeds may require very different work from a machine emulator. A cycle-by-cycle API is not inherently more complex than an instruction-level API; it is a different contract in which the host wires the CPU to memory and I/O every cycle rather than handing the core a private flat RAM image.

### Integration shape by family

| Integration shape | Cores/families in this snapshot | What the code exposes | Practical implication for adoption |
| --- | --- | --- | --- |
| Host-driven tick/pin bus | `qe6502`, `clk`, `floooh/chips` | The host supplies or observes bus activity at the CPU cycle/pin level. The wrappers build SingleStep bus traces directly from this interface. | Best fit when a machine emulator needs memory-mapped IO, bank switching, wait-state-like control, or exact bus logging. Adoption work is mainly bus wiring: the host participates every cycle rather than calling a single opaque "execute one instruction" function. `qe6502` exposes this through a small C API, `floooh/chips` through a C-style pin API, and `clk` through a C++ bus-handler/personality interface. |
| C++ bus abstraction with cycle stepping | `Peddle`, `O2` | Memory is served through host read/write methods or function callbacks, and the wrapper can step/count cycles. | More integration-friendly than flat-memory engines for bus-aware machines, but exactness depends on how faithfully the core sequences reads/writes and exposes internal state. Current results show remaining cycle/bus mismatches. |
| Instruction-step callback cores | `vrEmu6502`, `gianlucag/mos6502`, `fake6502`, `fake65c02` | The host can usually provide memory callbacks or a memory array, but the benchmark advances by whole instruction or instruction-count style APIs. | Usually easy to embed for functional emulation. Less suitable as a drop-in exact bus component because the wrapper cannot emit an authoritative per-cycle bus trace. Some wrappers also need internal-state access not exposed by the public API. |
| Direct/flat-memory emulator engines | `AppleWin`, `sflib6502`, `sflib65c02`, current `MAME` adapter | The CPU works against an internal or attached 64K memory image. Some callbacks may exist, but not all fetch/operand/stack paths are host-observable. | Often fast and practical for the emulator shape they came from. Speed and Klaus results should be read as flat-memory/direct-memory integration results, not as evidence of a generic host-observable bus component. |
| Framework or machine extractions | `MAME`, `AppleWin`, `AltirraSDL`, `FCEUX`, `Ares` | The CPU was extracted or adapted from a larger emulator framework with generated code, global machine state, scheduler concepts, memory maps, or machine-specific stubs. | Valuable reference code and often mature behavior, but adoption can require shims, generated-code understanding, framework pruning, or accepting machine-specific assumptions. |

### Per-core adoption notes from the wrappers

| Core/family | Integration surface seen in this benchmark | What an adopter should expect |
| --- | --- | --- |
| `qe6502` | Explicit C API, plus C++ wrapper, for a host-driven tick model with visible address, data, read/write state, opcode-fetch state, and model selection. | Wire the core to the host bus per cycle: read the current request/address/RW/data state, provide input data for reads, and observe writes. The surface is direct and small; compared with instruction-call cores, the key difference is the per-cycle host participation. The plain C boundary is also a practical advantage for FFI bindings. |
| `clk` | C++ template/personality CPU with a `BusHandler` that performs reads/writes and returns consumed cycles. | Reusable CPU-component shape with host-visible bus handling. Adoption requires C++ template/personality integration and a bus-handler implementation; FFI use normally needs an additional C-compatible wrapper layer. |
| `floooh/chips` | Pin-mask `m6502_tick()` API; the wrapper decodes address/data/RW/SYNC pins. | Low-level but transparent. Good fit for host-driven buses; adopters must be comfortable with pin-mask style integration. |
| `Peddle` | C++ subclass/override pattern for memory reads/writes and disassembly reads. | Reasonably direct bus abstraction. Current data shows small instruction failures plus cycle/bus mismatches, so exact bus use still needs verification. |
| `O2` | Constructor callbacks for read/write plus a cycle path; the benchmark wrapper uses subclass access to reset internal state. | Small and readable. Legal documented NMOS behavior is strong in the summary, but undocumented and timing/bus behavior dominate failures. |
| `vrEmu6502` | C API with memory callbacks and runtime CPU model selection; the benchmark advances by instruction-cycle API and duplicates internal state layout for full test setup. | Practical multi-model embedding, but the public API is less convenient for exhaustive state setup and does not expose a bus-trace stream. |
| `MAME` | Generated/device-style implementation adapted into a standalone harness with adapter-owned memory. | Broad model coverage and reference value, but not lightweight. Current integration does not expose a host-observable bus stream. |
| `AppleWin` | Apple II emulator CPU extraction; wrapper attaches a flat 64K RAM image, while opcode headers still use direct `mem[...]`, page macros, stack macros, and stubbed Apple II IO/page structures. | Very fast and useful as an emulator extraction. Adoption outside an Apple II-like or flat-memory shape requires accepting or reworking machine-specific memory assumptions. |
| `fake6502` / `fake65c02` | Small C cores with global/static-style state and fixed bridge memory. | Easy to compile and useful as baselines, but multi-instance isolation, public-state control, and bus observability are limited. |
| `gianlucag/mos6502` | C++ callbacks for bus read/write plus instruction-count execution. | Compact functional comparator. Good legal-opcode result in this data, but no bus trace and narrower model coverage. |
| `sflib6502` / `sflib65c02` | C library style with callbacks installed by the benchmark, but internal opcode fetch, operand, and stack paths still use direct memory-array access. | Very fast in NMOS Klaus for `sflib6502`, but not a complete host-observable CPU component. Treat as a throughput/flat-memory comparator. |
| `olcNES` | Educational Bus object plus CPU `clock()`/`complete()` pattern; instruction work is largely computed up front and then cycles count down. | Familiar and easy to understand, but not a strong exact bus-integration candidate in this harness. |
| `chris-pikul/mos6502` | Heavier C++ bus/device architecture; wrapper suppresses diagnostics and performs state access through test scaffolding. | Demonstrates a richer object model, but current cycle-count fit and integration burden are poor. |
| `Ares` | Framework MOS6502 component with virtual read/write and dummy-access concepts, wrapped through a shim. | Potentially valuable framework code, but current adapter result is weak and not scored as a bus-trace provider. |
| `AltirraSDL / ATCPU` | Coprocessor/scheduler and page-map style integration. | Useful Klaus-level evidence, but the supplied aggregate output excludes SingleStep because the state/scheduler adapter was too noisy. |
| `FCEUX` | NES/2A03 emulator CPU path with globals and stubs. | Listed as a NES-specific Klaus entry rather than a generic full-NMOS CPU candidate. |

### FFI and language-boundary considerations

This repository is a C/C++ benchmark, but the integration shape also affects adoption from Rust, Zig, C#, Python native extensions, or other hosts. The important distinction is not only whether a core is fast, but where the stable boundary sits.

| FFI/adoption shape | Cores/families | Practical implication |
| --- | --- | --- |
| Small C ABI or C-style surface | `qe6502`, `floooh/chips`, `vrEmu6502`, `fake6502`, `fake65c02`, `sflib6502`/`sflib65c02` | Usually the most straightforward group for non-C++ bindings. The remaining question is what the API exposes: `qe6502` and `floooh/chips` expose cycle/pin-visible operation, while `sflib6502`/AppleWin-style direct-memory extraction paths do not expose a complete bus-event stream. |
| C++ CPU-component surface | `clk`, `Peddle`, `O2`, `gianlucag/mos6502`, `chris-pikul/mos6502` | Practical in C++ projects. FFI users typically need a C wrapper around construction, stepping, state access, and callbacks/bus handlers. |
| Framework or machine extraction | `MAME`, `AppleWin`, `Ares`, `AltirraSDL`, `FCEUX` | Useful reference implementations, but non-C++ or standalone adoption may require isolating generated code, globals, scheduler assumptions, machine memory maps, or emulator-specific stubs before an FFI boundary is straightforward. |

Factual note: a plain C ABI is not automatically a better emulator contract than a C++ API, and a C++ API is not automatically less accurate. The distinction is practical: C ABIs are generally easier to bind across languages, while bus-observable APIs are generally more informative for machine emulators that need exact memory and I/O participation.

## Objective observations from the data

- `qe6502` and `clk` both show `0/256` failed NMOS opcodes at instruction, cycle-count, and bus-trace levels in the supplied full NMOS corpus results.
- `qe6502` shows `0` failed opcodes and `0` failed cases across WDC, Rockwell, and Synertek/ST 65C02 instruction, cycle-count, and bus-trace detail logs.
- `clk` shows zero 65C02 cycle-count failures across WDC, Rockwell, and Synertek/ST. Its 65C02 instruction failures are confined to NOP-space rows in the supplied detail logs.
- `floooh/chips` shows perfect NMOS cycle-count results and a small number of instruction/bus-trace opcode failures.
- `Peddle` and `O2` are bus-trace-capable in the NMOS matrix, but both have nonzero bus-trace failures.
- `MAME` shows zero cycle-count failures in its SingleStep rows, but the current adapter does not report bus-trace support.
- `vrEmu6502` is the best-scoring non-bus-trace 65C02 candidate in the data: WDC has `2/254` instruction-failed opcodes and Rockwell has `4/256`; Synertek/ST has many more total failures because many NOP-space rows differ.
- `sflib6502` is the fastest Klaus-NMOS row but has broad SingleStep failures and incomplete host-memory visibility in the methodology/code notes.
- `AppleWin` is the second-fastest Klaus-NMOS row and a very fast WDC-standard 65C02 row, but its benchmark path is also a direct flat-memory emulator extraction rather than a generic per-cycle host-bus component.
- `Ares` passes Klaus NMOS but currently reports `256/256` NMOS SingleStep instruction failures in the detail log, so the current SingleStep adapter/result should be interpreted separately from its Klaus result.
- `FCEUX` is a NES/2A03-style candidate in the supplied results and is not directly comparable to full-NMOS candidates on decimal-mode coverage.

# Part II — Interpretation

## How the data should be read

The supplied benchmark data is not a single speed contest. It measures several separable properties:

| Dimension | What a strong result looks like |
| --- | --- |
| Functional execution | Smoke and Klaus pass; low SingleStep instruction-failure count. |
| Cycle accuracy | Low or zero SingleStep cycle-count failures. |
| Bus observability | Bus-trace support plus low or zero bus-trace failures. |
| Host integration realism | Memory fetches, data reads/writes, stack/vector paths, dummy accesses, and I/O-relevant behavior can be host-visible. |
| Model coverage | Separate NMOS, WDC, Rockwell, Synertek/ST, W65C02S, or NES/2A03 behavior where applicable. |
| Practical embeddability | CPU can be extracted without heavy emulator framework assumptions. |
| FFI/language-boundary fit | Stable C or wrapper-friendly API surface for projects outside the original implementation language. |
| Throughput | High Klaus MHz under the printed build/runtime environment. |

A core can score strongly in one dimension and weakly in another. For example, `sflib6502` is the fastest measured Klaus-NMOS row but is not a complete bus-observable integration reference, and AppleWin is also very fast while being a direct flat-memory Apple II emulator extraction. Conversely, `qe6502`, `clk`, and `floooh/chips` are not the fastest Klaus rows, but they expose cycle/bus-level integration surfaces that make stronger SingleStep bus analysis possible.

## Main interpretive takeaways

### 1. Two cores define the current exactness ceiling

On the provided data, `qe6502` and `clk` are the only NMOS SingleStep candidates with zero failed opcodes across all three levels. `qe6502` also has zero failed opcodes across all three 65C02 corpora and bus-trace levels. `clk` has very strong 65C02 results: zero cycle-count failures and only NOP-space instruction failures, but nonzero bus-trace mismatches remain.

### 2. Bus-trace support separates hardware-style cores from whole-instruction cores

`qe6502`, `clk`, `floooh/chips`, `Peddle`, and `O2` are the key bus-trace-capable candidates in the current data. This does not automatically make them better for every use case, but it does make them more informative for host-visible machine emulation, memory-mapped I/O, bank switching, and cycle-by-cycle diagnostics.

### 3. Klaus PASS is useful but not sufficient for exactness claims

Every full-NMOS Klaus row in the supplied table passes, but SingleStep results vary widely. This is visible in `sflib6502`, `chris-pikul/mos6502`, `olcNES`, `O2`, `Ares`, and others. The correct interpretation is not that Klaus is unimportant; it is that Klaus and SingleStep answer different questions.

### 4. 65C02 model identity matters

The 65C02 results show why a generic `65C02` label is not enough. WDC, Rockwell, Synertek/ST, and W65C02S differ in opcode maps and NOP behavior. `qe6502`, `clk`, `vrEmu6502`, and `MAME` expose multiple useful model paths. AppleWin exposes a generic CMOS path that was mapped to WDC for aggregate reporting because its overall WDC opcode/NOP fit is better in the current result data.

### 5. Speed is meaningful only with the integration model attached

The raw Klaus MHz table is important, but the memory model changes what that number means. A core that hides memory internally may avoid host-callback or bus-observation overhead that a host-driven bus core pays. The clearest examples in this data are `sflib6502` and `AppleWin`: both are very fast in Klaus, and both should be read together with their direct-memory integration caveats rather than compared naively against fully host-mediated per-cycle bus cores.

### 6. Framework-heavy cores are still valuable

MAME, AltirraSDL, AppleWin, FCEUX, and Ares are not all standalone CPU libraries, but they represent serious emulator codebases. Their lower embeddability score should not be confused with lack of value. MAME in particular is valuable because of its model breadth.

## Core-by-core practical profile

| Core | Best use shown by this data | Main limitations shown by this data |
| --- | --- | --- |
| `qe6502` | Exact bus-visible NMOS/65C02 candidate with strong speed, explicit models, a small C API, C++ wrapper, and direct host-driven bus operation. | Not the top raw Klaus MHz row; not an opaque instruction-step drop-in because the host participates in each bus cycle. |
| `clk` | Strong reusable C++ CPU-component candidate with exact NMOS SingleStep behavior and strong 65C02 cycle/model behavior. | 65C02 NOP-space and bus-trace mismatches remain; performance is moderate; non-C++/FFI adoption needs an additional wrapper layer. |
| `floooh/chips` | Strong NMOS pin/tick bus-level comparison. | No 65C02 path in the benchmark; a few NMOS instruction/bus mismatches. |
| `vrEmu6502` | Practical multi-model C core with strong Klaus and good WDC/Rockwell 65C02 SingleStep fit. | No bus trace; API ergonomics require adapter work. |
| `MAME` | Broad CPU-family coverage and valuable WDC/Rockwell/W65C02S modes. | Framework-coupled/generated adapter; current adapter owns test memory and has no host bus trace. |
| `AppleWin` | Fast practical NMOS/CMOS emulator CPU extraction. | Generic CMOS best-fit mapping; direct flat-memory Apple II extraction; no general host bus hook model. |
| `Peddle` | C++ bus-abstraction comparator with bus-trace support. | Slower and not exact in cycle/bus traces. |
| `fake6502` | Very fast, simple NMOS speed baseline. | No bus trace; global/static style; SingleStep mismatch count is moderate. |
| `gianlucag/mos6502` | Compact NMOS functional comparator with few instruction failures. | No bus trace; limited model coverage; cycle mismatches on KIL/JAM slots. |
| `O2` | Small readable NMOS core; legal documented behavior looks good after patches. | Undocumented/timing/bus mismatches are high; lower speed. |
| `sflib6502` | Raw throughput baseline. | Incomplete host-memory visibility, no authoritative bus event stream, and broad SingleStep failures. |
| `fake65c02` | Simple partial 65C02 baseline. | Extended Klaus fails and SingleStep differences remain. |
| `olcNES` | Familiar educational NMOS comparator. | No bus trace; undocumented opcode behavior diverges strongly. |
| `Ares` | Serious NMOS component extracted from a larger framework. | Current SingleStep result is weak; no bus trace; shim/framework burden. |
| `AltirraSDL` | Klaus-level serious emulator CPU candidate. | SingleStep excluded because adapter integration is too noisy. |
| `FCEUX` | NES/2A03-specialized comparison. | Not a generic full-NMOS candidate; no SingleStep matrix row. |
| `sflib65c02` | Weak/negative 65C02 comparison case. | Extended Klaus fails; broad SingleStep failures; memory visibility caveat. |
| `chris-pikul/mos6502` | Heavier C++ design comparison that can pass Klaus. | Very weak cycle-count fit and heavy integration/diagnostic burden. |

# Part III — Overall ranking

The following ranking is a qualitative overall suitability ranking for this benchmark's goals: correctness-test fit, cycle behavior, bus visibility, host integration realism, model coverage, portability, and speed. It is not a pure MHz ranking and it is not a popularity ranking.

| Rank | Core | Pros | Cons | Important to know |
| --- | --- | --- | --- | --- |
| 1 | qe6502 | Broadest exact result set in this data: zero NMOS and 65C02 SingleStep failures at instruction, cycle-count, and bus-trace levels; high Klaus speed; explicit model selection; small C API plus C++ wrapper; host-visible bus operation. | Not the top raw MHz in the Klaus table; the API is cycle/bus-oriented rather than instruction-oriented, so the host must wire memory and I/O into each tick. | Ranked first here because it combines exact NMOS/65C02 SingleStep results, bus-trace support, high throughput, explicit model coverage, and an FFI-friendly C boundary. |
| 2 | clk / Clock Signal | Strong reusable C++ CPU-component shape; zero NMOS SingleStep failures; direct WDC/Rockwell/Synertek personalities; 65C02 cycle-count failures are zero. | Requires adopting its C++ personality/bus-handler model; FFI use needs an extra C-compatible wrapper; 65C02 instruction mismatches are mostly NOP-space; bus-trace mismatches remain in WDC/Rockwell/Synertek; raw speed is mid-pack. | Best C++ personality/bus-handler fit in this ranking; strongest when that interface style matches the target project. |
| 3 | floooh/chips | Genuine pin/tick model, perfect NMOS cycle-count result, small SingleStep mismatch set, and strong host-visible bus behavior. | No 65C02 coverage in this benchmark; a few BRK/PHP/stack-status and unstable undocumented-store mismatches. | Strong NMOS bus-level comparison core. |
| 4 | vrEmu6502 | Broad useful model support, strong Klaus NMOS/65C02 results, and good WDC/Rockwell 65C02 instruction-level fit. | No bus-trace support in this harness; CPU_65C02/Synertek mapping has many NOP mismatches; API ergonomics have state/callback-userdata limitations. | Strong practical multi-model C candidate. |
| 5 | MAME adapter | Broadest 65C02-family coverage among the tested adapters; zero cycle-count failures in the SingleStep rows where tested; valuable WDC/Rockwell/W65C02S modes. | Framework-coupled and adapter-heavy; no bus trace; WDC 65C02 instruction mismatches are significant. | High-coverage reference-style candidate, not lightweight. |
| 6 | AppleWin adapter | Very fast in Klaus; useful NMOS and CMOS 65C02 coverage; practical after extraction and flat-memory cleanup. | No true host-observable bus model; direct flat-memory Apple II extraction; generic CMOS mode is best-fit rather than explicit WDC/Rockwell/ST; WDC extended Klaus fails. | Treat speed as flat-memory emulator-extraction speed, not as a generic bus-core result. |
| 7 | Peddle | Host-visible C++ bus abstraction, bus-trace-capable in NMOS matrix, low instruction failure count, and exact common Klaus cycle total. | Slower Klaus result; cycle/bus mismatches remain; heavier object model than small C cores. | Good integration-model comparator. |
| 8 | fake6502 | Very fast NMOS Klaus result; simple C baseline; passes smoke and Klaus. | Global/static style, no bus trace, and moderate NMOS SingleStep instruction failures including decimal/status and undocumented areas. | Useful speed baseline with limitations. |
| 9 | gianlucag/mos6502 | Low NMOS instruction-failure count and strong legal-opcode instruction result; reasonable Klaus speed. | No bus trace; illegal-opcode cycle-count failures; narrower model coverage. | Good compact NMOS functional comparator. |
| 10 | O2 | Small and readable; legal NMOS instruction/cycle/bus columns have zero failures in the SingleStep summary; supports bus-trace measurement. | Undocumented opcode behavior dominates failures; Klaus speed is low; decimal-mode behavior needed benchmark-tree attention. | Good documented-NMOS comparator, weak undocumented/timing comparator. |
| 11 | sflib6502 | Fastest measured Klaus-NMOS MHz and useful as a throughput baseline. | Poor SingleStep fit; no bus trace; callbacks do not cover all fetch/operand/stack paths, so speed is not directly comparable to fully host-mediated cores. | Fast but heavily caveated. |
| 12 | fake65c02 | Easy 65C02 comparison candidate with materially better 65C02 SingleStep results than sflib65c02. | Klaus extended fails; no bus trace; NOP/opcode-table behavior differs across corpora. | Useful partial CMOS baseline. |
| 13 | olcNES | Recognizable educational comparator; Klaus NMOS passes; documented legal-opcode SingleStep failures are low compared with its total. | No bus trace; many undocumented-opcode mismatches; limited model coverage. | Useful context, not a strong exactness/integration candidate. |
| 14 | Ares | Klaus NMOS passes and the extracted MOS6502 component is credible after shim work. | Current SingleStep row reports 256/256 instruction failures; no bus trace; framework assumptions make standalone extraction heavier. | Potentially valuable, but current adapter result is weak. |
| 15 | AltirraSDL / ATCPU | Klaus NMOS passes and the underlying emulator CPU is capable. | SingleStep excluded because adapter/state/scheduler integration was too noisy; heavy framework concepts. | Keep for Klaus-level data, not current SingleStep scoring. |
| 16 | FCEUX | Useful NES/2A03-style benchmark entry; correct to keep it separate from full generic NMOS. | Slowest Klaus row in the current table; no SingleStep matrix data; emulator-global integration. | Special-purpose NES candidate. |
| 17 | sflib65c02 | Shows the behavior of a weak/partial 65C02 candidate and remains useful as negative evidence. | Klaus extended fails; 130+ failed instruction opcodes across 65C02 corpora; no bus trace and same memory visibility caveat as sflib6502. | Weak 65C02 candidate. |
| 18 | chris-pikul/mos6502 | Klaus NMOS passes and it represents a heavier C++ bus/device design. | Very poor cycle-count SingleStep result, high adapter/diagnostic burden visible in the benchmark scaffolding, and no bus trace. | Least compelling current candidate overall. |

## Ranking summary by tier

| Tier | Cores | Summary |
| --- | --- | --- |
| Top overall | `qe6502`, `clk` | Exact or near-exact across the most important dimensions; strongest combined accuracy/testability evidence for bus-aware emulator work. |
| Strong candidates | `floooh/chips`, `vrEmu6502`, `MAME`, `AppleWin` | Each has a clear major strength: bus-level NMOS, multi-model C portability, model breadth, or raw practical speed with integration caveats. |
| Useful comparators | `Peddle`, `fake6502`, `gianlucag/mos6502`, `O2`, `sflib6502`, `fake65c02` | Valuable for specific tradeoffs such as speed, bus abstraction, documented-opcode behavior, or partial CMOS coverage. |
| Caveated / niche | `olcNES`, `Ares`, `AltirraSDL`, `FCEUX`, `sflib65c02`, `chris-pikul/mos6502` | Still useful data points, but current integration/test-fit limits are substantial. |

## Appendix A — Failed-opcode highlights for quick diagnosis

This appendix intentionally lists patterns rather than every opcode for broad-failure cores. The full authoritative data is in the TSV detail logs.

| Core/model | Notable detail-log pattern |
| --- | --- |
| qe6502 NMOS / 65C02 | No failed opcodes in the provided SingleStep detail logs. |
| clk NMOS | No failed opcodes. |
| clk 65C02 WDC/Rockwell | Instruction failures are NOP opcodes 0x5C, 0xDC, 0xFC; cycle-count failures are zero; bus failures include those NOPs plus selected functional bus-timing rows such as 0x7C, STA indexed forms, DEC/INC absolute,X, and SBC immediate. |
| clk 65C02 Synertek/ST | Instruction failures are all NOP-space opcodes; cycle-count failures are zero; bus failures mix NOP-space rows with the same small functional bus-timing cluster. |
| floooh/chips NMOS | Instruction failures: BRK, PHP, PLP, RTI, and undocumented/unstable store-family opcodes 0x93, 0x9B, 0x9C, 0x9E, 0x9F. Cycle-count failures: none. |
| Peddle NMOS | Instruction failures: BRK, PHP, and the same undocumented/unstable store-family opcodes 0x93, 0x9B, 0x9C, 0x9E, 0x9F. Cycle-count failures are the KIL/JAM opcode slots 0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x92, 0xB2, 0xD2, 0xF2. |
| gianlucag/mos6502 NMOS | Instruction failures: 0x6B and undocumented/unstable store-family opcodes 0x93, 0x9B, 0x9C, 0x9E, 0x9F. Cycle-count failures are the KIL/JAM opcode slots. |
| vrEmu6502 65C02 WDC | Instruction failures: PLP and RTI. Cycle-count failures are mostly undefined/multi-byte NOP-like opcode slots such as 0x0F, 0x1F, 0x2F, 0x3C, 0x3F, 0x4F, 0x5C, and related xF rows. |
| vrEmu6502 65C02 Rockwell | Instruction failures: JSR with one failed case, PLP, RTI, and 0xDB NOP. |
| vrEmu6502 65C02 Synertek/ST | Instruction failures are mostly NOP-space rows, plus PLP and RTI. This matches the report note that CPU_65C02 was selected for Synertek/ST by functional behavior despite NOP mismatches. |
| MAME W65C02S | Instruction failures: BRK, PHP, PLP, RTI, CLI, SEI, DEC absolute,X, and INC absolute,X; cycle-count failures are zero. |
| AppleWin CMOS/WDC | Instruction failures are all functional, with zero NOP failures in the WDC mapping; WDC extended Klaus fails in the current aggregate output. The adapter is a flat-memory Apple II extraction, so this row should not be read as a generic WDC bus-core result. |
| sflib6502 / sflib65c02 | Failure sets are broad; the headline counts and failed-case totals are more informative than enumerating every opcode in the README. |
| Ares NMOS | Current detail log marks every opcode as instruction-failed, while Klaus NMOS passes. This is recorded as current adapter/test-fit data rather than an upstream correctness conclusion. |

## Appendix B — Suggested reading path for users

- To compare **speed**, start with the Klaus tables, then read the integration fact for each high-MHz core.
- To compare **CPU-state exactness**, read the SingleStep instruction columns.
- To compare **cycle timing**, read the cycle-count columns and remember that unsupported bus trace is not a failure.
- To compare **machine-emulator suitability**, prioritize bus-trace support and host-visible memory facts.
- To compare **65C02 behavior**, read WDC, Rockwell, and Synertek/ST separately; do not collapse them into one generic CMOS score.

The most compact way to inspect a specific core is: Klaus row -> SingleStep headline row -> failed-case detail row -> core-by-core practical profile -> ranking comment.

# Appendix C — Interrupt test results added after the main evaluation

This appendix summarizes the interrupt-focused results that were added after the main smoke, Klaus, SingleStep, speed, and adoption sections above. It is intentionally a separate appendix so that the earlier analysis and overall ranking remain readable as originally written. The full interrupt deep-dive and adapter notes are preserved in [`results/interrupts_results/INTERRUPT_ADAPTER_NOTES.md`](results/interrupts_results/INTERRUPT_ADAPTER_NOTES.md); this README section keeps only the most important conclusions.

## What the interrupt tests measure

The interrupt tests are split into two families with different goals.

| Family | Scope | What a pass means | What it does not prove |
| --- | --- | --- | --- |
| **Basic instruction-level tests** | All integrated `IInstructionCpu` adapters. Group1 has 19 tests, group2 has 13, group3 has 4. The opcode sweep has 1536 rows, with 114 intentionally skipped KIL/JAM or unstable NMOS rows. | The adapter/core handles reset-vector state, IRQ masking, NMI acceptance, stack frames, pushed flags, BRK/RTI behavior, interrupt vectors, held-line duration, priority, and post-opcode IRQ/NMI probes at instruction boundaries. | Cycle-exact bus timing, BRK/NMI hijack windows, or every transistor-level interrupt edge case. |
| **Precision bus/lockstep tests** | Cycle/bus-visible interrupt candidates: `qe6502`, `floooh/chips`, `CLK`, `O2`, and `Peddle`. The suites include I-flag windows, interrupt arbitration, and opcode pulse sweeps. | The target matches the reference public bus stream for the tested edge cases. The primary comparison is read/write direction, address, and data. | Applicability to instruction-only cores. Precision failures can still include adapter boundary issues, not only upstream CPU design errors. |

For the precision tests, opcode-fetch tags and visible IRQ/NMI line annotations are tracked as **secondary metadata**. If the public bus tuple matches but only a `FETCH` tag differs, that is reported separately and should not be confused with a wrong vector, wrong address, wrong data byte, or extra/missing interrupt sequence.

The opcode sweep also has an important interpretation rule: a `testcase_precondition` failure usually means the core did not execute the selected NMOS opcode testcase in the expected way before the interrupt probe. That is useful diagnostic data, but it is not the same thing as a direct IRQ/NMI failure.

## Basic instruction-level interrupt overview

| Core | Grouped basic tests | Basic opcode sweep | Interrupt-specific reading |
| --- | ---: | ---: | --- |
| `qe6502` | 19/19 + 13/13 + 4/4 | 1422 pass, 0 fail, 114 skip | Clean reference result for the basic suite. |
| `AppleWin` | 19/19 + 13/13 + 4/4 | 1422 pass, 0 fail, 114 skip | Clean instruction-level result; do not read it as a cycle/bus precision claim. |
| `MAME` | 19/19 + 13/13 + 4/4 | 1422 pass, 0 fail, 114 skip | Clean instruction-level result through the current standalone adapter; not a bus-trace target here. |
| `ares` | 19/19 + 13/13 + 4/4 | 1422 pass, 0 fail, 114 skip | Clean instruction-level interrupt adapter result, despite the separate SingleStep caveats discussed earlier. |
| `fake6502` | 19/19 + 13/13 + 4/4 | 1422 pass, 0 fail, 114 skip | Clean after mapping the immediate interrupt API onto deferred instruction-boundary assertions. |
| `FCEUX` | 19/19 + 13/13 + 4/4 | 1422 pass, 0 fail, 114 skip | Clean NES/Ricoh-style instruction-level result; not a generic full-NMOS precision claim. |
| `gianlucag_mos6502` | 19/19 + 13/13 + 4/4 | 1422 pass, 0 fail, 114 skip | Clean instruction-level basic interrupt result. |
| `floooh/chips` | 19/19 + 13/13 + 4/4 | 1410 pass, 12 fail, 114 skip | Grouped and legal-opcode basic interrupt behavior passes. The 12 failures are illegal-opcode synthetic-boundary rows, not broad IRQ/NMI breakage. |
| `CLK P6502` | 18/19 + 13/13 + 4/4 | 1185 pass, 237 fail, 114 skip | Mostly good grouped behavior, but one `IRQ RTI resumes` failure and many post-opcode IRQ probe failures show remaining boundary/acceptance risk. |
| `O2` | 17/19 + 13/13 + 3/4 | 912 pass, 510 fail, 114 skip | Some grouped behavior works, but BRK/RTI, NMI live-I status, short IRQ duration, and many synthetic/post-opcode rows fail. Current interrupt integration is noisy. |
| `Peddle` | 8/19 + 1/13 + 1/4 | 238 pass, 1184 fail, 114 skip | The current interrupt adapter is not a passing implementation of the shared instruction-level contract. Treat these as integration/investigation results. |
| `vrEmu6502` | 19/19 + 13/13 + 4/4 | 1420 pass, 2 fail, 114 skip | Grouped interrupt behavior passes. The only failures are `$6C JMP (abs)` NMOS indirect-wrap testcase-precondition rows. |
| `chris_pikul_mos6502` | 19/19 + 13/13 + 4/4 | 1262 pass, 160 fail, 114 skip | Grouped and legal-opcode interrupt behavior passes. Failures are undocumented NMOS opcode testcase-precondition rows. |
| `olcNES` | 19/19 + 13/13 + 4/4 | 1262 pass, 160 fail, 114 skip | Same pattern as `chris_pikul_mos6502`; legal/basic interrupt behavior passes, undocumented NMOS opcode flow differs. |
| `sflib6502` | 19/19 + 13/13 + 4/4 | 1282 pass, 140 fail, 114 skip | Grouped interrupt behavior passes. Sweep failures are testcase-precondition rows, mostly undocumented opcodes plus `$6C JMP (abs)`. |
| `sflib65c02` | 19/19 + 13/13 + 4/4 | 1282 pass, 140 fail, 114 skip | Grouped interrupt behavior passes, but the current sweep is NMOS-oriented and should be considered diagnostic rather than a 65C02-specific interrupt conformance result. |
| `altirrasdl` | not integrated | not integrated | Excluded from this interrupt matrix because the current wrapper has no clean public IRQ/NMI control surface for the shared adapter contract. |

The clean-pass group for the basic instruction-level interrupt contract is therefore `qe6502`, `AppleWin`, `MAME`, `ares`, `fake6502`, `FCEUX`, and `gianlucag_mos6502`. Several other cores also have clean grouped behavior, but retain opcode/testcase diagnostic failures that should remain visible.

## Precision interrupt overview

Precision interrupt tests are intentionally much stricter and only apply to cores with useful cycle/bus visibility in this benchmark tree.

| Core | I-flag windows | Interrupt arbitration | Pulse sweep | Interpretation |
| --- | ---: | ---: | ---: | --- |
| `qe6502` | 0/210 primary failures, secondary=0 | 0/132 primary failures, secondary=0 | 0/35856 failures | Full pass in the current precision interrupt suite. |
| `floooh/chips` | 0/210 primary failures, secondary=0 | 53/132 primary failures, secondary=0 | 2061/35856 failures | Simple I-flag IRQ windows match, but precision BRK/NMI hijack, NMI priority/lost-window, and illegal-opcode pulse windows diverge. |
| `CLK P6502` | 46/210 primary failures, secondary=68 | 34/132 primary failures, secondary=44 | 3431/35856 failures | Real bus mismatches remain, but many additional differences are lower-severity fetch-tag metadata mismatches. NMI pulse windows and branch opcodes are prominent failure clusters. |
| `O2` | 82/210 primary failures, secondary=39 | 73/132 primary failures, secondary=21 | 23987/35856 failures | Heavy precision divergence. Secondary fetch-tag noise exists but is small compared with primary bus mismatches. |
| `Peddle` | 86/210 primary failures, secondary=0 | 48/132 primary failures, secondary=0 | 19182/35856 failures | Heavy precision divergence with no secondary-metadata explanation in the suites that report secondary columns. |

The v45 pulse-sweep TSV output has a reporting caveat: the header contains secondary metadata columns, but the per-opcode rows do not yet emit those fields. The primary pulse-sweep failure counts above are still usable, but per-opcode secondary pulse metadata should not be relied on until the writer is fixed.

## Per-core interrupt notes

| Core | Most important interrupt conclusion |
| --- | --- |
| `qe6502` | Current reference for both basic instruction-level and precision bus/lockstep interrupt behavior. It passes all grouped, opcode-sweep, I-flag, arbitration, and pulse-window tests in this result set. |
| `AppleWin` | Strong basic instruction-level interrupt result through the current adapter. It remains a flat-memory emulator extraction, so the result should not be upgraded into a bus-cycle claim. |
| `MAME` | Strong basic instruction-level interrupt result. This is valuable because of MAME's CPU-family breadth, but the current adapter is not a host-visible precision bus target. |
| `ares` | Clean basic interrupt adapter result. This is separate from the older SingleStep adapter weaknesses, which should not be mixed into the interrupt appendix. |
| `fake6502` | Clean basic result after the adapter defers immediate native IRQ/NMI entry until `step_instruction()`, matching the shared assertion-duration contract. |
| `FCEUX` | Clean basic result for the NES/Ricoh-style path. Keep it separate from full generic NMOS claims, just as in the Klaus section. |
| `gianlucag_mos6502` | Clean basic instruction-level result with no remaining interrupt-specific caveat in this suite. |
| `floooh/chips` | Better than a raw fail count suggests: grouped basic and legal basic sweep pass. Precision still exposes real public-bus differences in BRK/NMI hijack and nested/lost-NMI timing. |
| `CLK P6502` | A mixed result. The core remains very strong in the non-interrupt SingleStep data above, but interrupt tests reveal post-opcode IRQ and precision boundary issues. Fetch-tag-only mismatches are now separated from real public-bus mismatches. |
| `O2` | Current interrupt integration is not clean. It has some working grouped behavior, but status/BRK/short-duration problems and very broad precision divergence make it experimental for interrupts. |
| `Peddle` | Current interrupt integration is the weakest among the precision candidates. Failures are broad enough that they should be treated as adapter/core-investigation data, not a final upstream-core verdict. |
| `vrEmu6502` | Grouped interrupts pass. The two opcode-sweep failures are `$6C JMP (abs)` NMOS testcase-precondition issues, not direct IRQ/NMI failures. |
| `chris_pikul_mos6502` | Grouped and legal-opcode basic interrupts pass. The remaining sweep failures are undocumented NMOS opcode testcase-flow differences. |
| `olcNES` | Similar to `chris_pikul_mos6502`: legal/basic interrupts pass, undocumented NMOS opcode behavior is not complete. This is not surprising for a NES-style educational core. |
| `sflib6502` | Grouped interrupts pass. The sweep failures are diagnostic opcode-flow differences, mostly undocumented opcodes plus the NMOS `$6C` indirect-wrap testcase. |
| `sflib65c02` | Grouped interrupts pass, but the current interrupt opcode sweep is NMOS-oriented; a future 65C02-specific sweep would be a fairer measure. |
| `altirrasdl` | Not scored for interrupts because a clean public IRQ/NMI adapter surface is missing. This should be treated as not applicable, not as a failed interrupt result. |

## Interrupt-only ranking summary

This ranking is deliberately limited to the interrupt test data above. It is not meant to replace the main overall ranking, which also considers speed, general SingleStep results, bus visibility, model breadth, and adoption tradeoffs.

| Interrupt tier | Cores | Reading |
| --- | --- | --- |
| **Reference pass** | `qe6502` | Passes both the instruction-level basic suite and the precision bus/lockstep interrupt suite. |
| **Clean basic instruction-level pass** | `AppleWin`, `MAME`, `ares`, `fake6502`, `FCEUX`, `gianlucag_mos6502` | Strong practical interrupt behavior at the shared instruction-boundary adapter level. No precision claim unless a separate precision adapter exists. |
| **Clean grouped behavior with opcode/testcase caveats** | `vrEmu6502`, `chris_pikul_mos6502`, `olcNES`, `sflib6502`, `sflib65c02` | IRQ/NMI grouped behavior is good. Reported sweep failures are mostly opcode/testcase-precondition limitations and should not be presented as direct interrupt failures. |
| **Strong basic result but precision edge failures** | `floooh/chips` | Basic behavior is good, including legal opcode sweep rows, but precision tests expose real NMI/BRK arbitration and lost-window differences. |
| **Promising but not clean** | `CLK P6502` | Many basic cases pass and some mismatches are only fetch-tag metadata, but real interrupt-boundary public-bus failures remain. |
| **Noisy / experimental interrupt integrations** | `O2`, `Peddle` | Current results are useful for debugging adapters and core edge behavior, but they are not clean interrupt-conformance passes. |
| **Not applicable** | `altirrasdl` | Excluded until a clean public IRQ/NMI control API is available. |

The main practical takeaway is that raw interrupt fail counts need interpretation. A core with grouped pass plus `testcase_precondition` opcode-sweep failures is in a very different situation from a core with broad grouped failures or precision public-bus divergence. The most important split for users is: instruction-level interrupt behavior, opcode/testcase compatibility, and true precision bus-window behavior are separate questions.


# Appendix D — External project and test-suite links

This section collects external links for the CPU cores and major external test suites referenced in this report. These links are provided for convenience; the benchmark results above are based on the vendored/integrated snapshots in this repository, not necessarily on the current upstream HEAD of each project.

## CPU core / emulator project links

| Core in this report | External project link | Notes |
| --- | --- | --- |
| `qe6502` | [nnqe/qe6502](https://github.com/nnqe/qe6502) | Main project behind this benchmark. |
| `clk` / Clock Signal | [TomHarte/CLK](https://github.com/TomHarte/CLK) | Clock Signal / CLK emulator project. |
| `floooh/chips` | [floooh/chips](https://github.com/floooh/chips) | Standalone C header chip/system emulators; this benchmark uses the `m6502` core. |
| `Peddle` | [dirkwhoffmann/Peddle](https://github.com/dirkwhoffmann/Peddle) | Standalone Peddle repository. See also the [Peddle project page](https://dirkwhoffmann.github.io/Peddle/index.html). |
| `O2` | [ericssonpaul/O2](https://github.com/ericssonpaul/O2) | Cycle-oriented 6502 emulator. |
| `vrEmu6502` | [visrealm/vrEmu6502](https://github.com/visrealm/vrEmu6502) | C99 6502/65C02 emulator library. |
| `MAME` | [mamedev/mame](https://github.com/mamedev/mame) | Multi-system emulation framework. |
| `AppleWin` | [AppleWin/AppleWin](https://github.com/AppleWin/AppleWin) | Apple II emulator for Windows. |
| `fake6502` | [omarandlorraine/fake6502](https://github.com/omarandlorraine/fake6502) | Small C 6502 emulator library. |
| `fake65c02` | [C-Chads/MyLittle6502](https://github.com/C-Chads/MyLittle6502) | Fake6502-derived project including `fake65c02`; archived upstream. |
| `gianlucag_mos6502` | [gianlucag/mos6502](https://github.com/gianlucag/mos6502) | C++ MOS 6502 emulator. |
| `sflib6502` | [ShonFrazier/lib6502](https://github.com/ShonFrazier/lib6502) | lib6502 fork used as the source project for the `sflib6502` comparison. |
| `sflib65c02` | [ShonFrazier/lib6502](https://github.com/ShonFrazier/lib6502) | Same family as `sflib6502`, adapted here as the 65C02 comparison target. |
| `olcNES` | [OneLoneCoder/olcNES](https://github.com/OneLoneCoder/olcNES) | NES emulator tutorial project; this benchmark uses the 6502 CPU component. |
| `chris_pikul_mos6502` | [chris-pikul/mos6502](https://github.com/chris-pikul/mos6502) | C++ MOS 6502 emulator. |
| `ares` | [ares-emulator/ares](https://github.com/ares-emulator/ares) | Multi-system emulator project. |
| `AltirraSDL` / `altirrasdl` | [ilmenit/AltirraSDL](https://github.com/ilmenit/AltirraSDL) | Portable SDL-based Altirra fork used for this comparison. |
| `FCEUX` | [TASEmulators/fceux](https://github.com/TASEmulators/fceux) | NES emulator project. |

## External test-suite links

| Test suite | Link | Used for |
| --- | --- | --- |
| Klaus Dormann 6502/65C02 functional tests | [Klaus2m5/6502_65C02_functional_tests](https://github.com/Klaus2m5/6502_65C02_functional_tests) | Long-running functional ROM tests for 6502 and 65C02 behavior. |
| SingleStepTests / ProcessorTests | [SingleStepTests/ProcessorTests](https://github.com/SingleStepTests/ProcessorTests) | Per-instruction state, cycle-count, and bus-trace testcase corpora used by the SingleStep comparisons. |
| perfect6502 | [mist64/perfect6502](https://github.com/mist64/perfect6502) | Transistor/netlist-style reference used by the precision lockstep interrupt work. |

