#ifndef __XEVENTS_H
#define __XEVENTS_H

#include <gdefines.h>

class IBaseEventChannel
{
};

template<typename T>
class IEventChannel: public IBaseEventChannel
{
	typedef void(*PFNLISTENER)(const T*);
	friend class CCore;
	IEventChannel()
	{
	}
public:
	void addListener(PFNLISTENER fnListener)
	{
		for(UINT i = 0, l = m_vListeners.size(); i < l; ++i)
		{
			if(m_vListeners[i] == fnListener)
			{
				return;
			}
		}
		m_vListeners.push_back(fnListener);
	}
	void removeListener(PFNLISTENER fnListener)
	{
		for(UINT i = 0, l = m_vListeners.size(); i < l; ++i)
		{
			if(m_vListeners[i] == fnListener)
			{
				m_vListeners.erase(i);
				return;
			}
		}
	}
	void broadcastEvent(const T *pEvent)
	{
		for(UINT i = 0, l = m_vListeners.size(); i < l; ++i)
		{
			m_vListeners[i](pEvent);
		}
	}
protected:
	Array<PFNLISTENER> m_vListeners;
};

// {DCC97EFB-5254-48E2-B3C3-D9C03392FFDA}
#define EVENT_LEVEL_GUID DEFINE_XGUID(0xdcc97efb, 0x5254, 0x48e2, 0xb3, 0xc3, 0xd9, 0xc0, 0x33, 0x92, 0xff, 0xda)

struct XEventLevel
{
	enum
	{
		TYPE_LOAD,
		TYPE_SAVE,
		TYPE_UNLOAD
	} type;
	const char *szLevelName;
};

#endif
