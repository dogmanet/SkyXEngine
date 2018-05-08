
#include "game_callback.h"

void SXLevelEditor::GameActivateAll(bool bf)
{
	SXLevelEditor::StaticGameClass->setVisible(bf);
	SXLevelEditor::ComboBoxGameClass->setVisible(bf);
	SXLevelEditor::ComboBoxGameClass->setSel(0);
	SXLevelEditor::ButtonGameTab->setVisible(bf);
	SXLevelEditor::ButtonGameTab->setText("Connections");
	SXLevelEditor::ListViewGameClass->setVisible(bf);
	SXLevelEditor::ListViewGameClass->clearStrings();
	SXLevelEditor::ComboBoxGameValue->setVisible(false);
	SXLevelEditor::ComboBoxGameValue->clear();
	SXLevelEditor::EditGameValue->setVisible(false);
	SXLevelEditor::EditGameValue->setText("");
	SXLevelEditor::ButtonGameValue->setVisible(false);
	SXLevelEditor::ListViewGameClass->setEnable(false);

	SXLevelEditor::StaticGameHelp->setVisible(bf);
	SXLevelEditor::MemoGameHelp->setText("");
	SXLevelEditor::MemoGameHelp->setVisible(bf);
	SXLevelEditor::ButtonGameCreate->setVisible(bf);

	for (int i = 0; i < 16; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i]->setVisible(false);
	}

	if (!bf)
		SXLevelEditor::GameVisibleConnections(false);
}

void SXLevelEditor::GameSel(int sel)
{
	for (int i = 0; i < 16; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i]->setVisible(false);
	}

	SXLevelEditor::ComboBoxGameValue->setVisible(false);
	SXLevelEditor::ComboBoxGameValue->clear();
	SXLevelEditor::EditGameValue->setVisible(false);
	SXLevelEditor::EditGameValue->setText("");
	SXLevelEditor::ButtonGameCreate->setVisible(false);
	SXLevelEditor::ListViewGameClass->setEnable(true);
	SXLevelEditor::ListViewGameClass->clearStrings();
	ID seldata = SXLevelEditor::ListBoxList->getItemData(sel);
	SXLevelEditor::ActiveElement = seldata;
	SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;
	CBaseEntity* bEnt = SXGame_EntGet(seldata);
	proptable_t* pt = SXGame_EntGetProptable(bEnt->getClassName());

	char txtclasscb[256];
	const char* txtclassent = bEnt->getClassName();
	for (int i = 0; i < SXLevelEditor::ComboBoxGameClass->getCount(); ++i)
	{
		SXLevelEditor::ComboBoxGameClass->getItemText(i, txtclasscb);
		if (strcmp(txtclasscb, txtclassent) == 0)
			SXLevelEditor::ComboBoxGameClass->setSel(i);
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
				int str = SXLevelEditor::ListViewGameClass->addString((long)pd);
				SXLevelEditor::ListViewGameClass->setItemText(txtkey, 0, str);
				//propdata_t* pd2 = (propdata_t*)SXLevelEditor::ListViewGameClass->getItemData(str);
				SXLevelEditor::ListViewGameClass->setItemText(txtval, 1, str);
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
			int str = SXLevelEditor::ListViewGameClass->addString((long)pd);
			SXLevelEditor::ListViewGameClass->setItemText(txtkey, 0, str);
			//propdata_t* pd2 = (propdata_t*)SXLevelEditor::ListViewGameClass->getItemData(str);
			SXLevelEditor::ListViewGameClass->setItemText(txtval, 1, str);
		}

		if (pd->szEdName && pd->type == PDF_FLAG)
		{
			int index = log2(pd->flags) - 16;
			SXLevelEditor::aGameObjectFlags[index] = pd->szEdName;
		}
	}

	//заполнение таблицы и ComboBoxGameConnectionsEvent
	SXLevelEditor::ComboBoxGameConnectionsEvent->clear();
	SXLevelEditor::ComboBoxGameConnectionsAction->clear();
	SXLevelEditor::ListViewGameConnections->clearStrings();
	SXLevelEditor::EditGameConnectionsName->setText("");
	SXLevelEditor::EditGameConnectionsDelay->setText("");
	SXLevelEditor::EditGameConnectionsParameter->setText("");

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
					int iNumStr = SXLevelEditor::ListViewGameConnections->addString((long)pd);
					sprintf(txtkey, "%s", pd->szEdName);
					SXLevelEditor::ListViewGameConnections->setItemText(txtkey, 0, iNumStr);

					for (int j = 0; j < iConns2; ++j)
						SXLevelEditor::ListViewGameConnections->setItemText(parts2[j], j + 1, iNumStr);

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
								SXLevelEditor::ListViewGameConnections->setItemText(txtkey, 2, iNumStr);
							}
						}
					}
				}
			}

			SXLevelEditor::ComboBoxGameConnectionsEvent->addItem(pd->szEdName);
			SXLevelEditor::ComboBoxGameConnectionsEvent->setItemData(SXLevelEditor::ComboBoxGameConnectionsEvent->getCount()-1, (LPARAM)(pd->szKey));
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

	for (int i = 0; i < SXLevelEditor::ListViewGameClass->getStringCount(); ++i)
	{
		pd = (propdata_t*)SXLevelEditor::ListViewGameClass->getItemData(i);

		if (pd)
		{
			if (stricmp(pd->szKey, "origin") == 0 || stricmp(pd->szKey, "rotation") == 0)
			{
				bEnt->getKV(pd->szKey, txtval, 256);
				SXLevelEditor::ListViewGameClass->setItemText(txtval, 1, i);
			}
		}
	}
}

