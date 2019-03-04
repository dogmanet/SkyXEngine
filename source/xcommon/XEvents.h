#ifndef __XEVENTS_H
#define __XEVENTS_H

#include <gdefines.h>

 // {DCC97EFB-5254-48E2-B3C3-D9C03392FFDA}
#define EVENT_LEVEL_LOAD_GUID DEFINE_XGUID(0xdcc97efb, 0x5254, 0x48e2, 0xb3, 0xc3, 0xd9, 0xc0, 0x33, 0x92, 0xff, 0xda)

struct XEventLevelLoad
{
	const char *szLevelName;
};

void XEventTrigger(const XGUID &guid, void *pData);

template<XGUID T, typename = void> class IEventBinder;
template<XGUID T, typename = std::enable_if<(T == EVENT_LEVEL_LOAD_GUID)>::type> 
class IEventBinder<EVENT_LEVEL_LOAD_GUID>
{
public:
	static void XEventAddListener(EVENT_LEVEL_LOAD_GUID, void(const XEventLevelLoad*)());
};

// XEventAddListener<EVENT_LEVEL_LOAD_GUID>(fn1);

#endif
