#pragma once
#define UNUSED(x) (void)(x)
#define TIMEOUT 1000
#define CHECK(r, msg)                                                          \
  if (r < 0) {                                                                 \
    fprintf(stderr, "%s: %s\n", msg, uv_strerror(r));                          \
    exit(1);                                                                   \
  }


typedef struct {
  double _1m;
  double _5m;
  double _15m;
} Load;

typedef struct {
  char *current;
  char *pending;
} Report;
