# Gather the source files for the example.
file(GLOB 01_SOURCES *.cpp)

# Create the executable using the source files.
add_executable(hello-world ${01_SOURCES})

# Link the executable with the engine.
target_link_libraries(hello-world PUBLIC
                      Kuma3D)

# Include the engine headers from the install directory.
target_include_directories(hello-world PUBLIC
                           ${INCLUDE_INSTALL_DIR})

# Install this example.
install(TARGETS hello-world DESTINATION ${EXAMPLES_INSTALL_DIR}/01-hello-world)
