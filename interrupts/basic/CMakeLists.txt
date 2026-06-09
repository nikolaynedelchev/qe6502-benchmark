add_executable(qe6502_instruction_cpu_smoke
    qe6502_instruction_cpu_smoke.cpp
)

target_link_libraries(qe6502_instruction_cpu_smoke
    PRIVATE qe6502_toolbox
)

target_compile_features(qe6502_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(qe6502_instruction_cpu_smoke)
endif()

add_library(instruction_interrupt_basic_common
    common/instruction_interrupt_group1.cpp
    common/instruction_interrupt_group2.cpp
    common/instruction_interrupt_group3.cpp
    common/instruction_interrupt_opcode_sweep.cpp
)

target_include_directories(instruction_interrupt_basic_common
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(instruction_interrupt_basic_common
    PUBLIC benchmark6502_common tools6502::tools6502
)

target_compile_features(instruction_interrupt_basic_common PUBLIC cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(instruction_interrupt_basic_common)
endif()

add_executable(qe6502_instruction_interrupt_group1
    qe6502_instruction_interrupt_group1.cpp
)

target_link_libraries(qe6502_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common qe6502_toolbox
)

target_compile_features(qe6502_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(qe6502_instruction_interrupt_group1)
endif()


add_executable(qe6502_instruction_interrupt_group2
    qe6502_instruction_interrupt_group2.cpp
)

target_link_libraries(qe6502_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common qe6502_toolbox
)

target_compile_features(qe6502_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(qe6502_instruction_interrupt_group2)
endif()


add_executable(qe6502_instruction_interrupt_group3
    qe6502_instruction_interrupt_group3.cpp
)

target_link_libraries(qe6502_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common qe6502_toolbox
)

target_compile_features(qe6502_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(qe6502_instruction_interrupt_group3)
endif()

add_executable(qe6502_instruction_interrupt_opcode_sweep
    qe6502_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(qe6502_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common qe6502_toolbox
)

target_compile_features(qe6502_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(qe6502_instruction_interrupt_opcode_sweep)
endif()


add_executable(qe6502_instruction_interrupt_basic
    qe6502_instruction_interrupt_basic.cpp
)

target_link_libraries(qe6502_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common qe6502_toolbox
)

target_compile_features(qe6502_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(qe6502_instruction_interrupt_basic)
endif()


add_executable(floooh_chips_instruction_cpu_smoke
    floooh_chips_instruction_cpu_smoke.cpp
)

target_link_libraries(floooh_chips_instruction_cpu_smoke
    PRIVATE floooh_chips_toolbox
)

target_compile_features(floooh_chips_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(floooh_chips_instruction_cpu_smoke)
endif()

add_executable(floooh_chips_instruction_interrupt_group1
    floooh_chips_instruction_interrupt_group1.cpp
)

target_link_libraries(floooh_chips_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common floooh_chips_toolbox
)

target_compile_features(floooh_chips_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(floooh_chips_instruction_interrupt_group1)
endif()

add_executable(floooh_chips_instruction_interrupt_group2
    floooh_chips_instruction_interrupt_group2.cpp
)

target_link_libraries(floooh_chips_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common floooh_chips_toolbox
)

target_compile_features(floooh_chips_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(floooh_chips_instruction_interrupt_group2)
endif()

add_executable(floooh_chips_instruction_interrupt_group3
    floooh_chips_instruction_interrupt_group3.cpp
)

target_link_libraries(floooh_chips_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common floooh_chips_toolbox
)

target_compile_features(floooh_chips_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(floooh_chips_instruction_interrupt_group3)
endif()

add_executable(floooh_chips_instruction_interrupt_opcode_sweep
    floooh_chips_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(floooh_chips_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common floooh_chips_toolbox
)

target_compile_features(floooh_chips_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(floooh_chips_instruction_interrupt_opcode_sweep)
endif()

add_executable(floooh_chips_instruction_interrupt_basic
    floooh_chips_instruction_interrupt_basic.cpp
)

target_link_libraries(floooh_chips_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common floooh_chips_toolbox
)

target_compile_features(floooh_chips_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(floooh_chips_instruction_interrupt_basic)
endif()


add_executable(fake6502_instruction_cpu_smoke
    fake6502_instruction_cpu_smoke.cpp
)

target_link_libraries(fake6502_instruction_cpu_smoke
    PRIVATE fake6502_toolbox
)

target_compile_features(fake6502_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fake6502_instruction_cpu_smoke)
endif()

add_executable(fake6502_instruction_interrupt_group1
    fake6502_instruction_interrupt_group1.cpp
)

target_link_libraries(fake6502_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common fake6502_toolbox
)

target_compile_features(fake6502_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fake6502_instruction_interrupt_group1)
endif()

add_executable(fake6502_instruction_interrupt_group2
    fake6502_instruction_interrupt_group2.cpp
)

target_link_libraries(fake6502_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common fake6502_toolbox
)

target_compile_features(fake6502_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fake6502_instruction_interrupt_group2)
endif()

add_executable(fake6502_instruction_interrupt_group3
    fake6502_instruction_interrupt_group3.cpp
)

target_link_libraries(fake6502_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common fake6502_toolbox
)

target_compile_features(fake6502_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fake6502_instruction_interrupt_group3)
endif()

add_executable(fake6502_instruction_interrupt_opcode_sweep
    fake6502_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(fake6502_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common fake6502_toolbox
)

target_compile_features(fake6502_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fake6502_instruction_interrupt_opcode_sweep)
endif()

add_executable(fake6502_instruction_interrupt_basic
    fake6502_instruction_interrupt_basic.cpp
)

target_link_libraries(fake6502_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common fake6502_toolbox
)

target_compile_features(fake6502_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fake6502_instruction_interrupt_basic)
endif()

add_executable(applewin_instruction_cpu_smoke
    applewin_instruction_cpu_smoke.cpp
)

target_link_libraries(applewin_instruction_cpu_smoke
    PRIVATE applewin_toolbox
)

target_compile_features(applewin_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(applewin_instruction_cpu_smoke)
endif()

add_executable(applewin_instruction_interrupt_group1
    applewin_instruction_interrupt_group1.cpp
)

target_link_libraries(applewin_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common applewin_toolbox
)

target_compile_features(applewin_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(applewin_instruction_interrupt_group1)
endif()

add_executable(applewin_instruction_interrupt_group2
    applewin_instruction_interrupt_group2.cpp
)

target_link_libraries(applewin_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common applewin_toolbox
)

target_compile_features(applewin_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(applewin_instruction_interrupt_group2)
endif()

add_executable(applewin_instruction_interrupt_group3
    applewin_instruction_interrupt_group3.cpp
)

target_link_libraries(applewin_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common applewin_toolbox
)

target_compile_features(applewin_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(applewin_instruction_interrupt_group3)
endif()

add_executable(applewin_instruction_interrupt_opcode_sweep
    applewin_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(applewin_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common applewin_toolbox
)

target_compile_features(applewin_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(applewin_instruction_interrupt_opcode_sweep)
endif()

add_executable(applewin_instruction_interrupt_basic
    applewin_instruction_interrupt_basic.cpp
)

target_link_libraries(applewin_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common applewin_toolbox
)

target_compile_features(applewin_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(applewin_instruction_interrupt_basic)
endif()

add_executable(mame_instruction_cpu_smoke
    mame_instruction_cpu_smoke.cpp
)

target_link_libraries(mame_instruction_cpu_smoke
    PRIVATE mame_toolbox
)

target_compile_features(mame_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(mame_instruction_cpu_smoke)
endif()

add_executable(mame_instruction_interrupt_group1
    mame_instruction_interrupt_group1.cpp
)

target_link_libraries(mame_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common mame_toolbox
)

target_compile_features(mame_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(mame_instruction_interrupt_group1)
endif()

add_executable(mame_instruction_interrupt_group2
    mame_instruction_interrupt_group2.cpp
)

target_link_libraries(mame_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common mame_toolbox
)

target_compile_features(mame_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(mame_instruction_interrupt_group2)
endif()

add_executable(mame_instruction_interrupt_group3
    mame_instruction_interrupt_group3.cpp
)

target_link_libraries(mame_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common mame_toolbox
)

target_compile_features(mame_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(mame_instruction_interrupt_group3)
endif()

add_executable(mame_instruction_interrupt_opcode_sweep
    mame_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(mame_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common mame_toolbox
)

target_compile_features(mame_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(mame_instruction_interrupt_opcode_sweep)
endif()

add_executable(mame_instruction_interrupt_basic
    mame_instruction_interrupt_basic.cpp
)

target_link_libraries(mame_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common mame_toolbox
)

target_compile_features(mame_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(mame_instruction_interrupt_basic)
endif()

add_executable(clk_instruction_cpu_smoke
    clk_instruction_cpu_smoke.cpp
)

target_link_libraries(clk_instruction_cpu_smoke
    PRIVATE clk_toolbox
)

target_compile_features(clk_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(clk_instruction_cpu_smoke)
endif()

add_executable(clk_instruction_interrupt_group1
    clk_instruction_interrupt_group1.cpp
)

target_link_libraries(clk_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common clk_toolbox
)

target_compile_features(clk_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(clk_instruction_interrupt_group1)
endif()

add_executable(clk_instruction_interrupt_group2
    clk_instruction_interrupt_group2.cpp
)

target_link_libraries(clk_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common clk_toolbox
)

target_compile_features(clk_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(clk_instruction_interrupt_group2)
endif()

add_executable(clk_instruction_interrupt_group3
    clk_instruction_interrupt_group3.cpp
)

target_link_libraries(clk_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common clk_toolbox
)

target_compile_features(clk_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(clk_instruction_interrupt_group3)
endif()

add_executable(clk_instruction_interrupt_opcode_sweep
    clk_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(clk_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common clk_toolbox
)

target_compile_features(clk_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(clk_instruction_interrupt_opcode_sweep)
endif()

add_executable(clk_instruction_interrupt_basic
    clk_instruction_interrupt_basic.cpp
)

target_link_libraries(clk_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common clk_toolbox
)

target_compile_features(clk_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(clk_instruction_interrupt_basic)
endif()

add_executable(peddle_instruction_cpu_smoke
    peddle_instruction_cpu_smoke.cpp
)

target_link_libraries(peddle_instruction_cpu_smoke
    PRIVATE peddle_toolbox
)

target_compile_features(peddle_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(peddle_instruction_cpu_smoke)
endif()

add_executable(peddle_instruction_interrupt_group1
    peddle_instruction_interrupt_group1.cpp
)

target_link_libraries(peddle_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common peddle_toolbox
)

target_compile_features(peddle_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(peddle_instruction_interrupt_group1)
endif()

add_executable(peddle_instruction_interrupt_group2
    peddle_instruction_interrupt_group2.cpp
)

target_link_libraries(peddle_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common peddle_toolbox
)

target_compile_features(peddle_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(peddle_instruction_interrupt_group2)
endif()

add_executable(peddle_instruction_interrupt_group3
    peddle_instruction_interrupt_group3.cpp
)

target_link_libraries(peddle_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common peddle_toolbox
)

target_compile_features(peddle_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(peddle_instruction_interrupt_group3)
endif()

add_executable(peddle_instruction_interrupt_opcode_sweep
    peddle_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(peddle_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common peddle_toolbox
)

target_compile_features(peddle_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(peddle_instruction_interrupt_opcode_sweep)
endif()

add_executable(peddle_instruction_interrupt_basic
    peddle_instruction_interrupt_basic.cpp
)

target_link_libraries(peddle_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common peddle_toolbox
)

target_compile_features(peddle_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(peddle_instruction_interrupt_basic)
endif()

add_executable(o2_instruction_cpu_smoke
    o2_instruction_cpu_smoke.cpp
)

target_link_libraries(o2_instruction_cpu_smoke
    PRIVATE o2_toolbox
)

target_compile_features(o2_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(o2_instruction_cpu_smoke)
endif()

add_executable(o2_instruction_interrupt_group1
    o2_instruction_interrupt_group1.cpp
)

target_link_libraries(o2_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common o2_toolbox
)

target_compile_features(o2_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(o2_instruction_interrupt_group1)
endif()

add_executable(o2_instruction_interrupt_group2
    o2_instruction_interrupt_group2.cpp
)

target_link_libraries(o2_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common o2_toolbox
)

target_compile_features(o2_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(o2_instruction_interrupt_group2)
endif()

add_executable(o2_instruction_interrupt_group3
    o2_instruction_interrupt_group3.cpp
)

target_link_libraries(o2_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common o2_toolbox
)

target_compile_features(o2_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(o2_instruction_interrupt_group3)
endif()

add_executable(o2_instruction_interrupt_opcode_sweep
    o2_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(o2_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common o2_toolbox
)

target_compile_features(o2_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(o2_instruction_interrupt_opcode_sweep)
endif()

add_executable(o2_instruction_interrupt_basic
    o2_instruction_interrupt_basic.cpp
)

target_link_libraries(o2_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common o2_toolbox
)

target_compile_features(o2_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(o2_instruction_interrupt_basic)
endif()

add_executable(vremu6502_instruction_cpu_smoke
    vremu6502_instruction_cpu_smoke.cpp
)

target_link_libraries(vremu6502_instruction_cpu_smoke
    PRIVATE vremu6502_toolbox
)

target_compile_features(vremu6502_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(vremu6502_instruction_cpu_smoke)
endif()

add_executable(vremu6502_instruction_interrupt_group1
    vremu6502_instruction_interrupt_group1.cpp
)

target_link_libraries(vremu6502_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common vremu6502_toolbox
)

target_compile_features(vremu6502_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(vremu6502_instruction_interrupt_group1)
endif()

add_executable(vremu6502_instruction_interrupt_group2
    vremu6502_instruction_interrupt_group2.cpp
)

target_link_libraries(vremu6502_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common vremu6502_toolbox
)

target_compile_features(vremu6502_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(vremu6502_instruction_interrupt_group2)
endif()

add_executable(vremu6502_instruction_interrupt_group3
    vremu6502_instruction_interrupt_group3.cpp
)

target_link_libraries(vremu6502_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common vremu6502_toolbox
)

target_compile_features(vremu6502_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(vremu6502_instruction_interrupt_group3)
endif()

add_executable(vremu6502_instruction_interrupt_opcode_sweep
    vremu6502_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(vremu6502_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common vremu6502_toolbox
)

target_compile_features(vremu6502_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(vremu6502_instruction_interrupt_opcode_sweep)
endif()

add_executable(vremu6502_instruction_interrupt_basic
    vremu6502_instruction_interrupt_basic.cpp
)

target_link_libraries(vremu6502_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common vremu6502_toolbox
)

target_compile_features(vremu6502_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(vremu6502_instruction_interrupt_basic)
endif()


# Additional instruction-level basic interrupt targets.

add_executable(chris_pikul_mos6502_instruction_cpu_smoke
    chris_pikul_mos6502_instruction_cpu_smoke.cpp
)

target_link_libraries(chris_pikul_mos6502_instruction_cpu_smoke
    PRIVATE chris_pikul_mos6502_toolbox
)

target_compile_features(chris_pikul_mos6502_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(chris_pikul_mos6502_instruction_cpu_smoke)
endif()

add_executable(chris_pikul_mos6502_instruction_interrupt_group1
    chris_pikul_mos6502_instruction_interrupt_group1.cpp
)

target_link_libraries(chris_pikul_mos6502_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common chris_pikul_mos6502_toolbox
)

target_compile_features(chris_pikul_mos6502_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(chris_pikul_mos6502_instruction_interrupt_group1)
endif()

add_executable(chris_pikul_mos6502_instruction_interrupt_group2
    chris_pikul_mos6502_instruction_interrupt_group2.cpp
)

target_link_libraries(chris_pikul_mos6502_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common chris_pikul_mos6502_toolbox
)

target_compile_features(chris_pikul_mos6502_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(chris_pikul_mos6502_instruction_interrupt_group2)
endif()

add_executable(chris_pikul_mos6502_instruction_interrupt_group3
    chris_pikul_mos6502_instruction_interrupt_group3.cpp
)

target_link_libraries(chris_pikul_mos6502_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common chris_pikul_mos6502_toolbox
)

target_compile_features(chris_pikul_mos6502_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(chris_pikul_mos6502_instruction_interrupt_group3)
endif()

add_executable(chris_pikul_mos6502_instruction_interrupt_opcode_sweep
    chris_pikul_mos6502_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(chris_pikul_mos6502_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common chris_pikul_mos6502_toolbox
)

target_compile_features(chris_pikul_mos6502_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(chris_pikul_mos6502_instruction_interrupt_opcode_sweep)
endif()

add_executable(chris_pikul_mos6502_instruction_interrupt_basic
    chris_pikul_mos6502_instruction_interrupt_basic.cpp
)

target_link_libraries(chris_pikul_mos6502_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common chris_pikul_mos6502_toolbox
)

target_compile_features(chris_pikul_mos6502_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(chris_pikul_mos6502_instruction_interrupt_basic)
endif()

add_executable(olcNES_instruction_cpu_smoke
    olcNES_instruction_cpu_smoke.cpp
)

target_link_libraries(olcNES_instruction_cpu_smoke
    PRIVATE olcNES_toolbox
)

target_compile_features(olcNES_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(olcNES_instruction_cpu_smoke)
endif()

add_executable(olcNES_instruction_interrupt_group1
    olcNES_instruction_interrupt_group1.cpp
)

target_link_libraries(olcNES_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common olcNES_toolbox
)

target_compile_features(olcNES_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(olcNES_instruction_interrupt_group1)
endif()

add_executable(olcNES_instruction_interrupt_group2
    olcNES_instruction_interrupt_group2.cpp
)

target_link_libraries(olcNES_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common olcNES_toolbox
)

target_compile_features(olcNES_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(olcNES_instruction_interrupt_group2)
endif()

add_executable(olcNES_instruction_interrupt_group3
    olcNES_instruction_interrupt_group3.cpp
)

target_link_libraries(olcNES_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common olcNES_toolbox
)

target_compile_features(olcNES_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(olcNES_instruction_interrupt_group3)
endif()

add_executable(olcNES_instruction_interrupt_opcode_sweep
    olcNES_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(olcNES_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common olcNES_toolbox
)

target_compile_features(olcNES_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(olcNES_instruction_interrupt_opcode_sweep)
endif()

add_executable(olcNES_instruction_interrupt_basic
    olcNES_instruction_interrupt_basic.cpp
)

target_link_libraries(olcNES_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common olcNES_toolbox
)

target_compile_features(olcNES_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(olcNES_instruction_interrupt_basic)
endif()

add_executable(ares_instruction_cpu_smoke
    ares_instruction_cpu_smoke.cpp
)

target_link_libraries(ares_instruction_cpu_smoke
    PRIVATE ares_toolbox
)

target_compile_features(ares_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(ares_instruction_cpu_smoke)
endif()

add_executable(ares_instruction_interrupt_group1
    ares_instruction_interrupt_group1.cpp
)

target_link_libraries(ares_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common ares_toolbox
)

target_compile_features(ares_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(ares_instruction_interrupt_group1)
endif()

add_executable(ares_instruction_interrupt_group2
    ares_instruction_interrupt_group2.cpp
)

target_link_libraries(ares_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common ares_toolbox
)

target_compile_features(ares_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(ares_instruction_interrupt_group2)
endif()

add_executable(ares_instruction_interrupt_group3
    ares_instruction_interrupt_group3.cpp
)

target_link_libraries(ares_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common ares_toolbox
)

target_compile_features(ares_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(ares_instruction_interrupt_group3)
endif()

add_executable(ares_instruction_interrupt_opcode_sweep
    ares_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(ares_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common ares_toolbox
)

target_compile_features(ares_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(ares_instruction_interrupt_opcode_sweep)
endif()

add_executable(ares_instruction_interrupt_basic
    ares_instruction_interrupt_basic.cpp
)

target_link_libraries(ares_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common ares_toolbox
)

target_compile_features(ares_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(ares_instruction_interrupt_basic)
endif()

add_executable(sflib6502_instruction_cpu_smoke
    sflib6502_instruction_cpu_smoke.cpp
)

target_link_libraries(sflib6502_instruction_cpu_smoke
    PRIVATE sflib6502_toolbox
)

target_compile_features(sflib6502_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib6502_instruction_cpu_smoke)
endif()

add_executable(sflib6502_instruction_interrupt_group1
    sflib6502_instruction_interrupt_group1.cpp
)

target_link_libraries(sflib6502_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common sflib6502_toolbox
)

target_compile_features(sflib6502_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib6502_instruction_interrupt_group1)
endif()

add_executable(sflib6502_instruction_interrupt_group2
    sflib6502_instruction_interrupt_group2.cpp
)

target_link_libraries(sflib6502_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common sflib6502_toolbox
)

target_compile_features(sflib6502_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib6502_instruction_interrupt_group2)
endif()

add_executable(sflib6502_instruction_interrupt_group3
    sflib6502_instruction_interrupt_group3.cpp
)

target_link_libraries(sflib6502_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common sflib6502_toolbox
)

target_compile_features(sflib6502_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib6502_instruction_interrupt_group3)
endif()

add_executable(sflib6502_instruction_interrupt_opcode_sweep
    sflib6502_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(sflib6502_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common sflib6502_toolbox
)

target_compile_features(sflib6502_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib6502_instruction_interrupt_opcode_sweep)
endif()

add_executable(sflib6502_instruction_interrupt_basic
    sflib6502_instruction_interrupt_basic.cpp
)

target_link_libraries(sflib6502_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common sflib6502_toolbox
)

target_compile_features(sflib6502_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib6502_instruction_interrupt_basic)
endif()


add_executable(sflib65c02_instruction_cpu_smoke
    sflib65c02_instruction_cpu_smoke.cpp
)

target_link_libraries(sflib65c02_instruction_cpu_smoke
    PRIVATE sflib65c02_toolbox
)

target_compile_features(sflib65c02_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib65c02_instruction_cpu_smoke)
endif()

add_executable(sflib65c02_instruction_interrupt_group1
    sflib65c02_instruction_interrupt_group1.cpp
)

target_link_libraries(sflib65c02_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common sflib65c02_toolbox
)

target_compile_features(sflib65c02_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib65c02_instruction_interrupt_group1)
endif()

add_executable(sflib65c02_instruction_interrupt_group2
    sflib65c02_instruction_interrupt_group2.cpp
)

target_link_libraries(sflib65c02_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common sflib65c02_toolbox
)

target_compile_features(sflib65c02_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib65c02_instruction_interrupt_group2)
endif()

add_executable(sflib65c02_instruction_interrupt_group3
    sflib65c02_instruction_interrupt_group3.cpp
)

target_link_libraries(sflib65c02_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common sflib65c02_toolbox
)

target_compile_features(sflib65c02_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib65c02_instruction_interrupt_group3)
endif()

add_executable(sflib65c02_instruction_interrupt_opcode_sweep
    sflib65c02_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(sflib65c02_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common sflib65c02_toolbox
)

target_compile_features(sflib65c02_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib65c02_instruction_interrupt_opcode_sweep)
endif()

add_executable(sflib65c02_instruction_interrupt_basic
    sflib65c02_instruction_interrupt_basic.cpp
)

target_link_libraries(sflib65c02_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common sflib65c02_toolbox
)

target_compile_features(sflib65c02_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(sflib65c02_instruction_interrupt_basic)
endif()


add_executable(fceux_instruction_cpu_smoke
    fceux_instruction_cpu_smoke.cpp
)

target_link_libraries(fceux_instruction_cpu_smoke
    PRIVATE fceux_toolbox
)

target_compile_features(fceux_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fceux_instruction_cpu_smoke)
endif()

add_executable(fceux_instruction_interrupt_group1
    fceux_instruction_interrupt_group1.cpp
)

target_link_libraries(fceux_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common fceux_toolbox
)

target_compile_features(fceux_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fceux_instruction_interrupt_group1)
endif()

add_executable(fceux_instruction_interrupt_group2
    fceux_instruction_interrupt_group2.cpp
)

target_link_libraries(fceux_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common fceux_toolbox
)

target_compile_features(fceux_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fceux_instruction_interrupt_group2)
endif()

add_executable(fceux_instruction_interrupt_group3
    fceux_instruction_interrupt_group3.cpp
)

target_link_libraries(fceux_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common fceux_toolbox
)

target_compile_features(fceux_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fceux_instruction_interrupt_group3)
endif()

add_executable(fceux_instruction_interrupt_opcode_sweep
    fceux_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(fceux_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common fceux_toolbox
)

target_compile_features(fceux_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fceux_instruction_interrupt_opcode_sweep)
endif()

add_executable(fceux_instruction_interrupt_basic
    fceux_instruction_interrupt_basic.cpp
)

target_link_libraries(fceux_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common fceux_toolbox
)

target_compile_features(fceux_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(fceux_instruction_interrupt_basic)
endif()

add_executable(gianlucag_mos6502_instruction_cpu_smoke
    gianlucag_mos6502_instruction_cpu_smoke.cpp
)

target_link_libraries(gianlucag_mos6502_instruction_cpu_smoke
    PRIVATE gianlucag_mos6502_toolbox
)

target_compile_features(gianlucag_mos6502_instruction_cpu_smoke PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(gianlucag_mos6502_instruction_cpu_smoke)
endif()

add_executable(gianlucag_mos6502_instruction_interrupt_group1
    gianlucag_mos6502_instruction_interrupt_group1.cpp
)

target_link_libraries(gianlucag_mos6502_instruction_interrupt_group1
    PRIVATE instruction_interrupt_basic_common gianlucag_mos6502_toolbox
)

target_compile_features(gianlucag_mos6502_instruction_interrupt_group1 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(gianlucag_mos6502_instruction_interrupt_group1)
endif()

add_executable(gianlucag_mos6502_instruction_interrupt_group2
    gianlucag_mos6502_instruction_interrupt_group2.cpp
)

target_link_libraries(gianlucag_mos6502_instruction_interrupt_group2
    PRIVATE instruction_interrupt_basic_common gianlucag_mos6502_toolbox
)

target_compile_features(gianlucag_mos6502_instruction_interrupt_group2 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(gianlucag_mos6502_instruction_interrupt_group2)
endif()

add_executable(gianlucag_mos6502_instruction_interrupt_group3
    gianlucag_mos6502_instruction_interrupt_group3.cpp
)

target_link_libraries(gianlucag_mos6502_instruction_interrupt_group3
    PRIVATE instruction_interrupt_basic_common gianlucag_mos6502_toolbox
)

target_compile_features(gianlucag_mos6502_instruction_interrupt_group3 PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(gianlucag_mos6502_instruction_interrupt_group3)
endif()

add_executable(gianlucag_mos6502_instruction_interrupt_opcode_sweep
    gianlucag_mos6502_instruction_interrupt_opcode_sweep.cpp
)

target_link_libraries(gianlucag_mos6502_instruction_interrupt_opcode_sweep
    PRIVATE instruction_interrupt_basic_common gianlucag_mos6502_toolbox
)

target_compile_features(gianlucag_mos6502_instruction_interrupt_opcode_sweep PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(gianlucag_mos6502_instruction_interrupt_opcode_sweep)
endif()

add_executable(gianlucag_mos6502_instruction_interrupt_basic
    gianlucag_mos6502_instruction_interrupt_basic.cpp
)

target_link_libraries(gianlucag_mos6502_instruction_interrupt_basic
    PRIVATE instruction_interrupt_basic_common gianlucag_mos6502_toolbox
)

target_compile_features(gianlucag_mos6502_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(gianlucag_mos6502_instruction_interrupt_basic)
endif()

add_executable(all_instruction_interrupt_basic
    all_instruction_interrupt_basic.cpp
)

target_compile_features(all_instruction_interrupt_basic PRIVATE cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(all_instruction_interrupt_basic)
endif()
