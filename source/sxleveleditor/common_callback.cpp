
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

void SXLevelEditor::LevelNew(bool mess)
{
	if (mess && (SGeom_ModelsGetCount() <= 0 || (SGeom_ModelsGetCount() > 0 && MessageBox(0, "Are you sure you need to create a new level?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO)))
		return;

	SLevel_Clear();
	char tmpcaption[256];
	sprintf(tmpcaption, "%s: new level ** ", "SXLevelEditor");
	SXLevelEditor::JobWindow->setText(tmpcaption);
	SXLevelEditor::ActiveGroupType = 0;
	SXLevelEditor::ActiveElement = -1;
	SXLevelEditor::ActiveGreenSplit = -1;
	SXLevelEditor::ActiveGreenObject = -1;

	ID gid = SML_LigthsGetGlobal();
	if (gid >= 0)
		SML_LigthsDeleteLight(gid);

	SXLevelEditor::CheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
	SXLevelEditor::CheckBoxTBLevelType->setCheck(false);
	SXLevelEditor::CheckBoxTBGLightEnable->setCheck(false);
}

void SXLevelEditor::LevelOpen()
{
	if (SGeom_ModelsGetCount() > 0 && MessageBox(0, "Are you sure that you need to open the level?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO)
		return;

	SXLevelEditor::LevelNew(false);

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
		sprintf(tmpcaption, "%s: %s", "SXLevelEditor", tmpname);
		SXLevelEditor::JobWindow->setText(tmpcaption);

		ID gid = SML_LigthsGetGlobal();
		if (gid >= 0)
		{
			SXLevelEditor::CheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP26);
			SXLevelEditor::CheckBoxTBLevelType->setCheck(true);
			SXLevelEditor::CheckBoxTBGLightEnable->setCheck(true);
		}
		else
		{
			SXLevelEditor::CheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
			SXLevelEditor::CheckBoxTBLevelType->setCheck(false);
			SXLevelEditor::CheckBoxTBGLightEnable->setCheck(false);
		}
	}
}

void SXLevelEditor::LevelSave()
{
	if (SLevel_GetName()[0])
		SLevel_Save(SLevel_GetName());
	else
	{
		if (SGeom_ModelsGetCount() <= 0)
			MessageBox(0, "You need to create a level!", 0, 0);
		else
			SXLevelEditor::LevelSaveAs();
	}
}

void SXLevelEditor::LevelSaveAs()
{
	if (SGeom_ModelsGetCount() <= 0)
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
		sprintf(tmpcaption, "%s: %s", "SXLevelEditor", tmpname);
		SXLevelEditor::JobWindow->setText(tmpcaption);
	}
}

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);

	//файл
	//новый
	if (id == ID_FILE_NEW)
	{
		SXLevelEditor::LevelNew(true);
	}
	//открыть
	else if (id == ID_FILE_OPEN)
	{
		SXLevelEditor::LevelOpen();
	}
	//сохранить
	else if (id == ID_FILE_SAVE)
	{
		SXLevelEditor::LevelSave();
	}
	//сохранить как
	else if (id == ID_FILE_SAVEAS)
	{
		SXLevelEditor::LevelSaveAs();
	}
	//выход
	else if (id == ID_FILE_EXIT)
	{
		return TrueExit(hwnd, msg, wParam, lParam);
	}

	else if (id == ID_VIEW_GRID)
	{
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
		SRender_EditorSetRenderGrid(SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	else if (id == ID_VIEW_AXES)
	{
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
		SRender_EditorSetRenderAxesStatic(SXLevelEditor::MainMenu->getCheckedItem(id));
	}

	//
	else if (id == ID_FINALIMAGE_COLOR)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		SXLevelEditor::FinalImageUncheckedMenu();
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}


	//
	else if (id == ID_SELECTIONSETTINGS_SELECTION)
	{
		SXLevelEditor::SelSelection = !SXLevelEditor::MainMenu->getCheckedItem(id);
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_ZTEST)
	{
		SXLevelEditor::SelZTest = !SXLevelEditor::MainMenu->getCheckedItem(id);
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_MESH)
	{
		SXLevelEditor::SelMesh = !SXLevelEditor::MainMenu->getCheckedItem(id);
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_SELECTIONSETTINGS_BACKFACESCULL)
	{
		SXLevelEditor::SelBackFacesCull = !SXLevelEditor::MainMenu->getCheckedItem(id);
		SXLevelEditor::MainMenu->setCheckItem(id, !SXLevelEditor::MainMenu->getCheckedItem(id));
	}

	else if (id >= SX_LE_MMENU_WEATHER_BEGIN_ID)
	{
		for (int i = 0; i < SXLevelEditor::MenuWeatherCount; ++i)
		{
			SXLevelEditor::MainMenu->setCheckItem(SX_LE_MMENU_WEATHER_BEGIN_ID+i, false);
		}
		SXLevelEditor::MainMenu->setCheckItem(id, true);
		SXLevelEditor::MenuWeatherCurrID = (id - SX_LE_MMENU_WEATHER_BEGIN_ID) - 1;

		if (SXLevelEditor::MenuWeatherCurrID >= 0)
		{
			char tmppath[1024];
			sprintf(tmppath, "%sweather\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_CONFIGS), SXLevelEditor::MenuWeatherArr[SXLevelEditor::MenuWeatherCurrID]);
			SLevel_WeatherLoad(tmppath);
		}
		else
		{
			SLevel_WeatherLoad(0);

			ID gid = SML_LigthsGetGlobal();
			if (gid >= 0)
			{
				SML_LigthsSetPos(gid, &float3(60, 60, 0), false);
				SML_LigthsSetColor(gid, &float3(1, 1, 1));
				SML_LigthsSetEnable(gid, true);
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

	SXLevelEditor::ObjAxesHelper->onMouseMove(((int)(short)LOWORD(lParam)), ((int)(short)HIWORD(lParam)));

	if (SXLevelEditor::ObjAxesHelper->m_bIsDragging == false)
		return 0;

	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM && SXLevelEditor::ActiveElement >= 0)
	{
		if (SXLevelEditor::ObjAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_MOVE)
		{
			float3* pos = SGeom_ModelsMGetPosition(SXLevelEditor::ActiveElement);
			float3 npos = (*pos) + (SXLevelEditor::ObjAxesHelper->getPosition() - SXLevelEditor::HelperPos);
			if ((*pos).x != npos.x || (*pos).y != npos.y || (*pos).z != npos.z)
			{
				*pos = npos;
				
				SGeom_ModelsMApplyTransform(SXLevelEditor::ActiveElement);
				float3 min, max;
				SGeom_ModelsMGetMinMax(SXLevelEditor::ActiveElement, &min, &max);

				SXLevelEditor::HelperPos = (max + min) * 0.5f;
			}
		}
		else if (SXLevelEditor::ObjAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_ROTATE)
		{
			float3* rot = SGeom_ModelsMGetRotation(SXLevelEditor::ActiveElement);
			float3 nrot = SXLevelEditor::ObjAxesHelper->getRotation();
			if ((*rot).x != nrot.x || (*rot).y != nrot.y || (*rot).z != nrot.z)
			{
				*rot = SXLevelEditor::ObjAxesHelper->getRotation();
				SGeom_ModelsMApplyTransform(SXLevelEditor::ActiveElement);
			}
		}
		else if (SXLevelEditor::ObjAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_SCALE)
		{
			float3* scale = SGeom_ModelsMGetScale(SXLevelEditor::ActiveElement);
			float3 nscale = SXLevelEditor::ObjAxesHelper->getScale() - float3(1,1,1);
			if ((*scale).x != nscale.x || (*scale).y != nscale.y || (*scale).z != nscale.z)
			{
				*scale += nscale;
				SGeom_ModelsMApplyTransform(SXLevelEditor::ActiveElement);
			}
		}
	}
	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && SXLevelEditor::ActiveElement >= 0 && SXLevelEditor::ActiveGreenSplit >= 0 && SXLevelEditor::ActiveGreenObject >= 0)
	{
		if (SXLevelEditor::ComboBoxGreenSel->getSel() == 0)
		{
			float3_t pos;
			SGeom_GreenGetPosObject(SXLevelEditor::ActiveElement, SXLevelEditor::ActiveGreenSplit, SXLevelEditor::ActiveGreenObject, &pos);
			float3 helperpos = SXLevelEditor::ObjAxesHelper->getPosition();
			if (pos.x != helperpos.x || pos.y != helperpos.y || pos.z != helperpos.z)
			{
				SGeom_GreenSetPosObject(SXLevelEditor::ActiveElement, &SXLevelEditor::ActiveGreenSplit, &SXLevelEditor::ActiveGreenObject, &(float3_t)helperpos);
			}
		}
	}
	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && SXLevelEditor::ActiveElement >= 0)
	{
		CBaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ActiveElement);
		if (!bEnt)
			return 0;

		if (SXLevelEditor::ObjAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_MOVE)
			bEnt->setPos(SXLevelEditor::ObjAxesHelper->getPosition());
		else if (SXLevelEditor::ObjAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_ROTATE)
			bEnt->setOrient(SXLevelEditor::ObjAxesHelper->getRotationQ());

		SXLevelEditor::GameUpdatePosRot();
	}
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_LDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;
	SXLevelEditor::ObjAxesHelper->m_bIsDragging = true;
	SXLevelEditor::ObjAxesHelper->m_bIsDraggingStart = true;
	return 0;
}

LRESULT SXLevelEditor_RenderWindow_LClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	SXLevelEditor::ObjAxesHelper->m_bIsDragging = false;
	SXLevelEditor::ObjAxesHelper->m_bIsDraggingStop = true;

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

	if (!SXLevelEditor::CheckBoxTBArrow->getCheck())
		return 0;

	SXLevelEditor::IdMtl = -1;

	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || SXLevelEditor::ActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_ModelsTraceBeam(&vCamPos, &camDir, &_res, &idmodel, &idmtl))
		{
			SXLevelEditor::ListBoxList->setSel(idmodel);
			SXLevelEditor::GeomSel(idmodel);
			SXLevelEditor::IdMtl = idmtl;
		}
	}
	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && SXLevelEditor::ActiveElement >= 0)
	{
		ID idgreen;
		ID idsplit;
		ID idobj;

		if (SXLevelEditor::ComboBoxGreenSel->getSel() == 0)
		{
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGeom_GreenTraceBeam(&vCamPos, &camDir, &_res, &idgreen, &idsplit, &idobj, &idmtl))
			{
				SXLevelEditor::ActiveGreenSplit = idsplit;
				SXLevelEditor::ActiveGreenObject = idobj;
				SGeom_GreenGetPosObject(idgreen, idsplit, idobj, &pos2);

				SXLevelEditor::StaticGreenSelX->setText("Pos X:");
				SXLevelEditor::StaticGreenSelY->setText("Pos Y:");
				SXLevelEditor::StaticGreenSelZ->setText("Pos Z:");

				SXLevelEditor::EditGreenSelX->setText(String(pos2.x).c_str());
				SXLevelEditor::EditGreenSelY->setText(String(pos2.y).c_str());
				SXLevelEditor::EditGreenSelZ->setText(String(pos2.z).c_str());
				
				SXLevelEditor::ObjAxesHelper->setPosition(pos2);
				SXLevelEditor::ObjAxesHelper->setRotation(float3(0,0,0));
				SXLevelEditor::ObjAxesHelper->setScale(float3(1, 1, 1));

				SXLevelEditor::IdMtl = idmtl;
			}
		}
		else if (SXLevelEditor::ComboBoxGreenSel->getSel() == 1)
		{
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGeom_ModelsTraceBeam(&vCamPos, &camDir, &_res, &idmodel, &idmtl))
			{
				idobj = SGeom_GreenAddObject(SXLevelEditor::ActiveElement, &_res, &idsplit);
				SXLevelEditor::ActiveGreenSplit = idsplit;
				SXLevelEditor::ActiveGreenObject = idobj;

				SXLevelEditor::StaticGreenSelX->setText("Pos X:");
				SXLevelEditor::StaticGreenSelY->setText("Pos Y:");
				SXLevelEditor::StaticGreenSelZ->setText("Pos Z:");

				SXLevelEditor::EditGreenSelX->setText(String(_res.x).c_str());
				SXLevelEditor::EditGreenSelY->setText(String(_res.y).c_str());
				SXLevelEditor::EditGreenSelZ->setText(String(_res.z).c_str());

				SXLevelEditor::ObjAxesHelper->setPosition(pos2);
				SXLevelEditor::ObjAxesHelper->setRotation(float3(0, 0, 0));
				SXLevelEditor::ObjAxesHelper->setScale(float3(1, 1, 1));
				
				int qwert = 0;
			}
		}

		else if (SXLevelEditor::ComboBoxGreenSel->getSel() == 2)
		{
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGeom_ModelsTraceBeam(&vCamPos, &camDir, &_res, &idmodel, &idmtl))
			{
				SXLevelEditor::GreenBoxPos = _res;
				int density = SXLevelEditor::TrackBarGreenDensity->getPos();

				float3 min, max, npos;
				min.x = _res.x - SXLevelEditor::GreenBoxWHD.x * 0.5f;
				min.z = _res.z - SXLevelEditor::GreenBoxWHD.z * 0.5f;

				max.x = _res.x + SXLevelEditor::GreenBoxWHD.x * 0.5f;
				max.z = _res.z + SXLevelEditor::GreenBoxWHD.z * 0.5f;

				npos.y = _res.y + SXLevelEditor::GreenBoxWHD.y;

				for (int i = 0; i < density; ++i)
				{
					npos.x = randf(min.x, max.x);
					npos.z = randf(min.z, max.z);
					if(SGeom_ModelsTraceBeam(&npos, &float3(0, -1, 0), &_res, &idmodel, &idmtl))
						SGeom_GreenAddObject(SXLevelEditor::ActiveElement, &_res, 0);
				}
			}
		}

		else if (SXLevelEditor::ComboBoxGreenSel->getSel() == 3)
		{
			static const int *r_win_width = GET_PCVAR_INT("r_win_width");
			static const int *r_win_height = GET_PCVAR_INT("r_win_height");
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGeom_GreenTraceBeam(&vCamPos, &camDir, &_res, &idgreen, &idsplit, &idobj, &idmtl))
			{
				SXLevelEditor::ActiveGreenSplit = -1;
				SXLevelEditor::ActiveGreenObject = -1;
				SXLevelEditor::EditGreenSelX->setText("");
				SXLevelEditor::EditGreenSelY->setText("");
				SXLevelEditor::EditGreenSelZ->setText("");
				SGeom_GreenDelObject(idgreen, idsplit, idobj);
				int qwert = 0;
			}
		}
	}
	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME && SXLevelEditor::ActiveElement >= 0)
	{

	}
	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		if (SXLevelEditor::RadioButtonAIQuadAdd->getCheck())
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
		else if (SXLevelEditor::RadioButtonAIQuadsMSel->getCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_QuadSelect(idaq, true);
		}
		else if (SXLevelEditor::RadioButtonAIQuadsSelDel->getCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_QuadDelete(idaq);
		}
		else if (SXLevelEditor::RadioButtonAIGPAdd->getCheck())
		{
			ID idaq = SAIG_GridTraceBeam(&pos, &camDir);

			if (idaq > -1)
				SAIG_GraphPointAdd(idaq);
		}
		else if (SXLevelEditor::RadioButtonAIGPDel->getCheck())
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

	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && SXLevelEditor::ActiveElement >= 0)
	{
		float3 _res;
		float3_t pos2;
		ID idmodel;
		ID idmtl;

		ID idgreen;
		ID idsplit;
		ID idobj;

		if (SXLevelEditor::ComboBoxGreenSel->getSel() == 2)
		{
			if (pt.x <= *r_win_width && pt.y <= *r_win_height && SGeom_ModelsTraceBeam(&vCamPos, &camDir, &_res, &idmodel, &idmtl))
				SXLevelEditor::GreenBoxPos = _res;
		}
	}

	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		if (SXLevelEditor::RadioButtonAIQuadsMSel->getCheck())
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

	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
	{
		SAIG_QuadSelect(-1, false);
	}

	return 0;
}

