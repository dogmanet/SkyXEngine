
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
		level_editor::pCheckBoxGameFlags[i]->setVisible(false);

	level_editor::GameVisibleConnections(false);
}

void level_editor::GameSel(int sel)
{
	for (int i = 0; i < 16; ++i)
		level_editor::pCheckBoxGameFlags[i]->setVisible(false);

	level_editor::pComboBoxGameValue->setVisible(false);
	level_editor::pComboBoxGameValue->clear();
	level_editor::pEditGameValue->setVisible(false);
	level_editor::pEditGameValue->setText("");
	level_editor::pButtonGameCreate->setVisible(false);
	level_editor::pListViewGameClass->setEnable(true);
	level_editor::pListViewGameClass->clearStrings();

	//получение id игрового объекта
	ID isSelected = level_editor::pListBoxList->getItemData(sel);
	level_editor::idActiveElement = isSelected;
	level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;
	CBaseEntity *pEntity = SXGame_EntGet(isSelected);
	proptable_t *pPropertyTable = SXGame_EntGetProptable(pEntity->getClassName());

	//определяем класс
	char szComboBoxClass[256];
	const char *szClassObj = pEntity->getClassName();
	for (int i = 0; i < level_editor::pComboBoxGameClass->getCount(); ++i)
	{
		level_editor::pComboBoxGameClass->getItemText(i, szComboBoxClass);
		if (strcmp(szComboBoxClass, szClassObj) == 0)
		{
			level_editor::pComboBoxGameClass->setSel(i);
			break;
		}
	}
	
	propdata_t *pPropertyData;
	char szKey[256];
	char szVal[256];

	Array<proptable_t*> aPropTable;

	proptable_t *pPropTableBase = pPropertyTable->pBaseProptable;
	while (pPropTableBase)
	{
		aPropTable.push_back(pPropTableBase);
		pPropTableBase = pPropTableBase->pBaseProptable;
	}

	
	memset(level_editor::aGameObjectFlags, 0, sizeof(const char*)* 16);

	//проход по всем родительским таблицам свойств и вставка их в таблицу 
	for (int k = 0; k < aPropTable.size(); ++k)
	{
		pPropTableBase = aPropTable[(aPropTable.size() - 1) - k];
		
		//проход по всему списку свойств родительской таблицы
		for (int i = 0; i < pPropTableBase->numFields; ++i)
		{
			pPropertyData = &pPropTableBase->pData[i];
			
			if (pPropertyData->szKey && pPropertyData->szEdName && !(pPropertyData->flags & PDFF_NOEDIT) && pPropertyData->editor.type != PDE_NONE)
			{
				sprintf(szKey, "%s", pPropertyData->szEdName);
				pEntity->getKV(pPropertyData->szKey, szVal, 256);

				//вставляем новую строку
				int iString = level_editor::pListViewGameClass->addString((long)pPropertyData);

				//вставляем название в первую ячейку, а во вторую значение
				level_editor::pListViewGameClass->setItemText(szKey, 0, iString);
				level_editor::pListViewGameClass->setItemText(szVal, 1, iString);
			}

			if (pPropertyData->szEdName && pPropertyData->type == PDF_FLAG)
			{
				int index = log2(pPropertyData->flags) - 16;
				level_editor::aGameObjectFlags[index] = pPropertyData->szEdName;
			}
		}
	}

	aPropTable.clear();

	//проход по всей собственной таблице свойств и вставка ее значений в таблицу
	for (int i = 0; i < pPropertyTable->numFields; ++i)
	{
		pPropertyData = &pPropertyTable->pData[i];
		if (pPropertyData->szKey && pPropertyData->szEdName && !(pPropertyData->flags & PDFF_NOEDIT) && pPropertyData->editor.type != PDE_NONE)
		{
			sprintf(szKey, "%s", pPropertyData->szEdName);
			pEntity->getKV(pPropertyData->szKey, szVal, 256);
			int iString = level_editor::pListViewGameClass->addString((long)pPropertyData);

			level_editor::pListViewGameClass->setItemText(szKey, 0, iString);
			level_editor::pListViewGameClass->setItemText(szVal, 1, iString);
		}

		if (pPropertyData->szEdName && pPropertyData->type == PDF_FLAG)
		{
			int index = log2(pPropertyData->flags) - 16;
			level_editor::aGameObjectFlags[index] = pPropertyData->szEdName;
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
	for (int i = 0; i < pPropertyTable->numFields; ++i)
	{
		pPropertyData = &pPropertyTable->pData[i];
		
		//если поле класса то что нам надо
		if (pPropertyData->flags & PDFF_OUTPUT)
		{
			//получаем строку с запакованными данными
			pEntity->getKV(pPropertyData->szKey, szBuffer4096, 4096);

			//парсим строку вида Имя:действие:задержка:параметр[,и т.д]
			int iCountConnections = parse_str(szBuffer4096, NULL, 0, ',');
			char **ppConnections = (char**)alloca(sizeof(char*)*iCountConnections);
			iCountConnections = parse_str(szBuffer4096, ppConnections, iCountConnections, ',');

			//разбор соеднинений
			for (int k = 0; k < iCountConnections; ++k)
			{
				//парсим строку соединения на части
				int iCountParts = parse_str(ppConnections[k], NULL, 0, ':');
				char **ppParts = (char**)alloca(sizeof(char*)*iCountParts);
				iCountParts = parse_str(ppConnections[k], ppParts, iCountParts, ':');

				//разбор частей соединения
				if (iCountParts > 0)
				{
					int iNumStr = level_editor::pListViewGameConnections->addString((long)pPropertyData);
					sprintf(szKey, "%s", pPropertyData->szEdName);
					level_editor::pListViewGameConnections->setItemText(szKey, 0, iNumStr);

					for (int j = 0; j < iCountParts; ++j)
						level_editor::pListViewGameConnections->setItemText(ppParts[j], j + 1, iNumStr);

					CBaseEntity *pEntity2 = SXGame_EntGetByName(ppParts[0], 0);
					if (pEntity2)
					{
						proptable_t *pPropTable2 = SXGame_EntGetProptable(pEntity2->getClassName());
						propdata_t *pPropData2 = 0;
						for (int j = 0; j < pPropTable2->numFields; ++j)
						{
							pPropData2 = &pPropTable2->pData[j];
							if (pPropData2->flags & PDFF_INPUT && strcmp(pPropData2->szKey, ppParts[1]) == 0)
							{
								sprintf(szKey, "%s", pPropData2->szEdName);
								level_editor::pListViewGameConnections->setItemText(szKey, 2, iNumStr);
							}
						}
					}
				}
			}

			level_editor::pComboBoxGameConnectionsEvent->addItem(pPropertyData->szEdName);
			level_editor::pComboBoxGameConnectionsEvent->setItemData(level_editor::pComboBoxGameConnectionsEvent->getCount()-1, (LPARAM)(pPropertyData->szKey));
		}
	}
		
	level_editor::pAxesHelper->setPosition(pEntity->getPos());
	level_editor::pAxesHelper->setRotation(pEntity->getOrient());
	level_editor::pAxesHelper->setScale(float3(1, 1, 1));
}

void level_editor::GameTraceSetPos()
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GAME || level_editor::idActiveElement < 0)
		return;

	CBaseEntity *pEntity = SXGame_EntGet(level_editor::pListBoxList->getItemData(level_editor::pListBoxList->getItemData(level_editor::idActiveElement)));
	
	float3 vResult;
	if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, 0, 0))
	{
		pEntity->setPos(vResult);
		level_editor::pAxesHelper->setPosition(vResult);
		level_editor::GameRestoreListViewObject();
	}
}

