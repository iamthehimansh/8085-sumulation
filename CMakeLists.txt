cmake_minimum_required(VERSION 3.10)
project(8085-simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/core/cpu.cpp
    src/core/memory.cpp
    src/core/simulator.cpp
    src/utils/utils.cpp
)

# Add header files
include_directories(include)

# Create executable
add_executable(8085sim ${SOURCES})