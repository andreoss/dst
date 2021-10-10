#include "uv.h"
#include <error.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "power.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE           *
dst_fopen(const char *path, const char *mode)
{
    FILE           *fp = fopen(path, mode);
    if (fp == NULL) {
	perror("fopen");
	exit(0);
    }
    return fp;
}
void
dst_slurp(const char *path, const char *format, void *ptr)
{
    FILE           *fp = dst_fopen(path, "r");
    int             res = fscanf(fp, format, (char *) ptr);
    assert(res == 1);
    fclose(fp);
}

void
power_req_handle(uv_fs_t * req)
{
    assert(req->path);
    char            buff[8];
    char            path[256];
    Power          *power = ((Power *) req->data);
    uv_dirent_t     ent = { 0 };
    while (!uv_fs_scandir_next(req, &ent)) {
	strcpy(path, req->path);
	strcat(path, ent.name);
	strcat(path, "/type");
	dst_slurp(path, "%s", buff);
	if (strcmp(buff, "Battery") == 0) {
	    strcpy(path, req->path);
	    strcat(path, ent.name);
	    strcat(path, "/capacity");
	    dst_slurp(path, "%d", &power->battery->capacity);
	    strcpy(path, req->path);
	    strcat(path, ent.name);
	    strcat(path, "/status");
	    dst_slurp(path, "%s", power->battery->status);
	} else if (strcmp(buff, "Mains") == 0) {
	    strcpy(power->ac_name, ent.name);
	    strcpy(path, req->path);
	    strcat(path, ent.name);
	    strcat(path, "/online");
	    dst_slurp(path, "%d", &power->online);
	}
    }
    uv_fs_req_cleanup(req);
}

void
power_cb(uv_timer_t * h)
{
    static uv_fs_t  req;	// = calloc(sizeof(uv_fs_t), 1);
    req.data = h->data;
    uv_fs_scandir(h->loop, &req, POWER_SUPPLY, O_RDONLY, power_req_handle);
}

Power          *
power_new()
{
    Power          *p = calloc(1, sizeof(Power));
    p->req = calloc(1, sizeof(uv_fs_t));
    p->battery = calloc(1, sizeof(Battery));
    p->battery->status = calloc(16, sizeof(char));
    p->battery_name = calloc(8, sizeof(char));
    p->ac_name = calloc(8, sizeof(char));
    return p;
}

void
power_destroy(Power * p)
{
    uv_fs_req_cleanup(p->req);
    free(p->req);
    free(p->battery);
    free(p->battery_name);
    free(p->ac_name);
}

void
power_str(Power * p, char *out)
{
    if (p->online && p->battery->capacity > 95) {
	sprintf(out, "%s", "");
    } else {
	sprintf(out, "%s %s | %s %s %d", p->ac_name,
		p->online ? "On" : "Off", p->battery_name,
		p->battery->status, p->battery->capacity);
    }
}

void
power_print(Power * p, FILE * out)
{
    char            buf[128];
    power_str(p, buf);
    fputs(buf, out);
}
