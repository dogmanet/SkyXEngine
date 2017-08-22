#include "EntityManager.h"

#include "SXbaseEntity.h"

#include <core/sxcore.h>

EntityManager::EntityManager():
	m_iThreadNum(1),
	m_pDefaultsConf(NULL),
	m_pDynClassConf(NULL)
{
	LoadDefaults();
	LoadDynClasses();

	Core_0RegisterConcmdClsArg("ent_dump", this, (SXCONCMDCLSARG)&EntityManager::DumpList);
	Core_0RegisterConcmdClsArg("ent_kv", this, (SXCONCMDCLSARG)&EntityManager::EntKV);
}

EntityManager::~EntityManager()
{
	mem_release(m_pDynClassConf);
	mem_release(m_pDefaultsConf);
}

void EntityManager::Update(int thread)
{
	time_point tNow = std::chrono::high_resolution_clock::now();
	timeout_t * t;
	long long mksdt;

	if(thread == 0)
	{
		for(int i = 0, l = m_vTimeout.size(); i < l; ++i)
		{
			t = &m_vTimeout[i];
			if(t->fNextTime < tNow)
			{
				mksdt = std::chrono::duration_cast<std::chrono::microseconds>(tNow - t->fStartTime).count();
				(t->pEnt->*(t->func))((float)mksdt / 1000000.0f);
				m_vTimeout.erase(i);
				--l;
				--i;
			}
		}
	}
	for(int i = thread, l = m_vInterval.size(); i < l; i += m_iThreadNum)
	{
		t = &m_vInterval[i];
		if(!t->done && t->fNextTime < tNow)
		{
			mksdt = std::chrono::duration_cast<std::chrono::microseconds>(tNow - t->fStartTime).count();
			t->fNextTime = t->fNextTime - t->fStartTime + tNow;
			t->fStartTime = tNow;
			(t->pEnt->*(t->func))((float)mksdt / 1000000.0f);
		}
	}
}

void EntityManager::SetThreadNum(int num)
{
	if(num > 0)
	{
		m_iThreadNum = num;
	}
}

void EntityManager::Sync()
{
	SXbaseEntity * pEnt;
	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		if(pEnt)
		{
			pEnt->OnSync();
		}
	}
}

ID EntityManager::Register(SXbaseEntity * pEnt)
{
	ID ent;
	if(!pEnt)
	{
		return(-1);
	}
	if(m_vFreeIDs.size())
	{
		ent = m_vFreeIDs[0];
		m_vFreeIDs.erase(0);
		m_vEntList[ent] = pEnt;
	}
	else
	{
		ent = m_vEntList.size();
		m_vEntList.push_back(pEnt);
	}
	return(ent);
}
void EntityManager::Unregister(ID ent)
{
	if(m_vEntList.size() <= (UINT)ent || ent < 0)
	{
		return;
	}
	m_vEntList[ent] = NULL;
	m_vFreeIDs.push_back(ent);
}

void EntityManager::SetTimeout(void(SXbaseEntity::*func)(float dt), SXbaseEntity * pEnt, float delay)
{
	timeout_t t;
	t.done = false;
	t.func = func;
	t.pEnt = pEnt;

	t.fStartTime = t.fNextTime = std::chrono::high_resolution_clock::now();
	t.fNextTime += std::chrono::microseconds((long long)(delay * 1000000.0f));
	
	m_vTimeout.push_back(t);
}

ID EntityManager::SetInterval(void(SXbaseEntity::*func)(float dt), SXbaseEntity * pEnt, float delay)
{
	timeout_t t;
	t.done = false;
	t.func = func;
	t.pEnt = pEnt;

	t.fStartTime = t.fNextTime = std::chrono::high_resolution_clock::now();
	t.fNextTime += std::chrono::microseconds((long long)(delay * 1000000.0f));

	ID id;
	if(m_vFreeInterval.size())
	{
		id = m_vFreeInterval[0];
		m_vFreeInterval.erase(0);
	}
	else
	{
		id = m_vInterval.size();
	}

	m_vInterval[id] = t;

	return(id);
}

void EntityManager::ClearInterval(ID id)
{
	if(id < 0 || (UINT)id >= m_vInterval.size())
	{
		return;
	}
	m_vInterval[id].done = true;
	m_vFreeInterval.push_back(id);
}

bool EntityManager::Export(const char * file)
{
	ISXConfig * conf = Core_CrConfig();
	conf->New(file);
	char buf[512], sect[32];
	SXbaseEntity * pEnt;
	proptable_t * pTbl;
	int ic = 0;

	conf->set("meta", "count", "0");

	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		sprintf(sect, "ent_%d", ic);

		if(!(pEnt->GetFlags() & EF_EXPORT))
		{
			continue;
		}

		conf->set(sect, "classname", pEnt->GetClassName());
		pTbl = EntityFactoryMap::GetInstance()->GetPropTable(pEnt->GetClassName());
		do
		{
			for(int j = 0; j < pTbl->numFields; ++j)
			{
				if(pTbl->pData[j].szKey && !conf->keyExists(sect, pTbl->pData[j].szKey))
				{
					pEnt->GetKV(pTbl->pData[j].szKey, buf, sizeof(buf));
					conf->set(sect, pTbl->pData[j].szKey, buf);
				}
			}
		}
		while((pTbl = pTbl->pBaseProptable));
		++ic;
	}

	sprintf(buf, "%d", ic);
	conf->set("meta", "count", buf);

	bool ret = !conf->save();
	mem_release(conf);
	return(ret);
}

