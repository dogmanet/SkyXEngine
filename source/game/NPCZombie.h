/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Базовый класс npc
*/

/*! \ingroup cbaseanimating
@{
*/

#ifndef _NPCZombie_H_
#define _NPCZombie_H_

#include "NPCBase.h"

//! Класс NPC зомби
class CNPCZombie : public CNPCBase
{
	DECLARE_CLASS(CNPCZombie, CNPCBase);
	DECLARE_PROPTABLE();

public:
	CNPCZombie(CEntityManager * pMgr);
	~CNPCZombie();

	void onSync();

protected:

	//virtual void initPhysics();

	void randWalk();
	ID m_idSndIdle;
	ID m_idSndIdle2;
	ID m_idSndDeath;
};

#endif

//! @}
