
#include <particles/sxparticles.h>
#include "SXbaseWeapon.h"
#include "SXplayer.h"

/*! \skydocent base_weapon
Базовый класс для оружия
*/


BEGIN_PROPTABLE(SXbaseWeapon)
	//! Совместимые прицелы, классы через запятую
	DEFINE_FIELD_STRING(m_szAddonScopes, PDFF_NOEDIT | PDFF_NOEXPORT, "addon_scopes", "", EDITOR_NONE)
	//! Совместимые глушители
	DEFINE_FIELD_STRING(m_szAddonSilencers, PDFF_NOEDIT | PDFF_NOEXPORT, "addon_silencers", "", EDITOR_NONE)
	//! Совместимые магазины
	DEFINE_FIELD_STRING(m_szAddonMags, PDFF_NOEDIT | PDFF_NOEXPORT, "addon_mags", "", EDITOR_NONE)
	//! Совместимые ручки
	DEFINE_FIELD_STRING(m_szAddonHandlers, PDFF_NOEDIT | PDFF_NOEXPORT, "addon_handlers", "", EDITOR_NONE)

	//! Доступные режимы стрельбы
	DEFINE_FIELD_STRING(m_szFireModes, PDFF_NOEDIT | PDFF_NOEXPORT, "fire_modes", "", EDITOR_NONE)
	//! Скорострельность одиночными
	DEFINE_FIELD_INT(m_iSingleSpeed, PDFF_NOEDIT | PDFF_NOEXPORT, "single_speed", "", EDITOR_NONE)
	//! Скорострельность в автоматическом режиме
	DEFINE_FIELD_INT(m_iBurstSpeed, PDFF_NOEDIT | PDFF_NOEXPORT, "burst_speed", "", EDITOR_NONE)
	//! Скорострельность отсечками
	DEFINE_FIELD_INT(m_iCutoffSpeed, PDFF_NOEDIT | PDFF_NOEXPORT, "cutoff_speed", "", EDITOR_NONE)
	//! Патронов в отсечке
	DEFINE_FIELD_INT(m_iCutoffSize, PDFF_NOEDIT | PDFF_NOEXPORT, "cutoff_size", "", EDITOR_NONE)
	//! Текущий режим стрельбы
	DEFINE_FIELD_INT(m_fireMode, PDFF_NOEDIT, "fire_mode", "", EDITOR_NONE)

	//! Звук извлечения
	DEFINE_FIELD_STRING(m_szSndDraw, PDFF_NOEDIT | PDFF_NOEXPORT, "snd_draw", "", EDITOR_NONE)
	//! Звук убирания
	DEFINE_FIELD_STRING(m_szSndHolster, PDFF_NOEDIT | PDFF_NOEXPORT, "snd_holster", "", EDITOR_NONE)
	//! Звук выстрела
	DEFINE_FIELD_STRING(m_szSndShoot, PDFF_NOEDIT | PDFF_NOEXPORT, "snd_shoot", "", EDITOR_NONE)
	//! Звук попытки выстрела без патрона
	DEFINE_FIELD_STRING(m_szSndEmpty, PDFF_NOEDIT | PDFF_NOEXPORT, "snd_empty", "", EDITOR_NONE)
	//! Звук перезарядки
	DEFINE_FIELD_STRING(m_szSndReload, PDFF_NOEDIT | PDFF_NOEXPORT, "snd_reload", "", EDITOR_NONE)
	//! Звук переключения
	DEFINE_FIELD_STRING(m_szSndSwitch, PDFF_NOEDIT | PDFF_NOEXPORT, "snd_switch", "", EDITOR_NONE)

	//! Эффективная дистанция, км
	DEFINE_FIELD_FLOAT(m_fEffectiveDistance, PDFF_NOEDIT | PDFF_NOEXPORT, "effective_distance", "", EDITOR_NONE)

	//! Вместимость патронов без магазина
	DEFINE_FIELD_INT(m_iCapacity, PDFF_NOEDIT | PDFF_NOEXPORT, "capacity", "", EDITOR_NONE)
	//! Текущая загрузка без учета магазина
	DEFINE_FIELD_INT(m_iCurrentLoad, PDFF_NOEDIT, "current_load", "", EDITOR_NONE)


	//! угол (в градусах) базовой дисперсии оружия (оружия, зажатого в тисках)
	DEFINE_FIELD_FLOAT(m_fBaseSpread, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_base", "", EDITOR_NONE)
	//! коэффициент разброса в стоя
	DEFINE_FIELD_FLOAT(m_fSpreadIdle, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_idle", "", EDITOR_NONE)
	//! коэффициент разброса пригнувшись
	DEFINE_FIELD_FLOAT(m_fSpreadCrouch, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_crouch", "", EDITOR_NONE)
	//! коэффициент разброса лежа
	DEFINE_FIELD_FLOAT(m_fSpreadCrawl, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_crawl", "", EDITOR_NONE)
	//! коэффициент разброса в ходьбе
	DEFINE_FIELD_FLOAT(m_fSpreadWalk, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_walk", "", EDITOR_NONE)
	//! коэффициент разброса в беге
	DEFINE_FIELD_FLOAT(m_fSpreadRun, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_run", "", EDITOR_NONE)
	//! коэффициент разброса в полете (прыжок)
	DEFINE_FIELD_FLOAT(m_fSpreadAirborne, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_airborne", "", EDITOR_NONE)
	//! коэффициент разброса от состояния оружия
	DEFINE_FIELD_FLOAT(m_fSpreadCondition, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_condition", "", EDITOR_NONE)
	//! коэффициент разброса от состояния рук
	DEFINE_FIELD_FLOAT(m_fSpreadArm, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_arm", "", EDITOR_NONE)
	//! коэффициент разброса в прицеливании
	DEFINE_FIELD_FLOAT(m_fSpreadIronSight, PDFF_NOEDIT | PDFF_NOEXPORT, "spread_ironsight", "", EDITOR_NONE)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseWeapon, base_weapon);

SXbaseWeapon::SXbaseWeapon(EntityManager * pMgr):
	BaseClass(pMgr),
	m_pSilencer(NULL),
	m_pScope(NULL),
	m_pHandle(NULL),
	m_pMag(NULL),
	m_fireMode(FIRE_MODE_SINGLE),
	m_iFireModes(0),

	m_idSndDraw(-1),
	m_idSndHolster(-1),
	m_idSndShoot(-1),
	m_idSndEmpty(-1),
	m_idSndReload(-1),
	m_idSndSwitch(-1),

	m_iCapacity(1),
	m_iCurrentLoad(0),

	m_fBaseSpread(0.33f),
	m_fSpreadIdle(0.01f),
	m_fSpreadCrouch(0.007f),
	m_fSpreadCrawl(0.001f),
	m_fSpreadWalk(1.0f),
	m_fSpreadRun(4.0f),
	m_fSpreadAirborne(5.0f),
	m_fSpreadCondition(3.0f),
	m_fSpreadArm(3.0f),
	m_fSpreadIronSight(-0.8f)
{
	m_bIsWeapon = true;
}

void SXbaseWeapon::OnPostLoad()
{
	BaseClass::OnPostLoad();

	if(m_szSndDraw[0])
	{
		m_idSndDraw = SSCore_SndCreate3dInst(m_szSndDraw, false, 0, 100);
	}
	if(m_szSndHolster[0])
	{
		m_idSndHolster = SSCore_SndCreate3dInst(m_szSndHolster, false, 0, 100);
	}
	if(m_szSndShoot[0])
	{
		m_idSndShoot = SSCore_SndCreate3dInst(m_szSndShoot, false, 0, 100);
	}
	if(m_szSndEmpty[0])
	{
		m_idSndEmpty = SSCore_SndCreate3dInst(m_szSndEmpty, false, 0, 100);
	}
	if(m_szSndReload[0])
	{
		m_idSndReload = SSCore_SndCreate3dInst(m_szSndReload, false, 0, 100);
	}
	if(m_szSndSwitch[0])
	{
		m_idSndSwitch = SSCore_SndCreate3dInst(m_szSndSwitch, false, 0, 100);
	}
}

bool SXbaseWeapon::SetKV(const char * name, const char * value)
{
	if(!BaseClass::SetKV(name, value))
	{
		return(false);
	}

	if(!strcmp(name, "fire_modes"))
	{
		int len = sizeof(char) * (strlen(value) + 1);
		char * tmpstr = (char*)alloca(len);
		memcpy(tmpstr, value, len);
		char * str[FIRE_MODE_COUNT];
		int c = parse_str(tmpstr, str, FIRE_MODE_COUNT);
		m_iFireModes = 0;
		for(int i = 0; i < c; ++i)
		{
			if(!strcmp(str[i], "single"))
			{
				m_iFireModes |= FIRE_MODE_SINGLE;
			}
			else if(!strcmp(str[i], "burst"))
			{
				m_iFireModes |= FIRE_MODE_BURST;
			}
			else if(!strcmp(str[i], "cutoff"))
			{
				m_iFireModes |= FIRE_MODE_CUTOFF;
			}
			else
			{
				printf(COLOR_LRED "Unknown fire mode '%s'\n" COLOR_RESET, str[i]);
			}
		}
		if(!m_iFireModes)
		{
			printf(COLOR_LRED "No firemodes defined for '%s'\n" COLOR_RESET, GetClassName());
		}
	}
	return(true);
}

void SXbaseWeapon::PrimaryAction(BOOL st)
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
			//SXDecals_ShootDecal(DECAL_TYPE_CONCRETE, BTVEC_F3(cb.m_hitPointWorld), BTVEC_F3(cb.m_hitNormalWorld));
			SPE_EffectPlayByName("fire", &BTVEC_F3(cb.m_hitPointWorld), &BTVEC_F3(cb.m_hitNormalWorld));
			if(!cb.m_collisionObject->isStaticOrKinematicObject())
			{
				((btRigidBody*)cb.m_collisionObject)->applyCentralImpulse(F3_BTVEC(dir * 10.0f));
				cb.m_collisionObject->activate();
			}
		}
	}
}

void SXbaseWeapon::SecondaryAction(BOOL st)
{
	m_bInSecondaryAction = st != FALSE;
	if(m_iZoomable)
	{
		((SXplayer*)m_pOwner)->GetCrosshair()->Enable(!st);
	}
}

void SXbaseWeapon::Reload()
{
	if(!m_pMag)
	{
		printf(COLOR_MAGENTA "Cannot reload without mag!\n" COLOR_RESET);
		return;
	}
	if(!m_pOwner)
	{
		printf(COLOR_MAGENTA "Cannot reload without owner!\n" COLOR_RESET);
		return;
	}
	if(CanUse())
	{
		//int count = m_pOwner->getInventory()->consumeItems("ammo_5.45x39ps", m_pMag->getCapacity() - m_pMag->getLoad() + m_iCapacity - m_iCurrentLoad);
		//count += m_iCurrentLoad;
		//m_iCurrentLoad = min(count, m_iCapacity);
		//count -= m_iCurrentLoad;
		//m_pMag->load(count);

		SetNextUse(m_fReloadTime);
		PlayAnimation("reload");
		if(ID_VALID(m_idSndReload))
		{
			SSCore_SndInstancePlay3d(m_idSndReload, &GetPos());
		}
	}
}

void SXbaseWeapon::setFireMode(FIRE_MODE mode)
{
	if(!(m_iFireModes & mode))
	{
		m_fireMode = mode;
		if(ID_VALID(m_idSndReload))
		{
			SSCore_SndInstancePlay3d(m_idSndSwitch, &GetPos());
		}
	}
}

void SXbaseWeapon::nextFireMode()
{
	int cur = (int)log2f((float)m_fireMode);
	int newMode = cur;
	do
	{
		newMode = (newMode + 1) % FIRE_MODE_COUNT;
	}
	while(newMode != cur && !(m_iFireModes & (1 << newMode)));
	if(newMode != cur)
	{
		setFireMode((FIRE_MODE)newMode);
	}
}

bool SXbaseWeapon::canShoot()
{
	return(m_iCurrentLoad > 0 || (m_pMag && m_pMag->getLoad() > 0));
}

float SXbaseWeapon::getWeight()
{
	return(m_iInvWeight +
		(m_pHandle ? m_pHandle->getWeight() : 0.0f) +
		(m_pScope ? m_pScope->getWeight() : 0.0f) +
		(m_pMag ? m_pMag->getWeight() : 0.0f) +
		(m_pSilencer ? m_pSilencer->getWeight() : 0.0f)
	);
}

float SXbaseWeapon::getBaseSpread() const
{
	return(m_fBaseSpread);
}

bool SXbaseWeapon::isIronSight() const
{
	return(m_iZoomable && m_bInSecondaryAction);
}

float SXbaseWeapon::getSpreadCoeff(SPREAD_COEFF what) const
{
	switch(what)
	{
	case SPREAD_COEFF_IDLE:
		return(m_fSpreadIdle);
	case SPREAD_COEFF_CROUCH:
		return(m_fSpreadCrouch);
	case SPREAD_COEFF_CRAWL:
		return(m_fSpreadCrawl);
	case SPREAD_COEFF_WALK:
		return(m_fSpreadWalk);
	case SPREAD_COEFF_RUN:
		return(m_fSpreadRun);
	case SPREAD_COEFF_AIRBORNE:
		return(m_fSpreadAirborne);
	case SPREAD_COEFF_CONDITION:
		return(m_fSpreadCondition);
	case SPREAD_COEFF_ARM:
		return(m_fSpreadArm);
	case SPREAD_COEFF_IRONSIGHT:
		return(m_fSpreadIronSight);
	}
	return(1.0f);
}
