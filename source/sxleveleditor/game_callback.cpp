
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

	for (int i = 0; i < 16; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i]->Visible(false);
	}

	if (!bf)
		SXLevelEditor::GameVisibleConnections(false);
}

void SXLevelEditor::GameSel(int sel)
{
	for (int i = 0; i < 16; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i]->Visible(false);
	}

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
	CBaseEntity* bEnt = SXGame_EntGet(seldata);
	proptable_t* pt = SXGame_EntGetProptable(bEnt->getClassName());

	char txtclasscb[256];
	const char* txtclassent = bEnt->getClassName();
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

	
	memset(SXLevelEditor::aGameObjectFlags, 0, sizeof(const char*)* 16);

	for (int k = 0; k < tmparr.size(); ++k)
	{
		ptparent = tmparr[(tmparr.size() - 1) - k];
		for (int i = 0; i < ptparent->numFields; ++i)
		{
			pd = &ptparent->pData[i];
			if (pd->szKey && pd->szEdName && !(pd->flags & PDFF_NOEDIT) && pd->editor.type != PDE_NONE)
			{
				sprintf(txtkey, "%s", pd->szEdName);
				bEnt->getKV(pd->szKey, txtval, 256);
				int str = SXLevelEditor::ListViewGameClass->AddString((long)pd);
				SXLevelEditor::ListViewGameClass->SetTextItem(txtkey, 0, str);
				//propdata_t* pd2 = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);
				SXLevelEditor::ListViewGameClass->SetTextItem(txtval, 1, str);
			}

			if (pd->szEdName && pd->type == PDF_FLAG)
			{
				int index = log2(pd->flags) - 16;
				SXLevelEditor::aGameObjectFlags[index] = pd->szEdName;
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
			bEnt->getKV(pd->szKey, txtval, 256);
			int str = SXLevelEditor::ListViewGameClass->AddString((long)pd);
			SXLevelEditor::ListViewGameClass->SetTextItem(txtkey, 0, str);
			//propdata_t* pd2 = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);
			SXLevelEditor::ListViewGameClass->SetTextItem(txtval, 1, str);
		}

		if (pd->szEdName && pd->type == PDF_FLAG)
		{
			int index = log2(pd->flags) - 16;
			SXLevelEditor::aGameObjectFlags[index] = pd->szEdName;
		}
	}

	//заполнение таблицы и ComboBoxGameConnectionsEvent
	SXLevelEditor::ComboBoxGameConnectionsEvent->Clear();
	SXLevelEditor::ComboBoxGameConnectionsAction->Clear();
	SXLevelEditor::ListViewGameConnections->ClearStrings();
	SXLevelEditor::EditGameConnectionsName->SetText("");
	SXLevelEditor::EditGameConnectionsDelay->SetText("");
	SXLevelEditor::EditGameConnectionsParameter->SetText("");

	char szBuffer4096[4096];

	//проходимся по всей таблице свойств класса объекта
	for (int i = 0; i < pt->numFields; ++i)
	{
		pd = &pt->pData[i];
		
		//если поле класса то что нам надо
		if (pd->flags & PDFF_OUTPUT)
		{
			//получаем строку с запакованными данными
			bEnt->getKV(pd->szKey, szBuffer4096, 4096);

			//парсим строку
			int iConns = parse_str(szBuffer4096, NULL, 0, ',');
			char ** parts = (char**)alloca(sizeof(char*)*iConns);
			iConns = parse_str(szBuffer4096, parts, iConns, ',');

			for (int k = 0; k < iConns; ++k)
			{
				int iConns2 = parse_str(parts[k], NULL, 0, ':');
				char ** parts2 = (char**)alloca(sizeof(char*)*iConns2);
				iConns2 = parse_str(parts[k], parts2, iConns2, ':');

				if (iConns2 > 0)
				{
					int iNumStr = SXLevelEditor::ListViewGameConnections->AddString((long)pd);
					sprintf(txtkey, "%s", pd->szEdName);
					SXLevelEditor::ListViewGameConnections->SetTextItem(txtkey, 0, iNumStr);

					for (int j = 0; j < iConns2; ++j)
						SXLevelEditor::ListViewGameConnections->SetTextItem(parts2[j], j + 1, iNumStr);

					CBaseEntity *pEnt2 = SXGame_EntGetByName(parts2[0], 0);
					if (pEnt2)
					{
						proptable_t *pPropTable2 = SXGame_EntGetProptable(pEnt2->getClassName());
						propdata_t *pPropData2 = 0;
						for (int j = 0; j < pPropTable2->numFields; ++j)
						{
							pPropData2 = &pPropTable2->pData[j];
							if (pPropData2->flags & PDFF_INPUT && strcmp(pPropData2->szKey, parts2[1])==0)
							{
								sprintf(txtkey, "%s", pPropData2->szEdName);
								SXLevelEditor::ListViewGameConnections->SetTextItem(txtkey, 2, iNumStr);
							}
						}
					}
				}
			}

			SXLevelEditor::ComboBoxGameConnectionsEvent->AddItem(pd->szEdName);
			SXLevelEditor::ComboBoxGameConnectionsEvent->SetItemData(SXLevelEditor::ComboBoxGameConnectionsEvent->GetCount()-1, (LPARAM)(pd->szKey));
		}
	}
	//-------
	
	SXLevelEditor::ObjAxesHelper->SetPosition(bEnt->getPos());
	SXLevelEditor::ObjAxesHelper->SetRotation(bEnt->getOrient());
	SXLevelEditor::ObjAxesHelper->SetScale(float3(1, 1, 1));
}

