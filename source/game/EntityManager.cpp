#include "EntityManager.h"

#include "SXbaseEntity.h"

#include <core/sxcore.h>

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
	ISXLConfig * conf = Core_CrLConfig();
	conf->New(file);
	char buf[512], sect[32];
	SXbaseEntity * pEnt;
	proptable_t * pTbl;
	int ic = 0;

	conf->Set("meta", "count", "0");

	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		sprintf(sect, "ent_%d", ic);

		if(!(pEnt->GetFlags() & EF_EXPORT))
		{
			continue;
		}

		conf->Set(sect, "classname", pEnt->GetClassName());
		pTbl = EntityFactoryMap::GetInstance()->GetPropTable(pEnt->GetClassName());
		do
		{
			for(int j = 0; j < pTbl->numFields; ++j)
			{
				if(pTbl->pData[j].szKey && !conf->KeyExists(sect, pTbl->pData[j].szKey))
				{
					pEnt->GetKV(pTbl->pData[j].szKey, buf, sizeof(buf));
					conf->Set(sect, pTbl->pData[j].szKey, buf);
				}
			}
		}
		while((pTbl = pTbl->pBaseProptable));
		++ic;
	}

	sprintf(buf, "%d", ic);
	conf->Set("meta", "count", buf);

	bool ret = !conf->Save();
	mem_release(conf);
	return(ret);
}

bool EntityManager::Import(const char * file)
{
	ISXLConfig * conf = Core_CrLConfig();
	char sect[32];
	SXbaseEntity * pEnt;
	Array<SXbaseEntity*> tmpList;
	if(conf->Open(file))
	{
		goto err;
	}

	if(!conf->KeyExists("meta", "count"))
	{
		goto err;
	}

	int ic;
	if(sscanf(conf->GetKey("meta", "count"), "%d", &ic) != 1)
	{
		goto err;
	}

	tmpList.reserve(ic);

	for(int i = 0; i < ic; ++i)
	{
		sprintf(sect, "ent_%d", i);
		if(!conf->KeyExists(sect, "classname"))
		{
			printf(COLOR_LRED "Unable to load entity #%d, classname undefined\n" COLOR_RESET, i);
			continue;
		}
		if(!(pEnt = CREATE_ENTITY(conf->GetKey(sect, "classname"), this)))
		{
			printf(COLOR_LRED "Unable to load entity #%d, classname '%s' undefined\n" COLOR_RESET, i, conf->GetKey(sect, "classname"));
			continue;
		}
		if(conf->KeyExists(sect, "name"))
		{
			pEnt->SetKV("name", conf->GetKey(sect, "name"));
		}
		if(conf->KeyExists(sect, "origin"))
		{
			pEnt->SetKV("origin", conf->GetKey(sect, "origin"));
		}
		if(conf->KeyExists(sect, "rotation"))
		{
			pEnt->SetKV("rotation", conf->GetKey(sect, "rotation"));
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
		int keyc = conf->GetKeyCount(sect);
		const char * key;
		for(int j = 0; j < keyc; ++j)
		{
			key = conf->GetKeyName(sect, j);
 			if(strcmp(key, "classname") && strcmp(key, "origin") && strcmp(key, "name") && strcmp(key, "rotation"))
			{
				pEnt->SetKV(key, conf->GetKey(sect, key));
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
