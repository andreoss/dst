#pragma once
#include <stddef.h>
#include <stdio.h>
#include <uv.h>

#define POWER_SUPPLY "/sys/class/power_supply/"

typedef struct {
    char            status[32];
    int             capacity;
} Battery;

typedef struct {
    int             online;
    Battery         battery;
    char            ac_name[64];
    char            battery_name[64];
} Power;

Power          *power_new(void);

void            power_destroy(Power * p);

void            power_print(Power * p, FILE * out);
void            power_str(Power * p, char *out, size_t size);
void            power_cb(uv_timer_t * h);
