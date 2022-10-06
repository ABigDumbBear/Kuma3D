# Create the executable.
add_executable(componentTest main.cpp)

# Link the executable with the engine.
target_link_libraries(componentTest PUBLIC
                      Kuma3D)

# Include the engine headers fron the install directory.
target_include_directories(componentTest PUBLIC
                           ${INCLUDE_INSTALL_DIR})

# Install the test executable.
install(TARGETS componentTest DESTINATION ${TESTS_INSTALL_DIR})
