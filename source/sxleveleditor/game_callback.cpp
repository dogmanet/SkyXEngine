#include "string.h"
void GameCActivateAllElems(bool bf)
{
	SXLevelEditor::StaticGameClass->Visible(bf);
	SXLevelEditor::ComboBoxGameClass->Visible(bf);
	SXLevelEditor::ComboBoxGameClass->SetSel(0);
	SXLevelEditor::ListViewGameClass->Visible(bf);
	SXLevelEditor::ListViewGameClass->ClearStrings();
	SXLevelEditor::ComboBoxGameValue->Visible(false);
	SXLevelEditor::ComboBoxGameValue->Clear();
	SXLevelEditor::EditGameValue->Visible(false);
	SXLevelEditor::EditGameValue->SetText("");
	//SXLevelEditor::ComboBoxGameValue->Clear();
	SXLevelEditor::ListViewGameClass->Enable(false);

	SXLevelEditor::StaticGameHelp->Visible(bf);
	SXLevelEditor::MemoGameHelp->SetText("");
	SXLevelEditor::MemoGameHelp->Visible(bf);
	SXLevelEditor::ButtonGameCreate->Visible(bf);
}

void GameCInitElemsSelModel(int sel)
{
	SXLevelEditor::ComboBoxGameValue->Visible(false);
	SXLevelEditor::ComboBoxGameValue->Clear();
	SXLevelEditor::EditGameValue->Visible(false);
	SXLevelEditor::EditGameValue->SetText("");
	SXLevelEditor::ButtonGameCreate->Visible(false);
	SXLevelEditor::ListViewGameClass->Enable(true);
	SXLevelEditor::ListViewGameClass->ClearStrings();
	SXbaseEntity* bEnt = SXGame_EntGet(sel);
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

		char tval[256];
		SXLevelEditor::ListViewGameClass->GetTextItem(tval, 1, str, 256);

		char tval2[256];
		bool isfound = false;

		for (int i = 0; i < SXLevelEditor::ComboBoxGameValue->GetCount(); ++i)
		{
			SXLevelEditor::ComboBoxGameValue->GetItemText(i,tval2);

			if (strcmp(tval, tval2) == 0)
			{
				SXLevelEditor::ComboBoxGameValue->SetSel(i);
				isfound = true;
				break;
			}
		}

		if (!isfound)
		{
			SXLevelEditor::ComboBoxGameValue->AddItem(tval);
			SXLevelEditor::ComboBoxGameValue->SetSel(SXLevelEditor::ComboBoxGameValue->GetCount()-1);
		}
	}
	return 0;
}

LRESULT SXLevelEditor_EditGameValue_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char txt[256];
	SXLevelEditor::EditGameValue->GetText(txt, 256);
	int sel = SXLevelEditor::ListBoxList->GetSel();
	int str = SXLevelEditor::ListViewGameClass->GetSelString();
	SXLevelEditor::ListViewGameClass->SetTextItem(txt, 1, str);
	SXbaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel));
	if (bEnt)
	{
		propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);
		bool bf = bEnt->SetKV(pd->szKey, txt);
		int qwerty = 0;
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
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::GameSource, txtfmt);
	if (def_str_validate(tmppath))
	{
		SXLevelEditor::EditGameValue->SetText(tmppath + strlen(GData::Pathes::GameSource));

		int sel = SXLevelEditor::ListBoxList->GetSel();
		SXLevelEditor::ListViewGameClass->SetTextItem(tmppath + strlen(GData::Pathes::GameSource), 1, str);
		SXbaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel));
		if (bEnt)
		{
			bEnt->SetKV(pd->szKey, tmppath + strlen(GData::Pathes::GameSource));
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
	GameCInitElemsSelModel(sel);
	GData::Editors::ActiveElement = sel;
	GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;

	return 0;
}