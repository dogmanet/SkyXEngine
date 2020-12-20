#include "EntityPointer.h"
#include "BaseEntity.h"

CEntityPointer::~CEntityPointer()
{
	if(m_pEntity)
	{
		m_pEntity->unregisterPointer(this);
	}

	unregisterWait();
}

void CEntityPointer::setEntityName(const char *szName)
{
	if(szName[0] == '{')
	{
		XGUID guid;
		if(XGUIDFromString(&guid, szName))
		{
			setEntityGUID(guid);
			return;
		}
	}

	setEntity(m_pMgr->findEntityByName(szName));
}

void CEntityPointer::setEntityGUID(const XGUID &guid)
{
	CBaseEntity *pEnt = m_pMgr->getByGUID(guid);
	setEntity(pEnt);
	if(!pEnt)
	{
		m_guid = guid;
		registerWait();
	}
}

void CEntityPointer::setEntity(CBaseEntity *pEnt)
{
	if(m_pEntity != pEnt)
	{
		unregisterWait();

		if(m_pEntity)
		{
			onLinkBroken(m_pEntity);
		}
		m_pEntity = pEnt;
		if(pEnt)
		{
			m_guid = *pEnt->getGUID();
		}
		else
		{
			m_guid = XGUID();
		}
		if(m_pEntity)
		{
			onLinkEstablished(m_pEntity);
		}
	}
}

CBaseEntity* CEntityPointer::getEntity()
{
	return(m_pEntity);
}

CBaseEntity* CEntityPointer::operator->()
{
	return(getEntity());
}
CEntityPointer::operator CBaseEntity*()
{
	return(getEntity());
}

CEntityPointer& CEntityPointer::operator=(CBaseEntity *pEnt)
{
	setEntity(pEnt);
	return(*this);
}

void CEntityPointer::getEntityName(char *szOutput, int iBufSize)
{
	if(m_pEntity && m_pEntity->getName()[0])
	{
		if(m_pMgr->countEntityByName(m_pEntity->getName()) == 1)
		{
			strncpy(szOutput, m_pEntity->getName(), iBufSize);
			szOutput[iBufSize - 1] = 0;
			return;
		}
	}
	
	if(m_guid == XGUID())
	{
		if(iBufSize)
		{
			szOutput[0] = 0;
		}
		return;
	}

	char tmp[64];
	XGUIDToSting(m_guid, tmp, sizeof(tmp));
	strncpy(szOutput, tmp, iBufSize);
	szOutput[iBufSize - 1] = 0;
}
const XGUID& CEntityPointer::getGUID()
{
	return(m_guid);
}

void CEntityPointer::init(CEntityManager *pWorld, CBaseEntity *pThisEntity)
{
	m_pMgr = pWorld;
	m_pThisEntity = pThisEntity;
}

void CEntityPointer::onLinkBroken(CBaseEntity *pOldEnt)
{
	if(m_pfnOnLinkBroken)
	{
		(m_pThisEntity->*m_pfnOnLinkBroken)(pOldEnt);
	}
}

void CEntityPointer::onLinkEstablished(CBaseEntity *pNewEnt)
{
	pNewEnt->registerPointer(this);

	if(m_pfnOnLinkEstablished)
	{
		(m_pThisEntity->*m_pfnOnLinkEstablished)(pNewEnt);
	}
}

void CEntityPointer::onTargetRemoved()
{
	onLinkBroken(NULL);
	m_pEntity = NULL;
	registerWait();
}

void CEntityPointer::registerWait()
{
	if(!m_isWaiting)
	{
		m_pMgr->registerWaitForGUID(m_guid, this);
		m_isWaiting = true;
	}
}

void CEntityPointer::unregisterWait()
{
	if(m_isWaiting)
	{
		m_pMgr->unregisterWaitForGUID(m_guid, this);
		m_isWaiting = false;
	}
}

void CEntityPointer::onWaitDone(CBaseEntity *pEnt)
{
	assert(m_isWaiting);

	m_isWaiting = false;

	m_pEntity = pEnt;

	onLinkEstablished(pEnt);
}
