
void GameCActivateAllElems(bool bf)
{
	SXLevelEditor::StaticGameClass->Visible(bf);
	SXLevelEditor::ComboBoxGameClass->Visible(bf);
	SXLevelEditor::ComboBoxGameClass->SetSel(0);
	SXLevelEditor::ListViewGameClass->Visible(bf);
	SXLevelEditor::ComboBoxGameValue->Visible(bf);
	SXLevelEditor::ComboBoxGameValue->Clear();

	SXLevelEditor::MemoGameHelp->Visible(bf);
	SXLevelEditor::MemoGameHelp->SetText("");
}