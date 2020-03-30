#ifndef __ASYNCTASKRUNNER_H
#define __ASYNCTASKRUNNER_H

#include <xcommon/IXCore.h>
#include "Task.h"
#include <xcommon/XEvents.h>

class CAsyncTask;
class CAsyncTaskRunner: public IAsyncTaskRunner
{
public:
	CAsyncTaskRunner(IXCore *pCore);
	~CAsyncTaskRunner();

	void XMETHODCALLTYPE runTask(IAsyncTask *pTask) override;
	void XMETHODCALLTYPE runCallbacks() override;

	ID XMETHODCALLTYPE forLoop(int iStart, int iEnd, const IParallelForBody *pBody, int iMaxChunkSize = 0) override;
	void XMETHODCALLTYPE waitForLoop(ID id) override;

protected:
	IXCore *m_pCore;

	Array<ITask*> m_vpQueue;

	IEventChannel<XEventAsyncTask> *m_pEventChannel = NULL;

	bool m_isTaskRunning = false;
};

#endif
