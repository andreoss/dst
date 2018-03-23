#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include "power.h"

static void
new_and_destroy(void **state)
{
    (void) state;
    Power          *p = power_new();
    assert_non_null(p);
    power_destroy(p);
}

static void
status_is_truncated(void **state)
{
    (void) state;
    Power          *p = power_new();
    assert_non_null(p);
    memset(p->ac_name, 'A', sizeof(p->ac_name) - 1);
    memset(p->battery_name, 'B', sizeof(p->battery_name) - 1);
    memset(p->battery.status, 'S', sizeof(p->battery.status) - 1);
    p->battery.capacity = 100;
    char            buf[16];
    power_str(p, buf, sizeof(buf));
    assert_true(strlen(buf) < sizeof(buf));
    power_destroy(p);
}

int
main(void)
{
    const struct CMUnitTest tests[] = {
	cmocka_unit_test(new_and_destroy),
	cmocka_unit_test(status_is_truncated),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
