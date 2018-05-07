
#include "aigrid_callback.h"

void SXLevelEditor::AIGridActivateAll(bool bf)
{
	ButtonAIQuadsDelSel->setVisible(bf);
	ButtonAIGridClear->setVisible(bf);
	ButtonAIGridGen->setVisible(bf);
	ButtonAIClearAll->setVisible(bf);
	StatiAIBBDimensions->setVisible(bf);
	StaticAIBBDimensionsWidth->setVisible(bf);
	EditAIBBDimensionsWidth->setVisible(bf);
	StaticAIBBDimensionsHeight->setVisible(bf);
	EditAIBBDimensionsHeight->setVisible(bf);
	StaticAIBBDimensionsDepth->setVisible(bf);
	EditAIBBDimensionsDepth->setVisible(bf);
	StaticAIBBPos->setVisible(bf);
	StaticAIBBPosX->setVisible(bf);
	EditAIBBPosX->setVisible(bf);
	StaticAIBBPosY->setVisible(bf);
	EditAIBBPosY->setVisible(bf);
	StaticAIBBPosZ->setVisible(bf);
	EditAIBBPosZ->setVisible(bf);
	ButtonAIBBFinish->setVisible(bf);
	ButtonAIGPGen->setVisible(bf);
	ButtonAIGPClear->setVisible(bf);
	RadioButtonAIGPAdd->setVisible(bf);
	RadioButtonAIGPDel->setVisible(bf);
	RadioButtonAIQuadAdd->setVisible(bf);
	RadioButtonAIQuadsMSel->setVisible(bf);
	RadioButtonAIQuadsSelDel->setVisible(bf);
	ButtonAIGridValidation->setVisible(bf);
	CheckBoxAIGridMarkedSplits->setVisible(bf);
	StaticAIStatistics->setVisible(bf);
	StaticAIStatsCountQuads->setVisible(bf);
	StaticAIStatsCountGP->setVisible(bf);
	StaticAIStatsCountSplits->setVisible(bf);
	EditAIStatsCountQuads->setVisible(bf);
	EditAIStatsCountGP->setVisible(bf);
	EditAIStatsCountSplits->setVisible(bf);

	SXLevelEditor::AIGridEnableBB(!(SAIG_BBIsCreatedFinish()));


	float3 tmpdim;
	float3 tmppos;

	SAIG_BBGetDimensions(&tmpdim);
	SAIG_BBGetPos(&tmppos);

	EditAIBBDimensionsWidth->setText(String(tmpdim.x).c_str());
	EditAIBBDimensionsHeight->setText(String(tmpdim.y).c_str());
	EditAIBBDimensionsDepth->setText(String(tmpdim.z).c_str());

	EditAIBBPosX->setText(String(tmppos.x).c_str());
	EditAIBBPosY->setText(String(tmppos.y).c_str());
	EditAIBBPosZ->setText(String(tmppos.z).c_str());

	EditAIStatsCountQuads->setText(String((DWORD)SAIG_GridGetCountQuads()).c_str());
	EditAIStatsCountGP->setText(String((DWORD)SAIG_GraphPointGetCount()).c_str());
	EditAIStatsCountSplits->setText(String((DWORD)SAIG_GridGetCountSplits()).c_str());
}

