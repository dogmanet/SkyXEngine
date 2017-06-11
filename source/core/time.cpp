
#include <core\\time.h>

TimeManager::TimeManager()
{

}

TimeManager::~TimeManager()
{
	for (int i = 0; i < ArrTimes.size(); ++i)
	{
		mem_delete(ArrTimes[i]);
	}

	ArrTimes.clear();
}

TimeManager::Time::Time()
{
	time_total = 0;
	speed = 1.f;
	working = false;
	unixtime_start = time(NULL);
	unixtime_curr = unixtime_start;
	tp = std::chrono::high_resolution_clock::now();
	delta_fraction = 0;
}

TimeManager::Time::~Time()
{
	
}



void TimeManager::Update()
{
	time_point tcurr = std::chrono::high_resolution_clock::now();
	
	Time* tobj = 0;
	for (int i = 0; i < ArrTimes.size(); ++i)
	{
		tobj = ArrTimes[i];
		if (tobj->working)
		{
			tobj->delta_fraction += (long double)std::chrono::duration_cast<std::chrono::microseconds>(tcurr - tobj->tp).count() * tobj->speed;
			tobj->time_total += (int64_t)tobj->delta_fraction;
			tobj->delta_fraction -= (int64_t)tobj->delta_fraction;
			tobj->unixtime_curr = tobj->unixtime_start + (tobj->time_total / 1000000);
		}

		tobj->tp = tcurr;
	}
}

ID TimeManager::TimeAdd()
{
	Time* tobj = new Time();
	ArrTimes.push_back(tobj);
	return ArrTimes.size()-1;
}

void TimeManager::TimeSpeedSet(ID id, float speed)
{
	TIMEMANAGER_PRECOND_ID(id, _VOID);
	ArrTimes[id]->speed = speed;
}

float TimeManager::TimeSpeedGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);
	return ArrTimes[id]->speed;
}

void TimeManager::TimeWorkingSet(ID id, bool working)
{
	TIMEMANAGER_PRECOND_ID(id, _VOID);

	if (working && !(ArrTimes[id]->working))
		ArrTimes[id]->tp = std::chrono::high_resolution_clock::now();

	ArrTimes[id]->working = working;
}

bool TimeManager::TimeWorkingGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, false);

	return ArrTimes[id]->working;
}

void TimeManager::TimeUnixStartSet(ID id, int64_t start_time)
{
	TIMEMANAGER_PRECOND_ID(id, _VOID);

	ArrTimes[id]->unixtime_start = start_time;
}

int64_t TimeManager::TimeUnixStartGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);

	return ArrTimes[id]->unixtime_start;
}

int64_t TimeManager::TimeUnixCurrGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);

	return ArrTimes[id]->unixtime_curr;
}

int64_t TimeManager::TimeTotalMcsGet(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);

	return ArrTimes[id]->time_total;
}

int64_t TimeManager::TimeTotalMcsGetU(ID id)
{
	TIMEMANAGER_PRECOND_ID(id, 0);

	Time* tobj = ArrTimes[id];

	if (!tobj->working)
		return tobj->time_total;

	time_point tcurr = std::chrono::high_resolution_clock::now();

	return tobj->time_total + (int64_t)((long double)std::chrono::duration_cast<std::chrono::microseconds>(tcurr - tobj->tp).count() * tobj->speed);
}