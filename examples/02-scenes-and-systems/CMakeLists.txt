# Gather the source files for this example.
file(GLOB 02_SOURCES *.cpp)

# Create the executable using the source files.
add_executable(scenes-and-systems ${02_SOURCES})

# Link the executable with the engine.
target_link_libraries(scenes-and-systems PUBLIC
                      Kuma3D)

# Include the engine headers from the install directory.
target_include_directories(scenes-and-systems PUBLIC
                           ${INCLUDE_INSTALL_DIR})

# Install this example and the necessary resources.
install(TARGETS scenes-and-systems DESTINATION ${EXAMPLES_INSTALL_DIR}/02-scenes-and-systems)
install(DIRECTORY resources DESTINATION ${EXAMPLES_INSTALL_DIR}/02-scenes-and-systems)
