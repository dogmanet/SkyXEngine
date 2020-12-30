#ifndef __ENTITY_LIST_H
#define __ENTITY_LIST_H

#include <gdefines.h>
#include "EntityPointer.h"
#include <common/string.h>

class CBaseEntity;
class CEntityManager;
class CEntityList: public IEntityPointer
{
	DECLARE_CLASS_NOBASE(CEntityList);
	friend class CBaseEntity;
	friend struct named_output_t;
public:
	~CEntityList();

	//! Установка имени или GUID связанного объекта
	void setEntityName(const char *szName);
	
	//! Установка
	void addEntity(CBaseEntity *pEnt);

	void removeEntity(CBaseEntity *pEnt);

	//! Получение указателя на объект
	CBaseEntity* getEntity(int idx);

	UINT getEntityCount();

	const char* getEntityName();
	
	CBaseEntity* operator[](int idx);

	CEntityList& operator+=(CBaseEntity *pEnt);

	CEntityList& operator-=(CBaseEntity *pEnt);

	//! Вызывается при разрушении связи
	template<typename F>
	void setLinkBrokenListener(void(F::*func)(CBaseEntity*))
	{
		static_assert(std::is_base_of<CBaseEntity, F>::value, "F must be subclass of CBaseEntity");

		m_pfnOnLinkBroken = func;
	}

	//! Вызывается при установке связи
	template<typename F>
	void setLinkEstablishedListener(void(F::*func)(CBaseEntity*))
	{
		static_assert(std::is_base_of<CBaseEntity, F>::value, "F must be subclass of CBaseEntity");

		m_pfnOnLinkEstablished = func;
	}

	void setLinkBrokenListener(IEntityPointerEvent *pFunc)
	{
		m_pOnLinkBroken = pFunc;
	}
	void setLinkEstablishedListener(IEntityPointerEvent *pFunc)
	{
		m_pOnLinkEstablished = pFunc;
	}
	
private:
	String m_sName;
	Array<CBaseEntity*> m_aEntities;
	CBaseEntity *m_pThisEntity = NULL;

	void(CBaseEntity::*m_pfnOnLinkEstablished)(CBaseEntity*) = NULL;
	void(CBaseEntity::*m_pfnOnLinkBroken)(CBaseEntity*) = NULL;
	IEntityPointerEvent *m_pOnLinkEstablished = NULL;
	IEntityPointerEvent *m_pOnLinkBroken = NULL;

	void onLinkBroken(CBaseEntity *pOldEnt);
	void onLinkEstablished(CBaseEntity *pNewEnt);
	void onTargetRemoved(CBaseEntity *pEnt) override;

	bool m_isWaiting = false;
	void registerWait();
	void unregisterWait();
	void onWaitDone(CBaseEntity *pEnt) override
	{
	}

	void clearList();

	void init(CBaseEntity *pThisEntity)
	{
		m_pThisEntity = pThisEntity;
	}
};

#endif
