#include <input/sxinput.h>

#include "SXplayer.h"

#include <decals/sxdecals.h>

BEGIN_PROPTABLE(SXplayer)
// empty
END_PROPTABLE()

REGISTER_ENTITY(SXplayer, player);

SXplayer::SXplayer(EntityManager * pMgr):
	BaseClass(pMgr),
	m_uMoveDir(PM_OBSERVER),
	m_vPitchYawRoll(float3_t(0, 0, 0)),
	m_bCanJump(true),
	m_fViewbobStep(0.0f),
	m_fViewbobY(0.0f),
	m_fViewbobStrafe(float3_t(0, 0, 0))
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
	m_pCollideShape = new btCapsuleShape(0.4f, 1.2f);
	m_pGhostObject->setCollisionShape(m_pCollideShape);
	m_pGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = 0.4f;
	m_pCharacter = new btKinematicCharacterController(m_pGhostObject, (btConvexShape*)m_pCollideShape, stepHeight, btVector3(0.0f, 1.0f, 0.0f));
	m_pCharacter->setMaxJumpHeight(0.60f);
	m_pCharacter->setJumpSpeed(3.50f);
	//m_pCharacter->setJumpSpeed(3.5f);
	m_pCharacter->setGravity(btVector3(0, -10.0f, 0));
	//m_pCharacter->setGravity(1.0f);
	m_pCharacter->setFallSpeed(300.0f);
	//m_pCharacter->setFallSpeed(30.0f);

	SXPhysics_GetDynWorld()->addCollisionObject(m_pGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	SXPhysics_GetDynWorld()->addAction(m_pCharacter);
}

SXplayer::~SXplayer()
{
	CLEAR_INTERVAL(m_iUpdIval);
	REMOVE_ENTITY(m_pCamera);
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

	if(!*editor_mode || SSInput_GetKeyState(SIM_LBUTTON))
	{
		SSInput_GetMouseDelta(&x, &y);

		float dx = (float)x * *sense * 10.0f /* / dt */;
		float dy = (float)y * *sense * 10.0f /* / dt */;
		//printf("%f %f : ", dx, dy);
		m_vPitchYawRoll.y -= dx;
		m_vPitchYawRoll.x -= dy;
		//printf(" %f %f\n", m_vPitchYawRoll.x, m_vPitchYawRoll.y);
		if(m_vPitchYawRoll.x > SM_PIDIV2)
		{
			m_vPitchYawRoll.x = SM_PIDIV2;
		}
		else if(m_vPitchYawRoll.x < -SM_PIDIV2)
		{
			m_vPitchYawRoll.x = -SM_PIDIV2;
		}
		while(m_vPitchYawRoll.y < 0.0f)
		{
			m_vPitchYawRoll.y += SM_2PI;
		}
		while(m_vPitchYawRoll.y > SM_2PI)
		{
			m_vPitchYawRoll.y -= SM_2PI;
		}

		m_vOrientation = SMQuaternion(m_vPitchYawRoll.x, 'x')
			* SMQuaternion(m_vPitchYawRoll.y, 'y')
			* SMQuaternion(m_vPitchYawRoll.z, 'z');
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
					if(m_bCanJump)
					{
						m_pCharacter->jump();
						m_bCanJump = false;
					}
				}
				else
				{
					m_bCanJump = false;
				}
			}
			else
			{
				m_bCanJump = true;
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
					if(m_uMoveDir & PM_RUN)
					{
						m_fViewbobStep += dt * *cl_bob_run * 0.2f;
					}
					else
					{
						m_fViewbobStep += dt * *cl_bob_walk;
					}
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
}

float3 SXplayer::GetWeaponOrigin()
{
	//@TODO: Implement me
	return(m_vPosition);
}

void SXplayer::Spawn()
{
	SXbaseEntity * pEnt;
	pEnt = m_pMgr->FindEntityByClass("info_player_spawn");
	if(pEnt/* && CanSpawn(pEnt)*/)
	{
		SetPos(pEnt->GetPos());
		SetOrient(pEnt->GetOrient());
		m_uMoveDir &= ~PM_OBSERVER;
	}
	else
	{
		printf(COLOR_RED "Cannot find valid spawnpoint\n" COLOR_RESET);
	}
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
	if(state)
	{
		//trace line
		float3 start = GetWeaponOrigin();
		float3 end = start + (m_vOrientation * float3(0.0f, 0.0f, 1.0f)) * 1000.0f;
		btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
		SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

		if(cb.hasHit())
		{
			//shoot decal
			SXDecals_ShootDecal(DECAL_TYPE_CONCRETE, BTVEC_F3(cb.m_hitPointWorld), BTVEC_F3(cb.m_hitNormalWorld));
		}
	}
}

void SXplayer::Attack2(BOOL state)
{
}
