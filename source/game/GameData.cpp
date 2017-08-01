#include "GameData.h"

SXplayer * GameData::m_pPlayer;
SXpointCamera * GameData::m_pActiveCamera;
EntityManager * GameData::m_pMgr;

GameData::GameData()
{
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


	m_pPlayer = (SXplayer*)CREATE_ENTITY("player", m_pMgr);
	m_pActiveCamera = m_pPlayer->GetCamera();

	Core_0RegisterConcmdClsArg("+debug_slot_move", m_pPlayer, (SXCONCMDCLSARG)&SXplayer::_ccmd_slot_on);
	Core_0RegisterConcmdCls("-debug_slot_move", m_pPlayer, (SXCONCMDCLS)&SXplayer::_ccmd_slot_off);

	//m_pPlayer->SetModel("models/stalker_zombi/stalker_zombi_a.dse");
	//m_pPlayer->PlayAnimation("reload");
}
GameData::~GameData()
{
	mem_delete(m_pMgr);
}

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
