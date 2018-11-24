
#include "common_callback.h"

LRESULT TrueExit(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Exit?", "Exit?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hWnd, uiMsg, wParam, lParam);

	return 0;
}

LRESULT ComMenuId(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);

	//файл
	//новый
	if (id == ID_FILE_NEW)
	{
		level_editor::LevelNew(true);
	}
	//открыть
	else if (id == ID_FILE_OPEN)
	{
		level_editor::LevelOpen();
	}
	//сохранить
	else if (id == ID_FILE_SAVE)
	{
		level_editor::LevelSave();
	}
	//сохранить как
	else if (id == ID_FILE_SAVEAS)
	{
		level_editor::LevelSaveAs();
	}
	//выход
	else if (id == ID_FILE_EXIT)
	{
		return TrueExit(hWnd, uiMsg, wParam, lParam);
	}

	else if (id == ID_VIEW_GRID)
	{
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
		SRender_EditorSetRenderGrid(level_editor::pMainMenu->getCheckedItem(id));
	}
	else if (id == ID_VIEW_AXES)
	{
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
		SRender_EditorSetRenderAxesStatic(level_editor::pMainMenu->getCheckedItem(id));
	}

	//
	else if (id == ID_FINALIMAGE_COLOR)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
		level_editor::pCheckBoxTBRColor->setCheck(true);
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
		level_editor::pCheckBoxTBRNormal->setCheck(true);
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
		level_editor::pCheckBoxTBRParam->setCheck(true);
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
		level_editor::pCheckBoxTBRAmDiff->setCheck(true);
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
		level_editor::pCheckBoxTBRSpecular->setCheck(true);
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
		level_editor::pCheckBoxTBRLighting->setCheck(true);
	}

	//
	else if (id == ID_SELECTIONSETTINGS_SELECTION)
	{
		level_editor::canSelSelection = !level_editor::pMainMenu->getCheckedItem(id);
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_ZTEST)
	{
		level_editor::canSelZTest = !level_editor::pMainMenu->getCheckedItem(id);
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_MESH)
	{
		level_editor::canSelMesh = !level_editor::pMainMenu->getCheckedItem(id);
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_BACKFACESCULL)
	{
		level_editor::canSelBackFacesCull = !level_editor::pMainMenu->getCheckedItem(id);
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
	}

	else if (id >= SX_LE_MMENU_WEATHER_BEGIN_ID)
	{
		for (int i = 0; i < level_editor::iMenuWeatherCount; ++i)
		{
			level_editor::pMainMenu->setCheckItem(SX_LE_MMENU_WEATHER_BEGIN_ID+i, false);
		}
		level_editor::pMainMenu->setCheckItem(id, true);
		level_editor::idMenuWeatherCurr = (id - SX_LE_MMENU_WEATHER_BEGIN_ID) - 1;

		if (level_editor::idMenuWeatherCurr >= 0)
		{
			char szPath[1024];
			sprintf(szPath, "%sweather\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_CONFIGS), level_editor::aMenuWeather[level_editor::idMenuWeatherCurr]);
			SLevel_WeatherLoad(szPath);
		}
		else
		{
			SLevel_WeatherLoad(0);

			ID idGlobalLight = SLight_GetGlobal();
			if (idGlobalLight >= 0)
			{
				SLight_SetPos(idGlobalLight, &float3(60, 60, 0), false);
				SLight_SetColor(idGlobalLight, &float3(1, 1, 1));
				SLight_SetEnable(idGlobalLight, true);
			}
		}
	}

	return 0;
}

LRESULT MsgEditSize(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	static int *r_resize = (int*)GET_PCVAR_INT("r_resize");

	if (!r_resize)
		r_resize = (int*)GET_PCVAR_INT("r_resize");

	if (!r_resize)
		return 0;

	*r_resize = RENDER_RESIZE_RESIZE;
	return 0;
}

//##########################################################################

LRESULT SXLevelEditor_RenderWindow_MouseMove(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	//если включено копирование и id копирования валидный
	if (level_editor::useCopyData && level_editor::idCopy >= 0)
	{
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
			level_editor::GeomUpdateCopyPos();

		return 0;
	}

	//сообщаем хелперу о движениях мыши
	level_editor::pAxesHelper->onMouseMove(((int)(short)LOWORD(lParam)), ((int)(short)HIWORD(lParam)));

	if (level_editor::pAxesHelper->m_bIsDragging == false)
		return 0;

	//если активна статическая геометрия
	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM && level_editor::idActiveElement >= 0)
		level_editor::GeomTransformByHelper();
	//если выделена растительность и выделен конкретный объект
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0 && level_editor::idActiveGreenSplit >= 0 && level_editor::idActiveGreenObject >= 0)
		level_editor::GreenTransformByHelper();
	//если выделен игровой объект
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && level_editor::idActiveElement >= 0)
		level_editor::GameTransformByHelper();

	return 0;
}

