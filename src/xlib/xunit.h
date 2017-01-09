#ifndef _XUNIT_H
#define _XUNIT_H

#include <xlib/xdefs.h>

__XLIB_BEGIN_DECL



struct _xunit_test
{
  void (*testfn)(void);
  const char *group;
  const char *name;
};

#define _XUNIT_TEST_FNAME(group,name) __xunit_test_ ## group ## _ ## name ## _func
#define _XUNIT_TEST_VNAME(group,name) __xunit_test_ ## group ## _ ## name ## _info

#define XUNIT_TEST(group,name) \
void _XUNIT_TEST_FNAME(group,name) (); \
struct _xunit_test _XUNIT_TEST_VNAME(group,name) = { _XUNIT_TEST_FNAME(group,name), #group, #name }; \
void _XUNIT_TEST_FNAME(group,name) ()


__XLIB_END_DECL

#endif /* _XUNIT_H */
