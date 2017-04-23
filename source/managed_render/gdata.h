
#ifndef __gdata_h
#define __gdata_h

#include <managed_render/model_sim.h>

namespace GData
{
	IDirect3DDevice9* DXDevice = 0;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void InitWin(const char* name);
	HWND Handle3D = 0;
	float2_t WinSize = float2_t(800, 600);
	ISXCamera* ObjCamera = 0;
	ModelSim* SimModel = 0;
	int ReSize = 0;	//0 - ничего не мен€ли, 1 - ресайз, 2 - переходмежду фулскрин и окном
	float2_t NearFar = float2_t(0.25,400);
	bool IsWindowed = true;
	float ProjFov = SM_PI * 0.25f;

	ID DefaultGeomIDArr = -1;
	ID DefaultGreenIDArr = -1;
	ID DefaultAnimIDArr = -1;
	
	float3 ConstCurrCamPos;	//позици€ камеры
	float3 ConstCurrCamDir;	//направление взгл€да камеры

	float4_t CamWalkParamEditor = float4_t(
		10.f,	//простое движенеи вперед
		5.f,	//коэфициент ускорени€
		0.7f,	//коэфициент от основного движени€ в стороны 
		0.5f	//коэфициент от основного движени€ назад
		);

	//дальн€€ плоскость отсечени€, необходима дл€ корректного отсечени€ по фрустуму камеры и освещени€
	D3DXPLANE PlaneZCulling;
	D3DXPLANE PlaneZCullingShader;

	//матрицы
	void InitAllMatrix();
	float4x4 MCamView;			//матрица вида камеры
	float4x4 MCamProj;			//матрица проекции камеры
	float4x4 MLightProj;		//матрица проекции аналогична€ камере, только дальна€€ плоскость дальше
	float4x4 MRefPlaneSkyProj;	//матрица проекции дл€ рендера skybox и sky clouds, дл€ плоских отражений, аналогична MCamProj
	float4x4 MRefCubeSkyProj;	//матрица проекции дл€ рендера skybox и sky clouds, дл€ куибческих отражений

	//////

	//пространство имен с пут€ми
	namespace Pathes
	{
		void InitAllPathes();

		char ForExe[1024];
		char WorkingTex[1024];
		char GameSource[1024];
		char Screenshots[1024];
		char Configs[1024];
		char Levels[1024];
		char Meshes[1024];
		char Models[1024];
		char Scripts[1024];
		char Materials[1024];
		char GUIresources[1024];
		char Sounds[1024];
		char Shaders[1024];
		char Textures[1024];
			char TexturesDetail[1024];
			char TexturesSkyBoxes[1024];
	};

	//пространство имен с идентификаторами шейдеров
	namespace IDsShaders
	{
		void InitAllShaders();
		//вершинные шейдеры
		namespace VS
		{
			ID ScreenOut;
			ID ResPos;
		};

		//пиксельные шейдеры
		namespace PS
		{
			ID ScreenOut;
			ID ComLightingNonShadow;
			ID ComLightingShadow;

			ID BlendAmbientSpecDiffColor;

			ID StencilStr;
			ID StencilColumn;
			ID StencilStrColumn;
			ID UnionAlpha;
			ID SmoothingAlpha;
		};
	};

	namespace IDSRenderTargets
	{

	};
};

#include <managed_render/model_sim.cpp>

#endif