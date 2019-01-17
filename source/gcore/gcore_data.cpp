
#include "gcore_data.h"

namespace gcore_data
{
	namespace rt_id
	{
		ID idComLight = -1;

		ID idColorScene = -1;
		ID idNormalScene = -1;
		ID idParamsScene = -1;
		ID idDepthScene = -1;
		ID idDepthScene0 = -1;
		ID idDepthScene1 = -1;

		ID idLightAmbientDiff = -1;
		ID idLightSpecular = -1;

		Array<ID> aToneMaps;
		Array<IGXSurface*> aSurfToneMap;
		int iCountArrToneMaps = 0;

		//**

		ID idAdaptLumCurr = -1;
		ID idAdaptLumLast = -1;

		int iHowAdaptedLum = 0;
		ID GetCurrAdaptedLum(){ if (iHowAdaptedLum == 0) return idAdaptLumCurr; else return  idAdaptLumLast; };
		ID GetLastAdaptedLum(){ if (iHowAdaptedLum == 1) return idAdaptLumCurr; else return  idAdaptLumLast; };
		void IncrAdaptedLum(){ if (iHowAdaptedLum >= 1) iHowAdaptedLum = 0; else iHowAdaptedLum = 1; };

		//**

		ID idLigthCom = -1;
		ID idLigthCom2 = -1;
		ID idLigthCom3 = -1;

		ID idLigthComScaled = -1;

		float2 aHDRSampleOffsets[16];
	};

	namespace vs_id
	{
		ID idScreenOut = -1;
	};

	namespace ps_id
	{
		ID idSampleLumIterative = -1;
		ID idSampleLumInit = -1;
		ID idCalcAdaptedLum = -1;
	};
};