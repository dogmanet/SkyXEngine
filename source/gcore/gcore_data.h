
#ifndef __GCORE_DATA
#define __GCORE_DATA

#include <gdefines.h>
#include <d3d9.h>
#include <common/array.h>
#include <common/SXMath.h>

namespace gcore_data
{
	namespace rt_id
	{
		extern ID idComLight;

		extern ID idColorScene;
		extern ID idNormalScene;
		extern ID idParamsScene;
		extern ID idDepthScene;
		extern ID idDepthScene0;
		extern ID idDepthScene1;

		extern ID idLightAmbientDiff;
		extern ID idLightSpecular;

		extern Array<ID> aToneMaps;
		extern Array<LPDIRECT3DSURFACE9> aSurfToneMap;
		extern int iCountArrToneMaps;

		//**

		extern ID idAdaptLumCurr;
		extern ID idAdaptLumLast;

		extern int iHowAdaptedLum;
		extern ID GetCurrAdaptedLum();
		extern ID GetLastAdaptedLum();
		extern void IncrAdaptedLum();

		//**

		extern ID idLigthCom;
		extern ID idLigthCom2;
		extern ID idLigthCom3;

		extern ID idLigthComScaled;

		extern float2 aHDRSampleOffsets[16];
	};

	namespace vs_id
	{
		extern ID idScreenOut;
	};

	namespace ps_id
	{
		extern ID idSampleLumIterative;
		extern ID idSampleLumInit;
		extern ID idCalcAdaptedLum;
	};
};

#endif