#ifndef __FUNC_ROTATING_H
#define __FUNC_ROTATING_H

#include "PointEntity.h"

#define FUNC_ROTATING_START_ENABLED ENT_FLAG_0
#define FUNC_ROTATING_REVERSE_DIRECTION ENT_FLAG_1

class CFuncRotating: public CPointEntity
{
	DECLARE_CLASS(CFuncRotating, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	~CFuncRotating();
	
protected:
	void turnOn(inputdata_t *pInputdata);
	void turnOff(inputdata_t *pInputdata);
	void toggle(inputdata_t *pInputdata);

	void think(float fDt);

	// output_t m_onTrigger;

	float m_fSpeed = 1.0f;

	void updateFlags() override;

private:
	ID m_idThinkInterval = -1;
	bool m_isReversed = false;
};

#endif
