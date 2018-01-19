
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Предметы инвентаря
*/

/*! \defgroup cbaseitem Предметы инвентаря
\ingroup cbaseanimating
@{
*/

#ifndef __BASE_ITEM_H
#define __BASE_ITEM_H

#include "BaseAnimating.h"

//! Базовый класс предмета инвентаря
class CBaseItem: public CBaseAnimating
{
	DECLARE_CLASS(CBaseItem, CBaseAnimating);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	const char * m_szInvName; //!< Имя, отображаемое в инвентаре
	bool m_bInvStackable; //!< Можно ли хранить несколько итемов в одной ячейке
	int m_iInvStackCurSize; //!< Количество итемов в стеке
	int m_iInvStackMaxSize; //!< Максимальное количество итемов в стеке
	float m_iInvWeight; //!< Масса объекта
	bool m_bPickable; //!< Можно ли поднять объект


	//! Масса объекта
	virtual float getWeight();
};

#endif

//! @}
