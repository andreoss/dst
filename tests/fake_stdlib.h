#pragma once
#include <string.h>

static double   fk_loadavg[3];

static void
fake_loadavg(double one, double five, double fifteen)
{
    fk_loadavg[0] = one;
    fk_loadavg[1] = five;
    fk_loadavg[2] = fifteen;
}

int             __wrap_getloadavg(double loadavg[], int n);

int
__wrap_getloadavg(double loadavg[], int n)
{
    memcpy(loadavg, fk_loadavg, n * sizeof(double));
    return n;
}
