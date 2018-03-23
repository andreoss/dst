#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include "context.h"

static void
when_added(void **state)
{
    (void) state;
    Context        *ctx = context_new(1);
    assert_non_null(ctx);
    context_put(ctx, 0, "???");
    assert_string_equal(context_get(ctx, 0), "???");
    context_destroy(ctx);
}

static char     before_clean[10];

static void
clean(void *ptr)
{
    strcpy(before_clean, (char *) ptr);
    free(ptr);
}

static void
created_and_destroyed_with_cb(void **state)
{
    (void) state;
    Context        *ctx = context_new(1);
    assert_non_null(ctx);
    char           *c = calloc(10, sizeof(char));
    assert_non_null(c);
    strcpy(c, "xxx");
    context_putw(ctx, 0, c, clean);
    context_destroy(ctx);
    assert_string_equal(before_clean, "xxx");
}

static void
created_and_destroyed(void **state)
{
    (void) state;
    Context        *ctx = context_new(1);
    assert_non_null(ctx);
    context_put(ctx, 0, "???");
    context_destroy(ctx);
}

int
main(void)
{
    const struct CMUnitTest tests[] = {
	cmocka_unit_test(when_added),
	cmocka_unit_test(created_and_destroyed),
	cmocka_unit_test(created_and_destroyed_with_cb)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
