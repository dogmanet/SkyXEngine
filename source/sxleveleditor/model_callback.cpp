
#include "model_callback.h"

void level_editor::FillListBoxGeom(int iSelect)
{
	level_editor::pListBoxList->clear();

	int iCountModels = SGeom_GetCountModels();
	level_editor::pStaticListValCount->setText(String(iCountModels).c_str());

	char szNote[1024];
	for (int i = 0; i < iCountModels; ++i)
	{
		sprintf(szNote, "%s | %d", SGeom_ModelGetName(i), SGeom_ModelGetCountPoly(i));
		level_editor::pListBoxList->addItem(szNote);
	}

	if (iSelect >= 0 && iCountModels > iSelect)
	{
		level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;
		level_editor::idActiveElement = iSelect;
		level_editor::pListBoxList->setSel(iSelect);
		level_editor::GeomSel(iSelect);
	}
	else
	{
		level_editor::iActiveGroupType = -EDITORS_LEVEL_GROUPTYPE_GEOM;
		level_editor::idActiveElement = -1;
	}
}


void level_editor::GeomActivateAll(bool bf)
{
	level_editor::GeomActivateCreate(bf);
	level_editor::GeomActivateTrans(bf);
}

void level_editor::GeomActivateCreate(bool bf)
{
	level_editor::pStaticGeomName->setVisible(bf);
	level_editor::pStaticGeomModel->setVisible(bf);
	level_editor::pStaticGeomLod1->setVisible(bf);
	level_editor::pStaticGeomPhysics->setVisible(bf);
	level_editor::pEditGeomName->setVisible(bf);
	level_editor::pEditGeomName->setText("");
	level_editor::pEditGeomModel->setVisible(bf);
	level_editor::pEditGeomModel->setText("");
	level_editor::pEditGeomLod1->setVisible(bf);
	level_editor::pEditGeomLod1->setText("");
	level_editor::pButtonGeomLod1->setVisible(bf);
	level_editor::pEditGeomPhysics->setVisible(bf);
	level_editor::pEditGeomPhysics->setText("");
	level_editor::pButtonGeomPhysics->setVisible(bf);
	level_editor::pCheckBoxSegmentation->setVisible(bf);
	level_editor::pCheckBoxSegmentation->setCheck(false);
	level_editor::pButtonGeomModel->setVisible(bf);
	level_editor::pButtonGeomFinish->setVisible(bf);
}


void level_editor::GeomActivateTrans(bool bf)
{
	level_editor::pStaticGeomPos->setVisible(bf);
	level_editor::pEditGeomPosX->setVisible(bf);
	level_editor::pEditGeomPosY->setVisible(bf);
	level_editor::pEditGeomPosZ->setVisible(bf);
	level_editor::pRadioButtonGeomPosX->setVisible(bf);
	level_editor::pRadioButtonGeomPosY->setVisible(bf);
	level_editor::pRadioButtonGeomPosZ->setVisible(bf);
	level_editor::pStaticGeomRot->setVisible(bf);
	level_editor::pEditGeomRotX->setVisible(bf);
	level_editor::pEditGeomRotY->setVisible(bf);
	level_editor::pEditGeomRotZ->setVisible(bf);
	level_editor::pRadioButtonGeomRotX->setVisible(bf);
	level_editor::pRadioButtonGeomRotY->setVisible(bf);
	level_editor::pRadioButtonGeomRotZ->setVisible(bf);
	level_editor::pStaticGeomScale->setVisible(bf);
	level_editor::pEditGeomScaleX->setVisible(bf);
	level_editor::pEditGeomScaleY->setVisible(bf);
	level_editor::pEditGeomScaleZ->setVisible(bf);
	level_editor::pRadioButtonGeomScaleX->setVisible(bf);
	level_editor::pRadioButtonGeomScaleY->setVisible(bf);
	level_editor::pRadioButtonGeomScaleZ->setVisible(bf);

	level_editor::pEditGeomModel->setEnable(true);
	level_editor::pButtonGeomModel->setEnable(true);
	level_editor::pCheckBoxSegmentation->setEnable(true);
}

