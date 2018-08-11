
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxrender.h"

#include <render/camera_update.h>
#include <render/render_func.h>

#define SXRENDER_VERSION 1

//#########################################################################

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

#define SR_PRECOND(retval) if(!GData::DXDevice){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxrender is not init", GEN_MSG_LOCATION); return retval;}

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
		
		GData::Handle3D = hWnd3D;
		GData::HandleParent3D = hWndParent3D;

		GData::DXDevice = SGCore_GetDXDevice();

		GData::ObjCamera = SGCore_CrCamera();
		GData::ObjCamera->setFOV(GData::ProjFov);

		GData::InitAllMatrix();

		GData::IDsShaders::InitAllShaders();

		//**********************

		GData::Editors::SimModel = new ModelSim();

		GData::Editors::ObjGrid = new CGrid();
		GData::Editors::ObjGrid->create(100, 100, D3DCOLOR_ARGB(255, 200, 200, 200));

		GData::Editors::ObjAxesStatic = new CAxesStatic();
		GData::Editors::ObjAxesStatic->create(1);

		GData::Editors::RenderGrid = GData::Editors::RenderAxesStatic = false;

		/*GData::Editors::RenderBoundBox = GData::Editors::RenderBoundSphere = GData::Editors::RenderBoundCone = false;

		D3DXCreateBox(GData::DXDevice, 1, 1, 1, &GData::Editors::FigureBox, 0);
		D3DXCreateSphere(GData::DXDevice, 1, 20, 20, &GData::Editors::FigureSphere, 0);

		GData::Editors::vFigureConeParam.x = 1;
		GData::Editors::vFigureConeParam.y = 0.1;
		GData::Editors::vFigureConeParam.z = 1;

		SGCore_FCreateCone(GData::Editors::vFigureConeParam.x, GData::Editors::vFigureConeParam.y, GData::Editors::vFigureConeParam.z, &GData::Editors::FigureCone, 20);
		*/
		//***********************

		IDirect3DTexture9* SelectMaterial;
		SGCore_GetDXDevice()->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &SelectMaterial, NULL);
		D3DLOCKED_RECT LockedRect;
		uint32_t tmpColor = D3DCOLOR_ARGB(255, 255, 0, 255);

		SelectMaterial->LockRect(0, &LockedRect, 0, 0);

		uint32_t* tmpOldColor = (uint32_t*)LockedRect.pBits + 0 * LockedRect.Pitch + 0 * sizeof(uint32_t);
		memcpy(tmpOldColor, &tmpColor, sizeof(uint32_t));

		SelectMaterial->UnlockRect(0);

		//SGCore_LoadTexLoadTextures();
		GData::IDSelectTex = SGCore_LoadTexCreate("select_material__", SelectMaterial);

		//SGCore_LoadTexLoadTextures();
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SRender_AKill()
{
	SR_PRECOND(_VOID);

	/*mem_delete(GData::Editors::SimModel);

	mem_delete(GData::Editors::ObjGrid);
	mem_delete(GData::Editors::ObjAxesStatic);
	mem_release(GData::Editors::FigureBox);
	mem_release(GData::Editors::FigureSphere);
	mem_release(GData::Editors::FigureCone);
	mem_delete(GData::Editors::ObjAxesHelper);*/

	mem_release(GData::ObjCamera);
}

//##########################################################################

SX_LIB_API HWND SRender_GetHandleWin3D()
{
	return GData::Handle3D;
}

SX_LIB_API HWND SRender_GetParentHandleWin3D()
{
	return GData::HandleParent3D;
}


SX_LIB_API void SRender_SetHandleWin3D(HWND hWnd)
{
	GData::Handle3D = hWnd;
}

SX_LIB_API void SRender_SetParentHandleWin3D(HWND hWnd)
{
	GData::HandleParent3D = hWnd;
}

SX_LIB_API void SRender_SetCamera(ICamera *pCamera)
{
	GData::ObjCamera = pCamera;
}

SX_LIB_API ICamera* SRender_GetCamera()
{
	return GData::ObjCamera;
}

SX_LIB_API void SRender_EditorCameraSetMove(bool canMove)
{
	GData::Editors::MoveCamera = canMove;
}

SX_LIB_API bool SRender_EditorCameraGetMove()
{
	return GData::Editors::MoveCamera;
}

//**************************************************************************

/*SX_LIB_API void SRender_SimModelSetPosition(const float3 *pPos)
{

}*/

SX_LIB_API void SRender_SimModelAdd(const char *szName)
{
	GData::Editors::SimModel->Add(szName);
}

SX_LIB_API ID SRender_SimModelGetIDMtl()
{
	return GData::Editors::SimModel->GetIDMtl();
}

SX_LIB_API MTLTYPE_MODEL SRender_SimModelGetType()
{
	return GData::Editors::SimModel->TypeModel;
}

