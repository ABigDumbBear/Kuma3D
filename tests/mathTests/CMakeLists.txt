# Create the executable.
add_executable(mathTest main.cpp)

# Link the executable with the engine.
target_link_libraries(mathTest PUBLIC
                      Kuma3D)

# Include the engine headers from the install directory.
target_include_directories(mathTest PUBLIC
                           ${INCLUDE_INCLUDE_DIR})

# Install the test executable.
install(TARGETS mathTest DESTINATION ${TESTS_INSTALL_DIR})
