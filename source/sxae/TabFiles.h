
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TAB_FILES_H
#define __TAB_FILES_H

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
