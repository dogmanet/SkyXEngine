
#include "game_callback.h"

void level_editor::GameActivateAll(bool bf)
{
	level_editor::pStaticGameClass->setVisible(bf);
	level_editor::pComboBoxGameClass->setVisible(bf);
	level_editor::pComboBoxGameClass->setSel(0);
	level_editor::pButtonGameTab->setVisible(bf);
	level_editor::pButtonGameTab->setText("Connections");
	level_editor::pListViewGameClass->setVisible(bf);
	level_editor::pListViewGameClass->clearStrings();
	level_editor::pComboBoxGameValue->setVisible(false);
	level_editor::pComboBoxGameValue->clear();
	level_editor::pEditGameValue->setVisible(false);
	level_editor::pEditGameValue->setText("");
	level_editor::pButtonGameValue->setVisible(false);
	level_editor::pListViewGameClass->setEnable(false);

	level_editor::pStaticGameHelp->setVisible(bf);
	level_editor::pMemoGameHelp->setText("");
	level_editor::pMemoGameHelp->setVisible(bf);
	level_editor::pButtonGameCreate->setVisible(bf);

	for (int i = 0; i < 16; ++i)
	{
		level_editor::pCheckBoxGameFlags[i]->setVisible(false);
	}

	if (!bf)
		level_editor::GameVisibleConnections(false);
}

void level_editor::GameSel(int sel)
{
	for (int i = 0; i < 16; ++i)
	{
		level_editor::pCheckBoxGameFlags[i]->setVisible(false);
	}

	level_editor::pComboBoxGameValue->setVisible(false);
	level_editor::pComboBoxGameValue->clear();
	level_editor::pEditGameValue->setVisible(false);
	level_editor::pEditGameValue->setText("");
	level_editor::pButtonGameCreate->setVisible(false);
	level_editor::pListViewGameClass->setEnable(true);
	level_editor::pListViewGameClass->clearStrings();
	ID seldata = level_editor::pListBoxList->getItemData(sel);
	level_editor::idActiveElement = seldata;
	level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;
	CBaseEntity* bEnt = SXGame_EntGet(seldata);
	proptable_t* pt = SXGame_EntGetProptable(bEnt->getClassName());

	char txtclasscb[256];
	const char* txtclassent = bEnt->getClassName();
	for (int i = 0; i < level_editor::pComboBoxGameClass->getCount(); ++i)
	{
		level_editor::pComboBoxGameClass->getItemText(i, txtclasscb);
		if (strcmp(txtclasscb, txtclassent) == 0)
			level_editor::pComboBoxGameClass->setSel(i);
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

	
	memset(level_editor::aGameObjectFlags, 0, sizeof(const char*)* 16);

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
				int str = level_editor::pListViewGameClass->addString((long)pd);
				level_editor::pListViewGameClass->setItemText(txtkey, 0, str);
				//propdata_t* pd2 = (propdata_t*)level_editor::pListViewGameClass->getItemData(str);
				level_editor::pListViewGameClass->setItemText(txtval, 1, str);
			}

			if (pd->szEdName && pd->type == PDF_FLAG)
			{
				int index = log2(pd->flags) - 16;
				level_editor::aGameObjectFlags[index] = pd->szEdName;
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
			int str = level_editor::pListViewGameClass->addString((long)pd);
			level_editor::pListViewGameClass->setItemText(txtkey, 0, str);
			//propdata_t* pd2 = (propdata_t*)level_editor::pListViewGameClass->getItemData(str);
			level_editor::pListViewGameClass->setItemText(txtval, 1, str);
		}

		if (pd->szEdName && pd->type == PDF_FLAG)
		{
			int index = log2(pd->flags) - 16;
			level_editor::aGameObjectFlags[index] = pd->szEdName;
		}
	}

	//заполнение таблицы и ComboBoxGameConnectionsEvent
	level_editor::pComboBoxGameConnectionsEvent->clear();
	level_editor::pComboBoxGameConnectionsAction->clear();
	level_editor::pListViewGameConnections->clearStrings();
	level_editor::pEditGameConnectionsName->setText("");
	level_editor::pEditGameConnectionsDelay->setText("");
	level_editor::pEditGameConnectionsParameter->setText("");

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
					int iNumStr = level_editor::pListViewGameConnections->addString((long)pd);
					sprintf(txtkey, "%s", pd->szEdName);
					level_editor::pListViewGameConnections->setItemText(txtkey, 0, iNumStr);

					for (int j = 0; j < iConns2; ++j)
						level_editor::pListViewGameConnections->setItemText(parts2[j], j + 1, iNumStr);

					CBaseEntity *pEnt2 = SXGame_EntGetByName(parts2[0], 0);
					if (pEnt2)
					{
						proptable_t *pPropTable2 = SXGame_EntGetProptable(pEnt2->getClassName());
						propdata_t *pPropData2 = 0;
						while(pPropTable2)
						{
							for(int j = 0; j < pPropTable2->numFields; ++j)
							{
								pPropData2 = &pPropTable2->pData[j];
								if(pPropData2->flags & PDFF_INPUT && strcmp(pPropData2->szKey, parts2[1]) == 0)
								{
									sprintf(txtkey, "%s", pPropData2->szEdName);
									level_editor::pListViewGameConnections->setItemText(txtkey, 2, iNumStr);
								}
							}
							pPropTable2 = pPropTable2->pBaseProptable;
						}
					}
				}
			}

			level_editor::pComboBoxGameConnectionsEvent->addItem(pd->szEdName);
			level_editor::pComboBoxGameConnectionsEvent->setItemData(level_editor::pComboBoxGameConnectionsEvent->getCount()-1, (LPARAM)(pd->szKey));
		}
	}
	//-------
	
	level_editor::pAxesHelper->setPosition(bEnt->getPos());
	level_editor::pAxesHelper->setRotation(bEnt->getOrient());
	level_editor::pAxesHelper->setScale(float3(1, 1, 1));
}

