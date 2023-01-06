# Set the install directory for the tests.
set(EXAMPLES_INSTALL_DIR "${CMAKE_SOURCE_DIR}/install/examples/")

# Add each test directory to configure.
add_subdirectory(cubes)
add_subdirectory(lighting)
