
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

#ifndef _SXpointCamera_H_
#define _SXpointCamera_H_

#include <gcore/sxgcore.h>

#include "SXpointEntity.h"

//! Класс камеры
class SXpointCamera: public SXpointEntity
{
	DECLARE_CLASS(SXpointCamera, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	SXpointCamera(EntityManager * pMgr);
	~SXpointCamera();

	//! Возвращает объект камеры из графической либы
	ISXCamera * GetCamera();

protected:
	ISXCamera * m_pSXC;

	void OnSync();
};

#endif

/*! @} */
