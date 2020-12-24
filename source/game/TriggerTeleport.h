
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Триггеры
*/

/*! \defgroup ctriggerteleport Триггер телепортации
\ingroup cbaseanimating
@{
*/

#ifndef __TRIGGER_TELEPORT_H
#define __TRIGGER_TELEPORT_H

#include "BaseTrigger.h"

//! Базовый класс триггера
class CTriggerTeleport: public CBaseTrigger
{
	DECLARE_CLASS(CTriggerTeleport, CBaseTrigger);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

protected:
	virtual void onTouchStart(CBaseEntity *pActivator) override;

private:
	CEntityPointer<CBaseEntity> m_pLandmark;
	CEntityPointer<CBaseEntity> m_pDestination;
};

#endif

/*! @} cbasetrigger */
