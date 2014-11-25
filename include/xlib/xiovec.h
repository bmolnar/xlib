#ifndef _XIOVEC_H
#define _XIOVEC_H

#include <stdint.h>

struct xiovec {
    void * iov_base;
    size_t iov_len;
};

#endif /* _XIOVEC_H */
