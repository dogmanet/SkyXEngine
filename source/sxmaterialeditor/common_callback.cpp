
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
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select textures", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		SML_MtlReloadMaterial(SRender_SimModelGetIDMtl(), tmpname);
		SML_MtlSetIsTextureLighting(SRender_SimModelGetIDMtl(), false);
		SGCore_LoadTexAllLoad();

		char headertest[1024];
		sprintf(headertest, "SXMaterialEditor - %s", tmpname);
		SXMaterialEditor::pJobWindow->setText(headertest);
		SXMaterialEditor::InitMtl(SRender_SimModelGetIDMtl());
	}
}

void SXMaterialEditor::MtlSave()
{
	SML_MtlSave(SXMaterialEditor::idMat);
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
		SXMaterialEditor::pMainMenu->setCheckItem(id, !SXMaterialEditor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::pMainMenu->setCheckItem(id, !SXMaterialEditor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::pMainMenu->setCheckItem(id, !SXMaterialEditor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::pMainMenu->setCheckItem(id, !SXMaterialEditor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::pMainMenu->setCheckItem(id, !SXMaterialEditor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		SXMaterialEditor::FinalImageUncheckedMenu();
		SXMaterialEditor::pMainMenu->setCheckItem(id, !SXMaterialEditor::pMainMenu->getCheckedItem(id));
	}


	return 0;
}

LRESULT SXMaterialEditor_ToolBar_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (SXMaterialEditor::pButtonTBOpen->getHWND() == handle_elem)
		{
			SXMaterialEditor::MtlOpen();
		}
		else if (SXMaterialEditor::pButtonTBSave->getHWND() == handle_elem)
		{
			SXMaterialEditor::MtlSave();
		}

		else if (SXMaterialEditor::pCheckBoxTBCam->getHWND() == handle_elem)
		{
			SRender_EditorCameraSetMove(SXMaterialEditor::pCheckBoxTBCam->getCheck());
		}
		else if (SXMaterialEditor::pButtonTBView->getHWND() == handle_elem)
		{
			SRender_GetCamera()->setPosition(&float3(0, 0, -1.2 * 100));
			SRender_GetCamera()->setOrientation(&SMQuaternion(float3(0, 0, 1), 0));
		}

		else if (SXMaterialEditor::pCheckBoxTBRColor->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_COLOR, true);
			SXMaterialEditor::pCheckBoxTBRColor->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		}
		else if (SXMaterialEditor::pCheckBoxTBRNormal->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, true);
			SXMaterialEditor::pCheckBoxTBRNormal->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		}
		else if (SXMaterialEditor::pCheckBoxTBRParam->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, true);
			SXMaterialEditor::pCheckBoxTBRParam->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		}
		else if (SXMaterialEditor::pCheckBoxTBRAmDiff->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			SXMaterialEditor::pCheckBoxTBRAmDiff->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		}
		else if (SXMaterialEditor::pCheckBoxTBRSpecular->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, true);
			SXMaterialEditor::pCheckBoxTBRSpecular->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		}
		else if (SXMaterialEditor::pCheckBoxTBRLighting->getHWND() == handle_elem)
		{
			SXMaterialEditor::FinalImageUncheckedMenu();
			SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			SXMaterialEditor::pCheckBoxTBRLighting->setCheck(true);
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

//##########################################################################

LRESULT SXMaterialEditor_JobWindow_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);

	if (Notification == BN_CLICKED)
	{
		HWND handle_elem = (HWND)(lParam);
		if (SXMaterialEditor::pCheckBoxLighting->getHWND() == handle_elem)
		{
			SML_MtlSetLighting(SXMaterialEditor::idMat, SXMaterialEditor::pCheckBoxLighting->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxTransparent->getHWND() == handle_elem)
		{
			SML_MtlSetTransparency(SXMaterialEditor::idMat, SXMaterialEditor::pCheckBoxTransparent->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxTexLighting->getHWND() == handle_elem)
		{
			SML_MtlSetIsTextureLighting(SXMaterialEditor::idMat, SXMaterialEditor::pCheckBoxTexLighting->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSVVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_VIEW, SXMaterialEditor::pCheckBoxDoSVVS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSWVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_WORLD, SXMaterialEditor::pCheckBoxDoSWVS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSWPS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_WORLD, SXMaterialEditor::pCheckBoxDoSWPS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSVPS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_VIEW, SXMaterialEditor::pCheckBoxDoSVPS->getCheck());
		}

		else if (SXMaterialEditor::pCheckBoxDoSPVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_PROJECTION, SXMaterialEditor::pCheckBoxDoSPVS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSPPS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_PROJECTION, SXMaterialEditor::pCheckBoxDoSPPS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSWVVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_WORLDVIEW, SXMaterialEditor::pCheckBoxDoSWVVS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSWVPS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_WORLDVIEW, SXMaterialEditor::pCheckBoxDoSWVPS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSCamposVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_CAMPOS, SXMaterialEditor::pCheckBoxDoSCamposVS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSWVPVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ, SXMaterialEditor::pCheckBoxDoSWVPVS->getCheck());
		}

		else if (SXMaterialEditor::pCheckBoxDoSWVPPS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ, SXMaterialEditor::pCheckBoxDoSWVPPS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSCamposPS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_CAMPOS, SXMaterialEditor::pCheckBoxDoSCamposPS->getCheck());
		}

		else if (SXMaterialEditor::pCheckBoxUDVSInPS->getHWND() == handle_elem)
		{
			SML_MtlSetUserDataVS_InPS(SXMaterialEditor::idMat, SXMaterialEditor::pCheckBoxUDVSInPS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_TIMEDELTA, SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_TIMEDELTA, SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->getCheck());
		}

		else if (SXMaterialEditor::pCheckBoxDoSWinSizeVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_WINSIZE, SXMaterialEditor::pCheckBoxDoSWinSizeVS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxDoSWinSizePS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_WINSIZE, SXMaterialEditor::pCheckBoxDoSWinSizePS->getCheck());
		}


		else if (SXMaterialEditor::pCheckBoxUDPSInVS->getHWND() == handle_elem)
		{
			SML_MtlSetUserDataPS_InVS(SXMaterialEditor::idMat, SXMaterialEditor::pCheckBoxUDPSInVS->getCheck());
		}

		else if (SXMaterialEditor::pCheckBoxUDVS->getHWND() == handle_elem)
		{
			SML_MtlSetStdVS(SXMaterialEditor::idMat, MTL_SHADERSTD_USERDATA, SXMaterialEditor::pCheckBoxUDVS->getCheck());
		}
		else if (SXMaterialEditor::pCheckBoxUDPS->getHWND() == handle_elem)
		{
			SML_MtlSetStdPS(SXMaterialEditor::idMat, MTL_SHADERSTD_USERDATA, SXMaterialEditor::pCheckBoxUDPS->getCheck());
		}

		else if (SXMaterialEditor::pCheckBoxAlphaTest->getHWND() == handle_elem)
		{
			SML_MtlSetUsingAlphaTest(SXMaterialEditor::idMat, SXMaterialEditor::pCheckBoxAlphaTest->getCheck());
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		HWND handle_elem = (HWND)(lParam);
		if (SXMaterialEditor::pComboBoxTypeModel->getHWND() == handle_elem)
		{
			SRender_SimModelSetType((MTLTYPE_MODEL)SXMaterialEditor::pComboBoxTypeModel->getSel());
			SML_MtlSetTypeModel(SRender_SimModelGetIDMtl(), (MTLTYPE_MODEL)SXMaterialEditor::pComboBoxTypeModel->getSel());
		}
		else if (SXMaterialEditor::pComboBoxTestModel->getHWND() == handle_elem)
		{
			SRender_SimModelSetNumCurrModel(SXMaterialEditor::pComboBoxTestModel->getSel());
		}
		/*else if (SXMaterialEditor::pComboBoxTypeRefract->getHWND() == handle_elem)
		{
			SML_MtlSetTypeTransparency(SXMaterialEditor::idMat, (MTLTYPE_TRANSPARENCY)SXMaterialEditor::pComboBoxTypeRefract->getSel());
		}*/
		else if (SXMaterialEditor::pComboBoxTypeReflect->getHWND() == handle_elem)
		{
			SML_MtlSetTypeReflection(SXMaterialEditor::idMat, (MTLTYPE_REFLECT)SXMaterialEditor::pComboBoxTypeReflect->getSel());
		}
		else if (SXMaterialEditor::pComboBoxPhysic->getHWND() == handle_elem)
		{
			SML_MtlSetPhysicMaterial(SXMaterialEditor::idMat, (MTLTYPE_PHYSIC)SXMaterialEditor::pComboBoxPhysic->getSel());
		}
		else if (SXMaterialEditor::pComboBoxShaders->getHWND() == handle_elem)
		{
			int sel = SXMaterialEditor::pComboBoxShaders->getSel();
			if (sel > 0)
			{
				sel -= 1;
				SML_MtlSetVS(SXMaterialEditor::idMat, SXMaterialEditor::pShaders->getVS(sel));
				SXMaterialEditor::pEditVS->setText(SXMaterialEditor::pShaders->getVS(sel));

				SML_MtlSetPS(SXMaterialEditor::idMat, SXMaterialEditor::pShaders->getPS(sel));
				SXMaterialEditor::pEditPS->setText(SXMaterialEditor::pShaders->getPS(sel));
			}
		}
		else if (SXMaterialEditor::pComboBoxParamL->getHWND() == handle_elem)
		{
			int sel = SXMaterialEditor::pComboBoxParamL->getSel();
			if (sel > 0)
			{
				sel -= 1;
				SML_MtlSetThickness(SXMaterialEditor::idMat, SXMaterialEditor::pParamL->getThickness(sel));
				SXMaterialEditor::pEditThickness->setText(String(SXMaterialEditor::pParamL->getThickness(sel)).c_str());
				SXMaterialEditor::pTrackBarThickness->setPos(SXMaterialEditor::pParamL->getThickness(sel)*100);

				SML_MtlSetRoughness(SXMaterialEditor::idMat, SXMaterialEditor::pParamL->getRoughness(sel));
				SXMaterialEditor::pEditRoughness->setText(String(SXMaterialEditor::pParamL->getRoughness(sel)).c_str());
				SXMaterialEditor::pTrackBarRoughness->setPos(SXMaterialEditor::pParamL->getRoughness(sel) * 100);

				SML_MtlSetF0(SXMaterialEditor::idMat, SXMaterialEditor::pParamL->getF0(sel));
				SXMaterialEditor::pEditF0->setText(String(SXMaterialEditor::pParamL->getF0(sel)).c_str());
				SXMaterialEditor::pTrackBarF0->setPos(SXMaterialEditor::pParamL->getF0(sel) * 100);
			}
		}
	}
	return 0;
}

