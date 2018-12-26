
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Точка пути
*/

/*! \ingroup cpointentity
@{
*/

#ifndef __PATH_CORNER_H
#define __PATH_CORNER_H

#include "PointEntity.h"

//! тип сглаживания пути
enum PATH_CORNER_TYPE
{
	PCT_CORNER = 0, //!< без сглаживания
	PCT_SPLINE = 1 //!< сплайном
};

//! Точка пути для #CFuncTrain
class CPathCorner: public CPointEntity
{
	DECLARE_CLASS(CPathCorner, CPointEntity);
	DECLARE_PROPTABLE();
public:
	CPathCorner(CEntityManager * pMgr);

	//! получает координаты точки на пути на расстоянии dist от начала
	float3 getPoint(float dist);
	//! получает вращение точки на пути на расстоянии dist от начала
	SMQuaternion getRot(float dist);
	//! получает длину пути
	float GetLength();

	//! Получает следующую точку на пути
	CPathCorner * GetNext();
	//! Получает предыдущую точку на пути
	CPathCorner * GetPrev();

	void setPos(const float3 & pos);

	void setNextPoint(CBaseEntity *pEnt);

protected:
	//! Пересчитывает путь
	void RecalcPath(float t);
	void onPostLoad();

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
	CPathCorner * m_pNextStop;
	//! Предыдущая точка
	CPathCorner * m_pPrevStop;
};

#endif

//! @}
