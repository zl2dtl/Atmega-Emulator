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
)

target_include_directories(atmega-emu
    PRIVATE
        include
)
