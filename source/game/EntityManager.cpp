
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "EntityManager.h"

#include "BaseEntity.h"

#include <core/sxcore.h>

#include <mutex>

CEntityManager::CEntityManager():
	m_iThreadNum(1),
	m_pDefaultsConf(NULL),
	m_pDynClassConf(NULL)
{
	loadDefaults();
	loadDynClasses();

	Core_0RegisterConcmdClsArg("ent_dump", this, (SXCONCMDCLSARG)&CEntityManager::dumpList);
	Core_0RegisterConcmdClsArg("ent_kv", this, (SXCONCMDCLSARG)&CEntityManager::entKV);
}

CEntityManager::~CEntityManager()
{
	mem_release(m_pDynClassConf);
	mem_release(m_pDefaultsConf);
}

void CEntityManager::update(int thread)
{
	time_point tNow = std::chrono::high_resolution_clock::now();
	timeout_t * t;
	timeout_output_t * to;
	long long mksdt;

	for(int i = thread, l = m_vOutputTimeout.size(); i < l; i += m_iThreadNum)
	{
		to = &m_vOutputTimeout[i];
		if(to->status == TS_WAIT && to->fNextTime < tNow)
		{
			//mksdt = std::chrono::duration_cast<std::chrono::microseconds>(tNow - t->fStartTime).count();
			//(t->pEnt->*(t->func))((float)mksdt / 1000000.0f);
			to->status = TS_DONE;
			for(int j = 0; j < to->pOutput->iOutCount; ++j)
			{
				if(!to->pOutput->pOutputs[j].pTarget)
				{
					continue;
				}
				to->data.parameter = to->pOutput->pOutputs[j].data.parameter;
				to->data.type = to->pOutput->pOutputs[j].data.type;
				to->data.v3Parameter = to->pOutput->pOutputs[j].data.v3Parameter;

				(to->pOutput->pOutputs[j].pTarget->*(to->pOutput->pOutputs[j].fnInput))(&to->data);

				// Update pointer. Array can be reallocated during previous function call
				to = &m_vOutputTimeout[i];
			}
		}
	}

	for(int i = thread, l = m_vTimeout.size(); i < l; i += m_iThreadNum)
	{
		t = &m_vTimeout[i];
		if(t->status == TS_WAIT && t->fNextTime < tNow)
		{
			mksdt = std::chrono::duration_cast<std::chrono::microseconds>(tNow - t->fStartTime).count();
			t->status = TS_DONE;

			// m_vTimeout array can be reallocated during this function call
			(t->pEnt->*(t->func))((float)mksdt / 1000000.0f);
		}
	}

	for(int i = thread, l = m_vInterval.size(); i < l; i += m_iThreadNum)
	{
		t = &m_vInterval[i];
		if(t->status == TS_WAIT && t->fNextTime < tNow)
		{
			mksdt = std::chrono::duration_cast<std::chrono::microseconds>(tNow - t->fStartTime).count();
			t->fNextTime = t->fNextTime - t->fStartTime + tNow;
			t->fStartTime = tNow;

			// m_vInterval array can be reallocated during this function call
			(t->pEnt->*(t->func))((float)mksdt / 1000000.0f);
		}
	}
}

void CEntityManager::setThreadNum(int num)
{
	if(num > 0)
	{
		m_iThreadNum = num;
	}
}

