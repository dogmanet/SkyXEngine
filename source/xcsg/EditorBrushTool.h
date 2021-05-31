#ifndef __EDITOR_BRUSH_TOOL_H
#define __EDITOR_BRUSH_TOOL_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>

class CEditable;
//class CEditorObject;

class CEditorBrushTool final: public IXUnknownImplementation<IXEditorTool>
{
public:
	CEditorBrushTool(CEditable *pEditable);
	~CEditorBrushTool();

	void* XMETHODCALLTYPE getIcon() override;
	const char* XMETHODCALLTYPE getTitle() override;

	void XMETHODCALLTYPE activate() override;
	void XMETHODCALLTYPE deactivate() override;

	bool XMETHODCALLTYPE wantMouse2D() override;
	bool XMETHODCALLTYPE wantMouse3D() override;


	bool XMETHODCALLTYPE onMouseDown(UINT uXpos, UINT uYpos, X_WINDOW_POS winPos) override;
	bool XMETHODCALLTYPE onMouseMove(UINT uXpos, UINT uYpos, X_WINDOW_POS winPos) override;
	void XMETHODCALLTYPE onMouseUp() override;

private:
	CEditable *m_pEditable;

	HBITMAP m_hBitmap;
};

#endif
