
#include <core\task.h>

SXTask::SXTask(THREAD_UPDATE_FUNCTION func,unsigned int flags) :mTaskFlags(flags)
{
	UpdateFunc = func;
}

SXTask::~SXTask()
{
}

unsigned int SXTask::getTaskFlags() const
{
	return(mTaskFlags);
}

SXTask::TaskBeginning::TaskBeginning(TaskPtr t):mTask(t)
{
}

SXTask::TaskCompleted::TaskCompleted(TaskPtr t):mTask(t)
{
}