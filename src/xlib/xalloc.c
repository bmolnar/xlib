#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include <xlib/xerr.h>
#include <xlib/xalloc.h>


static void *
xalloc_do_alloc(xalloc_t *xa, size_t size)
{
    xalloc_ops_t *ops = XALLOC_OPS(xa);
    if (ops->op_alloc == NULL)
    {
        return NULL;
    }
    return (*ops->op_alloc)(xa, size);
}
static void
xalloc_do_free(xalloc_t *xa, void *ptr)
{
    xalloc_ops_t *ops = XALLOC_OPS(xa);
    if (ops->op_free == NULL)
    {
        return;
    }
    (*ops->op_free)(xa, ptr);
}


void *
xalloc_alloc(xalloc_t *xa, size_t size)
{
    return xalloc_do_alloc(xa, size);
}
void
xalloc_free(xalloc_t *xa, void *ptr)
{
    xalloc_do_free(xa, ptr);
}
