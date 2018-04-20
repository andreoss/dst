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
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void on_open(uv_fs_t *req) {
  FILE *fp;
  char buff[8];
  char path[256];
  Power *power = ((Power *)req->data);
  uv_dirent_t *ent = calloc(sizeof(uv_dirent_t), 1);
  fprintf(stderr, "%s\n", req->path);
  while (!uv_fs_scandir_next(req, ent)) {
    strcpy(path, req->path);
    strcat(path, ent->name);
    strcat(path, "/type");
    fp = fopen(path, "r");
    fscanf(fp, "%s", buff);
    fclose(fp);
    if (strcmp(buff, "Battery") == 0) {
      strcpy(path, req->path);
      strcat(path, ent->name);
      strcat(path, "/capacity");
      strcpy(power->battery_name, ent->name);
      fp = fopen(path, "r");
      assert(fp);
      assert(power->battery != NULL);
      fscanf(fp, "%d", &power->battery->capacity);
      strcpy(path, req->path);
      strcat(path, ent->name);
      strcat(path, "/status");
      strcpy(power->battery_name, ent->name);
      fp = fopen(path, "r");
      assert(fp);
      assert(power->battery != NULL);
      fscanf(fp, "%s", power->battery->status);
    } else if (strcmp(buff, "Mains") == 0) {
      strcpy(path, req->path);
      strcat(path, ent->name);
      strcat(path, "/online");
      fp = fopen(path, "r");
      assert(fp);
      assert(power->battery != NULL);
      fscanf(fp, "%d", &power->online);
      strcpy(power->ac_name, ent->name);
    }
  }
  free(ent);
  fclose(fp);
}

void power_cb(uv_timer_t *h) {
  Power *p = (Power*)h->data;
  uv_fs_t req = {.data = p};
  uv_fs_scandir(h->loop, &req, POWER_SUPPLY, O_RDONLY, on_open);
  power_print(p, stderr);
}

Power *power_new(uv_loop_t *loop) {
  Power *p = calloc(sizeof(Power), 1);
  p->req = calloc(sizeof(uv_fs_t), 1);
  p->battery = calloc(sizeof(Battery), 1);
  p->battery->status = calloc(sizeof(char), 16);
  p->battery_name = calloc(sizeof(char), 8);
  p->ac_name = calloc(sizeof(char), 8);
  return p;
}

void power_destroy(Power *p) {
  uv_fs_req_cleanup(p->req);
  free(p->req);
  free(p->battery);
  free(p->battery_name);
  free(p->ac_name);
}

void power_str(Power *p, char *out) {
  sprintf(out, "%s %s | %s %s %d", p->ac_name, p->online ? "On" : "Off",
          p->battery_name, p->battery->status, p->battery->capacity);
}
void power_print(Power *p, FILE *out) {
  fprintf(out, "%s ", p->ac_name);
  fprintf(out, "%s |", p->online ? "On" : "Off");
  if (p->battery != NULL) {
    fprintf(out, "%s ", p->battery_name);
    fprintf(out, "%s ", p->battery->status);
    fprintf(out, "%d ", p->battery->capacity);
  }
  fprintf(out, "\n");
}
/* int main() { */
/*   uv_loop_t *loop = uv_default_loop(); */
/*   Power *power = power_new(loop); */
/*   uv_run(loop, UV_RUN_DEFAULT); */
/*   power_print(power, stdout); */
/*   power_destroy(power); */
/* } */
