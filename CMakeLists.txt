cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(dst C)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra -pedantic -Werror)

include(GNUInstallDirs)
find_package(X11 REQUIRED)
find_path(UV_INCLUDE_DIR uv.h REQUIRED)
find_library(UV_LIBRARY uv REQUIRED)
find_path(XKBFILE_INCLUDE_DIR X11/extensions/XKBrules.h REQUIRED)
find_library(XKBFILE_LIBRARY xkbfile REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
add_subdirectory(src)

include(CTest)
if(BUILD_TESTING)
  find_package(CMocka CONFIG REQUIRED)
  include(AddCMockaTest)
  include(AddMockedTest)
  add_subdirectory(tests)
endif()
