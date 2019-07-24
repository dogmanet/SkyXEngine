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

	//! Установка количества лампочек, инициализация кэша
	void setLightsCount(UINT iPoints, UINT iSpots, bool hasGlobal);

	//! Указывает, что начался новый кадр
	void nextFrame();

	//! Добавляет источник к текущему проходу, В случае отсутствия свободных слотов, возвращает false
	void addLight(IXLight *pLight);

	UINT processNextBunch();
	IXLight *getLight(ID id);
	IBaseShadowMap *getShadow(ID id);

protected:
	IXRenderPipeline *m_pRenderPipeline;
	IXMaterialSystem *m_pMaterialSystem;

	UINT m_uCurrentFrame = 0;
	Array<IXLight*> m_aFrameLights;

	ID m_idRSMCubeGeometryShader = -1;

	ID m_idRSMPixelShader = -1;
	ID m_idRSMCubePixelShader = -1;
	ID m_idRSMPixelShaderSpot = -1;

	struct ShadowMap
	{
		CShadowMap map;
		bool isDirty = false;
		IXLight *pLight = NULL;
	};

	struct ShadowCubeMap
	{
		CShadowCubeMap map;
		bool isDirty = false;
		IXLight *pLight = NULL;
	};

	struct ShadowPSSM
	{
		CShadowPSSM map;
		bool isDirty = false;
		IXLight *pLight = NULL;
	};

	Array<ShadowMap> m_aShadowMaps;
	Array<ShadowCubeMap> m_aShadowCubeMaps;
	ShadowPSSM m_shadowPSSM;

	struct ReadyShadows
	{
		IBaseShadowMap *pShadowMap;
		IXLight *pLight;
	};

	Array<ReadyShadows> m_aReadyMaps;
};

#endif
