
#include "NPCZombie.h"
#include "score/sxscore.h"

BEGIN_PROPTABLE(NPCZombie)
// empty
	
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(NPCZombie, npc_zombie);

NPCZombie::NPCZombie(EntityManager * pMgr) :
	BaseClass(pMgr)
{
	m_speed_walk = 0.07f;
	m_speed_run = 0.12f;

	m_idsnd_idle = SSCore_SndCreate3d("mobs/zombie/zombie_idle_16.ogg", false, 0, 30);
	m_idsnd_idle2 = SSCore_SndCreate3d("mobs/zombie/zombie_idle_17.ogg", false, 0, 30);
	m_idsnd_death = SSCore_SndCreate3d("mobs/zombie/zombie_die_1.ogg", false, 0, 45);
}

NPCZombie::~NPCZombie()
{

}

void NPCZombie::OnSync()
{
	BaseClass::OnSync();

	SSCore_SndPosWSet(m_idsnd_idle,&GetPos());
	SSCore_SndPosWSet(m_idsnd_idle2, &GetPos());
	SSCore_SndPosWSet(m_idsnd_death, &GetPos());

	//если здоровье меньше нуля
	if (m_health < 0.0f)
	{
		//то воспроизводим анимацию смерти
		this->PlayAnimation("fake_death0");
		SSCore_SndPlay(m_idsnd_death);
		m_health = 0.f;	//обнуляем здоровье говоря что больше проигрывать анимацию не надо
		return;
	}

	if (m_curr_aiquad < 0)
		return;

	RandWalk();
}

void NPCZombie::RandWalk()
{
	if (m_curr_quaid_in_path == -1)
	{
		float rndradius = randf(20.f,40.f)*0.5f;
		float3 rndpos;
		rndpos.x = m_vPosition.x + randf(-rndradius, rndradius);
		rndpos.y = m_vPosition.y + randf(-rndradius, rndradius);
		rndpos.z = m_vPosition.z + randf(-rndradius, rndradius);
		if (PathFind(SAIG_QuadGetNear(&rndpos)))
		{
			m_curr_quaid_in_path = 0;
			m_last_path_pos = GetPos();
		}

		return;
	}

	if (m_move_state == NPC_MOVE_STATE_IDLE_START && !this->PlayingAnimations("idle0"))
	{
		m_move_state = NPC_MOVE_STATE_IDLE;
		this->PlayAnimation("idle0");
		SSCore_SndPlay(m_idsnd_idle);
	}
	else if (m_move_state == NPC_MOVE_STATE_IDLE && !this->PlayingAnimations("idle0"))
		m_move_state = NPC_MOVE_STATE_WALK;
	else if (m_move_state != NPC_MOVE_STATE_IDLE)
	{
		if (!this->PlayingAnimations("walk0"))
		{
			this->PlayAnimation("walk0");
			SSCore_SndPlay(m_idsnd_idle2);
		}

		PathWalk();
	}
}