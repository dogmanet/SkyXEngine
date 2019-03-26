#ifndef __SHADOW_CACHE_H
#define __SHADOW_CACHE_H

#include <gdefines.h>

#include <light/IXLight.h>
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
	CShadowCache(IXRenderPipeline *pRenderPipeline);
	~CShadowCache();

	//! Установка количества лампочек, инициализация кэша
	void setLightsCount(UINT iPoints, UINT iSpots, bool hasGlobal);

	//! Указывает, что начался новый кадр
	void nextFrame();

	//! Добавляет источник к текущему проходу, В случае отсутствия свободных слотов, возвращает false
	void addLight(IXLight *pLight);

	UINT processNextBunch();
	IXLight *getLight(ID id);

protected:
	IXRenderPipeline *m_pRenderPipeline;

	UINT m_uCurrentFrame = 0;
	Array<IXLight*> m_aFrameLights;

	ID m_idRSMPixelShader = -1;

	struct _shadow_map_s
	{
		CShadowMap map;
		bool isDirty = false;
		IXLight *pLight = NULL;
	};

	Array<_shadow_map_s> m_aShadowMaps;
	//Array<CShadowCubeMap> m_aShadowCubeMaps;
	//CShadowPSSM *m_pShadowPSSM;

	struct _ready_shadows_s
	{
		IBaseShadowMap *pShadowMap;
		IXLight *pLight;
	};

	Array<_ready_shadows_s> m_aReadyMaps;
};

#endif
