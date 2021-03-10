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
	void onMouseDown();
	void onMouseUp();

private:
#define GTO(gt) Array<CGizmo##gt*> m_aGizmos##gt;
	GIZMO_TYPES();
#undef GTO

	IXGizmoRenderer *m_pGizmoRendererBoth = NULL;
	IXGizmoRenderer *m_pGizmoRenderer2D = NULL;
	IXGizmoRenderer *m_pGizmoRenderer3D = NULL;

	CGizmoHandle *m_pSelectedHandle = NULL;
	bool m_isCapturing = false;

	bool m_isDirty = false;
};

#endif