SX_LIB_API void SRender_SimModelSetType(MTLTYPE_MODEL type)
{
	GData::Editors::SimModel->TypeModel = type;
}

SX_LIB_API void SRender_SimModelSetNumCurrModel(int iCurrNumModel)
{
	GData::Editors::SimModel->CurrRenderModel = iCurrNumModel;
}

SX_LIB_API int SRender_SimModelGetNumCurrModel()
{
	return GData::Editors::SimModel->CurrRenderModel;
}

SX_LIB_API void SRender_SimModelSetRotationY(float Rotation)
{
	GData::Editors::SimModel->Rotation.y = Rotation;
}

SX_LIB_API float SRender_SimModelGetRotationY()
{
	return GData::Editors::SimModel->Rotation.y;
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
	GData::Editors::RenderGrid = canRender;
}

SX_LIB_API bool SRender_EditorGetRenderGrid()
{
	return GData::Editors::RenderGrid;
}


SX_LIB_API void SRender_EditorSetRenderAxesStatic(bool canRender)
{
	GData::Editors::RenderAxesStatic = canRender;
}

SX_LIB_API bool SRender_EditorGetRenderAxesStatic()
{
	return GData::Editors::RenderAxesStatic;
}

SX_LIB_API ID SRender_EditorGetSelectTex()
{
	return GData::IDSelectTex;
}

//##########################################################################


SX_LIB_API void SRender_ComDeviceLost(bool isSetWindowSize)
{
	SXRenderFunc::ComDeviceLost(isSetWindowSize);
}

SX_LIB_API void SRender_ComVisibleForLight()
{
	SXRenderFunc::ComVisibleForLight();
}

SX_LIB_API void SRender_ComVisibleForCamera()
{
	SXRenderFunc::ComVisibleForCamera();
}

SX_LIB_API void SRender_ComVisibleReflection()
{
	SXRenderFunc::ComVisibleReflection();
}


SX_LIB_API void SRender_UpdateView()
{
	SXRenderFunc::UpdateView();
}

SX_LIB_API int SRender_OutputDebugInfo(DWORD timeDelta, bool needGameTime, const char *szStr)
{
	return SXRenderFunc::OutputDebugInfo(timeDelta, needGameTime, szStr);
}


SX_LIB_API void SRender_BuildMRT(DWORD timeDelta, bool isRenderSimulation)
{
	SXRenderFunc::BuildMRT(timeDelta, isRenderSimulation);
}

SX_LIB_API void SRender_UpdateShadow(DWORD timeDelta)
{
	SXRenderFunc::UpdateShadow(timeDelta);
}

SX_LIB_API void SRender_UpdateReflection(DWORD timeDelta, bool isRenderSimulation)
{
	SXRenderFunc::UpdateReflection(timeDelta, isRenderSimulation);
}

SX_LIB_API void SRender_UpdateReflectionScene(DWORD timeDelta)
{
	SXRenderFunc::UpdateReflectionScene(timeDelta);
}

SX_LIB_API void SRender_UpdateReflectionSimModel(DWORD timeDelta)
{
	SXRenderFunc::UpdateReflectionSimModel(timeDelta);
}

SX_LIB_API void SRender_RenderSky(DWORD timeDelta)
{
	SXRenderFunc::RenderSky(timeDelta);
}

SX_LIB_API void SRender_ComLighting(DWORD timeDelta)
{
	SXRenderFunc::ComLighting(timeDelta);
}

SX_LIB_API void SRender_UnionLayers()
{
	SXRenderFunc::UnionLayers();
}

SX_LIB_API void SRender_ComToneMapping(DWORD timeDelta)
{
	SXRenderFunc::ComToneMapping(timeDelta);
}


SX_LIB_API void SRender_RenderParticles(DWORD timeDelta)
{
	SXRenderFunc::RenderParticles(timeDelta);
}

SX_LIB_API void SRender_RenderPostProcess(DWORD timeDelta)
{
	SXRenderFunc::RenderPostProcess(timeDelta);
}

SX_LIB_API void SRender_ShaderRegisterData()
{
	SXRenderFunc::ShaderRegisterData();
}

SX_LIB_API void SRender_RenderEditorMain()
{
	SXRenderFunc::RenderEditorMain();
}

//##########################################################################


SX_LIB_API void SRender_SaveScreenShot()
{
	SXRenderFunc::SaveScreenShot();
}

SX_LIB_API void SRender_SaveWorkTex()
{
	SXRenderFunc::SaveWorkTex();
}

SX_LIB_API void SRender_ChangeModeWindow()
{
	//LibReport(REPORT_MSG_LEVEL_NOTICE, "SRender_ChangeModeWindow\n");
	SXRenderFunc::ChangeModeWindow();
	//LibReport(REPORT_MSG_LEVEL_NOTICE, "SRender_ChangeModeWindow --\n");
}

SX_LIB_API void SRender_FullScreenChangeSizeAbs()
{
	SXRenderFunc::FullScreenChangeSizeAbs();
}