
#include "green_callback.h"

void level_editor::GreenActivateAll(bool bf)
{
	level_editor::GreenActivateMain(bf);
	level_editor::GreenActivateCreate(bf);
	level_editor::GreenActivateEdit(bf);
}

void level_editor::GreenActivateMain(bool bf)
{
	level_editor::pStaticGreenName->setVisible(bf);
	level_editor::pEditGreenName->setVisible(bf);
	level_editor::pEditGreenName->setText("");

	level_editor::pStaticGreenModel->setVisible(bf);
	level_editor::pEditGreenModel->setVisible(bf);
	level_editor::pEditGreenModel->setText("");
	level_editor::pButtonGreenModel->setVisible(bf);

	level_editor::pStaticGreenLod1->setVisible(bf);
	level_editor::pEditGreenLod1->setVisible(bf);
	level_editor::pEditGreenLod1->setText("");
	level_editor::pButtonGreenLod1->setVisible(bf);
	
	level_editor::pStaticGreenLod2->setVisible(bf);
	level_editor::pEditGreenLod2->setVisible(bf);
	level_editor::pEditGreenLod2->setText("");
	level_editor::pButtonGreenLod2->setVisible(bf);

	level_editor::pStaticGreenMask->setVisible(bf);
	level_editor::pEditGreenMask->setVisible(bf);
	level_editor::pEditGreenMask->setText("");
	level_editor::pButtonGreenMask->setVisible(bf);
	level_editor::pCheckBoxGreenAveragedRGB->setCheck(false);

	level_editor::pCheckBoxGreenAveragedRGB->setVisible(bf);

	level_editor::pStaticGreenNav->setVisible(bf);
	level_editor::pEditGreenNav->setVisible(bf);
	level_editor::pEditGreenNav->setText("");
	level_editor::pButtonGreenNav->setVisible(bf);
}

void level_editor::GreenActivateCreate(bool bf)
{
	level_editor::pStaticGreenMask->setEnable(bf);
	level_editor::pEditGreenMask->setEnable(bf);
	level_editor::pButtonGreenMask->setEnable(bf);
	level_editor::pCheckBoxGreenAveragedRGB->setEnable(bf);
	level_editor::pCheckBoxGreenAveragedRGB->setCheck(false);

	level_editor::pButtonGreenGenerate->setVisible(bf);

	level_editor::pTrackBarGreenDensity->setVisible(bf);
	level_editor::pTrackBarGreenDensity->setPos(0);

	level_editor::pStaticGreenDensityVal->setVisible(bf);
	level_editor::pStaticGreenDensityText->setVisible(bf);
}

void level_editor::GreenActivateEdit(bool bf)
{
	level_editor::pButtonGreenGenerate->setVisible(bf);

	level_editor::pTrackBarGreenDensity->setVisible(bf);
	level_editor::pTrackBarGreenDensity->setPos(0);

	level_editor::pStaticGreenDensityVal->setVisible(bf);
	level_editor::pStaticGreenDensityText->setVisible(bf);

	//level_editor::CheckBoxSingleSel->setVisible(bf);
	//level_editor::pStaticSingleSelPos->setVisible(bf);
	level_editor::pEditGreenSelX->setVisible(bf);
	level_editor::pEditGreenSelX->setText("");
	level_editor::pEditGreenSelY->setVisible(bf);
	level_editor::pEditGreenSelY->setText("");
	level_editor::pEditGreenSelZ->setVisible(bf);
	level_editor::pEditGreenSelZ->setText("");
	level_editor::pRadioButtonGreenSelX->setVisible(bf);
	level_editor::pRadioButtonGreenSelY->setVisible(bf);
	level_editor::pRadioButtonGreenSelZ->setVisible(bf);
	level_editor::pComboBoxGreenSel->setVisible(bf);
	level_editor::pStaticGreenSelX->setVisible(bf);
	level_editor::pStaticGreenSelY->setVisible(bf);
	level_editor::pStaticGreenSelZ->setVisible(bf);
}

void level_editor::GreenSel(int iSelect)
{
	if (iSelect >= 0 && iSelect < SGreen_GetCount())
	{
		level_editor::idActiveElement = iSelect;
		level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;

		level_editor::GreenActivateCreate(false);
		level_editor::GreenActivateEdit(true);
		level_editor::pButtonGreenGenerate->setVisible(false);

		level_editor::pEditGreenModel->setText(SGreen_MGetModel(iSelect));
		level_editor::pEditGreenLod1->setText((SGreen_MGetLod1(iSelect) ? SGreen_MGetLod1(iSelect) : ""));
		level_editor::pEditGreenLod2->setText((SGreen_MGetLod2(iSelect) ? SGreen_MGetLod2(iSelect) : ""));
		level_editor::pEditGreenMask->setText(SGreen_MGetMask(iSelect));
		level_editor::pEditGreenName->setText(SGreen_MGetName(iSelect));
		level_editor::pEditGreenNav->setText(SGreen_MGetNav(iSelect));
	}
}

