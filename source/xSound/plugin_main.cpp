
#include <xcommon/IXPlugin.h>
#include <xcommon/IXUpdatable.h>
#include "SoundSystem.h"
#include <Audio.h>

#if defined(_DEBUG)
#pragma comment(lib, "mital_d.lib")
#else
#pragma comment(lib, "mital.lib")
#endif


class CUpdatable : public IXUnknownImplementation<IXUpdatable>
{
public:
	CUpdatable(CSoundSystem *pSoundSystem)
	{
		m_pSoundSystem = pSoundSystem;
	}

	UINT startup() override
	{
		return 1;
	}
	void shutdown() override
	{
		mem_release(m_pSoundSystem);
	}

	ID run(float fDelta) override
	{
		float3 v;
		if (m_pSoundSystem)
			m_pSoundSystem->update(v, v, v);

		return -1;
	}

	void sync() override
	{

	}

protected:
	CSoundSystem *m_pSoundSystem = NULL;
};


class CSoundSystemPlugin : public IXUnknownImplementation<IXPlugin>
{
public:

	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;
		m_pSoundSystem = new CSoundSystem(m_pCore);
		m_pUpdatable = new CUpdatable(m_pSoundSystem);
	}

	void XMETHODCALLTYPE shutdown() override
	{
	}

	UINT XMETHODCALLTYPE getInterfaceCount() override
	{
		return(2);
	}
	const XGUID* XMETHODCALLTYPE getInterfaceGUID(UINT id) override
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = IXSOUNDSYSTEM_GUID;
			break;
		case 1:
			s_guid = IXUPDATABLE_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown* XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if (guid == IXSOUNDSYSTEM_GUID)
			return m_pSoundSystem;
		else if (guid == IXUPDATABLE_GUID)
			return m_pUpdatable;

		return(NULL);
	}

protected:
	IXCore *m_pCore;
	CSoundSystem *m_pSoundSystem = NULL;
	CUpdatable *m_pUpdatable = NULL;
};

DECLARE_XPLUGIN(CSoundSystemPlugin);
