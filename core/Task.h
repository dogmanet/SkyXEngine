#ifndef _SXTask_H_
#define _SXTask_H_

#include <memory>
#define _NTASKMANAGER

// Базовый класс задачи
class SXTask
{
public:
	typedef std::shared_ptr<SXTask> TaskPtr;

	struct TaskBeginning
	{
		TaskBeginning(TaskPtr t);
		TaskPtr mTask;
	};

	struct TaskCompleted
	{
		TaskCompleted(TaskPtr t);
		TaskPtr mTask;
	};

	SXTask(THREAD_UPDATE_FUNCTION func, unsigned int flags = CoreTF_SINGLETHREADED_REPEATING);
	~SXTask();

	void run() {
		if (UpdateFunc)
			(UpdateFunc)();
	}; //< Функция выполнения задачи, она и вызывается

	void stopRepeating()
	{
		mTaskFlags &= ~CoreTF_REPEATING;
	}

	unsigned int getTaskFlags() const;

private:
	unsigned int mTaskFlags;
	THREAD_UPDATE_FUNCTION UpdateFunc;
};


#endif
