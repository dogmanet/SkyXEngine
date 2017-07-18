
LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Вы действительно хотите выйти?", "Выход?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hwnd, msg, wParam, lParam);

	return 0;
}

void SXMaterialEditor::MtlOpen()
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SML_MtlReloadMaterial(GData::Editors::SimModel->GetIDMtl(), tmpname);
		SGCore_LoadTexLoadTextures();
		SXMaterialEditor::InitMtl(GData::Editors::SimModel->GetIDMtl());
	}
}

void SXMaterialEditor::MtlSave()
{
	SML_MtlSave(SXMaterialEditor::IDMat);
}

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);

	//файл
	//открыть
	if (id == ID_FILE_OPEN)
	{
		SXMaterialEditor::MtlOpen();
	}
	//сохранить
	else if (id == ID_FILE_SAVE)
	{
		SXMaterialEditor::MtlSave();
	}
	//выход
	else if (id == ID_FILE_EXIT)
	{
		return TrueExit(hwnd, msg, wParam, lParam);
	}

	
	//
	else if (id == ID_FINALIMAGE_COLOR)
	{
		GData::FinalImage = DS_RT::ds_rt_color;
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		GData::FinalImage = DS_RT::ds_rt_normal;
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		GData::FinalImage = DS_RT::ds_rt_param;
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		GData::FinalImage = DS_RT::ds_rt_ambient_diff;
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		GData::FinalImage = DS_RT::ds_rt_specular;
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		GData::FinalImage = DS_RT::ds_rt_scene_light_com;
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}


	return 0;
}

LRESULT SXMaterialEditor_ToolBar_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (SXMaterialEditor::ButtonTBOpen->GetHWND() == handle_elem)
		{
			SXMaterialEditor::MtlOpen();
		}
		else if (SXMaterialEditor::ButtonTBSave->GetHWND() == handle_elem)
		{
			SXMaterialEditor::MtlSave();
		}

		else if (SXMaterialEditor::CheckBoxTBCam->GetHWND() == handle_elem)
		{
			GData::Editors::MoveCamera = SXMaterialEditor::CheckBoxTBCam->GetCheck();
		}
		else if (SXMaterialEditor::ButtonTBView->GetHWND() == handle_elem)
		{
			GData::ObjCamera->SetPosition(&float3(0, 0, -1.2 * 100));
			GData::ObjCamera->SetOrientation(SMQuaternion(float3(0, 0, 1), 0));
		}

		else if (SXMaterialEditor::CheckBoxTBRColor->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_COLOR, true);
			SXMaterialEditor::CheckBoxTBRColor->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_color;
		}
		else if (SXMaterialEditor::CheckBoxTBRNormal->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_NORMALS, true);
			SXMaterialEditor::CheckBoxTBRNormal->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_normal;
		}
		else if (SXMaterialEditor::CheckBoxTBRParam->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_PARAMETERS, true);
			SXMaterialEditor::CheckBoxTBRParam->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_param;
		}
		else if (SXMaterialEditor::CheckBoxTBRAmDiff->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			SXMaterialEditor::CheckBoxTBRAmDiff->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_ambient_diff;
		}
		else if (SXMaterialEditor::CheckBoxTBRSpecular->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_SPECULAR, true);
			SXMaterialEditor::CheckBoxTBRSpecular->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_specular;
		}
		else if (SXMaterialEditor::CheckBoxTBRLighting->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			SXMaterialEditor::CheckBoxTBRLighting->SetCheck(true);
			GData::FinalImage = DS_RT::ds_rt_scene_light_com;
		}
	}

	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GData::ReSize = true;
	return 0;
}

///////

