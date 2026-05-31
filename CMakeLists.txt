cmake_minimum_required(VERSION 3.15)

project(qe6502-benchmark VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(QE6502_BUILD_WASM OFF CACHE BOOL "" FORCE)
set(QE6502_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(QE6502_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(QE6502_BUILD_CPP ON CACHE BOOL "" FORCE)
set(QE6502_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(QE6502_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(cores/qe6502)
add_subdirectory(toolbox/qe6502)
add_subdirectory(toolbox/floooh_chips)
add_subdirectory(toolbox/gianlucag_mos6502)
add_subdirectory(toolbox/fake6502)
add_subdirectory(toolbox/sflib6502)
add_subdirectory(toolbox/olcNES)
add_subdirectory(toolbox/chris_pikul_mos6502)

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

add_executable(floooh_chips_klaus_benchmark
    flooh_klaus_benchmark.cpp
)

target_link_libraries(floooh_chips_klaus_benchmark
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

if(MSVC)
    target_compile_options(qe6502_toolbox PRIVATE /W4)
    target_compile_options(floooh_chips_toolbox PRIVATE /W4)
    target_compile_options(gianlucag_mos6502_toolbox PRIVATE /W4)
    target_compile_options(fake6502_toolbox PRIVATE /W4)
    target_compile_options(sflib6502_toolbox PRIVATE /W4)
    target_compile_options(olcNES_toolbox PRIVATE /W4)
    target_compile_options(chris_pikul_mos6502_toolbox PRIVATE /W4)
    target_compile_options(qe6502_benchmark_smoketest PRIVATE /W4)
    target_compile_options(floooh_chips_smoketest PRIVATE /W4)
    target_compile_options(qe6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(floooh_chips_klaus_benchmark PRIVATE /W4)
    target_compile_options(gianlucag_mos6502_smoketest PRIVATE /W4)
    target_compile_options(gianlucag_mos6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(fake6502_smoketest PRIVATE /W4)
    target_compile_options(fake6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(sflib6502_smoketest PRIVATE /W4)
    target_compile_options(sflib6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(olcNES_smoketest PRIVATE /W4)
    target_compile_options(olcNES_klaus_benchmark PRIVATE /W4)
    target_compile_options(chris_pikul_mos6502_smoketest PRIVATE /W4)
    target_compile_options(chris_pikul_mos6502_klaus_benchmark PRIVATE /W4)
else()
    target_compile_options(qe6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(gianlucag_mos6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(olcNES_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chris_pikul_mos6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(qe6502_benchmark_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(qe6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(gianlucag_mos6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(gianlucag_mos6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fake6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(sflib6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(olcNES_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(olcNES_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chris_pikul_mos6502_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chris_pikul_mos6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
endif()
