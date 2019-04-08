#ifndef __PROPERTY_WINDOW_H
#define __PROPERTY_WINDOW_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <common/array.h>
#include <common/assotiativearray.h>
#include <common/string.h>
#include <common/aastring.h>

enum X_PROP_EDITOR_TYPE
{
	XPET_TEXT,
	XPET_FILE,
	XPET_COMBO,

	XPET__LAST
};

struct X_PROP_FIELD
{
	const char *szKey;
	const char *szName;
	X_PROP_EDITOR_TYPE editorType;
	const void *pEditorData;
};

class CPropertyWindow
{
protected:
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK PropDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ClassListWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void onSelChanged();
	void filterClassList(const char *szFilter);
public:
	CPropertyWindow(HINSTANCE hInstance, HWND hMainWnd);
	~CPropertyWindow();

	void show();
	void hide();
	bool isVisible();

	void clearClassList();
	void addClass(const char *szClassName);

	void clearProps();
	void addPropField(const X_PROP_FIELD *pField, const char *szValue = NULL);
	void setPropFieldValue(const char *szKey, const char *szValue);


protected:

	HINSTANCE m_hInstance;
	HWND m_hDlgWnd; // tab control handle
	HWND m_hTabControl; // tab control handle
	HWND m_hCurrentTab = NULL; // tab dialog handle
	Array<HWND> m_hPropTabs;

	HWND m_hClassListWnd = NULL;
	HWND m_hPropListWnd = NULL;

	WNDPROC m_pfnClassListOldWndproc;

	Array<String> m_asClassList;

	HWND m_phEditors[XPET__LAST];

	struct prop_s
	{
		X_PROP_FIELD field;
		String sValue;
	};

	AssotiativeArray<AAString, prop_s> m_aPropFields;
};

#endif
