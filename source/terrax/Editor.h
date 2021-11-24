#ifndef __EDITOR_H
#define __EDITOR_H

#include <xcommon/editor/IXEditor.h>
#include <xcommon/IXCore.h>
//#include "terrax.h"
#include "GizmoHandle.h"
#include "GizmoRadius.h"
#include "GizmoMove.h"
#include "GizmoRotate.h"
#include "GizmoScale.h"
#include "EditorMaterialBrowser.h"

#define GIZMO_TYPES() \
	GTO(Handle)\
	GTO(Radius)\
	GTO(Move)\
	GTO(Rotate)\
	GTO(Scale)

class CEditor: public IXUnknownImplementation<IXEditor>
{
public:
	CEditor(IXCore *pCore);
	~CEditor();

	void XMETHODCALLTYPE getCameraForView(X_WINDOW_POS winPos, ICamera **ppCamera) override;

#define GTO(gt) \
	void XMETHODCALLTYPE newGizmo##gt(IXEditorGizmo##gt **ppOut) override;\
	void onGizmoRemoved(CGizmo##gt *pGizmo);
	GIZMO_TYPES();
#undef GTO

	void render(bool is3D);

	void setDirty()
	{
		m_isDirty = true;
	}

	void onMouseMove();
	bool onMouseDown();
	void onMouseUp();

	const TerraXState* XMETHODCALLTYPE getState() override;

	X_2D_VIEW XMETHODCALLTYPE getViewForWindow(X_WINDOW_POS winPos) override;
	float XMETHODCALLTYPE getViewScale(X_WINDOW_POS winPos) override;
	bool XMETHODCALLTYPE getGridSnapState() override;
	float XMETHODCALLTYPE getGridStep() override;

	void XMETHODCALLTYPE addObject(IXEditorObject *pObject) override;
	void XMETHODCALLTYPE removeObject(IXEditorObject *pObject) override;

	bool XMETHODCALLTYPE execCommand(IXEditorCommand *pCmd) override;

	void* XMETHODCALLTYPE getMainWindow() override;

	void XMETHODCALLTYPE disableCurrentTool() override;

	IXEditorMaterialBrowser* XMETHODCALLTYPE getMaterialBrowser() override
	{
		return(&m_matBrowser);
	}

	bool XMETHODCALLTYPE isKeyPressed(UINT uKey) override;

private:
#define GTO(gt) Array<CGizmo##gt*> m_aGizmos##gt; CGizmo##gt *m_pSelected##gt = NULL;
	GIZMO_TYPES();
#undef GTO

	IXGizmoRenderer *m_pGizmoRendererBoth = NULL;
	IXGizmoRenderer *m_pGizmoRenderer2D = NULL;
	IXGizmoRenderer *m_pGizmoRenderer3D = NULL;

	bool m_isCapturing = false;

	bool m_isDirty = false;

	float3_t m_vOldCamPos;

	CEditorMaterialBrowser m_matBrowser;
};

#endif
