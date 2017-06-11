#include "EntityFactory.h"
#include "SXbaseEntity.h"


EntityFactoryMap::EntityFactoryMap():
	m_iShowInListCount(0)
{

}

void EntityFactoryMap::AddFactory(IEntityFactory * pFactory, const char * szName)
{
	if(!pFactory->IsEditorHidden())
	{
		++m_iShowInListCount;
	}
	m_mFactories[AAString(szName)] = pFactory;
}
SXbaseEntity * EntityFactoryMap::Create(const char * szName, EntityManager * pWorld)
{
	IEntityFactory * pFactory = GetFactory(szName);
	if(pFactory)
	{
		EntDefaultsMap * defs = pFactory->GetDefaults();
		SXbaseEntity * pEnt = pFactory->Create(pWorld);
		pEnt->SetDefaults();
		pEnt->SetClassName(pFactory->GetClassName());

		if(defs->Size() > 0)
		{
			proptable_t * pt = pFactory->GetPropTable();
			const char * key;
			const EntDefaultsMap::Node * pNode;
			while(pt)
			{
				for(int i = 0, l = pt->numFields; i < l; ++i)
				{
					key = pt->pData[i].szKey;
					if(defs->KeyExists(AAString(key), &pNode))
					{
						pEnt->SetKV(key, *(pNode->Val));
					}
				}

				pt = pt->pBaseProptable;
			}
		}

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
	AAString key(szName);
	if(m_mFactories.KeyExists(key))
	{
		return(m_mFactories[key]);
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

EntDefaultsMap * EntityFactoryMap::GetDefaults(const char * szClass)
{
	return(GetFactory(szClass)->GetDefaults());
}

int EntityFactoryMap::GetListCount()
{
	return(m_iShowInListCount);
}

void EntityFactoryMap::GetListing(const char ** pszOut, int size)
{
	int j = 0;
	if(size > m_iShowInListCount)
	{
		size = m_iShowInListCount;
	}
	for(AssotiativeArray<AAString, IEntityFactory*>::Iterator i = m_mFactories.begin(); i && j < size; i++)
	{
		if(!(*i.second)->IsEditorHidden())
		{
			pszOut[j++] = i.first->GetName();
		}
	}
}