LRESULT SXLevelEditor_ListViewGameClass_Click()
{
	int str = SXLevelEditor::ListViewGameClass->getSelString();
	if (str < 0)
	{
		SXLevelEditor::EditGameValue->setVisible(false);
		SXLevelEditor::ButtonGameValue->setVisible(false);
		SXLevelEditor::ComboBoxGameValue->setVisible(false);
		return 0;
	}
	propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->getItemData(str);

	SXLevelEditor::EditGameValue->setVisible(false);
	SXLevelEditor::ButtonGameValue->setVisible(false);
	SXLevelEditor::ComboBoxGameValue->setVisible(false);

	char txtval[256];

	for (int i = 0; i < 16; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i]->setVisible(false);
	}

	if (pd->editor.type == PDE_TEXTFIELD)
	{
		SXLevelEditor::EditGameValue->setVisible(true);
		SXLevelEditor::ListViewGameClass->getItemText(txtval, 1, str, 256);
		SXLevelEditor::EditGameValue->setText(txtval);

		SXLevelEditor::StaticGameHelp->setVisible(true);
		SXLevelEditor::MemoGameHelp->setVisible(true);
	}
	else if (pd->editor.type == PDE_FILEFIELD)
	{
		SXLevelEditor::EditGameValue->setVisible(true);
		SXLevelEditor::ButtonGameValue->setVisible(true);
		SXLevelEditor::ListViewGameClass->getItemText(txtval, 1, str, 256);
		SXLevelEditor::EditGameValue->setText(txtval);

		SXLevelEditor::StaticGameHelp->setVisible(true);
		SXLevelEditor::MemoGameHelp->setVisible(true);
	}
	else if (pd->editor.type == PDE_COMBOBOX)
	{
		SXLevelEditor::StaticGameHelp->setVisible(true);
		SXLevelEditor::MemoGameHelp->setVisible(true);

		SXLevelEditor::ComboBoxGameValue->clear();
		SXLevelEditor::ComboBoxGameValue->setVisible(true);
		editor_kv* ekv = (editor_kv*)pd->editor.pData;
		int elem = 0;
		while (ekv)
		{
			if (ekv->key && ekv->value)
			{
				SXLevelEditor::ComboBoxGameValue->addItem(ekv->key);
				SXLevelEditor::ComboBoxGameValue->setItemData(elem, (LPARAM)ekv->value);
				++elem;
				ekv++;
			}
			else
				break;
		}

		static char tval[256];
		tval[0] = 0;
		SXLevelEditor::ListViewGameClass->getItemText(tval, 1, str, 256);

		const char* tval2;
		char tval3[256];
		bool isfound = false;

		//проверяем в цикле какая строка из комбобокса выбрана
		for (int i = 0; i < SXLevelEditor::ComboBoxGameValue->getCount(); ++i)
		{
			//сначала проверяем по юзердате (там записано значение)
			tval2 = (const char*)SXLevelEditor::ComboBoxGameValue->getItemData(i);

			if (strcmp(tval, tval2) == 0)
			{
				SXLevelEditor::ComboBoxGameValue->setSel(i);
				isfound = true;
				break;
			}

			//затем проверям по имени строки
			SXLevelEditor::ComboBoxGameValue->getItemText(i, tval3);
			
			if (strcmp(tval, tval3) == 0)
			{
				SXLevelEditor::ComboBoxGameValue->setSel(i);
				isfound = true;
				break;
			}
		}

		//если не найдено значение значит надо его добавить
		if (!isfound)
		{
			MessageBox(0,"SXLevelEditor_ListViewGameClass_Click data not found",0,0);
			SXLevelEditor::ComboBoxGameValue->addItem(tval);
			SXLevelEditor::ComboBoxGameValue->setItemData(SXLevelEditor::ComboBoxGameValue->getCount() - 1, (LPARAM)tval);
			SXLevelEditor::ComboBoxGameValue->setSel(SXLevelEditor::ComboBoxGameValue->getCount()-1);
		}
	}
	else if (pd->editor.type == PDE_FLAGS)
	{
		SXLevelEditor::StaticGameHelp->setVisible(false);
		SXLevelEditor::MemoGameHelp->setVisible(false);
		char txtval[256];
		SXLevelEditor::ListViewGameClass->getItemText(txtval, 1, str, 256);
		UINT uiFlags;
		sscanf(txtval, "%d", &uiFlags);
		for (int i = 0; i < 16; ++i)
		{
			if (SXLevelEditor::aGameObjectFlags[i])
			{
				SXLevelEditor::CheckBoxGameFlags[i]->setText(SXLevelEditor::aGameObjectFlags[i]);
				SXLevelEditor::CheckBoxGameFlags[i]->setVisible(true);
				if (uiFlags & ((1 << i + 16)))
					SXLevelEditor::CheckBoxGameFlags[i]->setCheck(true);
				else
					SXLevelEditor::CheckBoxGameFlags[i]->setCheck(false);
			}
		}
	}
	return 0;
}

