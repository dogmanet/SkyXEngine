
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseTool.h"

#include <particles/sxparticles.h>
#include <decals/sxdecals.h>
#include "Player.h"

/*! \skydocent base_tool
Базовый класс для инструмента (в т.ч оружие). То, что игрок может взять в руки и использовать
*/


BEGIN_PROPTABLE(CBaseTool)
	//! Время перезарядки, с
	DEFINE_FIELD_FLOAT(m_fReloadTime, PDFF_NOEDIT | PDFF_NOEXPORT, "reload_time", "", EDITOR_NONE)
	//! Время прицеливания, с
	DEFINE_FIELD_FLOAT(m_fZoomTime, PDFF_NOEDIT | PDFF_NOEXPORT, "zoom_time", "", EDITOR_NONE)

	//! Смещение визуальной модели
	DEFINE_FIELD_VECTOR(m_vSlotPos, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_offset", "", EDITOR_NONE)
	//! Вращение визуальной модели
	DEFINE_FIELD_ANGLES(m_qSlotRot, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_rotation", "", EDITOR_NONE)
	//! Смещение визуальной модели в прицеливании
	DEFINE_FIELD_VECTOR(m_vSlotPosAim, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_offset_aim", "", EDITOR_NONE)
	//! Вращение визуальной модели в прицеливании
	DEFINE_FIELD_ANGLES(m_qSlotRotAim, PDFF_NOEDIT | PDFF_NOEXPORT, "slot_rotation_aim", "", EDITOR_NONE)
	
	//! Разрешить прицеливание
	DEFINE_FIELD_INT(m_iZoomable, PDFF_NOEDIT | PDFF_NOEXPORT, "zoomable", "", EDITOR_NONE)

	//! Звук первичного действия
	DEFINE_FIELD_STRING(m_szPrimaryActionSound, PDFF_NOEDIT | PDFF_NOEXPORT, "action1_sound", "", EDITOR_NONE)
	//! Звук вторичного действия
	DEFINE_FIELD_STRING(m_szSecondaryActionSound, PDFF_NOEDIT | PDFF_NOEXPORT, "action2_sound", "", EDITOR_NONE)
	//! Эффект первичного действия
	DEFINE_FIELD_STRING(m_szPrimaryActionMuzzleflash, PDFF_NOEDIT | PDFF_NOEXPORT, "action1_muzzle", "", EDITOR_NONE)
	//! Эффект вторичного действия
	DEFINE_FIELD_STRING(m_szSecondaryActionMuzzleflash, PDFF_NOEDIT | PDFF_NOEXPORT, "action2_muzzle", "", EDITOR_NONE)
	
	//! Максимальная дальность
	DEFINE_FIELD_FLOAT(m_fMaxDistance, PDFF_NOEDIT | PDFF_NOEXPORT, "max_distance", "", EDITOR_NONE)
	//! Подходящие типы припасов, классы, через запятую
	DEFINE_FIELD_STRING(m_szUsableAmmos, PDFF_NOEDIT | PDFF_NOEXPORT, "ammos", "", EDITOR_NONE)
	//! Класс заряженного в данный момент припаса
	DEFINE_FIELD_STRING(m_szLoadedAmmo, PDFF_NOEDIT | PDFF_NOEXPORT, "loaded_ammo", "", EDITOR_NONE)

END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseTool, base_tool);

CBaseTool::CBaseTool(CEntityManager * pMgr):
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
	m_fMaxDistance(1000.0f),
	m_bIsWeapon(false),
	m_pLoadedAmmo(NULL)
{
	m_bInvStackable = false;

	m_iIvalUpdate = SET_INTERVAL(_update, 0);
}

CBaseTool::~CBaseTool()
{
	CLEAR_INTERVAL(m_iIvalUpdate);
}

void CBaseTool::onPostLoad()
{
	BaseClass::onPostLoad();

	if(m_szPrimaryActionSound[0])
	{
		m_iSoundAction1 = SSCore_SndCreate3dInst(m_szPrimaryActionSound, SX_SOUND_CHANNEL_GAME, 100);
	}
	if(m_szSecondaryActionSound[0])
	{
		m_iSoundAction2 = SSCore_SndCreate3dInst(m_szSecondaryActionSound, SX_SOUND_CHANNEL_GAME, 100);
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

void CBaseTool::setNextUse(float time)
{
	m_bCanUse = false;
	SET_TIMEOUT(_allowUse, time);
}
bool CBaseTool::canUse()
{
	return(m_bCanUse && !m_bInPrimaryAction);
}

void CBaseTool::primaryAction(BOOL st)
{
	m_bInPrimaryAction = st != FALSE;
	if(st)
	{
		playAnimation("shoot1");
		if(ID_VALID(m_iMuzzleFlash))
		{
			SPE_EffectSetEnable(m_iMuzzleFlash, true);
		}
		if(ID_VALID(m_iSoundAction1))
		{
			SSCore_SndInstancePlay3d(m_iSoundAction1, false, false, &getPos());
		}

		//((CPlayer*)m_pOwner)->is

		//trace line
		float3 start = getPos();
		float3 dir = m_pParent->getOrient() * float3(0.0f, 0.0f, 1.0f);
		float3 end = start + dir * m_fMaxDistance;
		btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
		SPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

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

void CBaseTool::secondaryAction(BOOL st)
{
	m_bInSecondaryAction = st != FALSE;
	if(m_iZoomable)
	{
		((CPlayer*)m_pOwner)->getCrosshair()->enable(!st);
	}
}

void CBaseTool::setIsWorldModel(bool b)
{
	m_bWorldModel = b;
}

void CBaseTool::reload()
{
	if(canUse())
	{
		setNextUse(m_fReloadTime);
		playAnimation("reload");
	}
}

void CBaseTool::attachHands()
{
	//@TODO: Reimplement me
#if 0
	if(m_pAnimPlayer)
	{
		m_pAnimPlayer->addModel("models/weapons/hands.dse");
		m_pAnimPlayer->assembly();
	}
#endif
}

void CBaseTool::dbgMove(int dir, float dy)
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

void CBaseTool::onSync()
{
	float3_t ang = ((CPlayer*)m_pOwner)->getWeaponDeltaAngles();
	m_vOffsetOrient = m_qSlotRotResult * SMQuaternion(ang.x, 'x') * SMQuaternion(ang.y, 'y') * SMQuaternion(ang.z, 'z');
	BaseClass::onSync();
	if(m_pModel && m_pModel->asAnimatedModel())
	{
		//SPE_EffectPlayByID
		float3 pos = m_pModel->asAnimatedModel()->getBoneTransformPos(m_pModel->asAnimatedModel()->getBoneId("muzzle_rifle1"));
		SPE_EffectSetPos(m_iMuzzleFlash, &pos);
		//pos = m_vOrientation * float3(0, 0, 1);
		SPE_EffectSetRotQ(m_iMuzzleFlash, m_vOrientation);
	}
}

void CBaseTool::_update(float dt)
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
			_rezoom();
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
			_rezoom();
		}
	}
}

void CBaseTool::setParent(CBaseEntity * pEnt, int attachment)
{
	BaseClass::setParent(pEnt, attachment);

	_rezoom();
}

void CBaseTool::_rezoom()
{
	const float * r_default_fov = GET_PCVAR_FLOAT("r_default_fov");
	m_vOffsetPos = (float3)vlerp(m_vSlotPos, m_vSlotPosAim, m_fZoomProgress);
	//m_vWpnShakeAngles
	m_qSlotRotResult = SMquaternionSlerp(m_qSlotRot, m_qSlotRotAim, m_fZoomProgress);
	if(m_pOwner && m_pOwner->getClassName() && !fstrcmp(m_pOwner->getClassName(), "player"))
	{
		((CPlayer*)m_pOwner)->getCamera()->getCamera()->setFOV(SMToRadian(vlerp(*r_default_fov, *r_default_fov - 10.0f, m_fZoomProgress)));
	}
}

bool CBaseTool::isWeapon() const
{
	return(m_bIsWeapon);
}

float CBaseTool::getCondition() const
{
	return(1.0f);
}

CBaseSupply *CBaseTool::getAmmo() const
{
	return(m_pLoadedAmmo);
}

void CBaseTool::chargeAmmo(CBaseSupply *pAmmo)
{
	if(!pAmmo || !isValidAmmo(pAmmo))
	{
		return;
	}
	if(getAmmo())
	{
		uncharge();
	}
	m_pLoadedAmmo = pAmmo;
	m_pLoadedAmmo->setParent(this);
	_setStrVal(&m_szLoadedAmmo, m_pLoadedAmmo->getClassName());
}

void CBaseTool::uncharge()
{
	if(!getAmmo())
	{
		return;
	}
	_setStrVal(&m_szLoadedAmmo, "");
	m_pLoadedAmmo->setParent(NULL);
	//getOwner()->getInventory()->put(getAmmo());
	m_pLoadedAmmo = NULL;
}

bool CBaseTool::isValidAmmo(CBaseSupply *pAmmo)
{
	const char * str = strstr(m_szUsableAmmos, pAmmo->getClassName());
	if(str)
	{
		char chr = str[strlen(pAmmo->getClassName())];
		return(chr == ',' || chr == 0);
	}
	return(false);
}

float CBaseTool::getMaxDistance() const
{
	return(m_fMaxDistance);
}

void CBaseTool::stopAction()
{
}