void level_editor::GreenTraceSelect()
{
	float3 vResult;
	ID idGreen = -1;
	ID idSplit = -1;
	ID idObj = -1;
	ID idMtrl = -1;
	ID idModel = -1;

	//если выделение объектов
	if (level_editor::pComboBoxGreenSel->getSel() == 0)
	{
		if (SGreen_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, &idGreen, &idSplit, &idObj, &idMtrl))
		{
			float3_t vResult2;
			level_editor::idActiveGreenSplit = idSplit;
			level_editor::idActiveGreenObject = idObj;
			SGreen_GetPosObject(idGreen, idSplit, idObj, &vResult2);

			level_editor::pStaticGreenSelX->setText("Pos X:");
			level_editor::pStaticGreenSelY->setText("Pos Y:");
			level_editor::pStaticGreenSelZ->setText("Pos Z:");

			level_editor::pEditGreenSelX->setText(String(vResult2.x).c_str());
			level_editor::pEditGreenSelY->setText(String(vResult2.y).c_str());
			level_editor::pEditGreenSelZ->setText(String(vResult2.z).c_str());

			level_editor::pAxesHelper->setPosition(vResult2);
			level_editor::pAxesHelper->setRotation(float3(0, 0, 0));
			level_editor::pAxesHelper->setScale(float3(1, 1, 1));

			level_editor::idMtl = idMtrl;
		}
	}
	//если единичная генерация
	else if (level_editor::pComboBoxGreenSel->getSel() == 1)
	{
		if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, &idModel, &idMtrl))
		{
			idObj = SGreen_AddObject(level_editor::idActiveElement, &vResult, &idSplit);
			level_editor::idActiveGreenSplit = idSplit;
			level_editor::idActiveGreenObject = idObj;

			level_editor::pStaticGreenSelX->setText("Pos X:");
			level_editor::pStaticGreenSelY->setText("Pos Y:");
			level_editor::pStaticGreenSelZ->setText("Pos Z:");

			level_editor::pEditGreenSelX->setText(String(vResult.x).c_str());
			level_editor::pEditGreenSelY->setText(String(vResult.y).c_str());
			level_editor::pEditGreenSelZ->setText(String(vResult.z).c_str());

			level_editor::pAxesHelper->setPosition(vResult);
			level_editor::pAxesHelper->setRotation(float3(0, 0, 0));
			level_editor::pAxesHelper->setScale(float3(1, 1, 1));
		}
	}

	//если генерация в ограничивающем объеме
	else if (level_editor::pComboBoxGreenSel->getSel() == 2)
	{
		if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, &idModel, &idMtrl))
		{
			level_editor::vGreenBoxPos = vResult;
			int iDensity = level_editor::pTrackBarGreenDensity->getPos();

			float3 vMin, vMax, vPos;
			vMin.x = vResult.x - level_editor::vGreenBoxWHD.x * 0.5f;
			vMin.z = vResult.z - level_editor::vGreenBoxWHD.z * 0.5f;

			vMax.x = vResult.x + level_editor::vGreenBoxWHD.x * 0.5f;
			vMax.z = vResult.z + level_editor::vGreenBoxWHD.z * 0.5f;

			vPos.y = vResult.y + level_editor::vGreenBoxWHD.y;

			for (int i = 0; i < iDensity; ++i)
			{
				vPos.x = randf(vMin.x, vMax.x);
				vPos.z = randf(vMin.z, vMax.z);
				if (SGeom_TraceBeam(&vPos, &float3(0, -1, 0), &vResult, &idModel, &idMtrl))
					SGreen_AddObject(level_editor::idActiveElement, &vResult, 0);
			}
		}
	}
	//если удаление
	else if (level_editor::pComboBoxGreenSel->getSel() == 3)
	{
		if (SGreen_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, &idGreen, &idSplit, &idObj, &idMtrl))
		{
			level_editor::idActiveGreenSplit = -1;
			level_editor::idActiveGreenObject = -1;
			level_editor::pEditGreenSelX->setText("");
			level_editor::pEditGreenSelY->setText("");
			level_editor::pEditGreenSelZ->setText("");
			SGreen_DelObject(idGreen, idSplit, idObj);
		}
	}
}