LRESULT SXLevelEditor_EditGameValue_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->getSel();
	
	if (sel < 0)
		return 0;

	int str = SXLevelEditor::ListViewGameClass->getSelString();
	
	if (str < 0)
		return 0;

	char txt[256];
	SXLevelEditor::EditGameValue->getText(txt, 256);
	SXLevelEditor::ListViewGameClass->setItemText(txt, 1, str);
	CBaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->getItemData(sel));
	if (bEnt)
	{
		propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->getItemData(str);
		bool bf = bEnt->setKV(pd->szKey, txt);
		int qwerty = 0;

		if (strcmp(pd->szKey, "name") == 0)
		{
			char tmpname[512];
			sprintf(tmpname, "%s / %s", bEnt->getName(), bEnt->getClassName());
			SXLevelEditor::ListBoxList->setItemText(sel, tmpname);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGameValue_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int str = SXLevelEditor::ListViewGameClass->getSelString();
	propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->getItemData(str);

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
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), txtfmt);
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS), SXLevelEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String tmpstr = tmppath + strlen(Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE));
		tmpstr.replaceAll("\\", "/");
		sprintf(tmpname, "%s", tmpstr.c_str());
		SXLevelEditor::EditGameValue->setText(tmpname);

		int sel = SXLevelEditor::ListBoxList->getSel();
		SXLevelEditor::ListViewGameClass->setItemText(tmpname, 1, str);
		CBaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->getItemData(sel));
		if (bEnt)
		{
			bEnt->setKV(pd->szKey, tmpname);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGameCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::ComboBoxGameClass->getSel() == 0)
	{
		MessageBox(0, "Class of game object is not selected!", 0, 0);
		return 0;
	}
	char txt[256];
	SXLevelEditor::ComboBoxGameClass->getItemText(SXLevelEditor::ComboBoxGameClass->getSel(), txt);

	CBaseEntity* bEnt = SXGame_CreateEntity(txt);
	bEnt->setFlags(bEnt->getFlags() | EF_EXPORT | EF_LEVEL);

	SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);
	int sel = SXLevelEditor::ListBoxList->getItemCount() - 1;
	SXLevelEditor::ListBoxList->setSel(sel);
	SXLevelEditor::GameSel(sel);
	SXLevelEditor::ActiveElement = sel;
	SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;

	return 0;
}

