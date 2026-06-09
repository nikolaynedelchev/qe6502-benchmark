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
