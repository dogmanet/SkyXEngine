
#include "task.h"

CTask::CTask(THREAD_UPDATE_FUNCTION func,unsigned int flags) :m_iTaskFlags(flags)
{
	m_fnUpdateFunc = func;
}

CTask::~CTask()
{
}

unsigned int CTask::getTaskFlags() const
{
	return(m_iTaskFlags);
}

CTask::CTaskBeginning::CTaskBeginning(TaskPtr t):m_Task(t)
{
}

CTask::CTaskCompleted::CTaskCompleted(TaskPtr t):m_Task(t)
{
}