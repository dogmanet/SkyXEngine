#include "EntityList.h"
#include "BaseEntity.h"

CEntityList::~CEntityList()
{
	clearList();

	unregisterWait();
}

//! Установка имени или GUID связанного объекта
void CEntityList::setEntityName(const char *szName)
{
	clearList();

	unregisterWait();
	m_sName = szName;

	if(szName[0])
	{
		registerWait();

		CBaseEntity *pEnt = NULL;
		while((pEnt = m_pThisEntity->getEntByName(szName, pEnt)))
		{
			addEntity(pEnt);
		}
	}
}

//! Установка
void CEntityList::addEntity(CBaseEntity *pEnt)
{
	assert(pEnt);

	pEnt->registerPointer(this);

	int idx = m_aEntities.indexOf(pEnt);
	assert(idx < 0);
	if(idx < 0)
	{
		m_aEntities.push_back(pEnt);
		onLinkEstablished(pEnt);
	}
}

void CEntityList::removeEntity(CBaseEntity *pEnt)
{
	assert(pEnt);
	int idx = m_aEntities.indexOf(pEnt);
	assert(idx >= 0);
	if(idx >= 0)
	{
		onLinkBroken(pEnt);
		m_aEntities[idx] = m_aEntities[m_aEntities.size() - 1];
		m_aEntities.erase(m_aEntities.size() - 1);
	}
}

//! Получение указателя на объект
CBaseEntity* CEntityList::getEntity(int idx)
{
	return((int)m_aEntities.size() >= idx ? m_aEntities[idx] : NULL);
}

UINT CEntityList::getEntityCount()
{
	return(m_aEntities.size());
}

const char* CEntityList::getEntityName()
{
	return(m_sName.c_str());
}

CBaseEntity* CEntityList::operator[](int idx)
{
	return(getEntity(idx));
}

CEntityList& CEntityList::operator+=(CBaseEntity *pEnt)
{
	addEntity(pEnt);
	return(*this);
}

CEntityList& CEntityList::operator-=(CBaseEntity *pEnt)
{
	removeEntity(pEnt);
	return(*this);
}

void CEntityList::onLinkBroken(CBaseEntity *pOldEnt)
{
	if(m_pfnOnLinkBroken)
	{
		(m_pThisEntity->*m_pfnOnLinkBroken)(pOldEnt);
	}
	if(m_pOnLinkBroken)
	{
		m_pOnLinkBroken->onEvent(pOldEnt);
	}
}
void CEntityList::onLinkEstablished(CBaseEntity *pNewEnt)
{
	if(m_pfnOnLinkEstablished)
	{
		(m_pThisEntity->*m_pfnOnLinkEstablished)(pNewEnt);
	}
	if(m_pOnLinkEstablished)
	{
		m_pOnLinkEstablished->onEvent(pNewEnt);
	}
}
void CEntityList::onTargetRemoved(CBaseEntity *pEnt)
{
	removeEntity(pEnt);
}

void CEntityList::registerWait()
{
	if(!m_isWaiting)
	{
		m_pThisEntity->m_pMgr->registerWaitForName(m_sName.c_str(), this);
		m_isWaiting = true;
	}
}
void CEntityList::unregisterWait()
{
	if(m_isWaiting)
	{
		m_pThisEntity->m_pMgr->unregisterWaitForName(m_sName.c_str(), this);
		m_isWaiting = false;
	}
}

void CEntityList::clearList()
{
	for(UINT i = 0, l = m_aEntities.size(); i < l; ++i)
	{
		m_aEntities[i]->unregisterPointer(this);
	}
	m_aEntities.clearFast();
}


