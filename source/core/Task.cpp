
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "task.h"

CTask::CTask(THREAD_UPDATE_FUNCTION func, UINT flags):
	m_iFlags(flags)
{
	m_fnUpdateFunc = func;
}

CTask::~CTask()
{
}

UINT CTask::getFlags() const
{
	return(m_iFlags);
}
