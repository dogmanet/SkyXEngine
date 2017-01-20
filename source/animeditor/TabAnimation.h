#ifndef TabAnimation_H
#define TabAnimation_H

#include "EditorTab.h"

class TabAnimation:public EditorTab
{
public:
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