void level_editor::GameUpdatePosRot()
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GAME || level_editor::idActiveElement < 0)
		return;

	CBaseEntity* bEnt = SXGame_EntGet(level_editor::idActiveElement);
	propdata_t* pd = 0;
	char txtval[256];

	for (int i = 0; i < level_editor::pListViewGameClass->getStringCount(); ++i)
	{
		pd = (propdata_t*)level_editor::pListViewGameClass->getItemData(i);

		if (pd)
		{
			if (stricmp(pd->szKey, "origin") == 0 || stricmp(pd->szKey, "rotation") == 0)
			{
				bEnt->getKV(pd->szKey, txtval, 256);
				level_editor::pListViewGameClass->setItemText(txtval, 1, i);
			}
		}
	}
}

LRESULT SXLevelEditor_ListViewGameClass_Click()
{
	int str = level_editor::pListViewGameClass->getSelString();
	if (str < 0)
	{
		level_editor::pEditGameValue->setVisible(false);
		level_editor::pButtonGameValue->setVisible(false);
		level_editor::pComboBoxGameValue->setVisible(false);
		return 0;
	}
	propdata_t* pd = (propdata_t*)level_editor::pListViewGameClass->getItemData(str);

	level_editor::pEditGameValue->setVisible(false);
	level_editor::pButtonGameValue->setVisible(false);
	level_editor::pComboBoxGameValue->setVisible(false);

	char txtval[256];

	for (int i = 0; i < 16; ++i)
	{
		level_editor::pCheckBoxGameFlags[i]->setVisible(false);
	}

	if (pd->editor.type == PDE_TEXTFIELD)
	{
		level_editor::pEditGameValue->setVisible(true);
		level_editor::pListViewGameClass->getItemText(txtval, 1, str, 256);
		level_editor::pEditGameValue->setText(txtval);

		level_editor::pStaticGameHelp->setVisible(true);
		level_editor::pMemoGameHelp->setVisible(true);
	}
	else if (pd->editor.type == PDE_FILEFIELD)
	{
		level_editor::pEditGameValue->setVisible(true);
		level_editor::pButtonGameValue->setVisible(true);
		level_editor::pListViewGameClass->getItemText(txtval, 1, str, 256);
		level_editor::pEditGameValue->setText(txtval);

		level_editor::pStaticGameHelp->setVisible(true);
		level_editor::pMemoGameHelp->setVisible(true);
	}
	else if (pd->editor.type == PDE_COMBOBOX)
	{
		level_editor::pStaticGameHelp->setVisible(true);
		level_editor::pMemoGameHelp->setVisible(true);

		level_editor::pComboBoxGameValue->clear();
		level_editor::pComboBoxGameValue->setVisible(true);
		editor_kv* ekv = (editor_kv*)pd->editor.pData;
		int elem = 0;
		while (ekv)
		{
			if (ekv->key && ekv->value)
			{
				level_editor::pComboBoxGameValue->addItem(ekv->key);
				level_editor::pComboBoxGameValue->setItemData(elem, (LPARAM)ekv->value);
				++elem;
				ekv++;
			}
			else
				break;
		}

		static char tval[256];
		tval[0] = 0;
		level_editor::pListViewGameClass->getItemText(tval, 1, str, 256);

		const char* tval2;
		char tval3[256];
		bool isfound = false;

		//проверяем в цикле какая строка из комбобокса выбрана
		for (int i = 0; i < level_editor::pComboBoxGameValue->getCount(); ++i)
		{
			//сначала проверяем по юзердате (там записано значение)
			tval2 = (const char*)level_editor::pComboBoxGameValue->getItemData(i);

			if (strcmp(tval, tval2) == 0)
			{
				level_editor::pComboBoxGameValue->setSel(i);
				isfound = true;
				break;
			}

			//затем проверям по имени строки
			level_editor::pComboBoxGameValue->getItemText(i, tval3);
			
			if (strcmp(tval, tval3) == 0)
			{
				level_editor::pComboBoxGameValue->setSel(i);
				isfound = true;
				break;
			}
		}

		//если не найдено значение значит надо его добавить
		if (!isfound)
		{
			MessageBox(0,"SXLevelEditor_ListViewGameClass_Click data not found",0,0);
			level_editor::pComboBoxGameValue->addItem(tval);
			level_editor::pComboBoxGameValue->setItemData(level_editor::pComboBoxGameValue->getCount() - 1, (LPARAM)tval);
			level_editor::pComboBoxGameValue->setSel(level_editor::pComboBoxGameValue->getCount()-1);
		}
	}
	else if (pd->editor.type == PDE_FLAGS)
	{
		level_editor::pStaticGameHelp->setVisible(false);
		level_editor::pMemoGameHelp->setVisible(false);
		char txtval[256];
		level_editor::pListViewGameClass->getItemText(txtval, 1, str, 256);
		UINT uiFlags;
		sscanf(txtval, "%d", &uiFlags);
		for (int i = 0; i < 16; ++i)
		{
			if (level_editor::aGameObjectFlags[i])
			{
				level_editor::pCheckBoxGameFlags[i]->setText(level_editor::aGameObjectFlags[i]);
				level_editor::pCheckBoxGameFlags[i]->setVisible(true);
				if (uiFlags & ((1 << i + 16)))
					level_editor::pCheckBoxGameFlags[i]->setCheck(true);
				else
					level_editor::pCheckBoxGameFlags[i]->setCheck(false);
			}
		}
	}
	return 0;
}

