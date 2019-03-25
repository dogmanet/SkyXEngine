#ifndef __SHADOW_CACHE_H
#define __SHADOW_CACHE_H

#include <gdefines.h>

#include <light/IXLight.h>
#include <common/array.h>

enum SHADOW_TYPE
{
	SHADOW_TYPE_PSSM,
	SHADOW_TYPE_OMNI,
	SHADOW_TYPE_SPOT
};

class CShadowCache
{
public:
	CShadowCache();
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
	UINT m_uCurrentFrame = 0;
	Array<IXLight*> m_aFrameLights;
};

#endif
