#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include "context.h"
#include "load.h"
#include "main.h"
#include "power.h"
#include "report.h"
#include "x.h"

static void
destroy_x(void *ptr)
{
    x_close(ptr);
}

static void
destroy_power(void *ptr)
{
    power_destroy(ptr);
}

static void
destroy_report(void *ptr)
{
    report_destroy(ptr);
}

static void
destroy_load(void *ptr)
{
    load_destroy(ptr);
}

void
update_status(X * x, Report * report)
{
    if (report_is_unsynced(report)) {
	report_pop(report);
	x_setname(x, report_current(report));
    }
}

void
update_root(uv_timer_t * handle)
{
    char            bat[128];
    char            buf[512];
    char            la[96];
    char            layout[32];
    Context        *ctx = handle->data;
    X              *x = context_get(ctx, X11);
    Power          *pow = context_get(ctx, POW);
    Report         *report = context_get(ctx, REP);
    Load           *load = context_get(ctx, LOADAVG);

    load_str(load, la, sizeof(la));
    power_str(pow, bat, sizeof(bat));
    x_layout(x, layout, sizeof(layout));
    snprintf(buf, sizeof(buf), "%s | %s %s", bat, la, layout);
    report_pend(report, buf);
    update_status(x, report);
}

void
update_on_timer(uv_timer_t * handle)
{
    Context        *ctx = handle->data;
    x_update_layout(context_get(ctx, X11));
}

void
signal_cb(uv_signal_t * handle, int signum)
{
    Context        *ctx = handle->data;
    X              *x = context_get(ctx, X11);

    if (signum == SIGHUP) {
	x_update_layout(x);
	return;
    }
    x_setname(x, "");
    uv_stop(handle->loop);
}

void
setup_timers(Context * ctx, uv_loop_t * loop)
{
    static uv_timer_t keyboard_timer;
    static uv_timer_t loadavg_timer;
    static uv_timer_t display_timer;
    static uv_timer_t acpi_timer;

    CHECK(uv_timer_init(loop, &keyboard_timer), "uv_timer_init");
    CHECK(uv_timer_init(loop, &loadavg_timer), "uv_timer_init");
    CHECK(uv_timer_init(loop, &display_timer), "uv_timer_init");
    CHECK(uv_timer_init(loop, &acpi_timer), "uv_timer_init");

    acpi_timer.data = context_get(ctx, POW);
    keyboard_timer.data = ctx;
    loadavg_timer.data = ctx;
    display_timer.data = ctx;

    CHECK(uv_timer_start(&loadavg_timer, load_on_timer, 0, 2500),
	  "uv_timer_start");
    CHECK(uv_timer_start(&acpi_timer, power_cb, 0, 10000),
	  "uv_timer_start");
    CHECK(uv_timer_start(&keyboard_timer, update_on_timer, 0, TIMEOUT),
	  "uv_timer_start");
    CHECK(uv_timer_start(&display_timer, update_root, TIMEOUT, TIMEOUT),
	  "uv_timer_start");
}

void
setup_signals(Context * ctx, uv_loop_t * loop)
{
    static uv_signal_t hup;
    static uv_signal_t usr;
    static uv_signal_t term;

    CHECK(uv_signal_init(loop, &hup), "uv_signal_init");
    hup.data = ctx;
    CHECK(uv_signal_start(&hup, signal_cb, SIGHUP), "uv_signal_start");

    CHECK(uv_signal_init(loop, &usr), "uv_signal_init");
    usr.data = ctx;
    CHECK(uv_signal_start(&usr, signal_cb, SIGUSR1), "uv_signal_start");

    CHECK(uv_signal_init(loop, &term), "uv_signal_init");
    term.data = ctx;
    CHECK(uv_signal_start(&term, signal_cb, SIGTERM), "uv_signal_start");
}

int
main(void)
{
    Context        *ctx = context_new(CONTEXT_SIZE);
    if (ctx == NULL) {
	fprintf(stderr, "context_new: out of memory\n");
	return 1;
    }

    X              *x = x_open();
    if (x == NULL) {
	fprintf(stderr, "x_open: cannot open display\n");
	context_destroy(ctx);
	return 1;
    }

    context_putw(ctx, X11, x, destroy_x);
    context_putw(ctx, POW, power_new(), destroy_power);
    context_putw(ctx, REP, report_new(), destroy_report);
    context_putw(ctx, LOADAVG, load_new(), destroy_load);

    if (context_get(ctx, POW) == NULL || context_get(ctx, REP) == NULL
	|| context_get(ctx, LOADAVG) == NULL) {
	fprintf(stderr, "initialization: out of memory\n");
	context_destroy(ctx);
	return 1;
    }

    uv_loop_t       loop;
    CHECK(uv_loop_init(&loop), "uv_loop_init");
    setup_timers(ctx, &loop);
    setup_signals(ctx, &loop);
    x_update_layout(x);
    CHECK(uv_run(&loop, UV_RUN_DEFAULT), "uv_run");
    context_destroy(ctx);
    CHECK(uv_loop_close(&loop), "uv_loop_close");
    return 0;
}
