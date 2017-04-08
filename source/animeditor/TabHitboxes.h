#ifndef TabHitboxes_H
#define TabHitboxes_H

#include "EditorTab.h"

class TabHitboxes:public EditorTab
{
public:
	friend class Editor;

	TabHitboxes(TabManager * tm);
	~TabHitboxes();

protected:
	ISXGUIListBox* HBList;
	ISXGUIButton* BtnAdd;
	ISXGUIButton* BtnDel;
	ISXGUIGroupBox* PropsGB;
	ISXGUIStatic* Static1;
	ISXGUIStatic* Static2;
	ISXGUIStatic* Static3;
	ISXGUIEdit* EdName;
	ISXGUIComboBox* CBBone;
	ISXGUIComboBox* CBBodyPart;
	ISXGUIStatic* Static4;
	ISXGUIStatic* Static5;
	ISXGUIEdit* EdPosX;
	ISXGUIStatic* Static6;
	ISXGUIEdit* EdPosY;
	ISXGUIStatic* Static7;
	ISXGUIEdit* EdPosZ;
	ISXGUIStatic* Static8;
	ISXGUIStatic* Static9;
	ISXGUIStatic* Static10;
	ISXGUIStatic* Static11;
	ISXGUIEdit* EdRotX;
	ISXGUIEdit* EdRotY;
	ISXGUIEdit* EdRotZ;
	ISXGUIStatic* Static13;
	ISXGUIStatic* Static14;
	ISXGUIEdit* EdL;
	ISXGUIEdit* EdW;
	ISXGUIEdit* EdH;
	ISXGUIStatic* Static15;
	ISXGUIStatic* Static16;
	ISXGUIComboBox* CBType;

	ISXGUIRadioButton* RBTranslate;
	ISXGUIRadioButton* RBRotate;
	ISXGUIRadioButton* RBScale;
};

#endif
