
#include "common_callback.h"

LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Exit?", "Exit?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hwnd, msg, wParam, lParam);

	return 0;
}

void SXMaterialEditor::MtlOpen()
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SML_MtlReloadMaterial(SRender_SimModelGetIDMtl(), tmpname);
		SGCore_LoadTexLoadTextures();

		char headertest[1024];
		sprintf(headertest, "SXMaterialEditor - %s", tmpname);
		SXMaterialEditor::JobWindow->SetText(headertest);
		SXMaterialEditor::InitMtl(SRender_SimModelGetIDMtl());
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
		Core_0SetCVarInt("final_image", DS_RT_COLOR);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		Core_0SetCVarInt("final_image", DS_RT_NORMAL);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		Core_0SetCVarInt("final_image", DS_RT_PARAM);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		Core_0SetCVarInt("final_image", DS_RT_AMBIENTDIFF);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		Core_0SetCVarInt("final_image", DS_RT_SPECULAR);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->CheckItem(id, !SXMaterialEditor::MainMenu->GetCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		Core_0SetCVarInt("final_image", DS_RT_SCENELIGHT);
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
			SRender_EditorCameraSetMove(SXMaterialEditor::CheckBoxTBCam->GetCheck());
		}
		else if (SXMaterialEditor::ButtonTBView->GetHWND() == handle_elem)
		{
			SRender_GetCamera()->SetPosition(&float3(0, 0, -1.2 * 100));
			SRender_GetCamera()->SetOrientation(SMQuaternion(float3(0, 0, 1), 0));
		}

		else if (SXMaterialEditor::CheckBoxTBRColor->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_COLOR, true);
			SXMaterialEditor::CheckBoxTBRColor->SetCheck(true);
			Core_0SetCVarInt("final_image", DS_RT_COLOR);
		}
		else if (SXMaterialEditor::CheckBoxTBRNormal->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_NORMALS, true);
			SXMaterialEditor::CheckBoxTBRNormal->SetCheck(true);
			Core_0SetCVarInt("final_image", DS_RT_NORMAL);
		}
		else if (SXMaterialEditor::CheckBoxTBRParam->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_PARAMETERS, true);
			SXMaterialEditor::CheckBoxTBRParam->SetCheck(true);
			Core_0SetCVarInt("final_image", DS_RT_PARAM);
		}
		else if (SXMaterialEditor::CheckBoxTBRAmDiff->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			SXMaterialEditor::CheckBoxTBRAmDiff->SetCheck(true);
			Core_0SetCVarInt("final_image", DS_RT_AMBIENTDIFF);
		}
		else if (SXMaterialEditor::CheckBoxTBRSpecular->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_SPECULAR, true);
			SXMaterialEditor::CheckBoxTBRSpecular->SetCheck(true);
			Core_0SetCVarInt("final_image", DS_RT_SPECULAR);
		}
		else if (SXMaterialEditor::CheckBoxTBRLighting->GetHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			SXMaterialEditor::CheckBoxTBRLighting->SetCheck(true);
			Core_0SetCVarInt("final_image", DS_RT_SCENELIGHT);
		}
	}

	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int *resize = (int*)GET_PCVAR_INT("resize");
	*resize = RENDER_RESIZE_RESIZE;
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
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_VIEW, SXMaterialEditor::CheckBoxDoSVVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLD, SXMaterialEditor::CheckBoxDoSWVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLD, SXMaterialEditor::CheckBoxDoSWPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSVPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_VIEW, SXMaterialEditor::CheckBoxDoSVPS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSPVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_PROJECTION, SXMaterialEditor::CheckBoxDoSPVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSPPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_PROJECTION, SXMaterialEditor::CheckBoxDoSPPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLDVIEW, SXMaterialEditor::CheckBoxDoSWVVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLDVIEW, SXMaterialEditor::CheckBoxDoSWVPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSCamposVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_CAMPOS, SXMaterialEditor::CheckBoxDoSCamposVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVPVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ, SXMaterialEditor::CheckBoxDoSWVPVS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSWVPPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ, SXMaterialEditor::CheckBoxDoSWVPPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSCamposPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_CAMPOS, SXMaterialEditor::CheckBoxDoSCamposPS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxUDVSInPS->GetHWND() == handle_elem)
		{
			SML_MtlSetUDVS_InPS(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxUDVSInPS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSTimeDeltaVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_TIMEDELTA, SXMaterialEditor::CheckBoxDoSTimeDeltaVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSTimeDeltaPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_TIMEDELTA, SXMaterialEditor::CheckBoxDoSTimeDeltaPS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSWinSizeVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_WINSIZE, SXMaterialEditor::CheckBoxDoSWinSizeVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWinSizePS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_WINSIZE, SXMaterialEditor::CheckBoxDoSWinSizePS->GetCheck());
		}


		else if (SXMaterialEditor::CheckBoxUDPSInVS->GetHWND() == handle_elem)
		{
			SML_MtlSetUDPS_InVS(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxUDPSInVS->GetCheck());
		}

		else if (SXMaterialEditor::CheckBoxUDVS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_USERDATA, SXMaterialEditor::CheckBoxUDVS->GetCheck());
		}
		else if (SXMaterialEditor::CheckBoxUDPS->GetHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_USERDATA, SXMaterialEditor::CheckBoxUDPS->GetCheck());
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
			SRender_SimModelSetType((MTLTYPE_MODEL)SXMaterialEditor::ComboBoxTypeModel->GetSel());
			SML_MtlSetTypeModel(SRender_SimModelGetIDMtl(), (MTLTYPE_MODEL)SXMaterialEditor::ComboBoxTypeModel->GetSel());
		}
		else if (SXMaterialEditor::ComboBoxTestModel->GetHWND() == handle_elem)
		{
			SRender_SimModelSetNumCurrModel(SXMaterialEditor::ComboBoxTestModel->GetSel());
		}
		else if (SXMaterialEditor::ComboBoxTypeRefract->GetHWND() == handle_elem)
		{
			SML_MtlSetTypeTransparency(SXMaterialEditor::IDMat, (MTLTYPE_TRANSPARENCY)SXMaterialEditor::ComboBoxTypeRefract->GetSel());
		}
		else if (SXMaterialEditor::ComboBoxTypeReflect->GetHWND() == handle_elem)
		{
			SML_MtlSetTypeReflection(SXMaterialEditor::IDMat, (MTLTYPE_REFLECT)SXMaterialEditor::ComboBoxTypeReflect->GetSel());
		}
		else if (SXMaterialEditor::ComboBoxPhysic->GetHWND() == handle_elem)
		{
			SML_MtlSetPhysicMaterial(SXMaterialEditor::IDMat, (MTLTYPE_PHYSIC)SXMaterialEditor::ComboBoxPhysic->GetSel());
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
	SRender_SimModelSetType(SML_MtlGetTypeModel(id));
	SXMaterialEditor::ComboBoxTypeModel->SetSel(SML_MtlGetTypeModel(id));
	SXMaterialEditor::ComboBoxTestModel;

	SXMaterialEditor::ComboBoxPhysic->SetSel(SML_MtlGetPhysicMaterial(SRender_SimModelGetIDMtl()));

	char tmppath[1024];
	SML_MtlGetTexture(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditTex->SetText(tmppath);
	SML_MtlGetVS(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditVS->SetText(tmppath);
	SML_MtlGetPS(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditPS->SetText(tmppath);

	SXMaterialEditor::CheckBoxAlphaTest->SetCheck(SML_MtlGetUsingAlphaTest(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::EditRoughness->SetText(String(SML_MtlGetRoughness(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::EditThickness->SetText(String(SML_MtlGetThickness(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::EditF0->SetText(String(SML_MtlGetF0(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::EditPenetration->SetText(String(SML_MtlGetPenetration(SRender_SimModelGetIDMtl())).c_str());
	
	SXMaterialEditor::CheckBoxLighting->SetCheck(SML_MtlGetLighting(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::TrackBarRoughness->SetPos(SML_MtlGetRoughness(SRender_SimModelGetIDMtl())*100.f);
	SXMaterialEditor::TrackBarThickness->SetPos(SML_MtlGetThickness(SRender_SimModelGetIDMtl())*100.f);
	SXMaterialEditor::TrackBarF0->SetPos(SML_MtlGetF0(SRender_SimModelGetIDMtl()) * 100.f);
	SXMaterialEditor::TrackBarPenetration->SetPos(SML_MtlGetPenetration(SRender_SimModelGetIDMtl()) * 100.f);

	SML_MtlGetTextureLighting(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditTexLighting->SetText(tmppath);
	SXMaterialEditor::CheckBoxTexLighting->SetCheck(SML_MtlGetIsTextureLighting(SRender_SimModelGetIDMtl()));
	
	SXMaterialEditor::ComboBoxTypeRefract->SetSel(SML_MtlGetTypeTransparency(SRender_SimModelGetIDMtl()));
	SXMaterialEditor::ComboBoxTypeReflect->SetSel(SML_MtlGetTypeReflection(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::ComboBoxPhysic->SetSel(0);

	SML_MtlGetMaskTex(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditMask->SetText(tmppath);

	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 0, tmppath);
	SXMaterialEditor::EditMR->SetText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 1, tmppath);
	SXMaterialEditor::EditMG->SetText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 2, tmppath);
	SXMaterialEditor::EditMB->SetText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 3, tmppath);
	SXMaterialEditor::EditMA->SetText(tmppath);

	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 0, tmppath);
	SXMaterialEditor::EditDR->SetText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 1, tmppath);
	SXMaterialEditor::EditDG->SetText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 2, tmppath);
	SXMaterialEditor::EditDB->SetText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 3, tmppath);
	SXMaterialEditor::EditDA->SetText(tmppath);

	SXMaterialEditor::CheckBoxDoSWVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLD));
	SXMaterialEditor::CheckBoxDoSWPS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLD));

	SXMaterialEditor::CheckBoxDoSVVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_VIEW));
	SXMaterialEditor::CheckBoxDoSVPS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_VIEW));

	SXMaterialEditor::CheckBoxDoSPVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_PROJECTION));
	SXMaterialEditor::CheckBoxDoSPPS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_PROJECTION));

	SXMaterialEditor::CheckBoxDoSWVVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEW));
	SXMaterialEditor::CheckBoxDoSWVPS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEW));

	SXMaterialEditor::CheckBoxDoSCamposVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_CAMPOS));
	SXMaterialEditor::CheckBoxDoSCamposPS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_CAMPOS));

	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_TIMEDELTA));
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_TIMEDELTA));

	SXMaterialEditor::CheckBoxDoSWVPVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ));
	SXMaterialEditor::CheckBoxDoSWVPPS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ));

	SXMaterialEditor::CheckBoxDoSWinSizeVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_WINSIZE));
	SXMaterialEditor::CheckBoxDoSWinSizePS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_WINSIZE));

	SXMaterialEditor::CheckBoxUDVS->SetCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_USERDATA));
	SXMaterialEditor::CheckBoxUDPS->SetCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_USERDATA));

	//

	//
	SXMaterialEditor::CheckBoxUDVSInPS->SetCheck(false);

	SXMaterialEditor::TrackBarUDVSX->SetPos(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 0) * 100.f);
	SXMaterialEditor::EditUDVSX->SetText(String(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 0)).c_str());

	SXMaterialEditor::TrackBarUDVSY->SetPos(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 1) * 100.f);
	SXMaterialEditor::EditUDVSY->SetText(String(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 1)).c_str());
	
	SXMaterialEditor::TrackBarUDVSZ->SetPos(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 2) * 100.f);
	SXMaterialEditor::EditUDVSZ->SetText(String(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 2)).c_str());
	
	SXMaterialEditor::TrackBarUDVSW->SetPos(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 3) * 100.f);
	SXMaterialEditor::EditUDVSW->SetText(String(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 3)).c_str());
	
	SXMaterialEditor::CheckBoxUDPSInVS->SetCheck(false);

	SXMaterialEditor::TrackBarUDPSX->SetPos(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 0) * 100.f);
	SXMaterialEditor::EditUDPSX->SetText(String(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 0)).c_str());

	SXMaterialEditor::TrackBarUDPSY->SetPos(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 1) * 100.f);
	SXMaterialEditor::EditUDPSY->SetText(String(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 1)).c_str());
	
	SXMaterialEditor::TrackBarUDPSZ->SetPos(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 2) * 100.f);
	SXMaterialEditor::EditUDPSZ->SetText(String(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 2)).c_str());
	
	SXMaterialEditor::TrackBarUDPSW->SetPos(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 3) * 100.f);
	SXMaterialEditor::EditUDPSW->SetText(String(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 3)).c_str());

	SXMaterialEditor::CheckBoxUDPSInVS->SetCheck(SML_MtlGetUDPS_InVS(SRender_SimModelGetIDMtl()));
	SXMaterialEditor::CheckBoxUDVSInPS->SetCheck(SML_MtlGetUDVS_InPS(SRender_SimModelGetIDMtl()));
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