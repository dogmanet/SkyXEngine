
void SXLevelEditor::AIGridActivateAll(bool bf)
{
	ButtonAIQuadsDelSel->Visible(bf);
	ButtonAIGridClear->Visible(bf);
	ButtonAIGridGen->Visible(bf);
	ButtonAIClearAll->Visible(bf);
	StatiAIBBDimensions->Visible(bf);
	StaticAIBBDimensionsWidth->Visible(bf);
	EditAIBBDimensionsWidth->Visible(bf);
	StaticAIBBDimensionsHeight->Visible(bf);
	EditAIBBDimensionsHeight->Visible(bf);
	StaticAIBBDimensionsDepth->Visible(bf);
	EditAIBBDimensionsDepth->Visible(bf);
	StaticAIBBPos->Visible(bf);
	StaticAIBBPosX->Visible(bf);
	EditAIBBPosX->Visible(bf);
	StaticAIBBPosY->Visible(bf);
	EditAIBBPosY->Visible(bf);
	StaticAIBBPosZ->Visible(bf);
	EditAIBBPosZ->Visible(bf);
	ButtonAIBBFinish->Visible(bf);
	ButtonAIGPGen->Visible(bf);
	ButtonAIGPClear->Visible(bf);
	RadioButtonAIGPAdd->Visible(bf);
	RadioButtonAIGPDel->Visible(bf);
	RadioButtonAIQuadAdd->Visible(bf);
	RadioButtonAIQuadsMSel->Visible(bf);
	RadioButtonAIQuadsSelDel->Visible(bf);
	ButtonAIGridValidation->Visible(bf);
	CheckBoxAIGridMarkedSplits->Visible(bf);
	StaticAIStatistics->Visible(bf);
	StaticAIStatsCountQuads->Visible(bf);
	StaticAIStatsCountGP->Visible(bf);
	StaticAIStatsCountSplits->Visible(bf);
	EditAIStatsCountQuads->Visible(bf);
	EditAIStatsCountGP->Visible(bf);
	EditAIStatsCountSplits->Visible(bf);

	SXLevelEditor::AIGridEnableBB(!(SAIG_BBIsCreatedFinish()));


	float3 tmpdim;
	float3 tmppos;

	SAIG_BBGetDimensions(&tmpdim);
	SAIG_BBGetPos(&tmppos);

	EditAIBBDimensionsWidth->SetText(String(tmpdim.x).c_str());
	EditAIBBDimensionsHeight->SetText(String(tmpdim.y).c_str());
	EditAIBBDimensionsDepth->SetText(String(tmpdim.z).c_str());

	EditAIBBPosX->SetText(String(tmppos.x).c_str());
	EditAIBBPosY->SetText(String(tmppos.y).c_str());
	EditAIBBPosZ->SetText(String(tmppos.z).c_str());

	EditAIStatsCountQuads->SetText(String((DWORD)SAIG_GridGetCountQuads()).c_str());
	EditAIStatsCountGP->SetText(String((DWORD)SAIG_GraphPointGetCount()).c_str());
	EditAIStatsCountSplits->SetText(String((DWORD)SAIG_GridGetCountSplits()).c_str());
}

void SXLevelEditor::AIGridEnableBB(bool bf)
{
	StatiAIBBDimensions->Enable(bf);
	StaticAIBBDimensionsWidth->Enable(bf);
	EditAIBBDimensionsWidth->Enable(bf);
	StaticAIBBDimensionsHeight->Enable(bf);
	EditAIBBDimensionsHeight->Enable(bf);
	StaticAIBBDimensionsDepth->Enable(bf);
	EditAIBBDimensionsDepth->Enable(bf);
	StaticAIBBPos->Enable(bf);
	StaticAIBBPosX->Enable(bf);
	EditAIBBPosX->Enable(bf);
	StaticAIBBPosY->Enable(bf);
	EditAIBBPosY->Enable(bf);
	StaticAIBBPosZ->Enable(bf);
	EditAIBBPosZ->Enable(bf);
	ButtonAIBBFinish->Enable(bf);

	ButtonAIGridClear->Enable(!bf);
	ButtonAIGPGen->Enable(!bf);
	ButtonAIGPClear->Enable(!bf);
	RadioButtonAIGPAdd->Enable(!bf);
	RadioButtonAIGPDel->Enable(!bf);
	RadioButtonAIQuadAdd->Enable(!bf);
	RadioButtonAIQuadsMSel->Enable(!bf);
	RadioButtonAIQuadsSelDel->Enable(!bf);
	ButtonAIGridValidation->Enable(!bf);
	CheckBoxAIGridMarkedSplits->Enable(!bf);
	StaticAIStatistics->Enable(!bf);
	StaticAIStatsCountQuads->Enable(!bf);
	StaticAIStatsCountGP->Enable(!bf);
	StaticAIStatsCountSplits->Enable(!bf);
	EditAIStatsCountQuads->Enable(!bf);
	EditAIStatsCountGP->Enable(!bf);
	EditAIStatsCountSplits->Enable(!bf);
	ButtonAIQuadsDelSel->Enable(!bf);
	ButtonAIGridGen->Enable(!bf);
	ButtonAIClearAll->Enable(!bf);
}

LRESULT SXLevelEditor_EditAIBBDimensions_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char txt[64];
	float3 tmpdim;
	SAIG_BBGetDimensions(&tmpdim);

	if (hwnd == SXLevelEditor::EditAIBBDimensionsWidth->GetHWND())
	{
		SXLevelEditor::EditAIBBDimensionsWidth->GetText(txt, 64);
		sscanf(txt, "%f", &(tmpdim.x));
	}
	else if (hwnd == SXLevelEditor::EditAIBBDimensionsHeight->GetHWND())
	{
		SXLevelEditor::EditAIBBDimensionsHeight->GetText(txt, 64);
		sscanf(txt, "%f", &(tmpdim.y));
	}
	else if (hwnd == SXLevelEditor::EditAIBBDimensionsDepth->GetHWND())
	{
		SXLevelEditor::EditAIBBDimensionsDepth->GetText(txt, 64);
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

	if (hwnd == SXLevelEditor::EditAIBBPosX->GetHWND())
	{
		SXLevelEditor::EditAIBBPosX->GetText(txt, 64);
		sscanf(txt, "%f", &(tmppos.x));
	}
	else if (hwnd == SXLevelEditor::EditAIBBPosY->GetHWND())
	{
		SXLevelEditor::EditAIBBPosY->GetText(txt, 64);
		sscanf(txt, "%f", &(tmppos.y));
	}
	else if (hwnd == SXLevelEditor::EditAIBBPosZ->GetHWND())
	{
		SXLevelEditor::EditAIBBPosZ->GetText(txt, 64);
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