void level_editor::GameTraceCreate()
{
	if (level_editor::iActiveGroupType != -EDITORS_LEVEL_GROUPTYPE_GAME)
		return;

	float3 vResult;
	if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, 0, 0))
	{
		SXLevelEditor_ButtonGameCreate_Click(0, 0, 0, 0);
		CBaseEntity *pEntity = SXGame_EntGet(level_editor::pListBoxList->getItemData(level_editor::idActiveElement));
		pEntity->setPos(vResult);
		level_editor::pAxesHelper->setPosition(vResult);
	}
}

void level_editor::GameRestoreListViewObject()
{
	int iSelected = level_editor::pListBoxList->getSel();

	if (iSelected < 0)
		return;

	CBaseEntity* bEnt = SXGame_EntGet(level_editor::pListBoxList->getItemData(iSelected));
	char szStr4096[4096];

	for (int i = 0; i < level_editor::pListViewGameClass->getStringCount(); ++i)
	{
		propdata_t* pd = (propdata_t*)level_editor::pListViewGameClass->getItemData(i);
		bEnt->getKV(pd->szKey, szStr4096, 4096);
		level_editor::pListViewGameClass->setItemText(szStr4096, 1, i);
	}
}

void level_editor::GameUpdatePosRot()
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GAME || level_editor::idActiveElement < 0)
		return;

	CBaseEntity *pEntity = SXGame_EntGet(level_editor::pListBoxList->getItemData(level_editor::idActiveElement));
	propdata_t *pPropData = 0;
	char szVal[256];

	for (int i = 0; i < level_editor::pListViewGameClass->getStringCount(); ++i)
	{
		pPropData = (propdata_t*)level_editor::pListViewGameClass->getItemData(i);

		if (pPropData)
		{
			if (stricmp(pPropData->szKey, "origin") == 0 || stricmp(pPropData->szKey, "rotation") == 0)
			{
				pEntity->getKV(pPropData->szKey, szVal, 256);
				level_editor::pListViewGameClass->setItemText(szVal, 1, i);
			}
		}
	}
}