void SXLevelEditor::GameUpdatePosRot()
{
	if (SXLevelEditor::ActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GAME || SXLevelEditor::ActiveElement < 0)
		return;

	CBaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ActiveElement);
	propdata_t* pd = 0;
	char txtval[256];

	for (int i = 0; i < SXLevelEditor::ListViewGameClass->GetCountString(); ++i)
	{
		pd = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(i);

		if (pd)
		{
			if (stricmp(pd->szKey, "origin") == 0 || stricmp(pd->szKey, "rotation") == 0)
			{
				bEnt->getKV(pd->szKey, txtval, 256);
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

	for (int i = 0; i < 16; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i]->Visible(false);
	}

	if (pd->editor.type == PDE_TEXTFIELD)
	{
		SXLevelEditor::EditGameValue->Visible(true);
		SXLevelEditor::ListViewGameClass->GetTextItem(txtval, 1, str, 256);
		SXLevelEditor::EditGameValue->SetText(txtval);

		SXLevelEditor::StaticGameHelp->Visible(true);
		SXLevelEditor::MemoGameHelp->Visible(true);
	}
	else if (pd->editor.type == PDE_FILEFIELD)
	{
		SXLevelEditor::EditGameValue->Visible(true);
		SXLevelEditor::ButtonGameValue->Visible(true);
		SXLevelEditor::ListViewGameClass->GetTextItem(txtval, 1, str, 256);
		SXLevelEditor::EditGameValue->SetText(txtval);

		SXLevelEditor::StaticGameHelp->Visible(true);
		SXLevelEditor::MemoGameHelp->Visible(true);
	}
	else if (pd->editor.type == PDE_COMBOBOX)
	{
		SXLevelEditor::StaticGameHelp->Visible(true);
		SXLevelEditor::MemoGameHelp->Visible(true);

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
	else if (pd->editor.type == PDE_FLAGS)
	{
		SXLevelEditor::StaticGameHelp->Visible(false);
		SXLevelEditor::MemoGameHelp->Visible(false);
		char txtval[256];
		SXLevelEditor::ListViewGameClass->GetTextItem(txtval, 1, str, 256);
		UINT uiFlags;
		sscanf(txtval, "%d", &uiFlags);
		for (int i = 0; i < 16; ++i)
		{
			if (SXLevelEditor::aGameObjectFlags[i])
			{
				SXLevelEditor::CheckBoxGameFlags[i]->SetText(SXLevelEditor::aGameObjectFlags[i]);
				SXLevelEditor::CheckBoxGameFlags[i]->Visible(true);
				if (uiFlags & ((1 << i + 16)))
					SXLevelEditor::CheckBoxGameFlags[i]->SetCheck(true);
				else
					SXLevelEditor::CheckBoxGameFlags[i]->SetCheck(false);
			}
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
	CBaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel));
	if (bEnt)
	{
		propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->GetDataItem(str);
		bool bf = bEnt->setKV(pd->szKey, txt);
		int qwerty = 0;

		if (strcmp(pd->szKey, "name") == 0)
		{
			char tmpname[512];
			sprintf(tmpname, "%s / %s", bEnt->getName(), bEnt->getClassName());
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
		CBaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->GetItemData(sel));
		if (bEnt)
		{
			bEnt->setKV(pd->szKey, tmpname);
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

	CBaseEntity* bEnt = SXGame_CreateEntity(txt);
	bEnt->setFlags(bEnt->getFlags() | EF_EXPORT | EF_LEVEL);

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
		CBaseEntity* bEnt = SXGame_EntGet(i);
		if (bEnt)
		{
			strcpy(tmpname, bEnt->getName());
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

	for (int i = 0; i < 16; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i]->Visible(false);
	}
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
	SXLevelEditor::ButtonGameConnectionsCreate->Visible(bf);
	SXLevelEditor::ButtonGameConnectionsDelete->Visible(false);
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

//##########################################################################

LRESULT SXLevelEditor_ListViewGameConnections_Click()
{
	int iNumStr = SXLevelEditor::ListViewGameConnections->GetSelString();

	//очищаем комбобоксы
	//SXLevelEditor::ComboBoxGameConnectionsEvent->Clear();
	SXLevelEditor::ComboBoxGameConnectionsAction->Clear();

	//если нет выделенного тогда обнуляем
	if (iNumStr < 0)
	{
		SXLevelEditor::ComboBoxGameConnectionsEvent->SetSel(0);
		SXLevelEditor::EditGameConnectionsName->SetText("");
		SXLevelEditor::ComboBoxGameConnectionsAction->SetSel(0);
		SXLevelEditor::EditGameConnectionsDelay->SetText("");
		SXLevelEditor::EditGameConnectionsParameter->SetText("");
		return 0;
	}

	char szStr[256];
	char szStr2[256];

	//выделяем используемый event в ComboBoxGameConnectionsEvent
	SXLevelEditor::ListViewGameConnections->GetTextItem(szStr, 0, iNumStr, 256);

	for (int i = 0; i < SXLevelEditor::ComboBoxGameConnectionsEvent->GetCount(); ++i)
	{
		SXLevelEditor::ComboBoxGameConnectionsEvent->GetItemText(i, szStr2);
		if (strcmp(szStr, szStr2) == 0)
		{
			SXLevelEditor::ComboBoxGameConnectionsEvent->SetSel(i);
			break;
		}
	}
	//-------

	//устанавилваем имя используемого игрового объекта в EditGameConnectionsName
	SXLevelEditor::ListViewGameConnections->GetTextItem(szStr, 1, iNumStr, 256);
	SXLevelEditor::EditGameConnectionsName->SetText(szStr);
	//-------

	//устанавилваем данные в ComboBoxGameConnectionsAction и выделяем используемый
	CBaseEntity *pEnt2 = 0;

	//проходим по всем игровым объектам
	for (int i = 0; i < SXGame_EntGetCount(); ++i)
	{
		pEnt2 = SXGame_EntGet(i);

		//если имя игрового объекта идентично используемому
		if (pEnt2 && strcmp(pEnt2->getName(), szStr) == 0)
		{
			proptable_t *pPropTable = SXGame_EntGetProptable(pEnt2->getClassName());
			propdata_t *pPropData = 0;

			//проходимся по всем полям класса
			for (int k = 0; k < pPropTable->numFields; ++k)
			{
				pPropData = &pPropTable->pData[k];
				if (pPropData->flags & PDFF_INPUT)
				{
					SXLevelEditor::ComboBoxGameConnectionsAction->AddItem(pPropData->szEdName);
					SXLevelEditor::ComboBoxGameConnectionsAction->SetItemData(SXLevelEditor::ComboBoxGameConnectionsAction->GetCount() - 1, (LPARAM)(pPropData->szKey));
				}
			}
		}
	}

	SXLevelEditor::ListViewGameConnections->GetTextItem(szStr, 2, iNumStr, 256);
	for (int i = 0; i < SXLevelEditor::ComboBoxGameConnectionsAction->GetCount(); ++i)
	{
		SXLevelEditor::ComboBoxGameConnectionsAction->GetItemText(i, szStr2);
		if (strcmp(szStr, szStr2) == 0)
		{
			SXLevelEditor::ComboBoxGameConnectionsAction->SetSel(i);
			break;
		}
	}
	//-------

	//устанавливаем задержку в секундах в EditGameConnectionsDelay
	SXLevelEditor::ListViewGameConnections->GetTextItem(szStr, 3, iNumStr, 256);
	SXLevelEditor::EditGameConnectionsDelay->SetText(szStr);
	//-------

	//устанавливаем параметр в EditGameConnectionsParameter
	SXLevelEditor::ListViewGameConnections->GetTextItem(szStr, 4, iNumStr, 256);
	SXLevelEditor::EditGameConnectionsParameter->SetText(szStr);
	//-------

	SXLevelEditor::ButtonGameConnectionsDelete->Visible(true);
	SXLevelEditor::ButtonGameConnectionsCreate->SetText("Create");
	SXLevelEditor::isAddGameConections = false;

	return 0;
}

LRESULT SXLevelEditor_EditGameConnectionsName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = SXLevelEditor::ListBoxList->GetSel();
	ID idSelStrTable = SXLevelEditor::ListViewGameConnections->GetSelString();

	if (idSelListBoxList < 0)
		return 0;

	CBaseEntity *pEnt = SXGame_EntGet(idSelListBoxList);

	char szBuffer256[256];
	szBuffer256[0] = 0;

	SXLevelEditor::EditGameConnectionsName->GetText(szBuffer256, 256);

	CBaseEntity *pEnt2 = SXGame_EntGetByName(szBuffer256, 0);

	//записываем в szBuffer256 текущее имя action
	szBuffer256[0] = 0;
	SXLevelEditor::ComboBoxGameConnectionsAction->GetItemText(SXLevelEditor::ComboBoxGameConnectionsAction->GetSel(), szBuffer256);

	// если энтить с введенным именем существует
	if (pEnt2)
	{
		proptable_t *pPropTable2 = SXGame_EntGetProptable(pEnt2->getClassName());
		propdata_t *pPropData2 = 0;
		SXLevelEditor::ComboBoxGameConnectionsAction->Clear();

		//проходимся по всем данным таблицы свойств
		for (int j = 0; j < pPropTable2->numFields; ++j)
		{
			pPropData2 = &pPropTable2->pData[j];

			//если свойство input
			if (pPropData2->flags & PDFF_INPUT)
			{
				//добавляем в комбобокс action
				SXLevelEditor::ComboBoxGameConnectionsAction->AddItem(pPropData2->szEdName);
				SXLevelEditor::ComboBoxGameConnectionsAction->SetItemData(SXLevelEditor::ComboBoxGameConnectionsEvent->GetCount() - 1, (LPARAM)(pPropData2->szKey));

				//если предыдущее имя action равно последнему вставленному
				if (strcmp(pPropData2->szEdName, szBuffer256) == 0)
				{
					//активируем эту строку в комбобоксе
					SXLevelEditor::ComboBoxGameConnectionsAction->SetSel(SXLevelEditor::ComboBoxGameConnectionsAction->GetCount() - 1);
				}
			}
		}
	}
	// если такого энтитя не существует
	else
	{
		//возвращаем то что сейчас в таблице

		if (idSelStrTable < 0)
			return 0;

		szBuffer256[0] = 0;
		SXLevelEditor::ListViewGameConnections->GetTextItem(szBuffer256, 1, idSelStrTable, 256);
		SXLevelEditor::EditGameConnectionsName->SetText(szBuffer256);
	}

	return SXLevelEditor_EditGameConnections_Enter(hwnd, msg, wParam, lParam);
}

LRESULT SXLevelEditor_EditGameConnections_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::isAddGameConections)
		return 0;

	ID idSelListBoxList = SXLevelEditor::ListBoxList->GetSel();
	ID idSelStrTable = SXLevelEditor::ListViewGameConnections->GetSelString();

	if (idSelListBoxList < 0 || idSelStrTable < 0)
		return 0;

	ID idSelEnt = SXLevelEditor::ListBoxList->GetItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());
	propdata_t *pPropData = (propdata_t*)(SXLevelEditor::ListViewGameConnections->GetDataItem(idSelStrTable));

	//получаем скленные в единую строку данные
	char szBuffer4096[4096];
	szBuffer4096[0] = 0;
	SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, -1, szBuffer4096, 4096);


	//устанавливаем в выделенную в таблице строку данные из редакторов
	char szBuffer256[256];
	szBuffer256[0] = 0;

	SXLevelEditor::ComboBoxGameConnectionsEvent->GetItemText(SXLevelEditor::ComboBoxGameConnectionsEvent->GetSel(), szBuffer256);
	SXLevelEditor::ListViewGameConnections->SetTextItem(szBuffer256, 0, idSelStrTable);
	
	szBuffer256[0] = 0;
	SXLevelEditor::EditGameConnectionsName->GetText(szBuffer256, 256);
	SXLevelEditor::ListViewGameConnections->SetTextItem(szBuffer256, 1, idSelStrTable);

	szBuffer256[0] = 0;
	SXLevelEditor::ComboBoxGameConnectionsAction->GetItemText(SXLevelEditor::ComboBoxGameConnectionsAction->GetSel(), szBuffer256);
	SXLevelEditor::ListViewGameConnections->SetTextItem(szBuffer256, 2, idSelStrTable);

	szBuffer256[0] = 0;
	SXLevelEditor::EditGameConnectionsDelay->GetText(szBuffer256, 256);
	SXLevelEditor::ListViewGameConnections->SetTextItem(szBuffer256, 3, idSelStrTable);

	szBuffer256[0] = 0;
	SXLevelEditor::EditGameConnectionsParameter->GetText(szBuffer256, 256);
	SXLevelEditor::ListViewGameConnections->SetTextItem(szBuffer256, 4, idSelStrTable);

	pEnt->setKV(pPropData->szKey, szBuffer4096);

	return 0;
}

