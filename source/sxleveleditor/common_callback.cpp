
#include "common_callback.h"

bool HandlerPreviewLevel(const char *szPath, char *szBuff)
{
	String sPathImg = FileAppendSlash(szPath) + "preview.bmp";
	if (FileExistsFile(sPathImg.c_str()))
	{
		sprintf(szBuff, "%s", sPathImg.c_str());
		return true;
	}
	return false;
}

LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Exit?", "Exit?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hwnd, msg, wParam, lParam);

	return 0;
}

void level_editor::LevelNew(bool mess)
{
	if (mess && (SGeom_GetCountModels() <= 0 || (SGeom_GetCountModels() > 0 && MessageBox(0, "Are you sure you need to create a new level?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO)))
		return;

	SLevel_Clear();
	char tmpcaption[256];
	sprintf(tmpcaption, "%s: new level ** ", "level_editor");
	level_editor::pJobWindow->setText(tmpcaption);
	level_editor::iActiveGroupType = 0;
	level_editor::idActiveElement = -1;
	level_editor::idActiveGreenSplit = -1;
	level_editor::idActiveGreenObject = -1;

	ID gid = SLight_GetGlobal();
	if (gid >= 0)
		SLight_DeleteLight(gid);

	level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
	level_editor::pCheckBoxTBLevelType->setCheck(false);
	level_editor::pCheckBoxTBGLightEnable->setCheck(false);
}

void level_editor::LevelOpen()
{
	if (SGeom_GetCountModels() > 0 && MessageBox(0, "Are you sure that you need to open the level?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO)
		return;

	level_editor::LevelNew(false);

	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), FILE_FILTER_LEVEL);
	//char szSelName[MAX_PATH];
	//char szSelPath[2014];
	gui_func::dialogs::SelectDirOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), "Open level", false, false, 0, HandlerPreviewLevel);
	
	if (STR_VALIDATE(tmppath))
	{
		//StrCutNameNEx(tmppath, tmpname);
		SLevel_Load(tmpname, false);
		char tmpcaption[256];
		sprintf(tmpcaption, "%s: %s", "level_editor", tmpname);
		level_editor::pJobWindow->setText(tmpcaption);

		ID gid = SLight_GetGlobal();
		if (gid >= 0)
		{
			level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP26);
			level_editor::pCheckBoxTBLevelType->setCheck(true);
			level_editor::pCheckBoxTBGLightEnable->setCheck(true);
		}
		else
		{
			level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
			level_editor::pCheckBoxTBLevelType->setCheck(false);
			level_editor::pCheckBoxTBGLightEnable->setCheck(false);
		}
	}
}

void level_editor::LevelSave()
{
	if (SLevel_GetName()[0])
		SLevel_Save(SLevel_GetName());
	else
	{
		if (SGeom_GetCountModels() <= 0)
			MessageBox(0, "You need to create a level!", 0, 0);
		else
			level_editor::LevelSaveAs();
	}
}

void level_editor::LevelSaveAs()
{
	if (SGeom_GetCountModels() <= 0)
	{
		MessageBox(0, "You need to create a level!", 0, 0);
		return;
	}

	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_SAVE, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), FILE_FILTER_LEVEL);
	gui_func::dialogs::SelectDirOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), "Save level", false, true, 0/*, HandlerPreviewLevel*/);
	if (STR_VALIDATE(tmppath))
	{
		//StrCutNameNEx(tmppath, tmpname);
		SLevel_Save(tmpname);
		char tmpcaption[256];
		sprintf(tmpcaption, "%s: %s", "level_editor", tmpname);
		level_editor::pJobWindow->setText(tmpcaption);
	}
}

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		return TrueExit(hwnd, msg, wParam, lParam);
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
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		level_editor::FinalImageUncheckedMenu();
		level_editor::pMainMenu->setCheckItem(id, !level_editor::pMainMenu->getCheckedItem(id));
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
			char tmppath[1024];
			sprintf(tmppath, "%sweather\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_CONFIGS), level_editor::aMenuWeather[level_editor::idMenuWeatherCurr]);
			SLevel_WeatherLoad(tmppath);
		}
		else
		{
			SLevel_WeatherLoad(0);

			ID gid = SLight_GetGlobal();
			if (gid >= 0)
			{
				SLight_SetPos(gid, &float3(60, 60, 0), false);
				SLight_SetColor(gid, &float3(1, 1, 1));
				SLight_SetEnable(gid, true);
			}
		}
	}

	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int *r_resize = (int*)GET_PCVAR_INT("r_resize");

	if (!r_resize)
		r_resize = (int*)GET_PCVAR_INT("r_resize");

	if (!r_resize)
		return 0;

	*r_resize = RENDER_RESIZE_RESIZE;
	return 0;
}

///////

