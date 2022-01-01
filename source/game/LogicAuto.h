#ifndef __LOGIC_AUTO_H
#define __LOGIC_AUTO_H

#include "PointEntity.h"

class CLogicAuto: public CPointEntity
{
	DECLARE_CLASS(CLogicAuto, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	
	void activate();

protected:
	void doTrigger(float fDT);

	output_t m_onTrigger;

	float m_fDelay = 0.0f;
};

#endif
