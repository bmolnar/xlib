#ifndef _XERR_H
#define _XERR_H

/* error codes */
#define XERR_NOSYS  1
#define XERR_NOENT  2
#define XERR_NOMEM  3
#define XERR_INVAL  4
#define XERR_NOBUFS 5

const char *xerr_str(int xerr);

#endif /* _XERR_H */
