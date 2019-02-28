
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
	const char * szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
#endif

// Внимание, при использовании должна быть создана хотя бы одна фоновая задача!
class CTaskManager
{
public:
	typedef std::shared_ptr<ITask> TaskPtr;
	typedef CConcurrentQueue<TaskPtr> TaskList;

	CTaskManager(unsigned int numThreads = 0); //< Количество рабочих потоков, 0 для автоопределения
	~CTaskManager();

	void addTask(TaskPtr task); //< Добавляет задачу в планировщик
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
	void worker(bool bOneRun);
	void workerIO();
	void execute(TaskPtr task);
	void synchronize();
	void sheduleNextBunch();
	//void notifyWorkers();

	Array<std::thread*> m_aThreads;
	std::thread* m_pIOThread;
	unsigned int m_iNumThreads;

	bool m_isRunning;

	TaskList m_TaskList[2]; //!< В главном потоке (сонхронно)
	TaskList m_BackgroundTasks; //!< Фоновые задачи
	TaskList m_SyncTasks; //!< Синхронные задачи
	TaskList m_OnSyncTasks; //!< Задачи синхронизации
	TaskList m_aIOTasks; //!< Задачи ввода/вывода

	unsigned int m_iReadList;
	unsigned int m_iWriteList;

	typedef std::condition_variable Condition;
	typedef std::lock_guard<std::mutex> ScopedLock;

	mutable std::mutex m_mutexSync;
	mutable std::mutex m_mutexFor;
	mutable std::mutex m_mutexIOThread;
	Condition m_Condition;
	Condition m_ConditionIOThread;
	//mutable std::mutex m_mutexWorker[8];
	//Condition m_ConditionWorker[8];
	Condition m_ConditionFor;
	int m_iNumTasksToWaitFor;

	bool m_isSingleThreaded;

	Array<int> m_aiNumWaitFor;
};

#endif
