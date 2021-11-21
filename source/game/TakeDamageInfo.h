#ifndef __TAKEDAMAGEINFO_H
#define __TAKEDAMAGEINFO_H

#include <xcommon/resource/IXResourceModel.h>

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
		m_pAttacker(pAttacker),
		m_fDamage(fDamage),
		m_damageType(damageType)
	{
	}

	CBaseEntity *m_pInflictor = NULL;
	CBaseEntity *m_pAttacker;
	XHITBOXBODYPART m_bodyPart = XHBP_DEFAULT;
	float m_fDamage;
	DAMAGE_TYPE m_damageType;
};

#endif
