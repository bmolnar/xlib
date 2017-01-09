lib_LTLIBRARIES = libx.la

nobase_include_HEADERS = \
	xlib/xerr.h \
	xlib/xmembuf.h \
	xlib/xiovec.h \
	xlib/xfmt.h \
	xlib/xalloc.h \
	xlib/xstream.h \
	xlib/xstream_xmembuf.h \
	xlib/xprint.h \
	xlib/xprint_wrappers.h \
	xlib/xlib.h

libx_la_LDFLAGS = -version-info 1:0:0 -export-dynamic -no-undefined
libx_la_LIBADD =
libx_la_SOURCES = \
	xlib/xiovec.h \
	xlib/xerr.c \
	xlib/xerr.h \
	xlib/xmembuf.c \
	xlib/xmembuf.h \
	xlib/xfmt.c \
	xlib/xfmt.h \
	xlib/xalloc.c \
	xlib/xalloc.h \
	xlib/xstream.c \
	xlib/xstream.h \
	xlib/xstream_xmembuf.c \
	xlib/xstream_xmembuf.h \
	xlib/xprint.c \
	xlib/xprint.h \
	xlib/xprint_wrappers.c \
	xlib/xprint_wrappers.h \
	xlib/xlib.h

if ENABLE_XSTREAM_FILE
libx_la_SOURCES += xlib/xstream_file.c xlib/xstream_file.h
nobase_include_HEADERS += include/xlib/xstream_file.h
endif

if ENABLE_XSTREAM_STDIO
libx_la_SOURCES += xlib/xstream_stdio.c xlib/xstream_stdio.h
nobase_include_HEADERS += include/xlib/xstream_stdio.h
endif