LRESULT SXLevelEditor_RenderWindow_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	level_editor::pAxesHelper->onMouseMove(((int)(short)LOWORD(lParam)), ((int)(short)HIWORD(lParam)));

	if (level_editor::pAxesHelper->m_bIsDragging == false)
		return 0;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM && level_editor::idActiveElement >= 0)
	{
		if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_MOVE)
		{
			const float3* pos = SGeom_ModelGetPosition(level_editor::idActiveElement);
			float3 npos = (*pos) + (level_editor::pAxesHelper->getPosition() - level_editor::vHelperPos);
			if ((*pos).x != npos.x || (*pos).y != npos.y || (*pos).z != npos.z)
			{
				//*pos = npos;
				SGeom_ModelSetPosition(level_editor::idActiveElement, &npos);
				//SGeom_ModelsMApplyTransform(level_editor::idActiveElement);
				float3 min, max;
				SGeom_ModelGetMinMax(level_editor::idActiveElement, &min, &max);

				level_editor::vHelperPos = (max + min) * 0.5f;
			}
		}
		else if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_ROTATE)
		{
			const float3* rot = SGeom_ModelGetRotation(level_editor::idActiveElement);
			float3 nrot = level_editor::pAxesHelper->getRotation();
			if ((*rot).x != nrot.x || (*rot).y != nrot.y || (*rot).z != nrot.z)
			{
				//*rot = level_editor::pAxesHelper->getRotation();
				//SGeom_ModelsMApplyTransform(level_editor::idActiveElement);
				SGeom_ModelSetRotation(level_editor::idActiveElement, &nrot);
			}
		}
		else if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_SCALE)
		{
			const float3* scale = SGeom_ModelGetScale(level_editor::idActiveElement);
			float3 nscale = level_editor::pAxesHelper->getScale() - float3(1,1,1);
			if ((*scale).x != nscale.x || (*scale).y != nscale.y || (*scale).z != nscale.z)
			{
				//*scale += nscale;
				//SGeom_ModelsMApplyTransform(level_editor::idActiveElement);
				SGeom_ModelSetScale(level_editor::idActiveElement, &nscale);
			}
		}
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0 && level_editor::idActiveGreenSplit >= 0 && level_editor::idActiveGreenObject >= 0)
	{
		if (level_editor::pComboBoxGreenSel->getSel() == 0)
		{
			float3_t pos;
			SGreen_GetPosObject(level_editor::idActiveElement, level_editor::idActiveGreenSplit, level_editor::idActiveGreenObject, &pos);
			float3 helperpos = level_editor::pAxesHelper->getPosition();
			if (pos.x != helperpos.x || pos.y != helperpos.y || pos.z != helperpos.z)
			{
				SGreen_SetPosObject(level_editor::idActiveElement, &level_editor::idActiveGreenSplit, &level_editor::idActiveGreenObject, &(float3_t)helperpos);
			}
		}
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && level_editor::idActiveElement >= 0)
	{
		CBaseEntity* bEnt = SXGame_EntGet(level_editor::idActiveElement);
		if (!bEnt)
			return 0;

		if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_MOVE)
			bEnt->setPos(level_editor::pAxesHelper->getPosition());
		else if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_ROTATE)
			bEnt->setOrient(level_editor::pAxesHelper->getRotationQ());

		level_editor::GameUpdatePosRot();
	}
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_LDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;
	level_editor::pAxesHelper->m_bIsDragging = true;
	level_editor::pAxesHelper->m_bIsDraggingStart = true;
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_LClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	level_editor::pAxesHelper->m_bIsDragging = false;
	level_editor::pAxesHelper->m_bIsDraggingStop = true;

	/*if (!SSInput_GetKeyState(SIK_LALT))
		return 0;*/

	static float4x4 mCamView, mCamProj;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mCamView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mCamProj);

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	float3 _res;
	float3_t pos2;
	ID idmodel;
	ID idmtl;

	float3 camDir;
	float det;
	SMMATRIX mat = SMMatrixInverse(&det, mCamView);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(SRender_GetHandleWin3D(), &pt);

	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	float3 pos = float3(
		(2.0f * (float)pt.x / float(*r_win_width) - 1.0f) / mCamProj._11,
		-(2.0f * (float)pt.y / float(*r_win_height) - 1.0f) / mCamProj._22,
		1.0f
		) * mat;
	camDir = pos - vCamPos;

	if (!level_editor::pCheckBoxTBArrow->getCheck())
		return 0;

	level_editor::idMtl = -1;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || level_editor::iActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_TraceBeam(&vCamPos, &camDir, &_res, &idmodel, &idmtl))
		{
			level_editor::pListBoxList->setSel(idmodel);
			level_editor::GeomSel(idmodel);
			level_editor::idMtl = idmtl;
		}
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0)
	{
		ID idgreen;
		ID idsplit;
		ID idobj;

		if (level_editor::pComboBoxGreenSel->getSel() == 0)
		{
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGreen_TraceBeam(&vCamPos, &camDir, &_res, &idgreen, &idsplit, &idobj, &idmtl))
			{
				level_editor::idActiveGreenSplit = idsplit;
				level_editor::idActiveGreenObject = idobj;
				SGreen_GetPosObject(idgreen, idsplit, idobj, &pos2);

				level_editor::pStaticGreenSelX->setText("Pos X:");
				level_editor::pStaticGreenSelY->setText("Pos Y:");
				level_editor::pStaticGreenSelZ->setText("Pos Z:");

				level_editor::pEditGreenSelX->setText(String(pos2.x).c_str());
				level_editor::pEditGreenSelY->setText(String(pos2.y).c_str());
				level_editor::pEditGreenSelZ->setText(String(pos2.z).c_str());
				
				level_editor::pAxesHelper->setPosition(pos2);
				level_editor::pAxesHelper->setRotation(float3(0,0,0));
				level_editor::pAxesHelper->setScale(float3(1, 1, 1));

				level_editor::idMtl = idmtl;
			}
		}
		else if (level_editor::pComboBoxGreenSel->getSel() == 1)
		{
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGeom_TraceBeam(&vCamPos, &camDir, &_res, &idmodel, &idmtl))
			{
				idobj = SGreen_AddObject(level_editor::idActiveElement, &_res, &idsplit);
				level_editor::idActiveGreenSplit = idsplit;
				level_editor::idActiveGreenObject = idobj;

				level_editor::pStaticGreenSelX->setText("Pos X:");
				level_editor::pStaticGreenSelY->setText("Pos Y:");
				level_editor::pStaticGreenSelZ->setText("Pos Z:");

				level_editor::pEditGreenSelX->setText(String(_res.x).c_str());
				level_editor::pEditGreenSelY->setText(String(_res.y).c_str());
				level_editor::pEditGreenSelZ->setText(String(_res.z).c_str());

				level_editor::pAxesHelper->setPosition(pos2);
				level_editor::pAxesHelper->setRotation(float3(0, 0, 0));
				level_editor::pAxesHelper->setScale(float3(1, 1, 1));
				
				int qwert = 0;
			}
		}

		else if (level_editor::pComboBoxGreenSel->getSel() == 2)
		{
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGeom_TraceBeam(&vCamPos, &camDir, &_res, &idmodel, &idmtl))
			{
				level_editor::vGreenBoxPos = _res;
				int density = level_editor::pTrackBarGreenDensity->getPos();

				float3 min, max, npos;
				min.x = _res.x - level_editor::vGreenBoxWHD.x * 0.5f;
				min.z = _res.z - level_editor::vGreenBoxWHD.z * 0.5f;

				max.x = _res.x + level_editor::vGreenBoxWHD.x * 0.5f;
				max.z = _res.z + level_editor::vGreenBoxWHD.z * 0.5f;

				npos.y = _res.y + level_editor::vGreenBoxWHD.y;

				for (int i = 0; i < density; ++i)
				{
					npos.x = randf(min.x, max.x);
					npos.z = randf(min.z, max.z);
					if(SGeom_TraceBeam(&npos, &float3(0, -1, 0), &_res, &idmodel, &idmtl))
						SGreen_AddObject(level_editor::idActiveElement, &_res, 0);
				}
			}
		}

		else if (level_editor::pComboBoxGreenSel->getSel() == 3)
		{
			static const int *r_win_width = GET_PCVAR_INT("r_win_width");
			static const int *r_win_height = GET_PCVAR_INT("r_win_height");
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGreen_TraceBeam(&vCamPos, &camDir, &_res, &idgreen, &idsplit, &idobj, &idmtl))
			{
				level_editor::idActiveGreenSplit = -1;
				level_editor::idActiveGreenObject = -1;
				level_editor::pEditGreenSelX->setText("");
				level_editor::pEditGreenSelY->setText("");
				level_editor::pEditGreenSelZ->setText("");
				SGreen_DelObject(idgreen, idsplit, idobj);
				int qwert = 0;
			}
		}
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && level_editor::idActiveElement >= 0)
	{

	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		if (level_editor::pRadioButtonAIQuadAdd->getCheck())
		{
			float3 start = pos;
			float3 dir = camDir;
			float3 end = start + dir * 1000.0f;
			btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
			SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

			if (cb.hasHit())
			{
				SAIG_QuadAdd(&BTVEC_F3(cb.m_hitPointWorld));
			}
		}
		else if (level_editor::pRadioButtonAIQuadsMSel->getCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_QuadSelect(idaq, true);
		}
		else if (level_editor::pRadioButtonAIQuadsSelDel->getCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_QuadDelete(idaq);
		}
		else if (level_editor::pRadioButtonAIGPAdd->getCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_GraphPointAdd(idaq);
		}
		else if (level_editor::pRadioButtonAIGPDel->getCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_GraphPointDelete(idaq);
		}
	}
	
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_RClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	static float4x4 mCamView, mCamProj;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mCamView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mCamProj);

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	float3 camDir;
	float det;
	SMMATRIX mat = SMMatrixInverse(&det, mCamView);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(SRender_GetHandleWin3D(), &pt);

	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	float3 pos = float3(
		(2.0f * (float)pt.x / float(*r_win_width) - 1.0f) / mCamProj._11,
		-(2.0f * (float)pt.y / float(*r_win_height) - 1.0f) / mCamProj._22,
		1.0f
		) * mat;
	camDir = pos - vCamPos;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0)
	{
		float3 _res;
		float3_t pos2;
		ID idmodel;
		ID idmtl;

		ID idgreen;
		ID idsplit;
		ID idobj;

		if (level_editor::pComboBoxGreenSel->getSel() == 2)
		{
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGeom_TraceBeam(&vCamPos, &camDir, &_res, &idmodel, &idmtl))
				level_editor::vGreenBoxPos = _res;
		}
	}

	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		if (level_editor::pRadioButtonAIQuadsMSel->getCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_QuadSelect(idaq, false);
		}
	}

	return 0;
}

