# Create the executable.
add_executable(cubes main.cpp
                     PhysicsSystem.cpp)

# Link the executable with the engine.
target_link_libraries(cubes PUBLIC
                      Kuma3D)

# Include the engine headers fron the install directory.
target_include_directories(cubes PUBLIC
                           ${INCLUDE_INSTALL_DIR})

# Install the executable and the resources required.
install(TARGETS cubes DESTINATION ${EXAMPLES_INSTALL_DIR})
install(DIRECTORY resources DESTINATION ${EXAMPLES_INSTALL_DIR})