LRESULT SXLevelEditor_RenderWindow_LDown(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	level_editor::pAxesHelper->m_bIsDragging = true;
	level_editor::pAxesHelper->m_bIsDraggingStart = true;
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_LClick(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	// если включено копирование
	if (level_editor::useCopyData && level_editor::idCopy >= 0)
	{
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
			level_editor::GeomCopy();

		return 0;
	}

	level_editor::pAxesHelper->m_bIsDragging = false;
	level_editor::pAxesHelper->m_bIsDraggingStop = true;

	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	//если включено выделение, тогда находим 
	if (level_editor::pCheckBoxTBArrow->getCheck())
	{

		level_editor::idMtl = -1;

		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || level_editor::iActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GEOM)
			level_editor::GeomTraceSelect();
		else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0)
			level_editor::GreenTraceSelect();
		else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && level_editor::idActiveElement >= 0)
		{

		}
		else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
			level_editor::AIGridTraceSelect();
	}
	else if (
		!level_editor::pCheckBoxTBArrow->getCheck() &&
		!level_editor::pCheckBoxTBRot->getCheck() &&
		!level_editor::pCheckBoxTBScale->getCheck() &&
		!level_editor::pCheckBoxTBPos->getCheck()
		)
	{
		if (level_editor::iActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GEOM)
			level_editor::GeomTraceCreate();
		else if (level_editor::iActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GAME)
			level_editor::GameTraceCreate();
	}
	
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_RClick(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0)
		level_editor::GreenSetPos4Box();
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
		level_editor::AIGridMultiSelect();

	return 0;
}

LRESULT SXLevelEditor_RenderWindow_MBUp(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
		SAIG_QuadSelect(-1, false);
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM && level_editor::idActiveElement >= 0)
		level_editor::GeomTraceSetPos();
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0)
		level_editor::GeomTraceSetPos();
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && level_editor::idActiveElement >= 0)
		level_editor::GameTraceSetPos();

	return 0;
}

//##########################################################################

LRESULT SXLevelEditor_RenderWindow_Delete(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0 && level_editor::idActiveGreenSplit >= 0 && level_editor::idActiveGreenObject >= 0)
	{
		SGreen_DelObject(level_editor::idActiveElement, level_editor::idActiveGreenSplit, level_editor::idActiveGreenObject);
		level_editor::idActiveGreenSplit = -1;
		level_editor::idActiveGreenObject = -1;
		level_editor::pEditGreenSelX->setText("");
		level_editor::pEditGreenSelY->setText("");
		level_editor::pEditGreenSelZ->setText("");
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonDelete_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int iSelected = level_editor::pListBoxList->getSel();
	int iCount = level_editor::pListBoxList->getItemCount();
	int iSelSecond = iSelected;

	if (iCount - 1 == iSelected)
		iSelSecond = iSelected-1;

	if (iSelected < 0)
		return 0;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		level_editor::GeomDelete(iSelected);
		level_editor::FillListBoxGeom(iSelSecond);
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		level_editor::GreenDelete(iSelected);
		level_editor::FillListBoxGreen(iSelSecond);
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
	{
		level_editor::GameDelete(iSelected);
		level_editor::FillListBoxGameObj(iSelSecond);
	}

	return 0;
}

//##########################################################################

LRESULT SXLevelEditor_ButtonGeometryOpen_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	level_editor::FillListBoxGeom();

	level_editor::GreenActivateAll(false);
	level_editor::GameActivateAll(false);
	level_editor::AIGridActivateAll(false);
	level_editor::GeomActivateAll(false);
	level_editor::GeomActivateCreate(true);
	//level_editor::GeomActivateTrans(false);

	return 0;
}

LRESULT SXLevelEditor_ButtonGreenOpen_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	level_editor::FillListBoxGreen();

	level_editor::GeomActivateAll(false);
	level_editor::GameActivateAll(false);

	level_editor::GreenActivateAll(false);
	level_editor::GreenActivateCreate(true);
	level_editor::AIGridActivateAll(false);
	level_editor::GreenActivateMain(true);
	
	return 0;
}

