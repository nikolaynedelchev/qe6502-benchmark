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

if(MSVC)
    target_compile_options(qe6502_toolbox PRIVATE /W4)
    target_compile_options(floooh_chips_toolbox PRIVATE /W4)
    target_compile_options(qe6502_benchmark_smoketest PRIVATE /W4)
    target_compile_options(floooh_chips_smoketest PRIVATE /W4)
    target_compile_options(qe6502_klaus_benchmark PRIVATE /W4)
    target_compile_options(floooh_chips_klaus_benchmark PRIVATE /W4)
else()
    target_compile_options(qe6502_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_toolbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(qe6502_benchmark_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_smoketest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(qe6502_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(floooh_chips_klaus_benchmark PRIVATE -Wall -Wextra -Wpedantic)
endif()
