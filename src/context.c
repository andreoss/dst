#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "context.h"

struct Context {
  size_t size;
  void** data;
  callback_t** destroyers;
};

void* context_get(Context *ctx, size_t pos) {
  assert(ctx);
  assert(ctx->size > pos);
  return ctx->data[pos];
}

Context *context_new() {
  size_t s = 5;
  Context *ctx = (Context*)calloc(100, sizeof(Context));
  ctx->size = s;
  ctx->data = calloc(s, sizeof(void*));
  ctx->destroyers = calloc(s, sizeof(callback_t*));
  return ctx;
}

void context_put(Context *ctx, size_t pos, void *ptr) {
  assert(ctx->size > pos);
  ctx->data[pos] = ptr;
}

void context_putw(Context *ctx, size_t pos, void *ptr, callback_t *callback) {
  ctx->data[pos] = ptr;
  ctx->destroyers[pos] =callback;
}

void context_destroy(Context* ctx) {
  for (size_t i = 0; i < ctx->size; i++) {
    callback_t *cb  = ctx->destroyers[i];
    void* ptr = ctx->data[i];
    if (cb != NULL) {
      ctx->destroyers[i](ptr);
    } else {
      ctx->data[i] = NULL;
    }
  }
  free(ctx->destroyers);
  free(ctx->data);
}
