#include "TabActivities.h"
#include "Tools.h"

TabActivities::TabActivities(TabManager * tm):EditorTab(tm)
{
	m_pTabBtn = SXGUICrButtonEx("Activities", 479, 648, 100, 20, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pTabBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pTabBtn->GAlign = {false, true, true, false};
	m_pTabBtn->SetUserPtr(this);
	m_pTabBtn->AddHandler(EditorTab::TabBtnHandler, WM_LBUTTONUP);

	m_pRoot = SXGUICrGroupBox("Activities", 279, 480, 1016, 167, m_pTM->GetBaseWnd()->GetHWND(), 0, 0);
	m_pRoot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	m_pRoot->SetColorText(0, 0, 0);
	m_pRoot->SetColorTextBk(255, 255, 255);
	m_pRoot->SetTransparentTextBk(true);
	m_pRoot->SetColorBrush(255, 255, 255);
	m_pRoot->GAlign = {0, 1, 1, 1};
	m_pRoot->SetUserPtr(this);
	m_pRoot->AddHandler(OnCommand, WM_COMMAND);

	ActList = SXGUICrListBoxEx("", 6, 16, 1003, 122, 0, WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | LBS_NOTIFY/* | LBS_SORT*/, m_pRoot->GetHWND(), 0, IDC_LISTBOX);
	ActList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActList->SetColorText(0, 0, 0);
	ActList->SetColorTextBk(255, 255, 255);
	ActList->SetTransparentTextBk(true);
	ActList->SetColorBrush(255, 255, 255);
	ActList->GAlign = {1, 1, 1, 1};

	ActNewBtn = SXGUICrButton("New", 7, 140, 100, 20, 0, m_pRoot->GetHWND(), 0, 0);
	ActNewBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActNewBtn->GAlign = {0, 1, 1, 0};
	ActNewBtn->AddHandler(AddBtnCB, WM_LBUTTONUP);
	ActNewBtn->SetUserPtr(this);

	ActRenameBtn = SXGUICrButton("Rename", 110, 140, 100, 20, 0, m_pRoot->GetHWND(), 0, 0);
	ActRenameBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActRenameBtn->GAlign = {0, 1, 1, 0};
	ActRenameBtn->AddHandler(RenBtnCB, WM_LBUTTONUP);
	ActRenameBtn->SetUserPtr(this);
	ActRenameBtn->Enable(0);

	ActRemoveBtn = SXGUICrButton("Remove", 213, 140, 100, 20, 0, m_pRoot->GetHWND(), 0, 0);
	ActRemoveBtn->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	ActRemoveBtn->GAlign = {0, 1, 1, 0};
	ActRemoveBtn->AddHandler(DelBtnCB, WM_LBUTTONUP);
	ActRemoveBtn->SetUserPtr(this);
	ActRemoveBtn->Enable(0);

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
	ActRenameBtn->Enable(0);
	ActRemoveBtn->Enable(0);

	UINT c = m_vItems.size();
	ActivityItem * item;
	char tmpSN[MODEL_MAX_NAME + 32];
	int cur = ActList->GetSel();
	ActList->Clear();
	for(UINT i = 0; i < c; ++i)
	{
		item = &m_vItems[i];
	
		sprintf(tmpSN, "[%c] %s", item->isImported ? 'I' : '_', item->act); //I|_, L|_
		ActList->AddItem(tmpSN);
		ActList->SetItemData(ActList->GetCountItem() - 1, (LPARAM)i);
	}
	ActList->SetSel(cur);
}

LRESULT TabActivities::AddBtnCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	TabActivities * self = (TabActivities*)cmp->GetUserPtr();

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
	TabActivities * self = (TabActivities*)cmp->GetUserPtr();

	if(Tools::DlgConfirm("Are you shure want to remove this item?", "Remove?"))
	{
		int cur = self->ActList->GetSel();
		cur = self->ActList->GetItemData(cur);
		self->m_vItems.erase(cur);
		self->RenderList();
	}

	return(0);
}

LRESULT TabActivities::RenBtnCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	TabActivities * self = (TabActivities*)cmp->GetUserPtr();

	int cur = self->ActList->GetSel();
	cur = self->ActList->GetItemData(cur);
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
	TabActivities * self = (TabActivities*)cmp->GetUserPtr();

	switch(LOWORD(wParam))
	{
	case IDC_LISTBOX:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			{
				int sel = self->ActList->GetSel();
				ActivityItem * item;
				item = &self->m_vItems[self->ActList->GetItemData(sel)];
				self->ActRenameBtn->Enable(!item->isImported);
				self->ActRemoveBtn->Enable(!item->isImported);
			}
			break;
		}
	}

	return(0);
}