LRESULT SXLevelEditor_EditGameConnectionsName_IN(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char edit_text[256];
	SXLevelEditor::EditGameConnectionsName->getText(edit_text, 256);
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
					SXLevelEditor::EditGameConnectionsName->setText(lower_name);
					PostMessage(SXLevelEditor::EditGameConnectionsName->getHWND(), EM_SETSEL, strlen(edit_text), strlen(lower_name));
					return 0;
				}
			}
		}
	}

	return 0;
}

void SXLevelEditor::GameVisibleProperties(bool bf)
{
	SXLevelEditor::StaticGameClass->setVisible(bf);
	SXLevelEditor::ComboBoxGameClass->setVisible(bf);
	SXLevelEditor::ListViewGameClass->setVisible(bf);
	//SXLevelEditor::ComboBoxGameValue->setVisible(bf);
	//SXLevelEditor::EditGameValue->setVisible(bf);
	//SXLevelEditor::ButtonGameValue->setVisible(bf);
	SXLevelEditor::StaticGameHelp->setVisible(bf);
	SXLevelEditor::MemoGameHelp->setVisible(bf);
	SXLevelEditor::ButtonGameCreate->setVisible(bf);

	for (int i = 0; i < 16; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i]->setVisible(false);
	}
}

void SXLevelEditor::GameVisibleConnections(bool bf)
{
	SXLevelEditor::ListViewGameConnections->setVisible(bf);
	SXLevelEditor::StaticGameConnectionsEvent->setVisible(bf);
	SXLevelEditor::ComboBoxGameConnectionsEvent->setVisible(bf);
	SXLevelEditor::StaticGameConnectionsName->setVisible(bf);
	SXLevelEditor::EditGameConnectionsName->setVisible(bf);
	SXLevelEditor::StaticGameConnectionsAction->setVisible(bf);
	SXLevelEditor::ComboBoxGameConnectionsAction->setVisible(bf);
	SXLevelEditor::StaticGameConnectionsDelay->setVisible(bf);
	SXLevelEditor::EditGameConnectionsDelay->setVisible(bf);
	SXLevelEditor::StaticGameConnectionsParameter->setVisible(bf);
	SXLevelEditor::EditGameConnectionsParameter->setVisible(bf);
	SXLevelEditor::ButtonGameConnectionsCreate->setVisible(bf);
	SXLevelEditor::ButtonGameConnectionsDelete->setVisible(false);
}

LRESULT SXLevelEditor_ButtonGameTab_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::GameTabVal == 1)
	{
		SXLevelEditor::ButtonGameTab->setText("Connections");
		SXLevelEditor::GameVisibleProperties(true);
		SXLevelEditor::GameVisibleConnections(false);
		SXLevelEditor::GameTabVal = 0;
	}
	else
	{
		SXLevelEditor::ButtonGameTab->setText("Properties");
		SXLevelEditor::GameVisibleProperties(false);
		SXLevelEditor::GameVisibleConnections(true);
		SXLevelEditor::GameTabVal = 1;
	}

	return 0;
}

//##########################################################################

