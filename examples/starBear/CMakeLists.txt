# Gather the source files for the example.
file(GLOB STARBEAR_SOURCES *.cpp
                           systems/*.cpp)

# Create the executable using the source files.
add_executable(StarBear ${STARBEAR_SOURCES})

# Link the executable with the engine.
target_link_libraries(StarBear PUBLIC
                      Kuma3D)

# Include the engine headers from the install directory.
target_include_directories(StarBear PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${INCLUDE_INSTALL_DIR}
                           components
                           systems)

# Install this example.
install(TARGETS StarBear DESTINATION ${EXAMPLES_INSTALL_DIR}/StarBear)
install(DIRECTORY resources DESTINATION ${EXAMPLES_INSTALL_DIR}/StarBear)
