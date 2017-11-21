
#ifndef __TIME_H
#define __TIME_H

#include <gdefines.h>
extern report_func g_fnReportf;

#include <common/Array.h>

#include <chrono>
typedef std::chrono::system_clock::time_point time_point;

#define TIMEMANAGER_PRECOND_ID(id,stdval) \
if (!(id >= 0 && id < m_aTimes.size()))\
{g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - time manager: unresolved index of access '%d'", gen_msg_location, id); return stdval; }

class CTimeManager
{
public:

	CTimeManager();
	~CTimeManager();

	struct CTime
	{
		CTime();
		~CTime();

		time_point m_timePoint;
		int64_t m_iTimeTotal;		//общее прошедшее время с момента запуска
		long double m_fDeltaFraction;
		int64_t m_iUnixtimeCurr;	//текущее время в unix
		int64_t m_iUnixtimeStart;
		float m_fSpeed;
		bool m_isWorking;
	};

	void update();

	ID timeAdd();

	void timeSpeedSet(ID id, float fSpeed);
	float timeSpeedGet(ID id);

	void timeWorkingSet(ID id, bool isWorking);
	bool timeWorkingGet(ID id);

	void timeUnixStartSet(ID id, int64_t iStartTime);
	int64_t timeUnixStartGet(ID id);
	int64_t timeUnixCurrGet(ID id);

	int64_t timeTotalMcsGet(ID id);
	int64_t timeTotalMcsGetU(ID id);

protected:
	Array<CTime*> m_aTimes;
};

#endif