#include "AsyncFileReader.h"
#include "FileInMemory.h"
#include <atomic>

//@FIXME: refactor that
#include "TaskManager.h"
extern CTaskManager *g_pTaskManager;

class CIOTask: public ITaskImpl<ITask>
{
public:
	CIOTask(const char *szFile, IFileSystem *pFileSystem, IAsyncFileReaderCallback *pCallback):
		m_pFileSystem(pFileSystem),
		m_pCallback(pCallback)
	{
		m_szFileName = strdup(szFile);
		m_isDone.store(0);
	}
	~CIOTask()
	{
		free(m_szFileName);
		mem_release(m_pResult);
	}
	void run()
	{
		IFile *pFile = Core_CrFile();
		if(pFile->open(m_szFileName, CORE_FILE_BIN) == 0)
		{
			size_t uSize = pFile->getSize();
			byte *pData = new byte[uSize];
			size_t uRead = pFile->readBin(pData, uSize);
			assert(uRead == uSize);

			m_pResult = new CFileInMemory(pData, uSize, true);
		}
		mem_release(pFile);
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

//protected:
	IFile *m_pResult = NULL;
	char *m_szFileName = NULL;
	IFileSystem *m_pFileSystem;
	IAsyncFileReaderCallback *m_pCallback;

	std::atomic_int m_isDone;
};

//##########################################################################

CAsyncFileReader::CAsyncFileReader(IXCore *pCore):
	m_pCore(pCore)
{
	m_pCore->getFileSystem();
}
CAsyncFileReader::~CAsyncFileReader()
{
	mem_release(m_pFileSystem);
}

void CAsyncFileReader::loadFile(const char *szFile, IAsyncFileReaderCallback *pCallback)
{
	CIOTask *pTask = new CIOTask(szFile, m_pFileSystem, pCallback);
	m_vpQueue.push_back(pTask);
	g_pTaskManager->addTaskIO(pTask);
}

void CAsyncFileReader::runCallbacks()
{
	CIOTask *pTask;
	for(UINT i = 0, l = m_vpQueue.size(); i < l; ++i)
	{
		pTask = (CIOTask*)m_vpQueue[i];
		if(pTask->isDone())
		{
			if(pTask->m_pResult)
			{
				pTask->m_pCallback->onLoaded(pTask->m_pResult);
				mem_release(pTask->m_pResult);
			}
			else
			{
				pTask->m_pCallback->onError();
			}
			mem_release(pTask);

			m_vpQueue.erase(i);
			--i; --l;
		}
	}
}
