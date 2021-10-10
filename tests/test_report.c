#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <report.h>

static void
when_differs_should_be_unsynced(void **state)
{
    Report         *rep = report_new();
    report_pend(rep, "???");
    assert_true(report_is_unsynced(rep));
}
static void
when_differs_should_be_synced(void **state)
{
    Report         *rep = report_new();
    report_pend(rep, "???");
    report_pop(rep);
    assert_string_equal(report_current(rep), "???");
}

static void
becomes_synced_when_popped(void **state)
{
    Report         *rep = report_new();
    report_pend(rep, "hi there");
    assert_true(report_is_unsynced(rep));
    report_pop(rep);
    assert_false(report_is_unsynced(rep));
}
int
main(void)
{
    const struct CMUnitTest tests[] = {
	cmocka_unit_test(when_differs_should_be_synced),
	cmocka_unit_test(when_differs_should_be_unsynced),
	cmocka_unit_test(becomes_synced_when_popped)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
