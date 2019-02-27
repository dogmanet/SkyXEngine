#ifndef __ITEST_H
#define __ITEST_H

#include <xcommon/IXPlugin.h>

// {621E307F-A25E-4156-AE58-3FD24711C985}
#define ITEST_GUID DEFINE_XGUID(0x621e307f, 0xa25e, 0x4156, 0xae, 0x58, 0x3f, 0xd2, 0x47, 0x11, 0xc9, 0x85)


class ITest: public IXUnknown
{
public:
	virtual void Foo() = 0;
};

#endif