void level_editor::GreenTraceSetPos()
{
	if (!(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && level_editor::idActiveElement >= 0 && level_editor::idActiveGreenSplit >= 0 && level_editor::idActiveGreenObject >= 0))
		return;

	//если включено индивидуальное выделение, то перемещаем объект растительности
	if (level_editor::pComboBoxGreenSel->getSel() == 0)
	{
		float3_t vCurrPos;
		SGreen_GetPosObject(level_editor::idActiveElement, level_editor::idActiveGreenSplit, level_editor::idActiveGreenObject, &vCurrPos);
		float3 vResult;
		if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, 0, 0))
		{
			if (vCurrPos.x != vResult.x || vCurrPos.y != vResult.y || vCurrPos.z != vResult.z)
			{
				SGreen_SetPosObject(level_editor::idActiveElement, &level_editor::idActiveGreenSplit, &level_editor::idActiveGreenObject, &(float3_t)vResult);
				level_editor::pAxesHelper->setPosition(vResult);
			}
		}
		
	}
}

void level_editor::GreenTransformByHelper()
{
	if(!(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && ID_VALID(level_editor::idActiveElement) && ID_VALID(level_editor::idActiveGreenSplit) && ID_VALID(level_editor::idActiveGreenObject)))
		return;

	if(!level_editor::pAxesHelper->isDragging())
		return;

	//если включено индивидуальное выделение, то перемещаем объект растительности
	if(level_editor::pComboBoxGreenSel->getSel() == 0)
	{
		float3_t vCurrPos;
		SGreen_GetPosObject(level_editor::idActiveElement, level_editor::idActiveGreenSplit, level_editor::idActiveGreenObject, &vCurrPos);
		float3 vNewPos = level_editor::pAxesHelper->getPosition();
		if(vCurrPos.x != vNewPos.x || vCurrPos.y != vNewPos.y || vCurrPos.z != vNewPos.z)
			SGreen_SetPosObject(level_editor::idActiveElement, &level_editor::idActiveGreenSplit, &level_editor::idActiveGreenObject, &(float3_t)vNewPos);
	}
}

void level_editor::GreenSetPos4Box()
{
	if(!(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && ID_VALID(level_editor::idActiveElement)))
		return;

	float3 vResult;

	//если включена генерация в ограничивающем объеме, значит перемещаем этот объем
	if (level_editor::pComboBoxGreenSel->getSel() == 2)
	{
		if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, 0, 0))
			level_editor::vGreenBoxPos = vResult;
	}
}

void level_editor::GreenDelete(int iSelected)
{
	if (!(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN && iSelected >= 0))
		return;

	if (SGreen_GetCount() > 0 && iSelected < SGreen_GetCount())
	{
		SGreen_DelGreen(iSelected);
		level_editor::pListBoxList->deleteItem(iSelected);
	}
}

void level_editor::FillListBoxGreen(int iSelect)
{
	level_editor::pListBoxList->clear();
	int iCountGreens = SGreen_GetCount();

	level_editor::pStaticListValCount->setText(String(iCountGreens).c_str());

	char szNote[1024];
	for (int i = 0; i < iCountGreens; ++i)
	{
		sprintf(szNote, "%s | %s | %d",
			SGreen_MGetName(i),
			(SGreen_MGetTypeCountGen(i) == GREEN_TYPE_GRASS ? "grass" : "tree/shrub"),
			SGreen_MGetCountGen(i));
		level_editor::pListBoxList->addItem(szNote);
	}

	if (iSelect >= 0 && iCountGreens > iSelect)
	{
		level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;
		level_editor::idActiveElement = iSelect;
		level_editor::pListBoxList->setSel(iSelect);
		level_editor::GreenSel(iSelect);
	}
	else
	{
		level_editor::iActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GREEN;
		level_editor::idActiveElement = -1;
	}
}

//##########################################################################

