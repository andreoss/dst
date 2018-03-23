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
  MOCKS getloadavg
  )
add_mocked_test(
  power
  LINK_LIBRARIES power ${UV_LIBRARY}
  )
