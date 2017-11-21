/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Базовый класс припаса
*/

#ifndef _SXbaseSupply_H_
#define _SXbaseSupply_H_

#include "SXbaseItem.h"

/*! Базовый класс припаса (патроны, батарейки, прочие расходники)
\ingroup cbaseitem
*/
class SXbaseSupply: public SXbaseItem
{
	DECLARE_CLASS(SXbaseSupply, SXbaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
