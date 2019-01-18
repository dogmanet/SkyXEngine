
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __ML_DATA_H
#define __ML_DATA_H

#include <gdefines.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <common/array.h>

#include <GRegisterIndex.h>

#include "sxlight.h"

namespace light_data
{
	void Init();

	void ReCalcSize();

	extern IDirect3DDevice9 *pDXDevice;
	
	//размер текстуры глубины дл¤ локальных источников света
	extern float2_t vSizeTexDepthGlobal;

	//размер текстуры глубины дл¤ локальных источников света
	extern float2_t vSizeTexDepthLocal;

	//дистанции дл¤ 4 сплитов дл¤ глоальных теней PSSM
	extern float4_t vDistForPSSM;

	//коэфициент размера текстур дл¤ карт глубин локальных источников света
	extern float fCoefSizeDepthMapForLocal;

	//коэфициент размера текстур дл¤ карт глубин глобального источника света
	extern float fCoefSizeDepthMapForGlobal;

	extern float2_t vSizeTexReflection;

	//ориентаци¤ и верхний вектор дл¤ рендера в кубическую текстуру
	extern float3 vOrientedCube[6];
	extern float3 vUpVectorsCube[6];

	extern float4x4 mRefProjPlane;
	extern float4x4 mRefProjCube;

	namespace shader_id
	{
		namespace vs
		{
			extern ID idResPosDepth;
			
			extern ID idScreenOut;

			extern ID idSMDepthGeomPSSM;
			extern ID idSMDepthGeomDirect;
			extern ID idSMDepthGeomCube;

			extern ID idSMDepthGrassPSSM;
			extern ID idSMDepthGrassDirect;
			extern ID idSMDepthGrassCube;

			extern ID idSMDepthTreePSSM;
			extern ID idSMDepthTreeDirect;
			extern ID idSMDepthTreeCube;

			extern ID idSMDepthSkinPSSM;
			extern ID idSMDepthSkinDirect;
			extern ID idSMDepthSkinCube;

			extern ID idStdGeom;
			extern ID idStdTree;
			extern ID idStdGrass;
			extern ID idStdSkin;
		};

		namespace ps
		{
			extern ID idSMDepthGeomPSSM;
			extern ID idSMDepthGeomDirect;
			extern ID idSMDepthGeomCube;

			extern ID idSMDepthGreenPSSM;
			extern ID idSMDepthGreenDirect;
			extern ID idSMDepthGreenCube;

			extern ID idSMDepthSkinPSSM;
			extern ID idSMDepthSkinDirect;
			extern ID idSMDepthSkinCube;

			extern ID idPPBlurDepthBasedNoise;
			extern ID idPSSM4;
			extern	ID idPSSM3;
			extern ID idPPBlurDepthBased;

			extern ID idGenShadowDirect;
			extern ID idGenShadowCube;

			extern ID idScreenOut;

			extern ID idStdGeom;
			extern ID idStdGeomCP;
			extern ID idStdGreen;
			extern ID idStdGreenCP;
			extern ID idStdSkin;
			extern ID idStdSkinCP;
		};
	};

	namespace texture_id
	{
		extern ID idNoiseTex;
		extern ID idSurfaceDepthGlobalShadow;
		extern ID idSurfaceDepthPointShadow;
		extern ID idSurfaceDepthDirShadow;
	};
};

#endif