void level_editor::GameVisibleProperties(bool bf)
{
	level_editor::pStaticGameClass->setVisible(bf);
	level_editor::pComboBoxGameClass->setVisible(bf);
	level_editor::pListViewGameClass->setVisible(bf);
	level_editor::pComboBoxGameValue->setVisible(false);
	level_editor::pEditGameValue->setVisible(false);
	level_editor::pButtonGameValue->setVisible(false);
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

void level_editor::GameTransformByHelper()
{
	if (!(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && level_editor::idActiveElement >= 0))
		return;

	CBaseEntity *pEntity = SXGame_EntGet(level_editor::pListBoxList->getItemData(level_editor::idActiveElement));

	if (!pEntity)
		return;

	if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_MOVE)
		pEntity->setPos(level_editor::pAxesHelper->getPosition());
	else if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_ROTATE)
		pEntity->setOrient(level_editor::pAxesHelper->getRotationQ());

	level_editor::GameUpdatePosRot();
}

void level_editor::GameDelete(int iSelected)
{
	if (!(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && iSelected >= 0))
		return;

	if (SXGame_EntGetCount() > 0 && iSelected < SXGame_EntGetCount())
	{
		SXGame_RemoveEntity(SXGame_EntGet(level_editor::pListBoxList->getItemData(iSelected)));
		level_editor::pListBoxList->deleteItem(iSelected);
	}
}

void level_editor::FillListBoxGameObj(int iSelect)
{
	level_editor::pListBoxList->clear();

	int iCountGameObj = SXGame_EntGetCount();
	int iCountGameObj2 = 0;
	char szNote[1024];
	for (int i = 0; i < iCountGameObj; ++i)
	{
		CBaseEntity *pEntity = SXGame_EntGet(i);
		if (pEntity)
		{
			sprintf(szNote, "%s / %s", pEntity->getName(), pEntity->getClassName());
			level_editor::pListBoxList->addItem(szNote);
			level_editor::pListBoxList->setItemData(level_editor::pListBoxList->getItemCount() - 1, i);
			++iCountGameObj2;
		}
	}

	level_editor::pStaticListValCount->setText(String(iCountGameObj2).c_str());

	if (iSelect < 0)
	{
		level_editor::iActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GAME;
		level_editor::idActiveElement = -1;
	}
	else
	{
		level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;
		level_editor::idActiveElement = iSelect;
		level_editor::pListBoxList->setSel(iSelect);
		level_editor::GameSel(iSelect);
	}
}

