
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __ML_DATA_H
#define __ML_DATA_H

#include <gdefines.h>

//#define SM_D3D_CONVERSIONS
#include <common/Math.h>
#include <common/array.h>

#include <GRegisterIndex.h>

#include "sxmtrl.h"

namespace mtrl_data
{
	void Init();

	extern IGXContext *pDXDevice;

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

			extern ID idStdGeom;
			extern ID idStdTree;
			extern ID idStdGrass;
			extern ID idStdSkin;
		};

		namespace ps
		{
			extern ID idCalcAdaptedLum;
			extern ID idSampleLumInit;
			extern ID idSampleLumIterative;

			extern ID idScreenOut;

			extern ID idStdGeom;
			extern ID idStdGeomCP;
			extern ID idStdGreen;
			extern ID idStdGreenCP;
			extern ID idStdSkin;
			extern ID idStdSkinCP;
		};
	};
};

#endif