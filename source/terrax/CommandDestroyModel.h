#ifndef _COMMAND_DESTROY_MODEL_H_
#define _COMMAND_DESTROY_MODEL_H_

#include <xcommon/editor/IXEditorExtension.h>
#include "terrax.h"

#include <common/assotiativearray.h>
#include <common/string.h>
#include <xcommon/editor/IXEditable.h>

#include "CommandDelete.h"
#include "ProxyObject.h"

class CCommandDestroyModel final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	CCommandDestroyModel(CProxyObject *pObj);
	~CCommandDestroyModel();

	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override
	{
		return("to world");
	}

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(false);
	}

private:
	Array<IXEditorModel*> m_aModels;

	CProxyObject *m_pProxy = NULL;

	IXEditorObject *m_pEntity = NULL;
	
	struct ObjModel
	{
		IXEditorObject *pObj;
		IXEditorModel *pModel;
	};
	Array<ObjModel> aObjModels;

	CCommandDelete *m_pCommandDelete = NULL;
};

#endif