LRESULT SXLevelEditor_RenderWindow_MBUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		SAIG_QuadSelect(-1, false);
	}

	return 0;
}

LRESULT SXLevelEditor_RenderWindow_Delete(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

LRESULT SXLevelEditor_ButtonGeometryOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	level_editor::pListBoxList->clear();
	long tmpcountmodel = SGeom_GetCountModels();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	level_editor::pStaticListValCount->setText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	for (int i = 0; i < tmpcountmodel;++i)
	{
		sprintf(tmpnamecountpoly, "%s | %d", SGeom_ModelGetName(i), SGeom_ModelGetCountPoly(i));
		level_editor::pListBoxList->addItem(tmpnamecountpoly);
	}

	level_editor::GreenActivateAll(false);
	level_editor::GameActivateAll(false);
	level_editor::AIGridActivateAll(false);
	level_editor::GeomActivateAll(false);
	level_editor::GeomActivateCreate(true);
	//level_editor::GeomActivateTrans(false);

	level_editor::iActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GEOM;
	level_editor::idActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	level_editor::pListBoxList->clear();
	long tmpcountmodel = SGreen_GetCount();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	level_editor::pStaticListValCount->setText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	for (int i = 0; i < tmpcountmodel; ++i)
	{
		sprintf(tmpnamecountpoly, "%s | %s | %d", 
			SGreen_MGetName(i),
			(SGreen_MGetTypeCountGen(i) == GREEN_TYPE_GRASS ? "grass" : "tree/shrub"),
			SGreen_MGetCountGen(i));
		level_editor::pListBoxList->addItem(tmpnamecountpoly);
	}

	level_editor::GeomActivateAll(false);

	level_editor::GameActivateAll(false);

	level_editor::GreenActivateAll(false);
	level_editor::GreenActivateCreate(true);
	level_editor::AIGridActivateAll(false);
	level_editor::GreenActivateMain(true);
	
	//MCActivateTrans(false);

	level_editor::iActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GREEN;
	level_editor::idActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonGameObjectOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	level_editor::pListBoxList->clear();

	int tmpcoungo = SXGame_EntGetCount();
	int tmpcoungo2 = 0;
	char tmpname[1024];
	for (int i = 0; i < tmpcoungo; ++i)
	{
		CBaseEntity* bEnt = SXGame_EntGet(i);
		if (bEnt)
		{
			sprintf(tmpname, "%s / %s", bEnt->getName(), bEnt->getClassName());
			level_editor::pListBoxList->addItem(tmpname);
			level_editor::pListBoxList->setItemData(level_editor::pListBoxList->getItemCount() - 1, i);
			++tmpcoungo2;
		}
	}

	char tmptextvalcountgo[64];
	sprintf(tmptextvalcountgo, "%d", tmpcoungo2);
	level_editor::pStaticListValCount->setText(tmptextvalcountgo);

	level_editor::GeomActivateAll(false);
	level_editor::GreenActivateAll(false);
	level_editor::AIGridActivateAll(false);
	level_editor::GameActivateAll(true);

	level_editor::iActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GAME;
	level_editor::idActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

LRESULT SXLevelEditor_ListBoxList_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

LRESULT SXLevelEditor_ButtonDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = level_editor::pListBoxList->getSel();

	if (sel < 0)
		return 0;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_GetCountModels() > 0 && sel < SGeom_GetCountModels())
		{
			SGeom_ModelDelete(sel);
			level_editor::pListBoxList->deleteItem(sel);
			if (SGeom_GetCountModels() > 0)
			{
				if (sel > 0)
				{
					if (SGeom_GetCountModels() <= sel)
						sel -= 1;
				}
				level_editor::pListBoxList->setSel(sel);
				level_editor::GeomSel(sel);
			}
			else
			{
				SXLevelEditor_ButtonGeometryOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		if (SGreen_GetCount() > 0 && sel < SGreen_GetCount())
		{
			SGreen_DelGreen(sel);
			level_editor::pListBoxList->deleteItem(sel);
			if (SGreen_GetCount() > 0)
			{
				if (sel > 0)
				{
					if (SGreen_GetCount() <= sel)
						sel -= 1;
				}
				level_editor::pListBoxList->setSel(sel);
				level_editor::GreenSel(sel);
			}
			else
			{
				SXLevelEditor_ButtonGreenOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
	{
		if (SXGame_EntGetCount() > 0 && sel < SXGame_EntGetCount())
		{
			SXGame_RemoveEntity(SXGame_EntGet(level_editor::pListBoxList->getItemData(sel)));
			SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);

			if (sel >= level_editor::pListBoxList->getItemCount())
				--sel;
			if (sel >= 0)
			{
				level_editor::pListBoxList->setSel(sel);
				level_editor::GameSel(sel);
			}
		}
	}

	return 0;
}





LRESULT SXLevelEditor_GroupBox_Notify(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

LRESULT SXLevelEditor_ToolBar1_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (level_editor::pCheckBoxTBArrow->getHWND() == handle_elem)
		{
			level_editor::pCheckBoxTBPos->setCheck(false);
			level_editor::pCheckBoxTBRot->setCheck(false);
			level_editor::pCheckBoxTBScale->setCheck(false);
			level_editor::pAxesHelper->setType(CAxesHelper::HANDLER_TYPE_NONE);
		}
		else if (level_editor::pCheckBoxTBPos->getHWND() == handle_elem)
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
		else if (level_editor::pCheckBoxTBRot->getHWND() == handle_elem)
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
		else if (level_editor::pCheckBoxTBScale->getHWND() == handle_elem)
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

		else if (level_editor::pButtonTBNew->getHWND() == handle_elem)
		{
			level_editor::LevelNew(true);
		}
		else if (level_editor::pButtonTBOpen->getHWND() == handle_elem)
		{
			level_editor::LevelOpen();
		}
		else if (level_editor::pButtonTBSave->getHWND() == handle_elem)
		{
			level_editor::LevelSave();
		}
		else if (level_editor::pButtonTBSaveAs->getHWND() == handle_elem)
		{
			level_editor::LevelSaveAs();
		}

		else if (level_editor::pCheckBoxTBGrid->getHWND() == handle_elem)
		{
			level_editor::pMainMenu->setCheckItem(ID_VIEW_GRID, level_editor::pCheckBoxTBGrid->getCheck());
			SRender_EditorSetRenderGrid(level_editor::pCheckBoxTBGrid->getCheck());
		}
		else if (level_editor::pCheckBoxTBAxes->getHWND() == handle_elem)
		{
			level_editor::pMainMenu->setCheckItem(ID_VIEW_AXES, level_editor::pCheckBoxTBAxes->getCheck());
			SRender_EditorSetRenderAxesStatic(level_editor::pCheckBoxTBAxes->getCheck());
		}

		else if (level_editor::pCheckBoxTBRColor->getHWND() == handle_elem)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_COLOR, true);
			level_editor::pCheckBoxTBRColor->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		}
		else if (level_editor::pCheckBoxTBRNormal->getHWND() == handle_elem)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, true);
			level_editor::pCheckBoxTBRNormal->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		}
		else if (level_editor::pCheckBoxTBRParam->getHWND() == handle_elem)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, true);
			level_editor::pCheckBoxTBRParam->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		}
		else if (level_editor::pCheckBoxTBRAmDiff->getHWND() == handle_elem)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			level_editor::pCheckBoxTBRAmDiff->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		}
		else if (level_editor::pCheckBoxTBRSpecular->getHWND() == handle_elem)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, true);
			level_editor::pCheckBoxTBRSpecular->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		}
		else if (level_editor::pCheckBoxTBRLighting->getHWND() == handle_elem)
		{
			level_editor::FinalImageUncheckedMenu();
			level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			level_editor::pCheckBoxTBRLighting->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		}

		else if (level_editor::pCheckBoxTBSelS->getHWND() == handle_elem)
		{
			level_editor::canSelSelection = level_editor::pCheckBoxTBSelS->getCheck();
			level_editor::pMainMenu->setCheckItem(ID_SELECTIONSETTINGS_SELECTION, level_editor::pCheckBoxTBSelS->getCheck());
		}
		else if (level_editor::pCheckBoxTBSelZTest->getHWND() == handle_elem)
		{
			level_editor::canSelZTest = level_editor::pCheckBoxTBSelZTest->getCheck();
			level_editor::pMainMenu->setCheckItem(ID_SELECTIONSETTINGS_ZTEST, level_editor::pCheckBoxTBSelZTest->getCheck());
		}
		else if (level_editor::pCheckBoxTBSelMesh->getHWND() == handle_elem)
		{
			level_editor::canSelMesh = level_editor::pCheckBoxTBSelMesh->getCheck();
			level_editor::pMainMenu->setCheckItem(ID_SELECTIONSETTINGS_MESH, level_editor::pCheckBoxTBSelMesh->getCheck());
		}
		else if (level_editor::pCheckBoxTBSelCullBack->getHWND() == handle_elem)
		{
			level_editor::canSelBackFacesCull = level_editor::pCheckBoxTBSelCullBack->getCheck();
			level_editor::pMainMenu->setCheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, level_editor::pCheckBoxTBSelCullBack->getCheck());
		}

		else if (level_editor::pCheckBoxTBAIGBound->getHWND() == handle_elem)
		{
			level_editor::canAIGBound = level_editor::pCheckBoxTBAIGBound->getCheck();
		}
		else if (level_editor::pCheckBoxTBAIGQuad->getHWND() == handle_elem)
		{
			level_editor::canAIGQuad = level_editor::pCheckBoxTBAIGQuad->getCheck();
		}
		else if (level_editor::pCheckBoxTBAIGGraphPoint->getHWND() == handle_elem)
		{
			level_editor::canAIGGraphPoint = level_editor::pCheckBoxTBAIGGraphPoint->getCheck();
		}

		else if (level_editor::pCheckBoxTBLevelType->getHWND() == handle_elem)
		{
			if (level_editor::pCheckBoxTBLevelType->getCheck())
			{
				ID gid = SLight_GetGlobal();
				if (gid < 0)
				{
					gid = SLight_CreatePoint(
						&float3(60, 60, 0),
						0,
						&float3(1, 1, 1),
						true,
						true);
					SLight_SetEnable(gid, level_editor::pCheckBoxTBGLightEnable->getCheck());
					SLight_SetName(gid, "sun");
				}
				level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP26);
			}
			else
			{
				ID gid = SLight_GetGlobal();
				if (gid >= 0)
					SLight_DeleteLight(gid);
				level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
				level_editor::pCheckBoxTBGLightEnable->setCheck(false);
			}
		}

		else if (level_editor::pCheckBoxTBGLightEnable->getHWND() == handle_elem)
		{
			ID gid = SLight_GetGlobal();
			if (gid >= 0)
				SLight_SetEnable(gid, level_editor::pCheckBoxTBGLightEnable->getCheck());
			else
				level_editor::pCheckBoxTBGLightEnable->setCheck(false);
		}
		
	}

	return 0;
}

