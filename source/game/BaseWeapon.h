/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Базовый класс оружия
*/

#ifndef _CBaseWeapon_H_
#define _CBaseWeapon_H_

#include "BaseTool.h"

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

//! Идентификаторы для получения коэффициентов разброса
enum SPREAD_COEFF
{
	SPREAD_COEFF_IDLE, //!< стоя
	SPREAD_COEFF_CROUCH, //!< пригувшись
	SPREAD_COEFF_CRAWL, //!< лежа
	SPREAD_COEFF_WALK, //!< в ходьбе
	SPREAD_COEFF_RUN, //!<  в беге
	SPREAD_COEFF_AIRBORNE, //!< в прыжке
	SPREAD_COEFF_CONDITION, //!< состояние оружия
	SPREAD_COEFF_ARM, //!< состояние рук
	SPREAD_COEFF_IRONSIGHT, //!< в прицеливании
};

/*! Оружие
\ingroup cbaseitem
*/
class CBaseWeapon: public CBaseTool
{
	DECLARE_CLASS(CBaseWeapon, CBaseTool);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	virtual void onPostLoad();

	virtual void primaryAction(BOOL st);
	virtual void secondaryAction(BOOL st);
	virtual void reload();

	virtual bool setKV(const char * name, const char * value);

	void setFireMode(FIRE_MODE mode);
	void nextFireMode();

	bool canShoot();

	//! Масса объекта
	float getWeight();


	//! угол (в градусах) базовой дисперсии оружия (оружия, зажатого в тисках)
	float getBaseSpread() const;

	//! в прицеливании
	bool isIronSight() const;

	//! Коэффициент разброса
	float getSpreadCoeff(SPREAD_COEFF what) const;

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

	// Spread
	float m_fBaseSpread;
	float m_fSpreadIdle;
	float m_fSpreadCrouch;
	float m_fSpreadCrawl;
	float m_fSpreadWalk;
	float m_fSpreadRun;
	float m_fSpreadAirborne;
	float m_fSpreadCondition;
	float m_fSpreadArm;
	float m_fSpreadIronSight;
};

#endif
