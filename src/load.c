#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include "context.h"
#include "load.h"

struct load_s {
    double          curr[3];
};

Load           *
load_new(void)
{
    return calloc(1, sizeof(Load));
}

void
load_on_timer(uv_timer_t * timer)
{
    Context        *ctx = timer->data;
    load_update(context_get(ctx, LA));
}

void
load_update(Load * load)
{
    if (getloadavg(load->curr, 3) != 3) {
	return;
    }
}

void
load_str(Load * load, char *buff, size_t size)
{
    snprintf(buff, size, "%0.2f %0.2f %0.2f", load->curr[0],
	     load->curr[1], load->curr[2]);
}

void
load_destroy(Load * load)
{
    free(load);
}
