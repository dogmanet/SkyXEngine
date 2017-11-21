/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Базовый класс оружия
*/

#ifndef _SXbaseWeapon_H_
#define _SXbaseWeapon_H_

#include "SXbaseTool.h"

#include "BaseScope.h"
#include "BaseHandle.h"
#include "BaseMag.h"
#include "BaseSilencer.h"

enum FIRE_MODE
{
	FIRE_MODE_SINGLE = 0x01,
	FIRE_MODE_BURST = 0x02,
	FIRE_MODE_CUTOFF = 0x04,

	FIRE_MODE_FORCE_DWORD = 0x7fffffff  /* force 32-bit size enum */
};
#define FIRE_MODE_COUNT 3

/*! Оружие
\ingroup cbaseitem
*/
class SXbaseWeapon: public SXbaseTool
{
	DECLARE_CLASS(SXbaseWeapon, SXbaseTool);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	virtual void OnPostLoad();

	virtual void PrimaryAction(BOOL st);
	virtual void SecondaryAction(BOOL st);
	virtual void Reload();

	virtual bool SetKV(const char * name, const char * value);

	void setFireMode(FIRE_MODE mode);
	void nextFireMode();

	bool canShoot();

protected:

	// Compatible addons
	const char * m_szAddonScopes;
	const char * m_szAddonSilencers;
	const char * m_szAddonMags;
	const char * m_szAddonHandlers;

	// Addons
	CBaseSilencer * m_pSilencer;
	CBaseScope * m_pScope;
	CBaseHandle * m_pHandle;
	CBaseMag * m_pMag;

	// Fire modes
	FIRE_MODE m_fireMode;
	int m_iFireModes;
	const char * m_szFireModes;
	int m_iSingleSpeed;
	int m_iBurstSpeed;
	int m_iCutoffSpeed;
	int m_iCutoffSize;

	// Sounds
	const char * m_szSndDraw;
	const char * m_szSndHolster;
	const char * m_szSndShoot;
	const char * m_szSndEmpty;
	const char * m_szSndReload;
	const char * m_szSndSwitch;

	ID m_idSndDraw;
	ID m_idSndHolster;
	ID m_idSndShoot;
	ID m_idSndEmpty;
	ID m_idSndReload;
	ID m_idSndSwitch;

	// Shooting
	float m_fEffectiveDistance;

	// Without mag
	int m_iCapacity;
	int m_iCurrentLoad;
};

#endif
