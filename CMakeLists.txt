project(dst C)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_BUILD_TYPE Debug)
find_package (PkgConfig)
find_package(cgreen)
pkg_check_modules (PC_Libuv QUIET libuv)
find_package(X11 REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
add_test(NAME all_tests COMMAND all_tests)
