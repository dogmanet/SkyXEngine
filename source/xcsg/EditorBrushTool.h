#ifndef __EDITOR_BRUSH_TOOL_H
#define __EDITOR_BRUSH_TOOL_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>
#include "Outline.h"

class CEditable;
//class CEditorObject;

class CEditorBrushTool final: public IXUnknownImplementation<IXEditorTool>
{
public:
	CEditorBrushTool(CEditable *pEditable, IXEditor *pEditor);
	~CEditorBrushTool();

	void* XMETHODCALLTYPE getIcon() override;
	const char* XMETHODCALLTYPE getTitle() override;
	XAccelItem XMETHODCALLTYPE getAccel() override;

	void XMETHODCALLTYPE activate() override;
	void XMETHODCALLTYPE deactivate() override;

	bool XMETHODCALLTYPE onMouseDown(bool isPrimary) override;
	bool XMETHODCALLTYPE onMouseMove() override;
	void XMETHODCALLTYPE onMouseUp(bool isPrimary) override;

	bool XMETHODCALLTYPE onKeyDown(UINT uKey) override;
	void XMETHODCALLTYPE onKeyUp(UINT uKey) override;

	void render(bool is3D);

private:
	CEditable *m_pEditable;
	IXEditor *m_pEditor;

	HBITMAP m_hBitmap;

	IXGizmoRenderer *m_pRenderer = NULL;
	COutline *m_pNewOutline = NULL;

	bool m_isActive = false;

	bool m_isMouseDown = false;
};

#endif
