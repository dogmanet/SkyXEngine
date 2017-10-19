
#include "game_callback.h"

void SXLevelEditor::GameActivateAll(bool bf)
{
	SXLevelEditor::StaticGameClass->Visible(bf);
	SXLevelEditor::ComboBoxGameClass->Visible(bf);
	SXLevelEditor::ComboBoxGameClass->SetSel(0);
	SXLevelEditor::ButtonGameTab->Visible(bf);
	SXLevelEditor::ButtonGameTab->SetText("Connections");
	SXLevelEditor::ListViewGameClass->Visible(bf);
	SXLevelEditor::ListViewGameClass->ClearStrings();
	SXLevelEditor::ComboBoxGameValue->Visible(false);
	SXLevelEditor::ComboBoxGameValue->Clear();
	SXLevelEditor::EditGameValue->Visible(false);
	SXLevelEditor::EditGameValue->SetText("");
	SXLevelEditor::ButtonGameValue->Visible(false);
	SXLevelEditor::ListViewGameClass->Enable(false);

	SXLevelEditor::StaticGameHelp->Visible(bf);
	SXLevelEditor::MemoGameHelp->SetText("");
	SXLevelEditor::MemoGameHelp->Visible(bf);
	SXLevelEditor::ButtonGameCreate->Visible(bf);

	if (!bf)
		SXLevelEditor::GameVisibleConnections(false);
}

void SXLevelEditor::GameSel(int sel)
{
	SXLevelEditor::ComboBoxGameValue->Visible(false);
	SXLevelEditor::ComboBoxGameValue->Clear();
	SXLevelEditor::EditGameValue->Visible(false);
	SXLevelEditor::EditGameValue->SetText("");
	SXLevelEditor::ButtonGameCreate->Visible(false);
	SXLevelEditor::ListViewGameClass->Enable(true);
	SXLevelEditor::ListViewGameClass->ClearStrings();
	ID seldata = SXLevelEditor::ListBoxList->GetItemData(sel);
	SXLevelEditor::ActiveElement = seldata;
	SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;
	SXbaseEntity* bEnt = SXGame_EntGet(seldata);
	proptable_t* pt = SXGame_EntGetProptable(bEnt->GetClassName());

	char txtclasscb[256];
	const char* txtclassent = bEnt->GetClassName();
	for (int i = 0; i < SXLevelEditor::ComboBoxGameClass->GetCount(); ++i)
	{
		SXLevelEditor::ComboBoxGameClass->GetItemText(i, txtclasscb);
		if (strcmp(txtclasscb, txtclassent) == 0)
			SXLevelEditor::ComboBoxGameClass->SetSel(i);
	}
	
	propdata_t* pd;
	char txtkey[256];
	char txtval[256];

	Array<proptable_t*> tmparr;

	proptable_t* ptparent = pt->pBaseProptable;
	while (ptparent)
	{
		tmparr.push_back(ptparent);
		ptparent = ptparent->pBaseProptable;
	}

	for (int k = 0; k < tmparr.size(); ++k)
	{
		ptparent = tmparr[(tmparr.size() - 1) - k];
		for (int i = 0; i < ptparent->numFields; ++i)
		{
			pd = &ptparent->pData[i];
			if (pd->szKey && pd->szEdName && !(pd->flags & PDFF_NOEDIT) && pd->editor.type != PDE_NONE)
			{
				sprintf(txtkey, "%s", pd->szEdName);
				bEnt->GetKV(pd->szKey, txtval, 256);
				int str = SXLevelEditor::ListViewGameClass->AddString((long)pd);
				SXLevelEditor::ListViewGameClass->SetTextItem(txtkey, 0, str);
				//propdata_t* pd2 = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);
				SXLevelEditor::ListViewGameClass->SetTextItem(txtval, 1, str);
			}
		}
	}

	tmparr.clear();

	for (int i = 0; i < pt->numFields; ++i)
	{
		pd = &pt->pData[i];
		if (pd->szKey && pd->szEdName && !(pd->flags & PDFF_NOEDIT) && pd->editor.type != PDE_NONE)
		{
			sprintf(txtkey, "%s", pd->szEdName);
			bEnt->GetKV(pd->szKey, txtval, 256);
			int str = SXLevelEditor::ListViewGameClass->AddString((long)pd);
			SXLevelEditor::ListViewGameClass->SetTextItem(txtkey, 0, str);
			//propdata_t* pd2 = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);
			SXLevelEditor::ListViewGameClass->SetTextItem(txtval, 1, str);
		}
	}

	SXLevelEditor::ObjAxesHelper->SetPosition(bEnt->GetPos());
	SXLevelEditor::ObjAxesHelper->SetRotation(bEnt->GetOrient());
	SXLevelEditor::ObjAxesHelper->SetScale(float3(1, 1, 1));
}

