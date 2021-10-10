#include "fake_stdlib.h"
#include "io.h"
#include "main.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
new_and_destory(void **state)
{
    UNUSED(state);
    Io             *io = io_new();
    assert_non_null(io);
    io_destroy(io);
}

/*
 * static void reports_size(void **state) { 
 */
/*
 * UNUSED(state); 
 */
/*
 * char buf[256]; 
 */
/*
 * Io *io = io_new(); 
 */
/*
 * io_sample(io); 
 */
/*
 * io_print(io, buf); 
 */
/*
 * assert_string_equal(buf, "???"); 
 */
/*
 * io_destroy(io); 
 */
/*
 * } 
 */

int
main(void)
{
    const struct CMUnitTest tests[] = {
	cmocka_unit_test(new_and_destory),
	// cmocka_unit_test(reports_size)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
