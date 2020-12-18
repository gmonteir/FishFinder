cmake_minimum_required(VERSION 2.7)

# Name of the project
project(FishFinder)

# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "src/*.cpp" "ext/glad/src/*.c")

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "src/*.h" "ext/glad/*/*.h")
file(GLOB_RECURSE GLSL "resources/*.glsl")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-module")

include_directories("ext/glad/include")

# Set the executable.
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLSL})

# Add GLFW
# Get the GLFW environment variable.
# There should be a CMakeLists.txt in the specified directory.
set(GLFW_DIR "$ENV{GLFW_DIR}")
if(GLFW_DIR)
  message(STATUS "GLFW environment variable found")

  option(GLFW_BUILD_EXAMPLES "GLFW_BUILD_EXAMPLES" OFF)
  option(GLFW_BUILD_TESTS "GLFW_BUILD_TESTS" OFF)
  option(GLFW_BUILD_DOCS "GLFW_BUILD_DOCS" OFF)
  if(CMAKE_BUILD_TYPE MATCHES Release)
    add_subdirectory(${GLFW_DIR} ${GLFW_DIR}/release)
  else()
    add_subdirectory(${GLFW_DIR} ${GLFW_DIR}/debug)
  endif()

  include_directories(${GLFW_DIR}/include)
  target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES})
else()
  message(STATUS "GLFW environment variable `GLFW_DIR` not found, GLFW3 must be installed with the system")

  find_package(PkgConfig)
  if (PKGCONFIG_FOUND)
    message(STATUS "PkgConfig found")
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW_LIBRARIES})
  else()
    message(STATUS "No PkgConfig found")
    find_package(glfw3 REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} glfw)
  endif()
endif()

# Add GLM
# Get the GLM environment variable. Since GLM is a header-only library, we
# just need to add it to the include directory.
set(GLM_INCLUDE_DIR "$ENV{GLM_INCLUDE_DIR}")
if(GLM_INCLUDE_DIR)
  include_directories(${GLM_INCLUDE_DIR})
  message(STATUS "GLM environment variable found")
else()
# If the GLM_INCLUDE_DIR environment variable is not set, we assume
# the user has installed GLM properly on their system
  message(STATUS "GLM environment variable `GLM_INCLUDE_DIR` not found, GLM must be installed with the system")
endif()

# Add Freetype 2 from local source (for best results on Mac)
add_subdirectory("ext/freetype-2.10.0")
target_link_libraries(${CMAKE_PROJECT_NAME} freetype)
include_directories("ext/freetype-2.10.0/include")

#find and add irrKlang dir using environment variable
function(findIrrklang target)
    find_package(irrKlang QUIET)
    if(DEFINED ENV{IRRKLANG_DIR})
        set(IRRKLANG_DIR "$ENV{IRRKLANG_DIR}")
        message(STATUS "IRRKLANG_DIR environment variable found. Attempting use...")
        target_include_directories(${target} PUBLIC "${IRRKLANG_DIR}/include")
        target_link_libraries(${target} "./irrKlang")
    else()
        message(FATAL_ERROR "IRRKLANG_DIR could not be found through find_package or environment varaible 'IRRKLANG_DIR'! irrKlang must be installed!")
    endif()
endfunction(findIrrklang)
### findIrrklang(${CMAKE_PROJECT_NAME})

# OS specific options and libraries
if(WIN32)
  # c++0x is enabled by default.
  # -Wall produces way too many warnings.
  # -pedantic is not supported.
  add_subdirectory("ext/irrKlang-64bit-1.6.0")
  target_link_libraries(${CMAKE_PROJECT_NAME} "./irrKlang")
  target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "ext/irrKlang-64bit-1.6.0/include")
  target_link_libraries(${CMAKE_PROJECT_NAME} opengl32.lib)
else()
  # Enable all pedantic warnings.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -pedantic")
  if(APPLE)
    # irrKlang
    set(IRRKLANG_DIR "${CMAKE_SOURCE_DIR}/ext/irrKlang-64bit-1.6.0")
    include_directories(${IRRKLANG_DIR}/include/)
    include_directories(${IRRKLANG_DIR}/plugins/)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${IRRKLANG_DIR}/bin/macosx-gcc/libirrklang.dylib)
    # Add required frameworks for GLFW.
    target_link_libraries(${CMAKE_PROJECT_NAME} "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
  else()
    #Link the Linux OpenGL library
    target_link_libraries(${CMAKE_PROJECT_NAME} "GL" "dl")
  endif()
endif()
