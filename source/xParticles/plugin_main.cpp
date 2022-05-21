#include <xcommon/IXPlugin.h>
#include "ParticleSystem.h"
#include "Updatable.h"
#include "Renderable.h"


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
		return(3);
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
		case 2:
			s_guid = IXRENDERABLE_GUID;
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

		if(guid == IXRENDERABLE_GUID)
		{
			if(!m_pRenderable)
			{
				if(!m_pParticleSystem)
				{
					getInterface(IXPARTICLESYSTEM_GUID);
				}
				m_pRenderable = new CRenderable(getID(), m_pParticleSystem);
			}
			return(m_pRenderable);
		}
		return(NULL);
	}

protected:
	IXCore *m_pCore;
	CParticleSystem *m_pParticleSystem = NULL;
	CUpdatable *m_pUpdatable = NULL;
	CRenderable *m_pRenderable = NULL;
};

DECLARE_XPLUGIN(CEFFPlugin);
