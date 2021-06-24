
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Камера игровая
*/

/*! \ingroup cpointentity
@{
*/

#ifndef __POINT_CAMERA_H
#define __POINT_CAMERA_H

#include <gcore/sxgcore.h>

#include "PointEntity.h"

//! Класс камеры
class CPointCamera: public CPointEntity
{
	DECLARE_CLASS(CPointCamera, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CPointCamera();

	//! Возвращает объект камеры из графической либы
	ICamera* getCamera();

	void setPos(const float3 &pos) override;
	void setOrient(const SMQuaternion &q) override;

protected:
	ICamera *m_pSXC;
};

#endif

/*! @} */
