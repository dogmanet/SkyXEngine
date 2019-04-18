
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxrender.h"

#include <render/camera_update.h>
#include <render/render_func.h>

#include "RenderPipeline.h"

#define SXRENDER_VERSION 1

//#########################################################################

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

#define SR_PRECOND(retval) if(!gdata::pDXDevice){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxrender is not init", GEN_MSG_LOCATION); return retval;}

CRenderPipeline *g_pPipeline = NULL;

//##########################################################################

SX_LIB_API long SRender_0GetVersion()
{
	return SXRENDER_VERSION;
}

SX_LIB_API void SRender_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SRender_0Create(const char *szName, HWND hWnd3D, HWND hWndParent3D, bool isUnic)
{
	if (szName && strlen(szName) > 1)
	{
		if (isUnic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, szName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
				return;
			}
		}
		
		gdata::hHandle3D = hWnd3D;
		gdata::hHandleParent3D = hWndParent3D;

		gdata::pDXDevice = SGCore_GetDXDevice();
//		gdata::pDXDevice->GetDeviceCaps(&gdata::dxDeviceCaps);

		gdata::pCamera = SGCore_CrCamera();
		gdata::pCamera->setFOV(gdata::fProjFov);

		gdata::InitAllMatrix();

		gdata::shaders_id::InitAllShaders();

		//**********************

		gdata::Editors::pSimModel = new CSimulationModel();

		gdata::Editors::pGrid = new CGrid();
		gdata::Editors::pGrid->create(100, 100, GXCOLOR_ARGB(255, 200, 200, 200));

		gdata::Editors::pAxesStatic = new CAxesStatic();
		gdata::Editors::pAxesStatic->create(1);

		gdata::Editors::canRenderGrid = gdata::Editors::canRenderAxesStatic = false;

		/*gdata::Editors::RenderBoundBox = gdata::Editors::RenderBoundSphere = gdata::Editors::RenderBoundCone = false;

		D3DXCreateBox(gdata::pDXDevice, 1, 1, 1, &gdata::Editors::FigureBox, 0);
		D3DXCreateSphere(gdata::pDXDevice, 1, 20, 20, &gdata::Editors::FigureSphere, 0);

		gdata::Editors::vFigureConeParam.x = 1;
		gdata::Editors::vFigureConeParam.y = 0.1;
		gdata::Editors::vFigureConeParam.z = 1;

		SGCore_FCreateCone(gdata::Editors::vFigureConeParam.x, gdata::Editors::vFigureConeParam.y, gdata::Editors::vFigureConeParam.z, &gdata::Editors::FigureCone, 20);
		*/
		//***********************

		uint32_t tmpColor = GXCOLOR_ARGB(255, 255, 255, 0);

		IGXTexture2D* SelectMaterial = SGCore_GetDXDevice()->createTexture2D(1, 1, 1, 0, GXFMT_A8R8G8B8, &tmpColor);
		
		//SGCore_LoadTexLoadTextures();
		gdata::idSelectTex = SGCore_LoadTexCreate("select_material__", SelectMaterial);

		tmpColor = GXCOLOR_ARGB(255, 255, 255, 255);
		IGXTexture2D* pWhiteMaterial = SGCore_GetDXDevice()->createTexture2D(1, 1, 1, 0, GXFMT_A8R8G8B8, &tmpColor);
		gdata::idWhiteTex = SGCore_LoadTexCreate("white_material__", pWhiteMaterial);
		//SGCore_LoadTexLoadTextures();

		GXCOLOR w = GXCOLOR_ARGB(255, 255, 255, 255);
		GXCOLOR t = GXCOLOR_ARGB(0, 255, 255, 255);
		GXCOLOR colorData[] = {
			t, t, t, w, w, w,
			w, t, t, t, w, w,
			w, w, t, t, t, w,
			w, w, w, t, t, t,
			t, w, w, w, t, t,
			t, t, w, w, w, t
		};
		IGXTexture2D* pDashedMaterial = SGCore_GetDXDevice()->createTexture2D(6, 6, 1, 0, GXFMT_A8R8G8B8, colorData);
		gdata::idDashedTex = SGCore_LoadTexCreate("dashed_material__", pDashedMaterial);

		g_pPipeline = new CRenderPipeline(SGCore_GetDXDevice());
		Core_GetIXCore()->setRenderPipeline(g_pPipeline);

	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SRender_AKill()
{
	SR_PRECOND(_VOID);

	mem_delete(g_pPipeline);

	/*mem_delete(gdata::Editors::SimModel);

	mem_delete(gdata::Editors::ObjGrid);
	mem_delete(gdata::Editors::ObjAxesStatic);
	mem_release(gdata::Editors::FigureBox);
	mem_release(gdata::Editors::FigureSphere);
	mem_release(gdata::Editors::FigureCone);
	mem_delete(gdata::Editors::ObjAxesHelper);*/

	mem_release(gdata::pCamera);
}

//##########################################################################

SX_LIB_API HWND SRender_GetHandleWin3D()
{
	return gdata::hHandle3D;
}

SX_LIB_API HWND SRender_GetParentHandleWin3D()
{
	return gdata::hHandleParent3D;
}


SX_LIB_API void SRender_SetHandleWin3D(HWND hWnd)
{
	gdata::hHandle3D = hWnd;
}

SX_LIB_API void SRender_SetParentHandleWin3D(HWND hWnd)
{
	gdata::hHandleParent3D = hWnd;
}

SX_LIB_API void SRender_SetCamera(ICamera *pCamera)
{
	gdata::pCamera = pCamera;
}

SX_LIB_API ICamera* SRender_GetCamera()
{
	return gdata::pCamera;
}

SX_LIB_API void SRender_EditorCameraSetMove(bool canMove)
{
	gdata::Editors::canMoveCamera = canMove;
}

SX_LIB_API bool SRender_EditorCameraGetMove()
{
	return gdata::Editors::canMoveCamera;
}

//**************************************************************************

SX_LIB_API void SRender_SimModelAddModel(ISXDataStaticModel *pModel)
{
	gdata::Editors::pSimModel->add(pModel);
}

SX_LIB_API ID SRender_SimModelGetIDMtl()
{
	return gdata::Editors::pSimModel->getIdMtl();
}

SX_LIB_API MTLTYPE_MODEL SRender_SimModelGetType()
{
	return gdata::Editors::pSimModel->m_typeModel;
}

SX_LIB_API void SRender_SimModelSetType(MTLTYPE_MODEL type)
{
	gdata::Editors::pSimModel->m_typeModel = type;
}

SX_LIB_API void SRender_SimModelSetNumCurrModel(int iCurrNumModel)
{
	gdata::Editors::pSimModel->m_iCurrRenderModel = iCurrNumModel;
}

SX_LIB_API int SRender_SimModelGetNumCurrModel()
{
	return gdata::Editors::pSimModel->m_iCurrRenderModel;
}

SX_LIB_API void SRender_SimModelSetRotationY(float Rotation)
{
	gdata::Editors::pSimModel->m_vRotation.y = Rotation;
}

SX_LIB_API float SRender_SimModelGetRotationY()
{
	return gdata::Editors::pSimModel->m_vRotation.y;
}

//##########################################################################

SX_LIB_API void SRender_CentererCursor()
{
	CameraUpdate::CentererCursor();
}

SX_LIB_API void SRender_UpdateEditorial(DWORD timeDelta)
{
	CameraUpdate::UpdateEditorial(timeDelta);
}

//**************************************************************************

SX_LIB_API void SRender_EditorSetRenderGrid(bool canRender)
{
	gdata::Editors::canRenderGrid = canRender;
}

SX_LIB_API bool SRender_EditorGetRenderGrid()
{
	return gdata::Editors::canRenderGrid;
}


SX_LIB_API void SRender_EditorSetRenderAxesStatic(bool canRender)
{
	gdata::Editors::canRenderAxesStatic = canRender;
}

SX_LIB_API bool SRender_EditorGetRenderAxesStatic()
{
	return gdata::Editors::canRenderAxesStatic;
}

SX_LIB_API ID SRender_EditorGetSelectTex()
{
	return gdata::idSelectTex;
}

SX_LIB_API ID SRender_EditorGetWhiteTex()
{
	return gdata::idWhiteTex;
}

SX_LIB_API ID SRender_EditorGetDashedTex()
{
	return gdata::idDashedTex;
}

//##########################################################################


SX_LIB_API bool SRender_ComDeviceLost(bool isSetWindowSize)
{
	return(rfunc::ComDeviceLost(isSetWindowSize));
}

SX_LIB_API void SRender_ComVisibleForLight()
{
	rfunc::ComVisibleForLight();
}

SX_LIB_API void SRender_ComVisibleForCamera()
{
	rfunc::ComVisibleForCamera();
}

SX_LIB_API void SRender_ComVisibleReflection()
{
	rfunc::ComVisibleReflection();
}


SX_LIB_API void SRender_UpdateView()
{
	rfunc::UpdateView();
}

SX_LIB_API int SRender_OutputDebugInfo(DWORD timeDelta, bool needGameTime, const char *szStr)
{
	return rfunc::OutputDebugInfo(timeDelta, needGameTime, szStr);
}


SX_LIB_API void SRender_BuildMRT(DWORD timeDelta, bool isRenderSimulation)
{
	rfunc::BuildMRT(timeDelta, isRenderSimulation);
}

SX_LIB_API void SRender_UpdateShadow(DWORD timeDelta)
{
	rfunc::UpdateShadow(timeDelta);
}

SX_LIB_API void SRender_UpdateReflection(DWORD timeDelta, bool isRenderSimulation)
{
	rfunc::UpdateReflection(timeDelta, isRenderSimulation);
}

SX_LIB_API void SRender_UpdateReflectionScene(DWORD timeDelta)
{
	rfunc::UpdateReflectionScene(timeDelta);
}

SX_LIB_API void SRender_UpdateReflectionSimModel(DWORD timeDelta)
{
	rfunc::UpdateReflectionSimModel(timeDelta);
}

SX_LIB_API void SRender_RenderSky(DWORD timeDelta)
{
	rfunc::RenderSky(timeDelta);
}

SX_LIB_API void SRender_ComLighting(DWORD timeDelta)
{
	rfunc::ComLighting(timeDelta);
}

SX_LIB_API void SRender_UnionLayers()
{
	rfunc::UnionLayers();
}


SX_LIB_API void SRender_RenderParticles(DWORD timeDelta)
{
	rfunc::RenderParticles(timeDelta);
}

SX_LIB_API void SRender_RenderMainPostProcess(DWORD timeDelta)
{
	rfunc::RenderMainPostProcess(timeDelta);
}

SX_LIB_API void SRender_RenderFinalPostProcess(DWORD timeDelta)
{
	rfunc::RenderFinalPostProcess(timeDelta);
}

SX_LIB_API void SRender_ShaderRegisterData()
{
	rfunc::ShaderRegisterData();
}

SX_LIB_API void SRender_RenderEditorMain()
{
	rfunc::RenderEditorMain();
}

//##########################################################################


SX_LIB_API void SRender_SaveScreenShot()
{
	rfunc::SaveScreenShot();
}

SX_LIB_API void SRender_SaveWorkTex()
{
	rfunc::SaveWorkTex();
}

SX_LIB_API void SRender_ChangeModeWindow()
{
	//LibReport(REPORT_MSG_LEVEL_NOTICE, "SRender_ChangeModeWindow\n");
	rfunc::ChangeModeWindow();
	//LibReport(REPORT_MSG_LEVEL_NOTICE, "SRender_ChangeModeWindow --\n");
}

SX_LIB_API void SRender_FullScreenChangeSizeAbs()
{
	rfunc::FullScreenChangeSizeAbs();
}