LRESULT SXLevelEditor_ButtonGameObjectOpen_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	level_editor::FillListBoxGameObj();

	level_editor::GeomActivateAll(false);
	level_editor::GreenActivateAll(false);
	level_editor::AIGridActivateAll(false);
	level_editor::GameActivateAll(true);

	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridOpen_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	level_editor::pListBoxList->clear();

	level_editor::pStaticListValCount->setText("0");

	level_editor::GeomActivateAll(false);
	level_editor::GreenActivateAll(false);
	level_editor::GameActivateAll(false);

	level_editor::AIGridActivateAll(true);

	level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_AIGRID;
	level_editor::idActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ListBoxList_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int sel = level_editor::pListBoxList->getSel();

	level_editor::idActiveGreenSplit = -1;
	level_editor::idActiveGreenObject = -1;

	if (sel < 0)
		return 0;

	if (level_editor::iActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GEOM || level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_GetCountModels() > 0 && sel < SGeom_GetCountModels())
			level_editor::GeomSel(sel);
	}
	else if (level_editor::iActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GREEN || level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		if (SGreen_GetCount() > 0 && sel < SGreen_GetCount())
			level_editor::GreenSel(sel);
	}
	else if (level_editor::iActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GAME || level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
	{
		if (SXGame_EntGetCount() > 0 && sel < SXGame_EntGetCount())
			level_editor::GameSel(sel);
	}

	return 0;
}

//##########################################################################

LRESULT SXLevelEditor_GroupBox_Notify(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (((NMHDR*)lParam)->hwndFrom == level_editor::pListViewGameClass->getHWND() && ((NMHDR*)lParam)->code == NM_CLICK)
	{
		SXLevelEditor_ListViewGameClass_Click();
	}
	else if (((NMHDR*)lParam)->hwndFrom == level_editor::pListViewGameConnections->getHWND() && ((NMHDR*)lParam)->code == NM_CLICK)
	{
		SXLevelEditor_ListViewGameConnections_Click();
	}
	return 0;
}