LRESULT SXMaterialEditor_JobWindow_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);

	if (Notification == BN_CLICKED)
	{
		HWND handle_elem = (HWND)(lParam);
		if (SXMaterialEditor::CheckBoxLighting->GetHWND() == handle_elem)
		{
			SML_MtlSetLighting(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxLighting->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxTexLighting->GetHWND() == handle_elem)
		{
			SML_MtlSetIsTextureLighting(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxTexLighting->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSVVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_v, SXMaterialEditor::CheckBoxDoSVVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_w, SXMaterialEditor::CheckBoxDoSWVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_w, SXMaterialEditor::CheckBoxDoSWPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSVPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_v, SXMaterialEditor::CheckBoxDoSVPS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSPVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_p, SXMaterialEditor::CheckBoxDoSPVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSPPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_p, SXMaterialEditor::CheckBoxDoSPPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_wv, SXMaterialEditor::CheckBoxDoSWVVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_wv, SXMaterialEditor::CheckBoxDoSWVPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSCamposVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_campos, SXMaterialEditor::CheckBoxDoSCamposVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVPVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_wvp, SXMaterialEditor::CheckBoxDoSWVPVS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSWVPPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_mat_wvp, SXMaterialEditor::CheckBoxDoSWVPPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSCamposPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_campos, SXMaterialEditor::CheckBoxDoSCamposPS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxUDVSInPS->GetHWND() == handle_elem)
		{
			SML_MtlSetUDVS_InPS(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxUDVSInPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSTimeDeltaVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_timedelta, SXMaterialEditor::CheckBoxDoSTimeDeltaVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSTimeDeltaPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_timedelta, SXMaterialEditor::CheckBoxDoSTimeDeltaPS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSWinSizeVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_winsize, SXMaterialEditor::CheckBoxDoSWinSizeVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWinSizePS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_winsize, SXMaterialEditor::CheckBoxDoSWinSizePS->GetCheck());
		}


		else if (SXMaterialEditor::CheckBoxUDPSInVS->GetHWND() == handle_elem)
		{
			SML_MtlSetUDPS_InVS(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxUDPSInVS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxUDVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_ud, SXMaterialEditor::CheckBoxUDVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxUDPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MtlTransShaderStd::mtss_ud, SXMaterialEditor::CheckBoxUDPS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxAlphaTest->GetHWND() == handle_elem)
		{
			SML_MtlSetUsingAlphaTest(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxAlphaTest->GetCheck());
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		HWND handle_elem = (HWND)(lParam);
		if (SXMaterialEditor::ComboBoxTypeModel->GetHWND() == handle_elem)
		{
			GData::Editors::SimModel->TypeModel = (MtlTypeModel)SXMaterialEditor::ComboBoxTypeModel->GetSel();
			SML_MtlSetTypeModel(GData::Editors::SimModel->GetIDMtl(), (MtlTypeModel)SXMaterialEditor::ComboBoxTypeModel->GetSel());
		}
		else if (SXMaterialEditor::ComboBoxTestModel->GetHWND() == handle_elem)
		{
			GData::Editors::SimModel->CurrRenderModel = SXMaterialEditor::ComboBoxTestModel->GetSel();
		}
		else if (SXMaterialEditor::ComboBoxTypeRefract->GetHWND() == handle_elem)
		{
			SML_MtlSetTypeTransparency(SXMaterialEditor::IDMat, (MtlTypeTransparency)SXMaterialEditor::ComboBoxTypeRefract->GetSel());
		}
		else if (SXMaterialEditor::ComboBoxTypeReflect->GetHWND() == handle_elem)
		{
			SML_MtlSetTypeReflection(SXMaterialEditor::IDMat, (MtlTypeReflect)SXMaterialEditor::ComboBoxTypeReflect->GetSel());
		}
		else if (SXMaterialEditor::ComboBoxPhysic->GetHWND() == handle_elem)
		{
			SML_MtlSetPhysicMaterial(SXMaterialEditor::IDMat,(MtlPhysicType)SXMaterialEditor::ComboBoxPhysic->GetSel());
		}
		else if (SXMaterialEditor::ComboBoxShaders->GetHWND() == handle_elem)
		{
			int sel = SXMaterialEditor::ComboBoxShaders->GetSel();
			if (sel > 0)
			{
				sel -= 1;
				SML_MtlSetVS(SXMaterialEditor::IDMat, SXMaterialEditor::Shaders->GetVS(sel));
				SXMaterialEditor::EditVS->SetText(SXMaterialEditor::Shaders->GetVS(sel));

				SML_MtlSetPS(SXMaterialEditor::IDMat, SXMaterialEditor::Shaders->GetPS(sel));
				SXMaterialEditor::EditPS->SetText(SXMaterialEditor::Shaders->GetPS(sel));
			}
		}
		else if (SXMaterialEditor::ComboBoxParamL->GetHWND() == handle_elem)
		{
			int sel = SXMaterialEditor::ComboBoxParamL->GetSel();
			if (sel > 0)
			{
				sel -= 1;
				SML_MtlSetThickness(SXMaterialEditor::IDMat, SXMaterialEditor::ParamL->GetThickness(sel));
				SXMaterialEditor::EditThickness->SetText(String(SXMaterialEditor::ParamL->GetThickness(sel)).c_str());
				SXMaterialEditor::TrackBarThickness->SetPos(SXMaterialEditor::ParamL->GetThickness(sel)*100);

				SML_MtlSetRoughness(SXMaterialEditor::IDMat, SXMaterialEditor::ParamL->GetRoughness(sel));
				SXMaterialEditor::EditRoughness->SetText(String(SXMaterialEditor::ParamL->GetRoughness(sel)).c_str());
				SXMaterialEditor::TrackBarRoughness->SetPos(SXMaterialEditor::ParamL->GetRoughness(sel) * 100);

				SML_MtlSetF0(SXMaterialEditor::IDMat, SXMaterialEditor::ParamL->GetF0(sel));
				SXMaterialEditor::EditF0->SetText(String(SXMaterialEditor::ParamL->GetF0(sel)).c_str());
				SXMaterialEditor::TrackBarF0->SetPos(SXMaterialEditor::ParamL->GetF0(sel) * 100);
			}
		}
	}
	return 0;
}

//////

void SXMaterialEditor::Nulling()
{
	SXMaterialEditor::ComboBoxTypeModel;
	SXMaterialEditor::ComboBoxTestModel;

	SXMaterialEditor::ComboBoxPhysic->SetSel(0);

	SXMaterialEditor::EditTex->SetText("");
	SXMaterialEditor::EditVS->SetText("");
	SXMaterialEditor::EditPS->SetText("");
	
	SXMaterialEditor::CheckBoxAlphaTest->SetCheck(false);
	SXMaterialEditor::EditRoughness->SetText("0");
	SXMaterialEditor::EditThickness->SetText("1");
	SXMaterialEditor::EditF0->SetText("0");
	SXMaterialEditor::EditMR->SetText("");
	SXMaterialEditor::CheckBoxLighting->SetCheck(true);
	SXMaterialEditor::TrackBarRoughness->SetPos(0);
	SXMaterialEditor::TrackBarThickness->SetPos(100);
	SXMaterialEditor::TrackBarF0->SetPos(0);
	SXMaterialEditor::TrackBarUDVSX->SetPos(0);
	SXMaterialEditor::EditTexLighting->SetText("");
	SXMaterialEditor::CheckBoxTexLighting->SetCheck(false);
	SXMaterialEditor::ComboBoxTypeRefract->SetSel(0);
	SXMaterialEditor::CheckBoxDoSVVS->SetCheck(false);
	SXMaterialEditor::ComboBoxTypeReflect->SetSel(0);
	SXMaterialEditor::ComboBoxPhysic->SetSel(0);
	SXMaterialEditor::EditMask->SetText("");
	SXMaterialEditor::EditMG->SetText("");
	SXMaterialEditor::EditMB->SetText("");
	SXMaterialEditor::EditMA->SetText("");
	SXMaterialEditor::EditDR->SetText("");
	SXMaterialEditor::EditDG->SetText("");
	SXMaterialEditor::EditDB->SetText("");
	SXMaterialEditor::EditDA->SetText("");

	SXMaterialEditor::CheckBoxDoSWVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSWPS->SetCheck(false);

	SXMaterialEditor::CheckBoxDoSVVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSVPS->SetCheck(false);

	SXMaterialEditor::CheckBoxDoSPVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSPPS->SetCheck(false);

	SXMaterialEditor::CheckBoxDoSWVVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSWVPS->SetCheck(false);

	SXMaterialEditor::CheckBoxDoSCamposVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSCamposPS->SetCheck(false);

	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetCheck(false);

	SXMaterialEditor::CheckBoxDoSWVPVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSWVPPS->SetCheck(false);

	SXMaterialEditor::CheckBoxDoSWinSizeVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSWinSizePS->SetCheck(false);


	SXMaterialEditor::CheckBoxUDVS->SetCheck(false);
	SXMaterialEditor::CheckBoxUDPS->SetCheck(false);

	SXMaterialEditor::CheckBoxUDVSInPS->SetCheck(false);

	SXMaterialEditor::EditUDVSX->SetText("0");
	SXMaterialEditor::EditUDVSY->SetText("0");
	SXMaterialEditor::TrackBarUDVSY->SetPos(0);
	SXMaterialEditor::EditUDVSZ->SetText("0");
	SXMaterialEditor::TrackBarUDVSZ->SetPos(0);
	SXMaterialEditor::EditUDVSW->SetText("0");
	SXMaterialEditor::TrackBarUDVSW->SetPos(0);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetCheck(false);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetCheck(false);
	SXMaterialEditor::CheckBoxUDPSInVS->SetCheck(false);
	SXMaterialEditor::EditUDPSX->SetText("0");
	SXMaterialEditor::EditUDPSY->SetText("0");
	SXMaterialEditor::TrackBarUDPSX->SetPos(0);
	SXMaterialEditor::TrackBarUDPSY->SetPos(0);
	SXMaterialEditor::EditUDPSZ->SetText("0");
	SXMaterialEditor::TrackBarUDPSZ->SetPos(0);
	SXMaterialEditor::EditUDPSW->SetText("0");
	SXMaterialEditor::TrackBarUDPSW->SetPos(0);

	SXMaterialEditor::CheckBoxUDPSInVS->SetCheck(false);
	SXMaterialEditor::CheckBoxUDVSInPS->SetCheck(false);
}

void SXMaterialEditor::InitMtl(ID id)
{
	GData::Editors::SimModel->TypeModel = SML_MtlGetTypeModel(id);
	SXMaterialEditor::ComboBoxTypeModel->SetSel(SML_MtlGetTypeModel(id));
	SXMaterialEditor::ComboBoxTestModel;

	SXMaterialEditor::ComboBoxPhysic->SetSel(SML_MtlGetPhysicMaterial(GData::Editors::SimModel->GetIDMtl()));

	char tmppath[1024];
	SML_MtlGetTexture(GData::Editors::SimModel->GetIDMtl(), tmppath);
	SXMaterialEditor::EditTex->SetText(tmppath);
	SML_MtlGetVS(GData::Editors::SimModel->GetIDMtl(), tmppath);
	SXMaterialEditor::EditVS->SetText(tmppath);
	SML_MtlGetPS(GData::Editors::SimModel->GetIDMtl(), tmppath);
	SXMaterialEditor::EditPS->SetText(tmppath);

	SXMaterialEditor::CheckBoxAlphaTest->SetCheck(SML_MtlGetUsingAlphaTest(GData::Editors::SimModel->GetIDMtl()));

	SXMaterialEditor::EditRoughness->SetText(String(SML_MtlGetRoughness(GData::Editors::SimModel->GetIDMtl())).c_str());
	SXMaterialEditor::EditThickness->SetText(String(SML_MtlGetThickness(GData::Editors::SimModel->GetIDMtl())).c_str());
	SXMaterialEditor::EditF0->SetText(String(SML_MtlGetF0(GData::Editors::SimModel->GetIDMtl())).c_str());
	SXMaterialEditor::EditPenetration->SetText(String(SML_MtlGetPenetration(GData::Editors::SimModel->GetIDMtl())).c_str());
	
	SXMaterialEditor::CheckBoxLighting->SetCheck(SML_MtlGetLighting(GData::Editors::SimModel->GetIDMtl()));

	SXMaterialEditor::TrackBarRoughness->SetPos(SML_MtlGetRoughness(GData::Editors::SimModel->GetIDMtl())*100.f);
	SXMaterialEditor::TrackBarThickness->SetPos(SML_MtlGetThickness(GData::Editors::SimModel->GetIDMtl())*100.f);
	SXMaterialEditor::TrackBarF0->SetPos(SML_MtlGetF0(GData::Editors::SimModel->GetIDMtl()) * 100.f);
	SXMaterialEditor::TrackBarPenetration->SetPos(SML_MtlGetPenetration(GData::Editors::SimModel->GetIDMtl()) * 100.f);

	SML_MtlGetTextureLighting(GData::Editors::SimModel->GetIDMtl(), tmppath);
	SXMaterialEditor::EditTexLighting->SetText(tmppath);
	SXMaterialEditor::CheckBoxTexLighting->SetCheck(SML_MtlGetIsTextureLighting(GData::Editors::SimModel->GetIDMtl()));
	
	SXMaterialEditor::ComboBoxTypeRefract->SetSel(SML_MtlGetTypeTransparency(GData::Editors::SimModel->GetIDMtl()));
	SXMaterialEditor::ComboBoxTypeReflect->SetSel(SML_MtlGetTypeReflection(GData::Editors::SimModel->GetIDMtl()));

	SXMaterialEditor::ComboBoxPhysic->SetSel(0);

	SML_MtlGetMaskTex(GData::Editors::SimModel->GetIDMtl(), tmppath);
	SXMaterialEditor::EditMask->SetText(tmppath);

	SML_MtlGetMRTex(GData::Editors::SimModel->GetIDMtl(), 0, tmppath);
	SXMaterialEditor::EditMR->SetText(tmppath);
	SML_MtlGetMRTex(GData::Editors::SimModel->GetIDMtl(), 1, tmppath);
	SXMaterialEditor::EditMG->SetText(tmppath);
	SML_MtlGetMRTex(GData::Editors::SimModel->GetIDMtl(), 2, tmppath);
	SXMaterialEditor::EditMB->SetText(tmppath);
	SML_MtlGetMRTex(GData::Editors::SimModel->GetIDMtl(), 3, tmppath);
	SXMaterialEditor::EditMA->SetText(tmppath);

	SML_MtlGetDTex(GData::Editors::SimModel->GetIDMtl(), 0, tmppath);
	SXMaterialEditor::EditDR->SetText(tmppath);
	SML_MtlGetDTex(GData::Editors::SimModel->GetIDMtl(), 1, tmppath);
	SXMaterialEditor::EditDG->SetText(tmppath);
	SML_MtlGetDTex(GData::Editors::SimModel->GetIDMtl(), 2, tmppath);
	SXMaterialEditor::EditDB->SetText(tmppath);
	SML_MtlGetDTex(GData::Editors::SimModel->GetIDMtl(), 3, tmppath);
	SXMaterialEditor::EditDA->SetText(tmppath);

	SXMaterialEditor::CheckBoxDoSWVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_w));
	SXMaterialEditor::CheckBoxDoSWPS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_w));

	SXMaterialEditor::CheckBoxDoSVVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_v));
	SXMaterialEditor::CheckBoxDoSVPS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_v));

	SXMaterialEditor::CheckBoxDoSPVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_p));
	SXMaterialEditor::CheckBoxDoSPPS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_p));

	SXMaterialEditor::CheckBoxDoSWVVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_wv));
	SXMaterialEditor::CheckBoxDoSWVPS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_wv));

	SXMaterialEditor::CheckBoxDoSCamposVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_campos));
	SXMaterialEditor::CheckBoxDoSCamposPS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_campos));

	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_timedelta));
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_timedelta));

	SXMaterialEditor::CheckBoxDoSWVPVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_wvp));
	SXMaterialEditor::CheckBoxDoSWVPPS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_mat_wvp));

	SXMaterialEditor::CheckBoxDoSWinSizeVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_winsize));
	SXMaterialEditor::CheckBoxDoSWinSizePS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_winsize));

	SXMaterialEditor::CheckBoxUDVS->SetCheck(SML_MtlGetSTDVS(GData::Editors::SimModel->GetIDMtl(), mtss_ud));
	SXMaterialEditor::CheckBoxUDPS->SetCheck(SML_MtlGetSTDPS(GData::Editors::SimModel->GetIDMtl(), mtss_ud));

	//

	//
	SXMaterialEditor::CheckBoxUDVSInPS->SetCheck(false);

	SXMaterialEditor::TrackBarUDVSX->SetPos(SML_MtlGetUDVS(GData::Editors::SimModel->GetIDMtl(), 0) * 100.f);
	SXMaterialEditor::EditUDVSX->SetText(String(SML_MtlGetUDVS(GData::Editors::SimModel->GetIDMtl(), 0)).c_str());

	SXMaterialEditor::TrackBarUDVSY->SetPos(SML_MtlGetUDVS(GData::Editors::SimModel->GetIDMtl(), 1) * 100.f);
	SXMaterialEditor::EditUDVSY->SetText(String(SML_MtlGetUDVS(GData::Editors::SimModel->GetIDMtl(), 1)).c_str());
	
	SXMaterialEditor::TrackBarUDVSZ->SetPos(SML_MtlGetUDVS(GData::Editors::SimModel->GetIDMtl(), 2) * 100.f);
	SXMaterialEditor::EditUDVSZ->SetText(String(SML_MtlGetUDVS(GData::Editors::SimModel->GetIDMtl(), 2)).c_str());
	
	SXMaterialEditor::TrackBarUDVSW->SetPos(SML_MtlGetUDVS(GData::Editors::SimModel->GetIDMtl(), 3) * 100.f);
	SXMaterialEditor::EditUDVSW->SetText(String(SML_MtlGetUDVS(GData::Editors::SimModel->GetIDMtl(), 3)).c_str());
	
	SXMaterialEditor::CheckBoxUDPSInVS->SetCheck(false);

	SXMaterialEditor::TrackBarUDPSX->SetPos(SML_MtlGetUDPS(GData::Editors::SimModel->GetIDMtl(), 0) * 100.f);
	SXMaterialEditor::EditUDPSX->SetText(String(SML_MtlGetUDPS(GData::Editors::SimModel->GetIDMtl(), 0)).c_str());

	SXMaterialEditor::TrackBarUDPSY->SetPos(SML_MtlGetUDPS(GData::Editors::SimModel->GetIDMtl(), 1) * 100.f);
	SXMaterialEditor::EditUDPSY->SetText(String(SML_MtlGetUDPS(GData::Editors::SimModel->GetIDMtl(), 1)).c_str());
	
	SXMaterialEditor::TrackBarUDPSZ->SetPos(SML_MtlGetUDPS(GData::Editors::SimModel->GetIDMtl(), 2) * 100.f);
	SXMaterialEditor::EditUDPSZ->SetText(String(SML_MtlGetUDPS(GData::Editors::SimModel->GetIDMtl(), 2)).c_str());
	
	SXMaterialEditor::TrackBarUDPSW->SetPos(SML_MtlGetUDPS(GData::Editors::SimModel->GetIDMtl(), 3) * 100.f);
	SXMaterialEditor::EditUDPSW->SetText(String(SML_MtlGetUDPS(GData::Editors::SimModel->GetIDMtl(), 3)).c_str());

	SXMaterialEditor::CheckBoxUDPSInVS->SetCheck(SML_MtlGetUDPS_InVS(GData::Editors::SimModel->GetIDMtl()));
	SXMaterialEditor::CheckBoxUDVSInPS->SetCheck(SML_MtlGetUDVS_InPS(GData::Editors::SimModel->GetIDMtl()));
}

void SXMaterialEditor::FinalImageUncheckedMenu()
{
	SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_COLOR, false);
	SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_NORMALS, false);
	SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_PARAMETERS, false);
	SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, false);
	SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_SPECULAR, false);
	SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, false);

	SXMaterialEditor::CheckBoxTBRColor->SetCheck(false);
	SXMaterialEditor::CheckBoxTBRNormal->SetCheck(false);
	SXMaterialEditor::CheckBoxTBRParam->SetCheck(false);
	SXMaterialEditor::CheckBoxTBRAmDiff->SetCheck(false);
	SXMaterialEditor::CheckBoxTBRSpecular->SetCheck(false);
	SXMaterialEditor::CheckBoxTBRLighting->SetCheck(false);
}