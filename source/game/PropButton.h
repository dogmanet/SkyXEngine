
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Кнопка
*/

#ifndef __PROP_BUTTON_H
#define __PROP_BUTTON_H

#include "BaseAnimating.h"

#define BUTTON_START_DISABLED ENT_FLAG_0
#define BUTTON_TOGGLEABLE ENT_FLAG_1
#define BUTTON_START_PRESSED ENT_FLAG_2

/*! Кнопка
\ingroup cbaseanimating
*/
class CPropButton: public CBaseAnimating
{
	DECLARE_CLASS(CPropButton, CBaseAnimating);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	void onUse(CBaseEntity *pUser);

protected:
	void turnOn(inputdata_t * pInputdata);
	void turnOff(inputdata_t * pInputdata);
	void press(inputdata_t * pInputdata);
	void pushUp(inputdata_t * pInputdata);
	void pushDown(inputdata_t * pInputdata);
	void toggle(inputdata_t * pInputdata);

	void onPostLoad();

	output_t m_onPressed;
	output_t m_onPushUp;
	output_t m_onPushDown;
	output_t m_onToggle;
	output_t m_onUseDisabled;

	bool m_isEnabled;
	bool m_isToggleable;

	bool m_bState;
};

#endif
