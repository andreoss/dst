#include "load.h"
#include "context.h"
#include <stdlib.h>
#include <string.h>
#include <uv.h>

struct load_s {
  double curr[3];
  double prev[3];
};
Load *load_new() { return calloc(sizeof(Load), 1); }
void load_on_timer(uv_timer_t *timer) {
  load_update(context_get(timer->data, LA));
}

void load_update(Load *load) {
  memmove(load->curr, load->prev, sizeof(double) * 3);
  getloadavg(load->curr, 3);
}

bool load_cmp(Load *load, double tt) {
  return load->curr[0] > tt || load->curr[1] > tt || load->curr[1] > tt;
}

void load_str(Load *load, char *buff) {
  sprintf(buff, "%0.2f %0.2f %0.2f", load->curr[0], load->curr[1], load->curr[2]);
}
