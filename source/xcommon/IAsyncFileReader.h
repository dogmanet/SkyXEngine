#ifndef __IASYNCFILEREADER_H
#define __IASYNCFILEREADER_H

#include <gdefines.h>

// {CE5CEFB0-9757-45BC-92BC-D040612916F2}
#define IASYNCFILELOADER_GUID DEFINE_XGUID(0xce5cefb0, 0x9757, 0x45bc, 0x92, 0xbc, 0xd0, 0x40, 0x61, 0x29, 0x16, 0xf2)

class IFile;
class IAsyncFileLoaderCallback: public IXUnknown
{
public:
	virtual void onLoaded(IFile *pFile) = 0;
	virtual void onError() = 0;
};

class IAsyncFileLoader: public IXUnknown
{
public:
	virtual void loadFile(const char *szFile, IAsyncFileLoaderCallback *pCallback) = 0;
};

#endif
