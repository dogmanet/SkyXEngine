#include "Editor.h"
#include "terrax.h"

CEditor::CEditor(IXCore *pCore)
{
	IXRenderUtils *pUtils = (IXRenderUtils*)pCore->getPluginManager()->getInterface(IXRENDERUTILS_GUID);
	pUtils->newGizmoRenderer(&m_pGizmoRenderer2D);
	pUtils->newGizmoRenderer(&m_pGizmoRenderer3D);
	pUtils->newGizmoRenderer(&m_pGizmoRendererBoth);

	IXEditorGizmoHandle *pHandle;
	IXEditorGizmoRadius *pRadius;

	newGizmoHandle(&pHandle);
	newGizmoRadius(&pRadius);
}

CEditor::~CEditor()
{
	mem_release(m_pGizmoRendererBoth);
	mem_release(m_pGizmoRenderer2D);
	mem_release(m_pGizmoRenderer3D);
}

void XMETHODCALLTYPE CEditor::getCameraForView(X_WINDOW_POS winPos, ICamera **ppCamera)
{
	*ppCamera = g_xConfig.m_pViewportCamera[winPos];
}

#define GTO(gt) void XMETHODCALLTYPE CEditor::newGizmo##gt(IXEditorGizmo##gt **ppOut)\
{\
	CGizmo##gt *pGizmo = new CGizmo##gt(this);\
	*ppOut = pGizmo;\
	m_aGizmos##gt.push_back(pGizmo);\
}\
void CEditor::onGizmoRemoved(CGizmo##gt *pGizmo)\
{\
	int idx = m_aGizmos##gt.indexOf(pGizmo);\
	assert(idx >= 0);\
	if(idx >= 0){\
		m_aGizmos##gt.erase(idx);\
	}\
}
GIZMO_TYPES()
#undef GTO

void CEditor::render(bool is3D)
{
	if(m_isDirty)
	{
		m_isDirty = false;
		m_pGizmoRendererBoth->reset();
		m_pGizmoRenderer2D->reset();
		m_pGizmoRenderer3D->reset();

#define GTO(gt) for(UINT i = 0, l = m_aGizmos##gt.size(); i < l; ++i)\
		{\
			m_aGizmos##gt[i]->draw(m_pGizmoRendererBoth, m_pGizmoRenderer2D, m_pGizmoRenderer3D);\
		}
		GIZMO_TYPES();
#undef GTO

	}

	m_pGizmoRendererBoth->render(!is3D);
	if(is3D)
	{
		m_pGizmoRenderer3D->render(false);
	}
	else
	{
		m_pGizmoRenderer2D->render(true);
	}
}