//int recurion = 0;

LRESULT SXLevelEditor_GroupBox_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (handle_elem == level_editor::pCheckBoxAIGridMarkedSplits->getHWND())
			SAIG_GridSetMarkSplits(level_editor::pCheckBoxAIGridMarkedSplits->getCheck());
		else
		{
			ID seldata = level_editor::pListBoxList->getItemData(level_editor::pListBoxList->getSel());
			CBaseEntity* bEnt = SXGame_EntGet(seldata);
			int str = level_editor::pListViewGameClass->getSelString();

			if (seldata < 0 || str < 0)
				return 0;
			char txtval[256];
			level_editor::pListViewGameClass->getItemText(txtval, 1, str, 256);
			UINT uiFlags;
			sscanf(txtval, "%d", &uiFlags);

			for (int i = 0; i < 16; ++i)
			{
				if (level_editor::pCheckBoxGameFlags[i]->getHWND() == handle_elem)
				{
					if (level_editor::pCheckBoxGameFlags[i]->getCheck())
						uiFlags |= (1 << (i + 16));
					else
						uiFlags ^= (1 << (i + 16));
				}
			}

			sprintf(txtval, "%d", uiFlags);
			level_editor::pListViewGameClass->setItemText(txtval, 1, str);
			bEnt->setKV("flags", txtval);
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		if (level_editor::pComboBoxGameValue->getHWND() == handle_elem)
		{
			int sel = level_editor::pListBoxList->getSel();
			char txt[256];
			level_editor::pComboBoxGameValue->getItemText(level_editor::pComboBoxGameValue->getSel(), txt);
			level_editor::pListViewGameClass->setItemText(txt, 1, level_editor::pListViewGameClass->getSelString());
			CBaseEntity* bEnt = SXGame_EntGet(level_editor::pListBoxList->getItemData(sel));
			if (bEnt)
			{
				propdata_t* pd = (propdata_t*)level_editor::pListViewGameClass->getItemData(level_editor::pListViewGameClass->getSelString());
				level_editor::pComboBoxGameValue->getItemData(level_editor::pComboBoxGameValue->getSel());
				bEnt->setKV(pd->szKey, (const char*)level_editor::pComboBoxGameValue->getItemData(level_editor::pComboBoxGameValue->getSel()));
			}
		}
		else if (level_editor::pComboBoxGameClass->getHWND() == handle_elem)
		{
			if (level_editor::pComboBoxGameClass->getSel() == 0)
				return 0;

			int sel = level_editor::pListBoxList->getSel();

			if (sel < 0)
				return 0;

			CBaseEntity* bEnt = SXGame_EntGet(level_editor::pListBoxList->getItemData(sel));
			proptable_t* pt = SXGame_EntGetProptable(bEnt->getClassName());

			propdata_t* pd;
			char txtkey[256];
			char txtval[256];

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

			Array<KeyVal> tmparrdata;

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
						bEnt->getKV(pd->szKey, txtval, 256);
						tmparrdata.push_back(KeyVal(pd->szKey, txtval));
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
					tmparrdata.push_back(KeyVal(pd->szKey, txtval));
				}
			}

			char txt[256];
			level_editor::pComboBoxGameClass->getItemText(level_editor::pComboBoxGameClass->getSel(), txt);
			SXGame_RemoveEntity(bEnt);

			bEnt = SXGame_CreateEntity(txt);
			bEnt->setFlags(bEnt->getFlags() | EF_EXPORT | EF_LEVEL);


			for(int i = 0; i < tmparrdata.size(); ++i)
			{
				bEnt->setKV(tmparrdata[i].key.c_str(), tmparrdata[i].val.c_str());
			}

			SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);
			sel = -1;
			for (int i = 0; i < level_editor::pListBoxList->getItemCount(); ++i)
			{
				if(level_editor::pListBoxList->getItemData(i) == bEnt->getId())
				{
					sel = i;
					break;
				}
			}

			if(sel < 0)
			{

			}

			level_editor::pListBoxList->setSel(sel);
			level_editor::GameSel(sel);
		}
		else if (level_editor::pComboBoxGreenSel->getHWND() == handle_elem)
		{
			int sel = level_editor::pComboBoxGreenSel->getSel();
			level_editor::canGreenRenderBox = false;
			
			if (sel == 0)
			{
				level_editor::pStaticGreenSelX->setText("Position X:");
				level_editor::pStaticGreenSelY->setText("Position Y:");
				level_editor::pStaticGreenSelZ->setText("Position Z:");
				level_editor::pEditGreenSelX->setText("");
				level_editor::pEditGreenSelY->setText("");
				level_editor::pEditGreenSelZ->setText("");
			}
			else if (sel == 2)
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

		else if (level_editor::pComboBoxGameConnectionsEvent->getHWND() == handle_elem)
		{
			ID idSelListBoxList = level_editor::pListBoxList->getSel();
			ID idSelStrTable = level_editor::pListViewGameConnections->getSelString();

			if (idSelListBoxList < 0 || idSelStrTable < 0)
				return 0;

			char szBuffer256[256];
			szBuffer256[0] = 0;
			level_editor::pComboBoxGameConnectionsEvent->getItemText(256, szBuffer256);
			level_editor::pListViewGameConnections->setItemText(szBuffer256, 0, idSelStrTable);
			return SXLevelEditor_EditGameConnections_Enter(hwnd, msg, wParam, lParam);
		}
		else if (level_editor::pComboBoxGameConnectionsAction->getHWND() == handle_elem)
		{
			ID idSelListBoxList = level_editor::pListBoxList->getSel();
			ID idSelStrTable = level_editor::pListViewGameConnections->getSelString();

			if (idSelListBoxList < 0 || idSelStrTable < 0)
				return 0;

			char szBuffer256[256];
			szBuffer256[0] = 0;
			level_editor::pComboBoxGameConnectionsAction->getItemText(level_editor::pComboBoxGameConnectionsAction->getSel(), szBuffer256);
			level_editor::pListViewGameConnections->setItemText(szBuffer256, 2, idSelStrTable);
			return SXLevelEditor_EditGameConnections_Enter(hwnd, msg, wParam, lParam);
		}
	}
	/*else if (Notification == EN_CHANGE)
	{
		if (level_editor::pEditGameConnectionsACValue->getHWND() == handle_elem)
		{
			static int recurion = 0;
			if (recurion > 0)
			{
				recurion = 0;
				return 0;
			}
				
			
			char edit_text[256];
			level_editor::pEditGameConnectionsACValue->GetText(edit_text, 256);
			if (edit_text[0] == 0)
				return 0;

			//if (!(isalpha(wParam) || isdigit(wParam) || (char)wParam == '_'))
				//return 0;

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
							++recurion;
							level_editor::pEditGameConnectionsACValue->setText(lower_name);
							PostMessage(level_editor::pEditGameConnectionsACValue->getHWND(), EM_SETSEL, strlen(edit_text), strlen(lower_name));
							return 0;
						}
					}
				}
			}
		}
	}*/

	return 0;
}

