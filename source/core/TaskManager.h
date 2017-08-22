#ifndef _SXTaskManager_H_
#define _SXTaskManager_H_

#include <memory>
#include <thread>
#include <algorithm>
#include <atomic>
#include "concurrent_queue.h"
#include "task.h"
#include "eventchannel.h"

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
	typedef std::shared_ptr<CTask> TaskPtr;
	typedef ConcurrentQueue<TaskPtr> TaskList;

	struct StopEvent
	{
	};

	CTaskManager(unsigned int numThreads = 0); //< Количество рабочих потоков, 0 для автоопределения
	~CTaskManager();

	void  addTask(TaskPtr task); //< Добавляет задачу в планировщик
	void  add(THREAD_UPDATE_FUNCTION fnFunc, DWORD dwFlag = CORE_TASK_FLAG_SINGLETHREADED_REPEATING); //< Добавляет задачу в планировщик

	void start(); //< Запускает выполнение планировщика
	void stop(); //< Останавливает все

	void handle(const StopEvent&);
	void handle(const CTask::CTaskCompleted &tc);

private:
	void worker();
	void execute(TaskPtr task);
	void synchronize();

	std::list<std::thread*> m_aThreads;
	unsigned int m_iNumThreads;

	bool m_isRunning;

	TaskList m_TaskList[2];
	TaskList m_BackgroundTasks;
	TaskList m_SyncTasks;
	TaskList m_OnSyncTasks;

	unsigned int m_iReadList;
	unsigned int m_iWriteList;

	typedef std::mutex Mutex;
	typedef std::condition_variable Condition;
	typedef std::lock_guard<Mutex> ScopedLock;

	mutable Mutex m_SyncMutex;
	Condition m_Condition;
	int m_iNumTasksToWaitFor;
};

#endif
