
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Триггеры
*/

/*! \defgroup cbasetrigger Триггеры
\ingroup cbaseanimating
@{
*/

#ifndef _BaseTrigger_H_
#define _BaseTrigger_H_

#include "SXbaseAnimating.h"

//! Базовый класс триггера
class CBaseTrigger: public SXbaseAnimating
{
	DECLARE_CLASS(CBaseTrigger, SXbaseAnimating);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CBaseTrigger();

	void OnSync();
	void OnPostLoad();

	void enable();
	void disable();
	void toggle();

protected:
	bool m_bEnabled;
	ID m_idUpdateInterval;

	ID m_idDevMaterial;
	
	Array<SXbaseEntity*> m_aTouches;
	Array<SXbaseEntity*> m_aNewTouches;
	
	output_t m_onTouchStart;
	output_t m_onTouchEnd;
	output_t m_onTouchEndAll;

	void update(float dt);

	btPairCachingGhostObject *m_pGhostObject;

	void inEnable(inputdata_t * pInputdata);
	void inDisable(inputdata_t * pInputdata);
	void inToggle(inputdata_t * pInputdata);

	virtual void CreatePhysBody();
	virtual void RemovePhysBody();

	virtual void onTouchStart(SXbaseEntity *pActivator);
	virtual void onTouchEnd(SXbaseEntity *pActivator);
	virtual void onTouchEndAll(SXbaseEntity *pActivator);
};

#endif

/*! @} cbasetrigger */
