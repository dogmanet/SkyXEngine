
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

enum INVENTORY_ITEM_MODE
{
	IIM_WORLD,     //! В мире
	IIM_INVENTORY, //! В инвентаре
	IIM_EQUIPPED   //! В руках
};

//! Базовый класс предмета инвентаря
class CBaseItem: public CBaseAnimating
{
	DECLARE_CLASS(CBaseItem, CBaseAnimating);
	DECLARE_PROPTABLE();
	friend class CCharacterInventory;
public:
	DECLARE_CONSTRUCTOR();
	~CBaseItem();

	//! Масса объекта
	virtual float getWeight();
	void onUse(CBaseEntity *pUser);

	void setMode(INVENTORY_ITEM_MODE mode);

	void setHandsResource(IXResourceModelAnimated *pResource);

protected:
	virtual void onModeChanged(INVENTORY_ITEM_MODE oldMode, INVENTORY_ITEM_MODE newMode);
	void onSetViewModel(const char *mdl);
	void onModelChanged();
	void onSync() override;

	void setScale(float fScale) override;

	const char * m_szInvName; //!< Имя, отображаемое в инвентаре
	bool m_bInvStackable; //!< Можно ли хранить несколько итемов в одной ячейке
	int m_iInvStackCurSize; //!< Количество итемов в стеке
	int m_iInvStackMaxSize; //!< Максимальное количество итемов в стеке
	float m_iInvWeight; //!< Масса объекта
	bool m_bPickable; //!< Можно ли поднять объект

	output_t m_onPickUp;
	output_t m_onDrop;

	INVENTORY_ITEM_MODE m_inventoryMode = IIM_WORLD;
	IXAnimatedModel *m_pViewModel = NULL;
	IXResourceModelAnimated *m_pViewModelResource = NULL;
	const char * m_szViewModelFile = NULL;
	IXResourceModelAnimated *m_pHandsModelResource = NULL;
};

#endif

//! @}
