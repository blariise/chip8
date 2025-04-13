cmake_minimum_required(VERSION 3.16)
project(chip8)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)
add_executable(chip8 chip8.cpp chip8sdl.cpp main.cpp)

target_include_directories(chip8 PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(chip8 PRIVATE ${SDL3_LIBRARIES})

# GoogleTest https://google.github.io/googletest/quickstart-cmake.html
enable_testing()

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e90fe2485641bab0d6af4500192dc503384950d1.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
  chip8_tests
  tests/chip8_tests.cpp
)

target_link_libraries(
  chip8_tests
  PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(chip8_tests)
