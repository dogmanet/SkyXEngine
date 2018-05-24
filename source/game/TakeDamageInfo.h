#ifndef __TAKEDAMAGEINFO_H
#define __TAKEDAMAGEINFO_H

#include <anim/ModelFile.h>

class CBaseEntity;

enum DAMAGE_TYPE
{
	DAMAGE_GENERIC = 0,
	DAMAGE_FIRE,
	// ... add more
};

class CTakeDamageInfo
{
public:
	CTakeDamageInfo(CBaseEntity *pAttacker, float fDamage, DAMAGE_TYPE damageType = DAMAGE_GENERIC):
		m_bodyPart(HBP_DEFAULT),
		m_pInflictor(NULL),
		m_pAttacker(pAttacker),
		m_fDamage(fDamage),
		m_damageType(damageType)
	{
	}

	CBaseEntity *m_pInflictor;
	CBaseEntity *m_pAttacker;
	HITBOX_BODYPART m_bodyPart;
	float m_fDamage;
	DAMAGE_TYPE m_damageType;
};

#endif
