#ifndef __ASYNCFILEREADER_H
#define __ASYNCFILEREADER_H

#include <xcommon/IXCore.h>
#include "Task.h"

class CIOTask;
class CAsyncFileReader: public IXUnknownImplementation<IAsyncFileReader>
{
public:
	CAsyncFileReader(IXCore *pCore);
	~CAsyncFileReader();

	void loadFile(const char *szFile, IAsyncFileReaderCallback *pCallback);
	void runCallbacks();

protected:
	IXCore *m_pCore;
	IFileSystem *m_pFileSystem = NULL;

	Array<ITask*> m_vpQueue;
};

#endif
