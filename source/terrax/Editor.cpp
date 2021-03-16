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
	//newGizmoRadius(&pRadius);
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

void CEditor::onMouseMove()
{

	if(m_isCapturing)
	{
		m_pSelectedHandle->setWorldRay(g_xState.vWorldRayStart, g_xState.vWorldRayDir);
		return;
	}

	// raycast to handlers
	
	float3 vRayStart, vRayDir;
	vRayStart = g_xState.vWorldRayStart;
	vRayDir = g_xState.vWorldRayDir;

	CGizmoHandle *pGizmo, *pSelectedGizmo = NULL;
	float fDist2, fMinDist2 = FLT_MAX;
	for(UINT i = 0, l = m_aGizmosHandle.size(); i < l; ++i)
	{
		pGizmo = m_aGizmosHandle[i];
		fDist2 = SMDistancePointLine2(pGizmo->getPos(), vRayStart, vRayDir);
		if(fDist2 < fMinDist2)
		{
			pSelectedGizmo = pGizmo;
			fMinDist2 = fDist2;
		}
	}
	if(pSelectedGizmo)
	{
		if(g_xState.activeWindow == XWP_TOP_LEFT)
		{
			SMMATRIX mViewProj;
			Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEWPROJ, &mViewProj);

			float3 vScreenPos = pSelectedGizmo->getPos() * mViewProj;
			vScreenPos /= vScreenPos.w;
			vScreenPos = (vScreenPos + float3(1.0f, -1.0f, 0.0f))* float3(0.5f, -0.5f, 1.0f) * g_xState.vWinSize;
			// check for distance

			float2 vec = float2(vScreenPos.x, vScreenPos.y) - g_xState.vMousePos;
			float fSize = pSelectedGizmo->getOnscreenSize();
			fSize = fSize * fSize;
			if(fSize < SMVector2Dot(vec, vec))
			{
				m_pSelectedHandle = NULL;
				return;
			}

		}
		else
		{
			float fViewScale = g_xConfig.m_fViewportScale[g_xState.activeWindow];
			float fWorldSize = pSelectedGizmo->getOnscreenSize() * fViewScale;
			fWorldSize = fWorldSize * fWorldSize;
			if(fMinDist2 > fWorldSize)
			{
				m_pSelectedHandle = NULL;
				return;
			}
		}

		// set cursor
		SetCursor(LoadCursor(NULL, IDC_CROSS));
	}
	m_pSelectedHandle = pSelectedGizmo;
}
bool CEditor::onMouseDown()
{
	if(m_pSelectedHandle)
	{
		SetCapture(g_xState.hActiveWnd);
		m_isCapturing = true;

		m_pSelectedHandle->setBestPlaneNormal(g_xState.vBestPlaneNormal);
		m_pSelectedHandle->setTracking(true);

		return(true);
	}

	return(false);
}
void CEditor::onMouseUp()
{
	if(m_isCapturing)
	{
		m_isCapturing = false;
		ReleaseCapture();
		m_pSelectedHandle->setTracking(false);

	}
}
