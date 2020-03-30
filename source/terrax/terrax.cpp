
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 
#include <skyxengine.h>
#include <io.h>
#include <windowsx.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include "resource.h"

#include "terrax.h"
#include "Grid.h"

#include <xcommon/editor/IXEditorObject.h>
#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include "UndoManager.h"
#include "Tools.h"

#include <xEngine/IXEngine.h>

#ifdef _DEBUG
#	pragma comment(lib, "xEngine_d.lib")
#else
#	pragma comment(lib, "xEngine.lib")
#endif

GX_ENABLE_HIGH_PERFORMANCE_DUAL_GPU();

extern HWND g_hWndMain;
CGrid *g_pGrid = NULL;
CTerraXRenderStates g_xRenderStates;

ATOM XRegisterClass(HINSTANCE hInstance);
BOOL XInitInstance(HINSTANCE, int);
void XInitGuiWindow(bool pre);

Array<IXEditable*> g_pEditableSystems;
Array<IXEditorObject*> g_pLevelObjects;
AssotiativeArray<AAString, IXEditable*> g_mEditableSystems;
//SGeom_GetCountModels()

static IGXVertexBuffer *g_pBorderVertexBuffer;
static IGXRenderBuffer *g_pBorderRenderBuffer;

CUndoManager *g_pUndoManager = NULL;
extern HWND g_hComboTypesWnd;

String g_sLevelName;

void XUpdateWindowTitle();

HACCEL g_hAccelTable = NULL;
IXEngine *g_pEngine = NULL;

IGXConstantBuffer *g_pCameraConstantBuffer = NULL;
IGXSwapChain *g_pTopRightSwapChain = NULL;
IGXSwapChain *g_pBottomLeftSwapChain = NULL;
IGXSwapChain *g_pBottomRightSwapChain = NULL;
IGXSwapChain *g_pGuiSwapChain = NULL;
IGXDepthStencilSurface *g_pTopRightDepthStencilSurface = NULL;
IGXDepthStencilSurface *g_pBottomLeftDepthStencilSurface = NULL;
IGXDepthStencilSurface *g_BottomRightDepthStencilSurface = NULL;
IGXDepthStencilSurface *g_pGuiDepthStencilSurface = NULL;
IGXDepthStencilState *g_pDSNoZ;

IGXTexture2D *g_pDashedMaterial = NULL;

void XReleaseViewports();
void XInitViewports();
void XInitViewportLayout(X_VIEWPORT_LAYOUT layout);

class CEngineCallback: public IXEngineCallback
{
public:
	void XMETHODCALLTYPE onGraphicsResize(UINT uWidth, UINT uHeight, bool isFullscreen, bool isBorderless, IXEngine *pEngine) override
	{
		XReleaseViewports();
		XInitViewports();
	}

	bool XMETHODCALLTYPE processWindowMessages() override
	{
		MSG msg = {0};

		while(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				return(false);
			}
			if(g_hAccelTable && TranslateAccelerator(GetParent((HWND)SGCore_GetHWND()), g_hAccelTable, &msg))
			{
				continue;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		if(g_is3DRotating || g_is3DPanning)
		{
			int x, y;
			SSInput_GetMouseDelta(&x, &y);
			static const float * sense = GET_PCVAR_FLOAT("cl_mousesense");
			float fCoeff = SMToRadian(0.022) * *sense;
			float dx = (float)x * fCoeff;
			float dy = (float)y * fCoeff;
			ICamera *pCamera = g_xConfig.m_pViewportCamera[XWP_TOP_LEFT];

			if(g_is3DRotating)
			{
				static float3 m_vPitchYawRoll;
				m_vPitchYawRoll.y -= dx;
				m_vPitchYawRoll.x -= dy;
				m_vPitchYawRoll.x = clampf(m_vPitchYawRoll.x, -SM_PIDIV2, SM_PIDIV2);
				while(m_vPitchYawRoll.y < 0.0f)
				{
					m_vPitchYawRoll.y += SM_2PI;
				}
				while(m_vPitchYawRoll.y > SM_2PI)
				{
					m_vPitchYawRoll.y -= SM_2PI;
				}

				pCamera->setOrientation(&(SMQuaternion(m_vPitchYawRoll.x, 'x') * SMQuaternion(m_vPitchYawRoll.z, 'z') * SMQuaternion(m_vPitchYawRoll.y, 'y')));
			}
			else if(g_is3DPanning)
			{
				float3 vPos, vUp, vRight;
				pCamera->getPosition(&vPos);
				pCamera->getUp(&vUp);
				pCamera->getRight(&vRight);
				vPos += vUp * -dy * 10.0f + vRight * dx * 10.0f;
				pCamera->setPosition(&vPos);
			}

			float3 dir;
			static float s_fSpeed = 0;
			float fMaxSpeed = 10.0f; //@TODO: CVar this!
			float fMaxSpeedBoost = 40.0f; //@TODO: CVar this!
			float fAccelTime = 0.5f; //@TODO: CVar this!
			if(GetAsyncKeyState(VK_SHIFT) < 0)
			{
				fMaxSpeed = fMaxSpeedBoost;
			}
			float fAccel = fMaxSpeed / fAccelTime;
			bool mov = false;
			//! @FIXME add actial timeDelta
			//float dt = (float)timeDelta * 0.001f;
			float dt = (float)16 * 0.001f;
			if(GetAsyncKeyState('W') < 0)
			{
				dir.z += 1.0f;
				mov = true;
			}
			if(GetAsyncKeyState('S') < 0)
			{
				dir.z -= 1.0f;
				mov = true;
			}
			if(GetAsyncKeyState('A') < 0)
			{
				dir.x -= 1.0f;
				mov = true;
			}
			if(GetAsyncKeyState('D') < 0)
			{
				dir.x += 1.0f;
				mov = true;
			}

			if(mov)
			{
				s_fSpeed += fAccel * dt;
				if(s_fSpeed > fMaxSpeed)
				{
					s_fSpeed = fMaxSpeed;
				}
				float3 vPos, vDir, vRight;
				pCamera->getPosition(&vPos);
				pCamera->getLook(&vDir);
				pCamera->getRight(&vRight);
				dir = SMVector3Normalize(dir) * dt * s_fSpeed;
				vPos += vDir * dir.z + vRight * dir.x;
				pCamera->setPosition(&vPos);
			}
			else
			{
				s_fSpeed = 0;
			}
		}

		return(true);
	}

	ICamera* XMETHODCALLTYPE getCameraForFrame() override
	{
		return(g_xConfig.m_pViewportCamera[XWP_TOP_LEFT]);
	}

protected:
};

class CRenderPipeline: public IXRenderPipeline
{
public:
	CRenderPipeline(IXCore *pCore):
		m_pCore(pCore)
	{
		pCore->getRenderPipeline(&m_pOldPipeline);
		pCore->setRenderPipeline(this);

		IPluginManager *pPluginManager = pCore->getPluginManager();
		m_pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);

		{
			m_pRenderPassGeometry2D = m_pMaterialSystem->getRenderPass("xGBuffer");
			// m_pRenderPassGeometry2D = m_pMaterialSystem->registerRenderPass("xEditor2D", "terrax/geom2d.ps", NULL, NULL, NULL, NULL, true);
		}
	}
	~CRenderPipeline()
	{
		m_pCore->setRenderPipeline(m_pOldPipeline);
		mem_release(m_pOldPipeline);
	}

