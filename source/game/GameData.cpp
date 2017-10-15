#include "GameData.h"
#include "CrosshairManager.h"

#include "Ragdoll.h"

#include <score/sxscore.h>


SXplayer * GameData::m_pPlayer;
SXpointCamera * GameData::m_pActiveCamera;
EntityManager * GameData::m_pMgr;

CRagdoll * g_pRagdoll;
IAnimPlayer * pl;

GameData::GameData()
{
	loadFoostepsSounds();

	m_pMgr = new EntityManager();

	Core_0RegisterConcmd("+forward", ccmd_forward_on);
	Core_0RegisterConcmd("-forward", ccmd_forward_off);
	Core_0RegisterConcmd("+backward", ccmd_backward_on);
	Core_0RegisterConcmd("-backward", ccmd_backward_off);
	Core_0RegisterConcmd("+left", ccmd_left_on);
	Core_0RegisterConcmd("-left", ccmd_left_off);
	Core_0RegisterConcmd("+right", ccmd_right_on);
	Core_0RegisterConcmd("-right", ccmd_right_off);
	Core_0RegisterConcmd("+boost", ccmd_boost_on);
	Core_0RegisterConcmd("-boost", ccmd_boost_off);
	Core_0RegisterConcmd("+crouch", ccmd_crouch_on);
	Core_0RegisterConcmd("-crouch", ccmd_crouch_off);
	Core_0RegisterConcmd("+jump", ccmd_jump_on);
	Core_0RegisterConcmd("-jump", ccmd_jump_off);
	Core_0RegisterConcmd("+crawl", ccmd_crawl_on);
	Core_0RegisterConcmd("-crawl", ccmd_crawl_off);
	Core_0RegisterConcmd("+attack", ccmd_attack_on);
	Core_0RegisterConcmd("-attack", ccmd_attack_off);
	Core_0RegisterConcmd("+attack2", ccmd_attack2_on);
	Core_0RegisterConcmd("-attack2", ccmd_attack2_off);
	Core_0RegisterConcmd("reload", ccmd_reload);
	Core_0RegisterConcmd("switch_firemode", ccmd_switch_firemode);
	Core_0RegisterConcmd("flashlight", ccmd_toggleflashlight);

	Core_0RegisterConcmd("spawn", ccmd_spawn);
	Core_0RegisterConcmd("observe", ccmd_observe);

	Core_0RegisterCVarFloat("r_default_fov", 45.0f, "Default FOV value");
	Core_0RegisterCVarBool("cl_mode_editor", false, "Editor control mode");
	Core_0RegisterCVarBool("cl_grab_cursor", true, "Grab cursor on move");
	

	Core_0RegisterCVarBool("cl_bob", true, "View bobbing");
	Core_0RegisterCVarFloat("cl_bob_walk_y", 0.1f, "View bobbing walk y amplitude");
	Core_0RegisterCVarFloat("cl_bob_walk_x", 0.1f, "View bobbing walk strafe amplitude");
	Core_0RegisterCVarFloat("cl_bob_run_y", 0.2f, "View bobbing run y amplitude");
	Core_0RegisterCVarFloat("cl_bob_run_x", 0.15f, "View bobbing run strafe amplitude");
	Core_0RegisterCVarFloat("cl_bob_walk", 0.5f, "View bobbing walk period");
	Core_0RegisterCVarFloat("cl_bob_run", 1.0f, "View bobbing run period");

	CrosshairManager::loadConfig("config/game/crosshairs.cfg");

	m_pPlayer = (SXplayer*)CREATE_ENTITY("player", m_pMgr);
	m_pActiveCamera = m_pPlayer->GetCamera();
	m_pCrosshair = m_pPlayer->GetCrosshair();

	ID idCrosshairDefault = CrosshairManager::getCrosshairID("default");
	if(ID_VALID(idCrosshairDefault))
	{
		CrosshairManager::loadCrosshair(idCrosshairDefault, m_pCrosshair);
	}

	Core_0RegisterConcmdClsArg("+debug_slot_move", m_pPlayer, (SXCONCMDCLSARG)&SXplayer::_ccmd_slot_on);
	Core_0RegisterConcmdCls("-debug_slot_move", m_pPlayer, (SXCONCMDCLS)&SXplayer::_ccmd_slot_off);


	//m_pPlayer->SetModel("models/stalker_zombi/stalker_zombi_a.dse");
	//m_pPlayer->PlayAnimation("reload");

	//pl = SXAnim_CreatePlayer("models/stalker_zombi/stalker_zombi_a.dse");
	//pl->SetPos(float3(0, 0, 0));
	//g_pRagdoll = new CRagdoll(pl);
	//pl->setRagdoll(g_pRagdoll);
}
GameData::~GameData()
{
	//mem_delete(g_pRagdoll);

	mem_delete(m_pMgr);

	for(int i = 0; i < MPT_COUNT; ++i)
	{
		// @TODO: SSCore_SndDelete3dInst()
		mem_delete_a(m_pidFootstepSound[i]);
	}
}

void GameData::Update()
{
	m_pCrosshair->Update();
}
void GameData::Render()
{
}
void GameData::RenderHUD()
{
	m_pCrosshair->Render();
}
void GameData::Sync()
{
	m_pCrosshair->OnSync();
}

void GameData::playFootstepSound(MtlPhysicType mtl_type, const float3 &f3Pos)
{
	int iCount = m_iFootstepSoundCount[mtl_type];
	if(!iCount)
	{
		return;
	}
	ID idSound = m_pidFootstepSound[mtl_type][rand() % iCount];
	SSCore_SndInstancePlay3d(idSound, (float3*)&f3Pos);
}

