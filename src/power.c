#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "power.h"

static bool
dst_slurp(const char *path, const char *format,...)
{
    FILE           *fp = fopen(path, "r");
    if (fp == NULL) {
	return false;
    }
    va_list         args;
    va_start(args, format);
    int             res = vfscanf(fp, format, args);
    va_end(args);
    fclose(fp);
    return res == 1;
}

static char    *
supply_path(char *path, size_t size, const char *base, const char *name,
	    const char *suffix)
{
    int             n = snprintf(path, size, "%s%s%s", base, name, suffix);
    if (n < 0 || (size_t) n >= size) {
	return NULL;
    }
    return path;
}

void
power_req_handle(uv_fs_t * req)
{
    assert(req->path);
    Power          *power = req->data;
    uv_dirent_t     ent = { 0 };
    char            path[PATH_MAX];
    char            type[16];

    power->online = 0;
    power->ac_name[0] = '\0';
    power->battery_name[0] = '\0';
    power->battery.capacity = 0;
    power->battery.status[0] = '\0';

    while (!uv_fs_scandir_next(req, &ent)) {
	if (supply_path(path, sizeof(path), req->path, ent.name, "/type")
	    == NULL) {
	    continue;
	}
	if (!dst_slurp(path, "%15s", type)) {
	    continue;
	}
	if (strcmp(type, "Battery") == 0) {
	    snprintf(power->battery_name, sizeof(power->battery_name), "%s",
		     ent.name);
	    if (supply_path(path, sizeof(path), req->path, ent.name,
			    "/capacity") != NULL) {
		dst_slurp(path, "%d", &power->battery.capacity);
	    }
	    if (supply_path(path, sizeof(path), req->path, ent.name,
			    "/status") != NULL) {
		dst_slurp(path, "%31s", power->battery.status);
	    }
	} else if (strcmp(type, "Mains") == 0) {
	    snprintf(power->ac_name, sizeof(power->ac_name), "%s", ent.name);
	    if (supply_path(path, sizeof(path), req->path, ent.name,
			    "/online") != NULL) {
		dst_slurp(path, "%d", &power->online);
	    }
	}
    }
    uv_fs_req_cleanup(req);
}

void
power_cb(uv_timer_t * h)
{
    static uv_fs_t  req;
    req.data = h->data;
    uv_fs_scandir(h->loop, &req, POWER_SUPPLY, O_RDONLY, power_req_handle);
}

Power          *
power_new(void)
{
    return calloc(1, sizeof(Power));
}

void
power_destroy(Power * p)
{
    free(p);
}

void
power_str(Power * p, char *out, size_t size)
{
    if (p == NULL || out == NULL || size == 0) {
	return;
    }
    if (p->online && p->battery.capacity > 95) {
	out[0] = '\0';
	return;
    }
    snprintf(out, size, "%s %s | %s %s %d", p->ac_name,
	     p->online ? "On" : "Off", p->battery_name,
	     p->battery.status, p->battery.capacity);
}

void
power_print(Power * p, FILE * out)
{
    char            buf[192];
    power_str(p, buf, sizeof(buf));
    fputs(buf, out);
}
