
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Точечные объекты
*/

/*! \defgroup cpointentity Точечные объекты
\ingroup cbaseentity
@{
*/

//! \defgroup clight Свет

#ifndef __POINT_ENTITY_H
#define __POINT_ENTITY_H

#include "BaseEntity.h"

//! Класс точечного объекта
class CPointEntity: public CBaseEntity
{
	DECLARE_CLASS(CPointEntity, CBaseEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif

/*! @} cpointentity */