LRESULT SXLevelEditor_EditGameValue_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = level_editor::pListBoxList->getSel();
	
	if (sel < 0)
		return 0;

	int str = level_editor::pListViewGameClass->getSelString();
	
	if (str < 0)
		return 0;

	char txt[256];
	level_editor::pEditGameValue->getText(txt, 256);
	level_editor::pListViewGameClass->setItemText(txt, 1, str);
	CBaseEntity* bEnt = SXGame_EntGet(level_editor::pListBoxList->getItemData(sel));
	if (bEnt)
	{
		propdata_t* pd = (propdata_t*)level_editor::pListViewGameClass->getItemData(str);
		bool bf = bEnt->setKV(pd->szKey, txt);
		int qwerty = 0;

		if (strcmp(pd->szKey, "name") == 0)
		{
			char tmpname[512];
			sprintf(tmpname, "%s / %s", bEnt->getName(), bEnt->getClassName());
			level_editor::pListBoxList->setItemText(sel, tmpname);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGameValue_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int str = level_editor::pListViewGameClass->getSelString();
	propdata_t* pd = (propdata_t*)level_editor::pListViewGameClass->getItemData(str);

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
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS));
		/*String tmpstr = tmppath + strlen(Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE));
		tmpstr.replaceAll("\\", "/");
		sprintf(tmpname, "%s", tmpstr.c_str());*/
		level_editor::pEditGameValue->setText(sRpath.c_str());

		int sel = level_editor::pListBoxList->getSel();
		level_editor::pListViewGameClass->setItemText((char*)sRpath.c_str(), 1, str);
		CBaseEntity* bEnt = SXGame_EntGet(level_editor::pListBoxList->getItemData(sel));
		if (bEnt)
		{
			bEnt->setKV(pd->szKey, sRpath.c_str());
		}
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGameCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::pComboBoxGameClass->getSel() == 0)
	{
		MessageBox(0, "Class of game object is not selected!", 0, 0);
		return 0;
	}
	char txt[256];
	level_editor::pComboBoxGameClass->getItemText(level_editor::pComboBoxGameClass->getSel(), txt);

	CBaseEntity* bEnt = SXGame_CreateEntity(txt);
	bEnt->setFlags(bEnt->getFlags() | EF_EXPORT | EF_LEVEL);

	SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);
	int sel = level_editor::pListBoxList->getItemCount() - 1;
	level_editor::pListBoxList->setSel(sel);
	level_editor::GameSel(sel);
	level_editor::idActiveElement = sel;
	level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;

	return 0;
}

