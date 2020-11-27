#ifndef __XEDITABLE_H
#define __XEDITABLE_H

#include <gdefines.h>
#include <common/math.h>

#include "IXEditorObject.h"
#include "IXEditorExtension.h"

// {02AF1C06-01F7-447F-B197-FF23E9F010C6}
#define IXEDITABLE_GUID DEFINE_XGUID(0x2af1c06, 0x1f7, 0x447f, 0xb1, 0x97, 0xff, 0x23, 0xe9, 0xf0, 0x10, 0xc6)
#define IXEDITABLE_VERSION 2

class IGXDevice;
class IXEditable: public IXUnknown
{
public:
	virtual UINT XMETHODCALLTYPE getObjectCount() = 0;
	virtual IXEditorObject* XMETHODCALLTYPE getObject(UINT id) = 0;
	virtual IXEditorObject* XMETHODCALLTYPE newObject(const char *szClassName) = 0;

	virtual const char* XMETHODCALLTYPE getName() = 0;
	virtual UINT XMETHODCALLTYPE getClassCount() = 0;
	virtual const char* XMETHODCALLTYPE getClass(UINT id) = 0;

	virtual void XMETHODCALLTYPE startup(IGXDevice *pDevice) = 0;
	virtual void XMETHODCALLTYPE shutdown() = 0;

	virtual IXEditorExtension* XMETHODCALLTYPE getEditorExtension() = 0;
};


#endif
