
#ifndef _NPCZombie_H_
#define _NPCZombie_H_

#include "NPCBase.h"


class CNPCZombie : public CNPCBase
{
	DECLARE_CLASS(CNPCZombie, CNPCBase);
	DECLARE_PROPTABLE();

public:
	CNPCZombie(EntityManager * pMgr);
	~CNPCZombie();

	void OnSync();

protected:

	//virtual void InitPhysics();

	void randWalk();
	ID m_idSndIdle;
	ID m_idSndIdle2;
	ID m_idSndDeath;
};

#endif