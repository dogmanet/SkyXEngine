#ifndef _CEntityFactory_H_
#define _CEntityFactory_H_

#include <gdefines.h>
#include <common/AssotiativeArray.h>
#include <common/String.h>
#include <common/AAString.h>
#include <common/Array.h>

#include "proptable.h"

typedef AssotiativeArray<AAString, const char *> EntDefaultsMap;

class CBaseEntity;
class CEntityManager;
class IEntityFactory
{
public:
	virtual CBaseEntity * create(CEntityManager * pWorld) = 0;
	virtual void destroy(CBaseEntity * pEnt) = 0;
	virtual const char * getClassName() = 0;
	virtual proptable_t * getPropTable() = 0;
	virtual bool isEditorHidden() = 0;
	virtual EntDefaultsMap * getDefaults() = 0;
	virtual IEntityFactory * copy(const char * szName, bool showInListing) = 0;
};

class CEntityFactoryMap
{
	friend class CEntityManager;
public:
	CEntityFactoryMap();

	void addFactory(IEntityFactory * pFactory, const char * szName);
	CBaseEntity * create(const char * szName, CEntityManager * pMgr, bool bDelayPostLoad=false);
	void destroy(CBaseEntity * pEnt);

	static CEntityFactoryMap * GetInstance();
	proptable_t * getPropTable(const char * szClass);
	bool isEditorHidden(const char * szClass);
	EntDefaultsMap * getDefaults(const char * szClass);

	int getListCount();
	void getListing(const char ** pszOut, int size);
private:
	IEntityFactory * getFactory(const char * szName);
	AssotiativeArray<AAString, IEntityFactory*> m_mFactories;
	int m_iShowInListCount;
};

template <class T>
class CEntityFactory: public IEntityFactory
{
public:
	CEntityFactory(const char * szName, bool showInListing)
	{
		m_bShowInListing = showInListing;
		m_szClassName = szName;
		m_pPropTable = T::SGetPropTable();
		CEntityFactoryMap::GetInstance()->addFactory(this, szName);
	}

	~CEntityFactory()
	{
		T::ReleasePropData();
		for(int i = 0, l = m_vDerivatives.size(); i < l; ++i)
		{
			delete m_vDerivatives[i];
		}
	}

	IEntityFactory * copy(const char * szName, bool showInListing)
	{
		CEntityFactory<T> * newF = new CEntityFactory<T>(*this);
		newF->m_szClassName = szName;
		newF->m_bShowInListing = showInListing;
		m_vDerivatives.push_back(newF);
		return(newF);
	}

	CBaseEntity * create(CEntityManager * pWorld)
	{
		return(new T(pWorld));
	}

	void destroy(CBaseEntity * pEnt)
	{
		if(pEnt)
		{
			delete pEnt;
		}
	}

	const char * getClassName()
	{
		return(m_szClassName);
	}

	proptable_t * getPropTable()
	{
		return(m_pPropTable);
	}

	bool isEditorHidden()
	{
		return(!m_bShowInListing);
	}

	EntDefaultsMap * getDefaults()
	{
		return(&m_mDefaults);
	}

private:
	const char * m_szClassName;
	proptable_t * m_pPropTable;
	bool m_bShowInListing;
	EntDefaultsMap m_mDefaults;
	Array<CEntityFactory<T>*> m_vDerivatives;
};

#define REGISTER_ENTITY(cls, name) \
	CEntityFactory<cls> ent_ ## name ## _factory(#name, 1)

#define REGISTER_ENTITY_NOLISTING(cls, name) \
	CEntityFactory<cls> ent_ ## name ## _factory(#name, 0)

#define CREATE_ENTITY(cls, mgr) CEntityFactoryMap::GetInstance()->create(cls, mgr)
#define CREATE_ENTITY_NOPOST(cls, mgr) CEntityFactoryMap::GetInstance()->create(cls, mgr, 1)
#define REMOVE_ENTITY(ent) CEntityFactoryMap::GetInstance()->destroy(ent)

#endif