bool EntityManager::Import(const char * file)
{
	ISXConfig * conf = Core_CrConfig();
	char sect[32];
	SXbaseEntity * pEnt = NULL;
	Array<SXbaseEntity*> tmpList;
	if(conf->open(file))
	{
		goto err;
	}

	if(!conf->keyExists("meta", "count"))
	{
		goto err;
	}

	int ic;
	if(sscanf(conf->getKey("meta", "count"), "%d", &ic) != 1)
	{
		goto err;
	}

	tmpList.reserve(ic);

	for(int i = 0; i < ic; ++i)
	{
		sprintf(sect, "ent_%d", i);
		if(!conf->keyExists(sect, "classname"))
		{
			printf(COLOR_LRED "Unable to load entity #%d, classname undefined\n" COLOR_RESET, i);
			tmpList[i] = NULL;
			continue;
		}
		if(!(pEnt = CREATE_ENTITY(conf->getKey(sect, "classname"), this)))
		{
			printf(COLOR_LRED "Unable to load entity #%d, classname '%s' undefined\n" COLOR_RESET, i, conf->getKey(sect, "classname"));
			tmpList[i] = NULL;
			continue;
		}
		if(conf->keyExists(sect, "name"))
		{
			pEnt->SetKV("name", conf->getKey(sect, "name"));
		}
		if(conf->keyExists(sect, "origin"))
		{
			pEnt->SetKV("origin", conf->getKey(sect, "origin"));
		}
		if(conf->keyExists(sect, "rotation"))
		{
			pEnt->SetKV("rotation", conf->getKey(sect, "rotation"));
		}
		pEnt->SetFlags(pEnt->GetFlags() | EF_EXPORT | EF_LEVEL);
		tmpList[i] = pEnt;
	}

	for(int i = 0; i < ic; ++i)
	{
		sprintf(sect, "ent_%d", i);
		if(!(pEnt = tmpList[i]))
		{
			continue;
		}
		int keyc = conf->getKeyCount(sect);
		const char * key;
		for(int j = 0; j < keyc; ++j)
		{
			key = conf->getKeyName(sect, j);
 			if(strcmp(key, "classname") && strcmp(key, "origin") && strcmp(key, "name") && strcmp(key, "rotation"))
			{
				pEnt->SetKV(key, conf->getKey(sect, key));
			}
		}
		pEnt->OnPostLoad();
	}

	mem_release(conf);
	return(true);

err:
	mem_release(conf);
	return(false);
}


SXbaseEntity * EntityManager::FindEntityByName(const char * name, SXbaseEntity * pStart)
{
	if(!name[0])
	{
		return(NULL);
	}
	bool bFound = !pStart;
	SXbaseEntity * pEnt;
	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		if(bFound)
		{
			if(!strcmp(pEnt->GetName(), name))
			{
				return(pEnt);
			}
		}
		else
		{
			if(pEnt == pStart)
			{
				bFound = true;
			}
		}
	}
	return(NULL);
}

SXbaseEntity * EntityManager::FindEntityByClass(const char * name, SXbaseEntity * pStart)
{
	bool bFound = !pStart;
	SXbaseEntity * pEnt;
	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		if(bFound)
		{
			if(!strcmp(pEnt->GetClassName(), name))
			{
				return(pEnt);
			}
		}
		else
		{
			if(pEnt == pStart)
			{
				bFound = true;
			}
		}
	}
	return(NULL);
}

void EntityManager::LoadDefaults()
{
	m_pDefaultsConf = Core_CrConfig();
	if(m_pDefaultsConf->open("entities/defaults.ent") < 0)
	{
		mem_release(m_pDefaultsConf);
		return;
	}

	const char * sect, * key;
	EntDefaultsMap * defs;

	for(int i = 0, l = m_pDefaultsConf->getSectionCount(); i < l; ++i)
	{
		sect = m_pDefaultsConf->getSectionName(i);
		if(!(defs = EntityFactoryMap::GetInstance()->GetDefaults(sect)))
		{
			continue;
		}
		for(int j = 0, jl = m_pDefaultsConf->getKeyCount(sect); j < jl; ++j)
		{
			key = m_pDefaultsConf->getKeyName(sect, j);
			defs[0][AAString(key)] = m_pDefaultsConf->getKey(sect, key);
		}
	}
}

