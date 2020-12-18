
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый класс оружия
*/

#ifndef __BASE_WEAPON_H
#define __BASE_WEAPON_H

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

//! Тип нарезки ствола
enum RIFLE_TYPE
{
	RIFLE_TYPE_LEFT = -1, //!< Левая
	RIFLE_TYPE_UNRIFLED = 0, //!< Гладкоствольное
	RIFLE_TYPE_RIGHT = 1, //!< Правая

	RIFLE_TYPE_FORCE_DWORD = 0x7fffffff  /* force 32-bit size enum */
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
	~CBaseWeapon();
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

	//! Тип нарезки ствола
	RIFLE_TYPE getRifleType() const {return(m_rifleType);}

	//! Подсоединить магазин
	void attachMag(CBaseMag * pMag);

	void updateHUDinfo();
protected:

	//! Распределение гаусса вектора vDir в телесном угле fSpread
	float3 applySpread(const float3 &vDir, float fSpread);

	//! Задача стрельбы
	virtual void taskShoot(float dt);

	ID m_idTaskShoot = -1;

	// Compatible addons
	const char *m_szAddonScopes = NULL;
	const char *m_szAddonSilencers = NULL;
	const char *m_szAddonMags = NULL;
	const char *m_szAddonHandlers = NULL;

	// Addons
	CBaseSilencer *m_pSilencer = NULL;
	CBaseScope *m_pScope = NULL;
	CBaseHandle *m_pHandle = NULL;
	CBaseMag *m_pMag = NULL;

	// Fire modes
	FIRE_MODE m_fireMode = FIRE_MODE_SINGLE;
	int m_iFireModes = 0;
	const char *m_szFireModes = NULL;
	int m_iSingleRate = 1;
	int m_iBurstRate = 1;
	int m_iCutoffRate = 1;
	int m_iCutoffSize = 1;

	int m_iCutoffCurrent = 0;

	// Sounds
	const char *m_szSndDraw = NULL;
	const char *m_szSndHolster = NULL;
	const char *m_szSndShoot = NULL;
	const char *m_szSndEmpty = NULL;
	const char *m_szSndReload = NULL;
	const char *m_szSndSwitch = NULL;

	IXSoundPlayer *m_pSndDraw = NULL;
	IXSoundPlayer *m_pSndHolster = NULL;
	IXSoundEmitter *m_pSndShoot = NULL;
	IXSoundPlayer *m_pSndEmpty = NULL;
	IXSoundPlayer *m_pSndReload = NULL;
	IXSoundPlayer *m_pSndSwitch = NULL;

	// Shooting
	float m_fEffectiveDistance = 1000.0f;
	RIFLE_TYPE m_rifleType = RIFLE_TYPE_UNRIFLED;
	float m_fRifleStep = 0.0f;
	float m_fAimingRange = 100.0f;

	// Without mag
	int m_iCapacity = 1;
	int m_iCurrentLoad = 0;

	// Spread
	float m_fBaseSpread = 0.33f;
	float m_fSpreadIdle = 0.01f;
	float m_fSpreadCrouch = 0.007f;
	float m_fSpreadCrawl = 0.001f;
	float m_fSpreadWalk = 1.0f;
	float m_fSpreadRun = 4.0f;
	float m_fSpreadAirborne = 5.0f;
	float m_fSpreadCondition = 3.0f;
	float m_fSpreadArm = 3.0f;
	float m_fSpreadIronSight = -0.8f;
};

#endif
