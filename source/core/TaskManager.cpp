
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TaskManager.h"

#if defined(_WINDOWS)
static void SetThreadName(DWORD dwThreadID, const char *threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
#pragma warning(pop)
}
#endif

CTaskManager::CTaskManager(unsigned int numThreads):
	m_isSingleThreaded(false),
	m_isRunning(false)
{
	m_iNumThreads = numThreads;
	if(!numThreads)
	{
		m_iNumThreads = std::thread::hardware_concurrency();
		if(!m_iNumThreads)
		{
			m_iNumThreads = 1;
		}
	}

	m_iWriteList = 0;
	m_iReadList = 1;

	m_iNumTasksToWaitFor = 0;
}

CTaskManager::~CTaskManager()
{
	for(int i = 0, l = m_aThreads.size(); i < l; ++i)
	{
		m_aThreads[i]->join();
	}
}

void CTaskManager::forceSinglethreaded()
{
	if(m_isRunning)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Cannot switch to singlethreaded while running!\n");
		return;
	}
	m_isSingleThreaded = true;
	m_iNumThreads = 0;
}

void CTaskManager::addTask(TaskPtr task)
{
	unsigned int flags = task->getFlags();
	
	if(flags & CORE_TASK_FLAG_ON_SYNC)
	{
		m_OnSyncTasks.push(task);
	}
	else
	{
		if((flags & CORE_TASK_FLAG_THREADSAFE) && !m_isSingleThreaded)
		{
			if(flags & CORE_TASK_FLAG_FRAME_SYNC)
			{
				m_SyncTasks.push(task);
			}
			else
			{
				m_BackgroundTasks.push(task);
			}
		}
		else
		{
			m_TaskList[m_iWriteList].push(task);
		}
	}
}

void CTaskManager::add(THREAD_UPDATE_FUNCTION fnFunc, DWORD dwFlag)
{
	addTask(CTaskManager::TaskPtr(new CTask(fnFunc, dwFlag)));
}

void CTaskManager::start()
{
	m_isRunning = true;

	char name[64];

	//< Инициализируем пул рабочих потоков
	for(unsigned int i = 0; i < m_iNumThreads; ++i)
	{
		std::thread * t = new std::thread(std::bind(&CTaskManager::workerMain, this));
#if defined(_WINDOWS)
		sprintf(name, "Worker #%d", i);
		SetThreadName(GetThreadId(t->native_handle()), name);
#endif
		m_aThreads.push_back(t);
	}

	sheduleNextBunch();

	while(m_isRunning)
	{
		if(!m_TaskList[m_iReadList].empty())
		{
			TaskPtr t = m_TaskList[m_iReadList].pop();
			execute(t);
		}
		else
		{
			if(m_isSingleThreaded)
			{
				worker(true);
			}

			synchronize();
			std::swap(m_iReadList, m_iWriteList);

			if(m_isSingleThreaded)
			{
				worker(true);
			}

			sheduleNextBunch();
		}

		std::this_thread::yield();
	}
}

void CTaskManager::synchronize()
{
	std::unique_lock<std::mutex> lock(m_mutexSync);

	while(m_iNumTasksToWaitFor > 0)
	{
		m_Condition.wait(lock);
	}

	//m_iNumTasksToWaitFor = m_OnSyncTasks.size();

	while(!m_OnSyncTasks.empty())
	{
		m_BackgroundTasks.push(m_OnSyncTasks.pop());
		++m_iNumTasksToWaitFor;
	}
}

void CTaskManager::sheduleNextBunch()
{
	std::unique_lock<std::mutex> lock(m_mutexSync);

	while(m_iNumTasksToWaitFor > 0)
	{
		m_Condition.wait(lock);
	}

	//m_iNumTasksToWaitFor = m_SyncTasks.size();

	while(!m_SyncTasks.empty())
	{
		m_BackgroundTasks.push(m_SyncTasks.pop());
		++m_iNumTasksToWaitFor;
	}
}

void CTaskManager::stop()
{
	m_isRunning = false;
	for(int i = 0, l = m_aThreads.size(); i < l; ++i)
	{
		m_aThreads[i]->join();
		delete m_aThreads[i];
	}
	m_aThreads.clear();
}

void CTaskManager::execute(TaskPtr t)
{
	t->run();

	if(t->getFlags() & CORE_TASK_FLAG_REPEATING)
	{
		addTask(t);
	}
}

void CTaskManager::workerMain()
{
	worker(false);
}

void CTaskManager::worker(bool bOneRun)
{
	TaskPtr task;

	while(m_isRunning)
	{
		if(m_BackgroundTasks.tryPop(task))
		{
			execute(task);

			if(task->getFlags() & (CORE_TASK_FLAG_FRAME_SYNC | CORE_TASK_FLAG_ON_SYNC))
			{
				{
					std::lock_guard<std::mutex> lock(m_mutexSync);
					m_iNumTasksToWaitFor -= 1;
				}

				m_Condition.notify_one();
			}

			std::this_thread::yield();
		}
		else
		{
			if(bOneRun)
			{
				return;
			}
			// тут делать нечего, спим 1.667 мс (1/10 кадра при 60 FPS)
			std::this_thread::sleep_for(std::chrono::microseconds(166));
		}
	}
}
