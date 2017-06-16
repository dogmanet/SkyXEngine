
#ifndef SXGUI_LIST_VIEW_H
#define SXGUI_LIST_VIEW_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIListView : public SXGUIComponent, public virtual ISXGUIListView
{
public:
	SXGUIListView();
	SXGUIListView(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
	SXGUIListView(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

	int InsertColumn(char* text, int col, int width);
	int AddColumn(char* text, int width);
	void DeleteColumn(int col);
	int GetCountColumns();

	int AddString(long data = -1);
	void DeleteString(int str);
	int GetCountString();

	void SetTextItem(char* text, int col, int str);
	void GetTextItem(char* text, int col, int str, int sizebuff);

	void SetDataItem(int str, long data);
	long GetDataItem(int str);

	int GetSelString(int start = -1);
	void SetSelString(int str);

	int GetSelColumn();
	void SetSelColumn(int col);

	void Clear();
	void ClearStrings();
};

#endif