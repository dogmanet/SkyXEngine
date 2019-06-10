
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TASK_H
#define __TASK_H

#include "ITask.h"

// Ѕазовый класс задачи
class CTask: public ITaskImpl<ITask>
{
public:
	CTask(THREAD_UPDATE_FUNCTION fnFunc, UINT iFlags = CORE_TASK_FLAG_MAINTHREAD_REPEATING);

	void run();

private:
	THREAD_UPDATE_FUNCTION m_fnUpdateFunc;
};


#endif
