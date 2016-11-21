#include <core\taskManager.h>

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

SXTaskManager::SXTaskManager(unsigned int numThreads)
{
	mNumThreads = numThreads;
	if(numThreads == 0)
	{
		mNumThreads = std::thread::hardware_concurrency();
		if(mNumThreads == 0)
		{
			mNumThreads = 1;
		}
	}

	mWriteList = 0;
	mReadList = 1;

	mNumTasksToWaitFor = 0;
}

SXTaskManager::~SXTaskManager()
{
	for(auto itr : mThreads)
		itr->join();
}

void SXTaskManager::addTask(TaskPtr task)
{
	unsigned flags = task->getTaskFlags();

	if (flags & CoreTF_THREADSAFE)
	{
		if (flags & CoreTF_FRAME_SYNC)
		if (flags & CoreTF_ON_SYNC)
				mOnSyncTasks.push(task);
			else
				mSyncTasks.push(task);
		else
			mBackgroundTasks.push(task);
	}
	else
		mTaskList[mWriteList].push(task);
}

void SXTaskManager::add(THREAD_UPDATE_FUNCTION func, DWORD flag)
{
	this->addTask(SXTaskManager::TaskPtr(new SXTask(func, flag)));
}

void SXTaskManager::start()
{
	mRunning = true;

	EventChannel chan;

	chan.add<SXTask::TaskCompleted>(*this);
	chan.add<StopEvent>(*this);

	char name[64];


	//< Инициализируем пул рабочих потоков
	for(unsigned int i = 0; i < mNumThreads; ++i)
	{
		std::thread * t = new std::thread(std::bind(&SXTaskManager::worker, this));
#if defined(_WINDOWS)
		sprintf(name, "Worker qq #%d", i);
		SetThreadName(GetThreadId(t->native_handle()), name);
#endif
		mThreads.push_back(t);
	}
	//mThreads.front()->

	while(mRunning)
	{
		if(!mTaskList[mReadList].empty())
		{
			TaskPtr t = mTaskList[mReadList].wait_pop();
			execute(t);
		}
		/*else if(!mBackgroundTasks.empty())
		{

		}*/
		else
		{
			synchronize();
			std::swap(mReadList, mWriteList);
		}

		std::this_thread::yield();
	}
}

void SXTaskManager::synchronize()
{
	std::unique_lock<std::mutex> lock(mSyncMutex);

	while(mNumTasksToWaitFor > 0)
		mCondition.wait(lock);

	mNumTasksToWaitFor = mOnSyncTasks.size();

	while(!mOnSyncTasks.empty())
		mBackgroundTasks.push(mOnSyncTasks.wait_pop());




	while(mNumTasksToWaitFor > 0)
		mCondition.wait(lock);

	mNumTasksToWaitFor = mSyncTasks.size();

	while(!mSyncTasks.empty())
		mBackgroundTasks.push(mSyncTasks.wait_pop());
}

void SXTaskManager::stop()
{
	mRunning = false;
	mThreads.clear();
}

void SXTaskManager::execute(TaskPtr t)
{
	EventChannel chan;

	chan.broadcast(SXTask::TaskBeginning(t));
	t->run();
	chan.broadcast(SXTask::TaskCompleted(t));
}

void SXTaskManager::handle(const SXTaskManager::StopEvent&)
{
	stop();
}

void SXTaskManager::handle(const SXTask::TaskCompleted& tc)
{
	if (tc.mTask->getTaskFlags() & CoreTF_REPEATING)
		addTask(tc.mTask);
}

void SXTaskManager::worker()
{
	TaskPtr task;

	while(mRunning)
	{
		bool exec = mBackgroundTasks.try_pop(task);

		if(exec)
		{
			execute(task);

			if (task->getTaskFlags() & CoreTF_FRAME_SYNC)
			{
				{
					std::lock_guard<std::mutex> lock(mSyncMutex);
					mNumTasksToWaitFor -= 1;
				}

				mCondition.notify_one();
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