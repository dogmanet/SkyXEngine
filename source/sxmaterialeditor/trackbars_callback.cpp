
#include "trackbars_callback.h"

LRESULT SXMaterialEditor_TrackBarPenetration_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char thickness[64];
	thickness[0] = '0';

	int pos = SXMaterialEditor::TrackBarPenetration->getPos();
	sprintf(thickness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditPenetration->setText(thickness);

	SML_MtlSetPenetration(SXMaterialEditor::IDMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarRoughness_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarRoughness->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditRoughness->setText(roughness);

	SML_MtlSetRoughness(SXMaterialEditor::IDMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarThickness_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char thickness[64];
	thickness[0] = '0';

	int pos = SXMaterialEditor::TrackBarThickness->getPos();
	sprintf(thickness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditThickness->setText(thickness);

	SML_MtlSetThickness(SXMaterialEditor::IDMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarF0_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarF0->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditF0->setText(roughness);

	SML_MtlSetF0(SXMaterialEditor::IDMat, float(pos)*0.01f);

	return 0;
}

//////////

LRESULT SXMaterialEditor_TrackBarUDVSX_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarUDVSX->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditUDVSX->setText(roughness);

	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 0, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSY_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarUDVSY->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditUDVSY->setText(roughness);

	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 1, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSZ_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarUDVSZ->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditUDVSZ->setText(roughness);

	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 2, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSW_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarUDVSW->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditUDVSW->setText(roughness);

	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 3, float(pos)*0.01f);

	return 0;
}

///////

LRESULT SXMaterialEditor_TrackBarUDPSX_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarUDPSX->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditUDPSX->setText(roughness);

	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 0, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSY_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarUDPSY->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditUDPSY->setText(roughness);

	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 1, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSZ_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarUDPSZ->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditUDPSZ->setText(roughness);

	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 2, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSW_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::TrackBarUDPSW->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::EditUDPSW->setText(roughness);

	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 3, float(pos)*0.01f);

	return 0;
}