LRESULT SXLevelEditor_GroupBoxList_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == LBN_SELCHANGE)
	{
		if (handle_elem == level_editor::pListBoxList->getHWND())
			SXLevelEditor_ListBoxList_Click(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void SXLevelEditor_Transform(DWORD timeDelta)
{
	static bool IsSFirstLBMTransform = false;
	static bool IsSFirstRBMTransform = false;
	static int CoordinateTransformation = -1;

	if (level_editor::pRadioButtonGeomPosX->getCheck() || level_editor::pRadioButtonGeomRotX->getCheck() || level_editor::pRadioButtonGeomScaleX->getCheck() || level_editor::pRadioButtonGreenSelX->getCheck())
		CoordinateTransformation = 0;
	else if (level_editor::pRadioButtonGeomPosY->getCheck() || level_editor::pRadioButtonGeomRotY->getCheck() || level_editor::pRadioButtonGeomScaleY->getCheck() || level_editor::pRadioButtonGreenSelY->getCheck())
		CoordinateTransformation = 1;
	else if (level_editor::pRadioButtonGeomPosZ->getCheck() || level_editor::pRadioButtonGeomRotZ->getCheck() || level_editor::pRadioButtonGeomScaleZ->getCheck() || level_editor::pRadioButtonGreenSelZ->getCheck())
		CoordinateTransformation = 2;

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SSInput_GetKeyState(SIK_LSHIFT) && SGeom_GetCountModels() > 0)
		{
			DWORD selmodel = level_editor::pListBoxList->getSel();
			if (level_editor::pRadioButtonGeomPosX->getCheck() || level_editor::pRadioButtonGeomPosY->getCheck() || level_editor::pRadioButtonGeomPosZ->getCheck())
			{
				float3 pos = *(SGeom_ModelGetPosition(selmodel));

				if (SSInput_GetKeyState(SIK_UP))
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (SSInput_GetKeyState(SIK_DOWN))
					pos[CoordinateTransformation] -= timeDelta * 0.001f;
				if (SSInput_GetKeyState(SIM_LBUTTON))
				{
					if (IsSFirstRBMTransform)
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
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					SRender_CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				level_editor::pEditGeomPosX->setText(tmpPosX);
				level_editor::pEditGeomPosY->setText(tmpPosY);
				level_editor::pEditGeomPosZ->setText(tmpPosZ);

				SGeom_ModelSetPosition(selmodel, &pos);
			}

			else if (level_editor::pRadioButtonGeomRotX->getCheck() || level_editor::pRadioButtonGeomRotY->getCheck() || level_editor::pRadioButtonGeomRotZ->getCheck())
			{
				float3 pos = *(SGeom_ModelGetRotation(selmodel));

				if (SSInput_GetKeyState(SIK_UP))
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (SSInput_GetKeyState(SIK_DOWN))
					pos[CoordinateTransformation] -= timeDelta * 0.001f;

				if (SSInput_GetKeyState(SIM_LBUTTON))
				{
					if (IsSFirstRBMTransform)
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
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					SRender_CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				level_editor::pEditGeomRotX->setText(tmpPosX);
				level_editor::pEditGeomRotY->setText(tmpPosY);
				level_editor::pEditGeomRotZ->setText(tmpPosZ);

				SGeom_ModelSetRotation(selmodel, &pos);
			}

			else if (level_editor::pRadioButtonGeomScaleX->getCheck() || level_editor::pRadioButtonGeomScaleY->getCheck() || level_editor::pRadioButtonGeomScaleZ->getCheck())
			{
				float3 pos = *(SGeom_ModelGetScale(selmodel));

				if (SSInput_GetKeyState(SIK_UP))
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (SSInput_GetKeyState(SIK_DOWN))
					pos[CoordinateTransformation] -= timeDelta * 0.001f;

				if (SSInput_GetKeyState(SIM_LBUTTON))
				{
					if (IsSFirstRBMTransform)
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
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					SRender_CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				level_editor::pEditGeomScaleX->setText(tmpPosX);
				level_editor::pEditGeomScaleY->setText(tmpPosY);
				level_editor::pEditGeomScaleZ->setText(tmpPosZ);

				SGeom_ModelSetScale(selmodel, &pos);
			}
		}
	}
	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN &&
			(
			(level_editor::pComboBoxGreenSel->getSel() == 0 && level_editor::idActiveElement >= 0 && level_editor::idActiveGreenSplit >= 0 && level_editor::idActiveGreenObject >= 0) ||
			(level_editor::pComboBoxGreenSel->getSel() == 2)
			)
		)
	{
		if (SSInput_GetKeyState(SIK_LSHIFT) && SGreen_GetCount() > 0)
		{
			DWORD selmodel = level_editor::pListBoxList->getSel();
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
					pos[CoordinateTransformation] += float(timeDelta) * 0.001f;
				if (SSInput_GetKeyState(SIK_DOWN))
					pos[CoordinateTransformation] -= float(timeDelta) * 0.001f;

				if (SSInput_GetKeyState(SIM_LBUTTON))
				{
					if (IsSFirstRBMTransform)
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
							pos[CoordinateTransformation] += float(timeDelta) * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					SRender_CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
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
	}

	else if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		if (SSInput_GetKeyState(SIK_LSHIFT))
		{
			float posy = 0.f;

			if (SSInput_GetKeyState(SIK_UP))
				posy += float(timeDelta) * 0.001f;
			if (SSInput_GetKeyState(SIK_DOWN))
				posy -= float(timeDelta) * 0.001f;

			if (SSInput_GetKeyState(SIM_LBUTTON))
			{
				if (IsSFirstRBMTransform)
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
						posy += float(timeDelta) * 0.001f * float(-int(p.y - cy));
				}
				else
					IsSFirstRBMTransform = true;
				SRender_CentererCursor();
			}
			else
			{
				IsSFirstLBMTransform = false;
				IsSFirstRBMTransform = false;
			}

			SAIG_QuadSelectedAddPosY(posy);
		}
	}
}

void level_editor::FinalImageUncheckedMenu()
{
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_COLOR, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, false);

	level_editor::pCheckBoxTBRColor->setCheck(false);
	level_editor::pCheckBoxTBRNormal->setCheck(false);
	level_editor::pCheckBoxTBRParam->setCheck(false);
	level_editor::pCheckBoxTBRAmDiff->setCheck(false);
	level_editor::pCheckBoxTBRSpecular->setCheck(false);
	level_editor::pCheckBoxTBRLighting->setCheck(false);
}