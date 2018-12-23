
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "TabActivities.h"
#include "Tools.h"

TabActivities::TabActivities(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Activities", 479, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pTabBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->setFollowParentSides(false, true, true, false);
	m_pTabBtn->setUserPtr(this);
	m_pTabBtn->addHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Activities", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->getHWND(), 0, 0);
	m_pRoot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->setColorText(RGB(0, 0, 0));
	m_pRoot->setColorTextBk(RGB(255, 255, 255));
	m_pRoot->setTransparentTextBk(true);
	m_pRoot->setColorBrush(RGB(255, 255, 255));
	m_pRoot->setFollowParentSides(0, 1, 1, 1);
	m_pRoot->setUserPtr(this);
	m_pRoot->addHandler(OnCommand, WM_COMMAND);

	ActList = SXGUICrListBoxEx(6, 16, 1003, 122, 0, WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | LBS_NOTIFY/* | LBS_SORT*/, m_pRoot->getHWND(), 0, IDC_LISTBOX);
	ActList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActList->setColorText(RGB(0, 0, 0));
	ActList->setColorTextBk(RGB(255, 255, 255));
	ActList->setTransparentTextBk(true);
	ActList->setColorBrush(RGB(255, 255, 255));
	ActList->setFollowParentSides(1, 1, 1, 1);

	ActNewBtn = SXGUICrButton("New", 7, 140, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	ActNewBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActNewBtn->setFollowParentSides(0, 1, 1, 0);
	ActNewBtn->addHandler(AddBtnCB, WM_LBUTTONUP);
	ActNewBtn->setUserPtr(this);

	ActRenameBtn = SXGUICrButton("Rename", 110, 140, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	ActRenameBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActRenameBtn->setFollowParentSides(0, 1, 1, 0);
	ActRenameBtn->addHandler(RenBtnCB, WM_LBUTTONUP);
	ActRenameBtn->setUserPtr(this);
	ActRenameBtn->setEnable(0);

	ActRemoveBtn = SXGUICrButton("Remove", 213, 140, 100, 20, SXGUI_BUTTON_IMAGE_NONE, m_pRoot->getHWND(), 0, 0);
	ActRemoveBtn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActRemoveBtn->setFollowParentSides(0, 1, 1, 0);
	ActRemoveBtn->addHandler(DelBtnCB, WM_LBUTTONUP);
	ActRemoveBtn->setUserPtr(this);
	ActRemoveBtn->setEnable(0);

	Show(false);

	//m_vItems.push_back({"ACT_BASE", NULL, false});
	//m_vItems.push_back({"ACT_NEW", NULL, true});
	//RenderList();
}

TabActivities::~TabActivities()
{
	mem_delete(ActList);
	mem_delete(ActNewBtn);
	mem_delete(ActRenameBtn);
	mem_delete(ActRemoveBtn);
}


void TabActivities::RenderList()
{
	ActRenameBtn->setEnable(0);
	ActRemoveBtn->setEnable(0);

	UINT c = m_vItems.size();
	ActivityItem * item;
	char tmpSN[MODEL_MAX_NAME + 32];
	int cur = ActList->getSel();
	if(cur < 0)
	{
		cur = 0;
	}
	ActList->clear();
	for(UINT i = 0; i < c; ++i)
	{
		item = &m_vItems[i];
	
		sprintf(tmpSN, "[%c] %s", item->isImported ? 'I' : '_', item->act); //I|_, L|_
		ActList->addItem(tmpSN);
		ActList->setItemData(ActList->getItemCount() - 1, (LPARAM)i);
	}
	ActList->setSel(cur);
	
	PostMessage(GetAncestor(m_pRoot->getHWND(), GA_ROOTOWNER), EM_LOADACTIVITIES, 0, 0);
}

LRESULT TabActivities::AddBtnCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	TabActivities * self = (TabActivities*)cmp->getUserPtr();

	char out[MODEL_MAX_NAME];
	int size = MODEL_MAX_NAME;
	if(Tools::DlgPrompt(out, &size, "Activity name", "New activity") && size > 0)
	{
		for(int i = 0; i < size; ++i)
		{
			out[i] = toupper(out[i]);
		}
		for(int i = 0, l = self->m_vItems.size(); i < l; ++i)
		{
			if(!_stricmp(self->m_vItems[i].act.c_str(), out))
			{
				MessageBoxA(hwnd, "Activity with that name already exists", "Error", MB_OK | MB_ICONSTOP);
				return(0);
			}
		}
		self->m_vItems.push_back({out, NULL, false});
		self->RenderList();
	}

	return(0);
}

LRESULT TabActivities::DelBtnCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	TabActivities * self = (TabActivities*)cmp->getUserPtr();

	if(Tools::DlgConfirm("Are you shure want to remove this item?", "Remove?"))
	{
		int cur = self->ActList->getSel();
		cur = self->ActList->getItemData(cur);
		self->m_vItems.erase(cur);
		self->RenderList();
	}

	return(0);
}

LRESULT TabActivities::RenBtnCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	TabActivities * self = (TabActivities*)cmp->getUserPtr();

	int cur = self->ActList->getSel();
	cur = self->ActList->getItemData(cur);
	ActivityItem * item = &self->m_vItems[cur];

	char out[MODEL_MAX_NAME];
	int size = MODEL_MAX_NAME;
	if(Tools::DlgPrompt(out, &size, "Activity name", "Rename activity", item->act.c_str()) && size > 0)
	{
		for(int i = 0; i < size; ++i)
		{
			out[i] = toupper(out[i]);
		}
		item->act = out;
		self->RenderList();
	}
	

	return(0);
}

LRESULT TabActivities::OnCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	TabActivities * self = (TabActivities*)cmp->getUserPtr();

	switch(LOWORD(wParam))
	{
	case IDC_LISTBOX:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			{
				int sel = self->ActList->getSel();
				ActivityItem * item;
				item = &self->m_vItems[self->ActList->getItemData(sel)];
				self->ActRenameBtn->setEnable(!item->isImported);
				self->ActRemoveBtn->setEnable(!item->isImported);
			}
			break;
		}
	}

	return(0);
}
