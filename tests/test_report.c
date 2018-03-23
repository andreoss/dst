#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "report.h"

static void
when_differs_should_be_unsynced(void **state)
{
    (void) state;
    Report         *rep = report_new();
    assert_non_null(rep);
    report_pend(rep, "???");
    assert_true(report_is_unsynced(rep));
    report_destroy(rep);
}

static void
when_differs_should_be_synced(void **state)
{
    (void) state;
    Report         *rep = report_new();
    assert_non_null(rep);
    report_pend(rep, "???");
    report_pop(rep);
    assert_string_equal(report_current(rep), "???");
    report_destroy(rep);
}

static void
becomes_synced_when_popped(void **state)
{
    (void) state;
    Report         *rep = report_new();
    assert_non_null(rep);
    report_pend(rep, "hi there");
    assert_true(report_is_unsynced(rep));
    report_pop(rep);
    assert_false(report_is_unsynced(rep));
    report_destroy(rep);
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
