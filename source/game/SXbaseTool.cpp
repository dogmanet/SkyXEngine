#include "SXbaseTool.h"

#include <particles/sxparticles.h>
#include <decals/sxdecals.h>

BEGIN_PROPTABLE(SXbaseTool)
	DEFINE_FIELD_FLOAT(m_fReloadTime, PDFF_NOEDIT | PDFF_NOEXPORT, "reload_time", "", EDITOR_NONE)

	DEFINE_FIELD_VECTOR(m_vSlotPos, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_offset", "", EDITOR_NONE)
	DEFINE_FIELD_ANGLES(m_qSlotRot, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_rotation", "", EDITOR_NONE)
	DEFINE_FIELD_VECTOR(m_vSlotPosAim, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_offset_aim", "", EDITOR_NONE)
	DEFINE_FIELD_ANGLES(m_qSlotRotAim, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_rotation_aim", "", EDITOR_NONE)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseTool, base_tool);

SXbaseTool::SXbaseTool(EntityManager * pMgr):
	BaseClass(pMgr),
	m_bInPrimaryAction(false),
	m_bInSecondaryAction(false),
	m_bWorldModel(false),
	m_bCanUse(true),
	m_fZoomTime(0.0f),
	m_fReloadTime(0.0f),
	m_fZoomProgress(0.0f)
{
	m_bInvStackable = false;

	m_iMuzzleFlash = SPE_EffectCopyName("muzzleflash_ak74");

	m_iIvalUpdate = SET_INTERVAL(_Update, 0);
}

SXbaseTool::~SXbaseTool()
{
	CLEAR_INTERVAL(m_iIvalUpdate);
}

void SXbaseTool::SetNextUse(float time)
{
	m_bCanUse = false;
	SET_TIMEOUT(_AllowUse, time);
}
bool SXbaseTool::CanUse()
{
	return(m_bCanUse);
}

void SXbaseTool::PrimaryAction(BOOL st)
{
	m_bInPrimaryAction = st != FALSE;
	if(st)
	{
		PlayAnimation("shoot1");
		SPE_EffectEnableSet(m_iMuzzleFlash, true);

		//trace line
		float3 start = GetPos();
		float3 dir = m_pParent->GetOrient() * float3(0.0f, 0.0f, 1.0f);
		float3 end = start + dir * 1000.0f;
		btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
		SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

		if(cb.hasHit())
		{
			//shoot decal
			SXDecals_ShootDecal(DECAL_TYPE_CONCRETE, BTVEC_F3(cb.m_hitPointWorld), BTVEC_F3(cb.m_hitNormalWorld));
			if(!cb.m_collisionObject->isStaticOrKinematicObject())
			{
				((btRigidBody*)cb.m_collisionObject)->applyCentralImpulse(F3_BTVEC(dir * 10.0f));
				cb.m_collisionObject->activate();
			}
		}
	}
}

void SXbaseTool::SecondaryAction(BOOL st)
{
	m_bInSecondaryAction = st != FALSE;
}

void SXbaseTool::SetIsWorldModel(bool b)
{
	m_bWorldModel = b;
}

void SXbaseTool::Reload()
{
	if(CanUse())
	{
		SetNextUse(m_fReloadTime);
		PlayAnimation("reload");
	}
}

void SXbaseTool::AttachHands()
{
	if(m_pAnimPlayer)
	{
		m_pAnimPlayer->AddModel("models/weapons/hands.dse");
		m_pAnimPlayer->Assembly();
	}
}

void SXbaseTool::DbgMove(int dir, float dy)
{
	switch(dir)
	{
	case DSM_POS_X:
		m_vOffsetPos.x += dy;
		break;
	case DSM_POS_Y:
		m_vOffsetPos.y += dy;
		break;
	case DSM_POS_Z:
		m_vOffsetPos.z += dy;
		break;
	case DSM_ROT_X:
		m_vOffsetOrient = m_vOffsetOrient * SMQuaternion(dy, 'x');
		break;
	case DSM_ROT_Y:
		m_vOffsetOrient = m_vOffsetOrient * SMQuaternion(dy, 'y');
		break;
	case DSM_ROT_Z:
		m_vOffsetOrient = m_vOffsetOrient * SMQuaternion(dy, 'z');
		break;

	case DSM_PRINT:
		printf(COLOR_GREEN "offset: " COLOR_LGREEN "%f %f %f\n"
			COLOR_GREEN "orient: " COLOR_LGREEN "%f %f %f %f\n" COLOR_RESET
			, m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z
			, m_vOffsetOrient.x, m_vOffsetOrient.y, m_vOffsetOrient.z, m_vOffsetOrient.w);
		break;
	}
}

void SXbaseTool::OnSync()
{
	BaseClass::OnSync();
	float3 pos = m_pAnimPlayer->GetBoneTransformPos(m_pAnimPlayer->GetBone("muzzle_rifle1"));
	SPE_EffectPosSet(m_iMuzzleFlash, &pos);
	pos = m_vOrientation * float3(0, 0, 1);
	SPE_EffectDirSet(m_iMuzzleFlash, &pos);
}

void SXbaseTool::_Update(float dt)
{
	if(m_bInSecondaryAction)
	{
		if(m_fZoomProgress < 1.0f)
		{
			m_fZoomProgress += dt * 8.0f;
			if(m_fZoomProgress > 1.0f)
			{
				m_fZoomProgress = 1.0f;
			}
			_Rezoom();
		}
	}
	else
	{
		if(m_fZoomProgress > 0.0f)
		{
			m_fZoomProgress -= dt * 8.0f;
			if(m_fZoomProgress < 0.0f)
			{
				m_fZoomProgress = 0.0f;
			}
			_Rezoom();
		}
	}
}

void SXbaseTool::SetParent(SXbaseEntity * pEnt, int attachment)
{
	BaseClass::SetParent(pEnt, attachment);

	_Rezoom();
}

void SXbaseTool::_Rezoom()
{
	m_vOffsetPos = (float3)vlerp(m_vSlotPos, m_vSlotPosAim, m_fZoomProgress);
	m_vOffsetOrient = SMquaternionSlerp(m_qSlotRot, m_qSlotRotAim, m_fZoomProgress);
}
