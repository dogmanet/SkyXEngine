#include "AsyncTaskRunner.h"
#include <atomic>

//@FIXME: refactor that
#include "TaskManager.h"
extern CTaskManager *g_pTaskManager;

class CAsyncTask: public ITaskImpl<ITask>
{
public:
	CAsyncTask(IAsyncTask *pUserTask):
		ITaskImpl<ITask>(CORE_TASK_FLAG_BACKGROUND),
		m_pUserTask(pUserTask)
	{
		assert(m_pUserTask);

		m_pUserTask->AddRef();
		m_isDone.store(0);
		m_isRunning.store(0);
	}
	~CAsyncTask()
	{
		mem_release(m_pUserTask);
	}
	void run()
	{
		setRunning();

		m_pUserTask->exec();

		setDone();
	}

	bool isDone()
	{
		return(m_isDone.load() != 0);
	}

	void setDone()
	{
		m_isDone.store(1);
	}

	bool isRunning()
	{
		return(m_isRunning.load() != 0);
	}

	void setRunning()
	{
		m_isRunning.store(1);
	}

//protected:
	IAsyncTask *m_pUserTask;

	std::atomic_int m_isDone;
	std::atomic_int m_isRunning;

	bool m_isRunningOld = false;
	int m_iOldProgress = 101;
};

//##########################################################################

CAsyncTaskRunner::CAsyncTaskRunner(IXCore *pCore):
	m_pCore(pCore)
{
	m_pEventChannel = pCore->getEventChannel<XEventAsyncTask>(EVENT_ASYNC_TASK_GUID);
}
CAsyncTaskRunner::~CAsyncTaskRunner()
{
}

void XMETHODCALLTYPE CAsyncTaskRunner::runTask(IAsyncTask *pUserTask)
{
	CAsyncTask *pTask = new CAsyncTask(pUserTask);
	m_vpQueue.push_back(pTask);
	pTask->AddRef();

	XEventAsyncTask ev;
	ev.type = XEventAsyncTask::TYPE_ADDED;
	ev.szTaskName = pTask->m_pUserTask->getName();
	ev.pTaskId = pTask->m_pUserTask;
	m_pEventChannel->broadcastEvent(&ev);

	if(!m_isTaskRunning)
	{
		m_isTaskRunning = true;
		g_pTaskManager->addTask(pTask);
	}
}

void XMETHODCALLTYPE CAsyncTaskRunner::runCallbacks()
{
	CAsyncTask *pTask;
	for(UINT i = 0, l = m_vpQueue.size(); i < l; ++i)
	{
		pTask = (CAsyncTask*)m_vpQueue[i];
		if(pTask->isDone())
		{
			m_isTaskRunning = false;
			pTask->m_pUserTask->onFinished();

			XEventAsyncTask ev;
			ev.type = XEventAsyncTask::TYPE_FINISHED;
			ev.szTaskName = pTask->m_pUserTask->getName();
			ev.iProgress = pTask->m_pUserTask->getProgress();
			ev.pTaskId = pTask->m_pUserTask;
			m_pEventChannel->broadcastEvent(&ev);

			mem_release(pTask);

			m_vpQueue.erase(i);
			--i; --l;
		}
		else
		{
			if(pTask->isRunning())
			{
				int iCurrentProgress = pTask->m_pUserTask->getProgress();

				XEventAsyncTask ev;
				ev.szTaskName = pTask->m_pUserTask->getName();
				ev.iProgress = iCurrentProgress;
				ev.pTaskId = pTask->m_pUserTask;

				if(!pTask->m_isRunningOld)
				{
					pTask->m_isRunningOld = true;

					ev.type = XEventAsyncTask::TYPE_STARTED;
					m_pEventChannel->broadcastEvent(&ev);

					ev.type = XEventAsyncTask::TYPE_PROGRESS;
					m_pEventChannel->broadcastEvent(&ev);
					pTask->m_iOldProgress = iCurrentProgress;
				}
				else if(pTask->m_iOldProgress != iCurrentProgress)
				{
					pTask->m_iOldProgress = iCurrentProgress;

					ev.type = XEventAsyncTask::TYPE_PROGRESS;
					m_pEventChannel->broadcastEvent(&ev);
				}

			}
		}
	}

	if(!m_isTaskRunning && m_vpQueue.size())
	{
		m_isTaskRunning = true;
		g_pTaskManager->addTask(m_vpQueue[0]);
	}
}

ID XMETHODCALLTYPE CAsyncTaskRunner::forLoop(int iStart, int iEnd, const IParallelForBody *pBody, int iMaxChunkSize)
{
	return(g_pTaskManager->forLoop(iStart, iEnd, pBody, iMaxChunkSize));
}
void XMETHODCALLTYPE CAsyncTaskRunner::waitForLoop(ID id)
{
	g_pTaskManager->waitFor(id);
}