LRESULT SXLevelEditor_EditGameConnectionsName_IN(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char edit_text[256];
	level_editor::pEditGameConnectionsName->getText(edit_text, 256);
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
					level_editor::pEditGameConnectionsName->setText(lower_name);
					PostMessage(level_editor::pEditGameConnectionsName->getHWND(), EM_SETSEL, strlen(edit_text), strlen(lower_name));
					return 0;
				}
			}
		}
	}

	return 0;
}

void level_editor::GameVisibleProperties(bool bf)
{
	level_editor::pStaticGameClass->setVisible(bf);
	level_editor::pComboBoxGameClass->setVisible(bf);
	level_editor::pListViewGameClass->setVisible(bf);
	//level_editor::pComboBoxGameValue->setVisible(bf);
	//level_editor::pEditGameValue->setVisible(bf);
	//level_editor::pButtonGameValue->setVisible(bf);
	level_editor::pStaticGameHelp->setVisible(bf);
	level_editor::pMemoGameHelp->setVisible(bf);
	level_editor::pButtonGameCreate->setVisible(bf);

	for (int i = 0; i < 16; ++i)
	{
		level_editor::pCheckBoxGameFlags[i]->setVisible(false);
	}
}

void level_editor::GameVisibleConnections(bool bf)
{
	level_editor::pListViewGameConnections->setVisible(bf);
	level_editor::pStaticGameConnectionsEvent->setVisible(bf);
	level_editor::pComboBoxGameConnectionsEvent->setVisible(bf);
	level_editor::pStaticGameConnectionsName->setVisible(bf);
	level_editor::pEditGameConnectionsName->setVisible(bf);
	level_editor::pStaticGameConnectionsAction->setVisible(bf);
	level_editor::pComboBoxGameConnectionsAction->setVisible(bf);
	level_editor::pStaticGameConnectionsDelay->setVisible(bf);
	level_editor::pEditGameConnectionsDelay->setVisible(bf);
	level_editor::pStaticGameConnectionsParameter->setVisible(bf);
	level_editor::pEditGameConnectionsParameter->setVisible(bf);
	level_editor::pButtonGameConnectionsCreate->setVisible(bf);
	level_editor::pButtonGameConnectionsDelete->setVisible(false);
}

LRESULT SXLevelEditor_ButtonGameTab_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::GameTabVal == 1)
	{
		level_editor::pButtonGameTab->setText("Connections");
		level_editor::GameVisibleProperties(true);
		level_editor::GameVisibleConnections(false);
		level_editor::GameTabVal = 0;
	}
	else
	{
		level_editor::pButtonGameTab->setText("Properties");
		level_editor::GameVisibleProperties(false);
		level_editor::GameVisibleConnections(true);
		level_editor::GameTabVal = 1;
	}

	return 0;
}

//##########################################################################

