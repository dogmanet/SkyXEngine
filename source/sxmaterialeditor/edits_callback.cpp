


//////////

LRESULT SXMaterialEditor_EditSkyBox_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditSkyBox->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditTex_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditTex->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditVS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditVS->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditPS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditPS->GetText(tmptextval, 256);


	return 0;
}


LRESULT SXMaterialEditor_EditTexLighting_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditTexLighting->GetText(tmptextval, 256);


	return 0;
}


LRESULT SXMaterialEditor_EditMask_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditMask->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditMR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditMR->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditMG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditMG->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditMB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditMB->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditMA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditMA->GetText(tmptextval, 256);


	return 0;
}


LRESULT SXMaterialEditor_EditDR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditDR->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditDG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditDG->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditDB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditDB->GetText(tmptextval, 256);


	return 0;
}

LRESULT SXMaterialEditor_EditDA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditDA->GetText(tmptextval, 256);


	return 0;
}

/////////////

LRESULT SXMaterialEditor_EditRoughness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditRoughness->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarRoughness->SetPos(tmpval*100);

	return 0;
}

LRESULT SXMaterialEditor_EditF0_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditF0->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarF0->SetPos(tmpval * 100);

	return 0;
}



LRESULT SXMaterialEditor_EditUDVSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSX->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSX->SetPos(tmpval * 100);

	return 0;
}

LRESULT SXMaterialEditor_EditUDVSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSY->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSY->SetPos(tmpval * 100);

	return 0;
}

LRESULT SXMaterialEditor_EditUDVSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSZ->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSZ->SetPos(tmpval * 100);

	return 0;
}

LRESULT SXMaterialEditor_EditUDVSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSW->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSW->SetPos(tmpval * 100);

	return 0;
}


LRESULT SXMaterialEditor_EditUDPSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSX->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSX->SetPos(tmpval * 100);

	return 0;
}

LRESULT SXMaterialEditor_EditUDPSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSY->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSY->SetPos(tmpval * 100);

	return 0;
}

LRESULT SXMaterialEditor_EditUDPSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSZ->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSZ->SetPos(tmpval * 100);

	return 0;
}

LRESULT SXMaterialEditor_EditUDPSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSW->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSW->SetPos(tmpval * 100);

	return 0;
}