
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
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select textures", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		SML_MtlReloadMaterial(SRender_SimModelGetIDMtl(), tmpname);
		SGCore_LoadTexAllLoad();

		char headertest[1024];
		sprintf(headertest, "SXMaterialEditor - %s", tmpname);
		SXMaterialEditor::JobWindow->setText(headertest);
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
		Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->setCheckItem(id, !SXMaterialEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->setCheckItem(id, !SXMaterialEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->setCheckItem(id, !SXMaterialEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->setCheckItem(id, !SXMaterialEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->setCheckItem(id, !SXMaterialEditor::MainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::MainMenu->setCheckItem(id, !SXMaterialEditor::MainMenu->getCheckedItem(id));
	}


	return 0;
}

LRESULT SXMaterialEditor_ToolBar_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (SXMaterialEditor::ButtonTBOpen->getHWND() == handle_elem)
		{
			SXMaterialEditor::MtlOpen();
		}
		else if (SXMaterialEditor::ButtonTBSave->getHWND() == handle_elem)
		{
			SXMaterialEditor::MtlSave();
		}

		else if (SXMaterialEditor::CheckBoxTBCam->getHWND() == handle_elem)
		{
			SRender_EditorCameraSetMove(SXMaterialEditor::CheckBoxTBCam->getCheck());
		}
		else if (SXMaterialEditor::ButtonTBView->getHWND() == handle_elem)
		{
			SRender_GetCamera()->setPosition(&float3(0, 0, -1.2 * 100));
			SRender_GetCamera()->setOrientation(&SMQuaternion(float3(0, 0, 1), 0));
		}

		else if (SXMaterialEditor::CheckBoxTBRColor->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_COLOR, true);
			SXMaterialEditor::CheckBoxTBRColor->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		}
		else if (SXMaterialEditor::CheckBoxTBRNormal->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, true);
			SXMaterialEditor::CheckBoxTBRNormal->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		}
		else if (SXMaterialEditor::CheckBoxTBRParam->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, true);
			SXMaterialEditor::CheckBoxTBRParam->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		}
		else if (SXMaterialEditor::CheckBoxTBRAmDiff->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			SXMaterialEditor::CheckBoxTBRAmDiff->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		}
		else if (SXMaterialEditor::CheckBoxTBRSpecular->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, true);
			SXMaterialEditor::CheckBoxTBRSpecular->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		}
		else if (SXMaterialEditor::CheckBoxTBRLighting->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			SXMaterialEditor::CheckBoxTBRLighting->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		}
	}

	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int *r_resize = (int*)GET_PCVAR_INT("r_resize");
	*r_resize = RENDER_RESIZE_RESIZE;
	return 0;
}

///////

