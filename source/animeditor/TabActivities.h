#ifndef TabActivities_H
#define TabActivities_H

#include "EditorTab.h"
#include <anim/animated.h>

class TabActivities:public EditorTab
{
public:
	TabActivities(TabManager * tm);
	~TabActivities();

	struct ActivityItem
	{
		String act;
		ModelFile const * mdl;
		bool isImported;
	};

	static LRESULT AddBtnCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT DelBtnCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT RenBtnCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static LRESULT OnCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:

	ISXGUIListBox* ActList;
	ISXGUIButton* ActNewBtn;
	ISXGUIButton* ActRenameBtn;
	ISXGUIButton* ActRemoveBtn;

	Array<ActivityItem> m_vItems;



private:
	void RenderList();
};

#endif
