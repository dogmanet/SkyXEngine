#ifndef __COMMAND_CLIP_H
#define __COMMAND_CLIP_H

#include <xcommon/editor/IXEditorExtension.h>

//#include <common/assotiativearray.h>
//#include <common/string.h>
#include <xcommon/editor/IXEditable.h>
#include "EditorObject.h"

class CCommandClip final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	CCommandClip(IXEditor *pEditor, CEditable *pEditable);
	~CCommandClip();

	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override
	{
		return("clip objects");
	}

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(false);
	}

protected:
	IXEditor *m_pEditor = NULL;
	CEditable *m_pEditable = NULL;

	CLIP_PLANE_STATE m_clipPlaneState = CPS_NONE;
	SMPLANE m_clipPlane;

	//Array<CEditorObject*> m_aObjectsToClip;
	Array<CEditorObject*> m_aObjectsToRemove;
	//CEditorObject *m_pObject = NULL;

	struct clip_obj_t
	{
		CEditorObject *pSrcObj;
		CEditorObject *pDstObj;
		String sBefore;
	};

	Array<clip_obj_t> m_aClippedObjects;
};

#endif