LRESULT SXLevelEditor_ListViewGameConnections_Click()
{
	int iNumStr = level_editor::pListViewGameConnections->getSelString();

	//очищаем комбобоксы
	//level_editor::pComboBoxGameConnectionsEvent->clear();
	level_editor::pComboBoxGameConnectionsAction->clear();

	//если нет выделенного тогда обнуляем
	if (iNumStr < 0)
	{
		level_editor::pComboBoxGameConnectionsEvent->setSel(0);
		level_editor::pEditGameConnectionsName->setText("");
		level_editor::pComboBoxGameConnectionsAction->setSel(0);
		level_editor::pEditGameConnectionsDelay->setText("");
		level_editor::pEditGameConnectionsParameter->setText("");
		return 0;
	}

	char szStr[256];
	char szStr2[256];

	//выделяем используемый event в ComboBoxGameConnectionsEvent
	level_editor::pListViewGameConnections->getItemText(szStr, 0, iNumStr, 256);

	for (int i = 0; i < level_editor::pComboBoxGameConnectionsEvent->getCount(); ++i)
	{
		level_editor::pComboBoxGameConnectionsEvent->getItemText(i, szStr2);
		if (strcmp(szStr, szStr2) == 0)
		{
			level_editor::pComboBoxGameConnectionsEvent->setSel(i);
			break;
		}
	}
	//-------

	//устанавилваем имя используемого игрового объекта в EditGameConnectionsName
	level_editor::pListViewGameConnections->getItemText(szStr, 1, iNumStr, 256);
	level_editor::pEditGameConnectionsName->setText(szStr);
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
			while(pPropTable)
			{
				//проходимся по всем полям класса
				for(int k = 0; k < pPropTable->numFields; ++k)
				{
					pPropData = &pPropTable->pData[k];
					if(pPropData->flags & PDFF_INPUT)
					{
						level_editor::pComboBoxGameConnectionsAction->addItem(pPropData->szEdName);
						level_editor::pComboBoxGameConnectionsAction->setItemData(level_editor::pComboBoxGameConnectionsAction->getCount() - 1, (LPARAM)(pPropData->szKey));
					}
				}
				pPropTable = pPropTable->pBaseProptable;
			}
		}
	}

	level_editor::pListViewGameConnections->getItemText(szStr, 2, iNumStr, 256);
	for (int i = 0; i < level_editor::pComboBoxGameConnectionsAction->getCount(); ++i)
	{
		level_editor::pComboBoxGameConnectionsAction->getItemText(i, szStr2);
		if (strcmp(szStr, szStr2) == 0)
		{
			level_editor::pComboBoxGameConnectionsAction->setSel(i);
			break;
		}
	}
	//-------

	//устанавливаем задержку в секундах в EditGameConnectionsDelay
	level_editor::pListViewGameConnections->getItemText(szStr, 3, iNumStr, 256);
	level_editor::pEditGameConnectionsDelay->setText(szStr);
	//-------

	//устанавливаем параметр в EditGameConnectionsParameter
	level_editor::pListViewGameConnections->getItemText(szStr, 4, iNumStr, 256);
	level_editor::pEditGameConnectionsParameter->setText(szStr);
	//-------

	level_editor::pButtonGameConnectionsDelete->setVisible(true);
	level_editor::pButtonGameConnectionsCreate->setText("Create");
	level_editor::isAddGameConections = false;

	return 0;
}

