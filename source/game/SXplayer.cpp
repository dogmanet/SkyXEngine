#include <input/sxinput.h>
#include <mtllight/sxmtllight.h>
#include "SXplayer.h"
#include "LightDirectional.h"
#include "SXbaseTool.h"
#include <aigrid/sxaigrid.h>

#include "GameData.h"

/*! \skydocent player
Объект игрока в мире
*/

BEGIN_PROPTABLE(SXplayer)
// empty
END_PROPTABLE()

REGISTER_ENTITY(SXplayer, player);

class btKinematicClosestNotMeRayResultCallback: public btCollisionWorld::ClosestRayResultCallback
{
public:
	btKinematicClosestNotMeRayResultCallback(btCollisionObject* me, const btVector3&	rayFromWorld, const btVector3&	rayToWorld): btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld)
	{
		m_me = me;
		m_shapeInfo = {-1, -1};
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
	{
		if(rayResult.m_collisionObject == m_me)
			return 1.0;
		if(rayResult.m_localShapeInfo)
		{
			m_shapeInfo = *rayResult.m_localShapeInfo;
		}
		return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
	}
	btCollisionWorld::LocalShapeInfo m_shapeInfo;
protected:
	btCollisionObject* m_me;
};

SXplayer::SXplayer(EntityManager * pMgr):
	BaseClass(pMgr),
	m_uMoveDir(PM_OBSERVER),
	m_vPitchYawRoll(float3_t(0, 0, 0)),
	m_canJump(true),
	m_fViewbobStep(0.0f),
	m_fViewbobY(0.0f),
	m_fViewbobStrafe(float3_t(0, 0, 0)),
	m_vWpnShakeAngles(float3_t(0.0f, 0.0f, 0.0f)),
	m_pActiveTool(NULL),
	m_iDSM(DSM_NONE)
{
	m_pCamera = (SXpointCamera*)CREATE_ENTITY("point_camera", pMgr);
	m_pCamera->SetParent(this);

	m_iUpdIval = SET_INTERVAL(UpdateInput, 0);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(F3_BTVEC(m_vPosition));
	//startTransform.setOrigin(btVector3(0, 12, 10));

	m_pGhostObject = new btPairCachingGhostObject();
	m_pGhostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	m_pCollideShape = new btCapsuleShape(0.4f, 1.0f);
	m_pGhostObject->setCollisionShape(m_pCollideShape);
	m_pGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	m_pGhostObject->setUserPointer(this);

	btScalar stepHeight = 0.4f;
	m_pCharacter = new btKinematicCharacterController(m_pGhostObject, (btConvexShape*)m_pCollideShape, stepHeight, btVector3(0.0f, 1.0f, 0.0f));
	m_pCharacter->setMaxJumpHeight(0.60f);
	m_pCharacter->setJumpSpeed(3.50f);
	//m_pCharacter->setJumpSpeed(3.5f);
	m_pCharacter->setGravity(btVector3(0, -10.0f, 0));
	//m_pCharacter->setGravity(1.0f);
	m_pCharacter->setFallSpeed(300.0f);
	//m_pCharacter->setFallSpeed(30.0f);

	SXPhysics_GetDynWorld()->addCollisionObject(m_pGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::DebrisFilter);

	m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	SXPhysics_GetDynWorld()->addAction(m_pCharacter);



/*
	m_pActiveTool = (SXbaseTool*)CREATE_ENTITY("weapon_ak74", m_pMgr);
	m_pActiveTool->SetOwner(this);
	m_pActiveTool->AttachHands();
	m_pActiveTool->PlayAnimation("idle");
	m_pActiveTool->SetPos(GetPos() + float3(1.0f, 0.0f, 1.0f));
	m_pActiveTool->SetOrient(GetOrient());
	m_pActiveTool->SetParent(this);
	*/


	m_flashlight = (CLightDirectional*)CREATE_ENTITY("light_directional", m_pMgr);
	//m_flashlight->SetPos(GetPos() + float3(0.f, 0.1f, 0.f));
	m_flashlight->SetPos(GetPos() + float3(0.f, 0.2f, 0.1f));
	m_flashlight->SetOrient(GetOrient() * SMQuaternion(SM_PIDIV2, 'x'));
	m_flashlight->SetParent(this);
	m_flashlight->setDist(20.f);
	m_flashlight->setAngle(SMToRadian(60));
	m_flashlight->setColor(float3(3.5, 3.5, 3.5));
	//m_flashlight->setShadowType(-1);
	m_flashlight->setShadowType(1);

	m_idQuadCurr = -1;

	m_pCrosshair = new Crosshair();
}

SXplayer::~SXplayer()
{
	mem_delete(m_pCrosshair);
	CLEAR_INTERVAL(m_iUpdIval);
	REMOVE_ENTITY(m_pCamera);
}