void level_editor::GeomSel(int iSelected)
{
	if (iSelected >= 0 && iSelected < SGeom_GetCountModels())
	{
		level_editor::idActiveElement = iSelected;
		level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;

		level_editor::GeomActivateTrans(true);

		level_editor::pEditGeomModel->setText(SGeom_ModelGetPath4Model(iSelected));
		level_editor::pEditGeomLod1->setText(SGeom_ModelGetPath4Lod(iSelected));
		const char *szName = SGeom_ModelGetName(iSelected);
		const float3 *pPos = SGeom_ModelGetPosition(iSelected);
		const float3 *pRot = SGeom_ModelGetRotation(iSelected);
		const float3 *pScale = SGeom_ModelGetScale(iSelected);

		float3 vMin, vMax;
		SGeom_ModelGetMinMax(iSelected, &vMin, &vMax);

		/*level_editor::vHelperPos = (vMax + vMin) * 0.5f;
		level_editor::vHelperScale = *pScale;*/

		level_editor::pAxesHelper->setPosition((vMax + vMin) * 0.5f);
		level_editor::pAxesHelper->setRotation(*pRot);
		level_editor::pAxesHelper->setScale(float3(1,1,1));

		level_editor::pEditGeomName->setText(szName);

		char szStr[64];

		sprintf(szStr, "%f", pPos->x);
		level_editor::pEditGeomPosX->setText(szStr);

		sprintf(szStr, "%f", pPos->y);
		level_editor::pEditGeomPosY->setText(szStr);

		sprintf(szStr, "%f", pPos->z);
		level_editor::pEditGeomPosZ->setText(szStr);


		sprintf(szStr, "%f", pRot->x);
		level_editor::pEditGeomRotX->setText(szStr);

		sprintf(szStr, "%f", pRot->y);
		level_editor::pEditGeomRotY->setText(szStr);

		sprintf(szStr, "%f", pRot->z);
		level_editor::pEditGeomRotZ->setText(szStr);


		sprintf(szStr, "%f", pScale->x);
		level_editor::pEditGeomScaleX->setText(szStr);

		sprintf(szStr, "%f", pScale->y);
		level_editor::pEditGeomScaleY->setText(szStr);

		sprintf(szStr, "%f", pScale->z);
		level_editor::pEditGeomScaleZ->setText(szStr);

		level_editor::pCheckBoxSegmentation->setCheck(SGeom_ModelGetSegmentation(iSelected));

		level_editor::pEditGeomModel->setEnable(false);
		level_editor::pButtonGeomModel->setEnable(false);
		level_editor::pCheckBoxSegmentation->setEnable(false);
		level_editor::pButtonGeomFinish->setVisible(false);
	}
}

void level_editor::GeomTraceSelect()
{
	float3 vResult;
	ID idModel;
	ID idMtrl;

	if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, &idModel, &idMtrl))
	{
		level_editor::pListBoxList->setSel(idModel);
		level_editor::GeomSel(idModel);
		level_editor::idMtl = idMtrl;
	}
}

void level_editor::GeomTraceSetPos()
{
	if (!(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM && level_editor::idActiveElement >= 0))
		return;

	float3 vResult;
	if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, 0, 0))
	{
		if (level_editor::useCopyData && level_editor::idCopy >= 0)
			level_editor::vCopyPos = vResult;
		else
		{
			SGeom_ModelSetPosition(level_editor::idActiveElement, &vResult);
			level_editor::pAxesHelper->setPosition(vResult);
		}
	}
}

