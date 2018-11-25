
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __GAME_DATA_H
#define __GAME_DATA_H


#include "Player.h"
#include "crosshair.h"
#include <mtrl/sxmtrl.h>
#include <gui/guimain.h>
#include "HUDcontroller.h"
#include "GameStateManager.h"

class GameData
{
public:
	GameData(HWND hWnd, bool isGame);
	~GameData();


	static CPlayer * m_pPlayer;
	static CPointCamera * m_pActiveCamera;
	static CEntityManager * m_pMgr;
	static gui::IGUI * m_pGUI;
	static CHUDcontroller * m_pHUDcontroller;
	static CGameStateManager * m_pGameStateManager;
	HWND m_hWnd;

	CCrosshair * m_pCrosshair;

	void update();
	void render();
	void renderHUD();
	void sync();

	void playFootstepSound(MTLTYPE_PHYSIC mtl_type, const float3 &f3Pos);

	static gui::dom::IDOMnode *m_pCell;
	//static gui::IDesktop *m_pStatsUI;

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

	static void ccmd_game_menu();

	static void ccmd_use_on();
	static void ccmd_use_off();

	bool m_isGame;

};

extern GameData * g_pGameData;

#endif
