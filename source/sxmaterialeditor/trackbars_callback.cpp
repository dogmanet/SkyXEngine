
#include "trackbars_callback.h"

LRESULT SXMaterialEditor_TrackBarHitChance_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char thickness[64];
	thickness[0] = '0';

	int pos = material_editor::pTrackBarHitChance->getPos();
	sprintf(thickness, "%.2f", float(pos)*0.01f);
	material_editor::pEditHitChance->setText(thickness);

	SMtrl_MtlSetHitChance(material_editor::idMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarRoughness_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarRoughness->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditRoughness->setText(roughness);

	SMtrl_MtlSetRoughness(material_editor::idMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarThickness_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char thickness[64];
	thickness[0] = '0';

	int pos = material_editor::pTrackBarThickness->getPos();
	sprintf(thickness, "%.2f", float(pos)*0.01f);
	material_editor::pEditThickness->setText(thickness);

	SMtrl_MtlSetThickness(material_editor::idMat, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarF0_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarF0->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditF0->setText(roughness);

	SMtrl_MtlSetF0(material_editor::idMat, float(pos)*0.01f);

	return 0;
}

//////////

LRESULT SXMaterialEditor_TrackBarUDVSX_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarUDVSX->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditUDVSX->setText(roughness);

	SMtrl_MtlSetUserDataVS(material_editor::idMat, 0, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSY_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarUDVSY->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditUDVSY->setText(roughness);

	SMtrl_MtlSetUserDataVS(material_editor::idMat, 1, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSZ_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarUDVSZ->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditUDVSZ->setText(roughness);

	SMtrl_MtlSetUserDataVS(material_editor::idMat, 2, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDVSW_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarUDVSW->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditUDVSW->setText(roughness);

	SMtrl_MtlSetUserDataVS(material_editor::idMat, 3, float(pos)*0.01f);

	return 0;
}

///////

LRESULT SXMaterialEditor_TrackBarUDPSX_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarUDPSX->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditUDPSX->setText(roughness);

	SMtrl_MtlSetUserDataPS(material_editor::idMat, 0, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSY_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarUDPSY->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditUDPSY->setText(roughness);

	SMtrl_MtlSetUserDataPS(material_editor::idMat, 1, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSZ_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarUDPSZ->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditUDPSZ->setText(roughness);

	SMtrl_MtlSetUserDataPS(material_editor::idMat, 2, float(pos)*0.01f);

	return 0;
}

LRESULT SXMaterialEditor_TrackBarUDPSW_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char roughness[64];
	roughness[0] = '0';

	int pos = material_editor::pTrackBarUDPSW->getPos();
	sprintf(roughness, "%.2f", float(pos)*0.01f);
	material_editor::pEditUDPSW->setText(roughness);

	SMtrl_MtlSetUserDataPS(material_editor::idMat, 3, float(pos)*0.01f);

	return 0;
}