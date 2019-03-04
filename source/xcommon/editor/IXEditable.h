#ifndef XEDITABLE_H_
#define XEDITABLE_H_

#include <gdefines.h>
#include <common/math.h>

#include "IXEditorObject.h"

// {02AF1C06-01F7-447F-B197-FF23E9F010C6}
#define IXEDITABLE_GUID DEFINE_XGUID(0x2af1c06, 0x1f7, 0x447f, 0xb1, 0x97, 0xff, 0x23, 0xe9, 0xf0, 0x10, 0xc6)
#define IXEDITABLE_VERSION 1

class IGXContext;
class IXEditable: public IXUnknown
{
public:
	virtual UINT getObjectCount() = 0;
	virtual IXEditorObject *getObject(UINT id) = 0;
	virtual IXEditorObject *newObject() = 0;

	virtual UINT getVersion()
	{
		return(IXEDITABLE_VERSION);
	}

	virtual void startup(IGXContext *pDevice) = 0;
	virtual void shutdown() = 0;
};


#endif
