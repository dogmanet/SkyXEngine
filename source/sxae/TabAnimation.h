
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TAB_ANIMATION_H
#define __TAB_ANIMATION_H

#include "EditorTab.h"

class TabAnimation:public EditorTab
{
public:
	friend class Editor;
	TabAnimation(TabManager * tm);
	~TabAnimation();

protected:
	ISXGUIGroupBox* AnimCtlGB;
	ISXGUIStatic* Static7;
	ISXGUIEdit* AnimCtlProgress;
	ISXGUIButton* AnimCtlPlayBtn;
	ISXGUIButton* AnimCtlPauseBtn;
	ISXGUITrackBar* AnimCtlProgressTrack;
	ISXGUIGroupBox* AnimPropsGB;
	ISXGUICheckBox* AnimPropLoopCB;
	ISXGUIStatic* Static8;
	ISXGUIEdit* AnimPropSpeed;
	ISXGUIUpDown* AnimPropSpeedSpin;
	ISXGUIStatic* Static9;
	ISXGUIEdit* AnimPropName;
	ISXGUIStatic* Static10;
	ISXGUIComboBox* AnimPropActCmb;
	ISXGUIStatic* Static11;
	ISXGUIEdit* AnimPropActChance;
};

#endif
