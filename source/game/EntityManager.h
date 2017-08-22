#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include <gdefines.h>
#include <common/array.h>
#include <chrono>

typedef std::chrono::system_clock::time_point time_point;

class SXbaseEntity;
struct ISXConfig;

#define SET_TIMEOUT(fn, time) m_pMgr->SetTimeout((void(SXbaseEntity::*)(float))&ThisClass::fn, this, time)
#define SET_INTERVAL(fn, time) m_pMgr->SetInterval((void(SXbaseEntity::*)(float))&ThisClass::fn, this, time)
#define CLEAR_INTERVAL(id) m_pMgr->ClearInterval(id)

struct timeout_t
{
	bool done;
	void(SXbaseEntity::*func)(float dt);
	SXbaseEntity * pEnt;
	time_point fStartTime;
	time_point fNextTime;
};

class EntityManager
{
	friend class SXbaseEntity;
public:
	EntityManager();
	~EntityManager();

	void Update(int thread);
	void SetThreadNum(int num);
	void Sync();

	void LoadDefaults();
	void LoadDynClasses();

	void SetTimeout(void(SXbaseEntity::*func)(float dt), SXbaseEntity * pEnt, float delay);
	ID SetInterval(void(SXbaseEntity::*func)(float dt), SXbaseEntity * pEnt, float delay);
	void ClearInterval(ID id);

	SXbaseEntity * FindEntityByName(const char * name, SXbaseEntity * pStart = 0);

	SXbaseEntity * FindEntityByClass(const char * name, SXbaseEntity * pStart = 0);

	bool Export(const char * file);
	bool Import(const char * file);

	void DumpList(int argc, const char ** argv);
	void EntKV(int argc, const char ** argv);

	int GetCount();
	SXbaseEntity * GetById(ID id);

protected:
	ID Register(SXbaseEntity * pEnt);
	void Unregister(ID ent);

	Array<SXbaseEntity*, 64> m_vEntList;
	Array<ID> m_vFreeIDs;

	Array<timeout_t> m_vTimeout;
	Array<timeout_t> m_vInterval;
	Array<ID> m_vFreeInterval;

	int m_iThreadNum;

	//! @warning это нужно хранить в течение работы проги, т.к. таблицы дефолтов ссылаются напрямую на этот объект
	ISXConfig * m_pDefaultsConf;
	ISXConfig * m_pDynClassConf;
};

#endif
