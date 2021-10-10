#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <uv.h>

typedef struct load_s Load;

Load           *load_new(void);
void            load_str(Load * load, char *buff, size_t size);
void            load_on_timer(uv_timer_t * h);
void            load_update(Load * load);
void            load_destroy(Load * load);