LRESULT SXMaterialEditor_JobWindow_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);

	if (Notification == BN_CLICKED)
	{
		HWND handle_elem = (HWND)(lParam);
		if (SXMaterialEditor::CheckBoxLighting->getHWND() == handle_elem)
		{
			SML_MtlSetLighting(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxLighting->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxTexLighting->getHWND() == handle_elem)
		{
			SML_MtlSetIsTextureLighting(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxTexLighting->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSVVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_VIEW, SXMaterialEditor::CheckBoxDoSVVS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLD, SXMaterialEditor::CheckBoxDoSWVS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWPS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLD, SXMaterialEditor::CheckBoxDoSWPS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSVPS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_VIEW, SXMaterialEditor::CheckBoxDoSVPS->getCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSPVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_PROJECTION, SXMaterialEditor::CheckBoxDoSPVS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSPPS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_PROJECTION, SXMaterialEditor::CheckBoxDoSPPS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLDVIEW, SXMaterialEditor::CheckBoxDoSWVVS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVPS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLDVIEW, SXMaterialEditor::CheckBoxDoSWVPS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSCamposVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_CAMPOS, SXMaterialEditor::CheckBoxDoSCamposVS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWVPVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ, SXMaterialEditor::CheckBoxDoSWVPVS->getCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSWVPPS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ, SXMaterialEditor::CheckBoxDoSWVPPS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSCamposPS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_CAMPOS, SXMaterialEditor::CheckBoxDoSCamposPS->getCheck());
		}

		else if (SXMaterialEditor::CheckBoxUDVSInPS->getHWND() == handle_elem)
		{
			SML_MtlSetUDVS_InPS(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxUDVSInPS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSTimeDeltaVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_TIMEDELTA, SXMaterialEditor::CheckBoxDoSTimeDeltaVS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSTimeDeltaPS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_TIMEDELTA, SXMaterialEditor::CheckBoxDoSTimeDeltaPS->getCheck());
		}

		else if (SXMaterialEditor::CheckBoxDoSWinSizeVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_WINSIZE, SXMaterialEditor::CheckBoxDoSWinSizeVS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxDoSWinSizePS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_WINSIZE, SXMaterialEditor::CheckBoxDoSWinSizePS->getCheck());
		}


		else if (SXMaterialEditor::CheckBoxUDPSInVS->getHWND() == handle_elem)
		{
			SML_MtlSetUDPS_InVS(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxUDPSInVS->getCheck());
		}

		else if (SXMaterialEditor::CheckBoxUDVS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDVS(SXMaterialEditor::IDMat, MTL_SHADERSTD_USERDATA, SXMaterialEditor::CheckBoxUDVS->getCheck());
		}
		else if (SXMaterialEditor::CheckBoxUDPS->getHWND() == handle_elem)
		{
			SML_MtlSetSTDPS(SXMaterialEditor::IDMat, MTL_SHADERSTD_USERDATA, SXMaterialEditor::CheckBoxUDPS->getCheck());
		}

		else if (SXMaterialEditor::CheckBoxAlphaTest->getHWND() == handle_elem)
		{
			SML_MtlSetUsingAlphaTest(SXMaterialEditor::IDMat, SXMaterialEditor::CheckBoxAlphaTest->getCheck());
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		HWND handle_elem = (HWND)(lParam);
		if (SXMaterialEditor::ComboBoxTypeModel->getHWND() == handle_elem)
		{
			SRender_SimModelSetType((MTLTYPE_MODEL)SXMaterialEditor::ComboBoxTypeModel->getSel());
			SML_MtlSetTypeModel(SRender_SimModelGetIDMtl(), (MTLTYPE_MODEL)SXMaterialEditor::ComboBoxTypeModel->getSel());
		}
		else if (SXMaterialEditor::ComboBoxTestModel->getHWND() == handle_elem)
		{
			SRender_SimModelSetNumCurrModel(SXMaterialEditor::ComboBoxTestModel->getSel());
		}
		else if (SXMaterialEditor::ComboBoxTypeRefract->getHWND() == handle_elem)
		{
			SML_MtlSetTypeTransparency(SXMaterialEditor::IDMat, (MTLTYPE_TRANSPARENCY)SXMaterialEditor::ComboBoxTypeRefract->getSel());
		}
		else if (SXMaterialEditor::ComboBoxTypeReflect->getHWND() == handle_elem)
		{
			SML_MtlSetTypeReflection(SXMaterialEditor::IDMat, (MTLTYPE_REFLECT)SXMaterialEditor::ComboBoxTypeReflect->getSel());
		}
		else if (SXMaterialEditor::ComboBoxPhysic->getHWND() == handle_elem)
		{
			SML_MtlSetPhysicMaterial(SXMaterialEditor::IDMat, (MTLTYPE_PHYSIC)SXMaterialEditor::ComboBoxPhysic->getSel());
		}
		else if (SXMaterialEditor::ComboBoxShaders->getHWND() == handle_elem)
		{
			int sel = SXMaterialEditor::ComboBoxShaders->getSel();
			if (sel > 0)
			{
				sel -= 1;
				SML_MtlSetVS(SXMaterialEditor::IDMat, SXMaterialEditor::Shaders->GetVS(sel));
				SXMaterialEditor::EditVS->setText(SXMaterialEditor::Shaders->GetVS(sel));

				SML_MtlSetPS(SXMaterialEditor::IDMat, SXMaterialEditor::Shaders->GetPS(sel));
				SXMaterialEditor::EditPS->setText(SXMaterialEditor::Shaders->GetPS(sel));
			}
		}
		else if (SXMaterialEditor::ComboBoxParamL->getHWND() == handle_elem)
		{
			int sel = SXMaterialEditor::ComboBoxParamL->getSel();
			if (sel > 0)
			{
				sel -= 1;
				SML_MtlSetThickness(SXMaterialEditor::IDMat, SXMaterialEditor::ParamL->GetThickness(sel));
				SXMaterialEditor::EditThickness->setText(String(SXMaterialEditor::ParamL->GetThickness(sel)).c_str());
				SXMaterialEditor::TrackBarThickness->setPos(SXMaterialEditor::ParamL->GetThickness(sel)*100);

				SML_MtlSetRoughness(SXMaterialEditor::IDMat, SXMaterialEditor::ParamL->GetRoughness(sel));
				SXMaterialEditor::EditRoughness->setText(String(SXMaterialEditor::ParamL->GetRoughness(sel)).c_str());
				SXMaterialEditor::TrackBarRoughness->setPos(SXMaterialEditor::ParamL->GetRoughness(sel) * 100);

				SML_MtlSetF0(SXMaterialEditor::IDMat, SXMaterialEditor::ParamL->GetF0(sel));
				SXMaterialEditor::EditF0->setText(String(SXMaterialEditor::ParamL->GetF0(sel)).c_str());
				SXMaterialEditor::TrackBarF0->setPos(SXMaterialEditor::ParamL->GetF0(sel) * 100);
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

	SXMaterialEditor::ComboBoxPhysic->setSel(0);

	SXMaterialEditor::EditTex->setText("");
	SXMaterialEditor::EditVS->setText("");
	SXMaterialEditor::EditPS->setText("");
	
	SXMaterialEditor::CheckBoxAlphaTest->setCheck(false);
	SXMaterialEditor::EditRoughness->setText("0");
	SXMaterialEditor::EditThickness->setText("1");
	SXMaterialEditor::EditF0->setText("0");
	SXMaterialEditor::EditMR->setText("");
	SXMaterialEditor::CheckBoxLighting->setCheck(true);
	SXMaterialEditor::TrackBarRoughness->setPos(0);
	SXMaterialEditor::TrackBarThickness->setPos(100);
	SXMaterialEditor::TrackBarF0->setPos(0);
	SXMaterialEditor::TrackBarUDVSX->setPos(0);
	SXMaterialEditor::EditTexLighting->setText("");
	SXMaterialEditor::CheckBoxTexLighting->setCheck(false);
	SXMaterialEditor::ComboBoxTypeRefract->setSel(0);
	SXMaterialEditor::CheckBoxDoSVVS->setCheck(false);
	SXMaterialEditor::ComboBoxTypeReflect->setSel(0);
	SXMaterialEditor::ComboBoxPhysic->setSel(0);
	SXMaterialEditor::EditMask->setText("");
	SXMaterialEditor::EditMG->setText("");
	SXMaterialEditor::EditMB->setText("");
	SXMaterialEditor::EditMA->setText("");
	SXMaterialEditor::EditDR->setText("");
	SXMaterialEditor::EditDG->setText("");
	SXMaterialEditor::EditDB->setText("");
	SXMaterialEditor::EditDA->setText("");

	SXMaterialEditor::CheckBoxDoSWVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSWPS->setCheck(false);

	SXMaterialEditor::CheckBoxDoSVVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSVPS->setCheck(false);

	SXMaterialEditor::CheckBoxDoSPVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSPPS->setCheck(false);

	SXMaterialEditor::CheckBoxDoSWVVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSWVPS->setCheck(false);

	SXMaterialEditor::CheckBoxDoSCamposVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSCamposPS->setCheck(false);

	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->setCheck(false);

	SXMaterialEditor::CheckBoxDoSWVPVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSWVPPS->setCheck(false);

	SXMaterialEditor::CheckBoxDoSWinSizeVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSWinSizePS->setCheck(false);


	SXMaterialEditor::CheckBoxUDVS->setCheck(false);
	SXMaterialEditor::CheckBoxUDPS->setCheck(false);

	SXMaterialEditor::CheckBoxUDVSInPS->setCheck(false);

	SXMaterialEditor::EditUDVSX->setText("0");
	SXMaterialEditor::EditUDVSY->setText("0");
	SXMaterialEditor::TrackBarUDVSY->setPos(0);
	SXMaterialEditor::EditUDVSZ->setText("0");
	SXMaterialEditor::TrackBarUDVSZ->setPos(0);
	SXMaterialEditor::EditUDVSW->setText("0");
	SXMaterialEditor::TrackBarUDVSW->setPos(0);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->setCheck(false);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->setCheck(false);
	SXMaterialEditor::CheckBoxUDPSInVS->setCheck(false);
	SXMaterialEditor::EditUDPSX->setText("0");
	SXMaterialEditor::EditUDPSY->setText("0");
	SXMaterialEditor::TrackBarUDPSX->setPos(0);
	SXMaterialEditor::TrackBarUDPSY->setPos(0);
	SXMaterialEditor::EditUDPSZ->setText("0");
	SXMaterialEditor::TrackBarUDPSZ->setPos(0);
	SXMaterialEditor::EditUDPSW->setText("0");
	SXMaterialEditor::TrackBarUDPSW->setPos(0);

	SXMaterialEditor::CheckBoxUDPSInVS->setCheck(false);
	SXMaterialEditor::CheckBoxUDVSInPS->setCheck(false);
}

void SXMaterialEditor::InitMtl(ID id)
{
	SRender_SimModelSetType(SML_MtlGetTypeModel(id));
	SXMaterialEditor::ComboBoxTypeModel->setSel(SML_MtlGetTypeModel(id));
	SXMaterialEditor::ComboBoxTestModel;

	SXMaterialEditor::ComboBoxPhysic->setSel(SML_MtlGetPhysicMaterial(SRender_SimModelGetIDMtl()));

	char tmppath[1024];
	SML_MtlGetTexture(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditTex->setText(tmppath);
	SML_MtlGetVS(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditVS->setText(tmppath);
	SML_MtlGetPS(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditPS->setText(tmppath);

	SXMaterialEditor::CheckBoxAlphaTest->setCheck(SML_MtlGetUsingAlphaTest(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::EditRoughness->setText(String(SML_MtlGetRoughness(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::EditThickness->setText(String(SML_MtlGetThickness(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::EditF0->setText(String(SML_MtlGetF0(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::EditPenetration->setText(String(SML_MtlGetPenetration(SRender_SimModelGetIDMtl())).c_str());
	
	SXMaterialEditor::CheckBoxLighting->setCheck(SML_MtlGetLighting(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::TrackBarRoughness->setPos(SML_MtlGetRoughness(SRender_SimModelGetIDMtl())*100.f);
	SXMaterialEditor::TrackBarThickness->setPos(SML_MtlGetThickness(SRender_SimModelGetIDMtl())*100.f);
	SXMaterialEditor::TrackBarF0->setPos(SML_MtlGetF0(SRender_SimModelGetIDMtl()) * 100.f);
	SXMaterialEditor::TrackBarPenetration->setPos(SML_MtlGetPenetration(SRender_SimModelGetIDMtl()) * 100.f);

	SML_MtlGetTextureLighting(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditTexLighting->setText(tmppath);
	SXMaterialEditor::CheckBoxTexLighting->setCheck(SML_MtlGetIsTextureLighting(SRender_SimModelGetIDMtl()));
	
	SXMaterialEditor::ComboBoxTypeRefract->setSel(SML_MtlGetTypeTransparency(SRender_SimModelGetIDMtl()));
	SXMaterialEditor::ComboBoxTypeReflect->setSel(SML_MtlGetTypeReflection(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::ComboBoxPhysic->setSel(0);

	SML_MtlGetMaskTex(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::EditMask->setText(tmppath);

	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 0, tmppath);
	SXMaterialEditor::EditMR->setText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 1, tmppath);
	SXMaterialEditor::EditMG->setText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 2, tmppath);
	SXMaterialEditor::EditMB->setText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 3, tmppath);
	SXMaterialEditor::EditMA->setText(tmppath);

	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 0, tmppath);
	SXMaterialEditor::EditDR->setText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 1, tmppath);
	SXMaterialEditor::EditDG->setText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 2, tmppath);
	SXMaterialEditor::EditDB->setText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 3, tmppath);
	SXMaterialEditor::EditDA->setText(tmppath);

	SXMaterialEditor::CheckBoxDoSWVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLD));
	SXMaterialEditor::CheckBoxDoSWPS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLD));

	SXMaterialEditor::CheckBoxDoSVVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_VIEW));
	SXMaterialEditor::CheckBoxDoSVPS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_VIEW));

	SXMaterialEditor::CheckBoxDoSPVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_PROJECTION));
	SXMaterialEditor::CheckBoxDoSPPS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_PROJECTION));

	SXMaterialEditor::CheckBoxDoSWVVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEW));
	SXMaterialEditor::CheckBoxDoSWVPS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEW));

	SXMaterialEditor::CheckBoxDoSCamposVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_CAMPOS));
	SXMaterialEditor::CheckBoxDoSCamposPS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_CAMPOS));

	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_TIMEDELTA));
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_TIMEDELTA));

	SXMaterialEditor::CheckBoxDoSWVPVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ));
	SXMaterialEditor::CheckBoxDoSWVPPS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ));

	SXMaterialEditor::CheckBoxDoSWinSizeVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_WINSIZE));
	SXMaterialEditor::CheckBoxDoSWinSizePS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_WINSIZE));

	SXMaterialEditor::CheckBoxUDVS->setCheck(SML_MtlGetSTDVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_USERDATA));
	SXMaterialEditor::CheckBoxUDPS->setCheck(SML_MtlGetSTDPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_USERDATA));

	//

	//
	SXMaterialEditor::CheckBoxUDVSInPS->setCheck(false);

	SXMaterialEditor::TrackBarUDVSX->setPos(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 0) * 100.f);
	SXMaterialEditor::EditUDVSX->setText(String(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 0)).c_str());

	SXMaterialEditor::TrackBarUDVSY->setPos(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 1) * 100.f);
	SXMaterialEditor::EditUDVSY->setText(String(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 1)).c_str());
	
	SXMaterialEditor::TrackBarUDVSZ->setPos(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 2) * 100.f);
	SXMaterialEditor::EditUDVSZ->setText(String(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 2)).c_str());
	
	SXMaterialEditor::TrackBarUDVSW->setPos(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 3) * 100.f);
	SXMaterialEditor::EditUDVSW->setText(String(SML_MtlGetUDVS(SRender_SimModelGetIDMtl(), 3)).c_str());
	
	SXMaterialEditor::CheckBoxUDPSInVS->setCheck(false);

	SXMaterialEditor::TrackBarUDPSX->setPos(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 0) * 100.f);
	SXMaterialEditor::EditUDPSX->setText(String(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 0)).c_str());

	SXMaterialEditor::TrackBarUDPSY->setPos(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 1) * 100.f);
	SXMaterialEditor::EditUDPSY->setText(String(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 1)).c_str());
	
	SXMaterialEditor::TrackBarUDPSZ->setPos(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 2) * 100.f);
	SXMaterialEditor::EditUDPSZ->setText(String(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 2)).c_str());
	
	SXMaterialEditor::TrackBarUDPSW->setPos(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 3) * 100.f);
	SXMaterialEditor::EditUDPSW->setText(String(SML_MtlGetUDPS(SRender_SimModelGetIDMtl(), 3)).c_str());

	SXMaterialEditor::CheckBoxUDPSInVS->setCheck(SML_MtlGetUDPS_InVS(SRender_SimModelGetIDMtl()));
	SXMaterialEditor::CheckBoxUDVSInPS->setCheck(SML_MtlGetUDVS_InPS(SRender_SimModelGetIDMtl()));
}

void SXMaterialEditor::FinalImageUncheckedMenu()
{
	SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_COLOR, false);
	SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, false);
	SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, false);
	SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, false);
	SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, false);
	SXMaterialEditor::MainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, false);

	SXMaterialEditor::CheckBoxTBRColor->setCheck(false);
	SXMaterialEditor::CheckBoxTBRNormal->setCheck(false);
	SXMaterialEditor::CheckBoxTBRParam->setCheck(false);
	SXMaterialEditor::CheckBoxTBRAmDiff->setCheck(false);
	SXMaterialEditor::CheckBoxTBRSpecular->setCheck(false);
	SXMaterialEditor::CheckBoxTBRLighting->setCheck(false);
}