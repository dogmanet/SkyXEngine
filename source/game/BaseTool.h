
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый класс инструмента
*/

#ifndef __BASE_TOOL_H
#define __BASE_TOOL_H

#include "BaseItem.h"
#include "BaseSupply.h"

enum
{
	DSM_NONE,
	DSM_POS_X,
	DSM_POS_Y,
	DSM_POS_Z,
	DSM_ROT_X,
	DSM_ROT_Y,
	DSM_ROT_Z,
	DSM_LEN,
	DSM_PRINT
};

/*! Инструмент
\ingroup cbaseitem
*/
class CBaseTool: public CBaseItem
{
	DECLARE_CLASS(CBaseTool, CBaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CBaseTool();

	virtual void onPostLoad();

	virtual void primaryAction(BOOL st);
	virtual void secondaryAction(BOOL st);
	virtual void reload();

	//virtual void Deploy() = 0;

	void setNextUse(float time);
	bool canUse();

	void setIsWorldModel(bool b);

	void dbgMove(int dir, float delta);

	void onSync();

	void setParent(CBaseEntity * pEnt, int attachment = -1);

	//! Этот инструмент - оружие
	bool isWeapon() const;

	//! Состояние: 1 - целое; 0 - сломанное
	float getCondition() const;

	//! Объект заряженного припаса
	CBaseSupply *getAmmo() const;
	//! Зарядить
	void chargeAmmo(CBaseSupply *pAmmo);
	//! Разрядить
	void uncharge();

	float getMaxDistance() const;

	virtual void stopAction();

	virtual void updateHUDinfo();

protected:

	bool isValidAmmo(CBaseSupply *pAmmo);

	bool m_bInPrimaryAction;
	bool m_bInSecondaryAction;

	bool m_bWorldModel;

	bool m_bCanUse;

	float m_fZoomTime;

	float m_fZoomProgress = 0.0f;
	float m_fCloseProgress = 0.0f;

	float m_fCenterLength = 0.0f;
	bool m_isClose = false;

	void _allowUse(float dt)
	{
		m_bCanUse = true;
	}

	void _update(float dt);
	void _rezoom();

	float m_fReloadTime;

	float3_t m_vSlotPos;
	float3_t m_vSlotPosAim;
	float3_t m_vSlotPosClose;
	float3_t m_vSlotPosResult;
	SMQuaternion m_qSlotRot;
	SMQuaternion m_qSlotRotAim;
	SMQuaternion m_qSlotRotClose;
	SMQuaternion m_qSlotRotResult;

	int m_iZoomable;

	ID m_iIvalUpdate;

	ID m_iMuzzleFlash;
	ID m_iMuzzleFlash2;
	IXSoundEmitter *m_pSoundAction1 = NULL;
	IXSoundEmitter *m_pSoundAction2 = NULL;

	const char * m_szPrimaryActionSound;
	const char * m_szSecondaryActionSound;

	const char * m_szPrimaryActionMuzzleflash;
	const char * m_szSecondaryActionMuzzleflash;

	const char * m_szUsableAmmos;
	const char * m_szLoadedAmmo;
	CBaseSupply * m_pLoadedAmmo;

	float m_fMaxDistance;

	//! Этот инструмент - оружие
	bool m_bIsWeapon;
};

#endif