LRESULT SXLevelEditor_RenderWindow_Delete(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SSInput_GetKeyState(SIK_LCONTROL) || SSInput_GetKeyState(SIK_LSHIFT))
		return 0;

	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && SXLevelEditor::ActiveElement >= 0 && SXLevelEditor::ActiveGreenSplit >= 0 && SXLevelEditor::ActiveGreenObject >= 0)
	{
		SGeom_GreenDelObject(SXLevelEditor::ActiveElement, SXLevelEditor::ActiveGreenSplit, SXLevelEditor::ActiveGreenObject);
		SXLevelEditor::ActiveGreenSplit = -1;
		SXLevelEditor::ActiveGreenObject = -1;
		SXLevelEditor::EditGreenSelX->setText("");
		SXLevelEditor::EditGreenSelY->setText("");
		SXLevelEditor::EditGreenSelZ->setText("");
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGeometryOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->clear();
	long tmpcountmodel = SGeom_ModelsGetCount();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	SXLevelEditor::StaticListValCount->setText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	for (int i = 0; i < tmpcountmodel;++i)
	{
		sprintf(tmpnamecountpoly, "%s | %d", SGeom_ModelsMGetName(i), SGeom_ModelsMGetCountPoly(i));
		SXLevelEditor::ListBoxList->addItem(tmpnamecountpoly);
	}

	SXLevelEditor::GreenActivateAll(false);
	SXLevelEditor::GameActivateAll(false);
	SXLevelEditor::AIGridActivateAll(false);
	SXLevelEditor::GeomActivateAll(false);
	SXLevelEditor::GeomActivateCreate(true);
	//SXLevelEditor::GeomActivateTrans(false);

	SXLevelEditor::ActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GEOM;
	SXLevelEditor::ActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->clear();
	long tmpcountmodel = SGeom_GreenGetCount();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	SXLevelEditor::StaticListValCount->setText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	for (int i = 0; i < tmpcountmodel; ++i)
	{
		sprintf(tmpnamecountpoly, "%s | %s | %d", 
			SGeom_GreenMGetName(i),
			(SGeom_GreenMGetTypeCountGen(i) == GREEN_TYPE_GRASS ? "grass" : "tree/shrub"),
			SGeom_GreenMGetCountGen(i));
		SXLevelEditor::ListBoxList->addItem(tmpnamecountpoly);
	}

	SXLevelEditor::GeomActivateAll(false);

	SXLevelEditor::GameActivateAll(false);

	SXLevelEditor::GreenActivateAll(false);
	SXLevelEditor::GreenActivateCreate(true);
	SXLevelEditor::AIGridActivateAll(false);
	SXLevelEditor::GreenActivateMain(true);
	
	//MCActivateTrans(false);

	SXLevelEditor::ActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GREEN;
	SXLevelEditor::ActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonGameObjectOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->clear();

	int tmpcoungo = SXGame_EntGetCount();
	int tmpcoungo2 = 0;
	char tmpname[1024];
	for (int i = 0; i < tmpcoungo; ++i)
	{
		CBaseEntity* bEnt = SXGame_EntGet(i);
		if (bEnt)
		{
			sprintf(tmpname, "%s / %s", bEnt->getName(), bEnt->getClassName());
			SXLevelEditor::ListBoxList->addItem(tmpname);
			SXLevelEditor::ListBoxList->setItemData(SXLevelEditor::ListBoxList->getItemCount() - 1, i);
			++tmpcoungo2;
		}
	}

	char tmptextvalcountgo[64];
	sprintf(tmptextvalcountgo, "%d", tmpcoungo2);
	SXLevelEditor::StaticListValCount->setText(tmptextvalcountgo);

	SXLevelEditor::GeomActivateAll(false);
	SXLevelEditor::GreenActivateAll(false);
	SXLevelEditor::AIGridActivateAll(false);
	SXLevelEditor::GameActivateAll(true);

	SXLevelEditor::ActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GAME;
	SXLevelEditor::ActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->clear();

	SXLevelEditor::StaticListValCount->setText("0");

	SXLevelEditor::GeomActivateAll(false);
	SXLevelEditor::GreenActivateAll(false);
	SXLevelEditor::GameActivateAll(false);

	SXLevelEditor::AIGridActivateAll(true);

	SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_AIGRID;
	SXLevelEditor::ActiveElement = -1;
	return 0;
}

