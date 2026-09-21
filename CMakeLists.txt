cmake_minimum_required(VERSION 3.20)

project(Atmega328_Emulator
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(atmega-emu
    src/main.cpp
    src/cpu.cpp
)

add_executable(test_cpu
    tests/test_cpu.cpp
    src/cpu.cpp
)

target_include_directories(test_cpu
    PRIVATE
        include
)

target_include_directories(atmega-emu
    PRIVATE
        include
)

enable_testing()

add_test(
    NAME cpu_tests
    COMMAND test_cpu
)