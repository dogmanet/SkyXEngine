#ifndef __EDITOR_EXTENSION_H
#define __EDITOR_EXTENSION_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>
#include "EditorBrushTool.h"
#include "FaceEditTool.h"

class CEditorExtension final: public IXUnknownImplementation<IXEditorExtension>
{
public:
	CEditorExtension(CEditable *pEditable, IXEditor *pEditor);
	~CEditorExtension();

	UINT XMETHODCALLTYPE getPropertyTabCount() override;
	IXEditorPropertyTab* XMETHODCALLTYPE getPropertyTab(UINT uId) override;

	UINT XMETHODCALLTYPE getToolCount() override;
	bool XMETHODCALLTYPE getTool(UINT uId, IXEditorTool **ppOut) override;

	void XMETHODCALLTYPE render(bool is3D) override;

	//void onSelectionChanged(CEditorObject *pObject);

private:
	CEditorBrushTool *m_pBrushTool = NULL;
	CFaceEditTool *m_pFaceEditTool = NULL;

	CEditable *m_pEditable = NULL;
};

#endif