void SXplayer::playFootstepsSound()
{
	if(!(m_uMoveDir & PM_OBSERVER))
	{
		if(onGround())
		{
			float3 start = GetPos(),
				end = start + float3(0.0f, -2.0f, 0.0f);
			btKinematicClosestNotMeRayResultCallback cb(m_pGhostObject, F3_BTVEC(start), F3_BTVEC(end));
			SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

			if(cb.hasHit() && cb.m_shapeInfo.m_shapePart == 0 && cb.m_shapeInfo.m_triangleIndex >= 0)
			{
				MTLTYPE_PHYSIC type = (MTLTYPE_PHYSIC)SXPhysics_GetMtlType(cb.m_collisionObject, &cb.m_shapeInfo);
				g_pGameData->playFootstepSound(type, BTVEC_F3(cb.m_hitPointWorld));
			}
		}
	}
}

void SXplayer::UpdateInput(float dt)
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
			m_pActiveTool->DbgMove(m_iDSM, dy);
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

			m_vOrientation = SMQuaternion(m_vPitchYawRoll.x, 'x')
				* SMQuaternion(m_vPitchYawRoll.y, 'y')
				* SMQuaternion(m_vPitchYawRoll.z, 'z');
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
			m_vPosition = (float3)(m_vPosition + m_vOrientation * (SMVector3Normalize(dir) * dt));
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

Crosshair * SXplayer::GetCrosshair()
{
	return(m_pCrosshair);
}

void SXplayer::Move(UINT dir, bool start)
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

SXpointCamera * SXplayer::GetCamera()
{
	return(m_pCamera);
}

void SXplayer::OnSync()
{
	BaseClass::OnSync();
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	btTransform trans;
	trans = m_pGhostObject->getWorldTransform();
	
	m_vPosition = (float3)(float3(trans.getOrigin().x(), trans.getOrigin().y() + 0.75f + m_fViewbobY, trans.getOrigin().z()) + m_fViewbobStrafe);

	//находим текущий квад аи сетки на котором находится игрок
	ID idq = SAIG_QuadGet(&float3(m_vPosition.x, m_vPosition.y - (0.75f + m_fViewbobY), m_vPosition.z), true);
	
	//если нашли
	if (idq >= 0)
	{
		//занимаем этот квад
		SAIG_QuadSetState(idq, AIQUAD_STATE_TEMPBUSY);
		SAIG_QuadSetStateWho(idq, GetId());
	}

	//если предыдущий и текущие квады не идентичны
	if (m_idQuadCurr != idq)
	{
		//если предыдщий был действительным, убираем занятость
		if (m_idQuadCurr >= 0)
			SAIG_QuadSetState(m_idQuadCurr, AIQUAD_STATE_FREE);

		m_idQuadCurr = idq;
	}
}

float3 SXplayer::GetWeaponOrigin()
{
	//@TODO: Implement me
	return(m_vPosition);
}

void SXplayer::Spawn()
{
	SXbaseEntity * pEnt = NULL;
	while((pEnt = m_pMgr->FindEntityByClass("info_player_spawn", pEnt)))
	{
		//if(CanSpawn(pEnt))
		{
		SetPos(pEnt->GetPos());
		SetOrient(pEnt->GetOrient());
		m_uMoveDir &= ~PM_OBSERVER;
			m_pCrosshair->Enable();
			return;
		}
	}
	
		printf(COLOR_RED "Cannot find valid spawnpoint\n" COLOR_RESET);
	}

void SXplayer::SetPos(const float3 & pos)
{
	BaseClass::SetPos(pos);
	m_pGhostObject->getWorldTransform().setOrigin(F3_BTVEC(pos));
}

void SXplayer::Attack(BOOL state)
{
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	if(m_pActiveTool)
	{
		m_pActiveTool->PrimaryAction(state);
	}
}

void SXplayer::Attack2(BOOL state)
{
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	if(m_pActiveTool)
	{
		m_pActiveTool->SecondaryAction(state);
	}
}

void SXplayer::Reload()
{
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	if(m_pActiveTool)
	{
		m_pActiveTool->Reload();
	}
}

void SXplayer::ToggleFlashlight()
{
	m_flashlight->toggleEnable();
}

void SXplayer::nextFireMode()
{
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	if(m_pActiveTool)
	{
		//@FIXME: Add correct call
		//m_pActiveTool->nextFireMode();
	}
}

void SXplayer::_ccmd_slot_on(int argc, const char ** argv)
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

void SXplayer::_ccmd_slot_off()
{
	m_iDSM = DSM_NONE;
}

float3_t & SXplayer::GetWeaponDeltaAngles()
{
	return(m_vWpnShakeAngles);
}

bool SXplayer::onGround()
{
	return(m_pCharacter->onGround());
}
