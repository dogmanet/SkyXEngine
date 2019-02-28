#ifndef __IASYNCTASKRUNNER_H
#define __IASYNCTASKRUNNER_H

#include <gdefines.h>

class IAsyncTask: public IXUnknown
{
public:
	virtual void exec() = 0;
	virtual void abort() = 0;
	virtual void onFinished() = 0;

	virtual const char *getName()
	{
		return("task");
	}

	virtual int getProgress()
	{
		return(-1);
	}
};

class IAsyncTaskRunner: public IXUnknown
{
public:
	virtual void runTask(IAsyncTask *pTask) = 0;
	virtual void runCallbacks() = 0;
};

#endif
