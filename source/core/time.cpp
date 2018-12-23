
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "Time.h"

CTimeManager::CTimeManager()
{

}

CTimeManager::~CTimeManager()
{
	for (int i = 0; i < m_aTimes.size(); ++i)
	{
		mem_delete(m_aTimes[i]);
	}

	m_aTimes.clear();
}

CTimeManager::CTime::CTime()
{
	m_iTimeTotal = 0;
	m_fSpeed = 1.f;
	m_isWorking = false;
	m_iUnixtimeStart = time(NULL);
	m_iUnixtimeCurr = m_iUnixtimeStart;
	m_timePoint = std::chrono::high_resolution_clock::now();
	m_fDeltaFraction = 0;
}

CTimeManager::CTime::~CTime()
{
	
}



void CTimeManager::update()
{
	time_point tcurr = std::chrono::high_resolution_clock::now();
	
	CTime* tobj = 0;
	for (int i = 0; i < m_aTimes.size(); ++i)
	{
		tobj = m_aTimes[i];
		if (tobj->m_isWorking)
		{
			tobj->m_fDeltaFraction += (long double)std::chrono::duration_cast<std::chrono::microseconds>(tcurr - tobj->m_timePoint).count() * tobj->m_fSpeed;
			tobj->m_iTimeTotal += (int64_t)tobj->m_fDeltaFraction;
			tobj->m_fDeltaFraction -= (int64_t)tobj->m_fDeltaFraction;
			tobj->m_iUnixtimeCurr = tobj->m_iUnixtimeStart + (tobj->m_iTimeTotal / 1000000);
		}

		tobj->m_timePoint = tcurr;
	}
}

ID CTimeManager::timeAdd()
{
	CTime* tobj = new CTime();
	m_aTimes.push_back(tobj);
	return m_aTimes.size()-1;
}

void CTimeManager::timeSpeedSet(ID id, float fSpeed)
{
	TIMEMANAGER_PRECOND_ID(id, _VOID);
	m_aTimes[id]->m_fSpeed = fSpeed;
}

float CTimeManager::timeSpeedGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);
	return m_aTimes[id]->m_fSpeed;
}

void CTimeManager::timeWorkingSet(ID id, bool isWorking)
{
	TIMEMANAGER_PRECOND_ID(id, _VOID);

	if (isWorking && !(m_aTimes[id]->m_isWorking))
		m_aTimes[id]->m_timePoint = std::chrono::high_resolution_clock::now();

	m_aTimes[id]->m_isWorking = isWorking;
}

bool CTimeManager::timeWorkingGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, false);

	return m_aTimes[id]->m_isWorking;
}

void CTimeManager::timeUnixStartSet(ID id, int64_t iStartTime)
{
	TIMEMANAGER_PRECOND_ID(id, _VOID);

	m_aTimes[id]->m_iUnixtimeStart = iStartTime;
}

int64_t CTimeManager::timeUnixStartGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);

	return m_aTimes[id]->m_iUnixtimeStart;
}

int64_t CTimeManager::timeUnixCurrGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);

	return m_aTimes[id]->m_iUnixtimeCurr;
}

int64_t CTimeManager::timeTotalMcsGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);

	return m_aTimes[id]->m_iTimeTotal;
}

int64_t CTimeManager::timeTotalMcsGetU(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);

	CTime* tobj = m_aTimes[id];

	if (!tobj->m_isWorking)
		return tobj->m_iTimeTotal;

	time_point tcurr = std::chrono::high_resolution_clock::now();

	return tobj->m_iTimeTotal + (int64_t)((long double)std::chrono::duration_cast<std::chrono::microseconds>(tcurr - tobj->m_timePoint).count() * tobj->m_fSpeed);
}