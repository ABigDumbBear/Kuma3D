# Create the executable.
add_executable(lighting main.cpp
                        OrbitSystem.cpp)

# Link the executable with the engine.
target_link_libraries(lighting PUBLIC
                      Kuma3D)

# Include the engine headers fron the install directory.
target_include_directories(lighting PUBLIC
                           ${INCLUDE_INSTALL_DIR})

# Install the executable and the resources required.
install(TARGETS lighting DESTINATION ${EXAMPLES_INSTALL_DIR}/lighting/)
install(DIRECTORY resources DESTINATION ${EXAMPLES_INSTALL_DIR}/lighting/)
