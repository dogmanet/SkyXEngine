#ifndef __XEVENTS_H
#define __XEVENTS_H

#include <gdefines.h>
#include <common/math.h>

class IBaseEventChannel
{
public:
	virtual ~IBaseEventChannel() = default;
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
		TYPE_UNLOAD,
	} type;
	const char *szLevelName;
};



// {ED67CE1B-C328-45A3-B0E1-288321236F2C}
#define EVENT_LEVEL_PROGRESS_GUID DEFINE_XGUID(0xed67ce1b, 0xc328, 0x45a3, 0xb0, 0xe1, 0x28, 0x83, 0x21, 0x23, 0x6f, 0x2c)

struct XEventLevelProgress
{
	enum
	{
		TYPE_PROGRESS_BEGIN,
		TYPE_PROGRESS_VALUE,
		TYPE_PROGRESS_END,
	} type;
	ID idPlugin;
	float fProgress;
};


// {E20F61DF-BBFC-4920-A669-3143CB7D3717}
#define EVENT_LEVEL_GET_SIZE_GUID DEFINE_XGUID(0xe20f61df, 0xbbfc, 0x4920, 0xa6, 0x69, 0x31, 0x43, 0xcb, 0x7d, 0x37, 0x17)

struct XEventLevelSize
{
	mutable float3 vMin;
	mutable float3 vMax;
};

#endif
