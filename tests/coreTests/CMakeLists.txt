# Create the executable.
add_executable(coreTest main.cpp)

# Link the executable with the engine.
target_link_libraries(coreTest PUBLIC
                      Kuma3D)

# Include the engine headers fron the install directory.
target_include_directories(coreTest PUBLIC
                           ${INCLUDE_INSTALL_DIR})

# Install the test executable.
install(TARGETS coreTest DESTINATION ${TESTS_INSTALL_DIR})