//////

void SXMaterialEditor::Nulling()
{
	SXMaterialEditor::pComboBoxTypeModel;
	SXMaterialEditor::pComboBoxTestModel;

	SXMaterialEditor::pComboBoxPhysic->setSel(0);

	SXMaterialEditor::pEditTex->setText("");
	SXMaterialEditor::pEditVS->setText("");
	SXMaterialEditor::pEditPS->setText("");
	
	SXMaterialEditor::pCheckBoxAlphaTest->setCheck(false);
	SXMaterialEditor::pEditRoughness->setText("0");
	SXMaterialEditor::pEditThickness->setText("1");
	SXMaterialEditor::pEditF0->setText("0");
	SXMaterialEditor::pEditMR->setText("");
	SXMaterialEditor::pCheckBoxLighting->setCheck(true);
	SXMaterialEditor::pCheckBoxTransparent->setCheck(false);
	SXMaterialEditor::pTrackBarRoughness->setPos(0);
	SXMaterialEditor::pTrackBarThickness->setPos(100);
	SXMaterialEditor::pTrackBarF0->setPos(0);
	SXMaterialEditor::pTrackBarUDVSX->setPos(0);
	SXMaterialEditor::pEditTexLighting->setText("");
	SXMaterialEditor::pCheckBoxTexLighting->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSVVS->setCheck(false);
	SXMaterialEditor::pComboBoxTypeReflect->setSel(0);
	SXMaterialEditor::pComboBoxPhysic->setSel(0);
	SXMaterialEditor::pEditMask->setText("");
	SXMaterialEditor::pEditMG->setText("");
	SXMaterialEditor::pEditMB->setText("");
	SXMaterialEditor::pEditMA->setText("");
	SXMaterialEditor::pEditDR->setText("");
	SXMaterialEditor::pEditDG->setText("");
	SXMaterialEditor::pEditDB->setText("");
	SXMaterialEditor::pEditDA->setText("");

	SXMaterialEditor::pCheckBoxDoSWVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSWPS->setCheck(false);

	SXMaterialEditor::pCheckBoxDoSVVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSVPS->setCheck(false);

	SXMaterialEditor::pCheckBoxDoSPVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSPPS->setCheck(false);

	SXMaterialEditor::pCheckBoxDoSWVVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSWVPS->setCheck(false);

	SXMaterialEditor::pCheckBoxDoSCamposVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSCamposPS->setCheck(false);

	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->setCheck(false);

	SXMaterialEditor::pCheckBoxDoSWVPVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSWVPPS->setCheck(false);

	SXMaterialEditor::pCheckBoxDoSWinSizeVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSWinSizePS->setCheck(false);


	SXMaterialEditor::pCheckBoxUDVS->setCheck(false);
	SXMaterialEditor::pCheckBoxUDPS->setCheck(false);

	SXMaterialEditor::pCheckBoxUDVSInPS->setCheck(false);

	SXMaterialEditor::pEditUDVSX->setText("0");
	SXMaterialEditor::pEditUDVSY->setText("0");
	SXMaterialEditor::pTrackBarUDVSY->setPos(0);
	SXMaterialEditor::pEditUDVSZ->setText("0");
	SXMaterialEditor::pTrackBarUDVSZ->setPos(0);
	SXMaterialEditor::pEditUDVSW->setText("0");
	SXMaterialEditor::pTrackBarUDVSW->setPos(0);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->setCheck(false);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->setCheck(false);
	SXMaterialEditor::pCheckBoxUDPSInVS->setCheck(false);
	SXMaterialEditor::pEditUDPSX->setText("0");
	SXMaterialEditor::pEditUDPSY->setText("0");
	SXMaterialEditor::pTrackBarUDPSX->setPos(0);
	SXMaterialEditor::pTrackBarUDPSY->setPos(0);
	SXMaterialEditor::pEditUDPSZ->setText("0");
	SXMaterialEditor::pTrackBarUDPSZ->setPos(0);
	SXMaterialEditor::pEditUDPSW->setText("0");
	SXMaterialEditor::pTrackBarUDPSW->setPos(0);

	SXMaterialEditor::pCheckBoxUDPSInVS->setCheck(false);
	SXMaterialEditor::pCheckBoxUDVSInPS->setCheck(false);
}

