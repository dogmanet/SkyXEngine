#include "SXbaseTool.h"

#include <particles/sxparticles.h>
#include <decals/sxdecals.h>
#include "SXplayer.h"

BEGIN_PROPTABLE(SXbaseTool)
	DEFINE_FIELD_FLOAT(m_fReloadTime, PDFF_NOEDIT | PDFF_NOEXPORT, "reload_time", "", EDITOR_NONE)
	DEFINE_FIELD_FLOAT(m_fZoomTime, PDFF_NOEDIT | PDFF_NOEXPORT, "zoom_time", "", EDITOR_NONE)

	DEFINE_FIELD_VECTOR(m_vSlotPos, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_offset", "", EDITOR_NONE)
	DEFINE_FIELD_ANGLES(m_qSlotRot, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_rotation", "", EDITOR_NONE)
	DEFINE_FIELD_VECTOR(m_vSlotPosAim, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_offset_aim", "", EDITOR_NONE)
	DEFINE_FIELD_ANGLES(m_qSlotRotAim, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_rotation_aim", "", EDITOR_NONE)
	
	DEFINE_FIELD_INT(m_iZoomable, PDFF_NOEDIT | PDFF_NOEXPORT, "zoomable", "", EDITOR_NONE)

	DEFINE_FIELD_STRING(m_szPrimaryActionSound, PDFF_NOEDIT | PDFF_NOEXPORT, "action1_sound", "", EDITOR_NONE)
	DEFINE_FIELD_STRING(m_szSecondaryActionSound, PDFF_NOEDIT | PDFF_NOEXPORT, "action2_sound", "", EDITOR_NONE)
	DEFINE_FIELD_STRING(m_szPrimaryActionMuzzleflash, PDFF_NOEDIT | PDFF_NOEXPORT, "action1_muzzle", "", EDITOR_NONE)
	DEFINE_FIELD_STRING(m_szSecondaryActionMuzzleflash, PDFF_NOEDIT | PDFF_NOEXPORT, "action2_muzzle", "", EDITOR_NONE)
	
	DEFINE_FIELD_FLOAT(m_fMaxDistance, PDFF_NOEDIT | PDFF_NOEXPORT, "max_distance", "", EDITOR_NONE)
	DEFINE_FIELD_STRING(m_szUsableAmmos, PDFF_NOEDIT | PDFF_NOEXPORT, "ammos", "", EDITOR_NONE)

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
	m_fZoomProgress(0.0f),
	m_iZoomable(1),
	m_iSoundAction1(-1),
	m_iSoundAction2(-1),
	m_iMuzzleFlash(-1),
	m_iMuzzleFlash2(-1),
	m_fMaxDistance(1000.0f)
{
	m_bInvStackable = false;

	m_iIvalUpdate = SET_INTERVAL(_Update, 0);
}

SXbaseTool::~SXbaseTool()
{
	CLEAR_INTERVAL(m_iIvalUpdate);
}

void SXbaseTool::OnPostLoad()
{
	BaseClass::OnPostLoad();

	if(m_szPrimaryActionSound[0])
	{
		m_iSoundAction1 = SSCore_SndCreate3dInst(m_szPrimaryActionSound, false, 0, 100);
	}
	if(m_szSecondaryActionSound[0])
	{
		m_iSoundAction2 = SSCore_SndCreate3dInst(m_szSecondaryActionSound, false, 0, 100);
	}
	if(m_szPrimaryActionMuzzleflash[0])
	{
		m_iMuzzleFlash = SPE_EffectInstanceByName(m_szPrimaryActionMuzzleflash);
	}
	if(m_szSecondaryActionMuzzleflash[0])
	{
		m_iMuzzleFlash2 = SPE_EffectInstanceByName(m_szSecondaryActionMuzzleflash);
	}
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
		if(ID_VALID(m_iMuzzleFlash))
		{
			SPE_EffectEnableSet(m_iMuzzleFlash, true);
		}
		if(ID_VALID(m_iSoundAction1))
		{
			SSCore_SndInstancePlay3d(m_iSoundAction1, &GetPos());
		}

		//((SXplayer*)m_pOwner)->is

		//trace line
		float3 start = GetPos();
		float3 dir = m_pParent->GetOrient() * float3(0.0f, 0.0f, 1.0f);
		float3 end = start + dir * m_fMaxDistance;
		btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
		SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

		if(cb.hasHit())
		{
			//shoot decal
			SXDecals_ShootDecal(DECAL_TYPE_CONCRETE, BTVEC_F3(cb.m_hitPointWorld), BTVEC_F3(cb.m_hitNormalWorld));
			SPE_EffectPlayByName("fire", &BTVEC_F3(cb.m_hitPointWorld), &BTVEC_F3(cb.m_hitNormalWorld));
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
	if(m_iZoomable)
	{
		((SXplayer*)m_pOwner)->GetCrosshair()->Enable(!st);
	}
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
		m_qSlotRotResult = m_qSlotRotResult * SMQuaternion(dy, 'x');
		break;
	case DSM_ROT_Y:
		m_qSlotRotResult = m_qSlotRotResult * SMQuaternion(dy, 'y');
		break;
	case DSM_ROT_Z:
		m_qSlotRotResult = m_qSlotRotResult * SMQuaternion(dy, 'z');
		break;

	case DSM_PRINT:
		printf(COLOR_GREEN "offset: " COLOR_LGREEN "%f %f %f\n"
			COLOR_GREEN "orient: " COLOR_LGREEN "%f %f %f %f\n" COLOR_RESET
			, m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z
			, m_qSlotRotResult.x, m_qSlotRotResult.y, m_qSlotRotResult.z, m_qSlotRotResult.w);
		break;
	}
}

void SXbaseTool::OnSync()
{
	float3_t ang = ((SXplayer*)m_pOwner)->GetWeaponDeltaAngles();
	m_vOffsetOrient = m_qSlotRotResult * SMQuaternion(ang.x, 'x') * SMQuaternion(ang.y, 'y') * SMQuaternion(ang.z, 'z');
	BaseClass::OnSync();
	if(m_pAnimPlayer)
	{
		//SPE_EffectPlayByID
		float3 pos = m_pAnimPlayer->GetBoneTransformPos(m_pAnimPlayer->GetBone("muzzle_rifle1"));
		SPE_EffectPosSet(m_iMuzzleFlash, &pos);
		//pos = m_vOrientation * float3(0, 0, 1);
		SPE_EffectRotSetQ(m_iMuzzleFlash, m_vOrientation);
	}
}

void SXbaseTool::_Update(float dt)
{
	float speed = 1.0f / m_fZoomTime;
	if(m_bInSecondaryAction && m_iZoomable)
	{
		if(m_fZoomProgress < 1.0f)
		{
			m_fZoomProgress += dt * speed;
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
			m_fZoomProgress -= dt * speed;
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
	const float * r_default_fov = GET_PCVAR_FLOAT("r_default_fov");
	m_vOffsetPos = (float3)vlerp(m_vSlotPos, m_vSlotPosAim, m_fZoomProgress);
	//m_vWpnShakeAngles
	m_qSlotRotResult = SMquaternionSlerp(m_qSlotRot, m_qSlotRotAim, m_fZoomProgress);
	if(m_pOwner)
	{
		((SXplayer*)m_pOwner)->GetCamera()->GetCamera()->SetFOV(SMToRadian(vlerp(*r_default_fov, *r_default_fov - 10.0f, m_fZoomProgress)));
	}
}