void CEntityManager::sync()
{
	CBaseEntity * pEnt;

	// do not store m_vEntRemoveList.size()! It can change during iteration
	for(int i = 0; i < (int)m_vEntRemoveList.size(); ++i)
	{
		pEnt = m_vEntRemoveList[i];
		if(pEnt)
		{
			pEnt->_cleanup();
			delete pEnt;
		}
	}
	m_vEntRemoveList.clearFast();

	for(int i = 0, l = m_vTimeout.size(); i < l; ++i)
	{
		if(m_vTimeout[i].status == TS_DONE)
		{
			m_vTimeout[i].status = TS_EMPTY;
			m_vFreeTimeout.push_back(i);
		}
	}
	for(int i = 0, l = m_vInterval.size(); i < l; ++i)
	{
		if(m_vInterval[i].status == TS_DONE)
		{
			m_vInterval[i].status = TS_EMPTY;
			m_vFreeInterval.push_back(i);
		}
	}
	for(int i = 0, l = m_vOutputTimeout.size(); i < l; ++i)
	{
		if(m_vOutputTimeout[i].status == TS_DONE)
		{
			m_vOutputTimeout.erase(i);
			--i;
			--l;
		}
	}
	//static time_point tOld = std::chrono::high_resolution_clock::now();
	//float dt;
	//dt = (float)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - tOld).count() / 1000000.0f;
	for(int i = 0, l = m_vEntSyncList.size(); i < l; ++i)
	{
		pEnt = m_vEntSyncList[i];
		if(pEnt)
		{
			pEnt->m_bSynced = false;
		}
	}
	
	for(int i = 0, l = m_vEntSyncList.size(); i < l; ++i)
	{
		pEnt = m_vEntSyncList[i];
		if(pEnt && !pEnt->m_bSynced)
		{
			//pEnt->updateDiscreteLinearVelocity(0, dt);
			pEnt->onSync();
			//pEnt->updateDiscreteLinearVelocity(1, dt);
		}
	}
	//tOld = std::chrono::high_resolution_clock::now();
}

void CEntityManager::unloadObjLevel()
{
	CBaseEntity * pEnt;
	for (int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		if (pEnt && (pEnt->getFlags() & EF_LEVEL))
		{
			REMOVE_ENTITY(pEnt);
		}
	}
}

ID CEntityManager::reg(CBaseEntity *pEnt)
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
void CEntityManager::unreg(ID ent)
{
	//@TODO: Clear all outputs
	if(m_vEntList.size() <= (UINT)ent || ent < 0)
	{
		return;
	}

	timeout_t * t;
	timeout_output_t * to;

	CBaseEntity *pEnt = m_vEntList[ent];

	for(int i = 0, l = m_vOutputTimeout.size(); i < l; ++i)
	{
		to = &m_vOutputTimeout[i];
		if(to->status == TS_WAIT)
		{
			to->status = TS_DONE;
			for(int j = 0; j < to->pOutput->iOutCount; ++j)
			{
				if(to->pOutput->pOutputs[j].pTarget == pEnt)
				{
					to->pOutput->pOutputs[j].pTarget = NULL;
				}
			}
		}
	}

	for(int i = 0, l = m_vTimeout.size(); i < l; ++i)
	{
		t = &m_vTimeout[i];
		if(t->status == TS_WAIT && t->pEnt == pEnt)
		{
			t->status = TS_DONE;
		}
	}

	for(int i = 0, l = m_vInterval.size(); i < l; ++i)
	{
		t = &m_vInterval[i];
		if(t->status == TS_WAIT && t->pEnt == pEnt)
		{
			t->status = TS_DONE;
		}
	}

	m_vEntList[ent] = NULL;
	m_vFreeIDs.push_back(ent);
}

void CEntityManager::regSync(CBaseEntity *pEnt)
{
	assert(pEnt);
	m_vEntSyncList.push_back(pEnt);
}
void CEntityManager::unregSync(CBaseEntity *pEnt)
{
	assert(pEnt);
	for(UINT i = 0, l = m_vEntSyncList.size(); i < l; ++i)
	{
		if(m_vEntSyncList[i] == pEnt)
		{
			m_vEntSyncList.erase(i);
			break;
		}
	}
}

ID CEntityManager::setTimeout(void(CBaseEntity::*func)(float dt), CBaseEntity *pEnt, float delay)
{
	timeout_t t;
	t.status = TS_WAIT;
	t.func = func;
	t.pEnt = pEnt;

	t.fStartTime = t.fNextTime = std::chrono::high_resolution_clock::now();
	t.fNextTime += std::chrono::microseconds((long long)(delay * 1000000.0f));
	
	ID id;
	{
		std::unique_lock<std::mutex> lock;
		if(m_vFreeTimeout.size())
		{
			id = m_vFreeTimeout[0];
			m_vFreeTimeout.erase(0);
		}
		else
		{
			id = m_vTimeout.size();
		}
		m_vTimeout[id] = t;
	}

	return(id);
}