LRESULT SXLevelEditor_ListViewGameConnections_Click()
{
	int iNumStr = SXLevelEditor::ListViewGameConnections->getSelString();

	//очищаем комбобоксы
	//SXLevelEditor::ComboBoxGameConnectionsEvent->clear();
	SXLevelEditor::ComboBoxGameConnectionsAction->clear();

	//если нет выделенного тогда обнуляем
	if (iNumStr < 0)
	{
		SXLevelEditor::ComboBoxGameConnectionsEvent->setSel(0);
		SXLevelEditor::EditGameConnectionsName->setText("");
		SXLevelEditor::ComboBoxGameConnectionsAction->setSel(0);
		SXLevelEditor::EditGameConnectionsDelay->setText("");
		SXLevelEditor::EditGameConnectionsParameter->setText("");
		return 0;
	}

	char szStr[256];
	char szStr2[256];

	//выделяем используемый event в ComboBoxGameConnectionsEvent
	SXLevelEditor::ListViewGameConnections->getItemText(szStr, 0, iNumStr, 256);

	for (int i = 0; i < SXLevelEditor::ComboBoxGameConnectionsEvent->getCount(); ++i)
	{
		SXLevelEditor::ComboBoxGameConnectionsEvent->getItemText(i, szStr2);
		if (strcmp(szStr, szStr2) == 0)
		{
			SXLevelEditor::ComboBoxGameConnectionsEvent->setSel(i);
			break;
		}
	}
	//-------

	//устанавилваем имя используемого игрового объекта в EditGameConnectionsName
	SXLevelEditor::ListViewGameConnections->getItemText(szStr, 1, iNumStr, 256);
	SXLevelEditor::EditGameConnectionsName->setText(szStr);
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
					SXLevelEditor::ComboBoxGameConnectionsAction->addItem(pPropData->szEdName);
					SXLevelEditor::ComboBoxGameConnectionsAction->setItemData(SXLevelEditor::ComboBoxGameConnectionsAction->getCount() - 1, (LPARAM)(pPropData->szKey));
				}
			}
		}
	}

	SXLevelEditor::ListViewGameConnections->getItemText(szStr, 2, iNumStr, 256);
	for (int i = 0; i < SXLevelEditor::ComboBoxGameConnectionsAction->getCount(); ++i)
	{
		SXLevelEditor::ComboBoxGameConnectionsAction->getItemText(i, szStr2);
		if (strcmp(szStr, szStr2) == 0)
		{
			SXLevelEditor::ComboBoxGameConnectionsAction->setSel(i);
			break;
		}
	}
	//-------

	//устанавливаем задержку в секундах в EditGameConnectionsDelay
	SXLevelEditor::ListViewGameConnections->getItemText(szStr, 3, iNumStr, 256);
	SXLevelEditor::EditGameConnectionsDelay->setText(szStr);
	//-------

	//устанавливаем параметр в EditGameConnectionsParameter
	SXLevelEditor::ListViewGameConnections->getItemText(szStr, 4, iNumStr, 256);
	SXLevelEditor::EditGameConnectionsParameter->setText(szStr);
	//-------

	SXLevelEditor::ButtonGameConnectionsDelete->setVisible(true);
	SXLevelEditor::ButtonGameConnectionsCreate->setText("Create");
	SXLevelEditor::isAddGameConections = false;

	return 0;
}

