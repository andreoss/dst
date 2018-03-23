#pragma once
#include <uv.h>
#include <stdbool.h>
typedef struct load_s Load;

Load *load_new();
void load_str(Load *load, char *buff);
bool load_cmp(Load *load, double tt);
void load_on_timer(uv_timer_t *h);
void load_update(Load *load);
