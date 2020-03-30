
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __ENTITY_MANAGER_H
#define __ENTITY_MANAGER_H

/*! \file */

#include <gdefines.h>
#include <common/array.h>
#include <chrono>
#include <mutex>

#include "proptable.h"

typedef std::chrono::high_resolution_clock::time_point time_point;

class CBaseEntity;
struct ISXConfig;

/*! Однократно запустить функцию fn через время time
	\note Должно вызываться изнутри класса объекта
*/
#define SET_TIMEOUT(fn, time) m_pMgr->setTimeout((void(CBaseEntity::*)(float))&ThisClass::fn, this, time)

/*! Запускать функцию fn через каждые time секунд
Возвращает идентификатор таймера
\note Должно вызываться изнутри класса объекта
*/
#define SET_INTERVAL(fn, time) m_pMgr->setInterval((void(CBaseEntity::*)(float))&ThisClass::fn, this, time)

/*! Отменить интервал по идентификатору
\note Должно вызываться изнутри класса объекта
*/
#define CLEAR_INTERVAL(id) m_pMgr->clearInterval(id)

/*! Отменить таймер по идентификатору
\note Должно вызываться изнутри класса объекта
*/
#define CLEAR_TIMEOUT(id) m_pMgr->clearTimeout(id)

enum TIMEOUT_STATUS
{
	TS_WAIT = 0,
	TS_DONE,
	TS_EMPTY
};

struct timeout_t
{
	TIMEOUT_STATUS status;
	void(CBaseEntity::*func)(float dt);
	CBaseEntity * pEnt;
	time_point fStartTime;
	time_point fNextTime;
};

struct timeout_output_t
{
	TIMEOUT_STATUS status;
	named_output_t *pOutput;
	time_point fStartTime;
	time_point fNextTime;
	inputdata_t data;
};

class CEntityManager
{
	friend class CBaseEntity;
	friend class CEntityFactoryMap;
public:
	CEntityManager();
	~CEntityManager();

	void update(int thread);
	void setThreadNum(int num);
	void sync();

	void unloadObjLevel();

	void loadDefaults();
	void loadDynClasses();

	ID setTimeout(void(CBaseEntity::*func)(float dt), CBaseEntity *pEnt, float delay);
	ID setInterval(void(CBaseEntity::*func)(float dt), CBaseEntity *pEnt, float delay);
	void setOutputTimeout(named_output_t * pOutput, inputdata_t *pData);
	void clearTimeout(ID id);
	void clearInterval(ID id);

	int countEntityByName(const char *szName);
	CBaseEntity* findEntityByName(const char *szName, CBaseEntity *pStart = 0);

	CBaseEntity* findEntityByClass(const char *szName, CBaseEntity *pStart = 0);

	CBaseEntity* findEntityInSphere(const float3 &f3Origin, float fRadius, CBaseEntity *pStart = 0);

	bool exportList(const char *szFile);
	bool import(const char *szFile, bool shouldSendProgress = false);

	void dumpList(int argc, const char **argv);
	void entKV(int argc, const char **argv);

	int getCount();
	CBaseEntity* getById(ID id);

	CBaseEntity* cloneEntity(CBaseEntity *pEnt);

	void sheduleDestroy(CBaseEntity *pEnt);

	void setEditorMode(bool isEditor = true);
	bool isEditorMode();

protected:
	ID reg(CBaseEntity *pEnt);
	void unreg(ID ent);

	void regSync(CBaseEntity *pEnt);
	void unregSync(CBaseEntity *pEnt);

	Array<CBaseEntity*, 64> m_vEntList;
	Array<CBaseEntity*, 64> m_vEntSyncList;
	Array<CBaseEntity*> m_vEntRemoveList;
	Array<ID> m_vFreeIDs;

	Array<timeout_t> m_vTimeout;
	Array<timeout_t> m_vInterval;
	Array<timeout_output_t> m_vOutputTimeout;
	Array<ID> m_vFreeInterval;
	Array<ID> m_vFreeTimeout;

	int m_iThreadNum;

	//! @warning это нужно хранить в течение работы проги, т.к. таблицы дефолтов ссылаются напрямую на этот объект
	ISXConfig *m_pDefaultsConf;
	ISXConfig *m_pDynClassConf;

	bool m_isEditorMode = false;

	std::mutex m_mxTimeout;
	std::mutex m_mxInterval;
};

#endif
