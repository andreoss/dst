#pragma once
#include <stddef.h>

typedef struct display_s X;

X              *x_open(void);
void            x_close(X * x);
void            x_update_layout(X * x);
void            x_setname(X * x, const char *s);
char           *x_getname(X * x);
void            x_layout(X * x, char *buff, size_t size);
