#ifndef Editor_H
#define Editor_H

#include <SXGUIWinApi/sxguielements.h>
#include "../../SkyXEngine/animeditor/resource.h"

#include "TabManager.h"

#include <anim/ModelFile.h>
//#include <anim/animated.h>


/**@TODO:
	- Добавить окошко лодов
	- Добавить окошко хитбоксов
*/
class Editor
{
public:
	Editor();
	~Editor();


	static LRESULT MenuCmd(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	void MenuBrowse(HWND hwnd);
	void MenuSave();
	void MenuSaveAs(HWND hwnd);
	
	void InitUI();
	void DestroyUI();

protected:
	ISXGUIBaseWnd* MainWindow;
	ISXGUIBaseWnd* Window1;
	ISXGUIListBox* AnimList;
	ISXGUIGroupBox* AnimationsGB;
	ISXGUIEdit* AnimFilter;
	ISXGUIStatic* Static1;

	ISXGUIStatic* CurAnimName;

	ISXGUIMenu * Menu;

	TabManager * m_pTM;

	bool m_bDirty;
};

#endif
