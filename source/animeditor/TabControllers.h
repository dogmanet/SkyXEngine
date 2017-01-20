#ifndef TabControllers_H
#define TabControllers_H

#include "EditorTab.h"

class TabControllers:public EditorTab
{
public:
	TabControllers(TabManager * tm);
	~TabControllers();

protected:
	ISXGUIListBox* CtlsList;
	ISXGUIButton* CtlsNewBtn;
	ISXGUIButton* CtlsRenameBtn;
	ISXGUIButton* CtlsRemove;
	ISXGUIGroupBox* CtlsBonesGB;
	ISXGUIListBox* CtlsBoneList;
	ISXGUIButton* CtlsBoneAddBtn;
	ISXGUIButton* CtlsBoneRemoveBtn;
	ISXGUIGroupBox* CtlsPropsGB;
	ISXGUITrackBar* CtlsPropCurTrack;
	ISXGUIEdit* CtlsPropCur;
	ISXGUIGroupBox* CtlsPropTranslationGB;
	ISXGUICheckBox* CtlsPropTrXCB;
	ISXGUIEdit* CtlsPropTrX;
	ISXGUICheckBox* CtlsPropTrYCB;
	ISXGUICheckBox* CtlsPropTrZCB;
	ISXGUIUpDown* CtlsPropTrXSpin;
	ISXGUIEdit* CtlsPropTrY;
	ISXGUIEdit* CtlsPropTrZ;
	ISXGUIUpDown* CtlsPropTrYSpin;
	ISXGUIUpDown* CtlsPropTrZSpin;
	ISXGUICheckBox* CtlsPropRotXCB;
	ISXGUIGroupBox* CtlsPropRotationGB;
	ISXGUICheckBox* CtlsPropRotYCB;
	ISXGUICheckBox* CtlsPropRotZCB;
	ISXGUIEdit* CtlsPropRotX;
	ISXGUIEdit* CtlsPropRotY;
	ISXGUIEdit* CtlsPropRotZ;
	ISXGUIUpDown* CtlsPropRotXSpin;
	ISXGUIUpDown* CtlsPropRotYSpin;
	ISXGUIUpDown* CtlsPropRotZSpin;
};

#endif
