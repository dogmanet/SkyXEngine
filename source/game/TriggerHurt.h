
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Триггеры
*/

/*! \defgroup ctriggerhurt Триггеры урона
\ingroup cbaseanimating
@{
*/

#ifndef __TRIGGER_HURT_H
#define __TRIGGER_HURT_H

#include "BaseTrigger.h"

//! Базовый класс триггера
class CTriggerHurt: public CBaseTrigger
{
	DECLARE_CLASS(CTriggerHurt, CBaseTrigger);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	~CTriggerHurt();

protected:
	virtual void onTouchStart(CBaseEntity *pActivator) override;
	virtual void onTouchEndAll(CBaseEntity *pActivator) override;

private:
	ID m_idHurtInterval = -1;
	float m_fHurtInterval = 0.5f;
	float m_fDamage = 10.0f;

	void think(float dt);
};

#endif

/*! @} cbasetrigger */
