#include <xcommon/IXPlugin.h>
#include "ParticleSystem.h"
#include "Updatable.h"


class CEFFPlugin: public IXUnknownImplementation<IXPlugin>
{
public:
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;
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
			s_guid = IXPARTICLESYSTEM_GUID;
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
		if(guid == IXPARTICLESYSTEM_GUID)
		{
			if(!m_pParticleSystem)
			{
				m_pParticleSystem = new CParticleSystem(m_pCore);
			}
			return(m_pParticleSystem);
		}

		if(guid == IXUPDATABLE_GUID)
		{
			if(!m_pUpdatable)
			{
				if(!m_pParticleSystem)
				{
					getInterface(IXPARTICLESYSTEM_GUID);
				}
				m_pUpdatable = new CUpdatable(m_pParticleSystem);
			}
			return(m_pUpdatable);
		}
		return(NULL);
	}

protected:
	IXCore *m_pCore;
	CParticleSystem *m_pParticleSystem = NULL;
	CUpdatable *m_pUpdatable = NULL;
};

DECLARE_XPLUGIN(CEFFPlugin);
