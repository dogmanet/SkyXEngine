#ifndef __LEVELOPEN_H
#define __LEVELOPEN_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <common/array.h>
#include <common/assotiativearray.h>
#include <common/string.h>
#include <common/aastring.h>
#include "terrax.h"

#define MAX_LEVEL_STRING 128

class CLevelOpenDialog
{
protected:
	INT_PTR CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
public:
	CLevelOpenDialog(HINSTANCE hInstance, HWND hMainWnd);
	~CLevelOpenDialog();

	const char *getLevelName();

protected:

	void loadLevels();

	HINSTANCE m_hInstance;
	HWND m_hMainWnd;
	HWND m_hDlgWnd = NULL;
	HWND m_hListWnd = NULL;
	HWND m_hOkButtonWnd = NULL;

	char m_szLevelName[MAX_LEVEL_STRING];
};

#endif
