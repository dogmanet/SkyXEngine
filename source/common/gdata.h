
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
	float4_t CamUpdateWalkParam = float4_t(
		10.f,	//простое движенеи вперед
		3.f,	//ускорение
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
			DWORD FreeGeometry;
			DWORD ScreenOut;
			DWORD ResPos;
		};

		namespace PS
		{
			DWORD FreeGeometry;
			DWORD ScreenOut;
			DWORD ComLightingNonShadow;
			DWORD BlendAmbientSpecDiffcolor;
			DWORD CalcAdaptedLum;
			DWORD SampleLumInit;
			DWORD SampleLumIterative;
			DWORD SampleLumFinal;
			DWORD FinalHRDL;
		};
	};

	namespace IDSRenderTargets
	{
		DWORD ColorScene;
		DWORD NormalScene;
		DWORD DepthScene;
		DWORD ParamsScene;

		DWORD ToneMaps[4];
		DWORD AdaptLumCurr;
		DWORD AdaptLumLast;
		DWORD LightAmbient;
		DWORD LightSpecDiff;
		DWORD LightSpecDiffD2;
		DWORD LigthCom;
		DWORD LigthCom2;
		DWORD LigthComScaled;
		DWORD LigthCom_1x1;
		DWORD LigthComD2;
		DWORD LigthComD4;
		DWORD LigthComD8;
	};

	//////

	//StaticGeom* Geometry = 0;
	//DWORD IDShaderVSRenderGreenTree = -1;
	//DWORD IDShaderVSRenderGreenGrass = -1;
	//DWORD IDShaderPSRenderGreenTree = -1;
	//Green* StaticGreen = 0;
	
	IDirect3DSurface9* ComLightSurf1x1;
};

#endif