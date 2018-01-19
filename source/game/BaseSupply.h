
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый класс припаса
*/

#ifndef __BASE_SUPPLY_H
#define __BASE_SUPPLY_H

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
