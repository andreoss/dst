#pragma once
#include "uv.h"
#include <error.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include <assert.h>
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <check.h>

#define POWER_SUPPLY "/sys/class/power_supply/"

typedef struct {
  char* status;
  int capacity;
} Battery;

typedef struct {
  int online;
  Battery *battery;
  char *ac_name;
  char *battery_name;
  uv_fs_t *req;
} Power;

void on_open(uv_fs_t *req);

Power *power_new(uv_loop_t *loop);

void power_destroy(Power *p);

void power_print(Power *p, FILE *out);
void power_str(Power *p, char *out);
void power_cb(uv_timer_t *h);
