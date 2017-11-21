#ifndef _CTask_H_
#define _CTask_H_

#include "sxcore.h"
#include <memory>
#define _NTASKMANAGER

// Ѕазовый класс задачи
class CTask
{
public:
	typedef std::shared_ptr<CTask> TaskPtr;

	struct CTaskBeginning
	{
		CTaskBeginning(TaskPtr t);
		TaskPtr m_Task;
	};

	struct CTaskCompleted
	{
		CTaskCompleted(TaskPtr t);
		TaskPtr m_Task;
	};

	CTask(THREAD_UPDATE_FUNCTION fnFunc, unsigned int iFlags = CORE_TASK_FLAG_SINGLETHREADED_REPEATING);
	~CTask();

	void run() {
		if (m_fnUpdateFunc)
			(m_fnUpdateFunc)();
	}; //< функция выполнения задачи, она и вызывается

	void stopRepeating()
	{
		m_iTaskFlags &= ~CORE_TASK_FLAG_REPEATING;
	}

	unsigned int getTaskFlags() const;

private:
	unsigned int m_iTaskFlags;
	THREAD_UPDATE_FUNCTION m_fnUpdateFunc;
};


#endif