ID CEntityManager::setInterval(void(CBaseEntity::*func)(float dt), CBaseEntity *pEnt, float delay)
{
	timeout_t t;
	t.status = TS_WAIT;
	t.func = func;
	t.pEnt = pEnt;

	t.fStartTime = t.fNextTime = std::chrono::high_resolution_clock::now();
	t.fNextTime += std::chrono::microseconds((long long)(delay * 1000000.0f));

	ID id;
	{
		std::unique_lock<std::mutex> lock;
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
	}

	return(id);
}

void CEntityManager::clearInterval(ID id)
{
	if(id < 0 || (UINT)id >= m_vInterval.size())
	{
		return;
	}
	if(m_vInterval[id].status == TS_WAIT)
	{
		m_vInterval[id].status = TS_DONE;
	}
}
void CEntityManager::clearTimeout(ID id)
{
	if(id < 0 || (UINT)id >= m_vTimeout.size())
	{
		return;
	}
	if(m_vTimeout[id].status == TS_WAIT)
	{
		m_vTimeout[id].status = TS_DONE;
	}
}

bool CEntityManager::exportList(const char * file)
{
	ISXConfig * conf = Core_CrConfig();
	conf->New(file);
	char buf[4096], sect[32];
	CBaseEntity * pEnt;
	proptable_t * pTbl;
	int ic = 0;

	conf->set("meta", "count", "0");

	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		if(!pEnt)
		{
			continue;
		}
		sprintf(sect, "ent_%d", ic);

		if(!(pEnt->getFlags() & EF_EXPORT))
		{
			continue;
		}

		conf->set(sect, "classname", pEnt->getClassName());
		pTbl = CEntityFactoryMap::GetInstance()->getPropTable(pEnt->getClassName());
		do
		{
			for(int j = 0; j < pTbl->numFields; ++j)
			{
				if(pTbl->pData[j].szKey && !(pTbl->pData[j].flags & (PDFF_INPUT | PDFF_NOEXPORT)) && !conf->keyExists(sect, pTbl->pData[j].szKey))
				{
					pEnt->getKV(pTbl->pData[j].szKey, buf, sizeof(buf));
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

bool CEntityManager::import(const char * file, bool shouldSendProgress)
{
	IEventChannel<XEventLevelProgress> *pEventChannel = NULL;
	if(shouldSendProgress)
	{
		pEventChannel = Core_GetIXCore()->getEventChannel<XEventLevelProgress>(EVENT_LEVEL_PROGRESS_GUID);
	}
	ISXConfig * conf = Core_CrConfig();
	char sect[32];
	CBaseEntity * pEnt = NULL;
	Array<CBaseEntity*> tmpList;
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

	{

		XEventLevelProgress ev;
		ev.idPlugin = -3;
		ev.szLoadingText = "Загрузка объектов игрового мира";
		ev.type = XEventLevelProgress::TYPE_PROGRESS_VALUE;
		ev.fProgress = 0.0f;
		for(int i = 0; i < ic; ++i)
		{
			sprintf(sect, "ent_%d", i);
			if(!conf->keyExists(sect, "classname"))
			{
				printf(COLOR_LRED "Unable to load entity #%d, classname undefined\n" COLOR_RESET, i);
				tmpList[i] = NULL;
				continue;
			}
			if(!(pEnt = CREATE_ENTITY_NOPOST(conf->getKey(sect, "classname"), this)))
			{
				printf(COLOR_LRED "Unable to load entity #%d, classname '%s' undefined\n" COLOR_RESET, i, conf->getKey(sect, "classname"));
				tmpList[i] = NULL;
				continue;
			}
			if(conf->keyExists(sect, "name"))
			{
				pEnt->setKV("name", conf->getKey(sect, "name"));
			}
			if(conf->keyExists(sect, "origin"))
			{
				pEnt->setKV("origin", conf->getKey(sect, "origin"));
			}
			if(conf->keyExists(sect, "rotation"))
			{
				pEnt->setKV("rotation", conf->getKey(sect, "rotation"));
			}
			pEnt->setFlags(pEnt->getFlags() | EF_EXPORT | EF_LEVEL);
			tmpList[i] = pEnt;

			if((i & 0xF) == 0 && pEventChannel)
			{
				ev.fProgress = (float)i / (float)ic * 0.1f;
				pEventChannel->broadcastEvent(&ev);
			}
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
					pEnt->setKV(key, conf->getKey(sect, key));
				}
			}
			pEnt->onPostLoad();

			if(/*(i & 0xF) == 0 && */pEventChannel)
			{
				ev.fProgress = (float)i / (float)ic * 0.9f + 0.1f;
				pEventChannel->broadcastEvent(&ev);
			}
		}
	}

	mem_release(conf);
	return(true);

err:
	mem_release(conf);
	return(false);
}


CBaseEntity* CEntityManager::findEntityByName(const char *szName, CBaseEntity *pStart)
{
	if(!szName[0])
	{
		return(NULL);
	}
	bool bFound = !pStart;
	CBaseEntity * pEnt;
	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		if(!pEnt)
		{
			continue;
		}
		if(bFound)
		{
			if(!strcmp(pEnt->getName(), szName))
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

int CEntityManager::countEntityByName(const char *name)
{
	if(!name[0])
	{
		return(0);
	}
	int c = 0;
	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		if(m_vEntList[i] && !strcmp(m_vEntList[i]->getName(), name))
		{
			++c;
		}
	}
	return(c);
}

CBaseEntity* CEntityManager::findEntityByClass(const char *name, CBaseEntity *pStart)
{
	bool bFound = !pStart;
	CBaseEntity * pEnt;
	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		if(!pEnt)
		{
			continue;
		}
		if(bFound)
		{
			if(!strcmp(pEnt->getClassName(), name))
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

CBaseEntity* CEntityManager::findEntityInSphere(const float3 &f3Origin, float fRadius, CBaseEntity *pStart)
{
	bool bFound = !pStart;
	CBaseEntity * pEnt;
	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		pEnt = m_vEntList[i];
		if(!pEnt)
		{
			continue;
		}
		if(bFound)
		{
			if(SMVector3Length2(pEnt->getPos() - f3Origin) < fRadius * fRadius)
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

void CEntityManager::loadDefaults()
{
	m_pDefaultsConf = Core_CrConfig();
	if(m_pDefaultsConf->open("config/entities/defaults.ent") < 0)
	{
		mem_release(m_pDefaultsConf);
		return;
	}

	const char * sect, * key;
	EntDefaultsMap * defs;

	for(int i = 0, l = m_pDefaultsConf->getSectionCount(); i < l; ++i)
	{
		sect = m_pDefaultsConf->getSectionName(i);
		if(!(defs = CEntityFactoryMap::GetInstance()->getDefaults(sect)))
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

void CEntityManager::loadDynClasses()
{
	m_pDynClassConf = Core_CrConfig();
	if(m_pDynClassConf->open("config/entities/classes.ent") < 0)
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
		IEntityFactory * pOldFactory = CEntityFactoryMap::GetInstance()->getFactory(baseClass);
		if(!pOldFactory)
		{
			printf(COLOR_LRED "Couldn't create entity class '%s': Base class '%s' is undefined\n" COLOR_RESET, newClass, baseClass);
			continue;
		}
		if((key = m_pDynClassConf->getKey(newClass, "show_in_listing")))
		{
			bShow = strcmp(key, "0") && strcmp(key, "false");
		}
		IEntityFactory * newFactory = pOldFactory->copy(newClass, bShow);
		CEntityFactoryMap::GetInstance()->addFactory(newFactory, newClass);

		if(!(defs = CEntityFactoryMap::GetInstance()->getDefaults(newClass)))
		{
			continue;
		}
		if((baseDefs = CEntityFactoryMap::GetInstance()->getDefaults(baseClass)))
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

void CEntityManager::dumpList(int argc, const char **argv)
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
		CBaseEntity * pEnt = m_vEntList[i];
		if(!pEnt)
		{
			continue;
		}
		if(!filter[0] || strstr(pEnt->getClassName(), filter))
		{
			printf(" " COLOR_LGREEN "%4d" COLOR_GREEN " | " COLOR_LGREEN "%24s" COLOR_GREEN " | " COLOR_LGREEN "%16s" COLOR_GREEN " |\n", i, pEnt->getClassName(), pEnt->getName());
		}
	}

	printf("-----------------------------------------------------\n" COLOR_RESET);
}

void CEntityManager::entKV(int argc, const char **argv)
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
	CBaseEntity * pEnt;
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
			proptable_t * pt = CEntityFactoryMap::GetInstance()->getPropTable(pEnt->getClassName());
			while(pt)
			{
				for(int i = 0; i < pt->numFields; ++i)
				{
					if(pt->pData[i].szKey)
					{
						pEnt->getKV(pt->pData[i].szKey, buf, sizeof(buf));
						printf("%s = %s\n", pt->pData[i].szKey, buf);
					}
				}
				pt = pt->pBaseProptable;
			}
		}
		break;
	case 3: // show KV
		pEnt->getKV(argv[2], buf, sizeof(buf));
		printf("%s = %s\n", argv[2], buf);
		break;

	case 4: // set new KV
		pEnt->setKV(argv[2], argv[3]);
		break;
	}
}

int CEntityManager::getCount()
{
	return(m_vEntList.size());
}

CBaseEntity* CEntityManager::getById(ID id)
{
	if(id < 0 || (UINT)id >= m_vEntList.size())
	{
		return(NULL);
	}
	CBaseEntity *pEnt = m_vEntList[id];
	return(pEnt ? (pEnt->getFlags() & EF_REMOVED ? NULL : pEnt) : NULL);
}

CBaseEntity* CEntityManager::cloneEntity(CBaseEntity *pOther)
{
	if(!pOther)
	{
		return(NULL);
	}
	CBaseEntity *pEnt = CREATE_ENTITY_NOPOST(pOther->getClassName(), this);

	pEnt->setKV("name", pOther->getName());
	pEnt->setPos(pOther->getPos());
	pEnt->setOrient(pOther->getOrient());
	pEnt->setParent(pOther->getParent());
	pEnt->setOwner(pOther->getOwner());
	char buf[4096];

	proptable_t *pTbl = CEntityFactoryMap::GetInstance()->getPropTable(pOther->getClassName());
	do
	{
		for(int j = 0; j < pTbl->numFields; ++j)
		{
			if(pTbl->pData[j].szKey && !(pTbl->pData[j].flags & PDFF_INPUT))
			{
				if(fstrcmp(pTbl->pData[j].szKey, "origin") 
					&& fstrcmp(pTbl->pData[j].szKey, "name") 
					&& fstrcmp(pTbl->pData[j].szKey, "rotation") 
					&& fstrcmp(pTbl->pData[j].szKey, "parent") 
					&& fstrcmp(pTbl->pData[j].szKey, "owner"))
				{
					pOther->getKV(pTbl->pData[j].szKey, buf, sizeof(buf));
					pEnt->setKV(pTbl->pData[j].szKey, buf);
				}
			}
		}
	}
	while((pTbl = pTbl->pBaseProptable));

	pEnt->setFlags(pOther->getFlags());

	pEnt->onPostLoad();

	return(pEnt);
}

void CEntityManager::setOutputTimeout(named_output_t *pOutput, inputdata_t *pData)
{
	timeout_output_t t;
	t.status = TS_WAIT;
	t.pOutput = pOutput;
	t.data = *pData;

	t.fStartTime = t.fNextTime = std::chrono::high_resolution_clock::now();
	t.fNextTime += std::chrono::microseconds((long long)(pOutput->fDelay * 1000000.0f));

	m_vOutputTimeout.push_back(t);
}

void CEntityManager::sheduleDestroy(CBaseEntity *pEnt)
{
	if(pEnt->getFlags() & EF_REMOVED)
	{
		return;
	}
	pEnt->setFlags(pEnt->getFlags() | EF_REMOVED);
	m_vEntRemoveList.push_back(pEnt);

	if(m_isEditorMode)
	{
		pEnt->_releaseEditorBoxes();
	}
}

void CEntityManager::setEditorMode(bool isEditor)
{
	if(m_isEditorMode == isEditor)
	{
		return;
	}
	m_isEditorMode = isEditor;

	for(int i = 0, l = m_vEntList.size(); i < l; ++i)
	{
		CBaseEntity * pEnt = m_vEntList[i];
		if(!pEnt)
		{
			continue;
		}
		if(isEditor)
		{
			pEnt->_initEditorBoxes();
		}
		else
		{
			pEnt->_releaseEditorBoxes();
		}
	}
}

bool CEntityManager::isEditorMode()
{
	return(m_isEditorMode);
}
