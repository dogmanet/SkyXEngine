#ifndef __EDITABLE_H
#define __EDITABLE_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>
#include "EditorExtension.h"

class CEditable final: public IXUnknownImplementation<IXEditable>
{
	friend class CEditorObject;

public:
	CEditable(IXCore *pCore);
	~CEditable();

	XIMPLEMENT_VERSION(IXEDITABLE_VERSION);

	UINT XMETHODCALLTYPE getObjectCount() override;
	IXEditorObject* XMETHODCALLTYPE getObject(UINT id) override;
	IXEditorObject* XMETHODCALLTYPE newObject(const char *szClassName) override;

	const char* XMETHODCALLTYPE getName() override
	{
		return("Solid geometry");
	}
	UINT XMETHODCALLTYPE getClassCount() override
	{
		return(0);
	}
	const char* XMETHODCALLTYPE getClass(UINT id) override
	{
		assert(id < getClassCount());
		return(NULL);
	}

	void XMETHODCALLTYPE startup(IGXDevice *pDevice) override;
	void XMETHODCALLTYPE shutdown() override;
	
	IXEditorExtension* XMETHODCALLTYPE getEditorExtension() override
	{
		return(m_pEditorExtension);
	}

	//void resync();

	//void onSelectionChanged(CEditorObject *pObject);

	const Array<CEditorObject*>& getObjects() const
	{
		return(m_aObjects);
	}

	IXCore* getCore()
	{
		return(m_pCore);
	}

	void save(const char *szLevelName);
	void load(const char *szLevelName, ID idPlugin);
	void unload();

	void onObjectCreated(CEditorObject *pObj)
	{
		m_aObjects.push_back(pObj);
	}

protected:
	IGXDevice *m_pDevice = NULL;
	IXCore *m_pCore = NULL;
	//IXMaterialSystem *m_pMaterialSystem = NULL;

	//IGXBlendState *m_pBlendColorFactor = NULL;
	//IGXRasterizerState *m_pRSWireframe = NULL;
	//IXTexture *m_pWhiteTexture = NULL;

	Array<CEditorObject*> m_aObjects;

	void removeObject(CEditorObject *pObject);

	CEditorExtension *m_pEditorExtension = NULL;
};

#endif
