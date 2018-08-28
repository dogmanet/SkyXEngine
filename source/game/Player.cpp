
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <input/sxinput.h>
#include <light/sxlight.h>
#include "Player.h"
#include "LightDirectional.h"
#include "BaseTool.h"
#include "BaseAmmo.h"

#include "BaseWeapon.h"

#include "GameData.h"

#include "HUDcontroller.h"

#include <aigrid/sxaigrid.h>

/*! \skydocent player
Объект игрока в мире
*/

BEGIN_PROPTABLE(CPlayer)
// empty
END_PROPTABLE()

REGISTER_ENTITY(CPlayer, player);

CPlayer::CPlayer(CEntityManager * pMgr):
	BaseClass(pMgr),
	m_canJump(true),
	m_fViewbobStep(0.0f),
	m_fViewbobY(0.0f),
	m_fViewbobStrafe(float3_t(0, 0, 0)),
	m_vWpnShakeAngles(float3_t(0.0f, 0.0f, 0.0f)),
	m_iDSM(DSM_NONE),
	m_bCanRespawn(false)
{
	m_pCamera = (CPointCamera*)CREATE_ENTITY("point_camera", pMgr);
	m_pCamera->setPos(m_pHeadEnt->getPos());
	m_pCamera->setParent(m_pHeadEnt);

	m_iUpdIval = SET_INTERVAL(updateInput, 0);

	/*m_pActiveTool = (CBaseTool*)CREATE_ENTITY("weapon_ak74", m_pMgr);
	m_pActiveTool->setOwner(this);
	m_pActiveTool->attachHands();
	m_pActiveTool->playAnimation("idle");
	m_pActiveTool->setPos(getPos() + float3(1.0f, m_fCapsHeight - 0.1f, 1.0f));
	m_pActiveTool->setOrient(m_pHeadEnt->getOrient());
	m_pActiveTool->setParent(m_pHeadEnt);

	CBaseAmmo *pAmmo = (CBaseAmmo*)CREATE_ENTITY("ammo_5.45x39ps", m_pMgr);
	m_pActiveTool->chargeAmmo(pAmmo);

	getInventory()->putItems("ammo_5.45x39ps", 60);

	CBaseMag *pMag = (CBaseMag*)CREATE_ENTITY("mag_ak74_30", m_pMgr);
	pMag->load(30);
	((CBaseWeapon*)m_pActiveTool)->attachMag(pMag);

	*/

	m_idQuadCurr = -1;

	m_pCrosshair = new CCrosshair();

	m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
}

CPlayer::~CPlayer()
{
	mem_delete(m_pCrosshair);
	CLEAR_INTERVAL(m_iUpdIval);
	REMOVE_ENTITY(m_pCamera);
}