LRESULT SXLevelEditor_ListBoxList_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->getSel();

	SXLevelEditor::ActiveGreenSplit = -1;
	SXLevelEditor::ActiveGreenObject = -1;

	if (sel < 0)
		return 0;

	if (SXLevelEditor::ActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GEOM || SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_ModelsGetCount() > 0 && sel < SGeom_ModelsGetCount())
			SXLevelEditor::GeomSel(sel);
	}
	else if (SXLevelEditor::ActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GREEN || SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		if (SGeom_GreenGetCount() > 0 && sel < SGeom_GreenGetCount())
			SXLevelEditor::GreenSel(sel);
	}
	else if (SXLevelEditor::ActiveGroupType == -EDITORS_LEVEL_GROUPTYPE_GAME || SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
	{
		if (SXGame_EntGetCount() > 0 && sel < SXGame_EntGetCount())
			SXLevelEditor::GameSel(sel);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->getSel();

	if (sel < 0)
		return 0;

	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SGeom_ModelsGetCount() > 0 && sel < SGeom_ModelsGetCount())
		{
			SGeom_ModelsDelModel(sel);
			SXLevelEditor::ListBoxList->deleteItem(sel);
			if (SGeom_ModelsGetCount() > 0)
			{
				if (sel > 0)
				{
					if (SGeom_ModelsGetCount() <= sel)
						sel -= 1;
				}
				SXLevelEditor::ListBoxList->setSel(sel);
				SXLevelEditor::GeomSel(sel);
			}
			else
			{
				SXLevelEditor_ButtonGeometryOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}
	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		if (SGeom_GreenGetCount() > 0 && sel < SGeom_GreenGetCount())
		{
			SGeom_GreenDelGreen(sel);
			SXLevelEditor::ListBoxList->deleteItem(sel);
			if (SGeom_GreenGetCount() > 0)
			{
				if (sel > 0)
				{
					if (SGeom_GreenGetCount() <= sel)
						sel -= 1;
				}
				SXLevelEditor::ListBoxList->setSel(sel);
				SXLevelEditor::GreenSel(sel);
			}
			else
			{
				SXLevelEditor_ButtonGreenOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}
	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
	{
		if (SXGame_EntGetCount() > 0 && sel < SXGame_EntGetCount())
		{
			SXGame_RemoveEntity(SXGame_EntGet(SXLevelEditor::ListBoxList->getItemData(sel)));
			SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);

			if (sel >= SXLevelEditor::ListBoxList->getItemCount())
				--sel;
			if (sel >= 0)
			{
				SXLevelEditor::ListBoxList->setSel(sel);
				SXLevelEditor::GameSel(sel);
			}
		}
	}

	return 0;
}





LRESULT SXLevelEditor_GroupBox_Notify(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (((NMHDR*)lParam)->hwndFrom == SXLevelEditor::ListViewGameClass->getHWND() && ((NMHDR*)lParam)->code == NM_CLICK)
	{
		SXLevelEditor_ListViewGameClass_Click();
	}
	else if (((NMHDR*)lParam)->hwndFrom == SXLevelEditor::ListViewGameConnections->getHWND() && ((NMHDR*)lParam)->code == NM_CLICK)
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
		if (SXLevelEditor::CheckBoxTBArrow->getHWND() == handle_elem)
		{
			SXLevelEditor::CheckBoxTBPos->setCheck(false);
			SXLevelEditor::CheckBoxTBRot->setCheck(false);
			SXLevelEditor::CheckBoxTBScale->setCheck(false);
			SXLevelEditor::ObjAxesHelper->setType(CAxesHelper::HANDLER_TYPE_NONE);
		}
		else if (SXLevelEditor::CheckBoxTBPos->getHWND() == handle_elem)
		{
			if (
				SXLevelEditor::ActiveElement >= 0 &&
				(
				SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM ||
				(SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && SXLevelEditor::ActiveGreenSplit >= 0 && SXLevelEditor::ActiveGreenObject >= 0) ||
				SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME
				)
				)
			{
				SXLevelEditor::CheckBoxTBArrow->setCheck(false);
				SXLevelEditor::CheckBoxTBRot->setCheck(false);
				SXLevelEditor::CheckBoxTBScale->setCheck(false);
				SXLevelEditor::ObjAxesHelper->setType(CAxesHelper::HANDLER_TYPE_MOVE);
			}
			else
				SXLevelEditor::CheckBoxTBPos->setCheck(false);
		}
		else if (SXLevelEditor::CheckBoxTBRot->getHWND() == handle_elem)
		{
			if ((SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME) && SXLevelEditor::ActiveElement >= 0)
			{
				SXLevelEditor::CheckBoxTBArrow->setCheck(false);
				SXLevelEditor::CheckBoxTBPos->setCheck(false);
				SXLevelEditor::CheckBoxTBScale->setCheck(false);
				SXLevelEditor::ObjAxesHelper->setType(CAxesHelper::HANDLER_TYPE_ROTATE);
			}
			else
				SXLevelEditor::CheckBoxTBRot->setCheck(false);
		}
		else if (SXLevelEditor::CheckBoxTBScale->getHWND() == handle_elem)
		{
			if ((SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM || SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME) && SXLevelEditor::ActiveElement >= 0)
			{
				SXLevelEditor::CheckBoxTBPos->setCheck(false);
				SXLevelEditor::CheckBoxTBRot->setCheck(false);
				SXLevelEditor::CheckBoxTBArrow->setCheck(false);
				SXLevelEditor::ObjAxesHelper->setType(CAxesHelper::HANDLER_TYPE_SCALE);
			}
			else
				SXLevelEditor::CheckBoxTBScale->setCheck(false);
		}

		else if (SXLevelEditor::ButtonTBNew->getHWND() == handle_elem)
		{
			SXLevelEditor::LevelNew(true);
		}
		else if (SXLevelEditor::ButtonTBOpen->getHWND() == handle_elem)
		{
			SXLevelEditor::LevelOpen();
		}
		else if (SXLevelEditor::ButtonTBSave->getHWND() == handle_elem)
		{
			SXLevelEditor::LevelSave();
		}
		else if (SXLevelEditor::ButtonTBSaveAs->getHWND() == handle_elem)
		{
			SXLevelEditor::LevelSaveAs();
		}

		else if (SXLevelEditor::CheckBoxTBGrid->getHWND() == handle_elem)
		{
			SXLevelEditor::MainMenu->setCheckItem(ID_VIEW_GRID, SXLevelEditor::CheckBoxTBGrid->getCheck());
			SRender_EditorSetRenderGrid(SXLevelEditor::CheckBoxTBGrid->getCheck());
		}
		else if (SXLevelEditor::CheckBoxTBAxes->getHWND() == handle_elem)
		{
			SXLevelEditor::MainMenu->setCheckItem(ID_VIEW_AXES, SXLevelEditor::CheckBoxTBAxes->getCheck());
			SRender_EditorSetRenderAxesStatic(SXLevelEditor::CheckBoxTBAxes->getCheck());
		}

		else if (SXLevelEditor::CheckBoxTBRColor->getHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_COLOR, true);
			SXLevelEditor::CheckBoxTBRColor->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		}
		else if (SXLevelEditor::CheckBoxTBRNormal->getHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, true);
			SXLevelEditor::CheckBoxTBRNormal->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		}
		else if (SXLevelEditor::CheckBoxTBRParam->getHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, true);
			SXLevelEditor::CheckBoxTBRParam->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		}
		else if (SXLevelEditor::CheckBoxTBRAmDiff->getHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			SXLevelEditor::CheckBoxTBRAmDiff->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		}
		else if (SXLevelEditor::CheckBoxTBRSpecular->getHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, true);
			SXLevelEditor::CheckBoxTBRSpecular->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		}
		else if (SXLevelEditor::CheckBoxTBRLighting->getHWND() == handle_elem)
		{
			SXLevelEditor::FinalImageUncheckedMenu();
			SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			SXLevelEditor::CheckBoxTBRLighting->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		}

		else if (SXLevelEditor::CheckBoxTBSelS->getHWND() == handle_elem)
		{
			SXLevelEditor::SelSelection = SXLevelEditor::CheckBoxTBSelS->getCheck();
			SXLevelEditor::MainMenu->setCheckItem(ID_SELECTIONSETTINGS_SELECTION, SXLevelEditor::CheckBoxTBSelS->getCheck());
		}
		else if (SXLevelEditor::CheckBoxTBSelZTest->getHWND() == handle_elem)
		{
			SXLevelEditor::SelZTest = SXLevelEditor::CheckBoxTBSelZTest->getCheck();
			SXLevelEditor::MainMenu->setCheckItem(ID_SELECTIONSETTINGS_ZTEST, SXLevelEditor::CheckBoxTBSelZTest->getCheck());
		}
		else if (SXLevelEditor::CheckBoxTBSelMesh->getHWND() == handle_elem)
		{
			SXLevelEditor::SelMesh = SXLevelEditor::CheckBoxTBSelMesh->getCheck();
			SXLevelEditor::MainMenu->setCheckItem(ID_SELECTIONSETTINGS_MESH, SXLevelEditor::CheckBoxTBSelMesh->getCheck());
		}
		else if (SXLevelEditor::CheckBoxTBSelCullBack->getHWND() == handle_elem)
		{
			SXLevelEditor::SelBackFacesCull = SXLevelEditor::CheckBoxTBSelCullBack->getCheck();
			SXLevelEditor::MainMenu->setCheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, SXLevelEditor::CheckBoxTBSelCullBack->getCheck());
		}

		else if (SXLevelEditor::CheckBoxTBAIGBound->getHWND() == handle_elem)
		{
			SXLevelEditor::AIGBound = SXLevelEditor::CheckBoxTBAIGBound->getCheck();
		}
		else if (SXLevelEditor::CheckBoxTBAIGQuad->getHWND() == handle_elem)
		{
			SXLevelEditor::AIGQuad = SXLevelEditor::CheckBoxTBAIGQuad->getCheck();
		}
		else if (SXLevelEditor::CheckBoxTBAIGGraphPoint->getHWND() == handle_elem)
		{
			SXLevelEditor::AIGGraphPoint = SXLevelEditor::CheckBoxTBAIGGraphPoint->getCheck();
		}

		else if (SXLevelEditor::CheckBoxTBLevelType->getHWND() == handle_elem)
		{
			if (SXLevelEditor::CheckBoxTBLevelType->getCheck())
			{
				ID gid = SML_LigthsGetGlobal();
				if (gid < 0)
				{
					gid = SML_LigthsCreatePoint(
						&float3(60, 60, 0),
						0,
						&float3(1, 1, 1),
						true,
						true);
					SML_LigthsSetEnable(gid, SXLevelEditor::CheckBoxTBGLightEnable->getCheck());
					SML_LigthsSetName(gid, "sun");
				}
				SXLevelEditor::CheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP26);
			}
			else
			{
				ID gid = SML_LigthsGetGlobal();
				if (gid >= 0)
					SML_LigthsDeleteLight(gid);
				SXLevelEditor::CheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
				SXLevelEditor::CheckBoxTBGLightEnable->setCheck(false);
			}
		}

		else if (SXLevelEditor::CheckBoxTBGLightEnable->getHWND() == handle_elem)
		{
			ID gid = SML_LigthsGetGlobal();
			if (gid >= 0)
				SML_LigthsSetEnable(gid, SXLevelEditor::CheckBoxTBGLightEnable->getCheck());
			else
				SXLevelEditor::CheckBoxTBGLightEnable->setCheck(false);
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
		if (handle_elem == SXLevelEditor::CheckBoxAIGridMarkedSplits->getHWND())
			SAIG_GridSetMarkSplits(SXLevelEditor::CheckBoxAIGridMarkedSplits->getCheck());
		else
		{
			ID seldata = SXLevelEditor::ListBoxList->getItemData(SXLevelEditor::ListBoxList->getSel());
			CBaseEntity* bEnt = SXGame_EntGet(seldata);
			int str = SXLevelEditor::ListViewGameClass->getSelString();

			if (seldata < 0 || str < 0)
				return 0;
			char txtval[256];
			SXLevelEditor::ListViewGameClass->getItemText(txtval, 1, str, 256);
			UINT uiFlags;
			sscanf(txtval, "%d", &uiFlags);

			for (int i = 0; i < 16; ++i)
			{
				if (SXLevelEditor::CheckBoxGameFlags[i]->getHWND() == handle_elem)
				{
					if (SXLevelEditor::CheckBoxGameFlags[i]->getCheck())
						uiFlags |= (1 << (i + 16));
					else
						uiFlags ^= (1 << (i + 16));
				}
			}

			sprintf(txtval, "%d", uiFlags);
			SXLevelEditor::ListViewGameClass->setItemText(txtval, 1, str);
			bEnt->setKV("flags", txtval);
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		if (SXLevelEditor::ComboBoxGameValue->getHWND() == handle_elem)
		{
			int sel = SXLevelEditor::ListBoxList->getSel();
			char txt[256];
			SXLevelEditor::ComboBoxGameValue->getItemText(SXLevelEditor::ComboBoxGameValue->getSel(), txt);
			SXLevelEditor::ListViewGameClass->setItemText(txt, 1, SXLevelEditor::ListViewGameClass->getSelString());
			CBaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->getItemData(sel));
			if (bEnt)
			{
				propdata_t* pd = (propdata_t*)SXLevelEditor::ListViewGameClass->getItemData(SXLevelEditor::ListViewGameClass->getSelString());
				SXLevelEditor::ComboBoxGameValue->getItemData(SXLevelEditor::ComboBoxGameValue->getSel());
				bEnt->setKV(pd->szKey, (const char*)SXLevelEditor::ComboBoxGameValue->getItemData(SXLevelEditor::ComboBoxGameValue->getSel()));
			}
		}
		else if (SXLevelEditor::ComboBoxGameClass->getHWND() == handle_elem)
		{
			if (SXLevelEditor::ComboBoxGameClass->getSel() == 0)
				return 0;

			int sel = SXLevelEditor::ListBoxList->getSel();

			if (sel < 0)
				return 0;

			CBaseEntity* bEnt = SXGame_EntGet(SXLevelEditor::ListBoxList->getItemData(sel));
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
			SXLevelEditor::ComboBoxGameClass->getItemText(SXLevelEditor::ComboBoxGameClass->getSel(), txt);
			SXGame_RemoveEntity(bEnt);

			bEnt = SXGame_CreateEntity(txt);
			bEnt->setFlags(bEnt->getFlags() | EF_EXPORT | EF_LEVEL);


			for(int i = 0; i < tmparrdata.size(); ++i)
			{
				bEnt->setKV(tmparrdata[i].key.c_str(), tmparrdata[i].val.c_str());
			}

			SXLevelEditor_ButtonGameObjectOpen_Click(hwnd, msg, wParam, lParam);
			sel = -1;
			for (int i = 0; i < SXLevelEditor::ListBoxList->getItemCount(); ++i)
			{
				if(SXLevelEditor::ListBoxList->getItemData(i) == bEnt->getId())
				{
					sel = i;
					break;
				}
			}

			if(sel < 0)
			{

			}

			SXLevelEditor::ListBoxList->setSel(sel);
			SXLevelEditor::GameSel(sel);
		}
		else if (SXLevelEditor::ComboBoxGreenSel->getHWND() == handle_elem)
		{
			int sel = SXLevelEditor::ComboBoxGreenSel->getSel();
			SXLevelEditor::GreenRenderBox = false;
			
			if (sel == 0)
			{
				SXLevelEditor::StaticGreenSelX->setText("Position X:");
				SXLevelEditor::StaticGreenSelY->setText("Position Y:");
				SXLevelEditor::StaticGreenSelZ->setText("Position Z:");
				SXLevelEditor::EditGreenSelX->setText("");
				SXLevelEditor::EditGreenSelY->setText("");
				SXLevelEditor::EditGreenSelZ->setText("");
			}
			else if (sel == 2)
			{
				SXLevelEditor::GreenRenderBox = true;

				SXLevelEditor::StaticGreenSelX->setText("Width volume:");
				SXLevelEditor::StaticGreenSelY->setText("Height volume:");
				SXLevelEditor::StaticGreenSelZ->setText("Depth volume:");
				SXLevelEditor::EditGreenSelX->setText(String(SXLevelEditor::GreenBoxWHD.x).c_str());
				SXLevelEditor::EditGreenSelY->setText(String(SXLevelEditor::GreenBoxWHD.y).c_str());
				SXLevelEditor::EditGreenSelZ->setText(String(SXLevelEditor::GreenBoxWHD.z).c_str());
			}

			
		}

		else if (SXLevelEditor::ComboBoxGameConnectionsEvent->getHWND() == handle_elem)
		{
			ID idSelListBoxList = SXLevelEditor::ListBoxList->getSel();
			ID idSelStrTable = SXLevelEditor::ListViewGameConnections->getSelString();

			if (idSelListBoxList < 0 || idSelStrTable < 0)
				return 0;

			char szBuffer256[256];
			szBuffer256[0] = 0;
			SXLevelEditor::ComboBoxGameConnectionsEvent->getItemText(256, szBuffer256);
			SXLevelEditor::ListViewGameConnections->setItemText(szBuffer256, 0, idSelStrTable);
			return SXLevelEditor_EditGameConnections_Enter(hwnd, msg, wParam, lParam);
		}
		else if (SXLevelEditor::ComboBoxGameConnectionsAction->getHWND() == handle_elem)
		{
			ID idSelListBoxList = SXLevelEditor::ListBoxList->getSel();
			ID idSelStrTable = SXLevelEditor::ListViewGameConnections->getSelString();

			if (idSelListBoxList < 0 || idSelStrTable < 0)
				return 0;

			char szBuffer256[256];
			szBuffer256[0] = 0;
			SXLevelEditor::ComboBoxGameConnectionsAction->getItemText(SXLevelEditor::ComboBoxGameConnectionsAction->getSel(), szBuffer256);
			SXLevelEditor::ListViewGameConnections->setItemText(szBuffer256, 2, idSelStrTable);
			return SXLevelEditor_EditGameConnections_Enter(hwnd, msg, wParam, lParam);
		}
	}
	/*else if (Notification == EN_CHANGE)
	{
		if (SXLevelEditor::EditGameConnectionsACValue->getHWND() == handle_elem)
		{
			static int recurion = 0;
			if (recurion > 0)
			{
				recurion = 0;
				return 0;
			}
				
			
			char edit_text[256];
			SXLevelEditor::EditGameConnectionsACValue->GetText(edit_text, 256);
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
							SXLevelEditor::EditGameConnectionsACValue->setText(lower_name);
							PostMessage(SXLevelEditor::EditGameConnectionsACValue->getHWND(), EM_SETSEL, strlen(edit_text), strlen(lower_name));
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
		if (handle_elem == SXLevelEditor::ListBoxList->getHWND())
			SXLevelEditor_ListBoxList_Click(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void SXLevelEditor_Transform(DWORD timeDelta)
{
	static bool IsSFirstLBMTransform = false;
	static bool IsSFirstRBMTransform = false;
	static int CoordinateTransformation = -1;

	if (SXLevelEditor::RadioButtonGeomPosX->getCheck() || SXLevelEditor::RadioButtonGeomRotX->getCheck() || SXLevelEditor::RadioButtonGeomScaleX->getCheck() || SXLevelEditor::RadioButtonGreenSelX->getCheck())
		CoordinateTransformation = 0;
	else if (SXLevelEditor::RadioButtonGeomPosY->getCheck() || SXLevelEditor::RadioButtonGeomRotY->getCheck() || SXLevelEditor::RadioButtonGeomScaleY->getCheck() || SXLevelEditor::RadioButtonGreenSelY->getCheck())
		CoordinateTransformation = 1;
	else if (SXLevelEditor::RadioButtonGeomPosZ->getCheck() || SXLevelEditor::RadioButtonGeomRotZ->getCheck() || SXLevelEditor::RadioButtonGeomScaleZ->getCheck() || SXLevelEditor::RadioButtonGreenSelZ->getCheck())
		CoordinateTransformation = 2;

	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		if (SSInput_GetKeyState(SIK_LSHIFT) && SGeom_ModelsGetCount() > 0)
		{
			DWORD selmodel = SXLevelEditor::ListBoxList->getSel();
			if (SXLevelEditor::RadioButtonGeomPosX->getCheck() || SXLevelEditor::RadioButtonGeomPosY->getCheck() || SXLevelEditor::RadioButtonGeomPosZ->getCheck())
			{
				float3& pos = *(SGeom_ModelsMGetPosition(selmodel));

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

				SXLevelEditor::EditGeomPosX->setText(tmpPosX);
				SXLevelEditor::EditGeomPosY->setText(tmpPosY);
				SXLevelEditor::EditGeomPosZ->setText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}

			else if (SXLevelEditor::RadioButtonGeomRotX->getCheck() || SXLevelEditor::RadioButtonGeomRotY->getCheck() || SXLevelEditor::RadioButtonGeomRotZ->getCheck())
			{
				float3& pos = *(SGeom_ModelsMGetRotation(selmodel));

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

				SXLevelEditor::EditGeomRotX->setText(tmpPosX);
				SXLevelEditor::EditGeomRotY->setText(tmpPosY);
				SXLevelEditor::EditGeomRotZ->setText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}

			else if (SXLevelEditor::RadioButtonGeomScaleX->getCheck() || SXLevelEditor::RadioButtonGeomScaleY->getCheck() || SXLevelEditor::RadioButtonGeomScaleZ->getCheck())
			{
				float3& pos = *(SGeom_ModelsMGetScale(selmodel));

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

				SXLevelEditor::EditGeomScaleX->setText(tmpPosX);
				SXLevelEditor::EditGeomScaleY->setText(tmpPosY);
				SXLevelEditor::EditGeomScaleZ->setText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}
		}
	}
	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN &&
			(
			(SXLevelEditor::ComboBoxGreenSel->getSel() == 0 && SXLevelEditor::ActiveElement >= 0 && SXLevelEditor::ActiveGreenSplit >= 0 && SXLevelEditor::ActiveGreenObject >= 0) ||
			(SXLevelEditor::ComboBoxGreenSel->getSel() == 2)
			)
		)
	{
		if (SSInput_GetKeyState(SIK_LSHIFT) && SGeom_GreenGetCount() > 0)
		{
			DWORD selmodel = SXLevelEditor::ListBoxList->getSel();
			if (SXLevelEditor::RadioButtonGreenSelX->getCheck() || SXLevelEditor::RadioButtonGreenSelY->getCheck() || SXLevelEditor::RadioButtonGreenSelZ->getCheck())
			{
				float3 pos;
				if (SXLevelEditor::ComboBoxGreenSel->getSel() == 2)
					pos = SXLevelEditor::GreenBoxWHD;
				else
				{
					float3_t pos2;
					SGeom_GreenGetPosObject(SXLevelEditor::ActiveElement, SXLevelEditor::ActiveGreenSplit, SXLevelEditor::ActiveGreenObject, &pos2);
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

				SXLevelEditor::EditGreenSelX->setText(tmpPosX);
				SXLevelEditor::EditGreenSelY->setText(tmpPosY);
				SXLevelEditor::EditGreenSelZ->setText(tmpPosZ);

				if (SXLevelEditor::ComboBoxGreenSel->getSel() == 2)
					SXLevelEditor::GreenBoxWHD = pos;
				else
					SGeom_GreenSetPosObject(SXLevelEditor::ActiveElement, &SXLevelEditor::ActiveGreenSplit, &SXLevelEditor::ActiveGreenObject, &float3_t(pos));
			}
		}
	}

	else if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_AIGRID)
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

void SXLevelEditor::FinalImageUncheckedMenu()
{
	SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_COLOR, false);
	SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, false);
	SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, false);
	SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, false);
	SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, false);
	SXLevelEditor::MainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, false);

	SXLevelEditor::CheckBoxTBRColor->setCheck(false);
	SXLevelEditor::CheckBoxTBRNormal->setCheck(false);
	SXLevelEditor::CheckBoxTBRParam->setCheck(false);
	SXLevelEditor::CheckBoxTBRAmDiff->setCheck(false);
	SXLevelEditor::CheckBoxTBRSpecular->setCheck(false);
	SXLevelEditor::CheckBoxTBRLighting->setCheck(false);
}