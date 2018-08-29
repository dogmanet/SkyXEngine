
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


	float3 tmpdim;
	float3 tmppos;

	SAIG_BBGetDimensions(&tmpdim);
	SAIG_BBGetPos(&tmppos);

	pEditAIBBDimensionsWidth->setText(String(tmpdim.x).c_str());
	pEditAIBBDimensionsHeight->setText(String(tmpdim.y).c_str());
	pEditAIBBDimensionsDepth->setText(String(tmpdim.z).c_str());

	pEditAIBBPosX->setText(String(tmppos.x).c_str());
	pEditAIBBPosY->setText(String(tmppos.y).c_str());
	pEditAIBBPosZ->setText(String(tmppos.z).c_str());

	pEditAIStatsCountQuads->setText(String((DWORD)SAIG_GridGetCountQuads()).c_str());
	pEditAIStatsCountGP->setText(String((DWORD)SAIG_GraphPointGetCount()).c_str());
	pEditAIStatsCountSplits->setText(String((DWORD)SAIG_GridGetCountSplits()).c_str());
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

LRESULT SXLevelEditor_EditAIBBDimensions_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char txt[64];
	float3 tmpdim;
	SAIG_BBGetDimensions(&tmpdim);

	if (hwnd == level_editor::pEditAIBBDimensionsWidth->getHWND())
	{
		level_editor::pEditAIBBDimensionsWidth->getText(txt, 64);
		sscanf(txt, "%f", &(tmpdim.x));
	}
	else if (hwnd == level_editor::pEditAIBBDimensionsHeight->getHWND())
	{
		level_editor::pEditAIBBDimensionsHeight->getText(txt, 64);
		sscanf(txt, "%f", &(tmpdim.y));
	}
	else if (hwnd == level_editor::pEditAIBBDimensionsDepth->getHWND())
	{
		level_editor::pEditAIBBDimensionsDepth->getText(txt, 64);
		sscanf(txt, "%f", &(tmpdim.z));
	}

	SAIG_BBSetDimensions(&tmpdim);

	return 0;
}

LRESULT SXLevelEditor_EditAIBBPos_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char txt[64];
	float3 tmppos;
	SAIG_BBGetPos(&tmppos);

	if (hwnd == level_editor::pEditAIBBPosX->getHWND())
	{
		level_editor::pEditAIBBPosX->getText(txt, 64);
		sscanf(txt, "%f", &(tmppos.x));
	}
	else if (hwnd == level_editor::pEditAIBBPosY->getHWND())
	{
		level_editor::pEditAIBBPosY->getText(txt, 64);
		sscanf(txt, "%f", &(tmppos.y));
	}
	else if (hwnd == level_editor::pEditAIBBPosZ->getHWND())
	{
		level_editor::pEditAIBBPosZ->getText(txt, 64);
		sscanf(txt, "%f", &(tmppos.z));
	}

	SAIG_BBSetPos(&tmppos);

	return 0;
}

LRESULT SXLevelEditor_ButtonAIBBFinish_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_BBCreateFinish();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIQuadsDelSel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_QuadSelectedDelete();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridGen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridGenerate();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridClear_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridClear();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGPGen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GraphPointGenerate();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGPClear_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GraphPointClear();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridValidation_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridTestValidation();
	level_editor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIClearAll_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_Clear();
	SAIG_BBCreate(&float3(0, 0, 0), &float3(10, 10, 10));
	level_editor::AIGridActivateAll(true);
	return 0;
}