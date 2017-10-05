#ifndef _GameData_H_
#define _GameData_H_


#include "SXplayer.h"
#include "crosshair.h"

class GameData
{
public:
	GameData();
	~GameData();


	static SXplayer * m_pPlayer;
	static SXpointCamera * m_pActiveCamera;
	static EntityManager * m_pMgr;

	Crosshair * m_pCrosshair;

	void Update();
	void Render();
	void RenderHUD();
	void Sync();

protected:

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

#endif