LRESULT SXLevelEditor_EditGreenName_Enter(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int iSelected = level_editor::pListBoxList->getSel();
	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		level_editor::pEditGreenName->getText(SGreen_MGetName(iSelected), 64);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGreenModel_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	szPath[0] = 0;
	char szName[1024];
	
	if (gui_func::dialogs::SelectFileOwn(szName, szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGreenModel->setText(sRpath.c_str());
		int iSelected = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (iSelected >= 0 && iSelected < SGreen_GetCount())
				SGreen_MSetLod(iSelected, 0, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenLod1_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	szPath[0] = 0;
	char szName[1024];
	
	if (gui_func::dialogs::SelectFileOwn(szName, szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGreenLod1->setText(sRpath.c_str());
		int iSelected = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (iSelected >= 0 && iSelected < SGreen_GetCount())
				SGreen_MSetLod(iSelected, 1, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenLod2_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	szPath[0] = 0;
	char szName[1024];
	
	if (gui_func::dialogs::SelectFileOwn(szName, szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGreenLod2->setText(sRpath.c_str());
		int iSelected = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (iSelected >= 0 && iSelected < SGreen_GetCount())
				SGreen_MSetLod(iSelected, 2, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_TrackBarGreenDensity_MouseMove(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szFreq[64];
	int iPos = level_editor::pTrackBarGreenDensity->getPos();
	sprintf(szFreq, "%d%", iPos);
	level_editor::pStaticGreenDensityVal->setText(szFreq);
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenMask_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	szPath[0] = 0;
	char szName[1024];
	
	if (gui_func::dialogs::SelectFileOwn(szName, szPath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture mask", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
		level_editor::pEditGreenMask->setText(szName);

	return 0;
}

LRESULT SXLevelEditor_ButtonGreenNav_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	szPath[0] = 0;
	char szName[1024];
	
	if (gui_func::dialogs::SelectFileOwn(szName, szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(szPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGreenNav->setText(sRpath.c_str());
		int iSelected = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (iSelected >= 0 && iSelected < SGreen_GetCount())
				SGreen_MSetNav(iSelected, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenGenerate_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (SGeom_GetCountModels() == 0)
	{
		MessageBox(0, "Not found geometry!", "Not found geometry", 0);
		return 0;
	}

	char szName[1024];
	szName[0] = 0;

	char szMask[1024];
	szMask[0] = 0;

	char szPath4Model[1024];
	szPath4Model[0] = 0;

	char szPath4Lod1[1024];
	szPath4Lod1[0] = 0;

	char szPath4Lod2[1024];
	szPath4Lod2[0] = 0;

	char szPath4Physics[1024];
	szPath4Physics[0] = 0;

	GREEN_TYPE typeGreen = GREEN_TYPE_TREE;

	level_editor::pEditGreenName->getText(szName, 1024);
	level_editor::pEditGreenMask->getText(szMask, 1024);
	level_editor::pEditGreenModel->getText(szPath4Model, 1024);
	level_editor::pEditGreenLod1->getText(szPath4Lod1, 1024);
	level_editor::pEditGreenLod2->getText(szPath4Lod2, 1024);
	level_editor::pEditGreenNav->getText(szPath4Physics, 1024);

	if (!STR_VALIDATE(szPath4Lod2))
	{
		typeGreen = GREEN_TYPE_GRASS;
		szPath4Lod1[0] = szPath4Lod2[0] = 0;
	}

	char szError[2048];

	if (!level_editor::existsFileStaticGeom(szPath4Model))
	{
		sprintf(szError, "%s%s%s", "Model [", szPath4Model, "] not found");
		MessageBox(0, szError, 0, 0);
		return 0;
	}

	if (STR_VALIDATE(szPath4Physics) && !level_editor::existsFileStaticGeom(szPath4Physics))
	{
		sprintf(szError, "%s%s%s", "Model [", szPath4Physics, "] not found");
		MessageBox(0, szError, 0, 0);
		return 0;
	}

	if (typeGreen == GREEN_TYPE_TREE)
	{
		if (!level_editor::existsFileStaticGeom(szPath4Lod1))
		{
			sprintf(szError, "%s%s%s", "Model [", szPath4Lod1, "] not found");
			MessageBox(0, szError, 0, 0);
			return 0;
		}

		if (!level_editor::existsFileStaticGeom(szPath4Lod2))
		{
			sprintf(szError, "%s%s%s", "Model [", szPath4Lod2, "] not found");
			MessageBox(0, szError, 0, 0);
			return 0;
		}
	}

	if (!STR_VALIDATE(szName))
	{
		if (MessageBox(0, "No name for the model, enter the file name automatically?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
			sprintf(szName, "%s", FileSetStrExt(FileBaseName(szPath4Model), "").c_str());
		else
			return 0;
	}

	float fDensity = level_editor::pTrackBarGreenDensity->getPos();

	SPhysics_LoadGeom(NULL);

	ID idGreen = SGreen_AddGreen(szName,
		szMask,
		level_editor::pCheckBoxGreenAveragedRGB->getCheck(),
		fDensity,
		szPath4Model,
		szPath4Lod1,
		szPath4Lod2,
		szPath4Physics
		);

	level_editor::GreenActivateCreate(false);
	level_editor::FillListBoxGreen(idGreen);

	return 0;
}