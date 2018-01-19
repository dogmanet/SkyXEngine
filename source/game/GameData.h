
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __GAME_DATA_H
#define __GAME_DATA_H


#include "Player.h"
#include "crosshair.h"
#include <mtllight/sxmtllight.h>

class GameData
{
public:
	GameData();
	~GameData();


	static CPlayer * m_pPlayer;
	static CPointCamera * m_pActiveCamera;
	static CEntityManager * m_pMgr;

	CCrosshair * m_pCrosshair;

	void update();
	void render();
	void renderHUD();
	void sync();

	void playFootstepSound(MTLTYPE_PHYSIC mtl_type, const float3 &f3Pos);

protected:

	void loadFoostepsSounds();

	ID *m_pidFootstepSound[MPT_COUNT];
	int m_iFootstepSoundCount[MPT_COUNT];

	static void ccmd_forward_on();
	static void ccmd_forward_off();

	static void ccmd_backward_on();
	static void ccmd_backward_off();

	static void ccmd_left_on();
	static void ccmd_left_off();

	static void ccmd_right_on();
	static void ccmd_right_off();

	static void ccmd_boost_on();
	static void ccmd_boost_off();

	static void ccmd_crouch_on();
	static void ccmd_crouch_off();

	static void ccmd_crawl_on();
	static void ccmd_crawl_off();

	static void ccmd_jump_on();
	static void ccmd_jump_off();

	static void ccmd_spawn();
	static void ccmd_observe();

	static void ccmd_attack_on();
	static void ccmd_attack_off();

	static void ccmd_attack2_on();
	static void ccmd_attack2_off();

	static void ccmd_reload();
	static void ccmd_switch_firemode();

	static void ccmd_toggleflashlight();

};

extern GameData * g_pGameData;

#endif
