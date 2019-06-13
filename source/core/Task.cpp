
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "task.h"

CTask::CTask(THREAD_UPDATE_FUNCTION func, UINT flags):
	ITaskImpl(flags),
	m_fnUpdateFunc(func)
{
	assert(m_fnUpdateFunc);
}

void CTask::run()
{
	m_fnUpdateFunc();
}
