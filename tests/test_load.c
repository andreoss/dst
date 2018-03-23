#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>

#include "fake_stdlib.h"
#include "load.h"

static void
as_string_without_sync(void **state)
{
    (void) state;
    Load           *load = load_new();
    assert_non_null(load);
    char            buf[100];
    load_str(load, buf, sizeof(buf));
    assert_string_equal(buf, "0.00 0.00 0.00");
    load_destroy(load);
}

static void
as_string_after_sync(void **state)
{
    (void) state;
    Load           *load = load_new();
    assert_non_null(load);
    char            buf[100];
    fake_loadavg(0.5, 0.5, 0.5);
    load_update(load);
    load_str(load, buf, sizeof(buf));
    assert_string_equal(buf, "0.50 0.50 0.50");
    load_destroy(load);
}

int
main(void)
{
    const struct CMUnitTest tests[] = {
	cmocka_unit_test(as_string_without_sync),
	cmocka_unit_test(as_string_after_sync)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
