
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __ITASK_H
#define __ITASK_H

#include "sxcore.h"
#include <memory>
#define _NTASKMANAGER

class ITask: public IXUnknown
{
public:
	virtual ~ITask() = default;

	virtual void run() = 0;

	virtual void stopRepeating() = 0;

	virtual UINT getFlags() = 0;
};

template<class T>
class ITaskImpl: public IXUnknownImplementation<T>
{
public:
	ITaskImpl<T>(UINT uFlags = CORE_TASK_FLAG_MAINTHREAD_REPEATING):
		m_uFlags(uFlags)
	{
	}

	void stopRepeating() override
	{
		m_uFlags &= ~CORE_TASK_FLAG_REPEATING;
	}

	UINT getFlags() override
	{
		return(m_uFlags);
	}

private:
	UINT m_uFlags;
};

#endif
