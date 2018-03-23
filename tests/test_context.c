#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "context.h"
#include <string.h>

static void when_added(void **state) {
  Context *ctx = context_new(1);
  context_put(ctx, 0, "???");
  assert_string_equal(context_get(ctx, 0), "???");
}


char before_clean[10];
void clean(void *ptr) {
  strcpy(before_clean, (char*)ptr);
  free(ptr);
}
static void created_and_destroyed_with_cb(void **state) {
  Context *ctx = context_new(1);
  char *c = calloc(10, sizeof(char));
  strcpy(c, "xxx");
  context_putw(ctx, 0, c, clean);
  context_destroy(ctx);
  assert_string_equal(before_clean, "xxx");
  assert_string_not_equal(context_get(ctx, 0), "xxx");
}
static void created_and_destroyed(void **state) {
  Context *ctx = context_new(1);
  char *c = "???";
  context_put(ctx, 0, c);
  context_destroy(ctx);
  assert_string_not_equal(context_get(ctx, 0), c);
}
int main(void)
{
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(when_added),
      cmocka_unit_test(created_and_destroyed),
      cmocka_unit_test(created_and_destroyed_with_cb)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
