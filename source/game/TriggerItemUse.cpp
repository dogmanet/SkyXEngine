
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <gcore/sxgcore.h>

#include "TriggerItemUse.h"

/*! \skydocent trigger_itemuse
Триггер телепортации
*/

BEGIN_PROPTABLE(CTriggerItemUse)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CTriggerItemUse, trigger_itemuse);

CTriggerItemUse::CTriggerItemUse(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	//m_idHurtInterval = SET_INTERVAL(think, 1000.0f);
}

void CTriggerItemUse::setItem(CBaseItem *pItem)
{
	m_pItem = pItem;
}

void CTriggerItemUse::onTouchStart(CBaseEntity *pActivator)
{
	if(!m_pItem || fstrcmp(pActivator->getClassName(), "player")/* || !pActivator->isCharacter()*/)
	{
		return;
	}
	disable();

	m_pItem->onUse(pActivator);
}
