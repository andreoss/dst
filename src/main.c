#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>

#include "context.h"
#include "main.h"
#include "power.h"
#include "report.h"
#include "load.h"
#include "x.h"

void update_status(X *x, Report *report) {
  if (report_is_unsynced(report)) {
    report_pop(report);
    x_setname(x, report_current(report));
  }
}

void update_root(uv_timer_t *handle) {
  char bat[30];
  char buf[256];
  char la[80];
  char layout[8];
  Context *ctx = (Context *)handle->data;
  X *x = context_get(ctx, X11);
  Power *pow = context_get(ctx, POW);
  Report *report = context_get(ctx, REP);
  Load *load = context_get(ctx, LOADAVG);
  load_str(load, la);
  update_status(x, report);
  power_str(pow, bat);
  x_layout(x, layout);
  sprintf(buf, "%s | %s %s", bat, la, layout);
  report_pend(report, buf);
}

void update_on_timer(uv_timer_t *handle) {
  x_update_layout((X *)(context_get((Context *)handle->data, X11)));
}

void close_signal(uv_handle_t *handle) {
  UNUSED(handle);
  printf("bye...\n");
}

void signal_cb(uv_signal_t *handle, int signum) {
  UNUSED(handle);
  UNUSED(signum);
  printf("Signal received!\n");
  if (uv_signal_stop(handle)) {
    printf("Unable to stop signal\n");
  }
  uv_close((uv_handle_t *)handle, close_signal);
}

void setup_timers(Context *ctx, uv_loop_t **loop) {
  static uv_timer_t keyboard_timer;
  static uv_timer_t loadavg_timer;
  static uv_timer_t display_timer;
  static uv_timer_t acpi_timer;
  uv_timer_init(*loop, &keyboard_timer);
  uv_timer_init(*loop, &loadavg_timer);
  uv_timer_init(*loop, &display_timer);
  uv_timer_init(*loop, &acpi_timer);
  acpi_timer.data = context_get(ctx, POW);
  keyboard_timer.data = ctx;
  loadavg_timer.data = ctx;
  display_timer.data = ctx;
  uv_timer_start(&display_timer, update_root, TIMEOUT, 100);
  uv_timer_start(&loadavg_timer, load_on_timer, TIMEOUT, 2500);
  uv_timer_start(&keyboard_timer, update_on_timer, TIMEOUT, 100);
  uv_timer_start(&acpi_timer, power_cb, TIMEOUT, 10000);
}

void setup_signals(uv_loop_t *loop) {
  uv_signal_t int_handle;
  uv_signal_init(loop, &int_handle);
  CHECK(uv_signal_start(&int_handle, signal_cb, SIGHUP), "signal");
}

int main() {
  uv_loop_t *loop = uv_default_loop();
  Context *ctx = context_new();
  context_putw(ctx, X11, x_open(), CALLBACK x_close);
  context_putw(ctx, POW, power_new(loop), CALLBACK power_destroy);
  context_putw(ctx, REP, report_new(), CALLBACK report_destroy);
  context_putw(ctx, LOADAVG, load_new(), CALLBACK load_destroy);
  setup_timers(ctx, &loop);
  setup_signals(loop);
  uv_run(loop, UV_RUN_DEFAULT);
}
