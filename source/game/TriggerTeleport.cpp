
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <gcore/sxgcore.h>

#include "TriggerTeleport.h"

/*! \skydocent trigger_teleport
Триггер телепортации
*/

BEGIN_PROPTABLE(CTriggerTeleport)
	//! Локальный маркер для относительных координат
	DEFINE_FIELD_ENTITY(m_pLandmark, PDFF_NONE, "landmark", "Landmark object", EDITOR_TEXTFIELD)
	//! Цель
	DEFINE_FIELD_ENTITY(m_pDestination, PDFF_NONE, "destination", "Destination landmark object", EDITOR_TEXTFIELD)
END_PROPTABLE()

REGISTER_ENTITY(CTriggerTeleport, trigger_teleport);

CTriggerTeleport::CTriggerTeleport(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	//m_idHurtInterval = SET_INTERVAL(think, 1000.0f);
}

CTriggerTeleport::~CTriggerTeleport()
{
	//CLEAR_INTERVAL(m_idHurtInterval);
}

void CTriggerTeleport::onTouchStart(CBaseEntity *pActivator)
{
	if(!m_pDestination)
	{
		return;
	}
	CBaseEntity *pEnt = pActivator;
	if(pEnt->getParent())
	{
		pEnt = pEnt->getParent();
	}
	if(m_pLandmark)
	{
		pEnt->setPos(pEnt->getPos() - m_pLandmark->getPos() + m_pDestination->getPos());
		pEnt->setOrient(pEnt->getOrient() * m_pLandmark->getOrient().Conjugate() * m_pDestination->getOrient());
	}
	else
	{
		pEnt->setPos(m_pDestination->getPos());
		pEnt->setOrient(m_pDestination->getOrient());
	}
}