void SXLevelEditor::AIGridEnableBB(bool bf)
{
	StatiAIBBDimensions->setEnable(bf);
	StaticAIBBDimensionsWidth->setEnable(bf);
	EditAIBBDimensionsWidth->setEnable(bf);
	StaticAIBBDimensionsHeight->setEnable(bf);
	EditAIBBDimensionsHeight->setEnable(bf);
	StaticAIBBDimensionsDepth->setEnable(bf);
	EditAIBBDimensionsDepth->setEnable(bf);
	StaticAIBBPos->setEnable(bf);
	StaticAIBBPosX->setEnable(bf);
	EditAIBBPosX->setEnable(bf);
	StaticAIBBPosY->setEnable(bf);
	EditAIBBPosY->setEnable(bf);
	StaticAIBBPosZ->setEnable(bf);
	EditAIBBPosZ->setEnable(bf);
	ButtonAIBBFinish->setEnable(bf);

	ButtonAIGridClear->setEnable(!bf);
	ButtonAIGPGen->setEnable(!bf);
	ButtonAIGPClear->setEnable(!bf);
	RadioButtonAIGPAdd->setEnable(!bf);
	RadioButtonAIGPDel->setEnable(!bf);
	RadioButtonAIQuadAdd->setEnable(!bf);
	RadioButtonAIQuadsMSel->setEnable(!bf);
	RadioButtonAIQuadsSelDel->setEnable(!bf);
	ButtonAIGridValidation->setEnable(!bf);
	CheckBoxAIGridMarkedSplits->setEnable(!bf);
	StaticAIStatistics->setEnable(!bf);
	StaticAIStatsCountQuads->setEnable(!bf);
	StaticAIStatsCountGP->setEnable(!bf);
	StaticAIStatsCountSplits->setEnable(!bf);
	EditAIStatsCountQuads->setEnable(!bf);
	EditAIStatsCountGP->setEnable(!bf);
	EditAIStatsCountSplits->setEnable(!bf);
	ButtonAIQuadsDelSel->setEnable(!bf);
	ButtonAIGridGen->setEnable(!bf);
	ButtonAIClearAll->setEnable(!bf);
}

LRESULT SXLevelEditor_EditAIBBDimensions_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char txt[64];
	float3 tmpdim;
	SAIG_BBGetDimensions(&tmpdim);

	if (hwnd == SXLevelEditor::EditAIBBDimensionsWidth->getHWND())
	{
		SXLevelEditor::EditAIBBDimensionsWidth->getText(txt, 64);
		sscanf(txt, "%f", &(tmpdim.x));
	}
	else if (hwnd == SXLevelEditor::EditAIBBDimensionsHeight->getHWND())
	{
		SXLevelEditor::EditAIBBDimensionsHeight->getText(txt, 64);
		sscanf(txt, "%f", &(tmpdim.y));
	}
	else if (hwnd == SXLevelEditor::EditAIBBDimensionsDepth->getHWND())
	{
		SXLevelEditor::EditAIBBDimensionsDepth->getText(txt, 64);
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

	if (hwnd == SXLevelEditor::EditAIBBPosX->getHWND())
	{
		SXLevelEditor::EditAIBBPosX->getText(txt, 64);
		sscanf(txt, "%f", &(tmppos.x));
	}
	else if (hwnd == SXLevelEditor::EditAIBBPosY->getHWND())
	{
		SXLevelEditor::EditAIBBPosY->getText(txt, 64);
		sscanf(txt, "%f", &(tmppos.y));
	}
	else if (hwnd == SXLevelEditor::EditAIBBPosZ->getHWND())
	{
		SXLevelEditor::EditAIBBPosZ->getText(txt, 64);
		sscanf(txt, "%f", &(tmppos.z));
	}

	SAIG_BBSetPos(&tmppos);

	return 0;
}

LRESULT SXLevelEditor_ButtonAIBBFinish_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_BBCreateFinish();
	SXLevelEditor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIQuadsDelSel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_QuadSelectedDelete();
	SXLevelEditor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridGen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridGenerate();
	SXLevelEditor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridClear_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridClear();
	SXLevelEditor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGPGen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GraphPointGenerate();
	SXLevelEditor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGPClear_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GraphPointClear();
	SXLevelEditor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIGridValidation_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_GridTestValidation();
	SXLevelEditor::AIGridActivateAll(true);
	return 0;
}

LRESULT SXLevelEditor_ButtonAIClearAll_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SAIG_Clear();
	SAIG_BBCreate(&float3(0, 0, 0), &float3(10, 10, 10));
	SXLevelEditor::AIGridActivateAll(true);
	return 0;
}