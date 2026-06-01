add_library(applewin_toolbox STATIC
    src/applewin_cpu.cpp
)

target_include_directories(applewin_toolbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/cores/applewin/source
)

target_compile_options(applewin_toolbox PRIVATE -Wno-unused-variable -Wno-unused-function -Wno-sign-compare)
