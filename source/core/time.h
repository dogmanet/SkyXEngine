
#ifndef __time_h
#define __time_h

#include <gdefines.h>
extern report_func reportf;

#include <common/Array.h>

#include <chrono>
typedef std::chrono::system_clock::time_point time_point;

#define TIMEMANAGER_PRECOND_ID(id,stdval) \
if (!(id >= 0 && id < ArrTimes.size()))\
{reportf(REPORT_MSG_LEVEL_ERROR, "%s - time manager: unresolved index of access '%d'", gen_msg_location, id); return stdval; }

class TimeManager
{
public:

	TimeManager();
	~TimeManager();

	struct Time
	{
		Time();
		~Time();

		time_point tp;
		int64_t time_total;		//общее прошедшее время с момента запуска
		long double delta_fraction;
		int64_t unixtime_curr;	//текущее время в unix
		int64_t unixtime_start;
		float speed;
		bool working;
	};

	void Update();

	ID TimeAdd();

	void TimeSpeedSet(ID id, float speed);
	float TimeSpeedGet(ID id);

	void TimeWorkingSet(ID id, bool working);
	bool TimeWorkingGet(ID id);

	void TimeUnixStartSet(ID id, int64_t start_time);
	int64_t TimeUnixStartGet(ID id);
	int64_t TimeUnixCurrGet(ID id);

	int64_t TimeTotalMcsGet(ID id);
	int64_t TimeTotalMcsGetU(ID id);

protected:
	Array<Time*> ArrTimes;
};

#endif