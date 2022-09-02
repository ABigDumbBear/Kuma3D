# Gather the required third-party libraries that are too big to be
# contained in a single header.
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

# Set the directory for single header third-party libraries.
set(3RD_PARTY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd_party)

# Gather the source files for the engine.
file(GLOB ENGINE_SOURCES audio/*.?pp
                         core/*.?pp
                         components/*.?pp
                         math/*.?pp
                         renderer/*.?pp)

# Create the engine library and link it with the third-party libraries.
add_library(Kuma3D SHARED ${ENGINE_SOURCES})
target_link_libraries(Kuma3D PUBLIC
                      glfw
                      ${GLEW_LIBRARIES}
                      ${FREETYPE_LIBRARIES})

# Set the include directories for the engine.
target_include_directories(Kuma3D PUBLIC
                           audio
                           core
                           components
                           math
                           renderer
                           ${3RD_PARTY_INCLUDE_DIR}
                           ${GLEW_INCLUDE_DIRS}
                           ${FREETYPE_INCLUDE_DIRS})

# Install the engine library.
set(ENGINE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/install)
set(LIB_INSTALL_DIR ${ENGINE_INSTALL_DIR}/lib)
install(TARGETS Kuma3D LIBRARY DESTINATION ${LIB_INSTALL_DIR})

# Install the engine header files.
set(INCLUDE_INSTALL_DIR ${ENGINE_INSTALL_DIR}/include)
file(GLOB ENGINE_INCLUDES audio/*.hpp
                          core/*.hpp
                          components/*.hpp
                          math/*.hpp
                          renderer/*.hpp)
install(FILES ${ENGINE_INCLUDES} DESTINATION ${INCLUDE_INSTALL_DIR})

# Create the Kuma3DConfig.cmake file, so other projects can
# easily inlude the engine with a call to find_package.
set(CMAKE_CONFIG_INSTALL_DIR ${ENGINE_INSTALL_DIR}/cmake)
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/Kuma3DConfig.cmake.in
                              ${CMAKE_CONFIG_INSTALL_DIR}/Kuma3DConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_CONFIG_INSTALL_DIR}
                              PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)
