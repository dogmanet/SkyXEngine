#ifndef __IASYNCFILEREADER_H
#define __IASYNCFILEREADER_H

#include <gdefines.h>

class IFile;
class IAsyncFileReaderCallback: public IXUnknown
{
public:
	virtual void onLoaded(IFile *pFile) = 0;
	virtual void onError() = 0;
};

class IAsyncFileReader: public IXUnknown
{
public:
	virtual void loadFile(const char *szFile, IAsyncFileReaderCallback *pCallback) = 0;
	virtual void runCallbacks() = 0;
};

#endif
