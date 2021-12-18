#ifndef __IXMODELWRITER_H
#define __IXMODELWRITER_H

#include <gdefines.h>
#include <core/IFile.h>
#include <xcommon/resource/IXResourceModel.h>


// {4E73A60B-4912-46BA-927E-42658304C5B8}
#define IXMODELWRITER_GUID DEFINE_XGUID(0x4e73a60b, 0x4912, 0x46ba, 0x92, 0x7e, 0x42, 0x65, 0x83, 0x4, 0xc5, 0xb8)
#define IXMODELWRITER_VERSION 1

class IXModelWriter: public IXUnknown
{
public:
	virtual UINT XMETHODCALLTYPE getExtCount() const = 0;
	virtual const char* XMETHODCALLTYPE getExt(UINT uIndex) const = 0;
	virtual const char* XMETHODCALLTYPE getExtText(UINT uIndex) const = 0;
	virtual const char* XMETHODCALLTYPE getAuthor() const = 0;
	virtual const char* XMETHODCALLTYPE getCopyright() const = 0;
	virtual const char* XMETHODCALLTYPE getDescription() const = 0;

	virtual bool XMETHODCALLTYPE writeModel(IXResourceModel *pResource, IFile *pFile) = 0;
};

#endif
