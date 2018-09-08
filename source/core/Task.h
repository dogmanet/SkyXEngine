
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TASK_H
#define __TASK_H

#include "sxcore.h"
#include <memory>
#define _NTASKMANAGER

class ITask
{
public:
	typedef std::shared_ptr<ITask> TaskPtr;

	ITask(UINT iFlags = CORE_TASK_FLAG_MAINTHREAD_REPEATING);
	virtual ~ITask(){}

	virtual void run() = 0;

	void stopRepeating()
	{
		m_iFlags &= ~CORE_TASK_FLAG_REPEATING;
	}

	UINT getFlags() const;

private:
	UINT m_iFlags;
};

// Ѕазовый класс задачи
class CTask: public ITask
{
public:

	CTask(THREAD_UPDATE_FUNCTION fnFunc, UINT iFlags = CORE_TASK_FLAG_MAINTHREAD_REPEATING);

	void run();

private:
	THREAD_UPDATE_FUNCTION m_fnUpdateFunc;
};


#endif
