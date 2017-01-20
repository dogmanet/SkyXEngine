#ifndef TabAttachments_H
#define TabAttachments_H

#include "EditorTab.h"

class TabAttachments:public EditorTab
{
public:
	TabAttachments(TabManager * tm);
	~TabAttachments();

protected:
	ISXGUIListBox* AttachmentsList;
	ISXGUIButton* AttachNew;
	ISXGUIButton* AttachRename;
	ISXGUIButton* AttachRemove;
	ISXGUIButton* AnimationBtn;
	ISXGUIGroupBox* AttachPropsGB;
	ISXGUIRadioButton* AttachBoneRB;
	ISXGUIRadioButton* AttachSkinRB;
	ISXGUICheckBox* AttachHideCB;
	ISXGUICheckBox* AttachEnaCollisionCB;
	ISXGUICheckBox* AttachEnaRaytraceCB;
	ISXGUIComboBox* AttachBone;
	ISXGUIEdit* AttachXshift;
	ISXGUIUpDown* AttachXshiftSpin;
	ISXGUIEdit* AttachYshift;
	ISXGUIUpDown* AttachYshiftSpin;
	ISXGUIEdit* AttachZshift;
	ISXGUIUpDown* AttachZshiftSpin;
	ISXGUIStatic* Static5;
	ISXGUIEdit* AttachFileField;
	ISXGUIButton* AttachFileBrowse;
};

#endif