LRESULT SXLevelEditor_EditGameConnectionsName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = level_editor::pListBoxList->getSel();
	ID idSelStrTable = level_editor::pListViewGameConnections->getSelString();

	if (idSelListBoxList < 0)
		return 0;

	CBaseEntity *pEnt = SXGame_EntGet(idSelListBoxList);

	char szBuffer256[256];
	szBuffer256[0] = 0;

	level_editor::pEditGameConnectionsName->getText(szBuffer256, 256);

	CBaseEntity *pEnt2 = SXGame_EntGetByName(szBuffer256, 0);

	//записываем в szBuffer256 текущее имя action
	szBuffer256[0] = 0;
	level_editor::pComboBoxGameConnectionsAction->getItemText(level_editor::pComboBoxGameConnectionsAction->getSel(), szBuffer256);

	// если энтить с введенным именем существует
	if (pEnt2)
	{
		proptable_t *pPropTable2 = SXGame_EntGetProptable(pEnt2->getClassName());
		propdata_t *pPropData2 = 0;
		level_editor::pComboBoxGameConnectionsAction->clear();

		while(pPropTable2)
		{
		//проходимся по всем данным таблицы свойств
			for(int j = 0; j < pPropTable2->numFields; ++j)
			{
				pPropData2 = &pPropTable2->pData[j];

				//если свойство input
				if(pPropData2->flags & PDFF_INPUT)
				{
					//добавляем в комбобокс action
					level_editor::pComboBoxGameConnectionsAction->addItem(pPropData2->szEdName);
					level_editor::pComboBoxGameConnectionsAction->setItemData(level_editor::pComboBoxGameConnectionsEvent->getCount() - 1, (LPARAM)(pPropData2->szKey));

					//если предыдущее имя action равно последнему вставленному
					if(strcmp(pPropData2->szEdName, szBuffer256) == 0)
					{
						//активируем эту строку в комбобоксе
						level_editor::pComboBoxGameConnectionsAction->setSel(level_editor::pComboBoxGameConnectionsAction->getCount() - 1);
					}
				}
			}
			pPropTable2 = pPropTable2->pBaseProptable;
		}
	}
	// если такого энтитя не существует
	else
	{
		//возвращаем то что сейчас в таблице

		if (idSelStrTable < 0)
			return 0;

		szBuffer256[0] = 0;
		level_editor::pListViewGameConnections->getItemText(szBuffer256, 1, idSelStrTable, 256);
		level_editor::pEditGameConnectionsName->setText(szBuffer256);
	}

	return SXLevelEditor_EditGameConnections_Enter(hwnd, msg, wParam, lParam);
}

LRESULT SXLevelEditor_EditGameConnections_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::isAddGameConections)
		return 0;

	ID idSelListBoxList = level_editor::pListBoxList->getSel();
	ID idSelStrTable = level_editor::pListViewGameConnections->getSelString();

	if (idSelListBoxList < 0 || idSelStrTable < 0)
		return 0;

	ID idSelEnt = level_editor::pListBoxList->getItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());
	propdata_t *pPropData = (propdata_t*)(level_editor::pListViewGameConnections->getItemData(idSelStrTable));

	//получаем скленные в единую строку данные
	char szBuffer4096[4096];
	szBuffer4096[0] = 0;
	SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, -1, szBuffer4096, 4096);


	//устанавливаем в выделенную в таблице строку данные из редакторов
	char szBuffer256[256];
	szBuffer256[0] = 0;

	level_editor::pComboBoxGameConnectionsEvent->getItemText(level_editor::pComboBoxGameConnectionsEvent->getSel(), szBuffer256);
	level_editor::pListViewGameConnections->setItemText(szBuffer256, 0, idSelStrTable);
	
	szBuffer256[0] = 0;
	level_editor::pEditGameConnectionsName->getText(szBuffer256, 256);
	level_editor::pListViewGameConnections->setItemText(szBuffer256, 1, idSelStrTable);

	szBuffer256[0] = 0;
	level_editor::pComboBoxGameConnectionsAction->getItemText(level_editor::pComboBoxGameConnectionsAction->getSel(), szBuffer256);
	level_editor::pListViewGameConnections->setItemText(szBuffer256, 2, idSelStrTable);

	szBuffer256[0] = 0;
	level_editor::pEditGameConnectionsDelay->getText(szBuffer256, 256);
	level_editor::pListViewGameConnections->setItemText(szBuffer256, 3, idSelStrTable);

	szBuffer256[0] = 0;
	level_editor::pEditGameConnectionsParameter->getText(szBuffer256, 256);
	level_editor::pListViewGameConnections->setItemText(szBuffer256, 4, idSelStrTable);

	pEnt->setKV(pPropData->szKey, szBuffer4096);

	return 0;
}

