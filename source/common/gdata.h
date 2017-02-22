
#ifndef __gdata_h
#define __gdata_h

namespace GData
{
	IDirect3DDevice9* DXDevice = 0;
	HWND Handle3D = 0;
	float2_t WinSize = float2_t(1024, 768);
	ISXCamera* ObjCamera = 0;
	bool ReSize = false;
	float2_t NearFar = float2_t(0.25,400);
	bool IsWindowed = true;
	float ProjFov = D3DX_PI * 0.25f;
	float ProjRatio = WinSize.x / WinSize.y;
	
	float3 ConstCurrCamPos;
	float3 ConstCurrCamDir;
	float4_t CamWalkParamEditor = float4_t(
		10.f,	//простое движенеи вперед
		5.f,	//коэфициент ускорени€
		0.7f,	//коэфициент от основного движени€ в стороны 
		0.5f	//коэфициент от основного движени€ назад
		);

	//дальн€€ плоскость отсечени€, необходима дл€ корректного отсечени€ по фрустуму камеры и освещени€
	D3DXPLANE PlaneZCulling;
	D3DXPLANE PlaneZCullingShader;

	float4x4 MCamView;
	float4x4 MCamProj;
	float4x4 MLightProj;

	//////

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

	namespace IDsShaders
	{
		namespace VS
		{
			ID ScreenOut;
			ID ResPos;
		};

		namespace PS
		{
			ID ScreenOut;
			ID ComLightingNonShadow;
			ID ComLightingShadow;
			ID BlendAmbientSpecDiffcolor;
		};
	};

	namespace IDSRenderTargets
	{

	};
};

#endif