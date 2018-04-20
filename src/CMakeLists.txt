set(LIB_SOURCES
  power.c
  x.c
)

set(HEADERS
  ${CONFIG_HEADER}
  power.h
  x.h
)

set(MAIN_SOURCES
  main.c
)

add_library(power STATIC ${LIB_SOURCES} ${HEADERS})
add_executable(main ${HEADERS} ${MAIN_SOURCES})
target_link_libraries(main power X11 uv xkbfile)

install(TARGETS power
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/power.h DESTINATION include)