	void resize(UINT uWidth, UINT uHeight, bool isWindowed = true) override
	{
		m_pOldPipeline->resize(uWidth, uHeight, isWindowed);
	}

	void renderFrame() override
	{
		m_pOldPipeline->renderFrame();
		
		IGXContext *pDXDevice = getDevice()->getThreadContext();
		pDXDevice->setDepthStencilState(NULL);

		m_pMaterialSystem->bindRenderPass(m_pRenderPassGeometry2D);

		XRender3D();

		//#############################################################################
		HWND hWnds[] = {g_hTopRightWnd, g_hBottomLeftWnd, g_hBottomRightWnd};
		IGXSwapChain *p2DSwapChains[] = {g_pTopRightSwapChain, g_pBottomLeftSwapChain, g_pBottomRightSwapChain};
		IGXDepthStencilSurface *p2DDepthStencilSurfaces[] = {g_pTopRightDepthStencilSurface, g_pBottomLeftDepthStencilSurface, g_BottomRightDepthStencilSurface};
		
		ICamera **pCameras = g_xConfig.m_pViewportCamera + 1;
		float *fScales = g_xConfig.m_fViewportScale + 1;
		X_2D_VIEW *views = g_xConfig.m_x2DView + 1;
		//ICamera *p3DCamera = SRender_GetCamera();
		pDXDevice->setSamplerState(NULL, 0);
		//#############################################################################

		XUpdateSelectionBound();


		for(int i = 0; i < 3; ++i)
		{
			if(!IsWindowVisible(hWnds[i]))
			{
				continue;
			}
			SRender_SetCamera(pCameras[i]);
			IGXSurface *pBackBuffer = p2DSwapChains[i]->getColorTarget();
			pDXDevice->setColorTarget(pBackBuffer);
			pDXDevice->setDepthStencilSurface(p2DDepthStencilSurfaces[i]);
			pDXDevice->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL);

			pDXDevice->setRasterizerState(g_xRenderStates.pRSWireframe);
			pDXDevice->setDepthStencilState(g_pDSNoZ);
 			pDXDevice->setBlendState(NULL);
			SMMATRIX mProj = SMMatrixOrthographicLH((float)pBackBuffer->getWidth() * fScales[i], (float)pBackBuffer->getHeight() * fScales[i], 1.0f, 2000.0f);
			SMMATRIX mView;
			pCameras[i]->getViewMatrix(&mView);
			Core_RMatrixSet(G_RI_MATRIX_OBSERVER_VIEW, &mView);
			Core_RMatrixSet(G_RI_MATRIX_VIEW, &mView);
			Core_RMatrixSet(G_RI_MATRIX_OBSERVER_PROJ, &mProj);
			Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &mProj);
			Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(mView * mProj));

			g_pCameraConstantBuffer->update(&SMMatrixTranspose(mView * mProj));
			pDXDevice->setVSConstant(g_pCameraConstantBuffer, 4);

			Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());
			Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_FREE);

			XRender2D(views[i], fScales[i], true);

			renderEditor2D();

			Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_MATERIAL);
			pDXDevice->setVSConstant(g_pCameraConstantBuffer, 4);
			XRender2D(views[i], fScales[i], false);
			mem_release(pBackBuffer);
		}

		/*
		IGXSurface *pBackBuffer = g_pGuiSwapChain->getColorTarget();
		pDXDevice->setColorTarget(pBackBuffer);
		pDXDevice->setDepthStencilSurface(g_pGuiDepthStencilSurface);
		pDXDevice->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL);
		XGuiRender();
		mem_release(pBackBuffer);
		*/
		//#############################################################################
		//SRender_SetCamera(p3DCamera);
		pDXDevice->setColorTarget(NULL);
		pDXDevice->setDepthStencilSurface(NULL);

	}
	void endFrame() override
	{
		m_pOldPipeline->endFrame();

		HWND hWnds[] = {g_hTopRightWnd, g_hBottomLeftWnd, g_hBottomRightWnd};
		IGXSwapChain *p2DSwapChains[] = {g_pTopRightSwapChain, g_pBottomLeftSwapChain, g_pBottomRightSwapChain};
		for(int i = 0; i < 3; ++i)
		{
			if(IsWindowVisible(hWnds[i]))
			{
				p2DSwapChains[i]->swapBuffers();
			}
		}
	}
	void updateVisibility() override
	{
		m_pOldPipeline->updateVisibility();
	}

	void renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility = NULL) override
	{
		m_pOldPipeline->renderStage(stage, pVisibility);
	}

	IGXDevice *getDevice() override
	{
		return(m_pOldPipeline->getDevice());
	}

	void newVisData(IXRenderableVisibility **ppVisibility) override
	{
		m_pOldPipeline->newVisData(ppVisibility);
	}

//protected:

	void renderPrepare() override
	{
		m_pOldPipeline->renderPrepare();
	}
	void renderGBuffer() override
	{
		m_pOldPipeline->renderGBuffer();
	}
	void renderShadows() override
	{
		m_pOldPipeline->renderShadows();
	}
	void renderGI() override
	{
		m_pOldPipeline->renderGI();
	}
	void renderPostprocessMain() override
	{
		m_pOldPipeline->renderPostprocessMain();
	}
	void renderTransparent() override
	{
		m_pOldPipeline->renderTransparent();
	}
	void renderPostprocessFinal()  override
	{
		m_pOldPipeline->renderPostprocessFinal();
	}
	void renderEditor2D() override
	{
		m_pOldPipeline->renderEditor2D();
	}

	IXCore *m_pCore;
	IXRenderPipeline *m_pOldPipeline = NULL;
	IXMaterialSystem *m_pMaterialSystem = NULL;

	XRenderPassHandler *m_pRenderPassGeometry2D = NULL;
};

#if defined(_WINDOWS)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	int argc = 0;
	char **argv = CommandLineToArgvA(lpCmdLine, &argc);
