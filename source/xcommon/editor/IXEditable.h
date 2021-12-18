#ifndef __XEDITABLE_H
#define __XEDITABLE_H

#include <gdefines.h>
//#include <common/math.h>
#include <xcommon/resource/IXResourceModel.h>

#include "IXEditorObject.h"
#include "IXEditorExtension.h"

class IXEditorModelCallback;

class IXEditorModel: public IXUnknown
{
public:
	virtual const XGUID* XMETHODCALLTYPE getGUID() = 0;

	virtual void XMETHODCALLTYPE getResource(IXResourceModel **ppOut) = 0;
	
	virtual UINT XMETHODCALLTYPE getObjectCount() = 0;
	virtual bool XMETHODCALLTYPE getObject(UINT id, IXEditorObject **ppObj) = 0;

	virtual void XMETHODCALLTYPE addObject(IXEditorObject *pObj) = 0;
	virtual void XMETHODCALLTYPE removeObject(IXEditorObject *pObj) = 0;

	virtual void XMETHODCALLTYPE setCallback(IXEditorModelCallback *pCallback) = 0;

	virtual void XMETHODCALLTYPE destroy() = 0;
	virtual void XMETHODCALLTYPE restore() = 0;

	virtual const char* XMETHODCALLTYPE getTypeName() = 0;
};

class IXEditorModelCallback
{
public:
	virtual void XMETHODCALLTYPE onChanged(IXEditorModel *pModel, IXEditorObject *pObject) = 0;
	virtual void XMETHODCALLTYPE onObjectAdded(IXEditorModel *pModel, IXEditorObject *pObject) = 0;
	virtual void XMETHODCALLTYPE onObjectRemoved(IXEditorModel *pModel, IXEditorObject *pObject) = 0;
	//virtual void XMETHODCALLTYPE onDestroyed(IXEditorModel *pModel) = 0;
};


//#############################################################################

// {02AF1C06-01F7-447F-B197-FF23E9F010C6}
#define IXEDITABLE_GUID DEFINE_XGUID(0x2af1c06, 0x1f7, 0x447f, 0xb1, 0x97, 0xff, 0x23, 0xe9, 0xf0, 0x10, 0xc6)
#define IXEDITABLE_VERSION 3

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

	virtual bool XMETHODCALLTYPE canProduceModel() = 0;
	virtual bool XMETHODCALLTYPE buildModelFromSelection(IXEditorModel **ppOut) = 0;
	virtual bool XMETHODCALLTYPE newModel(IXEditorModel **ppOut) = 0;
	virtual UINT XMETHODCALLTYPE getModelCount() = 0;
	virtual bool XMETHODCALLTYPE getModel(UINT id, IXEditorModel **ppOut) = 0;

	virtual bool XMETHODCALLTYPE canUseModel(const char *szClass) = 0;

};


#endif
