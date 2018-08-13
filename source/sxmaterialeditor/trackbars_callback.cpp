
#include "trackbars_callback.h"

LRESULT SXMaterialEditor_TrackBarHitChance_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char thickness[64];
	thickness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarHitChance->getPos();
	sprintf(thickness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditHitChance->setText(thickness);

	SML_MtlSetHitChance(SXMaterialEditor::idMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarRoughness_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarRoughness->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditRoughness->setText(roughness);

	SML_MtlSetRoughness(SXMaterialEditor::idMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarThickness_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char thickness[64];
	thickness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarThickness->getPos();
	sprintf(thickness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditThickness->setText(thickness);

	SML_MtlSetThickness(SXMaterialEditor::idMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarF0_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarF0->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditF0->setText(roughness);

	SML_MtlSetF0(SXMaterialEditor::idMat, float(pos)*0.01f);

	return 0;
}

//////////

LRESULT SXMaterialEditor_TrackBarUDVSX_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarUDVSX->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditUDVSX->setText(roughness);

	SML_MtlSetUserDataVS(SXMaterialEditor::idMat, 0, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSY_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarUDVSY->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditUDVSY->setText(roughness);

	SML_MtlSetUserDataVS(SXMaterialEditor::idMat, 1, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSZ_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarUDVSZ->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditUDVSZ->setText(roughness);

	SML_MtlSetUserDataVS(SXMaterialEditor::idMat, 2, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSW_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarUDVSW->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditUDVSW->setText(roughness);

	SML_MtlSetUserDataVS(SXMaterialEditor::idMat, 3, float(pos)*0.01f);

	return 0;
}

///////

LRESULT SXMaterialEditor_TrackBarUDPSX_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarUDPSX->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditUDPSX->setText(roughness);

	SML_MtlSetUserDataPS(SXMaterialEditor::idMat, 0, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSY_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarUDPSY->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditUDPSY->setText(roughness);

	SML_MtlSetUserDataPS(SXMaterialEditor::idMat, 1, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSZ_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarUDPSZ->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditUDPSZ->setText(roughness);

	SML_MtlSetUserDataPS(SXMaterialEditor::idMat, 2, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSW_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = SXMaterialEditor::pTrackBarUDPSW->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	SXMaterialEditor::pEditUDPSW->setText(roughness);

	SML_MtlSetUserDataPS(SXMaterialEditor::idMat, 3, float(pos)*0.01f);

	return 0;
}