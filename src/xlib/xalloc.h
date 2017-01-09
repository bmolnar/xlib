#ifndef _XALLOC_H
#define _XALLOC_H

#include <stdarg.h>

__XLIB_BEGIN_DECL

struct xalloc;
typedef struct xalloc xalloc_t;

typedef void *  (xalloc_alloc_fn_t)(xalloc_t *xa, size_t size);
typedef void    (xalloc_free_fn_t)(xalloc_t *xa, void *ptr);
struct xalloc_ops {
    xalloc_alloc_fn_t * op_alloc;
    xalloc_free_fn_t * op_free;
};
typedef struct xalloc_ops xalloc_ops_t;

struct xalloc {
    xalloc_ops_t xa_ops;
    void *       xa_priv;
};
#define XALLOC_INIT(ops,priv) ((xalloc_t) { .xa_ops = (ops), .xa_priv = (priv) })
#define XALLOC_OPS(xa)        (&(xa)->xa_ops)
#define XALLOC_PRIV(xa)       ((xa)->xa_priv)


void * xalloc_alloc(xalloc_t *xa, size_t size);
void xalloc_free(xalloc_t *xa, void *ptr);

__XLIB_END_DECL

#endif /* _XALLOC_H */