void CPlayer::updateInput(float dt)
{
	int x, y;
	static const float * sense = GET_PCVAR_FLOAT("cl_mousesense");
	static const bool * editor_mode = GET_PCVAR_BOOL("cl_mode_editor");
	static const bool * grab_cursor = GET_PCVAR_BOOL("cl_grab_cursor");

	if(*editor_mode && !SSInput_GetKeyState(SIK_LCONTROL))
	{
		m_pCharacter->setWalkDirection(btVector3(0.0f, 0.0f, 0.0f));
		return;
	}

	m_vWpnShakeAngles = (float3)(m_vWpnShakeAngles * 0.4f);

	if(!*editor_mode || SSInput_GetKeyState(SIM_LBUTTON))
	{
		SSInput_GetMouseDelta(&x, &y);

		float dx = (float)x * *sense * 10.0f /* / dt */;
		float dy = (float)y * *sense * 10.0f /* / dt */;
		if(m_iDSM && m_pActiveTool)
		{
			m_pActiveTool->dbgMove(m_iDSM, dy);
			if(m_iDSM == DSM_PRINT)
			{
				m_iDSM = DSM_NONE;
			}
		}
		else
		{
			//printf("%f %f : ", dx, dy);
			m_vPitchYawRoll.y -= dx;
			m_vPitchYawRoll.x -= dy;
			//printf(" %f %f\n", m_vPitchYawRoll.x, m_vPitchYawRoll.y);
			m_vPitchYawRoll.x = clampf(m_vPitchYawRoll.x, -SM_PIDIV2, SM_PIDIV2);
			while(m_vPitchYawRoll.y < 0.0f)
			{
				m_vPitchYawRoll.y += SM_2PI;
			}
			while(m_vPitchYawRoll.y > SM_2PI)
			{
				m_vPitchYawRoll.y -= SM_2PI;
			}
			m_vWpnShakeAngles.y += dx * 0.05f;
			m_vWpnShakeAngles.x += dy * 0.05f;
			const float fMaxAng = SM_PI * 0.1f;
			m_vWpnShakeAngles.y = clampf(m_vWpnShakeAngles.y, -fMaxAng, fMaxAng);

			//m_vOrientation = SMQuaternion(m_vPitchYawRoll.x, 'x')
			//	* SMQuaternion(m_vPitchYawRoll.y, 'y')
			//	* SMQuaternion(m_vPitchYawRoll.z, 'z');
			setOrient(SMQuaternion(m_vPitchYawRoll.y, 'y'));
			m_pHeadEnt->setOffsetOrient(SMQuaternion(m_vPitchYawRoll.x, 'x') * SMQuaternion(m_vPitchYawRoll.z, 'z'));
			
			GameData::m_pHUDcontroller->setPlayerRot(m_vPitchYawRoll);
		}

	}

	{
		if(m_uMoveDir & PM_RUN)
		{
			dt *= 5.0f;
		}
		dt *= 10.0f;
		float3 dir;
		bool mov = false;
		if(m_uMoveDir & PM_FORWARD)
		{
			dir.z += 1.0f;
			mov = true;
		}

		if(m_uMoveDir & PM_BACKWARD)
		{
			dir.z -= 1.0f;
			mov = true;
		}

		if(m_uMoveDir & PM_LEFT)
		{
			dir.x -= 1.0f;
			mov = true;
		}

		if(m_uMoveDir & PM_RIGHT)
		{
			dir.x += 1.0f;
			mov = true;
		}

		if(m_uMoveDir & PM_OBSERVER)
		{
			m_vPosition = (float3)(m_vPosition + m_pHeadEnt->getOrient() * (SMVector3Normalize(dir) * dt));
		}
		else
		{
			dir = SMQuaternion(m_vPitchYawRoll.y, 'y') * (SMVector3Normalize(dir) * dt);
			dir *= 0.5f;
			if(m_uMoveDir & PM_RUN)
			{
				dir *= 0.5f;
			}
			if((m_uMoveDir & PM_JUMP))
			{
				if(m_pCharacter->canJump())
				{
					if(m_canJump)
					{
						playFootstepsSound();
						m_pCharacter->jump();
						m_canJump = false;
					}
				}
				else
				{
					m_canJump = false;
				}
			}
			else
			{
				m_canJump = true;
			}
			m_pCharacter->setWalkDirection(F3_BTVEC(dir));

			static const bool * cl_bob = GET_PCVAR_BOOL("cl_bob");
			static const float * cl_bob_walk_y = GET_PCVAR_FLOAT("cl_bob_walk_y");
			static const float * cl_bob_walk_x = GET_PCVAR_FLOAT("cl_bob_walk_x");
			static const float * cl_bob_run_x = GET_PCVAR_FLOAT("cl_bob_run_x");
			static const float * cl_bob_run_y = GET_PCVAR_FLOAT("cl_bob_run_y");
			static const float * cl_bob_run = GET_PCVAR_FLOAT("cl_bob_run");
			static const float * cl_bob_walk = GET_PCVAR_FLOAT("cl_bob_walk");

			

			if(*cl_bob)
			{
				if(mov && m_pCharacter->onGround())
				{
					const float fFS1 = SM_PIDIV2;
					const float fFS2 = SM_PI + SM_PIDIV2;
					bool bFS1 = m_fViewbobStep < fFS1;
					bool bFS2 = m_fViewbobStep < fFS2;
					if(m_uMoveDir & PM_RUN)
					{
						m_fViewbobStep += dt * *cl_bob_run * 0.2f;
					}
					else
					{
						m_fViewbobStep += dt * *cl_bob_walk;
					}
					if((bFS1 && m_fViewbobStep > fFS1) || (bFS2 && m_fViewbobStep > fFS2))
					{
						playFootstepsSound();
					}
					//printf("%f\n", m_fViewbobStep);
					while(m_fViewbobStep > SM_2PI)
					{
						m_fViewbobStep -= SM_2PI;
					}
					while(m_fViewbobStep < -SM_2PI)
					{
						m_fViewbobStep += SM_2PI;
					}
				}
				else
				{
					if(m_fViewbobStep > SM_PI + SM_PIDIV2)
					{
						m_fViewbobStep = m_fViewbobStep - SM_2PI;
					}
					if(m_fViewbobStep > SM_PIDIV2 || m_fViewbobStep < -SM_PIDIV2)
					{
						m_fViewbobStep = SM_PI - m_fViewbobStep;
					}
					m_fViewbobStep *= 0.7f;
				}
				float sin = cosf(m_fViewbobStep * 2.0f);
				float sin2 = sinf(m_fViewbobStep);
				float3 vec(1.0f, 0.0f, 0.0f);
				vec = m_vOrientation * vec;
				m_fViewbobY = (sin * ((m_uMoveDir & PM_RUN) ? *cl_bob_run_y : *cl_bob_walk_y));
				m_fViewbobStrafe = (float3)(vec * sin2 * ((m_uMoveDir & PM_RUN) ? *cl_bob_run_x : *cl_bob_walk_x));
				//m_vOrientation = SMQuaternion(SMToRadian(10) * sinf(m_fViewbobStep), 'z') * m_vOrientation;
			}


			//m_vWpnShakeAngles
			float3 vel = BTVEC_F3(m_pCharacter->getLinearVelocity());
			//printf("%f, %f, %f\n", vel.x, vel.y, vel.z);
			//vel = getDiscreteLinearVelocity();
			//printf("%f, %f, %f\n", vel.x, vel.y, vel.z);

			m_vWpnShakeAngles.x += -vel.y * 0.05f;
			const float fMaxAng = SM_PI * 0.1f;
			m_vWpnShakeAngles.x = clampf(m_vWpnShakeAngles.x, -fMaxAng, fMaxAng);
		}

		GameData::m_pHUDcontroller->setPlayerPos(m_vPosition);

	}

#ifndef _SERVER
	if(*grab_cursor && (!*editor_mode || SSInput_GetKeyState(SIM_LBUTTON)))
	{
		RECT rc;
		GetWindowRect(GetForegroundWindow(), &rc);
		SetCursorPos((rc.right + rc.left) / 2, (rc.bottom + rc.top) / 2);
	}
#endif
}

