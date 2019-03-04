#ifndef __EDITABLE_H
#define __EDITABLE_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>


class CEditable: public IXEditable
{
	friend class CEditorObject;

public:
	CEditable(IXCore *pCore);
	~CEditable();

	UINT getObjectCount();
	IXEditorObject *getObject(UINT id);
	IXEditorObject *newObject();

	void startup(IGXContext *pDevice);
	void shutdown();

protected:
	IGXContext *m_pDevice = NULL;
	IXCore *m_pCore = NULL;
	IXMaterialSystem *m_pMaterialSystem = NULL;

	IGXBlendState *m_pBlendColorFactor = NULL;
	IGXRasterizerState *m_pRSWireframe = NULL;
	IXTexture *m_pWhiteTexture = NULL;
};

#endif
