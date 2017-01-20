#ifndef TabFiles_H
#define TabFiles_H

#include "EditorTab.h"

class TabFiles:public EditorTab
{
public:
	TabFiles(TabManager * tm);
	~TabFiles();

protected:
	ISXGUIListBox* FileList;
	ISXGUIButton* FileAddBtn;
	ISXGUIButton* FileRemoveBtn;
};

#endif
