#include "x.h"

struct display_s {
  Display *dpy;
  Window root;
  int screen;
  char *layout;
};

X *x_open() {
  X *x = (X*)calloc(sizeof(X), 1);
  x->layout = calloc(sizeof(char), 16);
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
  free(x->layout);
  free(x);
}

void x_setname(X *x, const char *s) {
  assert(s);
  XStoreName(x->dpy, x->root, s);
  XSync(x->dpy, x->screen);
}

void x_getname(X *x, char **s) {
  assert(s);
  XFetchName(x->dpy, x->root, s);
}

void x_update_layout(X *x) {
  XkbStateRec state;
  XkbRF_VarDefsRec vd;
  XkbGetState(x->dpy, XkbUseCoreKbd, &state);
  if (XkbRF_GetNamesProp(x->dpy, NULL, &vd)) {
    char *l;
    l = strtok(vd.layout, ",");
    for (int i = 0; i < state.group; i++) {
      l = strtok(NULL, ",");
    }
    if (l == NULL) {
      l = "??";
    }
    strcpy(x->layout, l);
    free(vd.model);
    free(vd.layout);
    free(vd.variant);
    free(vd.options);
  }
}

void x_layout(X *x, char *buff) {
  sprintf(buff, "%s", x->layout);
}