void level_editor::GeomTraceCreate()
{
	if (level_editor::iActiveGroupType != -EDITORS_LEVEL_GROUPTYPE_GEOM)
		return;

	float3 vResult;
	if (SGeom_TraceBeam(&(level_editor::vRayOrigin), &(level_editor::vRayDir), &vResult, 0, 0))
	{
		SXLevelEditor_ButtonGeomFinish_Click(0,0,0,0);
		SGeom_ModelSetPosition(level_editor::idActiveElement, &vResult);
		level_editor::pAxesHelper->setPosition(vResult);
	}
}

void level_editor::GeomUpdateCopyPos()
{
	if (!(level_editor::useCopyData && level_editor::idCopy >= 0))
		return;

	float3 vMin, vMax;
	SGeom_ModelGetMinMax(level_editor::idCopy, &vMin, &vMax);

	D3DXPLANE oPlane;
	D3DXPlaneFromPoints(&oPlane,
		&D3DXVECTOR3(vMin),
		&D3DXVECTOR3(vMax.x, vMin.y, vMin.z),
		&D3DXVECTOR3(vMin.x, vMin.y, vMax.z));

	D3DXVECTOR3 vResult;
	D3DXPlaneIntersectLine(&vResult, &oPlane, &D3DXVECTOR3(level_editor::vRayOrigin), &D3DXVECTOR3(float3(level_editor::vRayOrigin + level_editor::vRayDir * 10000.f)));

	float3 vPos;
	SGeom_ModelGetPosition(level_editor::idCopy, &vPos);
	level_editor::vCopyPos.x = vResult.x;
	level_editor::vCopyPos.y = vPos.y;
	level_editor::vCopyPos.z = vResult.z;
}

void level_editor::GeomCopy()
{
	if (!(level_editor::useCopyData && level_editor::idCopy >= 0))
		return;

	int iSelected = level_editor::pListBoxList->getSel();
	ID idCopy = SGeom_ModelCopy(iSelected);
	SGeom_ModelSetPosition(idCopy, &(level_editor::vCopyPos));

	level_editor::FillListBoxGeom(idCopy);

	level_editor::idCopy = idCopy;
	SGeom_ModelGetPosition(idCopy, &(level_editor::vCopyPos));
}

void level_editor::GeomTransformByHelper()
{
	if (!(level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM && level_editor::idActiveElement >= 0))
		return;

	static float3 vStartScale;
	static bool isStartScale = true;

	if (level_editor::pAxesHelper->m_bIsDragging == false)
	{
		isStartScale = true;
		return;
	}

	//перемещение
	if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_MOVE)
	{
		float3 vCurrPos = *SGeom_ModelGetPosition(level_editor::idActiveElement);
		float3 vMin, vMax;
		SGeom_ModelGetMinMax(level_editor::idActiveElement, &vMin, &vMax);

		float3 vCenterModel = (vMax + vMin) * 0.5f;
		float3 vNewPos = vCurrPos + (level_editor::pAxesHelper->getPosition() - vCenterModel);
		if (vCurrPos.x != vNewPos.x || vCurrPos.y != vNewPos.y || vCurrPos.z != vNewPos.z)
			SGeom_ModelSetPosition(level_editor::idActiveElement, &vNewPos);

		isStartScale = true;
	}
	//повороты
	else if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_ROTATE)
	{
		float3 vCurrRot = *SGeom_ModelGetRotation(level_editor::idActiveElement);
		float3 vNewRot = level_editor::pAxesHelper->getRotation();
		if (vCurrRot.x != vNewRot.x || vCurrRot.y != vNewRot.y || vCurrRot.z != vNewRot.z)
			SGeom_ModelSetRotation(level_editor::idActiveElement, &vNewRot);

		isStartScale = true;
	}
	//масштаб
	else if (level_editor::pAxesHelper->getType() == CAxesHelper::HANDLER_TYPE_SCALE)
	{
		float3 vCurrScale = *SGeom_ModelGetScale(level_editor::idActiveElement);
		float3 vNewScale;

		if (isStartScale)
		{
			isStartScale = false;
			vStartScale = vCurrScale;
			vNewScale = vCurrScale;
		}
		else
		{
			vNewScale = vStartScale + (level_editor::pAxesHelper->getScale() - float3(1, 1, 1));
		}
		
		//float3 vNewScale = vCurrScale + (level_editor::pAxesHelper->getScale() - float3(1, 1, 1));
		if (vCurrScale.x != vNewScale.x || vCurrScale.y != vNewScale.y || vCurrScale.z != vNewScale.z)
			SGeom_ModelSetScale(level_editor::idActiveElement, &vNewScale);
	}

	/*float3 vMin, vMax;
	SGeom_ModelGetMinMax(level_editor::idActiveElement, &vMin, &vMax);
	level_editor::vHelperPos = (vMax + vMin) * 0.5f;*/
}

