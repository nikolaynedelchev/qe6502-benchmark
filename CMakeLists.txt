cmake_minimum_required(VERSION 3.15)

project(qe6502-benchmark VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(QE6502_BUILD_WASM OFF CACHE BOOL "" FORCE)
set(QE6502_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(QE6502_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(QE6502_BUILD_CPP ON CACHE BOOL "" FORCE)
set(QE6502_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(QE6502_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(cores/qe6502)
add_subdirectory(toolbox/common)
add_subdirectory(toolbox/qe6502)
add_subdirectory(toolbox/floooh_chips)
add_subdirectory(toolbox/gianlucag_mos6502)
add_subdirectory(toolbox/fake6502)
add_subdirectory(toolbox/fake65c02)
add_subdirectory(toolbox/vremu6502)
add_subdirectory(toolbox/o2)
add_subdirectory(toolbox/peddle)
add_subdirectory(toolbox/sflib6502)
add_subdirectory(toolbox/sflib65c02)
add_subdirectory(toolbox/olcNES)
add_subdirectory(toolbox/chris_pikul_mos6502)
add_subdirectory(toolbox/clk)



add_executable(clk_smoketest
    clk_smoke_test.cpp
)

target_link_libraries(clk_smoketest
    PRIVATE clk_toolbox
)

add_executable(opcode_metadata_smoketest
    opcode_metadata_smoke_test.cpp
)

target_link_libraries(opcode_metadata_smoketest
    PRIVATE benchmark6502_common
)

add_executable(qe6502_benchmark_smoketest
    smoke_test.cpp
)

target_link_libraries(qe6502_benchmark_smoketest
    PRIVATE qe6502_toolbox
)

add_executable(qe6502_klaus_benchmark
    qe6502_klaus_benchmark.cpp
)

target_link_libraries(qe6502_klaus_benchmark
    PRIVATE qe6502_toolbox
)


add_executable(qe6502_singlestep_nmos
    qe6502_singlestep_nmos.cpp
)

target_link_libraries(qe6502_singlestep_nmos
    PRIVATE qe6502_toolbox
)

add_executable(floooh_chips_klaus_benchmark
    flooh_klaus_benchmark.cpp
)

target_link_libraries(floooh_chips_klaus_benchmark
    PRIVATE floooh_chips_toolbox
)

add_executable(floooh_chips_singlestep_nmos
    floooh_chips_singlestep_nmos.cpp
)

target_link_libraries(floooh_chips_singlestep_nmos
    PRIVATE floooh_chips_toolbox
)

add_executable(floooh_chips_smoketest
    flooh_smoke_test.cpp
)

target_link_libraries(floooh_chips_smoketest
    PRIVATE floooh_chips_toolbox
)


add_executable(gianlucag_mos6502_klaus_benchmark
    gianlucag_mos6502_klaus_benchmark.cpp
)

target_link_libraries(gianlucag_mos6502_klaus_benchmark
    PRIVATE gianlucag_mos6502_toolbox
)

add_executable(gianlucag_mos6502_smoketest
    gianlucag_mos6502_smoke_test.cpp
)

target_link_libraries(gianlucag_mos6502_smoketest
    PRIVATE gianlucag_mos6502_toolbox
)

add_executable(gianlucag_mos6502_singlestep_nmos
    gianlucag_mos6502_singlestep_nmos.cpp
)

target_link_libraries(gianlucag_mos6502_singlestep_nmos
    PRIVATE gianlucag_mos6502_toolbox
)

add_executable(fake6502_smoketest
    fake6502_smoke_test.cpp
)

target_link_libraries(fake6502_smoketest
    PRIVATE fake6502_toolbox
)

add_executable(fake6502_klaus_benchmark
    fake6502_klaus_benchmark.cpp
)

target_link_libraries(fake6502_klaus_benchmark
    PRIVATE fake6502_toolbox
)

add_executable(fake6502_singlestep_nmos
    fake6502_singlestep_nmos.cpp
)

target_link_libraries(fake6502_singlestep_nmos
    PRIVATE fake6502_toolbox
)

add_executable(fake65c02_smoketest
    fake65c02_smoke_test.cpp
)

target_link_libraries(fake65c02_smoketest
    PRIVATE fake65c02_toolbox
)

add_executable(fake65c02_klaus_extended_benchmark
    fake65c02_klaus_extended_benchmark.cpp
)

target_link_libraries(fake65c02_klaus_extended_benchmark
    PRIVATE fake65c02_toolbox
)

add_executable(vremu6502_smoketest
    vremu6502_smoke_test.cpp
)

target_link_libraries(vremu6502_smoketest
    PRIVATE vremu6502_toolbox
)

add_executable(vremu6502_klaus_benchmark
    vremu6502_klaus_benchmark.cpp
)

target_link_libraries(vremu6502_klaus_benchmark
    PRIVATE vremu6502_toolbox
)

add_executable(vremu6502_klaus_65c02_benchmark
    vremu6502_klaus_65c02_benchmark.cpp
)

target_link_libraries(vremu6502_klaus_65c02_benchmark
    PRIVATE vremu6502_toolbox
)

add_executable(vremu6502_singlestep_nmos
    vremu6502_singlestep_nmos.cpp
)

target_link_libraries(vremu6502_singlestep_nmos
    PRIVATE vremu6502_toolbox
)

add_executable(vremu6502_singlestep_cpu65c02
    vremu6502_singlestep_cpu65c02.cpp
)

target_link_libraries(vremu6502_singlestep_cpu65c02
    PRIVATE vremu6502_toolbox
)

add_executable(o2_smoketest
    o2_smoke_test.cpp
)

target_link_libraries(o2_smoketest
    PRIVATE o2_toolbox
)

add_executable(o2_klaus_benchmark
    o2_klaus_benchmark.cpp
)

target_link_libraries(o2_klaus_benchmark
    PRIVATE o2_toolbox
)

add_executable(o2_singlestep_nmos
    o2_singlestep_nmos.cpp
)

target_link_libraries(o2_singlestep_nmos
    PRIVATE o2_toolbox
)

add_executable(peddle_smoketest
    peddle_smoke_test.cpp
)

target_link_libraries(peddle_smoketest
    PRIVATE peddle_toolbox
)

add_executable(peddle_klaus_benchmark
    peddle_klaus_benchmark.cpp
)

target_link_libraries(peddle_klaus_benchmark
    PRIVATE peddle_toolbox
)

add_executable(peddle_singlestep_nmos
    peddle_singlestep_nmos.cpp
)

target_link_libraries(peddle_singlestep_nmos
    PRIVATE peddle_toolbox
)

add_executable(sflib6502_smoketest
    sflib6502_smoke_test.cpp
)

target_link_libraries(sflib6502_smoketest
    PRIVATE sflib6502_toolbox
)


add_executable(sflib6502_klaus_benchmark
    sflib6502_klaus_benchmark.cpp
)

target_link_libraries(sflib6502_klaus_benchmark
    PRIVATE sflib6502_toolbox
)

add_executable(sflib6502_singlestep_nmos
    sflib6502_singlestep_nmos.cpp
)

target_link_libraries(sflib6502_singlestep_nmos
    PRIVATE sflib6502_toolbox
)

add_executable(sflib65c02_smoketest
    sflib65c02_smoke_test.cpp
)

target_link_libraries(sflib65c02_smoketest
    PRIVATE sflib65c02_toolbox
)

add_executable(sflib65c02_klaus_extended_benchmark
    sflib65c02_klaus_extended_benchmark.cpp
)

target_link_libraries(sflib65c02_klaus_extended_benchmark
    PRIVATE sflib65c02_toolbox
)

add_executable(olcNES_smoketest
    olcNES_smoke_test.cpp
)

target_link_libraries(olcNES_smoketest
    PRIVATE olcNES_toolbox
)


add_executable(olcNES_klaus_benchmark
    olcNES_klaus_benchmark.cpp
)

target_link_libraries(olcNES_klaus_benchmark
    PRIVATE olcNES_toolbox
)

add_executable(olcNES_singlestep_nmos
    olcNES_singlestep_nmos.cpp
)

target_link_libraries(olcNES_singlestep_nmos
    PRIVATE olcNES_toolbox
)

add_executable(chris_pikul_mos6502_smoketest
    chris_pikul_mos6502_smoke_test.cpp
)

target_link_libraries(chris_pikul_mos6502_smoketest
    PRIVATE chris_pikul_mos6502_toolbox
)

add_executable(chris_pikul_mos6502_klaus_benchmark
    chris_pikul_mos6502_klaus_benchmark.cpp
)

target_link_libraries(chris_pikul_mos6502_klaus_benchmark
    PRIVATE chris_pikul_mos6502_toolbox
)

add_executable(chris_pikul_mos6502_singlestep_nmos
    chris_pikul_mos6502_singlestep_nmos.cpp
)

target_link_libraries(chris_pikul_mos6502_singlestep_nmos
    PRIVATE chris_pikul_mos6502_toolbox
)

add_executable(all_6502_tests
    all_6502_tests.cpp
)

target_link_libraries(all_6502_tests
    PRIVATE
        qe6502_toolbox
        floooh_chips_toolbox
        gianlucag_mos6502_toolbox
        fake6502_toolbox
        vremu6502_toolbox
        o2_toolbox
        peddle_toolbox
        sflib6502_toolbox
        olcNES_toolbox
        chris_pikul_mos6502_toolbox
)

add_executable(all_65c02_tests
    all_65c02_tests.cpp
)

target_link_libraries(all_65c02_tests
    PRIVATE
        fake65c02_toolbox
        vremu6502_toolbox
        sflib65c02_toolbox
        qe6502_toolbox
)

if(MSVC)
    target_compile_options(opcode_metadata_smoketest PRIVATE /W4)
    target_compile_options(qe6502_toolbox PRIVATE /W4)
    target_compile_options(floooh_chips_toolbox PRIVATE /W4)
    target_compile_options(gianlucag_mos6502_toolbox PRIVATE /W4)
    target_compile_options(fake6502_toolbox PRIVATE /W4)
    target_compile_options(fake65c02_toolbox PRIVATE /W4)
    target_compile_options(vremu6502_toolbox PRIVATE /W4)
    target_compile_options(o2_toolbox PRIVATE /W4)
    target_compile_options(sflib6502_toolbox PRIVATE /W4)
    target_compile_options(sflib65c02_toolbox PRIVATE /W4)
    target_compile_options(olcNES_toolbox PRIVATE /W4)
    target_compile_options(chris_pikul_mos6502_toolbox PRIVATE /W4)
    target_compile_options(qe6502_benchmark_smoketest PRIVATE /W4)
    target_compile_options(floooh_chips_smoketest PRIVATE /W4)
    target_compile_options(qe6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(qe6502_singlestep_nmos PRIVATE /W4)
    target_compile_options(floooh_chips_klaus_benchmark PRIVATE /W4)
    target_compile_options(gianlucag_mos6502_smoketest PRIVATE /W4)
    target_compile_options(gianlucag_mos6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(gianlucag_mos6502_singlestep_nmos PRIVATE /W4)
    target_compile_options(fake6502_smoketest PRIVATE /W4)
    target_compile_options(fake6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(fake6502_singlestep_nmos PRIVATE /W4)
    target_compile_options(fake65c02_smoketest PRIVATE /W4)
    target_compile_options(fake65c02_klaus_extended_benchmark PRIVATE /W4)
    target_compile_options(vremu6502_smoketest PRIVATE /W4)
    target_compile_options(vremu6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(vremu6502_klaus_65c02_benchmark PRIVATE /W4)
    target_compile_options(vremu6502_singlestep_nmos PRIVATE /W4)
    target_compile_options(vremu6502_singlestep_cpu65c02 PRIVATE /W4)
    target_compile_options(o2_smoketest PRIVATE /W4)
    target_compile_options(o2_klaus_benchmark PRIVATE /W4)
    target_compile_options(o2_singlestep_nmos PRIVATE /W4)
    target_compile_options(peddle_smoketest PRIVATE /W4)
    target_compile_options(peddle_klaus_benchmark PRIVATE /W4)
    target_compile_options(sflib6502_smoketest PRIVATE /W4)
    target_compile_options(sflib6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(sflib6502_singlestep_nmos PRIVATE /W4)
    target_compile_options(sflib65c02_smoketest PRIVATE /W4)
    target_compile_options(sflib65c02_klaus_extended_benchmark PRIVATE /W4)
    target_compile_options(olcNES_smoketest PRIVATE /W4)
    target_compile_options(olcNES_klaus_benchmark PRIVATE /W4)
    target_compile_options(olcNES_singlestep_nmos PRIVATE /W4)
    target_compile_options(chris_pikul_mos6502_smoketest PRIVATE /W4)
    target_compile_options(chris_pikul_mos6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(chris_pikul_mos6502_singlestep_nmos PRIVATE /W4)
    target_compile_options(all_6502_tests PRIVATE /W4)
    target_compile_options(all_65c02_tests PRIVATE /W4)
else()
    target_compile_options(opcode_metadata_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(qe6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(gianlucag_mos6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake65c02_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vremu6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(o2_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib65c02_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(olcNES_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chris_pikul_mos6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(qe6502_benchmark_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(qe6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(qe6502_singlestep_nmos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(gianlucag_mos6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(gianlucag_mos6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(gianlucag_mos6502_singlestep_nmos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake6502_singlestep_nmos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake65c02_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake65c02_klaus_extended_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vremu6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vremu6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vremu6502_klaus_65c02_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vremu6502_singlestep_nmos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(vremu6502_singlestep_cpu65c02 PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(o2_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(o2_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(o2_singlestep_nmos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(peddle_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(peddle_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib6502_singlestep_nmos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib65c02_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib65c02_klaus_extended_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(olcNES_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(olcNES_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(olcNES_singlestep_nmos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chris_pikul_mos6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chris_pikul_mos6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chris_pikul_mos6502_singlestep_nmos PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(all_6502_tests PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(all_65c02_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()
