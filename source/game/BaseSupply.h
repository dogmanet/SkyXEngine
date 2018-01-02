/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Базовый класс припаса
*/

#ifndef _CBaseSupply_H_
#define _CBaseSupply_H_

#include "BaseItem.h"

/*! Базовый класс припаса (патроны, батарейки, прочие расходники)
\ingroup cbaseitem
*/
class CBaseSupply: public CBaseItem
{
	DECLARE_CLASS(CBaseSupply, CBaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
