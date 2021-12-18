#ifndef __EDITOR_MODEL_H
#define __EDITOR_MODEL_H

#include <xcommon/editor/IXEditable.h>
//#include <mtrl/IXMaterialSystem.h>
//#include <xcommon/IXCore.h>
//#include "Outline.h"
//#include "BrushMesh.h"

class CEditable;
class CEditorObject;
class IXCore;

class CEditorModel final: public IXUnknownImplementation<IXEditorModel>
{
public:
	CEditorModel(CEditable *pEditable, IXCore *pCore);
	CEditorModel(CEditable *pEditable, IXCore *pCore, const XGUID &guid);
	~CEditorModel();

	const XGUID* XMETHODCALLTYPE getGUID() override;

	void XMETHODCALLTYPE getResource(IXResourceModel **ppOut) override;

	UINT XMETHODCALLTYPE getObjectCount() override;
	bool XMETHODCALLTYPE getObject(UINT id, IXEditorObject **ppObj) override;

	void XMETHODCALLTYPE addObject(IXEditorObject *pObj) override;
	void XMETHODCALLTYPE removeObject(IXEditorObject *pObj) override;

	void XMETHODCALLTYPE setCallback(IXEditorModelCallback *pCallback) override;

	void XMETHODCALLTYPE destroy() override;
	void XMETHODCALLTYPE restore() override;

	const char* XMETHODCALLTYPE getTypeName() override;

	void onObjectChanged(CEditorObject *pObj);
	void onObjectAdded(CEditorObject *pObj);
	void onObjectRemoved(CEditorObject *pObj);

private:
	CEditable *m_pEditable;
	IXCore *m_pCore;

	IXEditorModelCallback *m_pCallback = NULL;

	XGUID m_guid;

	Array<CEditorObject*> m_aObjects;
};

#endif
