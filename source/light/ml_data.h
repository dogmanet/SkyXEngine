
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

#include "sxlight.h"

namespace light_data
{
	void Init();

	extern IGXDevice *pDXDevice;
	
	extern IGXSamplerState *pSamplerPointClamp;
	extern IGXSamplerState *pSamplerLinearClamp;
	extern IGXSamplerState *pSamplerPointWrap;

	namespace shader_id
	{
		namespace vs
		{
			extern ID idResPosDepth;
			
			extern ID idScreenOut;

			extern ID idSMDepthGeomPSSMDirect;
			extern ID idSMDepthGeomCube;

			extern ID idSMDepthGrassPSSMDirect;
			extern ID idSMDepthGrassCube;

			extern ID idSMDepthTreePSSMDirect;
			extern ID idSMDepthTreeCube;

			extern ID idSMDepthSkinPSSMDirect;
			extern ID idSMDepthSkinCube;

			extern ID idStdGeom;
			extern ID idStdTree;
			extern ID idStdGrass;
			extern ID idStdSkin;

			extern ID idLightBound;
		};

		namespace ps
		{
			extern ID idSMDepthGeomPSSMDirect;
			extern ID idSMDepthGeomCube;

			extern ID idSMDepthGreenPSSMDirect;
			extern ID idSMDepthGreenCube;

			extern ID idSMDepthSkinPSSMDirect;
			extern ID idSMDepthSkinCube;

			extern ID idPPBlurDepthBasedNoise;
			extern ID idPSSM4;
			extern ID idPSSM3;
			extern ID idPPBlurDepthBased;
			extern ID idGenShadowDirect4;
			extern ID idGenShadowDirect9;
			
			extern ID idGenShadowCube1;
			extern ID idGenShadowCube6;

			extern ID idScreenOut;

			extern ID idStdGeom;
			extern ID idStdGeomCP;
			extern ID idStdGreen;
			extern ID idStdGreenCP;
			extern ID idStdSkin;
			extern ID idStdSkinCP;

			extern ID idLightBound;
		};

		namespace kit
		{
			extern ID idSMDepthGeomPSSMDirect;
			extern ID idResPosDepthPSSM3;
			extern ID idResPosDepthPSSM4;
			extern ID idResPosDepthGenShadowDirect4;
			extern ID idResPosDepthGenShadowDirect9;
			extern ID idSMDepthGeomCube;
			extern ID idResPosDepthGenShadowCube1;
			extern ID idResPosDepthGenShadowCube6;
			extern ID idSMDepthSkinPSSMDirect;
			extern ID idSMDepthTreePSSMDirect;
			extern ID idSMDepthGrassPSSMDirect;
			extern ID idSMDepthGrassCube;
			extern ID idSMDepthTreeCube;
			extern ID idSMDepthSkinCube;
			extern ID idPPBlurDepthBasedNoise;
			extern ID idPPBlurDepthBased;

			extern ID idLightBound;
		};
	};
};

#endif