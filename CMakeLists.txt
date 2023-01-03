cmake_minimum_required(VERSION 3.10)
project(Kuma3D VERSION 0.0.1)

# Exports compile commands as a .json file; some text editors use
# this file for syntax highlighting.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add options to build extra stuff.
option(BUILD_TESTS "Build the engine unit tests." ON)
option(BUILD_EXAMPLES "Build the engine example projects." ON)

# Add the source directory.
add_subdirectory(source)

# Add the optional directories.
if(BUILD_TESTS)
  add_subdirectory(tests)
endif(BUILD_TESTS)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif(BUILD_EXAMPLES)
