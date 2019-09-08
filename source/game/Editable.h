#ifndef __EDITABLE_H
#define __EDITABLE_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>
#include "GameData.h"

class CEditable: public IXEditable
{
	friend class CEditorObject;

public:
	CEditable(IXCore *pCore);
	~CEditable();

	UINT getObjectCount() override;
	IXEditorObject* getObject(UINT id) override;
	IXEditorObject* newObject(const char *szClassName) override;

	const char* getName() override
	{
		return("Entity");
	}
	UINT getClassCount() override
	{
		return(CEntityFactoryMap::GetInstance()->getListCount());
	}
	const char* getClass(UINT id) override
	{
		assert(id < getClassCount());
		return(m_pszClassList[id]);
	}

	void startup(IGXDevice *pDevice) override;
	void shutdown() override;

protected:
	IGXDevice *m_pDevice = NULL;
	IXCore *m_pCore = NULL;
	IXMaterialSystem *m_pMaterialSystem = NULL;

	IGXBlendState *m_pBlendColorFactor = NULL;
	IGXRasterizerState *m_pRSWireframe = NULL;
	IXTexture *m_pWhiteTexture = NULL;

	const char **m_pszClassList = NULL;
};

#endif
