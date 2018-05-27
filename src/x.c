#include "x.h"
X *x_open() {
  X *x = (X*)calloc(sizeof(X), 1);
  x->xkb_layout = calloc(sizeof(char), 16);
  x->dpy = XOpenDisplay(NULL);
  if (x->dpy == NULL) {
    error(1, 0, "Cannot open display\n");
  }
  x->screen = DefaultScreen(x->dpy);
  x->root = RootWindow(x->dpy, x->screen);
  return x;
}

void x_close(X *x) {
  XCloseDisplay(x->dpy);
  free(x->xkb_layout);
  free(x);
}

void x_setname(X *x, const char *s) {
  assert(s);
  XStoreName(x->dpy, x->root, s);
  XSync(x->dpy, x->screen);
}

void x_update_layout(X *x) {
  XkbStateRec state;
  XkbRF_VarDefsRec vd;
  XkbGetState(x->dpy, XkbUseCoreKbd, &state);
  char *l;
  if (XkbRF_GetNamesProp(x->dpy, NULL, &vd)) {
    l = strtok(vd.layout, ",");
    for (int i = 0; i < state.group; i++) {
      l = strtok(NULL, ",");
    }
    if (l == NULL) {
      l = "??";
    }
    strcpy(x->xkb_layout, l);
    free(vd.model);
    free(vd.layout);
    free(vd.variant);
    free(vd.options);
  }
}
