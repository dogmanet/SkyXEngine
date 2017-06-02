#include "EntityFactory.h"
#include "SXbaseEntity.h"


EntityFactoryMap::EntityFactoryMap()
{

}

void EntityFactoryMap::AddFactory(IEntityFactory * pFactory, const char * szName)
{
	m_mFactories[szName] = pFactory;
}
SXbaseEntity * EntityFactoryMap::Create(const char * szName, EntityManager * pWorld)
{
	IEntityFactory * pFactory = GetFactory(szName);
	if(pFactory)
	{
		SXbaseEntity * pEnt = pFactory->Create(pWorld);
		pEnt->SetClassName(pFactory->GetClassName());
		return(pEnt);
	}
	return(NULL);
}
void EntityFactoryMap::Destroy(SXbaseEntity * pEnt)
{
	if(pEnt)
	{
		IEntityFactory * pFactory = GetFactory(pEnt->GetClassName());
		if(pFactory)
		{
			return(pFactory->Destroy(pEnt));
		}
	}
}

IEntityFactory * EntityFactoryMap::GetFactory(const char * szName)
{
	if(m_mFactories.KeyExists(szName))
	{
		return(m_mFactories[szName]);
	}
	return(NULL);
}

EntityFactoryMap * EntityFactoryMap::GetInstance()
{
	static EntityFactoryMap map;
	return(&map);
}

proptable_t * EntityFactoryMap::GetPropTable(const char * szClass)
{
	return(GetFactory(szClass)->GetPropTable());
}

bool EntityFactoryMap::IsEditorHidden(const char * szClass)
{
	return(GetFactory(szClass)->IsEditorHidden());
}