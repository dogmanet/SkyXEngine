
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_LIST_VIEW_H
#define __SXGUI_LIST_VIEW_H

#include "base.h"


class CGUIListView : public CGUIComponent, public virtual ISXGUIListView
{
public:
	CGUIListView();
	CGUIListView(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIListView(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj);

	int insertColumn(const char* text, int col, int width);
	int addColumn(const char* text, int width);
	void deleteColumn(int col);
	int getColumnsCount();

	int addString(LPARAM data = -1);
	void deleteString(int str);
	int getStringCount();

	void setItemText(const char* text, int col, int str);
	void getItemText(char* text, int col, int str, int sizebuff);

	void setItemData(int str, LPARAM data);
	LPARAM getItemData(int str);

	int getSelString(int start = -1);
	void setSelString(int str);

	int getSelColumn();
	void setSelColumn(int col);

	void clear();
	void clearStrings();

private:
	Array<Array<String>> m_aStrings;
	Array<long> m_aStringData;
};

#endif