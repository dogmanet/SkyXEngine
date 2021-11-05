#ifndef __COMMAND_CREATE_H
#define __COMMAND_CREATE_H

#include <xcommon/editor/IXEditorExtension.h>

//#include <common/assotiativearray.h>
//#include <common/string.h>
#include <xcommon/editor/IXEditable.h>
#include "EditorObject.h"

class CCommandCreate final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	CCommandCreate(IXEditor *pEditor, CEditorObject *pObject);
	~CCommandCreate();

	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override
	{
		return("create brush");
	}

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(false);
	}

protected:
	IXEditor *m_pEditor = NULL;
	CEditorObject *m_pObject = NULL;
};

#endif