LRESULT SXLevelEditor_EditGameConnectionsName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = SXLevelEditor::ListBoxList->getSel();
	ID idSelStrTable = SXLevelEditor::ListViewGameConnections->getSelString();

	if (idSelListBoxList < 0)
		return 0;

	CBaseEntity *pEnt = SXGame_EntGet(idSelListBoxList);

	char szBuffer256[256];
	szBuffer256[0] = 0;

	SXLevelEditor::EditGameConnectionsName->getText(szBuffer256, 256);

	CBaseEntity *pEnt2 = SXGame_EntGetByName(szBuffer256, 0);

	//записываем в szBuffer256 текущее имя action
	szBuffer256[0] = 0;
	SXLevelEditor::ComboBoxGameConnectionsAction->getItemText(SXLevelEditor::ComboBoxGameConnectionsAction->getSel(), szBuffer256);

	// если энтить с введенным именем существует
	if (pEnt2)
	{
		proptable_t *pPropTable2 = SXGame_EntGetProptable(pEnt2->getClassName());
		propdata_t *pPropData2 = 0;
		SXLevelEditor::ComboBoxGameConnectionsAction->clear();

		//проходимся по всем данным таблицы свойств
		for (int j = 0; j < pPropTable2->numFields; ++j)
		{
			pPropData2 = &pPropTable2->pData[j];

			//если свойство input
			if (pPropData2->flags & PDFF_INPUT)
			{
				//добавляем в комбобокс action
				SXLevelEditor::ComboBoxGameConnectionsAction->addItem(pPropData2->szEdName);
				SXLevelEditor::ComboBoxGameConnectionsAction->setItemData(SXLevelEditor::ComboBoxGameConnectionsEvent->getCount() - 1, (LPARAM)(pPropData2->szKey));

				//если предыдущее имя action равно последнему вставленному
				if (strcmp(pPropData2->szEdName, szBuffer256) == 0)
				{
					//активируем эту строку в комбобоксе
					SXLevelEditor::ComboBoxGameConnectionsAction->setSel(SXLevelEditor::ComboBoxGameConnectionsAction->getCount() - 1);
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
		SXLevelEditor::ListViewGameConnections->getItemText(szBuffer256, 1, idSelStrTable, 256);
		SXLevelEditor::EditGameConnectionsName->setText(szBuffer256);
	}

	return SXLevelEditor_EditGameConnections_Enter(hwnd, msg, wParam, lParam);
}

LRESULT SXLevelEditor_EditGameConnections_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::isAddGameConections)
		return 0;

	ID idSelListBoxList = SXLevelEditor::ListBoxList->getSel();
	ID idSelStrTable = SXLevelEditor::ListViewGameConnections->getSelString();

	if (idSelListBoxList < 0 || idSelStrTable < 0)
		return 0;

	ID idSelEnt = SXLevelEditor::ListBoxList->getItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());
	propdata_t *pPropData = (propdata_t*)(SXLevelEditor::ListViewGameConnections->getItemData(idSelStrTable));

	//получаем скленные в единую строку данные
	char szBuffer4096[4096];
	szBuffer4096[0] = 0;
	SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, -1, szBuffer4096, 4096);


	//устанавливаем в выделенную в таблице строку данные из редакторов
	char szBuffer256[256];
	szBuffer256[0] = 0;

	SXLevelEditor::ComboBoxGameConnectionsEvent->getItemText(SXLevelEditor::ComboBoxGameConnectionsEvent->getSel(), szBuffer256);
	SXLevelEditor::ListViewGameConnections->setItemText(szBuffer256, 0, idSelStrTable);
	
	szBuffer256[0] = 0;
	SXLevelEditor::EditGameConnectionsName->getText(szBuffer256, 256);
	SXLevelEditor::ListViewGameConnections->setItemText(szBuffer256, 1, idSelStrTable);

	szBuffer256[0] = 0;
	SXLevelEditor::ComboBoxGameConnectionsAction->getItemText(SXLevelEditor::ComboBoxGameConnectionsAction->getSel(), szBuffer256);
	SXLevelEditor::ListViewGameConnections->setItemText(szBuffer256, 2, idSelStrTable);

	szBuffer256[0] = 0;
	SXLevelEditor::EditGameConnectionsDelay->getText(szBuffer256, 256);
	SXLevelEditor::ListViewGameConnections->setItemText(szBuffer256, 3, idSelStrTable);

	szBuffer256[0] = 0;
	SXLevelEditor::EditGameConnectionsParameter->getText(szBuffer256, 256);
	SXLevelEditor::ListViewGameConnections->setItemText(szBuffer256, 4, idSelStrTable);

	pEnt->setKV(pPropData->szKey, szBuffer4096);

	return 0;
}

