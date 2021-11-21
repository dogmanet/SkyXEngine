
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

class CBaseTrigger;
class CPhysicsTickEventListener final: public IEventListener<XEventPhysicsStep>
{
public:
	CPhysicsTickEventListener(CBaseTrigger *pTrigger):
		m_pTrigger(pTrigger)
	{
	}
	void onEvent(const XEventPhysicsStep *pData) override;

private:
	CBaseTrigger *m_pTrigger;
};

//! Базовый класс триггера
class CBaseTrigger: public CBaseAnimating
{
	DECLARE_CLASS(CBaseTrigger, CBaseAnimating);
	DECLARE_PROPTABLE();

	friend class CPhysicsTickEventListener;
public:
	DECLARE_CONSTRUCTOR();
	~CBaseTrigger();

	void onPostLoad() override;

	void enable();
	void disable();
	void toggle();

	void setPos(const float3 & pos) override;
	void setOrient(const SMQuaternion & q) override;

protected:
	bool m_bEnabled = true;

	ID m_idDevMaterial = -1;
	
	Array<CBaseEntity*> m_aTouches;
	Array<CBaseEntity*> m_aNewTouches;
	
	output_t m_onTouchStart;
	output_t m_onTouchEnd;
	output_t m_onTouchEndAll;

	void update();

	//btPairCachingGhostObject *m_pGhostObject = NULL;
	IXGhostObject *m_pGhostObject = NULL;

	bool m_isModelEnabled = true;

	void inEnable(inputdata_t * pInputdata);
	void inDisable(inputdata_t * pInputdata);
	void inToggle(inputdata_t * pInputdata);

	virtual void createPhysBody();
	virtual void removePhysBody();

	virtual void onTouchStart(CBaseEntity *pActivator);
	virtual void onTouchEnd(CBaseEntity *pActivator);
	virtual void onTouchEndAll(CBaseEntity *pActivator);

private:
	static IEventChannel<XEventPhysicsStep> *m_pTickEventChannel;

	CPhysicsTickEventListener m_physicsTicker;

	void onPhysicsStep();
};

#endif

/*! @} cbasetrigger */