#else
int main(int argc, char **argv)
{
#endif

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES;
	InitCommonControlsEx(&icex);

	XRegisterClass(hInstance);
	g_pUndoManager = new CUndoManager();

	if(!XInitInstance(hInstance, nCmdShow))
	{
		return(1);
	}

	 XInitGuiWindow(true);

	//SkyXEngine_Init(g_hTopLeftWnd, g_hWndMain, lpCmdLine);
	IXEngine *pEngine = XEngineInit(argc, argv, "TerraX");
	g_pEngine = pEngine;
	CEngineCallback engineCb;
	pEngine->initGraphics((XWINDOW_OS_HANDLE)g_hTopLeftWnd, &engineCb);
	pEngine->getCore()->execCmd("gmode editor");
	pEngine->getCore()->execCmd("exec ../config_editor.cfg");
	CRenderPipeline *pPipeline = new CRenderPipeline(Core_GetIXCore());
	 XInitGuiWindow(false);

	RECT rcTopLeft;
	GetClientRect(g_hTopLeftWnd, &rcTopLeft);

	g_pEngine->getCore()->execCmd2("r_win_width %d\nr_win_height %d", rcTopLeft.right - rcTopLeft.left, rcTopLeft.bottom - rcTopLeft.top);


	IPluginManager *pPluginManager = Core_GetIXCore()->getPluginManager();

	IXMaterialSystem *pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);

	GXCOLOR w = GX_COLOR_ARGB(255, 255, 255, 255);
	GXCOLOR t = GX_COLOR_ARGB(0, 255, 255, 255);
	GXCOLOR colorData[] = {
		t, t, t, w, w, w,
		w, t, t, t, w, w,
		w, w, t, t, t, w,
		w, w, w, t, t, t,
		t, w, w, w, t, t,
		t, t, w, w, w, t
	};
	g_pDashedMaterial = SGCore_GetDXDevice()->createTexture2D(6, 6, 1, 0, GXFMT_A8R8G8B8, colorData);
	//pMaterialSystem->addTexture("dev_dashed", pDashedMaterial);
	//mem_release(pDashedMaterial);

	//GXCOLOR tmpColor = GX_COLOR_ARGB(255, 255, 255, 255);
	//IGXTexture2D* pWhiteMaterial = SGCore_GetDXDevice()->createTexture2D(1, 1, 1, 0, GXFMT_A8R8G8B8, &tmpColor);
	////pMaterialSystem->addTexture("dev_white", pWhiteMaterial);
	//mem_release(pDashedMaterial);

	UINT ic = 0;
	IXEditable *pEditable;
	while((pEditable = (IXEditable*)pPluginManager->getInterface(IXEDITABLE_GUID, ic++)))
	{
		if(pEditable->getVersion() == IXEDITABLE_VERSION)
		{
			g_pEditableSystems.push_back(pEditable);
			pEditable->startup(SGCore_GetDXDevice());

			ComboBox_AddString(g_hComboTypesWnd, pEditable->getName());

			g_mEditableSystems[AAString(pEditable->getName())] = pEditable;
		}
	}
	if(g_pEditableSystems.size() > 0)
	{
		ComboBox_SetCurSel(g_hComboTypesWnd, 0);
		SendMessage(GetParent(g_hComboTypesWnd), WM_COMMAND, MAKEWPARAM(IDC_CMB_TYPE, CBN_SELCHANGE), (LPARAM)g_hComboTypesWnd);
		if(g_pEditableSystems.size() == 1)
		{
			ComboBox_Enable(g_hComboTypesWnd, FALSE);
		}
	}
	
	Core_GetIXCore()->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID)->addListener([](const XEventLevel *pData)
	{
		switch(pData->type)
		{
		case XEventLevel::TYPE_LOAD:
			g_sLevelName = pData->szLevelName;
			XUpdateWindowTitle();
			{
				char szPathLevel[1024], szKey[64];
				sprintf(szPathLevel, "%s%s/%s.lvl", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), pData->szLevelName, pData->szLevelName);

				ISXConfig *pCfg = Core_OpConfig(szPathLevel);
				const char *szVal;

				szVal = pCfg->getKey("terrax", "vp_layout");
				if(szVal)
				{
					int iVal = 0;
					if(sscanf(szVal, "%d", &iVal) && iVal >= 0 && iVal <= 3)
					{
						XInitViewportLayout((X_VIEWPORT_LAYOUT)iVal);
					}
				}

				szVal = pCfg->getKey("terrax", "grid_step");
				if(szVal)
				{
					int iVal = 0;
					if(sscanf(szVal, "%d", &iVal) && iVal >= GRID_STEP_MINIMAL && iVal <= GRID_STEP_MAXIMAL)
					{
						g_xConfig.m_gridStep = (GRID_STEP)iVal;
					}
				}

				szVal = pCfg->getKey("terrax", "grid_show");
				if(szVal)
				{
					int iVal = 0;
					if(sscanf(szVal, "%d", &iVal))
					{
						g_xConfig.m_bShowGrid = iVal != 0;
					}
				}

				for(UINT i = 0; i < 4; ++i)
				{
					float3 vec;
					sprintf_s(szKey, "cam%u_pos", i);
					szVal = pCfg->getKey("terrax", szKey);
					if(szVal)
					{
						if(sscanf(szVal, "%f %f %f", &vec.x, &vec.y, &vec.z) == 3)
						{
							g_xConfig.m_pViewportCamera[i]->setPosition(&vec);
						}
					}

					sprintf_s(szKey, "cam%u_dir", i);
					szVal = pCfg->getKey("terrax", szKey);
					if(szVal)
					{
						if(sscanf(szVal, "%f %f %f", &vec.x, &vec.y, &vec.z) == 3)
						{
							g_xConfig.m_pViewportCamera[i]->setOrientation(&(SMQuaternion(vec.x, 'x') * SMQuaternion(vec.y, 'y') * SMQuaternion(vec.z, 'z')));
						}
					}

					sprintf_s(szKey, "cam%u_scale", i);
					szVal = pCfg->getKey("terrax", szKey);
					if(szVal)
					{
						float fVal = 0.0;
						if(sscanf(szVal, "%f", &fVal) && fVal > 0.0f)
						{
							g_xConfig.m_fViewportScale[i] = fVal;
						}
					}

					sprintf_s(szKey, "cam%u_view", i);
					szVal = pCfg->getKey("terrax", szKey);
					if(szVal)
					{
						int iVal = 0;
						if(sscanf(szVal, "%d", &iVal) && iVal >= -1 && iVal <= 2)
						{
							g_xConfig.m_x2DView[i] = (X_2D_VIEW)iVal;
						}
					}
				}

				mem_release(pCfg);
			}
			break;
		case XEventLevel::TYPE_UNLOAD:

			g_sLevelName = "";

			for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
			{
				mem_release(g_pLevelObjects[i]);
			}
			g_pLevelObjects.clear();


			g_pUndoManager->reset();

			XUpdateWindowTitle();
			break;

		case XEventLevel::TYPE_SAVE:
			g_sLevelName = pData->szLevelName;
			g_pUndoManager->makeClean();
			{
				char szPathLevel[1024], szKey[64], szVal[1024];
				sprintf(szPathLevel, "%s%s/%s.lvl", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), pData->szLevelName, pData->szLevelName);

				ISXConfig *pCfg = Core_OpConfig(szPathLevel);
				
				if(!pCfg->keyExists("level", "type"))
				{
					pCfg->set("level", "type", "indoor");
				}
				if(!pCfg->keyExists("level", "local_name"))
				{
					pCfg->set("level", "local_name", pData->szLevelName);
				}

				for(UINT i = 0; i < 4; ++i)
				{
					float3 vec;
					g_xConfig.m_pViewportCamera[i]->getPosition(&vec);
					sprintf_s(szVal, "%f %f %f", vec.x, vec.y, vec.z);
					sprintf_s(szKey, "cam%u_pos", i);
					pCfg->set("terrax", szKey, szVal);

					g_xConfig.m_pViewportCamera[i]->getRotation(&vec);
					sprintf_s(szVal, "%f %f %f", vec.x, vec.y, vec.z);
					sprintf_s(szKey, "cam%u_dir", i);
					pCfg->set("terrax", szKey, szVal);

					sprintf_s(szVal, "%f", g_xConfig.m_fViewportScale[i]);
					sprintf_s(szKey, "cam%u_scale", i);
					pCfg->set("terrax", szKey, szVal);

					sprintf_s(szVal, "%d", g_xConfig.m_x2DView[i]);
					sprintf_s(szKey, "cam%u_view", i);
					pCfg->set("terrax", szKey, szVal);
				}

				sprintf_s(szVal, "%d", g_xConfig.m_xViewportLayout);
				pCfg->set("terrax", "vp_layout", szVal);

				sprintf_s(szVal, "%d", g_xConfig.m_gridStep);
				pCfg->set("terrax", "grid_step", szVal);

				sprintf_s(szVal, "%d", g_xConfig.m_bShowGrid ? 1 : 0);
				pCfg->set("terrax", "grid_show", szVal);

				pCfg->save();
				mem_release(pCfg);
			}
			break;

		case XEventLevel::TYPE_LOAD_END:
			for(UINT ic = 0, il = g_pEditableSystems.size(); ic < il; ++ic)
			{
				IXEditable *pEditable = g_pEditableSystems[ic];
				for(ID i = 0, l = pEditable->getObjectCount(); i < l; ++i)
				{
					g_pLevelObjects.push_back(pEditable->getObject(i));
				}
			}
			break;
		}
	});
	
	//SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	//SGCore_SkyBoxLoadTex("sky_hdr.dds");
	//SGCore_SkyBoxLoadTex("sky_test_cube.dds");
	//SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	//SGCore_SkyBoxSetUse(false);
	//SGCore_SkyCloudsSetUse(false);

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	g_xConfig.m_pViewportCamera[XWP_TOP_LEFT] = SGCore_CrCamera();
	g_xConfig.m_pViewportCamera[XWP_TOP_LEFT]->setFOV(*r_default_fov);

	g_xConfig.m_pViewportCamera[XWP_TOP_RIGHT] = SGCore_CrCamera();
	g_xConfig.m_pViewportCamera[XWP_TOP_RIGHT]->setPosition(&X2D_TOP_POS);
	g_xConfig.m_pViewportCamera[XWP_TOP_RIGHT]->setOrientation(&X2D_TOP_ROT);

	g_xConfig.m_pViewportCamera[XWP_BOTTOM_LEFT] = SGCore_CrCamera();
	g_xConfig.m_pViewportCamera[XWP_BOTTOM_LEFT]->setPosition(&X2D_SIDE_POS);
	g_xConfig.m_pViewportCamera[XWP_BOTTOM_LEFT]->setOrientation(&X2D_SIDE_ROT);

	g_xConfig.m_pViewportCamera[XWP_BOTTOM_RIGHT] = SGCore_CrCamera();
	g_xConfig.m_pViewportCamera[XWP_BOTTOM_RIGHT]->setPosition(&X2D_FRONT_POS);
	g_xConfig.m_pViewportCamera[XWP_BOTTOM_RIGHT]->setOrientation(&X2D_FRONT_ROT);