void SXLevelEditor::GameUpdatePosRot()
{
	if (SXLevelEditor::ActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GAME || SXLevelEditor::ActiveElement < 0)
		return;

	SXbaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ActiveElement);
	propdata_t* pd = 0;
	char txtval[256];

	for (int i = 0; i < SXLevelEditor::ListViewGameClass->GetCountString(); ++i)
	{
		pd = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(i);

		if (pd)
		{
			if (stricmp(pd->szKey, "origin") == 0 || stricmp(pd->szKey, "rotation") == 0)
			{
				bEnt->GetKV(pd->szKey, txtval, 256);
				SXLevelEditor::ListViewGameClass->SetTextItem(txtval, 1, i);
			}
		}
	}
}

LRESULT SXLevelEditor_ListViewGameClass_Click()
{
	int str = SXLevelEditor::ListViewGameClass->GetSelString();
	if (str < 0)
	{
		SXLevelEditor::EditGameValue->Visible(false);
		SXLevelEditor::ButtonGameValue->Visible(false);
		SXLevelEditor::ComboBoxGameValue->Visible(false);
		return 0;
	}
	propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);

	SXLevelEditor::EditGameValue->Visible(false);
	SXLevelEditor::ButtonGameValue->Visible(false);
	SXLevelEditor::ComboBoxGameValue->Visible(false);

	char txtval[256];

	if (pd->editor.type == PDE_TEXTFIELD)
	{
		SXLevelEditor::EditGameValue->Visible(true);
		SXLevelEditor::ListViewGameClass->GetTextItem(txtval, 1, str, 256);
		SXLevelEditor::EditGameValue->SetText(txtval);
	}
	else if (pd->editor.type == PDE_FILEFIELD)
	{
		SXLevelEditor::EditGameValue->Visible(true);
		SXLevelEditor::ButtonGameValue->Visible(true);
		SXLevelEditor::ListViewGameClass->GetTextItem(txtval, 1, str, 256);
		SXLevelEditor::EditGameValue->SetText(txtval);
	}
	else if (pd->editor.type == PDE_COMBOBOX)
	{
		SXLevelEditor::ComboBoxGameValue->Clear();
		SXLevelEditor::ComboBoxGameValue->Visible(true);
		editor_kv* ekv = (editor_kv*)pd->editor.pData;
		int elem = 0;
		while (ekv)
		{
			if (ekv->key && ekv->value)
			{
				SXLevelEditor::ComboBoxGameValue->AddItem(ekv->key);
				SXLevelEditor::ComboBoxGameValue->SetItemData(elem, (LPARAM)ekv->value);
				++elem;
				ekv++;
			}
			else
				break;
		}

		static char tval[256];
		tval[0] = 0;
		SXLevelEditor::ListViewGameClass->GetTextItem(tval, 1, str, 256);

		const char* tval2;
		char tval3[256];
		bool isfound = false;

		//проверяем в цикле какая строка из комбобокса выбрана
		for (int i = 0; i < SXLevelEditor::ComboBoxGameValue->GetCount(); ++i)
		{
			//сначала проверяем по юзердате (там записано значение)
			tval2 = (const char*)SXLevelEditor::ComboBoxGameValue->GetItemData(i);

			if (strcmp(tval, tval2) == 0)
			{
				SXLevelEditor::ComboBoxGameValue->SetSel(i);
				isfound = true;
				break;
			}

			//затем проверям по имени строки
			SXLevelEditor::ComboBoxGameValue->GetItemText(i, tval3);
			
			if (strcmp(tval, tval3) == 0)
			{
				SXLevelEditor::ComboBoxGameValue->SetSel(i);
				isfound = true;
				break;
			}
		}

		//если не найдено значение значит надо его добавить
		if (!isfound)
		{
			MessageBox(0,"SXLevelEditor_ListViewGameClass_Click data not found",0,0);
			SXLevelEditor::ComboBoxGameValue->AddItem(tval);
			SXLevelEditor::ComboBoxGameValue->SetItemData(SXLevelEditor::ComboBoxGameValue->GetCount() - 1, (LPARAM)tval);
			SXLevelEditor::ComboBoxGameValue->SetSel(SXLevelEditor::ComboBoxGameValue->GetCount()-1);
		}
	}
	return 0;
}

