#pragma once
#include <sys/stat.h>

typedef struct io_s Io;
Io *io_new();
void io_destroy(Io* io);
void io_sample(Io* io);

void io_print(Io *io, char *out);
