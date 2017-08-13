
#ifndef _NPCZombie_H_
#define _NPCZombie_H_

#include "NPCBase.h"


class NPCZombie : public NPCBase
{
	DECLARE_CLASS(NPCZombie, NPCBase);
	DECLARE_PROPTABLE();

public:
	NPCZombie(EntityManager * pMgr);
	~NPCZombie();

	void OnSync();

protected:

	//virtual void InitPhysics();

	void RandWalk();
	ID m_idsnd_idle;
	ID m_idsnd_idle2;
	ID m_idsnd_death;
};

#endif