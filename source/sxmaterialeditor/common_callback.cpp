
#include "common_callback.h"

LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Exit?", "Exit?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hwnd, msg, wParam, lParam);

	return 0;
}

void material_editor::MtlOpen()
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select textures", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		SMtrl_MtlReloadMaterial(SRender_SimModelGetIDMtl(), tmpname);
		SMtrl_MtlSetIsTextureLighting(SRender_SimModelGetIDMtl(), false);
		SGCore_LoadTexAllLoad();

		char headertest[1024];
		sprintf(headertest, "%s - %s | %s", SX_MATERIAL_EDITOR_NAME, tmpname, SKYXENGINE_VERSION4EDITORS);
		material_editor::pJobWindow->setText(headertest);
		material_editor::InitMtl(SRender_SimModelGetIDMtl());
	}
}

void material_editor::MtlSave()
{
	SMtrl_MtlSave(material_editor::idMat);
}

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);

	//файл
	//открыть
	if (id == ID_FILE_OPEN)
	{
		material_editor::MtlOpen();
	}
	//сохранить
	else if (id == ID_FILE_SAVE)
	{
		material_editor::MtlSave();
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
		material_editor::FinalImageUncheckedMenu();
		material_editor::pMainMenu->setCheckItem(id, !material_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_NORMALS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		material_editor::FinalImageUncheckedMenu();
		material_editor::pMainMenu->setCheckItem(id, !material_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_PARAMETERS)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		material_editor::FinalImageUncheckedMenu();
		material_editor::pMainMenu->setCheckItem(id, !material_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_AMBIENTDIFFUSE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		material_editor::FinalImageUncheckedMenu();
		material_editor::pMainMenu->setCheckItem(id, !material_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_SPECULAR)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		material_editor::FinalImageUncheckedMenu();
		material_editor::pMainMenu->setCheckItem(id, !material_editor::pMainMenu->getCheckedItem(id));
	}
	//
	else if (id == ID_FINALIMAGE_LIGHTINGSCENE)
	{
		Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		material_editor::FinalImageUncheckedMenu();
		material_editor::pMainMenu->setCheckItem(id, !material_editor::pMainMenu->getCheckedItem(id));
	}


	return 0;
}

LRESULT SXMaterialEditor_ToolBar_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (material_editor::pButtonTBOpen->getHWND() == handle_elem)
		{
			material_editor::MtlOpen();
		}
		else if (material_editor::pButtonTBSave->getHWND() == handle_elem)
		{
			material_editor::MtlSave();
		}

		else if (material_editor::pCheckBoxTBCam->getHWND() == handle_elem)
		{
			SRender_EditorCameraSetMove(material_editor::pCheckBoxTBCam->getCheck());
		}
		else if (material_editor::pButtonTBView->getHWND() == handle_elem)
		{
			SRender_GetCamera()->setPosition(&(ME_DEFAULT_OBSERVER_POS));
			SRender_GetCamera()->setOrientation(&SMQuaternion(float3(0, 0, 1), 0));
		}

		else if (material_editor::pCheckBoxTBRColor->getHWND() == handle_elem)
		{
			material_editor::FinalImageUncheckedMenu();
			material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_COLOR, true);
			material_editor::pCheckBoxTBRColor->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
		}
		else if (material_editor::pCheckBoxTBRNormal->getHWND() == handle_elem)
		{
			material_editor::FinalImageUncheckedMenu();
			material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, true);
			material_editor::pCheckBoxTBRNormal->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
		}
		else if (material_editor::pCheckBoxTBRParam->getHWND() == handle_elem)
		{
			material_editor::FinalImageUncheckedMenu();
			material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, true);
			material_editor::pCheckBoxTBRParam->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
		}
		else if (material_editor::pCheckBoxTBRAmDiff->getHWND() == handle_elem)
		{
			material_editor::FinalImageUncheckedMenu();
			material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, true);
			material_editor::pCheckBoxTBRAmDiff->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
		}
		else if (material_editor::pCheckBoxTBRSpecular->getHWND() == handle_elem)
		{
			material_editor::FinalImageUncheckedMenu();
			material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, true);
			material_editor::pCheckBoxTBRSpecular->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
		}
		else if (material_editor::pCheckBoxTBRLighting->getHWND() == handle_elem)
		{
			material_editor::FinalImageUncheckedMenu();
			material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
			material_editor::pCheckBoxTBRLighting->setCheck(true);
			Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
		}
	}

	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int *r_resize = (int*)GET_PCVAR_INT("r_resize");

	if (!r_resize)
	{
		r_resize = (int*)GET_PCVAR_INT("r_resize");
		return 0;
	}

	//*r_resize = RENDER_RESIZE_RESIZE;
	return 0;
}

