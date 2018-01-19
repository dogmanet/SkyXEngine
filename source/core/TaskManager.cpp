
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <core/taskManager.h>

#if defined(_WINDOWS)
void SetThreadName(DWORD dwThreadID, const char* threadName)
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

CTaskManager::CTaskManager(unsigned int numThreads)
{
	m_iNumThreads = numThreads;
	if (numThreads == 0)
	{
		m_iNumThreads = std::thread::hardware_concurrency();
		if(m_iNumThreads == 0)
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
	for(auto itr : m_aThreads)
		itr->join();
}

void CTaskManager::addTask(TaskPtr task)
{
	unsigned flags = task->getTaskFlags();

	if (flags & CORE_TASK_FLAG_THREADSAFE)
	{
		if (flags & CORE_TASK_FLAG_FRAME_SYNC)
		if (flags & CORE_TASK_FLAG_ON_SYNC)
				m_OnSyncTasks.push(task);
			else
				m_SyncTasks.push(task);
		else
			m_BackgroundTasks.push(task);
	}
	else
		m_TaskList[m_iWriteList].push(task);
}

void CTaskManager::add(THREAD_UPDATE_FUNCTION fnFunc, DWORD dwFlag)
{
	this->addTask(CTaskManager::TaskPtr(new CTask(fnFunc, dwFlag)));
}

void CTaskManager::start()
{
	m_isRunning = true;

	EventChannel chan;

	chan.add<CTask::CTaskCompleted>(*this);
	chan.add<StopEvent>(*this);

	char name[64];


	//< Инициализируем пул рабочих потоков
	for(unsigned int i = 0; i < m_iNumThreads; ++i)
	{
		std::thread * t = new std::thread(std::bind(&CTaskManager::worker, this));
#if defined(_WINDOWS)
		sprintf(name, "Worker qq #%d", i);
		SetThreadName(GetThreadId(t->native_handle()), name);
#endif
		m_aThreads.push_back(t);
	}
	//m_aThreads.front()->

	while(m_isRunning)
	{
		if(!m_TaskList[m_iReadList].empty())
		{
			TaskPtr t = m_TaskList[m_iReadList].wait_pop();
			execute(t);
		}
		/*else if(!m_BackgroundTasks.empty())
		{

		}*/
		else
		{
			synchronize();
			std::swap(m_iReadList, m_iWriteList);
		}

		std::this_thread::yield();
	}
}

void CTaskManager::synchronize()
{
	std::unique_lock<std::mutex> lock(m_SyncMutex);

	while(m_iNumTasksToWaitFor > 0)
		m_Condition.wait(lock);

	m_iNumTasksToWaitFor = m_OnSyncTasks.size();

	while(!m_OnSyncTasks.empty())
		m_BackgroundTasks.push(m_OnSyncTasks.wait_pop());




	while(m_iNumTasksToWaitFor > 0)
		m_Condition.wait(lock);

	m_iNumTasksToWaitFor = m_SyncTasks.size();

	while(!m_SyncTasks.empty())
		m_BackgroundTasks.push(m_SyncTasks.wait_pop());
}

void CTaskManager::stop()
{
	m_isRunning = false;
	m_aThreads.clear();
}

void CTaskManager::execute(TaskPtr t)
{
	EventChannel chan;

	chan.broadcast(CTask::CTaskBeginning(t));
	t->run();
	chan.broadcast(CTask::CTaskCompleted(t));
}

void CTaskManager::handle(const CTaskManager::StopEvent&)
{
	stop();
}

void CTaskManager::handle(const CTask::CTaskCompleted& tc)
{
	if (tc.m_Task->getTaskFlags() & CORE_TASK_FLAG_REPEATING)
		addTask(tc.m_Task);
}

void CTaskManager::worker()
{
	TaskPtr task;

	while(m_isRunning)
	{
		bool exec = m_BackgroundTasks.try_pop(task);

		if(exec)
		{
			execute(task);

			if (task->getTaskFlags() & CORE_TASK_FLAG_FRAME_SYNC)
			{
				{
					std::lock_guard<std::mutex> lock(m_SyncMutex);
					m_iNumTasksToWaitFor -= 1;
				}

				m_Condition.notify_one();
			}

			std::this_thread::yield();
		}
		else
		{
			// тут делать нечего, спим 1.667 мс (1/10 кадра при 60 FPS)
			std::this_thread::sleep_for(std::chrono::microseconds(166));
		}
	}
}