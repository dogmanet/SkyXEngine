#ifndef __IXGUI_H
#define __IXGUI_H

#include "IXDesktopStack.h"

// {F5890807-9525-40DD-97E3-12FAF7DBAFBC}
#define IXGUI_GUID DEFINE_XGUID(0xf5890807, 0x9525, 0x40dd, 0x97, 0xe3, 0x12, 0xfa, 0xf7, 0xdb, 0xaf, 0xbc)

class IXGUI: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE newDesktopStack(const char *szResPath, UINT uWidth, UINT uHeight, IXDesktopStack **pOut) = 0;
};

#endif