//##########################################################################

LRESULT SXMaterialEditor_JobWindow_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);

	if (Notification == BN_CLICKED)
	{
		HWND handle_elem = (HWND)(lParam);
		if (material_editor::pCheckBoxLighting->getHWND() == handle_elem)
		{
			SMtrl_MtlSetLighting(material_editor::idMat, material_editor::pCheckBoxLighting->getCheck());
		}
		else if (material_editor::pCheckBoxTransparent->getHWND() == handle_elem)
		{
			SMtrl_MtlSetTransparency(material_editor::idMat, material_editor::pCheckBoxTransparent->getCheck());
		}
		else if (material_editor::pCheckBoxTexLighting->getHWND() == handle_elem)
		{
			SMtrl_MtlSetIsTextureLighting(material_editor::idMat, material_editor::pCheckBoxTexLighting->getCheck());
		}
		else if (material_editor::pCheckBoxDoSVVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_MATRIX_VIEW, material_editor::pCheckBoxDoSVVS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSWVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_MATRIX_WORLD, material_editor::pCheckBoxDoSWVS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSWPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_MATRIX_WORLD, material_editor::pCheckBoxDoSWPS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSVPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_MATRIX_VIEW, material_editor::pCheckBoxDoSVPS->getCheck());
		}

		else if (material_editor::pCheckBoxDoSPVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_MATRIX_PROJECTION, material_editor::pCheckBoxDoSPVS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSPPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_MATRIX_PROJECTION, material_editor::pCheckBoxDoSPPS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSWVVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_MATRIX_WORLDVIEW, material_editor::pCheckBoxDoSWVVS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSWVPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_MATRIX_WORLDVIEW, material_editor::pCheckBoxDoSWVPS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSCamposVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_CAMPOS, material_editor::pCheckBoxDoSCamposVS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSWVPVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ, material_editor::pCheckBoxDoSWVPVS->getCheck());
		}

		else if (material_editor::pCheckBoxDoSWVPPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ, material_editor::pCheckBoxDoSWVPPS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSCamposPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_CAMPOS, material_editor::pCheckBoxDoSCamposPS->getCheck());
		}

		else if (material_editor::pCheckBoxUDVSInPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetUserDataVS_InPS(material_editor::idMat, material_editor::pCheckBoxUDVSInPS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSTimeDeltaVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_TIMEDELTA, material_editor::pCheckBoxDoSTimeDeltaVS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSTimeDeltaPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_TIMEDELTA, material_editor::pCheckBoxDoSTimeDeltaPS->getCheck());
		}

		else if (material_editor::pCheckBoxDoSWinSizeVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_WINSIZE, material_editor::pCheckBoxDoSWinSizeVS->getCheck());
		}
		else if (material_editor::pCheckBoxDoSWinSizePS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_WINSIZE, material_editor::pCheckBoxDoSWinSizePS->getCheck());
		}


		else if (material_editor::pCheckBoxUDPSInVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetUserDataPS_InVS(material_editor::idMat, material_editor::pCheckBoxUDPSInVS->getCheck());
		}

		else if (material_editor::pCheckBoxUDVS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdVS(material_editor::idMat, MTL_SHADERSTD_USERDATA, material_editor::pCheckBoxUDVS->getCheck());
		}
		else if (material_editor::pCheckBoxUDPS->getHWND() == handle_elem)
		{
			SMtrl_MtlSetStdPS(material_editor::idMat, MTL_SHADERSTD_USERDATA, material_editor::pCheckBoxUDPS->getCheck());
		}

		else if (material_editor::pCheckBoxAlphaTest->getHWND() == handle_elem)
		{
			SMtrl_MtlSetUsingAlphaTest(material_editor::idMat, material_editor::pCheckBoxAlphaTest->getCheck());
		}
		else if (material_editor::pCheckBoxDestColor->getHWND() == handle_elem)
		{
			SMtrl_MtlSetUseDestColor(material_editor::idMat, material_editor::pCheckBoxDestColor->getCheck());
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		HWND handle_elem = (HWND)(lParam);
		if (material_editor::pComboBoxTypeModel->getHWND() == handle_elem)
		{
			SRender_SimModelSetType((MTLTYPE_MODEL)material_editor::pComboBoxTypeModel->getSel());
			SMtrl_MtlSetTypeModel(SRender_SimModelGetIDMtl(), (MTLTYPE_MODEL)material_editor::pComboBoxTypeModel->getSel());
		}
		else if (material_editor::pComboBoxTestModel->getHWND() == handle_elem)
		{
			SRender_SimModelSetNumCurrModel(material_editor::pComboBoxTestModel->getSel());
		}
		/*else if (material_editor::pComboBoxTypeRefract->getHWND() == handle_elem)
		{
			SMtrl_MtlSetTypeTransparency(material_editor::idMat, (MTLTYPE_TRANSPARENCY)material_editor::pComboBoxTypeRefract->getSel());
		}*/
		else if (material_editor::pComboBoxTypeReflect->getHWND() == handle_elem)
		{
			SMtrl_MtlSetTypeReflection(material_editor::idMat, (MTLTYPE_REFLECT)material_editor::pComboBoxTypeReflect->getSel());
		}
		else if (material_editor::pComboBoxPhysic->getHWND() == handle_elem)
		{
			SMtrl_MtlSetPhysicMaterial(material_editor::idMat, (MTLTYPE_PHYSIC)material_editor::pComboBoxPhysic->getSel());
		}
		else if (material_editor::pComboBoxShaders->getHWND() == handle_elem)
		{
			int sel = material_editor::pComboBoxShaders->getSel();
			if (sel > 0)
			{
				sel -= 1;
				SMtrl_MtlSetVS(material_editor::idMat, material_editor::pShaders->getVS(sel));
				material_editor::pEditVS->setText(material_editor::pShaders->getVS(sel));

				SMtrl_MtlSetPS(material_editor::idMat, material_editor::pShaders->getPS(sel));
				material_editor::pEditPS->setText(material_editor::pShaders->getPS(sel));
			}
		}
		else if (material_editor::pComboBoxParamL->getHWND() == handle_elem)
		{
			int sel = material_editor::pComboBoxParamL->getSel();
			if (sel > 0)
			{
				sel -= 1;
				SMtrl_MtlSetThickness(material_editor::idMat, material_editor::pParamL->getThickness(sel));
				material_editor::pEditThickness->setText(String(material_editor::pParamL->getThickness(sel)).c_str());
				material_editor::pTrackBarThickness->setPos(material_editor::pParamL->getThickness(sel)*100);

				SMtrl_MtlSetRoughness(material_editor::idMat, material_editor::pParamL->getRoughness(sel));
				material_editor::pEditRoughness->setText(String(material_editor::pParamL->getRoughness(sel)).c_str());
				material_editor::pTrackBarRoughness->setPos(material_editor::pParamL->getRoughness(sel) * 100);

				SMtrl_MtlSetF0(material_editor::idMat, material_editor::pParamL->getF0(sel));
				material_editor::pEditF0->setText(String(material_editor::pParamL->getF0(sel)).c_str());
				material_editor::pTrackBarF0->setPos(material_editor::pParamL->getF0(sel) * 100);
			}
		}
	}
	return 0;
}