void SXLevelEditor_VeldStringGameConnections(proptable_t *pPropTable, propdata_t *pPropData, int iNumExclude, char* szStrOut, int iSize)
{
	char szBuffer256[256];
	szBuffer256[0] = 0;

	//проходимся по всем строкам таблицы
	for (int i = 0, il = level_editor::pListViewGameConnections->getStringCount(); i < il; ++i)
	{
		//если строку с этим номером надо исключить
		if (iNumExclude == i)
			continue;

		propdata_t *pPropData2 = (propdata_t*)level_editor::pListViewGameConnections->getItemData(i);

		//если юзердата то что нам надо
		if (pPropData == pPropData2)
		{
			if (strlen(szStrOut) > 0)
				sprintf(szStrOut + strlen(szStrOut), ",");

			szBuffer256[0] = 0;
			level_editor::pListViewGameConnections->getItemText(szBuffer256, 1, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s:", szBuffer256);

			//ищем энтить с которым будет соединение
			CBaseEntity *pEnt2 = SXGame_EntGetByName(szBuffer256);
			proptable_t *pPropTable2 = 0;

			if (pEnt2)
				pPropTable2 = SXGame_EntGetProptable(pEnt2->getClassName());

			szBuffer256[0] = 0;
			level_editor::pListViewGameConnections->getItemText(szBuffer256, 2, i, 256);

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
			level_editor::pListViewGameConnections->getItemText(szBuffer256, 3, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s:", szBuffer256);

			szBuffer256[0] = 0;
			level_editor::pListViewGameConnections->getItemText(szBuffer256, 4, i, 256);
			sprintf(szStrOut + strlen(szStrOut), "%s", szBuffer256);
		}
	}
}

LRESULT SXLevelEditor_ButtonGameConnectionsCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = level_editor::pListBoxList->getSel();
	ID idSelStrTable = level_editor::pListViewGameConnections->getSelString();

	if (idSelListBoxList < 0)
		return 0;

	ID idSelEnt = level_editor::pListBoxList->getItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());

	if (!pEnt)
		return 0;

	//если добавление нового соединения
	if (level_editor::isAddGameConections)
	{
		if (idSelStrTable < 0)
			return 0;

		propdata_t *pPropData = (propdata_t*)(level_editor::pListViewGameConnections->getItemData(idSelStrTable));

		level_editor::pButtonGameConnectionsDelete->setVisible(true);
		level_editor::pButtonGameConnectionsCreate->setText("Create");
		level_editor::isAddGameConections = false;

		char szBuffer4096[4096];
		szBuffer4096[0] = 0;

		SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, -1, szBuffer4096, 4096);

		if (strlen(szBuffer4096) > 0)
			sprintf(szBuffer4096 + strlen(szBuffer4096), ",");

		char szBuffer256[256];
		szBuffer256[0] = 0;

		level_editor::pEditGameConnectionsName->getText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", szBuffer256);

		szBuffer256[0] = 0;
		long lItemData = level_editor::pComboBoxGameConnectionsAction->getItemData(level_editor::pComboBoxGameConnectionsAction->getSel());
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", (char*)lItemData);

		szBuffer256[0] = 0;
		level_editor::pEditGameConnectionsDelay->getText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s:", szBuffer256);

		szBuffer256[0] = 0;
		level_editor::pEditGameConnectionsParameter->getText(szBuffer256, 256);
		sprintf(szBuffer4096 + strlen(szBuffer4096), "%s", szBuffer256);

		pEnt->setKV(pPropData->szKey, szBuffer4096);

		level_editor::GameSel(idSelListBoxList);
	}
	//иначе создание нового соединения
	else
	{
		level_editor::pButtonGameConnectionsDelete->setVisible(false);
		level_editor::pButtonGameConnectionsCreate->setText("Add");
		level_editor::isAddGameConections = true;
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGameConnectionsDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ID idSelListBoxList = level_editor::pListBoxList->getSel();
	ID idSelStrTable = level_editor::pListViewGameConnections->getSelString();

	if (idSelListBoxList < 0 || idSelStrTable < 0)
		return 0;

	ID idSelEnt = level_editor::pListBoxList->getItemData(idSelListBoxList);
	CBaseEntity *pEnt = SXGame_EntGet(idSelEnt);
	proptable_t *pPropTable = SXGame_EntGetProptable(pEnt->getClassName());
	propdata_t *pPropData = (propdata_t*)(level_editor::pListViewGameConnections->getItemData(idSelStrTable));

	if (!pEnt)
		return 0;

	char szBuffer4096[4096];
	szBuffer4096[0] = 0;

	SXLevelEditor_VeldStringGameConnections(pPropTable, pPropData, idSelStrTable, szBuffer4096, 4096);

	pEnt->setKV(pPropData->szKey, szBuffer4096);
	level_editor::GameSel(idSelListBoxList);

	return 0;
}

