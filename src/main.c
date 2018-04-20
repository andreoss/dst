#include <uv.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>

#include "main.h"
#include "x.h"
#include "power.h"
void report_new(Report *rep) {
  rep->current = calloc(sizeof(char), 100);
  rep->pending = calloc(sizeof(char), 100);
}

void report_destroy(Report *rep) {
  free(rep->current);
  free(rep->pending);
}

int report_unsync(Report *rep) {
  return strcmp(rep->current, rep->pending)!=0;
}
static Report report;
static Load load;
static char *status;

void load_sync() {
  static double loadavg[3];
  uv_loadavg(loadavg);
  load._1m = loadavg[0];
  load._5m = loadavg[1];
  load._15m = loadavg[2];
}

void report_pop(Report *report) {
  memmove(report->current, report->pending, 100);
}

void update_status(X *x, Report *report) {
  if (report_unsync(report)) {
    report_pop(report);
    fprintf(stdout, "%s\n", report->current);
    x_setname(x, report->current);
  }
}
int loadcmp(Load *load, double tt) {
  return load->_1m > tt || load->_5m > tt || load->_15m > tt;
}


void update_root(uv_timer_t *handle) {
  X *x = (X*)(((void**)handle->data)[0]);
  //Power *pow = (Power*)((((void**)handle->data)[1]));
  //assert(pow);
  if (loadcmp(&load, 0.5)) {
    sprintf(report.pending, "%0.2f %0.2f %0.2f %s", load._1m, load._5m,
            load._15m, x->xkb_layout);
  } else {
    sprintf(report.pending, "%s", x->xkb_layout);
  }
  update_status((X*)handle->data, &report);
}

void update_layout(uv_timer_t *handle) {
  x_update_layout((X*)handle->data);
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

void setup_timers(X *x, Power *p, uv_loop_t **loop) {
  static uv_timer_t kb;
  static uv_timer_t la;
  static uv_timer_t xroot_timer;
  static uv_timer_t bat_timer;
  uv_timer_init(*loop, &kb);
  uv_timer_init(*loop, &la);
  uv_timer_init(*loop, &xroot_timer);
  uv_timer_init(*loop, &bat_timer);
  bat_timer.data = p;
  kb.data = x;
  xroot_timer.data = (void *[]){x, p};
  uv_timer_start(&xroot_timer, update_root, TIMEOUT, 100);
  uv_timer_start(&la, load_sync, TIMEOUT, 2500);
  uv_timer_start(&kb, update_layout, TIMEOUT, 100);
  uv_timer_start(&bat_timer, power_cb, TIMEOUT, 100);
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
  X *x = x_open();
  Power *pow = power_new(loop);
  report_new(&report);
  setup_timers(x, pow, &loop);
  // setup_server(loop);
  setup_signals(loop);
  uv_run(loop, UV_RUN_DEFAULT);
  x_close(x);
  power_destroy(pow);
  report_destroy(&report);
}
