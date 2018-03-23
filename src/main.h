#pragma once
#define UNUSED(x) (void)(x)
#define TIMEOUT 100
#define CHECK(r, msg)                                                          \
  if (r < 0) {                                                                 \
    fprintf(stderr, "%s: %s\n", msg, uv_strerror(r));                          \
    exit(1);                                                                   \
  }
