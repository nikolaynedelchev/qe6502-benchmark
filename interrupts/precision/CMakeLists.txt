add_library(interrupt_precision_common STATIC
    common/common.cpp
    common/i_flag_windows.cpp
    common/interrupt_arbitration.cpp
    common/nmi_irq_pulse_sweep.cpp
)

target_link_libraries(interrupt_precision_common
    PUBLIC
        benchmark6502_common
        tools6502::tools6502
)

target_include_directories(interrupt_precision_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(interrupt_precision_common PUBLIC cxx_std_20)

if(COMMAND qe6502_configure_warnings)
    qe6502_configure_warnings(interrupt_precision_common)
endif()

add_executable(floooh_chips_icpu_smoke
    floooh_chips_icpu_smoke.cpp
)

target_link_libraries(floooh_chips_icpu_smoke
    PRIVATE
        floooh_chips_toolbox
        tools6502::tools6502
)

target_compile_features(floooh_chips_icpu_smoke PRIVATE cxx_std_20)

add_executable(floooh_chips_interrupt_i_flag_windows
    floooh_chips_i_flag_windows.cpp
)

target_link_libraries(floooh_chips_interrupt_i_flag_windows
    PRIVATE
        floooh_chips_toolbox
        interrupt_precision_common
)

target_compile_features(floooh_chips_interrupt_i_flag_windows PRIVATE cxx_std_20)

add_executable(floooh_chips_interrupt_arbitration
    floooh_chips_interrupt_arbitration.cpp
)

target_link_libraries(floooh_chips_interrupt_arbitration
    PRIVATE
        floooh_chips_toolbox
        interrupt_precision_common
)

target_compile_features(floooh_chips_interrupt_arbitration PRIVATE cxx_std_20)

add_executable(floooh_chips_interrupt_nmi_irq_pulse_sweep
    floooh_chips_nmi_irq_pulse_sweep.cpp
)

target_link_libraries(floooh_chips_interrupt_nmi_irq_pulse_sweep
    PRIVATE
        floooh_chips_toolbox
        interrupt_precision_common
)

target_compile_features(floooh_chips_interrupt_nmi_irq_pulse_sweep PRIVATE cxx_std_20)


add_executable(qe6502_icpu_smoke
    qe6502_icpu_smoke.cpp
)

target_link_libraries(qe6502_icpu_smoke
    PRIVATE
        tools6502::tools6502
)

target_compile_features(qe6502_icpu_smoke PRIVATE cxx_std_20)

add_executable(qe6502_interrupt_i_flag_windows
    qe6502_i_flag_windows.cpp
)

target_link_libraries(qe6502_interrupt_i_flag_windows
    PRIVATE
        interrupt_precision_common
)

target_compile_features(qe6502_interrupt_i_flag_windows PRIVATE cxx_std_20)

add_executable(qe6502_interrupt_arbitration
    qe6502_interrupt_arbitration.cpp
)

target_link_libraries(qe6502_interrupt_arbitration
    PRIVATE
        interrupt_precision_common
)

target_compile_features(qe6502_interrupt_arbitration PRIVATE cxx_std_20)

add_executable(qe6502_interrupt_nmi_irq_pulse_sweep
    qe6502_nmi_irq_pulse_sweep.cpp
)

target_link_libraries(qe6502_interrupt_nmi_irq_pulse_sweep
    PRIVATE
        interrupt_precision_common
)

target_compile_features(qe6502_interrupt_nmi_irq_pulse_sweep PRIVATE cxx_std_20)


add_executable(clk_icpu_smoke
    clk_icpu_smoke.cpp
)

target_link_libraries(clk_icpu_smoke
    PRIVATE
        clk_toolbox
        tools6502::tools6502
)

target_compile_features(clk_icpu_smoke PRIVATE cxx_std_20)

add_executable(clk_interrupt_i_flag_windows
    clk_i_flag_windows.cpp
)

target_link_libraries(clk_interrupt_i_flag_windows
    PRIVATE
        clk_toolbox
        interrupt_precision_common
)

target_compile_features(clk_interrupt_i_flag_windows PRIVATE cxx_std_20)

add_executable(clk_interrupt_arbitration
    clk_interrupt_arbitration.cpp
)

target_link_libraries(clk_interrupt_arbitration
    PRIVATE
        clk_toolbox
        interrupt_precision_common
)

target_compile_features(clk_interrupt_arbitration PRIVATE cxx_std_20)

add_executable(clk_interrupt_nmi_irq_pulse_sweep
    clk_nmi_irq_pulse_sweep.cpp
)

target_link_libraries(clk_interrupt_nmi_irq_pulse_sweep
    PRIVATE
        clk_toolbox
        interrupt_precision_common
)

target_compile_features(clk_interrupt_nmi_irq_pulse_sweep PRIVATE cxx_std_20)

add_executable(peddle_icpu_smoke
    peddle_icpu_smoke.cpp
)

target_link_libraries(peddle_icpu_smoke
    PRIVATE
        peddle_toolbox
        tools6502::tools6502
)

target_compile_features(peddle_icpu_smoke PRIVATE cxx_std_20)

add_executable(peddle_interrupt_i_flag_windows
    peddle_i_flag_windows.cpp
)

target_link_libraries(peddle_interrupt_i_flag_windows
    PRIVATE
        peddle_toolbox
        interrupt_precision_common
)

target_compile_features(peddle_interrupt_i_flag_windows PRIVATE cxx_std_20)

add_executable(peddle_interrupt_arbitration
    peddle_interrupt_arbitration.cpp
)

target_link_libraries(peddle_interrupt_arbitration
    PRIVATE
        peddle_toolbox
        interrupt_precision_common
)

target_compile_features(peddle_interrupt_arbitration PRIVATE cxx_std_20)

add_executable(peddle_interrupt_nmi_irq_pulse_sweep
    peddle_nmi_irq_pulse_sweep.cpp
)

target_link_libraries(peddle_interrupt_nmi_irq_pulse_sweep
    PRIVATE
        peddle_toolbox
        interrupt_precision_common
)

target_compile_features(peddle_interrupt_nmi_irq_pulse_sweep PRIVATE cxx_std_20)


add_executable(o2_icpu_smoke
    o2_icpu_smoke.cpp
)

target_link_libraries(o2_icpu_smoke
    PRIVATE
        o2_toolbox
        tools6502::tools6502
)

target_compile_features(o2_icpu_smoke PRIVATE cxx_std_20)

add_executable(o2_interrupt_i_flag_windows
    o2_i_flag_windows.cpp
)

target_link_libraries(o2_interrupt_i_flag_windows
    PRIVATE
        o2_toolbox
        interrupt_precision_common
)

target_compile_features(o2_interrupt_i_flag_windows PRIVATE cxx_std_20)

add_executable(o2_interrupt_arbitration
    o2_interrupt_arbitration.cpp
)

target_link_libraries(o2_interrupt_arbitration
    PRIVATE
        o2_toolbox
        interrupt_precision_common
)

target_compile_features(o2_interrupt_arbitration PRIVATE cxx_std_20)

add_executable(o2_interrupt_nmi_irq_pulse_sweep
    o2_nmi_irq_pulse_sweep.cpp
)

target_link_libraries(o2_interrupt_nmi_irq_pulse_sweep
    PRIVATE
        o2_toolbox
        interrupt_precision_common
)

target_compile_features(o2_interrupt_nmi_irq_pulse_sweep PRIVATE cxx_std_20)

foreach(target
    qe6502_icpu_smoke
    qe6502_interrupt_i_flag_windows
    qe6502_interrupt_arbitration
    qe6502_interrupt_nmi_irq_pulse_sweep
    floooh_chips_icpu_smoke
    floooh_chips_interrupt_i_flag_windows
    floooh_chips_interrupt_arbitration
    floooh_chips_interrupt_nmi_irq_pulse_sweep
    clk_icpu_smoke
    clk_interrupt_i_flag_windows
    clk_interrupt_arbitration
    clk_interrupt_nmi_irq_pulse_sweep
    peddle_icpu_smoke
    peddle_interrupt_i_flag_windows
    peddle_interrupt_arbitration
    peddle_interrupt_nmi_irq_pulse_sweep
    o2_icpu_smoke
    o2_interrupt_i_flag_windows
    o2_interrupt_arbitration
    o2_interrupt_nmi_irq_pulse_sweep
)
    if(COMMAND qe6502_configure_warnings)
        qe6502_configure_warnings(${target})
    endif()
endforeach()
