
#ifndef __ML_DATA_H
#define __ML_DATA_H

#pragma once

#include <gdefines.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <common\array.h>
#include <GRegisterIndex.h>
#include "sxmtllight.h"

namespace MLSet
{
	void MLInit();
	extern IDirect3DDevice9* DXDevice;
	//float3 ConstCurrCamPos;
	//float2_t WinSize = float2_t(1024, 768);
	//DWORD CountTimeDelta = 0;
	//char StdPathMaterial[1024];
	//char StdPathMesh[1024];
	
	//размер текстуры глубины дл¤ локальных источников света
	extern float2_t SizeTexDepthGlobal;

	//размер текстуры глубины дл¤ локальных источников света
	extern float2_t SizeTexDepthLocal;

	//дистанции дл¤ 4 сплитов дл¤ глоальных теней PSSM
	extern float4_t DistForPSSM;

	//коэфициент размера текстур дл¤ карт глубин локальных источников света
	extern float CoefSizeDepthMapForLocal;

	//коэфициент размера текстур дл¤ карт глубин глобального источника света
	extern float CoefSizeDepthMapForGlobal;

	extern float2_t SizeTexReflection;

	void ReCalcSize();

	void GetArrDownScale4x4(DWORD width, DWORD height, float2 arr[]);

	//fov and ration esesno
	//float ProjFov = SM_PI * 0.25f;
	//float ProjRatio = WinSize.x / WinSize.y;

	//ближн¤¤ и дальн¤¤ плоскости
	//float2_t NearFar = float2_t(0.25f, 400.f);
	
	//float4x4 MCamView;

	extern bool IsHalfGenPCFShadowLocal;

	//ориентаци¤ и верхний вектор дл¤ рендера в кубическую текстуру
	extern float3 OrientedCube[6];
	extern float3 UpVectorsCube[6];

	extern float2 HDRSampleOffsets[16];

	extern float4x4 RefMProjPlane;
	extern float4x4 RefMProjCube;

	namespace IDsShaders
	{
		namespace VS
		{
			extern ID ResPosDepth;
			
			extern ID ScreenOut;

			extern ID SMDepthGeomPSSMDirect;
			extern ID SMDepthGeomCube;

			extern ID SMDepthGrassPSSMDirect;
			extern ID SMDepthGrassCube;

			extern ID SMDepthTreePSSMDirect;
			extern ID SMDepthTreeCube;

			extern ID SMDepthSkinPSSMDirect;
			extern ID SMDepthSkinCube;

			extern ID StdGeom;
			extern ID StdTree;
			extern ID StdGrass;
			extern ID StdSkin;
		};

		namespace PS
		{
			extern ID SMDepthGeomPSSMDirect;
			extern ID SMDepthGeomCube;

			extern ID SMDepthGreenPSSMDirect;
			extern ID SMDepthGreenCube;

			extern ID SMDepthSkinPSSMDirect;
			extern ID SMDepthSkinCube;

			extern ID PPBlurDepthBasedNoise;
			extern ID PSSM4;
			extern	ID PSSM3;
			extern ID PPBlurDepthBased;
			extern ID GenShadowDirect4;
			extern ID GenShadowDirect9;
			
			extern ID GenShadowCube1;
			extern ID GenShadowCube6;

			extern ID CalcAdaptedLum;
			extern ID SampleLumInit;
			extern ID SampleLumIterative;

			extern ID ScreenOut;

			extern ID StdGeom;
			extern ID StdGeomCP;
			extern ID StdGreen;
			extern ID StdGreenCP;
			extern ID StdSkin;
			extern ID StdSkinCP;
		};
	};

	namespace IDsRenderTargets
	{
		extern ID DSComLight;

		extern ID ColorScene;//цвет (текстуры)
		extern ID NormalScene;//номрали + микрорельеф
		extern ID ParamsScene;//параметры освещени¤
		extern ID DepthScene;
		extern ID DepthScene0;
		extern ID DepthScene1;

		extern ID LightAmbientDiff;
		extern ID LightSpecular;

		extern Array<ID> ToneMaps;
		extern Array<LPDIRECT3DSURFACE9> SurfToneMap;
		extern int CountArrToneMaps;
		////
		extern ID AdaptLumCurr;
		extern ID AdaptLumLast;

		extern int HowAdaptedLum;
		ID GetCurrAdaptedLum();
		ID GetLastAdaptedLum();
		void IncrAdaptedLum();
		////

		extern ID LigthCom;
		extern ID LigthCom2;
		extern ID LigthCom3;

		extern ID LigthComScaled;
	};

	namespace IDsTexs
	{
		extern ID Tex_NoiseTex;
		extern ID ParamLight;
		extern ID NullMaterial;
		extern ID NullingTex;
	};
};

#endif