//##########################################################################

LRESULT SXLevelEditor_ListViewGameClass_Click()
{
	int iSelString = level_editor::pListViewGameClass->getSelString();
	if (iSelString < 0)
	{
		level_editor::pEditGameValue->setVisible(false);
		level_editor::pButtonGameValue->setVisible(false);
		level_editor::pComboBoxGameValue->setVisible(false);
		return 0;
	}

	propdata_t *pPropData = (propdata_t*)level_editor::pListViewGameClass->getItemData(iSelString);

	level_editor::pEditGameValue->setVisible(false);
	level_editor::pButtonGameValue->setVisible(false);
	level_editor::pComboBoxGameValue->setVisible(false);

	char txtval[256];

	for (int i = 0; i < 16; ++i)
	{
		level_editor::pCheckBoxGameFlags[i]->setVisible(false);
	}

	if (pPropData->editor.type == PDE_TEXTFIELD)
	{
		level_editor::pEditGameValue->setVisible(true);
		level_editor::pListViewGameClass->getItemText(txtval, 1, iSelString, 256);
		level_editor::pEditGameValue->setText(txtval);

		level_editor::pStaticGameHelp->setVisible(true);
		level_editor::pMemoGameHelp->setVisible(true);
	}
	else if (pPropData->editor.type == PDE_FILEFIELD)
	{
		level_editor::pEditGameValue->setVisible(true);
		level_editor::pButtonGameValue->setVisible(true);
		level_editor::pListViewGameClass->getItemText(txtval, 1, iSelString, 256);
		level_editor::pEditGameValue->setText(txtval);

		level_editor::pStaticGameHelp->setVisible(true);
		level_editor::pMemoGameHelp->setVisible(true);
	}
	else if (pPropData->editor.type == PDE_COMBOBOX)
	{
		level_editor::pStaticGameHelp->setVisible(true);
		level_editor::pMemoGameHelp->setVisible(true);

		level_editor::pComboBoxGameValue->clear();
		level_editor::pComboBoxGameValue->setVisible(true);
		editor_kv* ekv = (editor_kv*)pPropData->editor.pData;
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
		level_editor::pListViewGameClass->getItemText(tval, 1, iSelString, 256);

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
	else if (pPropData->editor.type == PDE_FLAGS)
	{
		level_editor::pStaticGameHelp->setVisible(false);
		level_editor::pMemoGameHelp->setVisible(false);
		
		char szVal[256];
		level_editor::pListViewGameClass->getItemText(szVal, 1, iSelString, 256);
		UINT uiFlags;
		sscanf(szVal, "%d", &uiFlags);
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

	int iSelString = level_editor::pListViewGameClass->getSelString();
	
	if (iSelString < 0)
		return 0;

	char txt[256];
	level_editor::pEditGameValue->getText(txt, 256);
	level_editor::pListViewGameClass->setItemText(txt, 1, iSelString);
	CBaseEntity* bEnt = SXGame_EntGet(level_editor::pListBoxList->getItemData(sel));
	if (bEnt)
	{
		propdata_t* pd = (propdata_t*)level_editor::pListViewGameClass->getItemData(iSelString);
		bool bf = bEnt->setKV(pd->szKey, txt);
		/*const char *szcName = bEnt->getName();
		char* szName = (char*)(bEnt->getName());
		level_editor::pListViewGameClass->setItemText(szName, 1, iSelString);*/

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
	int iSelString = level_editor::pListViewGameClass->getSelString();
	propdata_t *pPropData = (propdata_t*)level_editor::pListViewGameClass->getItemData(iSelString);

	int iLength = 0;
	editor_kv *pEkv = (editor_kv*)pPropData->editor.pData;

	while (pEkv)
	{
		if (pEkv->key && pEkv->value)
		{
			iLength += strlen(pEkv->key) + strlen(pEkv->value) + 2;
			pEkv++;
		}
		else
			break;
	}

	iLength += 2;

	char* txtfmt = new char[iLength];

	iLength = 0;
	pEkv = (editor_kv*)pPropData->editor.pData;

	while (pEkv)
	{
		if (pEkv->key && pEkv->value)
		{
			memcpy(txtfmt + iLength, pEkv->key, strlen(pEkv->key));
			iLength += strlen(pEkv->key);
			txtfmt[iLength++] = 0;
			memcpy(txtfmt + iLength, pEkv->value, strlen(pEkv->value));
			iLength += strlen(pEkv->value);
			txtfmt[iLength++] = 0;
			pEkv++;
		}
		else
			break;
	}

	txtfmt[iLength++] = 0;


	char szPath[1024];
	szPath[0] = 0;
	char szName[1024];
	
	if (gui_func::dialogs::SelectFileOwn(szName, szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(szPath, Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE));
		level_editor::pEditGameValue->setText(sRpath.c_str());

		int iSelected = level_editor::pListBoxList->getSel();
		level_editor::pListViewGameClass->setItemText((char*)sRpath.c_str(), 1, iSelString);
		CBaseEntity *pEntity = SXGame_EntGet(level_editor::pListBoxList->getItemData(iSelected));
		if (pEntity)
			pEntity->setKV(pPropData->szKey, sRpath.c_str());
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

	char szText[256];
	level_editor::pComboBoxGameClass->getItemText(level_editor::pComboBoxGameClass->getSel(), szText);

	CBaseEntity *pEntity = SXGame_CreateEntity(szText);
	pEntity->setFlags(pEntity->getFlags() | EF_EXPORT | EF_LEVEL);

	SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);
	int iSelected = level_editor::pListBoxList->getItemCount() - 1;
	level_editor::pListBoxList->setSel(iSelected);
	level_editor::GameSel(iSelected);
	level_editor::idActiveElement = iSelected;
	level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GAME;

	return 0;
}

LRESULT SXLevelEditor_EditGameConnectionsName_IN(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szEditText[256];
	level_editor::pEditGameConnectionsName->getText(szEditText, 256);
	if (szEditText[0] == 0)
		return 0;

	if (!(isalpha(wParam) || isdigit(wParam) || (char)wParam == '_'))
		return 0;

	char *szEditTextLower = CharLower(szEditText);
	int iCountObj = SXGame_EntGetCount();
	char szName[256];
	char *szNameLower = 0;
	char *szFound = 0;
	for (int i = 0; i < iCountObj; ++i)
	{
		CBaseEntity *pEntity = SXGame_EntGet(i);
		if (pEntity)
		{
			strcpy(szName, pEntity->getName());
			if (szName[0] == 0)
				continue;
			szNameLower = CharLower(szName);
			if (szFound = strstr(szNameLower, szEditTextLower))
			{
				int qq = szFound - szNameLower;
				if (qq == 0)
				{
					level_editor::pEditGameConnectionsName->setText(szNameLower);
					PostMessage(level_editor::pEditGameConnectionsName->getHWND(), EM_SETSEL, strlen(szEditText), strlen(szNameLower));
					return 0;
				}
			}
		}
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGameTab_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::GameTabVal == 1)
	{
		level_editor::pButtonGameTab->setText("Connections");
		level_editor::GameVisibleProperties(true);
		level_editor::GameVisibleConnections(false);
		level_editor::GameTabVal = 0;
		level_editor::GameRestoreListViewObject();
		level_editor::pListViewGameConnections->setSelString(-1);
	}
	else
	{
		level_editor::pButtonGameTab->setText("Properties");
		level_editor::GameVisibleProperties(false);
		level_editor::GameVisibleConnections(true);
		level_editor::GameTabVal = 1;
		level_editor::pListViewGameClass->setSelString(-1);
	}

	return 0;
}

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
			while(pPropTable)
			{
				propdata_t *pPropData = 0;

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

	CBaseEntity *pEnt2 = 0;

	if (stricmp(szBuffer256, "!this") == 0)
		pEnt2 = pEnt;
	else if (stricmp(szBuffer256, "!parent") == 0)
		pEnt2 = pEnt->getParent();
	else
		pEnt2 = SXGame_EntGetByName(szBuffer256, 0);

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

