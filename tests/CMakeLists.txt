include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(TEST_SOURCES
  power_test.c
)

add_library(power_test power_test.c)
add_executable(all_tests all_tests.c)
target_link_libraries(all_tests power_test X11 uv xkbfile power ${CGREEN_LIBRARIES})