LRESULT SXLevelEditor_ToolBar1_CallWmCommand(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int iNotification = HIWORD(wParam);
	HWND hElement = (HWND)(lParam);
	if (iNotification == BN_CLICKED)
	{
		if (level_editor::pCheckBoxTBArrow->getHWND() == hElement)
		{
			level_editor::pCheckBoxTBPos->setCheck(false);
			level_editor::pCheckBoxTBRot->setCheck(false);
			level_editor::pCheckBoxTBScale->setCheck(false);
			level_editor::pAxesHelper->setType(CAxesHelper::HANDLER_TYPE_NONE);
		}
		else if (level_editor::pCheckBoxTBPos->getHWND() == hElement)
		{
			if (
				level_editor::idActiveElement >= 0 &&
				(
				level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM ||
				(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveGreenSplit >= 0 && level_editor::idActiveGreenObject >= 0) ||
				level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME
				)
				)
			{
				level_editor::pCheckBoxTBArrow->setCheck(false);
				level_editor::pCheckBoxTBRot->setCheck(false);
				level_editor::pCheckBoxTBScale->setCheck(false);
				level_editor::pAxesHelper->setType(CAxesHelper::HANDLER_TYPE_MOVE);
			}
			else
				level_editor::pCheckBoxTBPos->setCheck(false);
		}
		else if (level_editor::pCheckBoxTBRot->getHWND() == hElement)
		{
			if ((level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME) && level_editor::idActiveElement >= 0)
			{
				level_editor::pCheckBoxTBArrow->setCheck(false);
				level_editor::pCheckBoxTBPos->setCheck(false);
				level_editor::pCheckBoxTBScale->setCheck(false);
				level_editor::pAxesHelper->setType(CAxesHelper::HANDLER_TYPE_ROTATE);
			}
			else
				level_editor::pCheckBoxTBRot->setCheck(false);
		}
		else if (level_editor::pCheckBoxTBScale->getHWND() == hElement)
		{
			if ((level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME) && level_editor::idActiveElement >= 0)
			{
				level_editor::pCheckBoxTBPos->setCheck(false);
				level_editor::pCheckBoxTBRot->setCheck(false);
				level_editor::pCheckBoxTBArrow->setCheck(false);
				level_editor::pAxesHelper->setType(CAxesHelper::HANDLER_TYPE_SCALE);
			}
			else
				level_editor::pCheckBoxTBScale->setCheck(false);
		}
		else if (level_editor::pButtonTBNew->getHWND() == hElement)
		{
			level_editor::LevelNew(true);
		}
		else if (level_editor::pButtonTBOpen->getHWND() == hElement)
		{
			level_editor::LevelOpen();
		}
		else if (level_editor::pButtonTBSave->getHWND() == hElement)
		{
			level_editor::LevelSave();
		}
		else if (level_editor::pButtonTBSaveAs->getHWND() == hElement)
		{
			level_editor::LevelSaveAs();
		}
		else if (level_editor::pCheckBoxTBGrid->getHWND() == hElement)
		{
			level_editor::pMainMenu->setCheckItem(ID_VIEW_GRID, level_editor::pCheckBoxTBGrid->getCheck());
			SRender_EditorSetRenderGrid(level_editor::pCheckBoxTBGrid->getCheck());
		}
		else if (level_editor::pCheckBoxTBAxes->getHWND() == hElement)
		{
			level_editor::pMainMenu->setCheckItem(ID_VIEW_AXES, level_editor::pCheckBoxTBAxes->getCheck());
			SRender_EditorSetRenderAxesStatic(level_editor::pCheckBoxTBAxes->getCheck());
		}
		else if (level_editor::pCheckBoxTBRColor->getHWND() == hElement)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_COLOR, true);
			level_editor::pCheckBoxTBRColor->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		}
		else if (level_editor::pCheckBoxTBRNormal->getHWND() == hElement)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, true);
			level_editor::pCheckBoxTBRNormal->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		}
		else if (level_editor::pCheckBoxTBRParam->getHWND() == hElement)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, true);
			level_editor::pCheckBoxTBRParam->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		}
		else if (level_editor::pCheckBoxTBRAmDiff->getHWND() == hElement)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			level_editor::pCheckBoxTBRAmDiff->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		}
		else if (level_editor::pCheckBoxTBRSpecular->getHWND() == hElement)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, true);
			level_editor::pCheckBoxTBRSpecular->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		}
		else if (level_editor::pCheckBoxTBRLighting->getHWND() == hElement)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			level_editor::pCheckBoxTBRLighting->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		}
		else if (level_editor::pCheckBoxTBSelS->getHWND() == hElement)
		{
			level_editor::canSelSelection = level_editor::pCheckBoxTBSelS->getCheck();
			level_editor::pMainMenu->setCheckItem(ID_SELECTIONSETTINGS_SELECTION, level_editor::pCheckBoxTBSelS->getCheck());
		}
		else if (level_editor::pCheckBoxTBSelZTest->getHWND() == hElement)
		{
			level_editor::canSelZTest = level_editor::pCheckBoxTBSelZTest->getCheck();
			level_editor::pMainMenu->setCheckItem(ID_SELECTIONSETTINGS_ZTEST, level_editor::pCheckBoxTBSelZTest->getCheck());
		}
		else if (level_editor::pCheckBoxTBSelMesh->getHWND() == hElement)
		{
			level_editor::canSelMesh = level_editor::pCheckBoxTBSelMesh->getCheck();
			level_editor::pMainMenu->setCheckItem(ID_SELECTIONSETTINGS_MESH, level_editor::pCheckBoxTBSelMesh->getCheck());
		}
		else if (level_editor::pCheckBoxTBSelCullBack->getHWND() == hElement)
		{
			level_editor::canSelBackFacesCull = level_editor::pCheckBoxTBSelCullBack->getCheck();
			level_editor::pMainMenu->setCheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, level_editor::pCheckBoxTBSelCullBack->getCheck());
		}
		else if (level_editor::pCheckBoxTBAIGBound->getHWND() == hElement)
		{
			level_editor::canAIGBound = level_editor::pCheckBoxTBAIGBound->getCheck();
		}
		else if (level_editor::pCheckBoxTBAIGQuad->getHWND() == hElement)
		{
			level_editor::canAIGQuad = level_editor::pCheckBoxTBAIGQuad->getCheck();
		}
		else if (level_editor::pCheckBoxTBAIGGraphPoint->getHWND() == hElement)
		{
			level_editor::canAIGGraphPoint = level_editor::pCheckBoxTBAIGGraphPoint->getCheck();
		}
		else if (level_editor::pCheckBoxTBLevelType->getHWND() == hElement)
		{
			if (level_editor::pCheckBoxTBLevelType->getCheck())
			{
				ID idGlobalLight = SLight_GetGlobal();
				if (idGlobalLight < 0)
				{
					idGlobalLight = SLight_CreatePoint(
						&float3(60, 60, 0),
						0,
						&float3(1, 1, 1),
						true,
						true);
					SLight_SetEnable(idGlobalLight, level_editor::pCheckBoxTBGLightEnable->getCheck());
					SLight_SetName(idGlobalLight, "sun");
				}
				level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP26);
			}
			else
			{
				ID idGlobalLight = SLight_GetGlobal();
				if (idGlobalLight >= 0)
					SLight_DeleteLight(idGlobalLight);
				level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
				level_editor::pCheckBoxTBGLightEnable->setCheck(false);
			}
		}
		else if (level_editor::pCheckBoxTBGLightEnable->getHWND() == hElement)
		{
			ID idGlobalLight = SLight_GetGlobal();
			if (idGlobalLight >= 0)
				SLight_SetEnable(idGlobalLight, level_editor::pCheckBoxTBGLightEnable->getCheck());
			else
				level_editor::pCheckBoxTBGLightEnable->setCheck(false);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_GroupBox_CallWmCommand(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int iNotification = HIWORD(wParam);
	HWND hElement = (HWND)(lParam);
	if (iNotification == BN_CLICKED)
	{
		if (hElement == level_editor::pCheckBoxAIGridMarkedSplits->getHWND())
			SAIG_GridSetMarkSplits(level_editor::pCheckBoxAIGridMarkedSplits->getCheck());
		else
		{
			ID iSelData = level_editor::pListBoxList->getItemData(level_editor::pListBoxList->getSel());
			CBaseEntity *pEntity = SXGame_EntGet(iSelData);
			int iSelString = level_editor::pListViewGameClass->getSelString();

			if (iSelData < 0 || iSelString < 0)
				return 0;

			char szVal[256];
			level_editor::pListViewGameClass->getItemText(szVal, 1, iSelString, 256);
			UINT uiFlags;
			sscanf(szVal, "%d", &uiFlags);

			for (int i = 0; i < 16; ++i)
			{
				if (level_editor::pCheckBoxGameFlags[i]->getHWND() == hElement)
				{
					if (level_editor::pCheckBoxGameFlags[i]->getCheck())
						uiFlags |= (1 << (i + 16));
					else
						uiFlags ^= (1 << (i + 16));
				}
			}

			sprintf(szVal, "%d", uiFlags);
			level_editor::pListViewGameClass->setItemText(szVal, 1, iSelString);
			pEntity->setKV("flags", szVal);
		}
	}
	else if (iNotification == CBN_SELCHANGE)
	{
		if (level_editor::pComboBoxGameValue->getHWND() == hElement)
		{
			int iSelected = level_editor::pListBoxList->getSel();
			char szStr[256];
			level_editor::pComboBoxGameValue->getItemText(level_editor::pComboBoxGameValue->getSel(), szStr);
			level_editor::pListViewGameClass->setItemText(szStr, 1, level_editor::pListViewGameClass->getSelString());
			CBaseEntity *pEntity = SXGame_EntGet(level_editor::pListBoxList->getItemData(iSelected));
			if (pEntity)
			{
				propdata_t *pPropData = (propdata_t*)level_editor::pListViewGameClass->getItemData(level_editor::pListViewGameClass->getSelString());
				level_editor::pComboBoxGameValue->getItemData(level_editor::pComboBoxGameValue->getSel());
				pEntity->setKV(pPropData->szKey, (const char*)level_editor::pComboBoxGameValue->getItemData(level_editor::pComboBoxGameValue->getSel()));
			}
		}
		else if (level_editor::pComboBoxGameClass->getHWND() == hElement)
		{
			if (level_editor::pComboBoxGameClass->getSel() == 0)
				return 0;

			int iSelected = level_editor::pListBoxList->getSel();

			if (iSelected < 0)
				return 0;

			CBaseEntity *pEntity = SXGame_EntGet(level_editor::pListBoxList->getItemData(iSelected));
			proptable_t *pPropTable = SXGame_EntGetProptable(pEntity->getClassName());

			propdata_t *pPropData;
			char szKey[256];
			char szVal[256];

			struct KeyVal
			{
				KeyVal(){}
				KeyVal(const char* _key, const char* _val)
				{
					key = _key;
					val = _val;
				}

				String key;
				String val;
			};

			Array<KeyVal> aKeyVal;

			Array<proptable_t*> aPropTable;

			proptable_t *pPropTableParent = pPropTable->pBaseProptable;
			while (pPropTable)
			{
				aPropTable.push_back(pPropTableParent);
				pPropTableParent = pPropTableParent->pBaseProptable;
			}

			for (int k = 0; k < aPropTable.size(); ++k)
			{
				pPropTableParent = aPropTable[(aPropTable.size() - 1) - k];
				for (int i = 0; i < pPropTableParent->numFields; ++i)
				{
					pPropData = &pPropTableParent->pData[i];
					if (pPropData->szKey && pPropData->szEdName && !(pPropData->flags & PDFF_NOEDIT) && pPropData->editor.type != PDE_NONE)
					{
						sprintf(szKey, "%s", pPropData->szEdName);
						pEntity->getKV(pPropData->szKey, szVal, 256);
						aKeyVal.push_back(KeyVal(pPropData->szKey, szVal));
					}
				}
			}

			aPropTable.clear();

			for (int i = 0; i < pPropTable->numFields; ++i)
			{
				pPropData = &pPropTable->pData[i];
				if (pPropData->szKey && pPropData->szEdName && !(pPropData->flags & PDFF_NOEDIT) && pPropData->editor.type != PDE_NONE)
				{
					sprintf(szKey, "%s", pPropData->szEdName);
					pEntity->getKV(pPropData->szKey, szVal, 256);
					aKeyVal.push_back(KeyVal(pPropData->szKey, szVal));
				}
			}

			char szStr[256];
			level_editor::pComboBoxGameClass->getItemText(level_editor::pComboBoxGameClass->getSel(), szStr);
			SXGame_RemoveEntity(pEntity);

			pEntity = SXGame_CreateEntity(szStr);
			pEntity->setFlags(pEntity->getFlags() | EF_EXPORT | EF_LEVEL);

			for (int i = 0; i < aKeyVal.size(); ++i)
			{
				pEntity->setKV(aKeyVal[i].key.c_str(), aKeyVal[i].val.c_str());
			}

			SXLevelEditor_ButtonGameObjectOpen_Click(hWnd, uiMsg, wParam, lParam);
			iSelected = -1;
			for (int i = 0; i < level_editor::pListBoxList->getItemCount(); ++i)
			{
				if (level_editor::pListBoxList->getItemData(i) == pEntity->getId())
				{
					iSelected = i;
					break;
				}
			}

			if (iSelected < 0)
			{

			}

			level_editor::pListBoxList->setSel(iSelected);
			level_editor::GameSel(iSelected);
		}
		else if (level_editor::pComboBoxGreenSel->getHWND() == hElement)
		{
			int iSelected = level_editor::pComboBoxGreenSel->getSel();
			level_editor::canGreenRenderBox = false;
			
			if (iSelected == 0)
			{
				level_editor::pStaticGreenSelX->setText("Position X:");
				level_editor::pStaticGreenSelY->setText("Position Y:");
				level_editor::pStaticGreenSelZ->setText("Position Z:");
				level_editor::pEditGreenSelX->setText("");
				level_editor::pEditGreenSelY->setText("");
				level_editor::pEditGreenSelZ->setText("");
			}
			else if (iSelected == 2)
			{
				level_editor::canGreenRenderBox = true;

				level_editor::pStaticGreenSelX->setText("Width volume:");
				level_editor::pStaticGreenSelY->setText("Height volume:");
				level_editor::pStaticGreenSelZ->setText("Depth volume:");
				level_editor::pEditGreenSelX->setText(String(level_editor::vGreenBoxWHD.x).c_str());
				level_editor::pEditGreenSelY->setText(String(level_editor::vGreenBoxWHD.y).c_str());
				level_editor::pEditGreenSelZ->setText(String(level_editor::vGreenBoxWHD.z).c_str());
			}
		}

		else if (level_editor::pComboBoxGameConnectionsEvent->getHWND() == hElement)
		{
			ID idSelListBoxList = level_editor::pListBoxList->getSel();
			ID idSelStrTable = level_editor::pListViewGameConnections->getSelString();

			if (idSelListBoxList < 0 || idSelStrTable < 0)
				return 0;

			char szBuffer256[256];
			szBuffer256[0] = 0;
			level_editor::pComboBoxGameConnectionsEvent->getItemText(256, szBuffer256);
			level_editor::pListViewGameConnections->setItemText(szBuffer256, 0, idSelStrTable);
			return SXLevelEditor_EditGameConnections_Enter(hWnd, uiMsg, wParam, lParam);
		}
		else if (level_editor::pComboBoxGameConnectionsAction->getHWND() == hElement)
		{
			ID idSelListBoxList = level_editor::pListBoxList->getSel();
			ID idSelStrTable = level_editor::pListViewGameConnections->getSelString();

			if (idSelListBoxList < 0 || idSelStrTable < 0)
				return 0;

			char szBuffer256[256];
			szBuffer256[0] = 0;
			level_editor::pComboBoxGameConnectionsAction->getItemText(level_editor::pComboBoxGameConnectionsAction->getSel(), szBuffer256);
			level_editor::pListViewGameConnections->setItemText(szBuffer256, 2, idSelStrTable);
			return SXLevelEditor_EditGameConnections_Enter(hWnd, uiMsg, wParam, lParam);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_GroupBoxList_CallWmCommand(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int iNotification = HIWORD(wParam);
	HWND hElement = (HWND)(lParam);
	if (iNotification == LBN_SELCHANGE)
	{
		if (hElement == level_editor::pListBoxList->getHWND())
			SXLevelEditor_ListBoxList_Click(hWnd, uiMsg, wParam, lParam);
	}

	return 0;
}

//##########################################################################

void SXLevelEditor_Transform(DWORD timeDelta)
{
	static bool isFirstLBMTransform = false;
	static bool isFirstRBMTransform = false;
	static int iNumComponent = -1;

	if (!SSInput_GetKeyState(SIK_LSHIFT))
		return;

	if (level_editor::pRadioButtonGeomPosX->getCheck() || level_editor::pRadioButtonGeomRotX->getCheck() || level_editor::pRadioButtonGeomScaleX->getCheck() || level_editor::pRadioButtonGreenSelX->getCheck())
		iNumComponent = 0;
	else if (level_editor::pRadioButtonGeomPosY->getCheck() || level_editor::pRadioButtonGeomRotY->getCheck() || level_editor::pRadioButtonGeomScaleY->getCheck() || level_editor::pRadioButtonGreenSelY->getCheck())
		iNumComponent = 1;
	else if (level_editor::pRadioButtonGeomPosZ->getCheck() || level_editor::pRadioButtonGeomRotZ->getCheck() || level_editor::pRadioButtonGeomScaleZ->getCheck() || level_editor::pRadioButtonGreenSelZ->getCheck())
		iNumComponent = 2;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_GetCountModels() <= 0)
			return;

		int iSelected = level_editor::pListBoxList->getSel();

		float3 vTransform;
		ISXGUIEdit *pEditX = 0, *pEditY = 0, *pEditZ = 0;
		int iTypeTransform = -1;

		//если отмечена радиокнопка на позиции
		if (level_editor::pRadioButtonGeomPosX->getCheck() || level_editor::pRadioButtonGeomPosY->getCheck() || level_editor::pRadioButtonGeomPosZ->getCheck())
		{
			vTransform = *(SGeom_ModelGetPosition(iSelected));

			pEditX = level_editor::pEditGeomPosX;
			pEditY = level_editor::pEditGeomPosY;
			pEditZ = level_editor::pEditGeomPosZ;
			iTypeTransform = 0;
		}
		//если отмечена радиокнопка на поворотах
		else if (level_editor::pRadioButtonGeomRotX->getCheck() || level_editor::pRadioButtonGeomRotY->getCheck() || level_editor::pRadioButtonGeomRotZ->getCheck())
		{
			vTransform = *(SGeom_ModelGetRotation(iSelected));

			pEditX = level_editor::pEditGeomRotX;
			pEditY = level_editor::pEditGeomRotY;
			pEditZ = level_editor::pEditGeomRotZ;
			iTypeTransform = 1;
		}
		//если отмечена радиокнопка на масштабировании
		else if (level_editor::pRadioButtonGeomScaleX->getCheck() || level_editor::pRadioButtonGeomScaleY->getCheck() || level_editor::pRadioButtonGeomScaleZ->getCheck())
		{
			vTransform = *(SGeom_ModelGetScale(iSelected));

			pEditX = level_editor::pEditGeomScaleX;
			pEditY = level_editor::pEditGeomScaleY;
			pEditZ = level_editor::pEditGeomScaleZ;
			iTypeTransform = 2;
		}
		else
			return;

		//управление стрелками
		if (SSInput_GetKeyState(SIK_UP))
			vTransform[iNumComponent] += timeDelta * 0.001f;
		if (SSInput_GetKeyState(SIK_DOWN))
			vTransform[iNumComponent] -= timeDelta * 0.001f;

		//управление мышью
		if (SSInput_GetKeyState(SIM_LBUTTON))
		{
			if (isFirstRBMTransform)
			{
				RECT oRect;
				GetWindowRect(GetForegroundWindow(), &oRect);
				long lCenterX = (oRect.right + oRect.left) / 2;
				long lCenterY = (oRect.bottom + oRect.top) / 2;
				POINT oPoint;
				GetCursorPos(&oPoint);

				if (lCenterY != UINT(oPoint.y))
					vTransform[iNumComponent] += timeDelta * 0.001f * float(-int(oPoint.y - lCenterY));
			}
			else
				isFirstRBMTransform = true;
			SRender_CentererCursor();
		}
		else
		{
			isFirstLBMTransform = false;
			isFirstRBMTransform = false;
		}

		//обновляем данные в интерфейсе редактора
		char szStr[32];

		sprintf(szStr, "%f", vTransform.x);
		pEditX->setText(szStr);

		sprintf(szStr, "%f", vTransform.y);
		pEditY->setText(szStr);

		sprintf(szStr, "%f", vTransform.z);
		pEditZ->setText(szStr);

		//обновляем трансформацию
		if (iTypeTransform == 0)
			SGeom_ModelSetPosition(iSelected, &vTransform);
		else if (iTypeTransform == 1)
			SGeom_ModelSetRotation(iSelected, &vTransform);
		else if (iTypeTransform == 2)
			SGeom_ModelSetScale(iSelected, &vTransform);
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN &&
		(
		(level_editor::pComboBoxGreenSel->getSel() == 0 && level_editor::idActiveElement >= 0 && level_editor::idActiveGreenSplit >= 0 && level_editor::idActiveGreenObject >= 0) ||
		(level_editor::pComboBoxGreenSel->getSel() == 2)
		)
		)
	{
		if (SGreen_GetCount() <= 0)
			return;

		int iSelected = level_editor::pListBoxList->getSel();
		if (level_editor::pRadioButtonGreenSelX->getCheck() || level_editor::pRadioButtonGreenSelY->getCheck() || level_editor::pRadioButtonGreenSelZ->getCheck())
		{
			float3 pos;
			if (level_editor::pComboBoxGreenSel->getSel() == 2)
				pos = level_editor::vGreenBoxWHD;
			else
			{
				float3_t pos2;
				SGreen_GetPosObject(level_editor::idActiveElement, level_editor::idActiveGreenSplit, level_editor::idActiveGreenObject, &pos2);
				pos = pos2;
			}

			if (SSInput_GetKeyState(SIK_UP))
				pos[iNumComponent] += float(timeDelta) * 0.001f;
			if (SSInput_GetKeyState(SIK_DOWN))
				pos[iNumComponent] -= float(timeDelta) * 0.001f;

			if (SSInput_GetKeyState(SIM_LBUTTON))
			{
				if (isFirstRBMTransform)
				{
					RECT rc;
					GetWindowRect(GetForegroundWindow(), &rc);
					UINT cx = (rc.right + rc.left) / 2;
					UINT cy = (rc.bottom + rc.top) / 2;
					POINT p;
					GetCursorPos(&p);
					POINT centr;
					centr.x = cx; centr.y = cy;

					if (cy != UINT(p.y))
						pos[iNumComponent] += float(timeDelta) * 0.001f * float(-int(p.y - cy));
				}
				else
					isFirstRBMTransform = true;
				SRender_CentererCursor();
			}
			else
			{
				isFirstLBMTransform = false;
				isFirstRBMTransform = false;
			}

			char tmpPosX[32];
			char tmpPosY[32];
			char tmpPosZ[32];

			sprintf(tmpPosX, "%f", pos.x);
			sprintf(tmpPosY, "%f", pos.y);
			sprintf(tmpPosZ, "%f", pos.z);

			level_editor::pEditGreenSelX->setText(tmpPosX);
			level_editor::pEditGreenSelY->setText(tmpPosY);
			level_editor::pEditGreenSelZ->setText(tmpPosZ);

			if (level_editor::pComboBoxGreenSel->getSel() == 2)
				level_editor::vGreenBoxWHD = pos;
			else
				SGreen_SetPosObject(level_editor::idActiveElement, &level_editor::idActiveGreenSplit, &level_editor::idActiveGreenObject, &float3_t(pos));
		}
	}

	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		float fBiasY = 0.f;

		if (SSInput_GetKeyState(SIK_UP))
			fBiasY += float(timeDelta) * 0.001f;
		if (SSInput_GetKeyState(SIK_DOWN))
			fBiasY -= float(timeDelta) * 0.001f;

		//управление мышью
		if (SSInput_GetKeyState(SIM_LBUTTON))
		{
			if (isFirstRBMTransform)
			{
				RECT oRect;
				GetWindowRect(GetForegroundWindow(), &oRect);
				long lCenterX = (oRect.right + oRect.left) / 2;
				long lCenterY = (oRect.bottom + oRect.top) / 2;
				POINT oPoint;
				GetCursorPos(&oPoint);

				if (lCenterY != UINT(oPoint.y))
					fBiasY += timeDelta * 0.001f * float(-int(oPoint.y - lCenterY));
			}
			else
				isFirstRBMTransform = true;
			SRender_CentererCursor();
		}
		else
		{
			isFirstLBMTransform = false;
			isFirstRBMTransform = false;
		}

		SAIG_QuadSelectedAddPosY(fBiasY);
	}
}