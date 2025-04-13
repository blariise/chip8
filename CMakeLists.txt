cmake_minimum_required(VERSION 3.16)
project(chip8)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)
add_executable(chip8
  src/chip8.cpp
  src/chip8sdl.cpp
  main.cpp)

include_directories(include)

target_include_directories(chip8 PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(chip8 PRIVATE ${SDL3_LIBRARIES})

# GoogleTest https://google.github.io/googletest/quickstart-cmake.html
enable_testing()
add_subdirectory(tests)


