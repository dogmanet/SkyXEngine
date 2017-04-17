#ifndef TabSkins_H
#define TabSkins_H

#include "EditorTab.h"

class TabSkins:public EditorTab
{
public:
	TabSkins(TabManager * tm);
	~TabSkins();

protected:
	ISXGUIListBox* SkinsList;
	ISXGUIButton* SkinNewBtn;
	ISXGUIButton* SkinRenameBtn;
	ISXGUIButton* SkinRemoveBtn;
	ISXGUIButton* SkinDuplicateBtn;
	ISXGUIGroupBox* SkinMatsGB;
	ISXGUIListBox* SkinMatsList;
	ISXGUIGroupBox* SkinMatPropsGB;
	ISXGUIEdit* SkinMatFile;
	ISXGUIStatic* Static6;
	ISXGUIButton* SkinMatBrowse;
	ISXGUIButton* SkinMatReset;
};

#endif
