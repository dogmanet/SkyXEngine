#ifndef __ENTITY_POINTER_H
#define __ENTITY_POINTER_H

#include <gdefines.h>

class IEntityPointer
{
	friend class CBaseEntity;
	friend class CEntityManager;

private:
	virtual void onTargetRemoved() = 0;
	virtual void onWaitDone(CBaseEntity *pEnt) = 0;
};

class CBaseEntity;
class CEntityManager;
template<typename T>
class CEntityPointer: public IEntityPointer
{
	DECLARE_CLASS_NOBASE(CEntityPointer);
	friend class CBaseEntity;
public:
	
	CEntityPointer():
		m_pfnCheckEntityType(&CEntityFactoryMap::IsEntityOfClass<T>)
	{
	}

	~CEntityPointer()
	{
		SAFE_CALL(m_pEntity, unregisterPointer, this);

		unregisterWait();
	}

	//! Установка имени или GUID связанного объекта
	void setEntityName(const char *szName)
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

		CBaseEntity *pEnt = m_pMgr->findEntityByName(szName);

		if(pEnt)
		{
			if(m_pfnCheckEntityType(pEnt))
			{
				setEntity(pEnt);
			}
			else
			{
				char tmp1[64], tmp2[64];
				XGUIDToSting(m_guid, tmp1, sizeof(tmp1));
				XGUIDToSting(*m_pThisEntity->getGUID(), tmp2, sizeof(tmp2));
				LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to link entity '%s' to entity '%s'. Invalid class %s\n", tmp1, tmp2, pEnt->getClassName());
			}
		}
	}

	//! Установка GUID
	void setEntityGUID(const XGUID &guid)
	{
		CBaseEntity *pEnt = m_pMgr->getByGUID(guid);
		
		if(pEnt)
		{
			if(m_pfnCheckEntityType(pEnt))
			{
				setEntity(pEnt);
			}
			else
			{
				char tmp1[64], tmp2[64];
				XGUIDToSting(m_guid, tmp1, sizeof(tmp1));
				XGUIDToSting(*m_pThisEntity->getGUID(), tmp2, sizeof(tmp2));
				LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to link entity '%s' to entity '%s'. Invalid class %s\n", tmp1, tmp2, pEnt->getClassName());
			}
		}
		else
		{
			m_guid = guid;
			registerWait();
		}
	}

	//! Установка
	void setEntity(T *pEnt)
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

	//! Получение указателя на объект
	T* getEntity()
	{
		return(m_pEntity);
	}

	void getEntityName(char *szOutput, int iBufSize)
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
	const XGUID& getGUID()
	{
		return(m_guid);
	}

	T* operator->()
	{
		return(getEntity());
	}
	operator T*()
	{
		return(getEntity());
	}

	CEntityPointer& operator=(T *pEnt)
	{
		setEntity(pEnt);
		return(*this);
	}

	//! Вызывается при разрушении связи (pEnt -- указатель на объект, если причиной разрыва связи стало не удаление объекта)
	template<typename F>
	void setLinkBrokenListener(void(F::*func)(T*))
	{
		static_assert(std::is_base_of<CBaseEntity, F>::value, "F must be subclass of CBaseEntity");

		m_pfnOnLinkBroken = (void(CBaseEntity::*)(T*))func;
	}

	//! Вызывается при установке связи
	template<typename F>
	void setLinkEstablishedListener(void(F::*func)(T*))
	{
		static_assert(std::is_base_of<CBaseEntity, F>::value, "F must be subclass of CBaseEntity");

		m_pfnOnLinkEstablished = (void(CBaseEntity::*)(T*))func;
	}
	
private:
	XGUID m_guid;
	T *m_pEntity = NULL;
	CEntityManager *m_pMgr = NULL;
	CBaseEntity *m_pThisEntity = NULL;
	bool(*m_pfnCheckEntityType)(CBaseEntity*) = NULL;

	void(CBaseEntity::*m_pfnOnLinkBroken)(T*) = NULL;
	void(CBaseEntity::*m_pfnOnLinkEstablished)(T*) = NULL;

	void init(CEntityManager *pWorld, CBaseEntity *pThisEntity)
	{
		m_pMgr = pWorld;
		m_pThisEntity = pThisEntity;
	}

	void onLinkBroken(T *pOldEnt)
	{
		if(m_pfnOnLinkBroken)
		{
			(m_pThisEntity->*m_pfnOnLinkBroken)(pOldEnt);
		}
	}
	void onLinkEstablished(T *pNewEnt)
	{
		pNewEnt->registerPointer(this);

		if(m_pfnOnLinkEstablished)
		{
			(m_pThisEntity->*m_pfnOnLinkEstablished)(pNewEnt);
		}
	}
	void onTargetRemoved() override
	{
		onLinkBroken(NULL);
		m_pEntity = NULL;
		registerWait();
	}

	bool m_isWaiting = false;
	void registerWait()
	{
		if(!m_isWaiting)
		{
			m_pMgr->registerWaitForGUID(m_guid, this);
			m_isWaiting = true;
		}
	}
	void unregisterWait()
	{
		if(m_isWaiting)
		{
			m_pMgr->unregisterWaitForGUID(m_guid, this);
			m_isWaiting = false;
		}
	}
	void onWaitDone(CBaseEntity *pEnt) override
	{
		assert(m_isWaiting);

		m_isWaiting = false;

		if(m_pfnCheckEntityType(pEnt))
		{
			m_pEntity = (T*)pEnt;

			onLinkEstablished((T*)pEnt);
		}
		else
		{
			char tmp1[64], tmp2[64];
			XGUIDToSting(m_guid, tmp1, sizeof(tmp1));
			XGUIDToSting(*m_pThisEntity->getGUID(), tmp2, sizeof(tmp2));
			LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to link entity '%s' to entity '%s'. Invalid class %s\n", tmp1, tmp2, pEnt->getClassName());
		}
	}
};

#endif
