#include <assert.h>
#include <stdlib.h>

#include "context.h"

struct Context {
    size_t          size;
    void          **data;
    callback_t    **destroyers;
};

Context        *
context_new(size_t size)
{
    assert(size > 0);
    Context        *ctx = calloc(1, sizeof(Context));
    if (ctx == NULL) {
	return NULL;
    }
    ctx->size = size;
    ctx->data = calloc(size, sizeof(void *));
    ctx->destroyers = calloc(size, sizeof(callback_t *));
    if (ctx->data == NULL || ctx->destroyers == NULL) {
	free(ctx->data);
	free(ctx->destroyers);
	free(ctx);
	return NULL;
    }
    return ctx;
}

void           *
context_get(Context * ctx, size_t pos)
{
    assert(ctx);
    assert(ctx->size > pos);
    return ctx->data[pos];
}

void
context_put(Context * ctx, size_t pos, void *ptr)
{
    assert(ctx);
    assert(ctx->size > pos);
    ctx->data[pos] = ptr;
    ctx->destroyers[pos] = NULL;
}

void
context_putw(Context * ctx, size_t pos, void *ptr, callback_t * callback)
{
    assert(ctx);
    assert(ctx->size > pos);
    ctx->data[pos] = ptr;
    ctx->destroyers[pos] = callback;
}

void
context_destroy(Context * ctx)
{
    if (ctx == NULL) {
	return;
    }
    for (size_t i = 0; i < ctx->size; i++) {
	if (ctx->destroyers[i] != NULL && ctx->data[i] != NULL) {
	    ctx->destroyers[i] (ctx->data[i]);
	}
	ctx->data[i] = NULL;
	ctx->destroyers[i] = NULL;
    }
    free(ctx->destroyers);
    free(ctx->data);
    free(ctx);
}
