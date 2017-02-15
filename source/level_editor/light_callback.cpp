
LRESULT SXLevelEditor_LightButtonFinish_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void LCActivateAllElems(bool bf)
{
	SXLevelEditor::StaticName->Visible(bf);
	SXLevelEditor::EditName->Visible(bf);
	SXLevelEditor::EditName->SetText("");
	SXLevelEditor::LightStaticBias->Visible(bf);

	SXLevelEditor::LightCheckBoxShadow->Visible(bf);
	SXLevelEditor::LightCheckBoxShadow->SetCheck(false);
	SXLevelEditor::LightCheckBoxGlobal->Visible(bf);
	SXLevelEditor::LightCheckBoxGlobal->SetCheck(false);
	SXLevelEditor::LightCheckBoxEnable->Visible(bf);
	SXLevelEditor::LightCheckBoxEnable->SetCheck(false);

	SXLevelEditor::LightComboBoxTypeLight->Visible(bf);
	SXLevelEditor::LightComboBoxTypeLight->SetSel(0);
	SXLevelEditor::LightComboBoxTypeLightShadow->Visible(bf);
	SXLevelEditor::LightComboBoxTypeLightShadow->SetSel(0);
	SXLevelEditor::LightStaticFarShadow->Visible(bf);
	SXLevelEditor::LightEditFarShadow->Visible(bf);
	SXLevelEditor::LightEditFarShadow->SetText("0");
	SXLevelEditor::LightStaticPos->Visible(bf);

	SXLevelEditor::LightEditPosX->Visible(bf);
	SXLevelEditor::LightEditPosY->Visible(bf);
	SXLevelEditor::LightEditPosZ->Visible(bf);

	SXLevelEditor::LightEditPosX->SetText("0");
	SXLevelEditor::LightEditPosY->SetText("0");
	SXLevelEditor::LightEditPosZ->SetText("0");

	SXLevelEditor::LightRadioButtonPosX->Visible(bf);
	SXLevelEditor::LightRadioButtonPosY->Visible(bf);
	SXLevelEditor::LightRadioButtonPosZ->Visible(bf);
	
	SXLevelEditor::LightStaticRot->Visible(bf);

	SXLevelEditor::LightEditRotX->Visible(bf);
	SXLevelEditor::LightEditRotY->Visible(bf);
	SXLevelEditor::LightEditRotZ->Visible(bf);

	SXLevelEditor::LightEditRotX->SetText("0");
	SXLevelEditor::LightEditRotY->SetText("0");
	SXLevelEditor::LightEditRotZ->SetText("0");

	SXLevelEditor::LightRadioButtonRotX->Visible(bf);
	SXLevelEditor::LightRadioButtonRotY->Visible(bf);
	SXLevelEditor::LightRadioButtonRotZ->Visible(bf);

	SXLevelEditor::LightRadioButtonRotX->SetCheck(false);
	SXLevelEditor::LightRadioButtonRotY->SetCheck(false);
	SXLevelEditor::LightRadioButtonRotZ->SetCheck(false);

	SXLevelEditor::LightCheckBoxDirOrRot->Visible(bf);
	SXLevelEditor::LightCheckBoxDirOrRot->SetCheck(false);
	SXLevelEditor::LightStaticCubeEdge->Visible(bf);

	SXLevelEditor::LightCheckBoxCubeEdgeXPos->Visible(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->Visible(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->Visible(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->Visible(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->Visible(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->Visible(bf);

	SXLevelEditor::LightCheckBoxCubeEdgeXPos->SetCheck(true);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->SetCheck(true);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->SetCheck(true);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->SetCheck(true);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->SetCheck(true);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->SetCheck(true);

	SXLevelEditor::LightStaticColor->Visible(bf);
	SXLevelEditor::LightEditColorR->Visible(bf);
	SXLevelEditor::LightEditColorG->Visible(bf);
	SXLevelEditor::LightEditColorB->Visible(bf);

	SXLevelEditor::LightEditColorR->SetText("230");
	SXLevelEditor::LightEditColorG->SetText("200");
	SXLevelEditor::LightEditColorB->SetText("180");

	SXLevelEditor::LightStaticPower->Visible(bf);
	SXLevelEditor::LightEditPower->Visible(bf);
	SXLevelEditor::LightEditPower->SetText("10");
	SXLevelEditor::LightStaticDist->Visible(bf);
	SXLevelEditor::LightEditDist->Visible(bf);
	SXLevelEditor::LightEditDist->SetText("100");
	SXLevelEditor::LightButtonColorSel->Visible(bf);
	
	SXLevelEditor::LightStaticColorR->Visible(bf);
	SXLevelEditor::LightStaticColorG->Visible(bf);
	SXLevelEditor::LightStaticColorB->Visible(bf);
	SXLevelEditor::LightStaticTopRadius->Visible(bf);
	SXLevelEditor::LightEditTopRadius->Visible(bf);
	SXLevelEditor::LightEditBias->Visible(bf);
	SXLevelEditor::LightStaticAngle->Visible(bf);
	SXLevelEditor::LightEditAngle->Visible(bf);

	SXLevelEditor::LightEditTopRadius->SetText("0.1");
	SXLevelEditor::LightEditBias->SetText("0");
	SXLevelEditor::LightEditAngle->SetText("2");

	SXLevelEditor::LightStaticSourceMesh->Visible(bf);
	SXLevelEditor::LightEditSourceMesh->Visible(bf);
	SXLevelEditor::LightEditSourceMesh->SetText("0");
	SXLevelEditor::LightButtonSourceMesh->Visible(bf);
	SXLevelEditor::LightComboBoxSourceMesh->Visible(bf);
	SXLevelEditor::LightComboBoxSourceMesh->Clear();

	//SXLevelEditor::LightCheckBoxPosLightOrMesh->Visible(bf);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->Visible(bf);
	//SXLevelEditor::LightCheckBoxPosLightOrMesh->SetCheck(false);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->SetCheck(false);
}

void LCActivateCreatingElems(bool bf)
{
	SXLevelEditor::LightButtonFinish->Visible(bf);
}

void LCActivatePoint(bool bf)
{
	SXLevelEditor::LightComboBoxTypeLightShadow->Enable(bf);
	SXLevelEditor::LightComboBoxTypeLight->Enable(bf);
	SXLevelEditor::LightEditFarShadow->Enable(bf);
	//SXLevelEditor::LightCheckBoxPosLightOrMesh->Enable(bf);
	SXLevelEditor::LightEditPosY->Enable(bf);
	SXLevelEditor::LightEditPosZ->Enable(bf);
	SXLevelEditor::LightRadioButtonPosX->Enable(bf);
	SXLevelEditor::LightRadioButtonPosY->Enable(bf);
	SXLevelEditor::LightRadioButtonPosZ->Enable(bf);
	SXLevelEditor::LightEditPosX->Enable(bf);

	SXLevelEditor::LightCheckBoxCubeEdgeXPos->Enable(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->Enable(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->Enable(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->Enable(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->Enable(bf);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->Enable(bf);

	SXLevelEditor::LightEditPower->Enable(bf);
	SXLevelEditor::LightEditDist->Enable(bf);

	SXLevelEditor::LightEditBias->Enable(bf);

	SXLevelEditor::LightEditSourceMesh->Enable(bf);
	SXLevelEditor::LightEditSourceMesh->SetText("");
	SXLevelEditor::LightButtonSourceMesh->Enable(bf);
	SXLevelEditor::LightComboBoxSourceMesh->Enable(bf);
	SXLevelEditor::LightComboBoxSourceMesh->Clear();
}

void LCActivateDirect(bool bf)
{
	//ISXGUICheckBox* LightCheckBoxEnable;
	//ISXGUIComboBox* LightComboBoxTypeLight;
	SXLevelEditor::LightComboBoxTypeLightShadow->Enable(bf);
	SXLevelEditor::LightComboBoxTypeLight->Enable(bf);
	//SXLevelEditor::LightCheckBoxGlobal->Enable(!bf);
	SXLevelEditor::LightEditFarShadow->Enable(bf);

	//SXLevelEditor::LightCheckBoxPosLightOrMesh->Enable(bf);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->Enable(bf);
	SXLevelEditor::LightEditPosY->Enable(bf);
	SXLevelEditor::LightEditPosZ->Enable(bf);
	SXLevelEditor::LightRadioButtonPosX->Enable(bf);
	SXLevelEditor::LightRadioButtonPosY->Enable(bf);
	SXLevelEditor::LightRadioButtonPosZ->Enable(bf);
	SXLevelEditor::LightEditPosX->Enable(bf);

	SXLevelEditor::LightEditRotX->Enable(bf);
	SXLevelEditor::LightEditRotY->Enable(bf);
	SXLevelEditor::LightEditRotZ->Enable(bf);
	SXLevelEditor::LightRadioButtonRotX->Enable(bf);
	SXLevelEditor::LightRadioButtonRotY->Enable(bf);
	SXLevelEditor::LightRadioButtonRotZ->Enable(bf);
	SXLevelEditor::LightCheckBoxDirOrRot->Enable(bf);

	SXLevelEditor::LightEditPower->Enable(bf);
	
	SXLevelEditor::LightEditDist->Enable(bf);
	
	SXLevelEditor::LightEditTopRadius->Enable(bf);
	SXLevelEditor::LightEditBias->Enable(bf);
	
	SXLevelEditor::LightEditAngle->Enable(bf);

	SXLevelEditor::LightEditSourceMesh->Enable(bf);
	SXLevelEditor::LightEditSourceMesh->SetText("");
	SXLevelEditor::LightButtonSourceMesh->Enable(bf);
	SXLevelEditor::LightComboBoxSourceMesh->Enable(bf);
	SXLevelEditor::LightComboBoxSourceMesh->Clear();
}

void LCActivateGlobal(bool bf)
{
	//SXLevelEditor::LightComboBoxTypeLight->Enable(!bf);

	SXLevelEditor::LightEditPosX->Enable(bf);
	SXLevelEditor::LightEditPosY->Enable(bf);
	SXLevelEditor::LightRadioButtonPosX->Enable(bf);
	SXLevelEditor::LightRadioButtonPosY->Enable(bf);
	
}

void LCActivateType(int type, bool is_new = true)
{
	if (type == LIGHTS_TYPE_GLOBAL)
	{
		LCActivatePoint(false);
		LCActivateDirect(false);
		LCActivateGlobal(true);

		if (is_new)
			SXLevelEditor::LightCheckBoxGlobal->Enable(true);
		else
			SXLevelEditor::LightCheckBoxGlobal->Enable(false);

		SXLevelEditor::LightComboBoxTypeLight->Enable(false);
	}
	else if (type == LIGHTS_TYPE_POINT)
	{
		LCActivateGlobal(false);
		LCActivateDirect(false);
		LCActivatePoint(true);

		if (is_new)
			SXLevelEditor::LightCheckBoxGlobal->Enable(true);
		else
			SXLevelEditor::LightCheckBoxGlobal->Enable(false);

		SXLevelEditor::LightComboBoxTypeLight->Enable(true);
	}
	else if (type == LIGHTS_TYPE_DIRECTION)
	{
		LCActivateGlobal(false);
		LCActivatePoint(false);
		LCActivateDirect(true);

		SXLevelEditor::LightCheckBoxGlobal->Enable(false);
		SXLevelEditor::LightComboBoxTypeLight->Enable(true);
	}
}

void LCInitElemsSelModel(int sel)
{
	if (sel >= 0 && sel < SML_LigthsGetCount())
	{
		sel = SML_LigthsGetIDOfKey(sel);
		SXLevelEditor::LightCheckBoxGlobal->Enable(false);

		LCActivateCreatingElems(false);

		LCActivateDirect(false);
		LCActivatePoint(false);
		LCActivateGlobal(false);

		if (SML_LigthsGetType(sel) == LIGHTS_TYPE_GLOBAL)
			LCActivateGlobal(true);
		else if (SML_LigthsGetType(sel) == LIGHTS_TYPE_POINT)
			LCActivatePoint(true);
		else if (SML_LigthsGetType(sel) == LIGHTS_TYPE_DIRECTION)
			LCActivateDirect(true);

		SXLevelEditor::EditName->SetText(SML_LigthsGetName(sel));
		SXLevelEditor::LightCheckBoxShadow->SetCheck(SML_LigthsIsShadow(sel));
		SXLevelEditor::LightCheckBoxEnable->SetCheck(SML_LigthsIsEnable(sel));
		SXLevelEditor::LightCheckBoxGlobal->SetCheck(SML_LigthsGetType(sel) == LIGHTS_TYPE_GLOBAL);
		SXLevelEditor::LightComboBoxTypeLight->SetSel((SML_LigthsGetType(sel) == LIGHTS_TYPE_GLOBAL || SML_LigthsGetType(sel) == LIGHTS_TYPE_POINT) ? 0 : 1);

		SXLevelEditor::LightCheckBoxCubeEdgeXPos->SetCheck(SML_LigthsGetEnableCubeEdge(sel, 0));
		SXLevelEditor::LightCheckBoxCubeEdgeYPos->SetCheck(SML_LigthsGetEnableCubeEdge(sel, 1));
		SXLevelEditor::LightCheckBoxCubeEdgeZPos->SetCheck(SML_LigthsGetEnableCubeEdge(sel, 2));
		SXLevelEditor::LightCheckBoxCubeEdgeXNeg->SetCheck(SML_LigthsGetEnableCubeEdge(sel, 3));
		SXLevelEditor::LightCheckBoxCubeEdgeYNeg->SetCheck(SML_LigthsGetEnableCubeEdge(sel, 4));
		SXLevelEditor::LightCheckBoxCubeEdgeZNeg->SetCheck(SML_LigthsGetEnableCubeEdge(sel, 5));

		SXLevelEditor::LightEditFarShadow->SetText(String(SML_LigthsGetShadowLocalFar(sel)).c_str());

		float3 pos;
		SML_LigthsGetPos(sel, &pos, SML_LigthsGetType(sel) == LIGHTS_TYPE_GLOBAL);
		SXLevelEditor::LightEditPosX->SetText(String(pos.x).c_str());
		SXLevelEditor::LightEditPosY->SetText(String(pos.y).c_str());
		SXLevelEditor::LightEditPosZ->SetText(String(pos.z).c_str());

		float3 rot;
		SML_LigthsGetRot(sel, &rot,false);
		SXLevelEditor::LightEditRotX->SetText(String(rot.x).c_str());
		SXLevelEditor::LightEditRotY->SetText(String(rot.y).c_str());
		SXLevelEditor::LightEditRotZ->SetText(String(rot.z).c_str());
		SXLevelEditor::LightCheckBoxDirOrRot->SetCheck(false);

		SXLevelEditor::LightRadioButtonPosX->SetCheck(false);
		SXLevelEditor::LightRadioButtonPosY->SetCheck(false);
		SXLevelEditor::LightRadioButtonPosZ->SetCheck(false);

		SXLevelEditor::LightRadioButtonRotX->SetCheck(false);
		SXLevelEditor::LightRadioButtonRotY->SetCheck(false);
		SXLevelEditor::LightRadioButtonRotZ->SetCheck(false);

		float3 color;
		SML_LigthsGetColor(sel, &color);
		SXLevelEditor::LightEditColorR->SetText(String(color.x*255.f).c_str());
		SXLevelEditor::LightEditColorG->SetText(String(color.y*255.f).c_str());
		SXLevelEditor::LightEditColorB->SetText(String(color.z*255.f).c_str());

		float power = SML_LigthsGetPower(sel);
		SXLevelEditor::LightEditPower->SetText(String(power).c_str());

		float dist = SML_LigthsGetDist(sel);
		SXLevelEditor::LightEditDist->SetText(String(dist).c_str());

		SXLevelEditor::LightEditTopRadius->SetText(String(SML_LigthsGetTopRadius(sel)).c_str());
		SXLevelEditor::LightEditBias->SetText(String(SML_LigthsGetBias(sel)).c_str());
		SXLevelEditor::LightEditAngle->SetText(String(SML_LigthsGetAngle(sel)).c_str());

		SXLevelEditor::LightComboBoxTypeLightShadow->SetSel(SML_LigthsGetTypeShadowed(sel));

		if (SML_LigthsExistsSource(sel))
		{
			SXLevelEditor::LightEditSourceMesh->SetText(SML_LigthsGetPathNameSource(sel));

			SXLevelEditor::LightComboBoxSourceMesh->Clear();

			for (int i = 0; i < SML_LigthsGetCountGroupSource(sel); ++i)
			{
				SXLevelEditor::LightComboBoxSourceMesh->AddItem(SML_LigthsGetNameGroupSource(SML_LigthsGetIDOfKey(sel), i));
			}

			SXLevelEditor::LightComboBoxSourceMesh->SetSel(SML_LigthsGetBindedGroupSource(SML_LigthsGetIDOfKey(sel)));
		}
	}
}

////////////

LRESULT SXLevelEditor_LightButtonColorSel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.hwndOwner = hwnd;
	cc.lStructSize = sizeof(CHOOSECOLOR);
	COLORREF cust_colors[16];
	cc.lpCustColors = cust_colors;
	char bufr[160], bufg[160], bufb[160];

	SXLevelEditor::LightEditColorR->GetText(bufr, 4);
	SXLevelEditor::LightEditColorG->GetText(bufg, 4);
	SXLevelEditor::LightEditColorB->GetText(bufb, 4);

	int tmpr = 0, tmpg = 0, tmpb = 0;
	sscanf(bufr, "%d", &tmpr);
	sscanf(bufg, "%d", &tmpg);
	sscanf(bufb, "%d", &tmpb);
	cc.rgbResult = RGB(tmpr, tmpg, tmpb);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{
		if (cc.rgbResult != RGB(tmpr, tmpg, tmpb))
		{
			tmpr = GetRValue(cc.rgbResult);
			tmpg = GetGValue(cc.rgbResult);
			tmpb = GetBValue(cc.rgbResult);
			sprintf(bufr, "%d", tmpr);
			sprintf(bufg, "%d", tmpg);
			sprintf(bufb, "%d", tmpb);

			SXLevelEditor::LightEditColorR->SetText(bufr);
			SXLevelEditor::LightEditColorG->SetText(bufg);
			SXLevelEditor::LightEditColorB->SetText(bufb);

			if (SXLevelEditor::HowActivateType == 3)
			{
				int sel = SXLevelEditor::ListBoxList->GetSel();
				if (sel >= 0 && sel < SML_LigthsGetCount())
				{
					SML_LigthsSetColor(SML_LigthsGetIDOfKey(sel), &float3(255.f / float(tmpr), 255.f / float(tmpg), 255.f / float(tmpb)));
				}
			}

		}
	}

	return 1;
}

LRESULT SXLevelEditor_LightButtonSourceMesh_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	DialogLoadMesh(tmppath);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::LightEditSourceMesh->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (SXLevelEditor::HowActivateType == 3)
		{
			if (sel >= 0 && sel < SML_LigthsGetCount())
			{
				sel = SML_LigthsGetIDOfKey(sel);
				SML_LigthsLoadSource(sel, tmpname);

				if (SML_LigthsExistsSource(sel))
				{
					SXLevelEditor::LightEditSourceMesh->SetText(SML_LigthsGetPathNameSource(sel));

					SXLevelEditor::LightComboBoxSourceMesh->Clear();

					for (int i = 0; i < SML_LigthsGetCountGroupSource(sel); ++i)
					{
						SXLevelEditor::LightComboBoxSourceMesh->AddItem(SML_LigthsGetNameGroupSource(sel, i));
					}

					SXLevelEditor::LightComboBoxSourceMesh->SetSel(SML_LigthsGetBindedGroupSource(sel));
				}
			}
		}
	}
	return 0;
}

LRESULT SXLevelEditor_LightEditColor_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmpcoord[256];
	tmpcoord[0] = 0;
	DWORD getcoord = 0;
	DWORD numsel = SXLevelEditor::ListBoxList->GetSel();
	float3 color;

	if (numsel >= 0 && numsel < SML_LigthsGetCount())
	{
		if (SXLevelEditor::LightEditColorR->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditColorR->GetText(tmpcoord, 256);
			sscanf(tmpcoord, "%d", &getcoord);
			SML_LigthsGetColor(SML_LigthsGetIDOfKey(numsel), &color);
			color.x = float(getcoord) / 255.f;
			SML_LigthsSetColor(SML_LigthsGetIDOfKey(numsel), &color);
		}
		else if (SXLevelEditor::LightEditColorG->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditColorG->GetText(tmpcoord, 256);
			sscanf(tmpcoord, "%d", &getcoord);
			SML_LigthsGetColor(SML_LigthsGetIDOfKey(numsel), &color);
			color.y = float(getcoord) / 255.f;
			SML_LigthsSetColor(SML_LigthsGetIDOfKey(numsel), &color);
		}
		else if (SXLevelEditor::LightEditColorB->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditColorB->GetText(tmpcoord, 256);
			sscanf(tmpcoord, "%d", &getcoord);
			SML_LigthsGetColor(SML_LigthsGetIDOfKey(numsel), &color);
			color.z = float(getcoord) / 255.f;
			SML_LigthsSetColor(SML_LigthsGetIDOfKey(numsel), &color);
		}
		else if (SXLevelEditor::LightEditPower->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditPower->GetText(tmpcoord, 256);
			float rh;
			sscanf(tmpcoord, "%d", &rh);
			SML_LigthsSetDist(SML_LigthsGetIDOfKey(numsel), rh);
		}
	}

	return 0;
}


LRESULT SXLevelEditor_GroupBoxLight_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	if (Notification == BN_CLICKED)
	{
		HWND handle_elem = (HWND)(lParam);
		if (SXLevelEditor::LightCheckBoxGlobal->GetHWND() == handle_elem)
		{
			if (SXLevelEditor::LightCheckBoxGlobal->GetCheck())
			{
				LCActivateType(LIGHTS_TYPE_GLOBAL, SXLevelEditor::HowActivateType != 3);
			}
			else
			{
				LCActivateType(SXLevelEditor::LightComboBoxTypeLight->GetSel() == 0 ? LIGHTS_TYPE_POINT : LIGHTS_TYPE_DIRECTION, SXLevelEditor::HowActivateType != 3);
			}
		}

		else if (SXLevelEditor::HowActivateType == 3)
		{
			DWORD numsel = SXLevelEditor::ListBoxList->GetSel();

			if (SXLevelEditor::LightCheckBoxShadow->GetHWND() == handle_elem)
			{
				if (numsel >= 0 && numsel < SML_LigthsGetCount())
					SML_LigthsSetShadow(SML_LigthsGetIDOfKey(numsel), SXLevelEditor::LightCheckBoxShadow->GetCheck());
			}
			else if (SXLevelEditor::LightCheckBoxEnable->GetHWND() == handle_elem)
			{
				if (numsel != -1 && numsel < SML_LigthsGetCount())
					SML_LigthsSetEnable(SML_LigthsGetIDOfKey(numsel), SXLevelEditor::LightCheckBoxEnable->GetCheck());
			}

			else if (SXLevelEditor::LightCheckBoxCubeEdgeXPos->GetHWND() == handle_elem)
			{
				if (numsel != -1 && numsel < SML_LigthsGetCount())
					SML_LigthsSetEnableCubeEdge(SML_LigthsGetIDOfKey(numsel), 0, SXLevelEditor::LightCheckBoxCubeEdgeXPos->GetCheck());
			}
			else if (SXLevelEditor::LightCheckBoxCubeEdgeXNeg->GetHWND() == handle_elem)
			{
				if (numsel != -1 && numsel < SML_LigthsGetCount())
					SML_LigthsSetEnableCubeEdge(SML_LigthsGetIDOfKey(numsel), 1, SXLevelEditor::LightCheckBoxCubeEdgeXNeg->GetCheck());
			}
			else if (SXLevelEditor::LightCheckBoxCubeEdgeYPos->GetHWND() == handle_elem)
			{
				if (numsel != -1 && numsel < SML_LigthsGetCount())
					SML_LigthsSetEnableCubeEdge(SML_LigthsGetIDOfKey(numsel), 2, SXLevelEditor::LightCheckBoxCubeEdgeYPos->GetCheck());
			}
			else if (SXLevelEditor::LightCheckBoxCubeEdgeYNeg->GetHWND() == handle_elem)
			{
				if (numsel != -1 && numsel < SML_LigthsGetCount())
					SML_LigthsSetEnableCubeEdge(SML_LigthsGetIDOfKey(numsel), 3, SXLevelEditor::LightCheckBoxCubeEdgeYNeg->GetCheck());
			}
			else if (SXLevelEditor::LightCheckBoxCubeEdgeZPos->GetHWND() == handle_elem)
			{
				if (numsel != -1 && numsel < SML_LigthsGetCount())
					SML_LigthsSetEnableCubeEdge(SML_LigthsGetIDOfKey(numsel), 4, SXLevelEditor::LightCheckBoxCubeEdgeZPos->GetCheck());
			}
			else if (SXLevelEditor::LightCheckBoxCubeEdgeZNeg->GetHWND() == handle_elem)
			{
				if (numsel != -1 && numsel < SML_LigthsGetCount())
					SML_LigthsSetEnableCubeEdge(SML_LigthsGetIDOfKey(numsel), 5, SXLevelEditor::LightCheckBoxCubeEdgeZNeg->GetCheck());
			}

			else if (SXLevelEditor::LightCheckBoxDirOrRot->GetHWND() == handle_elem)
			{
				if (numsel >= 0 && numsel < SML_LigthsGetCount())
				{
					float3 rot;
					if (SXLevelEditor::LightCheckBoxDirOrRot->GetCheck())
						SML_LigthsGetDir(SML_LigthsGetIDOfKey(numsel), &rot, SXLevelEditor::LightCheckBoxRotLightOrMesh->GetCheck());
					else
						SML_LigthsGetRot(SML_LigthsGetIDOfKey(numsel), &rot, SXLevelEditor::LightCheckBoxRotLightOrMesh->GetCheck());

					char tmpRotX[32];
					char tmpRotY[32];
					char tmpRotZ[32];

					sprintf(tmpRotX, "%f", rot.x);
					sprintf(tmpRotY, "%f", rot.y);
					sprintf(tmpRotZ, "%f", rot.z);

					SXLevelEditor::LightEditRotX->SetText(tmpRotX);
					SXLevelEditor::LightEditRotY->SetText(tmpRotY);
					SXLevelEditor::LightEditRotZ->SetText(tmpRotZ);
				}
			}

			else if (SXLevelEditor::LightCheckBoxRotLightOrMesh->GetHWND() == handle_elem)
			{
				if (numsel >= 0 && numsel < SML_LigthsGetCount())
				{
					float3 rot;
					if (SXLevelEditor::LightCheckBoxDirOrRot->GetCheck())
						SML_LigthsGetDir(SML_LigthsGetIDOfKey(numsel), &rot, SXLevelEditor::LightCheckBoxRotLightOrMesh->GetCheck());
					else
						SML_LigthsGetRot(SML_LigthsGetIDOfKey(numsel), &rot, SXLevelEditor::LightCheckBoxRotLightOrMesh->GetCheck());

					char tmpRotX[32];
					char tmpRotY[32];
					char tmpRotZ[32];

					sprintf(tmpRotX, "%f", rot.x);
					sprintf(tmpRotY, "%f", rot.y);
					sprintf(tmpRotZ, "%f", rot.z);

					SXLevelEditor::LightEditRotX->SetText(tmpRotX);
					SXLevelEditor::LightEditRotY->SetText(tmpRotY);
					SXLevelEditor::LightEditRotZ->SetText(tmpRotZ);
				}
			}
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		HWND handle_elem = (HWND)(lParam);
		if (SXLevelEditor::LightComboBoxTypeLight->GetHWND() == handle_elem)
		{
			int numsel = SXLevelEditor::LightComboBoxTypeLight->GetSel();
			if (numsel == 0)
			{
				LCActivateType(LIGHTS_TYPE_POINT, SXLevelEditor::HowActivateType != 3);

				if (SXLevelEditor::HowActivateType == 3)
					SXLevelEditor_LightButtonFinish_Click(hwnd, msg, wParam, lParam);
			}
			else
			{
				LCActivateType(LIGHTS_TYPE_DIRECTION, SXLevelEditor::HowActivateType != 3);

				if (SXLevelEditor::HowActivateType == 3)
					SXLevelEditor_LightButtonFinish_Click(hwnd, msg, wParam, lParam);
			}
		}
		else if (SXLevelEditor::LightComboBoxSourceMesh->GetHWND() == handle_elem)
		{
			DWORD numsel = SXLevelEditor::ListBoxList->GetSel();

			if (!(numsel >= 0 && numsel < SML_LigthsGetCount()))
				return 0;

			if (SML_LigthsExistsSource(SML_LigthsGetIDOfKey(numsel)))
			{
				SML_LigthsBindToGroupSource(SML_LigthsGetIDOfKey(numsel), SXLevelEditor::LightComboBoxSourceMesh->GetSel());
			}
		}

		else if (SXLevelEditor::LightComboBoxTypeLightShadow->GetHWND() == handle_elem)
		{
			DWORD numsel = SXLevelEditor::ListBoxList->GetSel();

			if (!(numsel >= 0 && numsel < SML_LigthsGetCount()))
				return 0;

			SML_LigthsSetTypeShadowed(SML_LigthsGetIDOfKey(numsel), SXLevelEditor::LightComboBoxTypeLightShadow->GetSel());
		}
	}
	return 0;
}


LRESULT SXLevelEditor_LightEditObject_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptext[256];
	tmptext[0] = 0;
	DWORD numsel = SXLevelEditor::ListBoxList->GetSel();
	if (SXLevelEditor::EditName->GetHWND() == hwnd)
	{
		SXLevelEditor::EditName->GetText(tmptext, 256);
		if (tmptext[0] != 0 && SXLevelEditor::HowActivateType == 3 && numsel != -1)
		{
			SML_LigthsSetName(SML_LigthsGetIDOfKey(numsel), tmptext);
			SXLevelEditor::ListBoxList->SetTextItem(numsel, tmptext);
		}
		else if (tmptext[0] != 0)
			MessageBox(0, "Необходимо ввести имя!", 0, 0);
	}
	/*else if (SXWinAddSetLight::EditVolume->GetHWND() == hwnd)
	{
		SXWinAddSetLight::EditVolume->GetText(tmptext, 256);
		if (!SXWinAddSetLight::IsAdd && numsel != -1)
		{
			SkyXEngine::Core::Data::Level::LightManager->Arr[numsel]->ChangeVolume(tmptext);
		}
	}
	else if (SXWinAddSetLight::EditSource->GetHWND() == hwnd)
	{
		SXWinAddSetLight::EditSource->GetText(tmptext, 256);
		if (!SXWinAddSetLight::IsAdd && numsel != -1)
		{
			SkyXEngine::Core::Data::Level::LightManager->Arr[numsel]->LoadSource(tmptext);
		}
	}
	else if (SXWinAddSetLight::EditVolumeLight->GetHWND() == hwnd)
	{
		SXWinAddSetLight::EditSource->GetText(tmptext, 256);
		if (!SXWinAddSetLight::IsAdd && numsel != -1)
		{
			SkyXEngine::Core::Data::Level::LightManager->Arr[numsel]->SetRays(tmptext);
		}
	}*/
	else if (SXLevelEditor::LightEditBias->GetHWND() == hwnd)
	{
		SXLevelEditor::LightEditBias->GetText(tmptext, 256);
		float bias = 0;
		sscanf(tmptext, "%f", &bias);
		if (SXLevelEditor::HowActivateType == 3 && numsel >= 0 && numsel < SML_LigthsGetCount())
		{
			SML_LigthsSetBias(SML_LigthsGetIDOfKey(numsel), bias);
		}
	}
	else if (SXLevelEditor::LightEditFarShadow->GetHWND() == hwnd)
	{
		SXLevelEditor::LightEditFarShadow->GetText(tmptext, 256);
		float dfe = 0;
		sscanf(tmptext, "%f", &dfe);
		if (SXLevelEditor::HowActivateType == 3 && numsel >= 0 && numsel < SML_LigthsGetCount())
		{
			SML_LigthsSetShadowLocalFar(SML_LigthsGetIDOfKey(numsel), dfe);
		}
	}
	else if (SXLevelEditor::LightEditAngle->GetHWND() == hwnd)
	{
		SXLevelEditor::LightEditAngle->GetText(tmptext, 256);
		float dfe = 0;
		sscanf(tmptext, "%f", &dfe);
		if (SXLevelEditor::HowActivateType == 3 && numsel >= 0 && numsel < SML_LigthsGetCount())
		{
			SML_LigthsSetAngle(SML_LigthsGetIDOfKey(numsel), dfe);
		}
	}
	else if (SXLevelEditor::LightEditTopRadius->GetHWND() == hwnd)
	{
		SXLevelEditor::LightEditTopRadius->GetText(tmptext, 256);
		float dfe = 0;
		sscanf(tmptext, "%f", &dfe);
		if (SXLevelEditor::HowActivateType == 3 && numsel >= 0 && numsel < SML_LigthsGetCount())
		{
			SML_LigthsSetTopRadius(SML_LigthsGetIDOfKey(numsel), dfe);
		}
	}
	else if (SXLevelEditor::LightEditPower->GetHWND() == hwnd)
	{
		SXLevelEditor::LightEditPower->GetText(tmptext, 256);
		float dfe = 0;
		sscanf(tmptext, "%f", &dfe);
		if (SXLevelEditor::HowActivateType == 3 && numsel >= 0 && numsel < SML_LigthsGetCount())
		{
			SML_LigthsSetPower(SML_LigthsGetIDOfKey(numsel), dfe);
		}
	}
	else if (SXLevelEditor::LightEditDist->GetHWND() == hwnd)
	{
		SXLevelEditor::LightEditDist->GetText(tmptext, 256);
		float dfe = 0;
		sscanf(tmptext, "%f", &dfe);
		if (SXLevelEditor::HowActivateType == 3 && numsel >= 0 && numsel < SML_LigthsGetCount())
		{
			SML_LigthsSetDist(SML_LigthsGetIDOfKey(numsel), dfe);
		}
		else
		{
			SXLevelEditor::LightEditDist->SetText(tmptext);
		}

	}

	return 0;
}

LRESULT SXLevelEditor_LightEditTransform_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DWORD numsel = SXLevelEditor::ListBoxList->GetSel();
	if (SXLevelEditor::HowActivateType == 3 && numsel >= 0 && numsel < SML_LigthsGetCount())
	{
		char tmpcoord[256];
		tmpcoord[0] = 0;
		bool ispos = false;
		int keycoord = 0;
		float3 float3coord;
		float getcoord = 0;

		if (SXLevelEditor::LightEditPosX->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditPosX->GetText(tmpcoord, 256);
			ispos = true;
			keycoord = 0;
		}
		else if (SXLevelEditor::LightEditPosY->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditPosY->GetText(tmpcoord, 256);
			ispos = true;
			keycoord = 1;
		}
		else if (SXLevelEditor::LightEditPosZ->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditPosZ->GetText(tmpcoord, 256);
			ispos = true;
			keycoord = 2;
		}

		else if (SXLevelEditor::LightEditRotX->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditRotX->GetText(tmpcoord, 256);
			ispos = false;
			keycoord = 0;
		}
		else if (SXLevelEditor::LightEditRotY->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditRotY->GetText(tmpcoord, 256);
			ispos = false;
			keycoord = 1;
		}
		else if (SXLevelEditor::LightEditRotZ->GetHWND() == hwnd)
		{
			SXLevelEditor::LightEditRotZ->GetText(tmpcoord, 256);
			ispos = false;
			keycoord = 2;
		}

		sscanf(tmpcoord, "%f", &getcoord);

		if (ispos)
		{
			SML_LigthsGetPos(SML_LigthsGetIDOfKey(numsel), &float3coord, false/*SXLevelEditor::LightCheckBoxPosLightOrMesh->GetCheck()*/);
			float3coord[keycoord] = getcoord;
			SML_LigthsSetPos(SML_LigthsGetIDOfKey(numsel), &float3coord/*, SXLevelEditor::LightCheckBoxPosLightOrMesh->GetCheck()*/);
		}
		else
		{
			float3 rotcoord;
			/*if (SkyXEngine::Core::Data::Settings::EditorsTransLightDirOrRot == 1)
			{*/
			SML_LigthsGetRot(SML_LigthsGetIDOfKey(numsel), &rotcoord, SXLevelEditor::LightCheckBoxRotLightOrMesh->GetCheck());
				rotcoord[keycoord] = getcoord;
				SML_LigthsSetRot(SML_LigthsGetIDOfKey(numsel), &rotcoord, SXLevelEditor::LightCheckBoxRotLightOrMesh->GetCheck());
			/*}
			else
			{
				SkyXEngine::Core::Data::Level::LightManager->Arr[numsel]->GetDirection(&rotcoord);
				rotcoord[keycoord] = getcoord;
				SkyXEngine::Core::Data::Level::LightManager->Arr[numsel]->SetDirection(&rotcoord);
			}*/
		}
	}

	return 0;
}


LRESULT SXLevelEditor_LightButtonFinish_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	long tmpid = -1;
	long tmpid2 = -1;
	if (SXLevelEditor::HowActivateType == 3)
		tmpid = SXLevelEditor::ListBoxList->GetSel();
	tmpid2 = tmpid;
	int typelight = SXLevelEditor::LightComboBoxTypeLight->GetSel();

	char tmpname[256];
	char tmppathsource[1024];
	SXLevelEditor::LightEditSourceMesh->GetText(tmppathsource, 1024);

	SXLevelEditor::EditName->GetText(tmpname, 256);

	if (!StrValidate(tmpname))
	{
		if (MessageBox(0, "Not input name of light, auto generate?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		{
			tmpname[0] = 0;
			sprintf(tmpname, "%s%s%s%d", (SXLevelEditor::LightComboBoxTypeLight->GetSel() == 1 ? "Directional" : "Point"), (SXLevelEditor::LightCheckBoxGlobal->GetCheck() ? " global" : ""), (SXLevelEditor::LightCheckBoxShadow->GetCheck() ? " shadowed light" : " non shadow light"), SXLevelEditor::LightComboBoxTypeLight->GetSel());
		}
		else
			return 0;
	}

	if (!SXLevelEditor::LightCheckBoxGlobal->GetCheck() && !StrValidate(tmppathsource))
	{
		if (MessageBox(0, "Not init source of light, continue?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) != IDYES)
			return 0;
	}

	char tmptextcolorr[32];
	char tmptextcolorg[32];
	char tmptextcolorb[32];
	char tmptextcolora[32];

	char tmptextposx[32];
	char tmptextposy[32];
	char tmptextposz[32];

	char tmptextrotx[32];
	char tmptextroty[32];
	char tmptextrotz[32];

	char tmptextpower[32];
	char tmptextdist[32];
	char tmptexttopradius[32];
	char tmptextangle[32];
	char tmpbias[32];
	char tmpdistforend[32];

	char tmprays[1024];
	tmprays[0] = 0;

	char tmppathvolume[1024];

	float3 tmpcolor;
	float3 tmppos;
	float3 tmprot;
	float tmpbiasnum = 0;
	float tmpdistforendnum = 0;

	float radius_height;
	float power;
	float top_radius;
	float angle;

	SXLevelEditor::LightEditFarShadow->GetText(tmpdistforend, 32);
	sscanf(tmpdistforend, "%f", &tmpdistforendnum);

	SXLevelEditor::LightEditBias->GetText(tmpbias, 32);
	sscanf(tmpbias, "%f", &tmpbiasnum);
	//SXWinAddSetLight::EditVolumeLight->GetText(tmprays, 1024);

	SXLevelEditor::LightEditColorR->GetText(tmptextcolorr, 32);
	SXLevelEditor::LightEditColorG->GetText(tmptextcolorg, 32);
	SXLevelEditor::LightEditColorB->GetText(tmptextcolorb, 32);
	//SXLevelEditor::LightEditColorA->GetText(tmptextcolora, 32);

	SXLevelEditor::LightEditPosX->GetText(tmptextposx, 32);
	SXLevelEditor::LightEditPosY->GetText(tmptextposy, 32);
	SXLevelEditor::LightEditPosZ->GetText(tmptextposz, 32);

	SXLevelEditor::LightEditRotX->GetText(tmptextrotx, 32);
	SXLevelEditor::LightEditRotY->GetText(tmptextroty, 32);
	SXLevelEditor::LightEditRotZ->GetText(tmptextrotz, 32);

	sscanf(tmptextcolorr, "%f", &tmpcolor.x);
	sscanf(tmptextcolorg, "%f", &tmpcolor.y);
	sscanf(tmptextcolorb, "%f", &tmpcolor.z);
	//sscanf(tmptextcolora, "%f", &tmpcolor.w);

	tmpcolor.x /= 255.f;
	tmpcolor.y /= 255.f;
	tmpcolor.z /= 255.f;
	//tmpcolor.w /= 255.f;

	sscanf(tmptextposx, "%f", &tmppos.x);
	sscanf(tmptextposy, "%f", &tmppos.y);
	sscanf(tmptextposz, "%f", &tmppos.z);

	sscanf(tmptextrotx, "%f", &tmprot.x);
	sscanf(tmptextroty, "%f", &tmprot.y);
	sscanf(tmptextrotz, "%f", &tmprot.z);

	if (typelight == 1 && (tmprot.x == 0.f && tmprot.y == 0.f && tmprot.z == 0.f))
	{
		MessageBox(0, "Direction is not init!!!", 0, MB_ICONWARNING | MB_TASKMODAL);

		if (tmpid != -1)
			SXLevelEditor::LightComboBoxTypeLight->SetSel(0);

		return 0;
	}

	bool is_shadow = SXLevelEditor::LightCheckBoxShadow->GetCheck();

	SXLevelEditor::LightEditPower->GetText(tmptextpower, 32);
	SXLevelEditor::LightEditDist->GetText(tmptextdist, 1024);

	SXLevelEditor::LightEditTopRadius->GetText(tmptexttopradius, 32);
	SXLevelEditor::LightEditAngle->GetText(tmptextangle, 32);

	sscanf(tmptexttopradius, "%f", &top_radius);
	sscanf(tmptextangle, "%f", &angle);
	sscanf(tmptextpower, "%f", &power);
	sscanf(tmptextdist, "%f", &radius_height);

	if (!SXLevelEditor::LightCheckBoxGlobal->GetCheck() && radius_height <= 0.f)
	{
		MessageBox(0, "Radius/height is not init!!!", 0, MB_ICONWARNING | MB_TASKMODAL);

		if (tmpid != -1)
			SXLevelEditor::LightComboBoxTypeLight->SetSel(0);

		return 0;
	}

	if (typelight == 1 && top_radius <= 0.f)
	{
		MessageBox(0, "Top radius is not init!!!", 0, MB_ICONWARNING | MB_TASKMODAL);

		if (tmpid != -1)
			SXLevelEditor::LightComboBoxTypeLight->SetSel(0);

		return 0;
	}

	if (typelight == 1 && angle <= 0.f)
	{
		MessageBox(0, "Angle is not init!!!", 0, MB_ICONWARNING | MB_TASKMODAL);

		if (tmpid != -1)
			SXLevelEditor::LightComboBoxTypeLight->SetSel(0);

		return 0;
	}
	

	if (tmpid >= 0)
	{
		if (SML_LigthsGetType(SML_LigthsGetIDOfKey(tmpid)) == LIGHTS_TYPE_GLOBAL)
		{
			for (int i = 0; i < 5; ++i)
			{
				SGeom_ModelsDelArrForCom(SML_LigthsGetIDArr(SML_LigthsGetIDOfKey(tmpid), 0, i));
			}
		}
		else if (SML_LigthsGetType(SML_LigthsGetIDOfKey(tmpid)) == LIGHTS_TYPE_POINT)
		{
			for (int i = 0; i < 6; ++i)
			{
				SGeom_ModelsDelArrForCom(SML_LigthsGetIDArr(SML_LigthsGetIDOfKey(tmpid), 0, i));
			}
		}
		else if (SML_LigthsGetType(SML_LigthsGetIDOfKey(tmpid)) == LIGHTS_TYPE_DIRECTION)
		{
			SGeom_ModelsDelArrForCom(SML_LigthsGetIDArr(SML_LigthsGetIDOfKey(tmpid), 0, 0));
		}
	}

	//tmppos.w = radius_height;
	if (typelight == 0)
	{
		if (!SXLevelEditor::LightCheckBoxGlobal->GetCheck())
		{
			tmpid = SML_LigthsReCreatePoint(SML_LigthsGetIDOfKey(tmpid),
				&tmppos,
				power,
				radius_height,
				&tmpcolor,
				false,
				is_shadow);

			SML_LigthsSetEnableCubeEdge(tmpid, 0, SXLevelEditor::LightCheckBoxCubeEdgeXPos->GetCheck());
			SML_LigthsSetEnableCubeEdge(tmpid, 1, SXLevelEditor::LightCheckBoxCubeEdgeXNeg->GetCheck());

			SML_LigthsSetEnableCubeEdge(tmpid, 2, SXLevelEditor::LightCheckBoxCubeEdgeYPos->GetCheck());
			SML_LigthsSetEnableCubeEdge(tmpid, 3, SXLevelEditor::LightCheckBoxCubeEdgeYNeg->GetCheck());

			SML_LigthsSetEnableCubeEdge(tmpid, 4, SXLevelEditor::LightCheckBoxCubeEdgeZPos->GetCheck());
			SML_LigthsSetEnableCubeEdge(tmpid, 5, SXLevelEditor::LightCheckBoxCubeEdgeZNeg->GetCheck());
		}
		else if (tmpid == -1)
		{
			//radius_height = LIGHTS_GLOBAL_STD_RADIUS;
			tmpid = SML_LigthsCreatePoint(
				&tmppos,
				LIGHTS_GLOBAL_MAX_POWER,
				LIGHTS_GLOBAL_STD_RADIUS,
				&tmpcolor,
				true,
				is_shadow);

			/*SkyXEngine::Core::Data::EngineID::Light_Global = SkyXEngine::Core::Data::Level::LightManager->Arr.size() - 1;
			SXWinAddSetLight::CheckBoxGlobal->SetCheck(false);
			SXWinAddSetLight::ComboBoxType->Enable(true);
			SXWinAddSetLight::CheckBoxGlobal->Enable(false);*/

			/*if (SkyXEngine::Core::Data::Settings::EditorsTransLightDirOrRot == 1)
				SkyXEngine::Core::Data::Level::LightManager->Arr[SkyXEngine::Core::Data::Level::LightManager->Arr.size() - 1]->SetRotation(&tmprot);
			else
				SkyXEngine::Core::Data::Level::LightManager->Arr[SkyXEngine::Core::Data::Level::LightManager->Arr.size() - 1]->SetDirection(&tmprot);*/
		}
	}
	else
	{
		tmpid = SML_LigthsReCreateDirection(SML_LigthsGetIDOfKey(tmpid),
			&tmppos,
			power,
			radius_height,
			&tmpcolor,
			&tmprot,
			top_radius,
			angle,
			is_shadow);

		if (SXLevelEditor::LightCheckBoxDirOrRot->GetCheck())
			SML_LigthsSetRot(tmpid, &tmprot, false);
		else
			SML_LigthsSetDir(tmpid, &tmprot, false);
	}

	if (!SXLevelEditor::LightCheckBoxGlobal->GetCheck())
	{
		SML_LigthsSetTypeShadowed(tmpid, SXLevelEditor::LightComboBoxTypeLightShadow->GetSel());
		SML_LigthsLoadSource(tmpid, tmppathsource);
	}

	/*if (StrValidate(tmppathsource))
	{
		SkyXEngine::Core::Data::Level::LightManager->Arr[SkyXEngine::Core::Data::Level::LightManager->GetCount() - 1]->LoadSource(tmppathsource);
	}

	if (StrValidate(tmprays))
	{
		SkyXEngine::Core::Data::Level::LightManager->Arr[SkyXEngine::Core::Data::Level::LightManager->GetCount() - 1]->SetRays(tmprays);
	}*/

	/*if (tmpbiasnum > 0.0f)
	{
		SkyXEngine::Core::Data::Level::LightManager->Arr[SkyXEngine::Core::Data::Level::LightManager->GetCount() - 1]->SetBias(tmpbiasnum);
	}

	if (tmpdistforendnum > 0.0f)
	{
		SkyXEngine::Core::Data::Level::LightManager->Arr[SkyXEngine::Core::Data::Level::LightManager->GetCount() - 1]->SetShadowLocalFar(tmpdistforendnum);
	}*/

	if (tmpid >= 0)
	{
		if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_GLOBAL)
		{
			for (int i = 0; i < 5; ++i)
			{
				SML_LigthsSetIDArr(tmpid, 0, i, SGeom_ModelsAddArrForCom());
			}
		}
		else if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_POINT)
		{
			for (int i = 0; i < 6; ++i)
			{
				SML_LigthsSetIDArr(tmpid, 0, i, SGeom_ModelsAddArrForCom());
			}
		}
		else if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_DIRECTION)
		{
			SML_LigthsSetIDArr(tmpid, 0, 0, SGeom_ModelsAddArrForCom());
		}
	}

	if (tmpid2 == -1)
		SXLevelEditor::ListBoxList->AddItem(tmpname);

	sprintf(SML_LigthsGetName(tmpid), "%s", tmpname);
	
	SML_LigthsSetEnable(tmpid, SXLevelEditor::LightCheckBoxEnable->GetCheck());

	char tmpcountlight[32];
	sprintf(tmpcountlight, "%d", tmpid);
	SXLevelEditor::StaticListValCount->SetText(tmpcountlight);

	SXLevelEditor::HowActivateType = 3;
	SXLevelEditor::ListBoxList->SetSel(tmpid);
	LCInitElemsSelModel(SXLevelEditor::ListBoxList->GetSel());

	return 1;
}