#include <stdlib.h>
enum { POW = 0, X11 = 1, REP = 2, LOADAVG = 3 };

#define CALLBACK (void(*)(void*))
typedef void    (callback_t) (void *);

typedef struct Context Context;

Context        *context_new();

void           *context_get(Context * ctx, size_t pos);
void            context_put(Context * ctx, size_t pos, void *ptr);
void            context_destroy(Context * ctx);

void            context_putw(Context * ctx, size_t pos, void *ptr,
			     callback_t * callback);