LRESULT SXLevelEditor_EditGameValue_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->GetSel();
	
	if (sel < 0)
		return 0;

	int str = SXLevelEditor::ListViewGameClass->GetSelString();
	
	if (str < 0)
		return 0;

	char txt[256];
	SXLevelEditor::EditGameValue->GetText(txt, 256);
	SXLevelEditor::ListViewGameClass->SetTextItem(txt, 1, str);
	SXbaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel));
	if (bEnt)
	{
		propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);
		bool bf = bEnt->SetKV(pd->szKey, txt);
		int qwerty = 0;

		if (strcmp(pd->szKey, "name") == 0)
		{
			char tmpname[512];
			sprintf(tmpname, "%s / %s", bEnt->GetName(), bEnt->GetClassName());
			SXLevelEditor::ListBoxList->SetTextItem(sel, tmpname);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGameValue_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int str = SXLevelEditor::ListViewGameClass->GetSelString();
	propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);

	int len = 0;
	editor_kv* ekv = (editor_kv*)pd->editor.pData;

	while (ekv)
	{
		if (ekv->key && ekv->value)
		{
			len += strlen(ekv->key) + strlen(ekv->value) + 2;
			ekv++;
		}
		else
			break;
	}

	len += 2;

	char* txtfmt = new char[len];

	len = 0;
	ekv = (editor_kv*)pd->editor.pData;

	while (ekv)
	{
		if (ekv->key && ekv->value)
		{
			memcpy(txtfmt + len, ekv->key, strlen(ekv->key));
			len += strlen(ekv->key);
			txtfmt[len++] = 0;
			memcpy(txtfmt + len, ekv->value, strlen(ekv->value));
			len += strlen(ekv->value);
			txtfmt[len++] = 0;
			ekv++;
		}
		else
			break;
	}

	txtfmt[len++] = 0;

	//char tmpstr[1024];
	//memcpy(tmpstr, txtfmt, len);

	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), txtfmt);
	if (def_str_validate(tmppath))
	{
		String tmpstr = tmppath + strlen(Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE));
		tmpstr.ReplaceAll("\\", "/");
		sprintf(tmpname, "%s", tmpstr.c_str());
		SXLevelEditor::EditGameValue->SetText(tmpname);

		int sel = SXLevelEditor::ListBoxList->GetSel();
		SXLevelEditor::ListViewGameClass->SetTextItem(tmpname, 1, str);
		SXbaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel));
		if (bEnt)
		{
			bEnt->SetKV(pd->szKey, tmpname);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGameCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::ComboBoxGameClass->GetSel() == 0)
	{
		MessageBox(0, "Class of game object is not selected!", 0, 0);
		return 0;
	}
	char txt[256];
	SXLevelEditor::ComboBoxGameClass->GetItemText(SXLevelEditor::ComboBoxGameClass->GetSel(), txt);

	SXbaseEntity* bEnt = SXGame_CreateEntity(txt);
	bEnt->SetFlags(bEnt->GetFlags() | EF_EXPORT | EF_LEVEL);

	SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);
	int sel = SXLevelEditor::ListBoxList->GetCountItem()-1;
	SXLevelEditor::ListBoxList->SetSel(sel);
	SXLevelEditor::GameSel(sel);
	SXLevelEditor::ActiveElement = sel;
	SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;

	return 0;
}

