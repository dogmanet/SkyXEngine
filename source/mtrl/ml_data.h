
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

#include "sxmtrl.h"

namespace MLSet
{
	void MLInit();

	extern IDirect3DDevice9* DXDevice;

	extern float2_t SizeTexReflection;

	//ориентаци¤ и верхний вектор дл¤ рендера в кубическую текстуру
	extern float3 OrientedCube[6];
	extern float3 UpVectorsCube[6];

	extern float4x4 RefMProjPlane;
	extern float4x4 RefMProjCube;

	namespace IDsShaders
	{
		namespace VS
		{
			extern ID ResPosDepth;
			
			extern ID ScreenOut;

			extern ID StdGeom;
			extern ID StdTree;
			extern ID StdGrass;
			extern ID StdSkin;
		};

		namespace PS
		{
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

	namespace IDsTexs
	{
		extern ID Tex_NoiseTex;
	};
};

#endif