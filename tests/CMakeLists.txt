cmake_minimum_required(VERSION 3.16)
project(chip8_tests)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e90fe2485641bab0d6af4500192dc503384950d1.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
  chip8_tests
  chip8_tests.cpp
  ../chip8.cpp
)

target_link_libraries(
  chip8_tests
  PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(chip8_tests)
