add_library(x OBJECT x.c x.h)
add_library(power OBJECT power.c power.h)
add_library(report OBJECT report.c report.h)
add_library(context OBJECT context.c context.h)
add_library(load OBJECT load.c load.h)

target_include_directories(x PUBLIC ${X11_INCLUDE_DIR} ${XKBFILE_INCLUDE_DIR})
target_include_directories(power PUBLIC ${UV_INCLUDE_DIR})
target_include_directories(load PUBLIC ${UV_INCLUDE_DIR})

add_executable(dst main.c main.h)
target_link_libraries(dst power report context x ${X11_LIBRARIES} ${UV_LIBRARY} ${XKBFILE_LIBRARY} load)
install(TARGETS dst RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