//	SkyXEngine_RunGenPreview();
	//Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
	Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
	
	g_pGrid = new CGrid();
	g_pGrid->setOpacity(0.7f);
	
	g_xRenderStates.idColoredShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "terrax_colored.vs");
	g_xRenderStates.idColoredShaderPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "terrax_colored.ps");
	g_xRenderStates.idColoredShaderKit = SGCore_ShaderCreateKit(g_xRenderStates.idColoredShaderVS, g_xRenderStates.idColoredShaderPS);
//	SkyXEngine_PreviewKill();
	IGXDevice *pDevice = SGCore_GetDXDevice();

	GXBlendDesc blendDesc;
	blendDesc.renderTarget[0].useBlend = true;
	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_BLEND_FACTOR;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_BLEND_FACTOR;
	g_xRenderStates.pBlendColorFactor = pDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_SRC_ALPHA;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_SRC_ALPHA;
	g_xRenderStates.pBlendAlpha = pDevice->createBlendState(&blendDesc);

	GXRasterizerDesc rsDesc; 
	rsDesc.cullMode = GXCULL_NONE;
	g_xRenderStates.pRSSolidNoCull = SGCore_GetDXDevice()->createRasterizerState(&rsDesc);

	rsDesc.fillMode = GXFILL_WIREFRAME;
	g_xRenderStates.pRSWireframe = SGCore_GetDXDevice()->createRasterizerState(&rsDesc);

	g_xRenderStates.idTexturedShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "terrax_textured.vs");
	g_xRenderStates.idTexturedShaderPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "terrax_textured.ps");
	g_xRenderStates.idTexturedShaderKit = SGCore_ShaderCreateKit(g_xRenderStates.idTexturedShaderVS, g_xRenderStates.idTexturedShaderPS);

	GXVertexElement oLayout[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		GX_DECL_END()
	};
	IGXVertexDeclaration *pVD = pDevice->createVertexDeclaration(oLayout);
	g_pBorderVertexBuffer = pDevice->createVertexBuffer(sizeof(XBorderVertex) * 5, GXBUFFER_USAGE_STREAM);
	g_pBorderRenderBuffer = pDevice->createRenderBuffer(1, &g_pBorderVertexBuffer, pVD);
	mem_release(pVD);

	// Object handlers
	GXVertexElement oLayoutHandler[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION, GXDECLSPEC_PER_VERTEX_DATA},
		{1, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD, GXDECLSPEC_PER_INSTANCE_DATA},
		GX_DECL_END()
	};
	pVD = pDevice->createVertexDeclaration(oLayoutHandler);
	g_xRenderStates.pHandlerVB = pDevice->createVertexBuffer(sizeof(float3_t) * 8, GXBUFFER_USAGE_STREAM);
	g_xRenderStates.pHandlerInstanceVB = pDevice->createVertexBuffer(sizeof(float3_t) * X_MAX_HANDLERS_PER_DIP, GXBUFFER_USAGE_STREAM);
	IGXVertexBuffer *ppVB[] = {g_xRenderStates.pHandlerVB, g_xRenderStates.pHandlerInstanceVB};
	g_xRenderStates.pHandlerRB = pDevice->createRenderBuffer(2, ppVB, pVD);
	mem_release(pVD);
	g_xRenderStates.idHandlerShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "terrax_handler.vs");
	g_xRenderStates.idHandlerShaderKit = SGCore_ShaderCreateKit(g_xRenderStates.idHandlerShaderVS, g_xRenderStates.idColoredShaderPS);
	USHORT pHandlerIndices[] = {0, 1, 2, 3, 4, 5, 6, 7};
	g_xRenderStates.pHandlerIB = pDevice->createIndexBuffer(sizeof(USHORT)* 8, GXBUFFER_USAGE_STATIC, GXIT_UINT16, pHandlerIndices);

	{
		// Transform handlers
		GXVertexElement oLayoutHandler[] =
		{
			{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION, GXDECLSPEC_PER_VERTEX_DATA},
			GX_DECL_END()
		};
		pVD = pDevice->createVertexDeclaration(oLayoutHandler);
		g_xRenderStates.pTransformHandlerVB = pDevice->createVertexBuffer(sizeof(float3_t)* 32, GXBUFFER_USAGE_STREAM);
		g_xRenderStates.pTransformHandlerRB = pDevice->createRenderBuffer(1, &g_xRenderStates.pTransformHandlerVB, pVD);
		USHORT pHandlerIndices[] = {
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14, 12, 14, 15,

			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23,
			24, 25, 26, 24, 26, 27,
			28, 29, 30, 28, 30, 31
		};
		g_xRenderStates.pTransformHandlerScaleIB = pDevice->createIndexBuffer(sizeof(USHORT)* 48, GXBUFFER_USAGE_STATIC, GXIT_UINT16, pHandlerIndices);
		
		USHORT pHandlerRotateIndices[] = {
			0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7,
			8, 9, 10, 8, 10, 11, 8, 11, 12, 8, 12, 13, 8, 13, 14, 8, 14, 15,
			16, 17, 18, 16, 18, 19, 16, 19, 20, 16, 20, 21, 16, 21, 22, 16, 22, 23,
			24, 25, 26, 24, 26, 27, 24, 27, 28, 24, 28, 29, 24, 29, 30, 24, 30, 31
		};
		g_xRenderStates.pTransformHandlerRotateIB = pDevice->createIndexBuffer(sizeof(USHORT)* 72, GXBUFFER_USAGE_STATIC, GXIT_UINT16, pHandlerRotateIndices);

		float3_t pVertices[] = {
			float3_t(10000.0f, 0.0f, 0.0f), float3_t(-10000.0f, 0.0f, 0.0f),
			float3_t(0.0f, 10000.0f, 0.0f), float3_t(0.0f, -10000.0f, 0.0f),
			float3_t(0.0f, 0.0f, 10000.0f), float3_t(0.0f, 0.0f, -10000.0f)
		};
		IGXVertexBuffer *pCreateCrossVB = pDevice->createVertexBuffer(sizeof(float3_t) * 6, GXBUFFER_USAGE_STATIC, pVertices);
		g_xRenderStates.pCreateCrossRB = pDevice->createRenderBuffer(1, &pCreateCrossVB, pVD);

		mem_release(pVD);
	}

	GXDepthStencilDesc dsDesc;
	dsDesc.useDepthTest = FALSE;
	dsDesc.useDepthWrite = FALSE;
	g_pDSNoZ = SGCore_GetDXDevice()->createDepthStencilState(&dsDesc);

	XInitViewports();

	g_pCameraConstantBuffer = SGCore_GetDXDevice()->createConstantBuffer(sizeof(SMMATRIX));

	int result = pEngine->start();

	for(UINT ic = 0, il = g_pEditableSystems.size(); ic < il; ++ic)
	{
		g_pEditableSystems[ic]->shutdown();
	}
	XReleaseViewports();

	mem_release(g_pDashedMaterial);
	mem_release(g_pDSNoZ);
	mem_delete(pPipeline);
	mem_release(g_pCameraConstantBuffer);
	mem_delete(g_pGrid);
	mem_delete(g_pUndoManager);
	//SkyXEngine_Kill();
	mem_release(pEngine);
	return result;
}