void GameData::loadFoostepsSounds()
{
	Array<const char*> aSounds[MPT_COUNT];

	aSounds[mpt_concrete].push_back("actor/step/default1.ogg");
	aSounds[mpt_concrete].push_back("actor/step/default2.ogg");
	aSounds[mpt_concrete].push_back("actor/step/default3.ogg");
	aSounds[mpt_concrete].push_back("actor/step/default4.ogg");

	aSounds[mpt_metal].push_back("actor/step/metal_plate1.ogg");
	aSounds[mpt_metal].push_back("actor/step/metal_plate2.ogg");
	aSounds[mpt_metal].push_back("actor/step/metal_plate3.ogg");
	aSounds[mpt_metal].push_back("actor/step/metal_plate4.ogg");

	aSounds[mpt_tree].push_back("actor/step/new_wood1.ogg");
	aSounds[mpt_tree].push_back("actor/step/new_wood2.ogg");
	aSounds[mpt_tree].push_back("actor/step/new_wood3.ogg");
	aSounds[mpt_tree].push_back("actor/step/new_wood4.ogg");

	aSounds[mpt_ground_sand].push_back("actor/step/earth1.ogg");
	aSounds[mpt_ground_sand].push_back("actor/step/earth2.ogg");
	aSounds[mpt_ground_sand].push_back("actor/step/earth3.ogg");
	aSounds[mpt_ground_sand].push_back("actor/step/earth4.ogg");

	aSounds[mpt_water].push_back("actor/step/t_water1.ogg");
	aSounds[mpt_water].push_back("actor/step/t_water2.ogg");

	aSounds[mpt_leaf_grass].push_back("actor/step/grass1.ogg");
	aSounds[mpt_leaf_grass].push_back("actor/step/grass2.ogg");
	aSounds[mpt_leaf_grass].push_back("actor/step/grass3.ogg");
	aSounds[mpt_leaf_grass].push_back("actor/step/grass4.ogg");

	//aSounds[mpt_glass].push_back("actor/step/.ogg");
	//aSounds[mpt_plastic].push_back("actor/step/.ogg");
	//aSounds[mpt_flesh].push_back("actor/step/.ogg");

	for(int i = 0; i < MPT_COUNT; ++i)
	{
		Array<const char*> *paSounds = &aSounds[i];
		int jl = paSounds->size();
		m_iFootstepSoundCount[i] = jl;
		m_pidFootstepSound[i] = jl ? new ID[jl] : NULL;
		for(int j = 0; j < jl; ++j)
		{
			m_pidFootstepSound[i][j] = SSCore_SndCreate3dInst(paSounds[0][j], false, 0, 100);
		}
	}
}

//###################################################################

void GameData::ccmd_forward_on()
{
	m_pPlayer->Move(PM_FORWARD, TRUE);
}
void GameData::ccmd_forward_off()
{
	m_pPlayer->Move(PM_FORWARD, FALSE);
}

void GameData::ccmd_backward_on()
{
	m_pPlayer->Move(PM_BACKWARD, TRUE);
}
void GameData::ccmd_backward_off()
{
	m_pPlayer->Move(PM_BACKWARD, FALSE);
}

void GameData::ccmd_left_on()
{
	m_pPlayer->Move(PM_LEFT, TRUE);
}
void GameData::ccmd_left_off()
{
	m_pPlayer->Move(PM_LEFT, FALSE);
}

void GameData::ccmd_right_on()
{
	m_pPlayer->Move(PM_RIGHT, TRUE);
}
void GameData::ccmd_right_off()
{
	m_pPlayer->Move(PM_RIGHT, FALSE);
}

void GameData::ccmd_boost_on()
{
	m_pPlayer->Move(PM_RUN, TRUE);
}
void GameData::ccmd_boost_off()
{
	m_pPlayer->Move(PM_RUN, FALSE);
}

void GameData::ccmd_crouch_on()
{
	m_pPlayer->Move(PM_CROUCH, TRUE);
}
void GameData::ccmd_crouch_off()
{
	m_pPlayer->Move(PM_CROUCH, FALSE);
}

void GameData::ccmd_crawl_on()
{
	m_pPlayer->Move(PM_CRAWL, TRUE);
}
void GameData::ccmd_crawl_off()
{
	m_pPlayer->Move(PM_CRAWL, FALSE);
}

void GameData::ccmd_jump_on()
{
	m_pPlayer->Move(PM_JUMP, TRUE);
}
void GameData::ccmd_jump_off()
{
	m_pPlayer->Move(PM_JUMP, FALSE);
}

void GameData::ccmd_attack_on()
{
	m_pPlayer->Attack(TRUE);
}
void GameData::ccmd_attack_off()
{
	m_pPlayer->Attack(FALSE);
}

void GameData::ccmd_attack2_on()
{
	m_pPlayer->Attack2(TRUE);
}
void GameData::ccmd_attack2_off()
{
	m_pPlayer->Attack2(FALSE);
}

void GameData::ccmd_spawn()
{
	Core_0ConsoleExecCmd("cl_mode_editor 0");
	m_pPlayer->Spawn();
}
void GameData::ccmd_observe()
{
	m_pPlayer->Observe();
}

void GameData::ccmd_reload()
{
	m_pPlayer->Reload();
}

void GameData::ccmd_toggleflashlight()
{
	m_pPlayer->ToggleFlashlight();
}

void GameData::ccmd_switch_firemode()
{
	m_pPlayer->nextFireMode();
}
