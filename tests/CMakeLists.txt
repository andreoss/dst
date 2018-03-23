include_directories (${TEST_SOURCE_DIR}/src)
add_compile_options(-Wno-error=unused-parameter)
add_mocked_test(
  report
  LINK_LIBRARIES report
  )
add_mocked_test(
  context
  LINK_LIBRARIES context
  )
add_mocked_test(
  load
  LINK_LIBRARIES load context
  )
add_mocked_test(
  io
  LINK_LIBRARIES io
  )
