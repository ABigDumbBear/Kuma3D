# Gather the source files for the example.
file(GLOB UNTITLED_SOURCES *.cpp)

# Create the executable using the source files.
add_executable(Untitled ${UNTITLED_SOURCES})

# Link the executable with the engine.
target_link_libraries(Untitled PUBLIC
                      Kuma3D)

# Include the engine headers from the install directory.
target_include_directories(Untitled PUBLIC
                           ${INCLUDE_INSTALL_DIR})

# Install this example.
install(TARGETS Untitled DESTINATION ${EXAMPLES_INSTALL_DIR}/Untitled)
install(DIRECTORY resources DESTINATION ${EXAMPLES_INSTALL_DIR}/Untitled)