LRESULT SXLevelEditor_EditGameConnectionsName_IN(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char edit_text[256];
	SXLevelEditor::EditGameConnectionsName->GetText(edit_text, 256);
	if (edit_text[0] == 0)
		return 0;

	if (!(isalpha(wParam) || isdigit(wParam) || (char)wParam == '_'))
		return 0;

	char* lower_text = CharLower(edit_text);
	int tmpcoungo = SXGame_EntGetCount();
	int tmpcoungo2 = 0;
	char tmpname[256];
	char* lower_name = 0;
	char* found = 0;
	for (int i = 0; i < tmpcoungo; ++i)
	{
		SXbaseEntity* bEnt = SXGame_EntGet(i);
		if (bEnt)
		{
			strcpy(tmpname, bEnt->GetName());
			if (tmpname[0] == 0)
				continue;
			lower_name = CharLower(tmpname);
			if (found = strstr(lower_name, lower_text))
			{
				int qq = found - lower_name;
				if (qq == 0)
				{
					SXLevelEditor::EditGameConnectionsName->SetText(lower_name);
					PostMessage(SXLevelEditor::EditGameConnectionsName->GetHWND(), EM_SETSEL, strlen(edit_text), strlen(lower_name));
					return 0;
				}
				//int qwerty = 0;
				//return 0;
			}
		}
	}

	return 0;
}

void SXLevelEditor::GameVisibleProperties(bool bf)
{
	SXLevelEditor::StaticGameClass->Visible(bf);
	SXLevelEditor::ComboBoxGameClass->Visible(bf);
	SXLevelEditor::ListViewGameClass->Visible(bf);
	//SXLevelEditor::ComboBoxGameValue->Visible(bf);
	//SXLevelEditor::EditGameValue->Visible(bf);
	//SXLevelEditor::ButtonGameValue->Visible(bf);
	SXLevelEditor::StaticGameHelp->Visible(bf);
	SXLevelEditor::MemoGameHelp->Visible(bf);
	SXLevelEditor::ButtonGameCreate->Visible(bf);
}

void SXLevelEditor::GameVisibleConnections(bool bf)
{
	SXLevelEditor::ListViewGameConnections->Visible(bf);
	SXLevelEditor::StaticGameConnectionsEvent->Visible(bf);
	SXLevelEditor::ComboBoxGameConnectionsEvent->Visible(bf);
	SXLevelEditor::StaticGameConnectionsName->Visible(bf);
	SXLevelEditor::EditGameConnectionsName->Visible(bf);
	SXLevelEditor::StaticGameConnectionsAction->Visible(bf);
	SXLevelEditor::ComboBoxGameConnectionsAction->Visible(bf);
	SXLevelEditor::StaticGameConnectionsDelay->Visible(bf);
	SXLevelEditor::EditGameConnectionsDelay->Visible(bf);
	SXLevelEditor::StaticGameConnectionsParameter->Visible(bf);
	SXLevelEditor::EditGameConnectionsParameter->Visible(bf);
}

LRESULT SXLevelEditor_ButtonGameTab_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::GameTabVal == 1)
	{
		SXLevelEditor::ButtonGameTab->SetText("Connections");
		SXLevelEditor::GameVisibleProperties(true);
		SXLevelEditor::GameVisibleConnections(false);
		SXLevelEditor::GameTabVal = 0;
	}
	else
	{
		SXLevelEditor::ButtonGameTab->SetText("Properties");
		SXLevelEditor::GameVisibleProperties(false);
		SXLevelEditor::GameVisibleConnections(true);
		SXLevelEditor::GameTabVal = 1;
	}

	return 0;
}

