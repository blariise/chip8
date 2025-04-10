cmake_minimum_required(VERSION 3.16)
project(chip8)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)
add_executable(chip8 chip8.cpp)

target_include_directories(chip8 PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(chip8 PRIVATE ${SDL3_LIBRARIES})
