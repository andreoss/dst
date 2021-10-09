#pragma once
#include <stddef.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

typedef struct {
    Display        *dpy;
    Window          root;
    int             screen;
    char            xkb_layout[64];
} X;

X              *x_open(void);
void            x_close(X * x);
void            x_update_layout(X * x);
char           *x_getname(X * x);
void            x_setname(X * x, const char *s);