void EntityManager::LoadDynClasses()
{
	m_pDynClassConf = Core_CrConfig();
	if(m_pDynClassConf->open("entities/classes.ent") < 0)
	{
		mem_release(m_pDynClassConf);
		return;
	}

	const char * newClass, *key, *baseClass;
	EntDefaultsMap * defs, *baseDefs;
	bool bShow = true;

	for(int i = 0, l = m_pDynClassConf->getSectionCount(); i < l; ++i)
	{
		newClass = m_pDynClassConf->getSectionName(i);
		if(!(baseClass = m_pDynClassConf->getKey(newClass, "base_class")))
		{
			printf(COLOR_LRED "Couldn't create entity class '%s': Unknown base class\n" COLOR_RESET, newClass);
			continue;
		}
		IEntityFactory * pOldFactory = EntityFactoryMap::GetInstance()->GetFactory(baseClass);
		if(!pOldFactory)
		{
			printf(COLOR_LRED "Couldn't create entity class '%s': Base class '%s' is undefined\n" COLOR_RESET, newClass, baseClass);
			continue;
		}
		if((key = m_pDynClassConf->getKey(newClass, "show_in_listing")))
		{
			bShow = strcmp(key, "0") && strcmp(key, "false");
		}
		IEntityFactory * newFactory = pOldFactory->Copy(newClass, bShow);
		EntityFactoryMap::GetInstance()->AddFactory(newFactory, newClass);

		if(!(defs = EntityFactoryMap::GetInstance()->GetDefaults(newClass)))
		{
			continue;
		}
		if((baseDefs = EntityFactoryMap::GetInstance()->GetDefaults(baseClass)))
		{
			for(EntDefaultsMap::Iterator i = baseDefs->begin(); i; i++)
			{
				defs[0][*i.first] = *i.second;
			}
		}
		for(int j = 0, jl = m_pDynClassConf->getKeyCount(newClass); j < jl; ++j)
		{
			key = m_pDynClassConf->getKeyName(newClass, j);
			defs[0][AAString(key)] = m_pDynClassConf->getKey(newClass, key);
		}
	}
}

void EntityManager::DumpList(int argc, const char ** argv)
{
	const char * filter = "";
	if(argc > 1)
	{
		filter = argv[1];
	}

	printf(COLOR_GREEN "-----------------------------------------------------\n"
		COLOR_GREEN "    Filter: " COLOR_LGREEN "%s\n"
		COLOR_GREEN "    Total count: " COLOR_LGREEN "%d\n"
		COLOR_GREEN "-----------------------------------------------------\n"
		"  id  |          class           |       name       |\n"
		"------|--------------------------|------------------|\n"
		, filter, m_vEntList.size());

	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		SXbaseEntity * pEnt = m_vEntList[i];
		if(!pEnt)
		{
			continue;
		}
		if(!filter[0] || strstr(pEnt->GetClassName(), filter))
		{
			printf(" " COLOR_LGREEN "%4d" COLOR_GREEN " | " COLOR_LGREEN "%24s" COLOR_GREEN " | " COLOR_LGREEN "%16s" COLOR_GREEN " |\n", i, pEnt->GetClassName(), pEnt->GetName());
		}
	}

	printf("-----------------------------------------------------\n" COLOR_RESET);
}

void EntityManager::EntKV(int argc, const char ** argv)
{
	int id = 0;
	if(argc == 1)
	{
		printf("Usage: \n    ent_kv <entid> [keyname [new_value]]\n");
		return;
	}
	if(!sscanf(argv[1], "%d", &id))
	{
		printf("Usage: \n    ent_kv <entid> [keyname [new_value]]\n");
		return;
	}
	SXbaseEntity * pEnt;
	if(id < 0 || (UINT)id >= m_vEntList.size() || !(pEnt = m_vEntList[id]))
	{
		printf(COLOR_LRED "Invalid entity id\n" COLOR_RESET);
	}
	char buf[128];
	buf[0] = 0;
	switch(argc)
	{
	case 2: // dump all KVs
		{
			proptable_t * pt = EntityFactoryMap::GetInstance()->GetPropTable(pEnt->GetClassName());
			while(pt)
			{
				for(int i = 0; i < pt->numFields; ++i)
				{
					if(pt->pData[i].szKey)
					{
						pEnt->GetKV(pt->pData[i].szKey, buf, sizeof(buf));
						printf("%s = %s\n", pt->pData[i].szKey, buf);
					}
				}
				pt = pt->pBaseProptable;
			}
		}
		break;
	case 3: // show KV
		pEnt->GetKV(argv[2], buf, sizeof(buf));
		printf("%s = %s\n", argv[2], buf);
		break;

	case 4: // set new KV
		pEnt->SetKV(argv[2], argv[3]);
		break;
	}
}

int EntityManager::GetCount()
{
	return(m_vEntList.size());
}

SXbaseEntity * EntityManager::GetById(ID id)
{
	if(id < 0 || (UINT)id >= m_vEntList.size())
	{
		return(NULL);
	}
	return(m_vEntList[id]);
}
