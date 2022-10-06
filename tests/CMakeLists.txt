# Set the install directory for the tests.
set(TESTS_INSTALL_DIR "${CMAKE_SOURCE_DIR}/install/tests/")

# Add each test directory to configure.
add_subdirectory(mathTests)
add_subdirectory(coreTests)