void XInitViewports()
{
	IGXDevice *pContext = SGCore_GetDXDevice();
	RECT rc;
	GetClientRect(g_hTopRightWnd, &rc);
	g_pTopRightSwapChain = pContext->createSwapChain(rc.right - rc.left, rc.bottom - rc.top, g_hTopRightWnd);
	g_pTopRightDepthStencilSurface = pContext->createDepthStencilSurface(rc.right - rc.left, rc.bottom - rc.top, GXFMT_D24S8, GXMULTISAMPLE_NONE);

	GetClientRect(g_hBottomLeftWnd, &rc);
	g_pBottomLeftSwapChain = pContext->createSwapChain(rc.right - rc.left, rc.bottom - rc.top, g_hBottomLeftWnd);
	g_pBottomLeftDepthStencilSurface = pContext->createDepthStencilSurface(rc.right - rc.left, rc.bottom - rc.top, GXFMT_D24S8, GXMULTISAMPLE_NONE);

	GetClientRect(g_hBottomRightWnd, &rc);
	g_pBottomRightSwapChain = pContext->createSwapChain(rc.right - rc.left, rc.bottom - rc.top, g_hBottomRightWnd);
	g_BottomRightDepthStencilSurface = pContext->createDepthStencilSurface(rc.right - rc.left, rc.bottom - rc.top, GXFMT_D24S8, GXMULTISAMPLE_NONE);
	/*
	GetClientRect(g_pGuiWnd, &rc);
	g_pGuiSwapChain = pContext->createSwapChain(rc.right - rc.left, rc.bottom - rc.top, g_pGuiWnd);
	g_pGuiDepthStencilSurface = pContext->createDepthStencilSurface(rc.right - rc.left, rc.bottom - rc.top, GXFMT_D24S8, GXMULTISAMPLE_NONE);
	*/
}

void XReleaseViewports()
{
	mem_release(g_pTopRightSwapChain);
	mem_release(g_pBottomLeftSwapChain);
	mem_release(g_pBottomRightSwapChain);
	mem_release(g_pGuiSwapChain);
	mem_release(g_pTopRightDepthStencilSurface);
	mem_release(g_pBottomLeftDepthStencilSurface);
	mem_release(g_BottomRightDepthStencilSurface);
	mem_release(g_pGuiDepthStencilSurface);
}

void XRender3D()
{
	IGXDevice *pDevice = SGCore_GetDXDevice();
	IGXContext *pCtx = pDevice->getThreadContext();

	for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
	{
		if(g_pLevelObjects[i]->isSelected())
		{
			g_pLevelObjects[i]->renderSelection(true);
		}
	}

	if(g_xState.isFrameSelect)
	{
		X_2D_VIEW xCurView = g_xConfig.m_x2DView[g_xState.activeWindow];

		float3_t va, vb, vc, vd;

		switch(xCurView)
		{
		case X2D_TOP:
			va = float3_t(g_xState.vFrameSelectStart.x, 0.0f, g_xState.vFrameSelectStart.y);
			vb = float3_t(g_xState.vWorldMousePos.x, 0.0f, g_xState.vFrameSelectStart.y);
			vc = float3_t(g_xState.vWorldMousePos.x, 0.0f, g_xState.vWorldMousePos.y);
			vd = float3_t(g_xState.vFrameSelectStart.x, 0.0f, g_xState.vWorldMousePos.y);
			break;
		case X2D_FRONT:
			va = float3_t(g_xState.vFrameSelectStart.x, g_xState.vFrameSelectStart.y, 0.0f);
			vb = float3_t(g_xState.vWorldMousePos.x, g_xState.vFrameSelectStart.y, 0.0f);
			vc = float3_t(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, 0.0f);
			vd = float3_t(g_xState.vFrameSelectStart.x, g_xState.vWorldMousePos.y, 0.0f);
			break;
		case X2D_SIDE:
			va = float3_t(0.0f, g_xState.vFrameSelectStart.y, g_xState.vFrameSelectStart.x);
			vb = float3_t(0.0f, g_xState.vFrameSelectStart.y, g_xState.vWorldMousePos.x);
			vc = float3_t(0.0f, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
			vd = float3_t(0.0f, g_xState.vWorldMousePos.y, g_xState.vFrameSelectStart.x);
			break;
		}

		XDrawBorder(GX_COLOR_ARGB(255, 255, 255, 0), va, vb, vc, vd);
	}

	static IGXConstantBuffer *s_pColorBuffer = pDevice->createConstantBuffer(sizeof(float4));
	static IGXConstantBuffer *s_mConstWVP = pDevice->createConstantBuffer(sizeof(SMMATRIX));

	if(g_xState.bCreateMode)
	{
		IGXRasterizerState *pOldRS = pCtx->getRasterizerState();
		pCtx->setRasterizerState(g_xRenderStates.pRSSolidNoCull);
		SMMATRIX mViewProj;
		Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mViewProj);
		pCtx->setVSConstant(s_mConstWVP, 4);
		pCtx->setPSConstant(s_pColorBuffer);
		SGCore_ShaderBind(g_xRenderStates.idColoredShaderKit);

		pCtx->setRenderBuffer(g_xRenderStates.pCreateCrossRB);
		s_mConstWVP->update(&SMMatrixTranspose(SMMatrixTranslation(g_xState.vCreateOrigin) * mViewProj));
		s_pColorBuffer->update(&float4(0.0f, 1.0f, 0.0f, 1.0f));
		pCtx->setPrimitiveTopology(GXPT_LINELIST);
		pCtx->drawPrimitive(0, 3);
		pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

		pCtx->setRasterizerState(pOldRS);
		mem_release(pOldRS);
	}
}

