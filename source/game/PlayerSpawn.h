
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Точка респауна игрока
*/

/*! \ingroup cpointentity
@{
*/

#ifndef __PLAYER_SPAWN_H
#define __PLAYER_SPAWN_H

#include "PointEntity.h"

//! Точка респауна игрока
class CPlayerSpawn: public CPointEntity
{
	DECLARE_CLASS(CPlayerSpawn, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

	int m_iTeam;
};

#endif

//! @}
