#ifndef _DSentityFactory_H_
#define _DSentityFactory_H_

#include <gdefines.h>
#include <common/AssotiativeArray.h>
#include <common/String.h>
#include <common/AAString.h>
#include <common/Array.h>

#include "proptable.h"

#ifdef GetClassName
#undef GetClassName
#endif

typedef AssotiativeArray<AAString, const char *> EntDefaultsMap;

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
	virtual EntDefaultsMap * GetDefaults() = 0;
	virtual IEntityFactory * Copy(const char * szName, bool showInListing) = 0;
};

class EntityFactoryMap
{
	friend class EntityManager;
public:
	EntityFactoryMap();

	void AddFactory(IEntityFactory * pFactory, const char * szName);
	SXbaseEntity * Create(const char * szName, EntityManager * pMgr, bool bDelayPostLoad=false);
	void Destroy(SXbaseEntity * pEnt);

	static EntityFactoryMap * GetInstance();
	proptable_t * GetPropTable(const char * szClass);
	bool IsEditorHidden(const char * szClass);
	EntDefaultsMap * GetDefaults(const char * szClass);

	int GetListCount();
	void GetListing(const char ** pszOut, int size);
private:
	IEntityFactory * GetFactory(const char * szName);
	AssotiativeArray<AAString, IEntityFactory*> m_mFactories;
	int m_iShowInListCount;
};

template <class T>
class EntityFactory: public IEntityFactory
{
public:
	EntityFactory(const char * szName, bool showInListing)
	{
		m_bShowInListing = showInListing;
		m_szClassName = szName;
		m_pPropTable = T::SGetPropTable();
		EntityFactoryMap::GetInstance()->AddFactory(this, szName);
	}

	~EntityFactory()
	{
		T::ReleasePropData();
		for(int i = 0, l = m_vDerivatives.size(); i < l; ++i)
		{
			delete m_vDerivatives[i];
		}
	}

	IEntityFactory * Copy(const char * szName, bool showInListing)
	{
		EntityFactory<T> * newF = new EntityFactory<T>(*this);
		newF->m_szClassName = szName;
		newF->m_bShowInListing = showInListing;
		m_vDerivatives.push_back(newF);
		return(newF);
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

	EntDefaultsMap * GetDefaults()
	{
		return(&m_mDefaults);
	}

private:
	const char * m_szClassName;
	proptable_t * m_pPropTable;
	bool m_bShowInListing;
	EntDefaultsMap m_mDefaults;
	Array<EntityFactory<T>*> m_vDerivatives;
};

#define REGISTER_ENTITY(cls, name) \
	EntityFactory<cls> ent_ ## name ## _factory(#name, 1)

#define REGISTER_ENTITY_NOLISTING(cls, name) \
	EntityFactory<cls> ent_ ## name ## _factory(#name, 0)

#define CREATE_ENTITY(cls, mgr) EntityFactoryMap::GetInstance()->Create(cls, mgr)
#define CREATE_ENTITY_NOPOST(cls, mgr) EntityFactoryMap::GetInstance()->Create(cls, mgr, 1)
#define REMOVE_ENTITY(ent) EntityFactoryMap::GetInstance()->Destroy(ent)

#endif
