#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include "context.h"
#include "load.h"

#define TREND_UP   u8"↑"
#define TREND_DOWN u8"↓"

struct load_s {
    bool            valid;
    double          curr[3];
    double          prev[3];
};

Load           *
load_new(void)
{
    return calloc(1, sizeof(Load));
}

static const char *
trend(double current, double previous)
{
    double          d = current - previous;
    if (d < -0.05) {
	return TREND_DOWN;
    } else if (d > 0.05) {
	return TREND_UP;
    }
    return " ";
}

void
load_on_timer(uv_timer_t * timer)
{
    Context        *ctx = timer->data;
    load_update(context_get(ctx, LOADAVG));
}

void
load_update(Load * load)
{
    double          next[3];
    if (getloadavg(next, 3) != 3) {
	return;
    }
    if (!load->valid) {
	memcpy(load->curr, next, sizeof(next));
	memcpy(load->prev, next, sizeof(next));
	load->valid = true;
	return;
    }
    memmove(load->prev, load->curr, sizeof(load->curr));
    memcpy(load->curr, next, sizeof(next));
}

void
load_str(Load * load, char *buff, size_t size)
{
    snprintf(buff, size, "%s%0.2f%s%0.2f%s%0.2f",
	     trend(load->curr[0], load->prev[0]), load->curr[0],
	     trend(load->curr[1], load->prev[1]), load->curr[1],
	     trend(load->curr[2], load->prev[2]), load->curr[2]);
}

void
load_destroy(Load * load)
{
    free(load);
}
