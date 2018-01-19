
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TAB_ATTACHMENTS_H
#define __TAB_ATTACHMENTS_H

#include "EditorTab.h"

class TabAttachments:public EditorTab
{
public:
	friend class Editor;

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
	ISXGUIButton* AttachFileApply;

	UINT m_iflags;
};

#endif