void SXLevelEditor_VeldStringGameConnections(proptable_t *pPropTable, propdata_t *pPropData, int iNumExclude, char* szStrOut, int iSize)
{
	char szBuffer256[256];
	szBuffer256[0] = 0;

	//проходимся по всем строкам таблицы
	for (int i = 0, il = SXLevelEditor::ListViewGameConnections->getStringCount(); i < il; ++i)
	{
		//если строку с этим номером надо исключить
		if (iNumExclude == i)
			continue;

		propdata_t *pPropData2 = (propdata_t*)SXLevelEditor::ListViewGameConnections->getItemData(i);

		//если юзердата то что нам надо
		if (pPropData == pPropData2)
		{
			if (strlen(szStrOut) > 0)
				sprintf(szStrOut + strlen(szStrOut), ",");

			szBuffer256[0] = 0;
			SXLevelEditor::ListViewGameConnections->getItemText(szBuffer256, 1, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s:", szBuffer256);

			//ищем энтить с которым будет соединение
			CBaseEntity *pEnt2 = SXGame_EntGetByName(szBuffer256);
			proptable_t *pPropTable2 = 0;

			if (pEnt2)
				pPropTable2 = SXGame_EntGetProptable(pEnt2->getClassName());

			szBuffer256[0] = 0;
			SXLevelEditor::ListViewGameConnections->getItemText(szBuffer256, 2, i, 256);

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
			SXLevelEditor::ListViewGameConnections->getItemText(szBuffer256, 3, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s:", szBuffer256);

			szBuffer256[0] = 0;
			SXLevelEditor::ListViewGameConnections->getItemText(szBuffer256, 4, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s", szBuffer256);
		}
	}
}

LRESULT SXLevelEditor_ButtonGameConnectionsCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = SXLevelEditor::ListBoxList->getSel();
	ID idSelStrTable = SXLevelEditor::ListViewGameConnections->getSelString();

	if (idSelListBoxList < 0)
		return 0;

	ID idSelEnt = SXLevelEditor::ListBoxList->getItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());

	if (!pEnt)
		return 0;

	//если добавление нового соединения
	if (SXLevelEditor::isAddGameConections)
	{
		if (idSelStrTable < 0)
			return 0;

		propdata_t *pPropData = (propdata_t*)(SXLevelEditor::ListViewGameConnections->getItemData(idSelStrTable));

		SXLevelEditor::ButtonGameConnectionsDelete->setVisible(true);
		SXLevelEditor::ButtonGameConnectionsCreate->setText("Create");
		SXLevelEditor::isAddGameConections = false;

		char szBuffer4096[4096];
		szBuffer4096[0] = 0;

		SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, -1, szBuffer4096, 4096);

		if (strlen(szBuffer4096) > 0)
			sprintf(szBuffer4096 + strlen(szBuffer4096), ",");

		char szBuffer256[256];
		szBuffer256[0] = 0;

		SXLevelEditor::EditGameConnectionsName->getText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", szBuffer256);

		szBuffer256[0] = 0;
		long lItemData = SXLevelEditor::ComboBoxGameConnectionsAction->getItemData(SXLevelEditor::ComboBoxGameConnectionsAction->getSel());
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", (char*)lItemData);

		szBuffer256[0] = 0;
		SXLevelEditor::EditGameConnectionsDelay->getText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", szBuffer256);

		szBuffer256[0] = 0;
		SXLevelEditor::EditGameConnectionsParameter->getText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s", szBuffer256);

		pEnt->setKV(pPropData->szKey, szBuffer4096);

		SXLevelEditor::GameSel(idSelListBoxList);
	}
	//иначе создание нового соединения
	else
	{
		SXLevelEditor::ButtonGameConnectionsDelete->setVisible(false);
		SXLevelEditor::ButtonGameConnectionsCreate->setText("Add");
		SXLevelEditor::isAddGameConections = true;
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGameConnectionsDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = SXLevelEditor::ListBoxList->getSel();
	ID idSelStrTable = SXLevelEditor::ListViewGameConnections->getSelString();

	if (idSelListBoxList < 0 || idSelStrTable < 0)
		return 0;

	ID idSelEnt = SXLevelEditor::ListBoxList->getItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());
	propdata_t *pPropData = (propdata_t*)(SXLevelEditor::ListViewGameConnections->getItemData(idSelStrTable));

	if (!pEnt)
		return 0;

	char szBuffer4096[4096];
	szBuffer4096[0] = 0;

	SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, idSelStrTable, szBuffer4096, 4096);

	pEnt->setKV(pPropData->szKey, szBuffer4096);
	SXLevelEditor::GameSel(idSelListBoxList);

	return 0;
}