void level_editor::GeomDelete(int iSelected)
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM || iSelected < 0)
		return;

	if (SGeom_GetCountModels() > 0 && iSelected < SGeom_GetCountModels())
	{
		SGeom_ModelDelete(iSelected);
		level_editor::pListBoxList->deleteItem(iSelected);
	}
}

//##########################################################################

LRESULT SXLevelEditor_EditGeomName_Enter(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int iSelected = level_editor::pListBoxList->getSel();

	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		char szStr[64];
		level_editor::pEditGeomName->getText(szStr, 64);
		SGeom_ModelSetName(iSelected, szStr);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGeomModel_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGeomModel->setText(sRpath.c_str());
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGeomLod1OrPhysics_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szFullPath[1024];
	szFullPath[0] = 0;
	char szName[1024];
	
	if (gui_func::dialogs::SelectFileOwn(szName, szFullPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(szFullPath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGeomLod1->setText(sRpath.c_str());
		int iSelected = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
		{
			if (iSelected >= 0 && iSelected < SGeom_GetCountModels())
			{
				if (hWnd == level_editor::pButtonGeomLod1->getHWND())
					SGeom_ModelSetPath4Lod(iSelected, sRpath.c_str());
				else if (hWnd == level_editor::pButtonGeomPhysics->getHWND())
					SGeom_ModelSetPath4Physics(iSelected, sRpath.c_str());
			}
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGeomFinish_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath4Model[1024];
	char szPath4Lod[1024];
	char szPath4Physics[1024];

	char szName[256];
	szName[0] = 0;
	level_editor::pEditGeomName->getText(szName, 256);
	level_editor::pEditGeomModel->getText(szPath4Model, 1024);
	level_editor::pEditGeomLod1->getText(szPath4Lod, 1024);
	level_editor::pEditGeomPhysics->getText(szPath4Physics, 1024);

	if (!STR_VALIDATE(szPath4Model))
		return 0;

	char szMsgError[1024];
	
	if (!level_editor::existsFileStaticGeom(szPath4Model))
	{
		sprintf(szMsgError, "%s%s%s", "Model [", szPath4Model, "] not found");
		MessageBox(0, szMsgError, 0, 0);
		return 0;
	}

	if (STR_VALIDATE(szPath4Lod) && !level_editor::existsFileStaticGeom(szPath4Lod))
	{
		sprintf(szMsgError, "%s%s%s", "Model/lod [", szPath4Lod, "] not found");
		MessageBox(0, szMsgError, 0, 0);
		return 0;
	}

	if (STR_VALIDATE(szPath4Physics) && !level_editor::existsFileStaticGeom(szPath4Physics))
	{
		sprintf(szMsgError, "%s%s%s", "Model/physics [", szPath4Physics, "] not found");
		MessageBox(0, szMsgError, 0, 0);
		return 0;
	}

	if (!STR_VALIDATE(szName))
	{
		//if (MessageBox(0, "No name for the model, enter the file name automatically?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
			sprintf(szName, "%s", FileSetStrExt(FileBaseName(szPath4Model), "").c_str());
		/*else
			return 0;*/
	}


	ID idModel = SGeom_ModelAdd(szPath4Model, szName, (STR_VALIDATE(szPath4Lod) ? szPath4Lod : 0), (STR_VALIDATE(szPath4Physics) ? szPath4Physics : 0), level_editor::pCheckBoxSegmentation->getCheck());

	char szNote[1024];
	sprintf(szNote, "%s | %d", SGeom_ModelGetName(idModel), SGeom_ModelGetCountPoly(idModel));
	level_editor::pListBoxList->addItem(szNote);

	level_editor::GeomActivateTrans(true);
	level_editor::pListBoxList->setSel(level_editor::pListBoxList->getItemCount() - 1);
	level_editor::GeomSel(level_editor::pListBoxList->getSel());

	level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;
	level_editor::idActiveElement = level_editor::pListBoxList->getSel();

	return 0;
}

//**************************************************************************

LRESULT SXLevelEditor_EditTransformPos_Enter(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;

	int iSelected = level_editor::pListBoxList->getSel();
	float3 vPos = *(SGeom_ModelGetPosition(iSelected));
	char szStr[64];

	if (hWnd == level_editor::pEditGeomPosX->getHWND())
	{
		level_editor::pEditGeomPosX->getText(szStr, 64);
		sscanf(szStr, "%f", &(vPos.x));
	}
	else if (hWnd == level_editor::pEditGeomPosY->getHWND())
	{
		level_editor::pEditGeomPosY->getText(szStr, 64);
		sscanf(szStr, "%f", &(vPos.y));
	}
	else if (hWnd == level_editor::pEditGeomPosZ->getHWND())
	{
		level_editor::pEditGeomPosZ->getText(szStr, 64);
		sscanf(szStr, "%f", &(vPos.z));
	}

	SGeom_ModelSetPosition(iSelected, &vPos);

	return 0;
}

LRESULT SXLevelEditor_EditTransformRot_Enter(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;

	int iSelected = level_editor::pListBoxList->getSel();
	float3 vRotation = *(SGeom_ModelGetRotation(iSelected));
	char szStr[64];

	if (hWnd == level_editor::pEditGeomRotX->getHWND())
	{
		level_editor::pEditGeomRotX->getText(szStr, 64);
		sscanf(szStr, "%f", &(vRotation.x));
	}
	else if (hWnd == level_editor::pEditGeomRotY->getHWND())
	{
		level_editor::pEditGeomRotY->getText(szStr, 64);
		sscanf(szStr, "%f", &(vRotation.y));
	}
	else if (hWnd == level_editor::pEditGeomRotZ->getHWND())
	{
		level_editor::pEditGeomRotZ->getText(szStr, 64);
		sscanf(szStr, "%f", &(vRotation.z));
	}

	SGeom_ModelSetRotation(iSelected, &vRotation);

	return 0;
}

LRESULT SXLevelEditor_EditTransformScale_Enter(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;

	int iSelected = level_editor::pListBoxList->getSel();
	float3 vScale = *(SGeom_ModelGetScale(iSelected));
	char szStr[64];

	if (hWnd == level_editor::pEditGeomScaleX->getHWND())
	{
		level_editor::pEditGeomScaleX->getText(szStr, 64);
		sscanf(szStr, "%f", &(vScale.x));
	}
	else if (hWnd == level_editor::pEditGeomScaleY->getHWND())
	{
		level_editor::pEditGeomScaleY->getText(szStr, 64);
		sscanf(szStr, "%f", &(vScale.y));
	}
	else if (hWnd == level_editor::pEditGeomScaleZ->getHWND())
	{
		level_editor::pEditGeomScaleZ->getText(szStr, 64);
		sscanf(szStr, "%f", &(vScale.z));
	}

	SGeom_ModelSetScale(iSelected, &vScale);

	return 0;
}