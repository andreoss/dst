#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#define TIMEOUT 100
#define CHECK(expr, msg)                                                       \
  do {                                                                         \
    int check_result = (expr);                                                 \
    if (check_result < 0) {                                                    \
      fprintf(stderr, "%s: %s\n", msg, uv_strerror(check_result));             \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)
