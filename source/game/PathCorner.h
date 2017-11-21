
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Точка пути
*/

/*! \ingroup cpointentity
@{
*/

#ifndef _PathCorner_H_
#define _PathCorner_H_

#include "SXpointEntity.h"

//! тип сглаживания пути
enum PATH_CORNER_TYPE
{
	PCT_CORNER = 0, //!< без сглаживания
	PCT_SPLINE = 1 //!< сплайном
};

//! Точка пути для #FuncTrain
class PathCorner: public SXpointEntity
{
	DECLARE_CLASS(PathCorner, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	PathCorner(EntityManager * pMgr);

	//! получает координаты точки на пути на расстоянии dist от начала
	float3 GetPoint(float dist);
	//! получает вращение точки на пути на расстоянии dist от начала
	SMQuaternion GetRot(float dist);
	//! получает длину пути
	float GetLength();

	//! Получает следующую точку на пути
	PathCorner * GetNext();
	//! Получает предыдущую точку на пути
	PathCorner * GetPrev();

protected:
	//! Пересчитывает путь
	void RecalcPath(float t);
	void OnPostLoad();

	//! Тип сглаживания
	int m_type;
	//! Новая скорость поезда
	float m_fNewSpeed;

	//! Длина сегмента пути
	float m_fLength;

	//! Для расчета сплайна @{
	float3_t m_fH;

	float3_t m_fCoeffsA;
	float3_t m_fCoeffsB;
	float3_t m_fCoeffsC;
	float3_t m_fCoeffsD;

	float3_t m_fDelta;
	float3_t m_fLambda;
	//! @}

	//! Следующая точка
	PathCorner * m_pNextStop;
	//! Предыдущая точка
	PathCorner * m_pPrevStop;
};

#endif

//! @}
