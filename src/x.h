#pragma once
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>
#include <string.h>

typedef struct display_s X;

X* x_open();
void x_close(X *x);
void x_update_layout(X *x);
void x_setname(X *x, const char *s) ;
void x_layout(X *x, char *buff);
