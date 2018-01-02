/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Предметы инвентаря
*/

/*! \defgroup cbaseitem Предметы инвентаря
\ingroup cbaseanimating
@{
*/

#ifndef _SXbaseItem_H_
#define _SXbaseItem_H_

#include "SXbaseAnimating.h"

//! Базовый класс предмета инвентаря
class SXbaseItem: public SXbaseAnimating
{
	DECLARE_CLASS(SXbaseItem, SXbaseAnimating);
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
