
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Триггеры
*/

/*! \defgroup cbasetrigger Триггеры
\ingroup cbaseanimating
@{
*/

#ifndef __BASE_TRIGGER_H
#define __BASE_TRIGGER_H

#include "BaseAnimating.h"

//! Базовый класс триггера
class CBaseTrigger: public CBaseAnimating
{
	DECLARE_CLASS(CBaseTrigger, CBaseAnimating);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CBaseTrigger();

	void onSync();
	void onPostLoad();

	void enable();
	void disable();
	void toggle();

	void setPos(const float3 & pos) override;
	void setOrient(const SMQuaternion & q) override;

protected:
	bool m_bEnabled;
	ID m_idUpdateInterval;

	ID m_idDevMaterial;
	
	Array<CBaseEntity*> m_aTouches;
	Array<CBaseEntity*> m_aNewTouches;
	
	output_t m_onTouchStart;
	output_t m_onTouchEnd;
	output_t m_onTouchEndAll;

	void update(float dt);

	btPairCachingGhostObject *m_pGhostObject;

	void inEnable(inputdata_t * pInputdata);
	void inDisable(inputdata_t * pInputdata);
	void inToggle(inputdata_t * pInputdata);

	virtual void createPhysBody();
	virtual void removePhysBody();

	virtual void onTouchStart(CBaseEntity *pActivator);
	virtual void onTouchEnd(CBaseEntity *pActivator);
	virtual void onTouchEndAll(CBaseEntity *pActivator);
};

#endif

/*! @} cbasetrigger */