CCrosshair * CPlayer::getCrosshair()
{
	return(m_pCrosshair);
}

void CPlayer::move(UINT dir, bool start)
{
	if(start)
	{
		m_uMoveDir |= dir;
	}
	else
	{
		m_uMoveDir &= ~dir;
	}
}

CPointCamera * CPlayer::getCamera()
{
	return(m_pCamera);
}

void CPlayer::onSync()
{
	BaseClass::onSync();

	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}

	m_vPosition.y += m_fViewbobY;
	m_vPosition = (float3)(m_vPosition + m_fViewbobStrafe);
}

void CPlayer::observe()
{
	m_uMoveDir |= PM_OBSERVER;
	m_pCrosshair->enable(false);


	if(ID_VALID(m_idQuadCurr))
	{
		SAIG_QuadSetState(m_idQuadCurr, AIQUAD_STATE_FREE);
		m_idQuadCurr = -1;
	}
}

void CPlayer::spawn()
{
	CBaseEntity * pEnt = NULL;
	while((pEnt = m_pMgr->findEntityByClass("info_player_spawn", pEnt)))
	{
		//if(CanSpawn(pEnt))
		{
			setPos(pEnt->getPos());
			setOrient(pEnt->getOrient());
			m_uMoveDir &= ~PM_OBSERVER;
			m_pCrosshair->enable();

			GameData::m_pHUDcontroller->setPlayerRot(m_vPitchYawRoll);
			GameData::m_pHUDcontroller->setPlayerPos(m_vPosition);
			GameData::m_pHUDcontroller->setPlayerHealth(m_fHealth);
			return;
		}
	}
	printf(COLOR_RED "Cannot find valid spawnpoint\n" COLOR_RESET);
}

void CPlayer::_ccmd_slot_on(int argc, const char ** argv)
{
	if(argc != 2)
	{
		printf("Usage: \n    debug_slot_move <direction>\n    direction: ox oy oz rx ry rz\n");
	}
	if(argv[1][0] == 'o')
	{
		switch(argv[1][1])
		{
		case 'x': m_iDSM = DSM_POS_X; break;
		case 'y': m_iDSM = DSM_POS_Y; break;
		case 'z': m_iDSM = DSM_POS_Z; break;
		}
	}
	else if(argv[1][0] == 'r')
	{
		switch(argv[1][1])
		{
		case 'x': m_iDSM = DSM_ROT_X; break;
		case 'y': m_iDSM = DSM_ROT_Y; break;
		case 'z': m_iDSM = DSM_ROT_Z; break;
		}
	}
	else if(!strcmp(argv[1], "dump"))
	{
		m_iDSM = DSM_PRINT;
	}
}

void CPlayer::_ccmd_slot_off()
{
	m_iDSM = DSM_NONE;
}

float3_t & CPlayer::getWeaponDeltaAngles()
{
	return(m_vWpnShakeAngles);
}

void CPlayer::updateSpread(float dt)
{
	BaseClass::updateSpread(dt);
	if(m_pCrosshair)
	{
		m_pCrosshair->setSize(getCurrentSpread() * 0.1f);
	}
}

CHUDcontroller * CPlayer::getHUDcontroller()
{
	return(GameData::m_pHUDcontroller);
}

void CPlayer::dispatchDamage(CTakeDamageInfo &takeDamageInfo)
{
	BaseClass::dispatchDamage(takeDamageInfo);
	getHUDcontroller()->setPlayerHealth(m_fHealth);
	getHUDcontroller()->fadeScreenDmg();
}

void CPlayer::attack(BOOL state)
{
	BaseClass::attack(state);

	if(state)
	{
		respawn();
	}
}

void CPlayer::respawn()
{
	if(m_fHealth <= 0.0f && m_bCanRespawn)
	{
		m_fHealth = 100.0f;

		SXPhysics_GetDynWorld()->addCollisionObject(m_pGhostObject, CG_CHARACTER, CG_ALL & ~(CG_DEBRIS | CG_HITBOX | CG_WATER));
		SXPhysics_GetDynWorld()->addAction(m_pCharacter);
		spawn();
	}
}

void CPlayer::onDeath()
{
	BaseClass::onDeath();

	observe();

	m_bCanRespawn = true;
}
