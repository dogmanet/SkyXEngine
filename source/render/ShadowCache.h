#ifndef __SHADOW_CACHE_H
#define __SHADOW_CACHE_H

#include <gdefines.h>

#include <light/IXLight.h>
#include <mtrl/IXMaterialSystem.h>
#include <common/array.h>
#include "shadow.h"

enum SHADOW_TYPE
{
	SHADOW_TYPE_PSSM,
	SHADOW_TYPE_OMNI,
	SHADOW_TYPE_SPOT
};

class CShadowCache
{
public:
	CShadowCache(IXRenderPipeline *pRenderPipeline, IXMaterialSystem *pMaterialSystem);
	~CShadowCache();

	SX_ALIGNED_OP_MEM2();

	//! Установка количества лампочек, инициализация кэша
	void setLightsCount(UINT iPoints, UINT iSpots, bool hasGlobal);

	//! Указывает, что начался новый кадр
	void nextFrame();

	void setObserverCamera(ICamera *pCamera);

	//! Добавляет источник к текущему проходу, В случае отсутствия свободных слотов, возвращает false
	void addLight(IXLight *pLight);

	UINT processNextBunch();
	IXLight *getLight(ID id);
	IBaseShadowMap *getShadow(ID id);

protected:
	IXRenderPipeline *m_pRenderPipeline;
	IXMaterialSystem *m_pMaterialSystem;

	ICamera *m_pCamera = NULL;

	IGXRasterizerState *m_pRasterizerConservative = NULL;

	UINT m_uCurrentFrame = 0;
	Array<IXLight*> m_aFrameLights;
	bool m_isFirstBunch = true;

	struct ShadowMap
	{
		CShadowMap map;
		bool isDirty = false;
		bool shouldProcess = false;
		IXLight *pLight = NULL;
	};

	struct ShadowCubeMap
	{
		CShadowCubeMap map;
		bool isDirty = false;
		bool shouldProcess = false;
		IXLight *pLight = NULL;
		UINT uLastUsed = UINT_MAX;
	};

	struct ShadowPSSM
	{
		CShadowPSSM map;
		bool isDirty = false;
		bool shouldProcess = false;
		IXLight *pLight = NULL;
	};

	Array<ShadowMap> m_aShadowMaps;
	Array<ShadowCubeMap> m_aShadowCubeMaps;
	Array<ShadowCubeMap*> m_aShadowCubeMapsQueue;
	ShadowPSSM *m_pShadowPSSM = NULL;

	struct ReadyShadows
	{
		IBaseShadowMap *pShadowMap;
		IXLight *pLight;
	};

	Array<ReadyShadows> m_aReadyMaps;

	//XGeometryShaderHandler *m_pRSMGeometryShader = NULL;
	XGeometryShaderHandler *m_pCubemapGeometryShader = NULL;
	XGeometryShaderHandler *m_pPSSMGeometryShader[PSSM_MAX_SPLITS];
	XRenderPassHandler *m_pRenderPassShadow = NULL;
};

#endif