void SXMaterialEditor::InitMtl(ID id)
{
	SRender_SimModelSetType(SML_MtlGetTypeModel(id));
	SXMaterialEditor::pComboBoxTypeModel->setSel(SML_MtlGetTypeModel(id));
	SXMaterialEditor::pComboBoxTestModel;

	SXMaterialEditor::pComboBoxPhysic->setSel(SML_MtlGetPhysicMaterial(SRender_SimModelGetIDMtl()));

	char tmppath[1024];
	SML_MtlGetTexture(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::pEditTex->setText(tmppath);
	SML_MtlGetVS(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::pEditVS->setText(tmppath);
	SML_MtlGetPS(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::pEditPS->setText(tmppath);

	SXMaterialEditor::pCheckBoxAlphaTest->setCheck(SML_MtlGetUsingAlphaTest(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::pEditRoughness->setText(String(SML_MtlGetRoughness(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::pEditThickness->setText(String(SML_MtlGetThickness(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::pEditF0->setText(String(SML_MtlGetF0(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::pEditHitChance->setText(String(SML_MtlGetHitChance(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::pEditDurability->setText(String(SML_MtlGetDurability(SRender_SimModelGetIDMtl())).c_str());
	SXMaterialEditor::pEditDensity->setText(String(SML_MtlGetDensity(SRender_SimModelGetIDMtl())).c_str());
	
	SXMaterialEditor::pCheckBoxLighting->setCheck(SML_MtlGetLighting(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::pCheckBoxTransparent->setCheck(SML_MtlGetTransparency(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::pTrackBarRoughness->setPos(SML_MtlGetRoughness(SRender_SimModelGetIDMtl())*100.f);
	SXMaterialEditor::pTrackBarThickness->setPos(SML_MtlGetThickness(SRender_SimModelGetIDMtl())*100.f);
	SXMaterialEditor::pTrackBarF0->setPos(SML_MtlGetF0(SRender_SimModelGetIDMtl()) * 100.f);
	SXMaterialEditor::pTrackBarHitChance->setPos(SML_MtlGetHitChance(SRender_SimModelGetIDMtl()) * 100.f);

	SML_MtlGetTextureLighting(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::pEditTexLighting->setText(tmppath);
	SXMaterialEditor::pCheckBoxTexLighting->setCheck(SML_MtlGetIsTextureLighting(SRender_SimModelGetIDMtl()));
	
	SXMaterialEditor::pComboBoxTypeReflect->setSel(SML_MtlGetTypeReflection(SRender_SimModelGetIDMtl()));

	SXMaterialEditor::pComboBoxPhysic->setSel(0);

	SML_MtlGetMaskTex(SRender_SimModelGetIDMtl(), tmppath);
	SXMaterialEditor::pEditMask->setText(tmppath);

	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 0, tmppath);
	SXMaterialEditor::pEditMR->setText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 1, tmppath);
	SXMaterialEditor::pEditMG->setText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 2, tmppath);
	SXMaterialEditor::pEditMB->setText(tmppath);
	SML_MtlGetMRTex(SRender_SimModelGetIDMtl(), 3, tmppath);
	SXMaterialEditor::pEditMA->setText(tmppath);

	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 0, tmppath);
	SXMaterialEditor::pEditDR->setText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 1, tmppath);
	SXMaterialEditor::pEditDG->setText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 2, tmppath);
	SXMaterialEditor::pEditDB->setText(tmppath);
	SML_MtlGetDTex(SRender_SimModelGetIDMtl(), 3, tmppath);
	SXMaterialEditor::pEditDA->setText(tmppath);

	SXMaterialEditor::pCheckBoxDoSWVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLD));
	SXMaterialEditor::pCheckBoxDoSWPS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLD));

	SXMaterialEditor::pCheckBoxDoSVVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_VIEW));
	SXMaterialEditor::pCheckBoxDoSVPS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_VIEW));

	SXMaterialEditor::pCheckBoxDoSPVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_PROJECTION));
	SXMaterialEditor::pCheckBoxDoSPPS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_PROJECTION));

	SXMaterialEditor::pCheckBoxDoSWVVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEW));
	SXMaterialEditor::pCheckBoxDoSWVPS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEW));

	SXMaterialEditor::pCheckBoxDoSCamposVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_CAMPOS));
	SXMaterialEditor::pCheckBoxDoSCamposPS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_CAMPOS));

	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_TIMEDELTA));
	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_TIMEDELTA));

	SXMaterialEditor::pCheckBoxDoSWVPVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ));
	SXMaterialEditor::pCheckBoxDoSWVPPS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ));

	SXMaterialEditor::pCheckBoxDoSWinSizeVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_WINSIZE));
	SXMaterialEditor::pCheckBoxDoSWinSizePS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_WINSIZE));

	SXMaterialEditor::pCheckBoxUDVS->setCheck(SML_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_USERDATA));
	SXMaterialEditor::pCheckBoxUDPS->setCheck(SML_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_USERDATA));

	//

	//
	SXMaterialEditor::pCheckBoxUDVSInPS->setCheck(false);

	SXMaterialEditor::pTrackBarUDVSX->setPos(SML_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 0) * 100.f);
	SXMaterialEditor::pEditUDVSX->setText(String(SML_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 0)).c_str());

	SXMaterialEditor::pTrackBarUDVSY->setPos(SML_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 1) * 100.f);
	SXMaterialEditor::pEditUDVSY->setText(String(SML_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 1)).c_str());
	
	SXMaterialEditor::pTrackBarUDVSZ->setPos(SML_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 2) * 100.f);
	SXMaterialEditor::pEditUDVSZ->setText(String(SML_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 2)).c_str());
	
	SXMaterialEditor::pTrackBarUDVSW->setPos(SML_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 3) * 100.f);
	SXMaterialEditor::pEditUDVSW->setText(String(SML_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 3)).c_str());
	
	SXMaterialEditor::pCheckBoxUDPSInVS->setCheck(false);

	SXMaterialEditor::pTrackBarUDPSX->setPos(SML_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 0) * 100.f);
	SXMaterialEditor::pEditUDPSX->setText(String(SML_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 0)).c_str());

	SXMaterialEditor::pTrackBarUDPSY->setPos(SML_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 1) * 100.f);
	SXMaterialEditor::pEditUDPSY->setText(String(SML_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 1)).c_str());
	
	SXMaterialEditor::pTrackBarUDPSZ->setPos(SML_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 2) * 100.f);
	SXMaterialEditor::pEditUDPSZ->setText(String(SML_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 2)).c_str());
	
	SXMaterialEditor::pTrackBarUDPSW->setPos(SML_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 3) * 100.f);
	SXMaterialEditor::pEditUDPSW->setText(String(SML_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 3)).c_str());

	SXMaterialEditor::pCheckBoxUDPSInVS->setCheck(SML_MtlGetUserDataPS_InVS(SRender_SimModelGetIDMtl()));
	SXMaterialEditor::pCheckBoxUDVSInPS->setCheck(SML_MtlGetUserDataVS_InPS(SRender_SimModelGetIDMtl()));
}

void SXMaterialEditor::FinalImageUncheckedMenu()
{
	SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_COLOR, false);
	SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, false);
	SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, false);
	SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, false);
	SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, false);
	SXMaterialEditor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, false);

	SXMaterialEditor::pCheckBoxTBRColor->setCheck(false);
	SXMaterialEditor::pCheckBoxTBRNormal->setCheck(false);
	SXMaterialEditor::pCheckBoxTBRParam->setCheck(false);
	SXMaterialEditor::pCheckBoxTBRAmDiff->setCheck(false);
	SXMaterialEditor::pCheckBoxTBRSpecular->setCheck(false);
	SXMaterialEditor::pCheckBoxTBRLighting->setCheck(false);
}