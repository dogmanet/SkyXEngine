#ifndef _SXbaseAmmo_H_
#define _SXbaseAmmo_H_

#include "SXbaseSupply.h"

class SXbaseAmmo: public SXbaseSupply
{
	DECLARE_CLASS(SXbaseAmmo, SXbaseSupply);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	void fire(const float3 &vStart, const float3 &vEnd);

protected:

	float m_fStartSpeed;
	float m_fBulletMass;
	float m_fArmorPiercing;
};

#endif
