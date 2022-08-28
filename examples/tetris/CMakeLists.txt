# Gather the source files for the example.
file(GLOB TETRIS_SOURCES *.cpp)

# Create the executable using the source files.
add_executable(Tetris ${TETRIS_SOURCES})

# Link the executable with the engine.
target_link_libraries(Tetris PUBLIC
                      Kuma3D)

# Include the engine headers from the install directory.
target_include_directories(Tetris PUBLIC
                           ${INCLUDE_INSTALL_DIR})

# Install this example.
install(TARGETS Tetris DESTINATION ${EXAMPLES_INSTALL_DIR}/Tetris)
install(DIRECTORY resources DESTINATION ${EXAMPLES_INSTALL_DIR}/Tetris)
