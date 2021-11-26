#include "Editor.h"
#include "terrax.h"
#include "UndoManager.h"

CEditor::CEditor(IXCore *pCore)
{
	IXRenderUtils *pUtils = (IXRenderUtils*)pCore->getPluginManager()->getInterface(IXRENDERUTILS_GUID);
	pUtils->newGizmoRenderer(&m_pGizmoRenderer2D);
	pUtils->newGizmoRenderer(&m_pGizmoRenderer3D);
	pUtils->newGizmoRenderer(&m_pGizmoRendererBoth);

	//IXEditorGizmoHandle *pHandle;
	//IXEditorGizmoRadius *pRadius;
	//IXEditorGizmoMove *pMove;
	//IXEditorGizmoRotate *pRotate;


	//newGizmoRotate(&pRotate);
	//newGizmoMove(&pMove);

	//newGizmoHandle(&pHandle);

	//pHandle->lockInPlane(float3_t(1.0f, 1.0f, 1.0f));
	//pHandle->lockInDir(float3_t(1.0f, 1.0f, 1.0f));
	//m_aGizmosHandle[0]->lockInPlane
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
			if(m_aGizmos##gt[i]->isEnabled()) \
			{\
				m_aGizmos##gt[i]->draw(m_pGizmoRendererBoth, m_pGizmoRenderer2D, m_pGizmoRenderer3D);\
			}\
		}
		GIZMO_TYPES();
#undef GTO
	}

	extern Array<IXEditorExtension*> g_apExtensions;

	for(UINT i = 0, l = g_apExtensions.size(); i < l; ++i)
	{
		g_apExtensions[i]->render(is3D);
	}

	m_pGizmoRendererBoth->render(!is3D);
	if(is3D)
	{
		m_pGizmoRenderer3D->render(false);

		ICamera *pCamera;
		getCameraForView(XWP_TOP_LEFT, &pCamera);
		if(SMVector3Length2(pCamera->getPosition() - m_vOldCamPos) > 0.1f)
		{
			m_vOldCamPos = pCamera->getPosition();
			setDirty();
		}
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
		SAFE_CALL(m_pSelectedHandle, setWorldRay, g_xState.vWorldRayStart, g_xState.vWorldRayDir);
		SAFE_CALL(m_pSelectedMove, setWorldRay, g_xState.vWorldRayStart, g_xState.vWorldRayDir);
		SAFE_CALL(m_pSelectedRotate, setWorldRay, g_xState.vWorldRayStart, g_xState.vWorldRayDir);
		return;
	}

	m_pSelectedHandle = NULL;
	m_pSelectedMove = NULL;
	m_pSelectedRotate = NULL;

	float3 vRayStart, vRayDir;
	vRayStart = g_xState.vWorldRayStart;
	vRayDir = g_xState.vWorldRayDir;

	if(g_xState.activeWindow == XWP_TOP_LEFT)
	{
		// raycast to transform gizmos
		{
			CGizmoMove *pGizmo, *pSelectedGizmo = NULL;
			float fDist2, fMinDist2 = FLT_MAX;
			for(UINT i = 0, l = m_aGizmosMove.size(); i < l; ++i)
			{
				pGizmo = m_aGizmosMove[i];
				if(pGizmo->isEnabled())
				{
					fDist2 = SMVector3Length2(pGizmo->getPos() - vRayStart);
					if(fDist2 < fMinDist2 && pGizmo->wantHandle(vRayStart, vRayDir))
					{
						pSelectedGizmo = pGizmo;
						fMinDist2 = fDist2;
					}
				}
			}

			if(pSelectedGizmo)
			{
				SetCursor(LoadCursor(NULL, IDC_SIZEALL));
				//m_pSelectedHandle = NULL;
				m_pSelectedMove = pSelectedGizmo;
				return;
			}
		}

		{
			CGizmoRotate *pGizmo, *pSelectedGizmo = NULL;
			float fDist2, fMinDist2 = FLT_MAX;
			for(UINT i = 0, l = m_aGizmosRotate.size(); i < l; ++i)
			{
				pGizmo = m_aGizmosRotate[i];
				if(pGizmo->isEnabled())
				{
					fDist2 = SMVector3Length2(pGizmo->getPos() - vRayStart);
					if(fDist2 < fMinDist2 && pGizmo->wantHandle(vRayStart, vRayDir))
					{
						pSelectedGizmo = pGizmo;
						fMinDist2 = fDist2;
					}
				}
			}

			if(pSelectedGizmo)
			{
				SetCursor(LoadCursor(NULL, IDC_SIZEALL));
				//m_pSelectedHandle = NULL;
				m_pSelectedRotate = pSelectedGizmo;
				return;
			}
		}
	}

	// raycast to handlers
	
	CGizmoHandle *pGizmo, *pSelectedGizmo = NULL;
	float fDist2, fMinDist2 = FLT_MAX;
	for(UINT i = 0, l = m_aGizmosHandle.size(); i < l; ++i)
	{
		pGizmo = m_aGizmosHandle[i];
		if(pGizmo->isEnabled())
		{
			fDist2 = SMDistancePointLine2(pGizmo->getPos(), vRayStart, vRayDir);
			if(fDist2 < fMinDist2 && pGizmo->wantHandle(vRayStart, vRayDir))
			{
				pSelectedGizmo = pGizmo;
				fMinDist2 = fDist2;
			}
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
	if(m_pSelectedHandle || m_pSelectedMove || m_pSelectedRotate)
	{
		SetCapture(g_xState.hActiveWnd);
		m_isCapturing = true;

		SAFE_CALL(m_pSelectedHandle, setBestPlaneNormal, g_xState.vBestPlaneNormal);
		SAFE_CALL(m_pSelectedHandle, setTracking, true);
		SAFE_CALL(m_pSelectedMove, setTracking, true);
		SAFE_CALL(m_pSelectedRotate, setTracking, true);

		onMouseMove();

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
		SAFE_CALL(m_pSelectedHandle, setTracking, false);
		SAFE_CALL(m_pSelectedMove, setTracking, false);
		SAFE_CALL(m_pSelectedRotate, setTracking, false);
	}
}

const TerraXState* XMETHODCALLTYPE CEditor::getState()
{
	return(&g_xState);
}

X_2D_VIEW XMETHODCALLTYPE CEditor::getViewForWindow(X_WINDOW_POS winPos)
{
	return(g_xConfig.m_x2DView[winPos]);
}

float XMETHODCALLTYPE CEditor::getViewScale(X_WINDOW_POS winPos)
{
	return(g_xConfig.m_fViewportScale[winPos]);
}

bool XMETHODCALLTYPE CEditor::getGridSnapState()
{
	return(g_xConfig.m_bSnapGrid);
}

float XMETHODCALLTYPE CEditor::getGridStep()
{
	return(XGetGridStep());
}

void XMETHODCALLTYPE CEditor::addObject(IXEditorObject *pObject)
{
	extern CUndoManager *g_pUndoManager;
	assert(pObject);
	assert(g_pUndoManager->isInCommandContext());
	if(!g_pUndoManager->isInCommandContext())
	{
		LibReport(REPORT_MSG_LEVEL_FATAL, "CEditor::addObject() is only available in undo/redo context!");
	}

	g_pLevelObjects.push_back(pObject);
	add_ref(pObject);
}

void XMETHODCALLTYPE CEditor::removeObject(IXEditorObject *pObject)
{
	extern CUndoManager *g_pUndoManager;
	assert(g_pUndoManager->isInCommandContext());
	if(!g_pUndoManager->isInCommandContext())
	{
		LibReport(REPORT_MSG_LEVEL_FATAL, "CEditor::removeObject() is only available in undo/redo context!");
	}

	//assert(pObject == g_pLevelObjects[g_pLevelObjects.size() - 1]);

	int idx = g_pLevelObjects.indexOf(pObject);
	assert(idx >= 0);
	if(idx >= 0)
	{
		mem_release(g_pLevelObjects[idx]);
		g_pLevelObjects.erase(idx);
	}
}

bool XMETHODCALLTYPE CEditor::execCommand(IXEditorCommand *pCmd)
{
	extern CUndoManager *g_pUndoManager;

	return(g_pUndoManager->execCommand(pCmd));
}

void* XMETHODCALLTYPE CEditor::getMainWindow()
{
	extern HWND g_hWndMain;
	return(g_hWndMain);
}

void XDisableCurrentTool();

void XMETHODCALLTYPE CEditor::disableCurrentTool()
{
	XDisableCurrentTool();
}

bool XMETHODCALLTYPE CEditor::isKeyPressed(UINT uKey)
{
	return(XIsKeyPressed(uKey));
}
