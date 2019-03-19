#ifndef __SHADOW_CACHE_H
#define __SHADOW_CACHE_H

#include <gdefines.h>

#include <light/IXLight.h>

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

	//! Проверяет, есть ли актуальная тень для запрошенной лампочки
	bool hasShadowFor(IXLight *pLight);

	//! Получает количество доступных слотов для определенного типа теней
	UINT getAvailableSlots(SHADOW_TYPE shadowType);

	//! Добавляет источник к текущему проходу, В случае отсутствия свободных слотов, возвращает false
	bool addLightToPass(IXLight *pLight);

	void updateShadows();

protected:
	UINT m_uCurrentFrame = 0;
};

#endif
