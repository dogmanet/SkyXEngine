
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Триггеры
*/

/*! \defgroup ctriggeritemuse Триггер подбора предметов
\ingroup cbaseanimating
@{
*/

#ifndef __TRIGGER_ITEMUSE_H
#define __TRIGGER_ITEMUSE_H

#include "BaseTrigger.h"
#include "BaseItem.h"

//! Базовый класс триггера
class CTriggerItemUse: public CBaseTrigger
{
	DECLARE_CLASS(CTriggerItemUse, CBaseTrigger);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	void setItem(CBaseItem *pItem);

protected:
	void onTouchStart(CBaseEntity *pActivator) override;


private:
	CBaseItem *m_pItem = NULL;
};

#endif

/*! @} cbasetrigger */
