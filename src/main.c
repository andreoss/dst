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
  Context *ctx = (Context *)handle->data;
  X *x = context_get(ctx, X11);
  Power *pow = context_get(ctx, POW);
  Report *report = context_get(ctx, REP);
  Load *load = context_get(ctx, LA);
  char bat[30];
  char buf[80];
  char la[16];
  load_str(load, la);
  update_status(x, report);
  power_str(pow, bat);
  sprintf(buf, "%s | %s %s", bat, la, x->xkb_layout);
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
  static uv_timer_t kb;
  static uv_timer_t la;
  static uv_timer_t xroot_timer;
  static uv_timer_t bat_timer;
  uv_timer_init(*loop, &kb);
  uv_timer_init(*loop, &la);
  uv_timer_init(*loop, &xroot_timer);
  uv_timer_init(*loop, &bat_timer);
  bat_timer.data = context_get(ctx, POW);
  kb.data = ctx;
  la.data = ctx;
  xroot_timer.data = ctx;
  uv_timer_start(&xroot_timer, update_root, TIMEOUT, 100);
  uv_timer_start(&la, load_on_timer, TIMEOUT, 2500);
  uv_timer_start(&kb, update_on_timer, TIMEOUT, 100);
  uv_timer_start(&bat_timer, power_cb, TIMEOUT, 10000);
}

void setup_signals(uv_loop_t *loop) {
  uv_signal_t int_handle;
  uv_signal_init(loop, &int_handle);
  CHECK(uv_signal_start(&int_handle, signal_cb, SIGHUP), "signal");
}

static void on_recv(uv_udp_t *handle, ssize_t nread, const uv_buf_t *rcvbuf,
                    const struct sockaddr *addr, unsigned flags) {
  UNUSED(flags);
  UNUSED(handle);
  UNUSED(addr);

  if (nread > 0) {
    printf("%lu\n", nread);
    printf("%s", rcvbuf->base);
  }
  printf("free  :%lu %p\n", rcvbuf->len, rcvbuf->base);
  free(rcvbuf->base);
}

static void on_alloc(uv_handle_t *client, size_t suggested_size,
                     uv_buf_t *buf) {
  UNUSED(client);
  buf->base = malloc(suggested_size);
  buf->len = suggested_size;
  printf("malloc:%lu %p\n", buf->len, buf->base);
}
void setup_server(uv_loop_t *loop) {
  int status;
  static struct sockaddr_in addr;
  static uv_udp_t server = {0};
  status = uv_udp_init(loop, &server);
  CHECK(status, "init");
  uv_ip4_addr("0.0.0.0", 12345, &addr);
  status = uv_udp_bind(&server, (const struct sockaddr *)&addr, 0);
  CHECK(status, "bind");
  status = uv_udp_recv_start(&server, on_alloc, on_recv);
  CHECK(status, "recv");
}

int main() {
  uv_loop_t *loop = uv_default_loop();
  Context *ctx = context_new();
  context_putw(ctx, X11, x_open(), CALLBACK x_close);
  context_putw(ctx, POW, power_new(loop), CALLBACK power_destroy);
  context_putw(ctx, REP, report_new(), CALLBACK report_destroy);
  context_putw(ctx, LA,  load_new(), CALLBACK free);

  setup_timers(ctx, &loop);
  // setup_server(loop);
  setup_signals(loop);
  uv_run(loop, UV_RUN_DEFAULT);
}
