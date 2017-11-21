
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Точечные объекты
*/

/*! \defgroup cpointentity Точечные объекты
\ingroup cbaseentity
@{
*/

//! \defgroup clight Свет

#ifndef _SXpointEntity_H_
#define _SXpointEntity_H_

#include "SXbaseEntity.h"

//! Класс точечного объекта
class SXpointEntity: public SXbaseEntity
{
	DECLARE_CLASS(SXpointEntity, SXbaseEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif

/*! @} cpointentity */