void XRender2D(X_2D_VIEW view, float fScale, bool preScene)
{
	IGXDevice *pDevice = SGCore_GetDXDevice();
	IGXContext *pCtx = pDevice->getThreadContext();

	if(preScene)
	{
		if(g_xConfig.m_bShowGrid)
		{
			GRID_STEP step = g_xConfig.m_gridStep;
			GRID_STEP minStep = CGrid::GetMinStepForScale(fScale);
			if(step < minStep)
			{
				step = minStep;
			}
			switch(view)
			{
			case X2D_FRONT:
				Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixRotationX(SM_PIDIV2));
				break;
			case X2D_SIDE:
				Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixRotationZ(SM_PIDIV2));
				break;
			}
			g_pGrid->setOpacity(g_xConfig.m_fGridOpacity);
			g_pGrid->setDottedMode(g_xConfig.m_bDottedGrid);
			g_pGrid->render(step);

			Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

		}
	}
	else
	{
		static IGXConstantBuffer *s_pColorBuffer = pDevice->createConstantBuffer(sizeof(float4));

		for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
		{
			if(g_pLevelObjects[i]->isSelected())
			{
				g_pLevelObjects[i]->renderSelection(false);
			}
		}

		// Draw handlers
		if(g_pLevelObjects.size())
		{
			pCtx->setPSConstant(s_pColorBuffer);

			pCtx->setPrimitiveTopology(GXPT_LINELIST);
			SGCore_ShaderBind(g_xRenderStates.idHandlerShaderKit);
			pCtx->setIndexBuffer(g_xRenderStates.pHandlerIB);

			s_pColorBuffer->update(&float4(0.0f, 1.0f, 0.0f, 1.0f));

			float3_t *pvData;
			float fPtSize = 3.5f * fScale;
			if(g_xRenderStates.pHandlerVB->lock((void**)&pvData, GXBL_WRITE))
			{
				pvData[0] = float3_t(fPtSize, fPtSize, fPtSize);
				pvData[1] = (float3)(pvData[0] * -1.0f);

				pvData[2] = float3_t(fPtSize, fPtSize, -fPtSize);
				pvData[3] = (float3)(pvData[2] * -1.0f);

				pvData[4] = float3_t(fPtSize, -fPtSize, fPtSize);
				pvData[5] = (float3)(pvData[4] * -1.0f);

				pvData[6] = float3_t(-fPtSize, fPtSize, fPtSize);
				pvData[7] = (float3)(pvData[6] * -1.0f);

				g_xRenderStates.pHandlerVB->unlock();
			}
			pCtx->setRenderBuffer(g_xRenderStates.pHandlerRB);
			float4 fViewportBorders = g_xState.m_vViewportBorders[g_xState.activeWindow];
			for(bool isSelected = false;; isSelected = true)
			{
				UINT uHandlerCount = 0;
				pvData = NULL;
				for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
				{
					float3_t vPos = g_pLevelObjects[i]->getPos();
					//@TODO: Add visibility check
					/*if(fViewportBorders.x > vPos.x || fViewportBorders.z < vPos.x || fViewportBorders.y < vPos.z) // not visible
					{
						continue;
					}*/
					if(isSelected != g_pLevelObjects[i]->isSelected())
					{
						continue;
					}
					if(!pvData && !g_xRenderStates.pHandlerInstanceVB->lock((void**)&pvData, GXBL_WRITE))
					{
						break;
					}

					pvData[uHandlerCount++] = vPos;
					if(uHandlerCount == X_MAX_HANDLERS_PER_DIP)
					{
						g_xRenderStates.pHandlerInstanceVB->unlock();
						pCtx->drawIndexedInstanced(uHandlerCount, 8, 4, 0, 0);
						pvData = NULL;
						uHandlerCount = 0;
					}
				}
				if(pvData)
				{
					g_xRenderStates.pHandlerInstanceVB->unlock();
				}
				if(uHandlerCount)
				{
					pCtx->drawIndexedInstanced(uHandlerCount, 8, 4, 0, 0);
				}
				if(isSelected)
				{
					break;
				}
				s_pColorBuffer->update(&float4(1.0f, 0.0f, 0.0f, 1.0f));
			}

			SGCore_ShaderUnBind();
		}

		if(g_xState.isFrameSelect)
		{
			X_2D_VIEW xCurView = g_xConfig.m_x2DView[g_xState.activeWindow];
			float3_t va, vb, vc, vd;
			switch(xCurView)
			{
			case X2D_TOP:
				va = float3_t(g_xState.vFrameSelectStart.x, 0.0f, g_xState.vFrameSelectStart.y);
				vb = float3_t(g_xState.vWorldMousePos.x, 0.0f, g_xState.vFrameSelectStart.y);
				vc = float3_t(g_xState.vWorldMousePos.x, 0.0f, g_xState.vWorldMousePos.y);
				vd = float3_t(g_xState.vFrameSelectStart.x, 0.0f, g_xState.vWorldMousePos.y);
				break;
			case X2D_FRONT:
				va = float3_t(g_xState.vFrameSelectStart.x, g_xState.vFrameSelectStart.y, 0.0f);
				vb = float3_t(g_xState.vWorldMousePos.x, g_xState.vFrameSelectStart.y, 0.0f);
				vc = float3_t(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, 0.0f);
				vd = float3_t(g_xState.vFrameSelectStart.x, g_xState.vWorldMousePos.y, 0.0f);
				break;
			case X2D_SIDE:
				va = float3_t(0.0f, g_xState.vFrameSelectStart.y, g_xState.vFrameSelectStart.x);
				vb = float3_t(0.0f, g_xState.vFrameSelectStart.y, g_xState.vWorldMousePos.x);
				vc = float3_t(0.0f, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
				vd = float3_t(0.0f, g_xState.vWorldMousePos.y, g_xState.vFrameSelectStart.x);
				break;
			}
			XDrawBorder(GX_COLOR_ARGB(255, 255, 255, 0), va, vb, vc, vd, fScale);
		}

		static IGXConstantBuffer *s_mConstWVP = pDevice->createConstantBuffer(sizeof(SMMATRIX));
		if(g_xState.bHasSelection)
		{
			float3_t va, vb, vc, vd;
			float4 vBorder;
			float2 vSelectionCenter;
			switch(view)
			{
			case X2D_TOP:
				va = float3_t(g_xState.vSelectionBoundMin.x, 0.0f, g_xState.vSelectionBoundMin.z);
				vb = float3_t(g_xState.vSelectionBoundMax.x, 0.0f, g_xState.vSelectionBoundMin.z);
				vc = float3_t(g_xState.vSelectionBoundMax.x, 0.0f, g_xState.vSelectionBoundMax.z);
				vd = float3_t(g_xState.vSelectionBoundMin.x, 0.0f, g_xState.vSelectionBoundMax.z);
				vBorder = float4(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.z,
					g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.z);
				vSelectionCenter = float2(g_xState.vSelectionBoundMin.x + g_xState.vSelectionBoundMax.x,
					g_xState.vSelectionBoundMin.z + g_xState.vSelectionBoundMax.z) * 0.5f;
				break;
			case X2D_FRONT:
				va = float3_t(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.y, 0.0f);
				vb = float3_t(g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMin.y, 0.0f);
				vc = float3_t(g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.y, 0.0f);
				vd = float3_t(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMax.y, 0.0f);
				vBorder = float4(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.y,
					g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.y);
				vSelectionCenter = float2(g_xState.vSelectionBoundMin.x + g_xState.vSelectionBoundMax.x,
					g_xState.vSelectionBoundMin.y + g_xState.vSelectionBoundMax.y) * 0.5f;
				break;
			case X2D_SIDE:
				va = float3_t(0.0f, g_xState.vSelectionBoundMin.y, g_xState.vSelectionBoundMin.z);
				vb = float3_t(0.0f, g_xState.vSelectionBoundMin.y, g_xState.vSelectionBoundMax.z);
				vc = float3_t(0.0f, g_xState.vSelectionBoundMax.y, g_xState.vSelectionBoundMax.z);
				vd = float3_t(0.0f, g_xState.vSelectionBoundMax.y, g_xState.vSelectionBoundMin.z);
				vBorder = float4(g_xState.vSelectionBoundMin.z, g_xState.vSelectionBoundMin.y,
					g_xState.vSelectionBoundMax.z, g_xState.vSelectionBoundMax.y);
				vSelectionCenter = float2(g_xState.vSelectionBoundMin.z + g_xState.vSelectionBoundMax.z,
					g_xState.vSelectionBoundMin.y + g_xState.vSelectionBoundMax.y) * 0.5f;
				break;
			}
			XDrawBorder(GX_COLOR_ARGB(255, 255, 0, 0), va, vb, vc, vd, fScale);

			float3_t *pvData;
			float fPtSize = 3.0f * fScale;
			float fPtMargin = 7.0f * fScale;
			if(g_xRenderStates.pTransformHandlerVB->lock((void**)&pvData, GXBL_WRITE))
			{
				UINT uCV = 0;

				if(g_xState.xformType == X2DXF_SCALE)
				{
					float3 vCenter(vSelectionCenter.x, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vSelectionCenter.y);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vSelectionCenter.x, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.x - fPtMargin, 0.0f, vSelectionCenter.y);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.x - fPtMargin, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.x - fPtMargin, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));
				}
				else if(g_xState.xformType == X2DXF_ROTATE)
				{
					float fPtSize2 = fPtSize * sqrtf(2.0f) * 0.5f;

					float3 vCenter = float3(vBorder.x - fPtMargin, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, fPtSize2));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, fPtSize2));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, fPtSize2));

					vCenter = float3(vBorder.x - fPtMargin, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, fPtSize2));
				}

				g_xRenderStates.pTransformHandlerVB->unlock();
			}

			SMMATRIX mViewProj;
			SMMATRIX mWorld;
			Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mViewProj);
			switch(view)
			{
			case X2D_FRONT:
				mWorld = SMMatrixRotationX(-SM_PIDIV2);
				break;
			case X2D_SIDE:
				mWorld = SMMatrixRotationY(-SM_PIDIV2) * SMMatrixRotationZ(-SM_PIDIV2);
				break;
			}
			
			s_mConstWVP->update(&SMMatrixTranspose(mWorld * mViewProj));
			pCtx->setVSConstant(s_mConstWVP, 4);

			IGXRasterizerState *pOldRS = pCtx->getRasterizerState();
			pCtx->setRasterizerState(g_xRenderStates.pRSSolidNoCull);
			pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);
			SGCore_ShaderBind(g_xRenderStates.idColoredShaderKit);
			pCtx->setRenderBuffer(g_xRenderStates.pTransformHandlerRB);
			s_pColorBuffer->update(&float4(1.0f, 1.0f, 1.0f, 1.0f));
			pCtx->setPSConstant(s_pColorBuffer);
			if(g_xState.xformType == X2DXF_SCALE)
			{
				pCtx->setIndexBuffer(g_xRenderStates.pTransformHandlerScaleIB);
				pCtx->drawIndexed(32, 16, 0, 0);
			}
			else if(g_xState.xformType == X2DXF_ROTATE)
			{
				pCtx->setIndexBuffer(g_xRenderStates.pTransformHandlerRotateIB);
				pCtx->drawIndexed(32, 24, 0, 0);
			}


			SGCore_ShaderUnBind();
			pCtx->setRasterizerState(pOldRS);
			mem_release(pOldRS);
		}

		if(g_xState.bCreateMode)
		{
			IGXRasterizerState *pOldRS = pCtx->getRasterizerState();
			pCtx->setRasterizerState(g_xRenderStates.pRSSolidNoCull);
			SMMATRIX mViewProj;
			Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mViewProj);
			pCtx->setVSConstant(s_mConstWVP, 4);
			pCtx->setPSConstant(s_pColorBuffer);
			SGCore_ShaderBind(g_xRenderStates.idColoredShaderKit);

			pCtx->setRenderBuffer(g_xRenderStates.pCreateCrossRB);
			s_mConstWVP->update(&SMMatrixTranspose(SMMatrixTranslation(g_xState.vCreateOrigin) * mViewProj));
			s_pColorBuffer->update(&float4(0.0f, 1.0f, 0.0f, 1.0f));
			pCtx->setPrimitiveTopology(GXPT_LINELIST);
			pCtx->drawPrimitive(0, 3);
			pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

			pCtx->setRasterizerState(pOldRS);
			mem_release(pOldRS);
		}
	}
}