void SXLevelEditor_VeldStringGameConnections(proptable_t *pPropTable, propdata_t *pPropData, int iNumExclude, char* szStrOut, int iSize)
{
	char szBuffer256[256];
	szBuffer256[0] = 0;

	//проходимся по всем строкам таблицы
	for (int i = 0, il = SXLevelEditor::ListViewGameConnections->GetCountString(); i < il; ++i)
	{
		//если строку с этим номером надо исключить
		if (iNumExclude == i)
			continue;

		propdata_t *pPropData2 = (propdata_t*)SXLevelEditor::ListViewGameConnections->GetDataItem(i);

		//если юзердата то что нам надо
		if (pPropData == pPropData2)
		{
			if (strlen(szStrOut) > 0)
				sprintf(szStrOut + strlen(szStrOut), ",");

			szBuffer256[0] = 0;
			SXLevelEditor::ListViewGameConnections->GetTextItem(szBuffer256, 1, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s:", szBuffer256);

			//ищем энтить с которым будет соединение
			CBaseEntity *pEnt2 = SXGame_EntGetByName(szBuffer256);
			proptable_t *pPropTable2 = 0;

			if (pEnt2)
				pPropTable2 = SXGame_EntGetProptable(pEnt2->getClassName());

			szBuffer256[0] = 0;
			SXLevelEditor::ListViewGameConnections->GetTextItem(szBuffer256, 2, i, 256);

			//если энтить для соединения существует
			if (pEnt2)
			{
				//проходимся по всем полям таблицы данных
				for (int k = 0; k < pPropTable2->numFields; ++k)
				{
					propdata_t *pPropData3 = &pPropTable2->pData[k];

					//если поле класса то что нам надо
					if (pPropData3->flags & PDFF_INPUT && strcmp(pPropData3->szEdName, szBuffer256) == 0)
					{
						sprintf(szStrOut + strlen(szStrOut), "%s:", pPropData3->szKey);
						break;
					}
				}
			}
			//если энтитя для соединения нет
			else
			{
				sprintf(szStrOut + strlen(szStrOut), ":");
			}

			szBuffer256[0] = 0;
			SXLevelEditor::ListViewGameConnections->GetTextItem(szBuffer256, 3, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s:", szBuffer256);

			szBuffer256[0] = 0;
			SXLevelEditor::ListViewGameConnections->GetTextItem(szBuffer256, 4, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s", szBuffer256);
		}
	}
}

LRESULT SXLevelEditor_ButtonGameConnectionsCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = SXLevelEditor::ListBoxList->GetSel();
	ID idSelStrTable = SXLevelEditor::ListViewGameConnections->GetSelString();

	if (idSelListBoxList < 0)
		return 0;

	ID idSelEnt = SXLevelEditor::ListBoxList->GetItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());

	if (!pEnt)
		return 0;

	//если добавление нового соединения
	if (SXLevelEditor::isAddGameConections)
	{
		if (idSelStrTable < 0)
			return 0;

		propdata_t *pPropData = (propdata_t*)(SXLevelEditor::ListViewGameConnections->GetDataItem(idSelStrTable));

		SXLevelEditor::ButtonGameConnectionsDelete->Visible(true);
		SXLevelEditor::ButtonGameConnectionsCreate->SetText("Create");
		SXLevelEditor::isAddGameConections = false;

		char szBuffer4096[4096];
		szBuffer4096[0] = 0;

		SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, -1, szBuffer4096, 4096);

		if (strlen(szBuffer4096) > 0)
			sprintf(szBuffer4096 + strlen(szBuffer4096), ",");

		char szBuffer256[256];
		szBuffer256[0] = 0;

		SXLevelEditor::EditGameConnectionsName->GetText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", szBuffer256);

		szBuffer256[0] = 0;
		long lItemData = SXLevelEditor::ComboBoxGameConnectionsAction->GetItemData(SXLevelEditor::ComboBoxGameConnectionsAction->GetSel());
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", (char*)lItemData);

		szBuffer256[0] = 0;
		SXLevelEditor::EditGameConnectionsDelay->GetText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", szBuffer256);

		szBuffer256[0] = 0;
		SXLevelEditor::EditGameConnectionsParameter->GetText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s", szBuffer256);

		pEnt->setKV(pPropData->szKey, szBuffer4096);

		SXLevelEditor::GameSel(idSelListBoxList);
	}
	//иначе создание нового соединения
	else
	{
		SXLevelEditor::ButtonGameConnectionsDelete->Visible(false);
		SXLevelEditor::ButtonGameConnectionsCreate->SetText("Add");
		SXLevelEditor::isAddGameConections = true;
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGameConnectionsDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = SXLevelEditor::ListBoxList->GetSel();
	ID idSelStrTable = SXLevelEditor::ListViewGameConnections->GetSelString();

	if (idSelListBoxList < 0 || idSelStrTable < 0)
		return 0;

	ID idSelEnt = SXLevelEditor::ListBoxList->GetItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());
	propdata_t *pPropData = (propdata_t*)(SXLevelEditor::ListViewGameConnections->GetDataItem(idSelStrTable));

	if (!pEnt)
		return 0;

	char szBuffer4096[4096];
	szBuffer4096[0] = 0;

	SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, idSelStrTable, szBuffer4096, 4096);

	pEnt->setKV(pPropData->szKey, szBuffer4096);
	SXLevelEditor::GameSel(idSelListBoxList);

	return 0;
}

