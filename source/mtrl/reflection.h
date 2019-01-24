
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __REFLECTION_H
#define __REFLECTION_H

#include <gdefines.h>

//#define SM_D3D_CONVERSIONS
#include <common/Math.h>
#include <common/Array.h>
#include <graphix/graphix.h>

#include "sxmtrl.h"
#include "ml_data.h"

/*! класс отражений, возможны плоские отражения и кубические
 \note автогенерация мип уровней не работала, пришлось сделать небольшой костыль:\n
  - рисуем отражения в рабочую текстуру
  - эту текстуру рисуем в текстуру отражений\n
  при таком подходе все мип уровни генерируются
*/
class CReflection
{
public:
	CReflection();
	~CReflection();

	void onLostDevice();
	void onResetDevice();

	SX_ALIGNED_OP_MEM2();

	//! инициализация отражений
	void init(MTLTYPE_REFLECT type);

	//! возвращает тип отражения
	MTLTYPE_REFLECT getTypeReflect();

	//! возвращает фрустум по id, для плоских отражений 0, для кубических [0,5] 
	const IFrustum* getFrustum(ID id);

	//! обновление количесвтенных данных обновления, возаращет true в случае если можно рисовать отражения, false если отражения рисовать не надо, pViewPos - позиция наблюдателя (обязательно)
	bool updateCountUpdate(const float3_t *pViewPos);
	
	//! разрешен ли рендер отражений
	bool allowedRender();

	//! обнуление информации об обновлении
	void nullingCountUpdate();

	//**********************************************************************

	//! подготовка в рендеру плоских отражений, pPlane - плоскость отражения (обязательно)
	void preRenderRefPlane(const SMPLANE *pPlane);

	//! окончание рендера плоских отражений
	void postRenderRefPlane();

	//! возвращает текстуру с плоским отражением
	IGXTexture2D* getRefPlaneTex();

	//**********************************************************************

	//! подготовка данных кубических отражений
	void beginRenderRefCube(const float3_t *pCenter);

	//! подготвка к рендеру кубических отражений грани, pWorld - мировая матрица модели которая принимает эти отражения
	void preRenderRefCube(ID idFace, const float4x4 *pWorld);

	//! окончание рендера кубических отражений для грани
	void postRenderRefCube(ID idFace);

	//! окончание подготовки кубических отражений, pViewPos - позиция наблюдателя (обязательно)
	void endRenderRefCube(const float3_t *pViewPos);
	

	//! возвращает кубическую текстуру с отражением
	IGXTextureCube* getRefCubeTex();

	//! установить экстремумы
	void setMinMax(const float3_t *pMin, const float3_t *pMax);

	//! запись данных массива прсочетов
	void setIDvisCalcObj(ID id, ID idFace, ID idVisCalcObj);

	//! размер массива массивов идентификаторов просчета 
	int getCountIDsVisCalcObj();

	//! получить идентификатор массива просчета
	ID getIDvisCalcObj(ID id, ID idFace);

protected:

	//! позиция камеры для кубческих отражений
	float3 m_vPosition;

	//! экстремумы геометрии кубических отражений, по ним определяется необходимость обновления статических отражений, измеряется расстояние до камеры
	float3 m_vMin, m_vMax;

	//! матрица вида для отражений
	float4x4 m_mView;

	//! массив фрустумов, для плоских отражений используется только нулевой, для кубических все
	IFrustum *m_aFrustums[6];

	//! массивы идентификаторов для просчета видимости
	Array<ID*> m_aIDsVisCalcObj;

	//! количество обновлений отражения
	int m_iCountUpdate;

	//! тип отражений
	MTLTYPE_REFLECT typeReflection;

	//! предыдущие матрицы
	float4x4 m_mOldMatProj, m_mOldMatView, m_mOldMatViewProj;

	//! указатель на бэк буфер
	IGXSurface *m_pBackBuffer;

	//! текстура плоских отражений
	IGXTexture2D *m_pTexPlaneRef;

	//! рабочий сюрфейс
	IGXSurface *m_pSurface;

	//! рабочая текстура
	IGXTexture2D *m_pTexWork;

	//! текстура кубических отражений
	IGXTextureCube *m_pTexCubeRef;

	IGXDepthStencilState *m_pDSState = NULL;
};

#endif