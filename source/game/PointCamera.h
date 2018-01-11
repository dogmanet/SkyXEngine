
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Камера игровая
*/

/*! \ingroup cpointentity
@{
*/

#ifndef _CPointCamera_H_
#define _CPointCamera_H_

#include <gcore/sxgcore.h>

#include "PointEntity.h"

//! Класс камеры
class CPointCamera: public CPointEntity
{
	DECLARE_CLASS(CPointCamera, CPointEntity);
	DECLARE_PROPTABLE();
public:
	CPointCamera(CEntityManager * pMgr);
	~CPointCamera();

	//! Возвращает объект камеры из графической либы
	ISXCamera * getCamera();

protected:
	ISXCamera * m_pSXC;

	void onSync();
};

#endif

/*! @} */
