
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

#include <memory>
#include <thread>
#include <algorithm>
#include <atomic>
#include <chrono>
#include "task.h"
#include "common/ConcurrentQueue.h"
#include <common/array.h>

#if defined(_WINDOWS)
// Это только для того, чтобы задать имя потока в отладччике
const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	const char *szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
#endif

// Внимание, при использовании должна быть создана хотя бы одна фоновая задача!
class CTaskManager
{
public:
	typedef ITask *TaskPtr;
	typedef CConcurrentQueue<TaskPtr> TaskList;

	CTaskManager(unsigned int numThreads = 0); //< Количество рабочих потоков, 0 для автоопределения
	~CTaskManager();

	void addTask(TaskPtr task); //< Добавляет задачу в планировщик
	void addTask(TaskPtr task, std::chrono::steady_clock::time_point tpWhen); //< Добавляет задачу в планировщик
	void addTaskIO(TaskPtr task); //< Добавляет задачу ввода/вывода
	void add(THREAD_UPDATE_FUNCTION fnFunc, DWORD dwFlag = CORE_TASK_FLAG_MAINTHREAD_REPEATING); //< Добавляет задачу в планировщик

	void forceSinglethreaded();

	void start(); //< Запускает выполнение планировщика
	void stop(); //< Останавливает все

	ID forLoop(int iStart, int iEnd, const IParallelForBody *pBody, int iMaxChunkSize = 0);
	void waitFor(ID id);

	int getThreadCount()
	{
		return(m_iNumThreads);
	}

	
private:
	void workerMain();
	void workerIOMain();
	void schedulerMain();
	void worker(bool bOneRun);
	void workerIO();
	void execute(TaskPtr task);
	void synchronize();
	void sheduleNextBunch();
	void notifyWorkers(UINT uCount = 1);

	Array<std::thread*> m_aThreads;
	std::thread* m_pIOThread;
	std::thread* m_pShedulerThread;
	unsigned int m_iNumThreads;

	volatile bool m_isRunning = false;

	TaskList m_TaskList[2]; //!< В главном потоке (синхронно)
	TaskList m_BackgroundTasks; //!< Фоновые задачи
	TaskList m_SyncTasks; //!< Синхронные задачи
	TaskList m_OnSyncTasks; //!< Задачи синхронизации
	TaskList m_aIOTasks; //!< Задачи ввода/вывода

	unsigned int m_iReadList;
	unsigned int m_iWriteList;

	typedef std::condition_variable Condition;
	typedef std::lock_guard<mutex> ScopedLock;

	mutable mutex m_mutexSync;
	mutable mutex m_mutexFor;
	mutable mutex m_mutexIOThread;
	mutable mutex m_mutexSchedulerThread;
	Condition m_Condition;
	Condition m_ConditionIOThread;
	Condition m_ConditionSchedulerThread;
	mutable SpinLock m_mutexWorker;
	Condition m_ConditionWorker;
	Condition m_ConditionFor;
	int m_iNumTasksToWaitFor;

	bool m_isSingleThreaded = false;

	Array<int> m_aiNumWaitFor;

	struct ScheduledTask
	{
		TaskPtr pTask;
		std::chrono::steady_clock::time_point tpRunAt;
	};
	Array<ScheduledTask> m_aScheduled;
};

#endif
