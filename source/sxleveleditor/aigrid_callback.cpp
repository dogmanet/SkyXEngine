
#include "aigrid_callback.h"

void level_editor::AIGridActivateAll(bool bf)
{
	pButtonAIQuadsDelSel->setVisible(bf);
	pButtonAIGridClear->setVisible(bf);
	pButtonAIGridGen->setVisible(bf);
	pButtonAIClearAll->setVisible(bf);
	pStaticAIBBDimensions->setVisible(bf);
	pStaticAIBBDimensionsWidth->setVisible(bf);
	pEditAIBBDimensionsWidth->setVisible(bf);
	pStaticAIBBDimensionsHeight->setVisible(bf);
	pEditAIBBDimensionsHeight->setVisible(bf);
	pStaticAIBBDimensionsDepth->setVisible(bf);
	pEditAIBBDimensionsDepth->setVisible(bf);
	pStaticAIBBPos->setVisible(bf);
	pStaticAIBBPosX->setVisible(bf);
	pEditAIBBPosX->setVisible(bf);
	pStaticAIBBPosY->setVisible(bf);
	pEditAIBBPosY->setVisible(bf);
	pStaticAIBBPosZ->setVisible(bf);
	pEditAIBBPosZ->setVisible(bf);
	pButtonAIBBFinish->setVisible(bf);
	pButtonAIGPGen->setVisible(bf);
	pButtonAIGPClear->setVisible(bf);
	pRadioButtonAIGPAdd->setVisible(bf);
	pRadioButtonAIGPDel->setVisible(bf);
	pRadioButtonAIQuadAdd->setVisible(bf);
	pRadioButtonAIQuadsMSel->setVisible(bf);
	pRadioButtonAIQuadsSelDel->setVisible(bf);
	pButtonAIGridValidation->setVisible(bf);
	pCheckBoxAIGridMarkedSplits->setVisible(bf);
	pStaticAIStatistics->setVisible(bf);
	pStaticAIStatsCountQuads->setVisible(bf);
	pStaticAIStatsCountGP->setVisible(bf);
	pStaticAIStatsCountSplits->setVisible(bf);
	pEditAIStatsCountQuads->setVisible(bf);
	pEditAIStatsCountGP->setVisible(bf);
	pEditAIStatsCountSplits->setVisible(bf);

	level_editor::AIGridEnableBB(!(SAIG_BBIsCreatedFinish()));

	if (bf)
	{
		float3 vDimensions;
		float3 vPosition;

		SAIG_BBGetDimensions(&vDimensions);
		SAIG_BBGetPos(&vPosition);

		pEditAIBBDimensionsWidth->setText(String(vDimensions.x).c_str());
		pEditAIBBDimensionsHeight->setText(String(vDimensions.y).c_str());
		pEditAIBBDimensionsDepth->setText(String(vDimensions.z).c_str());

		pEditAIBBPosX->setText(String(vPosition.x).c_str());
		pEditAIBBPosY->setText(String(vPosition.y).c_str());
		pEditAIBBPosZ->setText(String(vPosition.z).c_str());

		pEditAIStatsCountQuads->setText(String((DWORD)SAIG_GridGetCountQuads()).c_str());
		pEditAIStatsCountGP->setText(String((DWORD)SAIG_GraphPointGetCount()).c_str());
		pEditAIStatsCountSplits->setText(String((DWORD)SAIG_GridGetCountSplits()).c_str());
	}
}

void level_editor::AIGridEnableBB(bool bf)
{
	pStaticAIBBDimensions->setEnable(bf);
	pStaticAIBBDimensionsWidth->setEnable(bf);
	pEditAIBBDimensionsWidth->setEnable(bf);
	pStaticAIBBDimensionsHeight->setEnable(bf);
	pEditAIBBDimensionsHeight->setEnable(bf);
	pStaticAIBBDimensionsDepth->setEnable(bf);
	pEditAIBBDimensionsDepth->setEnable(bf);
	pStaticAIBBPos->setEnable(bf);
	pStaticAIBBPosX->setEnable(bf);
	pEditAIBBPosX->setEnable(bf);
	pStaticAIBBPosY->setEnable(bf);
	pEditAIBBPosY->setEnable(bf);
	pStaticAIBBPosZ->setEnable(bf);
	pEditAIBBPosZ->setEnable(bf);
	pButtonAIBBFinish->setEnable(bf);

	pButtonAIGridClear->setEnable(!bf);
	pButtonAIGPGen->setEnable(!bf);
	pButtonAIGPClear->setEnable(!bf);
	pRadioButtonAIGPAdd->setEnable(!bf);
	pRadioButtonAIGPDel->setEnable(!bf);
	pRadioButtonAIQuadAdd->setEnable(!bf);
	pRadioButtonAIQuadsMSel->setEnable(!bf);
	pRadioButtonAIQuadsSelDel->setEnable(!bf);
	pButtonAIGridValidation->setEnable(!bf);
	pCheckBoxAIGridMarkedSplits->setEnable(!bf);
	pStaticAIStatistics->setEnable(!bf);
	pStaticAIStatsCountQuads->setEnable(!bf);
	pStaticAIStatsCountGP->setEnable(!bf);
	pStaticAIStatsCountSplits->setEnable(!bf);
	pEditAIStatsCountQuads->setEnable(!bf);
	pEditAIStatsCountGP->setEnable(!bf);
	pEditAIStatsCountSplits->setEnable(!bf);
	pButtonAIQuadsDelSel->setEnable(!bf);
	pButtonAIGridGen->setEnable(!bf);
	pButtonAIClearAll->setEnable(!bf);
}

