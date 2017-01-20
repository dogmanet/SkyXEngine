#ifndef TabActivities_H
#define TabActivities_H

#include "EditorTab.h"

class TabActivities:public EditorTab
{
public:
	TabActivities(TabManager * tm);
	~TabActivities();

protected:

	ISXGUIListBox* ActList;
	ISXGUIButton* ActNewBtn;
	ISXGUIButton* ActRenameBtn;
	ISXGUIButton* ActRemoveBtn;
};

#endif
