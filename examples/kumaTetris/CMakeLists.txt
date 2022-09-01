# Gather the source files for the example.
file(GLOB KUMA_TETRIS_SOURCES *.cpp
                              systems/*.cpp)

# Create the executable using the source files.
add_executable(KumaTetris ${KUMA_TETRIS_SOURCES})

# Link the executable with the engine.
target_link_libraries(KumaTetris PUBLIC
                      Kuma3D)

# Include the engine headers from the install directory.
target_include_directories(KumaTetris PUBLIC
                           ${INCLUDE_INSTALL_DIR}
                           components
                           systems
                           util)

# Install this example.
install(TARGETS KumaTetris DESTINATION ${EXAMPLES_INSTALL_DIR}/KumaTetris)
install(DIRECTORY resources DESTINATION ${EXAMPLES_INSTALL_DIR}/KumaTetris)
