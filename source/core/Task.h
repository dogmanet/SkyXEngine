
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TASK_H
#define __TASK_H

#include "sxcore.h"
#include <memory>
#define _NTASKMANAGER

// Ѕазовый класс задачи
class CTask
{
public:
	typedef std::shared_ptr<CTask> TaskPtr;

	CTask(THREAD_UPDATE_FUNCTION fnFunc, UINT iFlags = CORE_TASK_FLAG_MAINTHREAD_REPEATING);
	~CTask();

	void run() 
	{
		if(m_fnUpdateFunc)
		{
			m_fnUpdateFunc();
		}
	}

	void stopRepeating()
	{
		m_iFlags &= ~CORE_TASK_FLAG_REPEATING;
	}

	UINT getFlags() const;

private:
	UINT m_iFlags;
	THREAD_UPDATE_FUNCTION m_fnUpdateFunc;
};


#endif
