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
	ISXGUICheckBox* CtlsPropTrYCB;
	ISXGUICheckBox* CtlsPropTrZCB;
	ISXGUIEdit* CtlsPropTrX;
	ISXGUIEdit* CtlsPropTrY;
	ISXGUIEdit* CtlsPropTrZ;
	ISXGUIUpDown* CtlsPropTrXSpin;
	ISXGUIUpDown* CtlsPropTrYSpin;
	ISXGUIUpDown* CtlsPropTrZSpin;

	ISXGUIGroupBox* CtlsPropRotationGB;
	ISXGUICheckBox* CtlsPropRotXCB;
	ISXGUICheckBox* CtlsPropRotYCB;
	ISXGUICheckBox* CtlsPropRotZCB;
	ISXGUIEdit* CtlsPropRotX;
	ISXGUIEdit* CtlsPropRotY;
	ISXGUIEdit* CtlsPropRotZ;
	ISXGUIUpDown* CtlsPropRotXSpin;
	ISXGUIUpDown* CtlsPropRotYSpin;
	ISXGUIUpDown* CtlsPropRotZSpin;

	ISXGUIGroupBox* CtlsPropTranslationGB2;
	ISXGUICheckBox* CtlsPropTrXCB2;
	ISXGUICheckBox* CtlsPropTrYCB2;
	ISXGUICheckBox* CtlsPropTrZCB2;
	ISXGUIEdit* CtlsPropTrX2;
	ISXGUIEdit* CtlsPropTrY2;
	ISXGUIEdit* CtlsPropTrZ2;
	ISXGUIUpDown* CtlsPropTrXSpin2;
	ISXGUIUpDown* CtlsPropTrYSpin2;
	ISXGUIUpDown* CtlsPropTrZSpin2;

	ISXGUIGroupBox* CtlsPropRotationGB2;
	ISXGUICheckBox* CtlsPropRotXCB2;
	ISXGUICheckBox* CtlsPropRotYCB2;
	ISXGUICheckBox* CtlsPropRotZCB2;
	ISXGUIEdit* CtlsPropRotX2;
	ISXGUIEdit* CtlsPropRotY2;
	ISXGUIEdit* CtlsPropRotZ2;
	ISXGUIUpDown* CtlsPropRotXSpin2;
	ISXGUIUpDown* CtlsPropRotYSpin2;
	ISXGUIUpDown* CtlsPropRotZSpin2;

	ISXGUIStatic * m_pstBones;
};

#endif
