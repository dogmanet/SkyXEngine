
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "task.h"

ITask::ITask(UINT flags):
	m_iFlags(flags)
{
}

UINT ITask::getFlags() const
{
	return(m_iFlags);
}

//##########################################################################

CTask::CTask(THREAD_UPDATE_FUNCTION func, UINT flags):
	ITask(flags),
	m_fnUpdateFunc(func)
{
}

void CTask::run()
{
	if(m_fnUpdateFunc)
	{
		m_fnUpdateFunc();
	}
}