void XLoadLevel(const char *szName)
{
	Core_0ConsoleExecCmd("map %s", szName);	
}

void XResetLevel()
{
	Core_0ConsoleExecCmd("endmap");
}

bool XSaveLevel(const char *szNewName, bool bForcePrompt)
{
	if(szNewName)
	{
		char szPathDirLevel[1024];
		sprintf(szPathDirLevel, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szNewName);

		if(!FileExistsDir(szPathDirLevel))
		{
			FileCreateDir(szPathDirLevel);
		}

		XEventLevel ev;
		ev.type = XEventLevel::TYPE_SAVE;
		ev.szLevelName = szNewName;
		Core_GetIXCore()->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID)->broadcastEvent(&ev);
		return(true);
	}
	
	if(!bForcePrompt && g_sLevelName[0])
	{
		// g_sLevelName can changed during save process, so pointer will be broken
		String tmp = g_sLevelName;
		return(XSaveLevel(tmp.c_str()));
	}
		
	char szName[1024];
	int iMaxLength = sizeof(szName);
	if(Tools::DlgPrompt(szName, &iMaxLength, "Level name", "Save new level"))
	{
		return(XSaveLevel(szName));
	}
	
	return(false);
}

void XDrawBorder(GXCOLOR color, const float3_t &vA, const float3_t &vB, const float3_t &vC, const float3_t &vD, float fViewportScale)
{
	float2_t ta, tb, tc, td;
	if(vA.x == vB.x && vB.x == vC.x)
	{
		ta = float2_t(vA.y, vA.z);
		tb = float2_t(vB.y, vB.z);
		tc = float2_t(vC.y, vC.z);
		td = float2_t(vD.y, vD.z);
	}
	else if(vA.y == vB.y && vB.y == vC.y)
	{
		ta = float2_t(vA.x, vA.z);
		tb = float2_t(vB.x, vB.z);
		tc = float2_t(vC.x, vC.z);
		td = float2_t(vD.x, vD.z);
	}
	else
	{
		ta = float2_t(vA.x, vA.y);
		tb = float2_t(vB.x, vB.y);
		tc = float2_t(vC.x, vC.y);
		td = float2_t(vD.x, vD.y);
	}

	XBorderVertex *pData;
	if(g_pBorderVertexBuffer->lock((void**)&pData, GXBL_WRITE))
	{
		pData[0] = {vA, ta / fViewportScale / 6.0f};
		pData[1] = {vB, tb / fViewportScale / 6.0f};
		pData[2] = {vC, tc / fViewportScale / 6.0f};
		pData[3] = {vD, td / fViewportScale / 6.0f};
		pData[4] = pData[0];
		g_pBorderVertexBuffer->unlock();
	}

	IGXDevice *pDevice = SGCore_GetDXDevice();
	IGXContext *pCtx = pDevice->getThreadContext();

	IGXBlendState *pOldBlendState = pCtx->getBlendState();
	SGCore_ShaderBind(g_xRenderStates.idTexturedShaderKit);

	static IGXConstantBuffer *s_pColorBuffer = pDevice->createConstantBuffer(sizeof(float4));
	s_pColorBuffer->update(&GX_COLOR_COLOR_TO_F4(color));
	pCtx->setPSConstant(s_pColorBuffer);

	pCtx->setPSTexture(g_pDashedMaterial);
	pCtx->setBlendState(g_xRenderStates.pBlendAlpha);
	pCtx->setRenderBuffer(g_pBorderRenderBuffer);
	pCtx->setPrimitiveTopology(GXPT_LINESTRIP);
	pCtx->drawPrimitive(0, 4);
	SGCore_ShaderUnBind();

	pCtx->setBlendState(pOldBlendState);
	mem_release(pOldBlendState);
}

