#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#include "x.h"

struct display_s {
    Display        *dpy;
    Window          root;
    int             screen;
    char            layout[64];
};

X              *
x_open(void)
{
    X              *x = calloc(1, sizeof(X));
    if (x == NULL) {
	return NULL;
    }
    x->dpy = XOpenDisplay(NULL);
    if (x->dpy == NULL) {
	free(x);
	return NULL;
    }
    x->screen = DefaultScreen(x->dpy);
    x->root = RootWindow(x->dpy, x->screen);
    return x;
}

void
x_close(X * x)
{
    if (x == NULL) {
	return;
    }
    XCloseDisplay(x->dpy);
    free(x);
}

void
x_setname(X * x, const char *s)
{
    if (x == NULL || s == NULL) {
	return;
    }
    XStoreName(x->dpy, x->root, s);
    XSync(x->dpy, False);
}

char           *
x_getname(X * x)
{
    char           *s = NULL;
    if (x == NULL || !XFetchName(x->dpy, x->root, &s)) {
	return NULL;
    }
    return s;
}

void
x_update_layout(X * x)
{
    XkbStateRec     state;
    XkbRF_VarDefsRec vd;
    if (x == NULL) {
	return;
    }
    if (XkbGetState(x->dpy, XkbUseCoreKbd, &state) != Success) {
	return;
    }
    if (!XkbRF_GetNamesProp(x->dpy, NULL, &vd)) {
	return;
    }
    char           *l = vd.layout ? strtok(vd.layout, ",") : NULL;
    for (int i = 0; i < state.group && l != NULL; i++) {
	l = strtok(NULL, ",");
    }
    snprintf(x->layout, sizeof(x->layout), "%s", l ? l : "??");
    free(vd.model);
    free(vd.layout);
    free(vd.variant);
    free(vd.options);
}

void
x_layout(X * x, char *buff, size_t size)
{
    if (x == NULL || buff == NULL || size == 0) {
	return;
    }
    snprintf(buff, size, "%s", x->layout);
}
