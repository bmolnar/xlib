#ifndef _XLIB_XSTREAM_FILE_H
#define _XLIB_XSTREAM_FILE_H

#include <xlib/xdefs.h>
#include <xlib/xstream.h>

__XLIB_BEGIN_DECL

xstream_t *xstream_file_open(const char *path, const char *mode);

__XLIB_END_DECL

#endif /* _XLIB_XSTREAM_FILE_H */
