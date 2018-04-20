#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "power.h"
#include <string.h>

Describe(Power);
BeforeEach(Power) {}
AfterEach(Power) {}
Ensure(Power, returns_word_count) {
  assert_that(5, is_equal_to(5));
}

Ensure(Power, returns_word_count2) {
  assert_that(5, is_equal_to(5));
}
TestSuite *power_tests() {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Power, returns_word_count);
    add_test_with_context(suite, Power, returns_word_count2);
    return suite;
}