//////

void material_editor::Nulling()
{
	material_editor::pComboBoxTypeModel;
	material_editor::pComboBoxTestModel;

	material_editor::pComboBoxPhysic->setSel(0);

	material_editor::pEditTex->setText("");
	material_editor::pEditVS->setText("");
	material_editor::pEditPS->setText("");
	
	material_editor::pCheckBoxAlphaTest->setCheck(false);
	material_editor::pCheckBoxDestColor->setCheck(false);
	material_editor::pEditRoughness->setText("0");
	material_editor::pEditThickness->setText("1");
	material_editor::pEditF0->setText("0");
	material_editor::pEditMR->setText("");
	material_editor::pCheckBoxLighting->setCheck(true);
	material_editor::pCheckBoxTransparent->setCheck(false);
	material_editor::pTrackBarRoughness->setPos(0);
	material_editor::pTrackBarThickness->setPos(100);
	material_editor::pTrackBarF0->setPos(0);
	material_editor::pTrackBarUDVSX->setPos(0);
	material_editor::pEditTexLighting->setText("");
	material_editor::pCheckBoxTexLighting->setCheck(false);
	material_editor::pCheckBoxDoSVVS->setCheck(false);
	material_editor::pComboBoxTypeReflect->setSel(0);
	material_editor::pComboBoxPhysic->setSel(0);
	material_editor::pEditMask->setText("");
	material_editor::pEditMG->setText("");
	material_editor::pEditMB->setText("");
	material_editor::pEditMA->setText("");
	material_editor::pEditDR->setText("");
	material_editor::pEditDG->setText("");
	material_editor::pEditDB->setText("");
	material_editor::pEditDA->setText("");

	material_editor::pCheckBoxDoSWVS->setCheck(false);
	material_editor::pCheckBoxDoSWPS->setCheck(false);

	material_editor::pCheckBoxDoSVVS->setCheck(false);
	material_editor::pCheckBoxDoSVPS->setCheck(false);

	material_editor::pCheckBoxDoSPVS->setCheck(false);
	material_editor::pCheckBoxDoSPPS->setCheck(false);

	material_editor::pCheckBoxDoSWVVS->setCheck(false);
	material_editor::pCheckBoxDoSWVPS->setCheck(false);

	material_editor::pCheckBoxDoSCamposVS->setCheck(false);
	material_editor::pCheckBoxDoSCamposPS->setCheck(false);

	material_editor::pCheckBoxDoSTimeDeltaVS->setCheck(false);
	material_editor::pCheckBoxDoSTimeDeltaPS->setCheck(false);

	material_editor::pCheckBoxDoSWVPVS->setCheck(false);
	material_editor::pCheckBoxDoSWVPPS->setCheck(false);

	material_editor::pCheckBoxDoSWinSizeVS->setCheck(false);
	material_editor::pCheckBoxDoSWinSizePS->setCheck(false);


	material_editor::pCheckBoxUDVS->setCheck(false);
	material_editor::pCheckBoxUDPS->setCheck(false);

	material_editor::pCheckBoxUDVSInPS->setCheck(false);

	material_editor::pEditUDVSX->setText("0");
	material_editor::pEditUDVSY->setText("0");
	material_editor::pTrackBarUDVSY->setPos(0);
	material_editor::pEditUDVSZ->setText("0");
	material_editor::pTrackBarUDVSZ->setPos(0);
	material_editor::pEditUDVSW->setText("0");
	material_editor::pTrackBarUDVSW->setPos(0);
	material_editor::pCheckBoxDoSTimeDeltaVS->setCheck(false);
	material_editor::pCheckBoxDoSTimeDeltaPS->setCheck(false);
	material_editor::pCheckBoxUDPSInVS->setCheck(false);
	material_editor::pEditUDPSX->setText("0");
	material_editor::pEditUDPSY->setText("0");
	material_editor::pTrackBarUDPSX->setPos(0);
	material_editor::pTrackBarUDPSY->setPos(0);
	material_editor::pEditUDPSZ->setText("0");
	material_editor::pTrackBarUDPSZ->setPos(0);
	material_editor::pEditUDPSW->setText("0");
	material_editor::pTrackBarUDPSW->setPos(0);

	material_editor::pCheckBoxUDPSInVS->setCheck(false);
	material_editor::pCheckBoxUDVSInPS->setCheck(false);
}