void XUpdateUndoRedo();
bool XExecCommand(CCommand *pCommand)
{
	if(g_pUndoManager->execCommand(pCommand))
	{
		XUpdateUndoRedo();
		return(true);
	}
	return(false);
}

void XUpdateSelectionBound()
{
	g_xState.bHasSelection = false;
	float3 vMin, vMax;

	for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
	{
		if(g_pLevelObjects[i]->isSelected())
		{
			g_pLevelObjects[i]->getBound(&vMin, &vMax);
			if(!g_xState.bHasSelection)
			{
				g_xState.bHasSelection = true;
				g_xState.vSelectionBoundMax = vMax;
				g_xState.vSelectionBoundMin = vMin;
			}
			else
			{
				g_xState.vSelectionBoundMax = (float3)SMVectorMax(g_xState.vSelectionBoundMax, vMax);
				g_xState.vSelectionBoundMin = (float3)SMVectorMin(g_xState.vSelectionBoundMin, vMin);
			}
		}
	}
}

bool XRayCast(X_WINDOW_POS wnd)
{
	float3 vStart, vEnd, vPos, vCamPos;
	if(!g_xConfig.m_pViewportCamera[wnd])
	{
		return(false);
	}
	g_xConfig.m_pViewportCamera[wnd]->getPosition(&vCamPos);
	switch(g_xConfig.m_x2DView[wnd])
	{
	case X2D_NONE:
		return(false);
	case X2D_TOP:
		vStart = float3(g_xState.vWorldMousePos.x, vCamPos.y, g_xState.vWorldMousePos.y);
		vEnd = float3(0.0f, -2000.0f, 0.0f);
		break;
	case X2D_FRONT:
		vStart = float3(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, vCamPos.z);
		vEnd = float3(0.0f, 0.0f, 2000.0f);
		break;
	case X2D_SIDE:
		vStart = float3(vCamPos.x, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
		vEnd = float3(-2000.0f, 0.0f, 0.0f);
		break;
	}
	vEnd += vStart;
	for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
	{
		if(g_pLevelObjects[i]->isSelected() && g_pLevelObjects[i]->rayTest(vStart, vEnd, &vPos, NULL))
		{
			return(true);
		}
	}
	return(false);
}

bool XIsMouseInSelection(X_WINDOW_POS wnd)
{
	if(!g_xConfig.m_pViewportCamera[wnd] || !g_xState.bHasSelection)
	{
		return(false);
	}
	const float2_t &fMPos = g_xState.vWorldMousePos;
	switch(g_xConfig.m_x2DView[wnd])
	{
	case X2D_TOP:
		return(fMPos.x >= g_xState.vSelectionBoundMin.x && fMPos.x <= g_xState.vSelectionBoundMax.x && 
			fMPos.y >= g_xState.vSelectionBoundMin.z && fMPos.y <= g_xState.vSelectionBoundMax.z);

	case X2D_FRONT:
		return(fMPos.x >= g_xState.vSelectionBoundMin.x && fMPos.x <= g_xState.vSelectionBoundMax.x &&
			fMPos.y >= g_xState.vSelectionBoundMin.y && fMPos.y <= g_xState.vSelectionBoundMax.y);

	case X2D_SIDE:
		return(fMPos.x >= g_xState.vSelectionBoundMin.z && fMPos.x <= g_xState.vSelectionBoundMax.z &&
			fMPos.y >= g_xState.vSelectionBoundMin.y && fMPos.y <= g_xState.vSelectionBoundMax.y);
	}
	return(false);
}

void XUpdateWindowTitle()
{
	const char *szLevelName = g_sLevelName.c_str();
	char szCaption[256];
	bool isDirty = g_pUndoManager->isDirty();
	if(szLevelName && szLevelName[0])
	{
		sprintf(szCaption, "%s - [%s]%s | %s", MAIN_WINDOW_TITLE, szLevelName, isDirty ? "*" : "", SKYXENGINE_VERSION4EDITORS);
	}
	else
	{
		sprintf(szCaption, "%s%s | %s", MAIN_WINDOW_TITLE, isDirty ? " - *" : "", SKYXENGINE_VERSION4EDITORS);
	}
	SetWindowText(g_hWndMain, szCaption);
}
