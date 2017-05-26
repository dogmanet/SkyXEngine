#ifndef _DSentityFactory_H_
#define _DSentityFactory_H_

#include <gdefines.h>
#include <common/AssotiativeArray.h>
#include <common/String.h>

#include "proptable.h"

#ifdef GetClassName
#undef GetClassName
#endif

class SXbaseEntity;
class EntityManager;
class IEntityFactory
{
public:
	virtual SXbaseEntity * Create(EntityManager * pWorld) = 0;
	virtual void Destroy(SXbaseEntity * pEnt) = 0;
	virtual const char * GetClassName() = 0;
	virtual proptable_t * GetPropTable() = 0;
	virtual bool IsEditorHidden() = 0;
};

class EntityFactoryMap
{
public:
	EntityFactoryMap();

	void AddFactory(IEntityFactory * pFactory, const char * szName);
	SXbaseEntity * Create(const char * szName, EntityManager * pMgr);
	void Destroy(SXbaseEntity * pEnt);

	static EntityFactoryMap * GetInstance();
	proptable_t * GetPropTable(const char * szClass);
	bool IsEditorHidden(const char * szClass);

private:
	IEntityFactory * GetFactory(const char * szName);
	AssotiativeArray<String, IEntityFactory*> m_mFactories;
};

template <class T>
class EntityFactory: public IEntityFactory
{
public:
	EntityFactory(const char * szName, bool showInListing)
	{
		EntityFactoryMap::GetInstance()->AddFactory(this, szName);
		m_szClassName = szName;
		m_pPropTable = T::GetPropTable();
		m_bShowInListing = showInListing;
	}

	~EntityFactory()
	{
		T::ReleasePropData();
	}

	SXbaseEntity * Create(EntityManager * pWorld)
	{
		return(new T(pWorld));
	}

	void Destroy(SXbaseEntity * pEnt)
	{
		if(pEnt)
		{
			delete pEnt;
		}
	}

	const char * GetClassName()
	{
		return(m_szClassName);
	}

	proptable_t * GetPropTable()
	{
		return(m_pPropTable);
	}

	bool IsEditorHidden()
	{
		return(!m_bShowInListing);
	}


private:
	const char * m_szClassName;
	proptable_t * m_pPropTable;
	bool m_bShowInListing;
};

#define REGISTER_ENTITY(cls, name) \
	EntityFactory<cls> ent_ ## name ## _factory(#name, 1)

#define REGISTER_ENTITY_NOLISTING(cls, name) \
	EntityFactory<cls> ent_ ## name ## _factory(#name, 0)

#define CREATE_ENTITY(cls, mgr) EntityFactoryMap::GetInstance()->Create(cls, mgr)
#define REMOVE_ENTITY(ent) EntityFactoryMap::GetInstance()->Destroy(ent)

#endif
