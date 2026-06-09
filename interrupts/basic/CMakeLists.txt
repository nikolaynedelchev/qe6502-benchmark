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
