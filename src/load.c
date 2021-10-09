#include "load.h"
#include "context.h"
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define TREND_UP   u8"↑"
#define TREND_DOWN u8"↓"

struct load_s {
  double curr[3];
  double prev[3];
};

Load *load_new() {
  return calloc(sizeof(Load), 1);
}

void load_on_timer(uv_timer_t *timer) {
  load_update(context_get(timer->data, LA));
}

char* trend(double d) {
  if (d < -0.05) {
    return TREND_DOWN;
  }
  else if (d > 0.05) {
    return TREND_UP;
  }
  return " ";
}

void load_update(Load *load) {
  memmove(load->prev, load->curr, sizeof(double) * 3);
  getloadavg(load->curr, 3);
}

bool load_cmp(Load *load, double tt) {
  return load->curr[0] > tt || load->curr[1] > tt || load->curr[2] > tt;
}

void load_str(Load *load, char *buff) {
  sprintf(
     buff,
    "%s%0.2f%s%0.2f%s%0.2f",
          trend(load->curr[0] - load->prev[0]),
          load->curr[0],
          trend(load->curr[1] - load->prev[1]),
          load->curr[1],
          trend(load->curr[2] - load->prev[2]),
          load->curr[2]
   );
}
