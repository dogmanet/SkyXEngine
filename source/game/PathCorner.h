
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
	DECLARE_TRIVIAL_CONSTRUCTOR();
	~CPathCorner();

	//! получает координаты точки на пути на расстоянии dist от начала
	float3 getPoint(float dist);
	//! получает вращение точки на пути на расстоянии dist от начала
	SMQuaternion getRot(float dist);
	//! получает длину пути
	float getLength();
	//! получает новую скорость
	float getNewSpeed();

	//! Получает следующую точку на пути
	CPathCorner * getNext();
	//! Получает предыдущую точку на пути
	CPathCorner * getPrev();

	void setPos(const float3 & pos);

	void setNextPoint(CPathCorner *pEnt);

protected:
	//! Пересчитывает путь
	void recalcPath(float t);
	void onPostLoad();

	//! Тип сглаживания
	int m_type = PCT_SPLINE;
	//! Новая скорость поезда
	float m_fNewSpeed = 0.0f;

	//! Длина сегмента пути
	float m_fLength = 0.0f;

	//! Для расчета сплайна @{
	float3_t m_fH = 0.0f;

	float3_t m_fCoeffsA = 0.0f;
	float3_t m_fCoeffsB = 0.0f;
	float3_t m_fCoeffsC = 0.0f;
	float3_t m_fCoeffsD = 0.0f;

	float3_t m_fDelta = 0.0f;
	float3_t m_fLambda = 0.0f;
	//! @}

	//! Следующая точка
	CPathCorner *m_pNextStop = NULL;
	//! Предыдущая точка
	CPathCorner *m_pPrevStop = NULL;
};

#endif

//! @}
