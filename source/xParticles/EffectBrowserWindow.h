#ifndef __EDITORBROWSERWINDOW_H
#define __EDITORBROWSERWINDOW_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <commctrl.h>
#include <common/array.h>
#include <common/assotiativearray.h>
#include <common/string.h>
#include <common/aastring.h>
#include <xcommon/IFileSystem.h>

class CEffectBrowserWindow final
{
private:
	INT_PTR CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
public:
	CEffectBrowserWindow(HINSTANCE hInstance, HWND hMainWnd, IFileSystem *pFS);
	~CEffectBrowserWindow();

	//const char *getLevelName();

private:
	void registerClass();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void createTree();
	void createTreeLevel(const char *szCurrentFolder, HTREEITEM hParent);

private:
	HINSTANCE m_hInstance;
	HWND m_hMainWnd;
	HWND m_hDlgWnd = NULL;

	IFileSystem *m_pFS;

	HWND m_hTreeWnd = NULL;
	HWND m_hOkButtonWnd = NULL;
};

#endif