void material_editor::InitMtl(ID id)
{
	SRender_SimModelSetType(SMtrl_MtlGetTypeModel(id));
	material_editor::pComboBoxTypeModel->setSel(SMtrl_MtlGetTypeModel(id));
	material_editor::pComboBoxTestModel;

	material_editor::pComboBoxPhysic->setSel(SMtrl_MtlGetPhysicMaterial(SRender_SimModelGetIDMtl()));

	char tmppath[1024];
	SMtrl_MtlGetTexture(SRender_SimModelGetIDMtl(), tmppath);
	material_editor::pEditTex->setText(tmppath);
	SMtrl_MtlGetVS(SRender_SimModelGetIDMtl(), tmppath);
	material_editor::pEditVS->setText(tmppath);
	SMtrl_MtlGetPS(SRender_SimModelGetIDMtl(), tmppath);
	material_editor::pEditPS->setText(tmppath);

	material_editor::pCheckBoxAlphaTest->setCheck(SMtrl_MtlGetUsingAlphaTest(SRender_SimModelGetIDMtl()));
	material_editor::pCheckBoxDestColor->setCheck(SMtrl_MtlGetUseDestColor(SRender_SimModelGetIDMtl()));

	material_editor::pEditRoughness->setText(String(SMtrl_MtlGetRoughness(SRender_SimModelGetIDMtl())).c_str());
	material_editor::pEditThickness->setText(String(SMtrl_MtlGetThickness(SRender_SimModelGetIDMtl())).c_str());
	material_editor::pEditF0->setText(String(SMtrl_MtlGetF0(SRender_SimModelGetIDMtl())).c_str());
	material_editor::pEditHitChance->setText(String(SMtrl_MtlGetHitChance(SRender_SimModelGetIDMtl())).c_str());
	material_editor::pEditDurability->setText(String(SMtrl_MtlGetDurability(SRender_SimModelGetIDMtl())).c_str());
	material_editor::pEditDensity->setText(String(SMtrl_MtlGetDensity(SRender_SimModelGetIDMtl())).c_str());
	
	material_editor::pCheckBoxLighting->setCheck(SMtrl_MtlGetLighting(SRender_SimModelGetIDMtl()));

	material_editor::pCheckBoxTransparent->setCheck(SMtrl_MtlGetTransparency(SRender_SimModelGetIDMtl()));

	material_editor::pTrackBarRoughness->setPos(SMtrl_MtlGetRoughness(SRender_SimModelGetIDMtl())*100.f);
	material_editor::pTrackBarThickness->setPos(SMtrl_MtlGetThickness(SRender_SimModelGetIDMtl())*100.f);
	material_editor::pTrackBarF0->setPos(SMtrl_MtlGetF0(SRender_SimModelGetIDMtl()) * 100.f);
	material_editor::pTrackBarHitChance->setPos(SMtrl_MtlGetHitChance(SRender_SimModelGetIDMtl()) * 100.f);

	SMtrl_MtlGetTextureLighting(SRender_SimModelGetIDMtl(), tmppath);
	material_editor::pEditTexLighting->setText(tmppath);
	material_editor::pCheckBoxTexLighting->setCheck(SMtrl_MtlGetIsTextureLighting(SRender_SimModelGetIDMtl()));
	
	material_editor::pComboBoxTypeReflect->setSel(SMtrl_MtlGetTypeReflection(SRender_SimModelGetIDMtl()));

	material_editor::pComboBoxPhysic->setSel(0);

	SMtrl_MtlGetMaskTex(SRender_SimModelGetIDMtl(), tmppath);
	material_editor::pEditMask->setText(tmppath);

	SMtrl_MtlGetMRTex(SRender_SimModelGetIDMtl(), 0, tmppath);
	material_editor::pEditMR->setText(tmppath);
	SMtrl_MtlGetMRTex(SRender_SimModelGetIDMtl(), 1, tmppath);
	material_editor::pEditMG->setText(tmppath);
	SMtrl_MtlGetMRTex(SRender_SimModelGetIDMtl(), 2, tmppath);
	material_editor::pEditMB->setText(tmppath);
	SMtrl_MtlGetMRTex(SRender_SimModelGetIDMtl(), 3, tmppath);
	material_editor::pEditMA->setText(tmppath);

	SMtrl_MtlGetDTex(SRender_SimModelGetIDMtl(), 0, tmppath);
	material_editor::pEditDR->setText(tmppath);
	SMtrl_MtlGetDTex(SRender_SimModelGetIDMtl(), 1, tmppath);
	material_editor::pEditDG->setText(tmppath);
	SMtrl_MtlGetDTex(SRender_SimModelGetIDMtl(), 2, tmppath);
	material_editor::pEditDB->setText(tmppath);
	SMtrl_MtlGetDTex(SRender_SimModelGetIDMtl(), 3, tmppath);
	material_editor::pEditDA->setText(tmppath);

	material_editor::pCheckBoxDoSWVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLD));
	material_editor::pCheckBoxDoSWPS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLD));

	material_editor::pCheckBoxDoSVVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_VIEW));
	material_editor::pCheckBoxDoSVPS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_VIEW));

	material_editor::pCheckBoxDoSPVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_PROJECTION));
	material_editor::pCheckBoxDoSPPS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_PROJECTION));

	material_editor::pCheckBoxDoSWVVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEW));
	material_editor::pCheckBoxDoSWVPS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEW));

	material_editor::pCheckBoxDoSCamposVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_CAMPOS));
	material_editor::pCheckBoxDoSCamposPS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_CAMPOS));

	material_editor::pCheckBoxDoSTimeDeltaVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_TIMEDELTA));
	material_editor::pCheckBoxDoSTimeDeltaPS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_TIMEDELTA));

	material_editor::pCheckBoxDoSWVPVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ));
	material_editor::pCheckBoxDoSWVPPS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ));

	material_editor::pCheckBoxDoSWinSizeVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_WINSIZE));
	material_editor::pCheckBoxDoSWinSizePS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_WINSIZE));

	material_editor::pCheckBoxUDVS->setCheck(SMtrl_MtlGetStdVS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_USERDATA));
	material_editor::pCheckBoxUDPS->setCheck(SMtrl_MtlGetStdPS(SRender_SimModelGetIDMtl(), MTL_SHADERSTD_USERDATA));

	//

	//
	material_editor::pCheckBoxUDVSInPS->setCheck(false);

	material_editor::pTrackBarUDVSX->setPos(SMtrl_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 0) * 100.f);
	material_editor::pEditUDVSX->setText(String(SMtrl_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 0)).c_str());

	material_editor::pTrackBarUDVSY->setPos(SMtrl_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 1) * 100.f);
	material_editor::pEditUDVSY->setText(String(SMtrl_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 1)).c_str());
	
	material_editor::pTrackBarUDVSZ->setPos(SMtrl_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 2) * 100.f);
	material_editor::pEditUDVSZ->setText(String(SMtrl_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 2)).c_str());
	
	material_editor::pTrackBarUDVSW->setPos(SMtrl_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 3) * 100.f);
	material_editor::pEditUDVSW->setText(String(SMtrl_MtlGetUserDataVS(SRender_SimModelGetIDMtl(), 3)).c_str());
	
	material_editor::pCheckBoxUDPSInVS->setCheck(false);

	material_editor::pTrackBarUDPSX->setPos(SMtrl_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 0) * 100.f);
	material_editor::pEditUDPSX->setText(String(SMtrl_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 0)).c_str());

	material_editor::pTrackBarUDPSY->setPos(SMtrl_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 1) * 100.f);
	material_editor::pEditUDPSY->setText(String(SMtrl_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 1)).c_str());
	
	material_editor::pTrackBarUDPSZ->setPos(SMtrl_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 2) * 100.f);
	material_editor::pEditUDPSZ->setText(String(SMtrl_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 2)).c_str());
	
	material_editor::pTrackBarUDPSW->setPos(SMtrl_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 3) * 100.f);
	material_editor::pEditUDPSW->setText(String(SMtrl_MtlGetUserDataPS(SRender_SimModelGetIDMtl(), 3)).c_str());

	material_editor::pCheckBoxUDPSInVS->setCheck(SMtrl_MtlGetUserDataPS_InVS(SRender_SimModelGetIDMtl()));
	material_editor::pCheckBoxUDVSInPS->setCheck(SMtrl_MtlGetUserDataVS_InPS(SRender_SimModelGetIDMtl()));
}

void material_editor::FinalImageUncheckedMenu()
{
	material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_COLOR, false);
	material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, false);
	material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, false);
	material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, false);
	material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, false);
	material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, false);

	material_editor::pCheckBoxTBRColor->setCheck(false);
	material_editor::pCheckBoxTBRNormal->setCheck(false);
	material_editor::pCheckBoxTBRParam->setCheck(false);
	material_editor::pCheckBoxTBRAmDiff->setCheck(false);
	material_editor::pCheckBoxTBRSpecular->setCheck(false);
	material_editor::pCheckBoxTBRLighting->setCheck(false);
}