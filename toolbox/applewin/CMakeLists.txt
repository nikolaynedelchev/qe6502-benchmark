add_library(applewin_toolbox STATIC
    src/applewin_cpu.cpp
    src/smoke.cpp
    src/klaus_benchmark.cpp
)

target_include_directories(applewin_toolbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/cores/applewin/source
        ${PROJECT_SOURCE_DIR}
)

target_link_libraries(applewin_toolbox
    PUBLIC benchmark6502_common
)

target_compile_options(applewin_toolbox PRIVATE -Wno-unused-variable -Wno-unused-function -Wno-sign-compare)
