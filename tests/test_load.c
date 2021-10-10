#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "load.h"
#include <string.h>
#include "fake_stdlib.h"

static void
as_string_without_sync(void **state)
{
    Load           *load = load_new();
    char            buf[100];
    load_str(load, buf);
    assert_string_equal(buf, " 0.00 0.00 0.00");
}
static void
as_string_after_sync(void **state)
{
    Load           *load = load_new();
    char            buf[100];
    fake_loadavg(0.5, 0.5, 0.5);
    load_update(load);
    fake_loadavg(0.0, 0.0, 0.0);
    load_update(load);
    load_str(load, buf);
    assert_string_equal(buf, "↓0.00↓0.00↓0.00");
}

static void
as_string_after_second_sync(void **state)
{
    Load           *load = load_new();
    char            buf[100];
    fake_loadavg(0.5, 0.5, 0.5);
    load_update(load);
    fake_loadavg(1.0, 1.0, 1.0);
    load_update(load);
    load_str(load, buf);
    assert_string_equal(buf, "↑1.00↑1.00↑1.00");
}

int
main(void)
{
    const struct CMUnitTest tests[] = {
	cmocka_unit_test(as_string_without_sync),
	cmocka_unit_test(as_string_after_sync),
	cmocka_unit_test(as_string_after_second_sync)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
