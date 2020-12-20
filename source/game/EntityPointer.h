#ifndef __ENTITY_POINTER_H
#define __ENTITY_POINTER_H

#include <gdefines.h>

class CBaseEntity;
class CEntityManager;
class CEntityPointer
{
	DECLARE_CLASS_NOBASE(CEntityPointer);
	friend class CBaseEntity;
	friend class CEntityManager;
public:
	
	~CEntityPointer();

	//! Установка имени или GUID связанного объекта
	void setEntityName(const char *szName);

	//! Установка GUID
	void setEntityGUID(const XGUID &guid);

	//! Установка
	void setEntity(CBaseEntity *pEnt);

	//! Получение указателя на объект
	CBaseEntity* getEntity();

	void getEntityName(char *szOutput, int iBufSize);
	const XGUID& getGUID();

	CBaseEntity* operator->();
	operator CBaseEntity*();

	CEntityPointer& operator=(CBaseEntity *pEnt);

	//! Вызывается при разрушении связи (pEnt -- указатель на объект, если причиной разрыва связи стало не удаление объекта)
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
	
private:
	XGUID m_guid;
	CBaseEntity *m_pEntity = NULL;
	CEntityManager *m_pMgr = NULL;
	CBaseEntity *m_pThisEntity = NULL;

	void(CBaseEntity::*m_pfnOnLinkBroken)(CBaseEntity*) = NULL;
	void(CBaseEntity::*m_pfnOnLinkEstablished)(CBaseEntity*) = NULL;

	void init(CEntityManager *pWorld, CBaseEntity *pThisEntity);

	void onLinkBroken(CBaseEntity *pOldEnt);
	void onLinkEstablished(CBaseEntity *pNewEnt);
	void onTargetRemoved();

	bool m_isWaiting = false;
	void registerWait();
	void unregisterWait();
	void onWaitDone(CBaseEntity *pEnt);
};

#endif