void level_editor::AIGridTraceSelect()
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_AIGRID)
		return;

	if (level_editor::pRadioButtonAIQuadAdd->getCheck())
	{
		float3 vEndPos = level_editor::vRayDirDirect + level_editor::vRayDir * 10000.0f;
		btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(level_editor::vRayDirDirect), F3_BTVEC(vEndPos));
		SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(level_editor::vRayDirDirect), F3_BTVEC(vEndPos), cb);

		if (cb.hasHit())
			SAIG_QuadAdd(&BTVEC_F3(cb.m_hitPointWorld));
	}
	else if (level_editor::pRadioButtonAIQuadsMSel->getCheck())
	{
		ID idQuad = SAIG_GridTraceBeam(&(level_editor::vRayDirDirect), &(level_editor::vRayDir));

		if (idQuad > -1)
			SAIG_QuadSelect(idQuad, true);
	}
	else if (level_editor::pRadioButtonAIQuadsSelDel->getCheck())
	{
		ID idQuad = SAIG_GridTraceBeam(&(level_editor::vRayDirDirect), &(level_editor::vRayDir));

		if (idQuad > -1)
			SAIG_QuadDelete(idQuad);
	}
	else if (level_editor::pRadioButtonAIGPAdd->getCheck())
	{
		ID idQuad = SAIG_GridTraceBeam(&(level_editor::vRayDirDirect), &(level_editor::vRayDir));

		if (idQuad > -1)
			SAIG_GraphPointAdd(idQuad);
	}
	else if (level_editor::pRadioButtonAIGPDel->getCheck())
	{
		ID idQuad = SAIG_GridTraceBeam(&(level_editor::vRayDirDirect), &(level_editor::vRayDir));

		if (idQuad > -1)
			SAIG_GraphPointDelete(idQuad);
	}
}

void level_editor::AIGridMultiSelect()
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_AIGRID)
		return;

	if (level_editor::pRadioButtonAIQuadsMSel->getCheck())
	{
		ID idQuad = SAIG_GridTraceBeam(&(level_editor::vRayDirDirect), &(level_editor::vRayDir));

		if (idQuad > -1)
			SAIG_QuadSelect(idQuad, false);
	}
}

//##########################################################################

LRESULT SXLevelEditor_EditAIBBDimensions_Enter(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szStr[64];
	float3 vDimensions;
	SAIG_BBGetDimensions(&vDimensions);

	if (hWnd == level_editor::pEditAIBBDimensionsWidth->getHWND())
	{
		level_editor::pEditAIBBDimensionsWidth->getText(szStr, 64);
		sscanf(szStr, "%f", &(vDimensions.x));
	}
	else if (hWnd == level_editor::pEditAIBBDimensionsHeight->getHWND())
	{
		level_editor::pEditAIBBDimensionsHeight->getText(szStr, 64);
		sscanf(szStr, "%f", &(vDimensions.y));
	}
	else if (hWnd == level_editor::pEditAIBBDimensionsDepth->getHWND())
	{
		level_editor::pEditAIBBDimensionsDepth->getText(szStr, 64);
		sscanf(szStr, "%f", &(vDimensions.z));
	}

	SAIG_BBSetDimensions(&vDimensions);

	return 0;
}

LRESULT SXLevelEditor_EditAIBBPos_Enter(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	char szStr[64];
	float3 vPos;
	SAIG_BBGetPos(&vPos);

	if (hWnd == level_editor::pEditAIBBPosX->getHWND())
	{
		level_editor::pEditAIBBPosX->getText(szStr, 64);
		sscanf(szStr, "%f", &(vPos.x));
	}
	else if (hWnd == level_editor::pEditAIBBPosY->getHWND())
	{
		level_editor::pEditAIBBPosY->getText(szStr, 64);
		sscanf(szStr, "%f", &(vPos.y));
	}
	else if (hWnd == level_editor::pEditAIBBPosZ->getHWND())
	{
		level_editor::pEditAIBBPosZ->getText(szStr, 64);
		sscanf(szStr, "%f", &(vPos.z));
	}

	SAIG_BBSetPos(&vPos);

	return 0;
}

LRESULT SXLevelEditor_ButtonAIBBFinish_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	SAIG_BBCreateFinish();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIQuadsDelSel_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	SAIG_QuadSelectedDelete();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridGen_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridGenerate();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridClear_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridClear();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGPGen_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GraphPointGenerate();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGPClear_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GraphPointClear();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridValidation_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridTestValidation();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIClearAll_Click(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	SAIG_Clear();
	SAIG_BBCreate(&float3(0, 0, 0), &float3(10, 10, 10));
	level_editor::AIGridActivateAll(true);
	return 0;
}