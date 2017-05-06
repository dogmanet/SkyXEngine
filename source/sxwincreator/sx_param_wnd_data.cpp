
extern LRESULT CALLBACK WndProcChildJob(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern void InLog(const char* format, ...);

namespace SXUpdateParam
{
	//обноелние информации активного элемента
	void PosSize();	// позиция и габариты (x,y width,height)
	void Caption();
	void VarName();
	void Enable();
	void Visible();
	void VisibleHint();
	void HintText();
	void ColorText();
	void ColorTextBK();
	void ColorBK();
	void TransparentTextBk();
	void ParentFont();
	void Font();
	void MenuWindow();

	void UpdateAll();

	void SetPosX();
	void SetPosY();
	void SetWidth();
	void SetHeight();
	void SetVisible();
	void SetEnable();
	void SetCaption();
	void SetVarName();
	void SetVisibleHint();
	void SetHintText();
	void SetColorText();
	void SetColorTextBK();
	void SetColorBK();
	void SetTransparentTextBk();
	void SetParentFont();
	void SetFont();
	void SetImgButton();
	void SetMenuWindow();

	void LockPosSizeBlock();
	void LockFontBlock();
	void LockColorBlock();
	void LockColorTextBlock();
	void LockImgButton();

	void UnLockPosSizeBlock();
	void UnLockFontBlock();
	void UnLockColorBlock();
	void UnLockColorTextBlock();
	void UnlockAll();
	void UnLockImgButton();

	void SetDefaultValue();
}

void SXUpdateParam::SetDefaultValue()
{

}

void SXUpdateParam::LockColorBlock()
{
	SXMainWndElem::StaticParamColorText->Enable(false);
	SXMainWndElem::StaticParamColorBK->Enable(false);
	SXMainWndElem::StaticParamColorBKText->Enable(false);
	SXMainWndElem::StaticParamTransparentText->Enable(false);

	SXMainWndElem::EditParamColorTextR->Enable(false);
	SXMainWndElem::EditParamColorTextG->Enable(false);
	SXMainWndElem::EditParamColorTextB->Enable(false);

	SXMainWndElem::EditParamColorTextBKR->Enable(false);
	SXMainWndElem::EditParamColorTextBKG->Enable(false);
	SXMainWndElem::EditParamColorTextBKB->Enable(false);

	SXMainWndElem::EditParamColorBKR->Enable(false);
	SXMainWndElem::EditParamColorBKG->Enable(false);
	SXMainWndElem::EditParamColorBKB->Enable(false);

	SXMainWndElem::ComboBoxParamTransparentText->Enable(false);
}

void SXUpdateParam::UnLockColorBlock()
{
	SXMainWndElem::StaticParamColorText->Enable(true);
	SXMainWndElem::StaticParamColorBK->Enable(true);
	SXMainWndElem::StaticParamColorBKText->Enable(true);
	SXMainWndElem::StaticParamTransparentText->Enable(true);

	SXMainWndElem::EditParamColorTextR->Enable(true);
	SXMainWndElem::EditParamColorTextG->Enable(true);
	SXMainWndElem::EditParamColorTextB->Enable(true);

	SXMainWndElem::EditParamColorTextBKR->Enable(true);
	SXMainWndElem::EditParamColorTextBKG->Enable(true);
	SXMainWndElem::EditParamColorTextBKB->Enable(true);

	SXMainWndElem::EditParamColorBKR->Enable(true);
	SXMainWndElem::EditParamColorBKG->Enable(true);
	SXMainWndElem::EditParamColorBKB->Enable(true);

	SXMainWndElem::ComboBoxParamTransparentText->Enable(true);
}

void SXUpdateParam::LockColorTextBlock()
{
	SXMainWndElem::StaticParamColorText->Enable(false);
	SXMainWndElem::StaticParamColorBKText->Enable(false);
	SXMainWndElem::StaticParamTransparentText->Enable(false);

	SXMainWndElem::EditParamColorTextR->Enable(false);
	SXMainWndElem::EditParamColorTextG->Enable(false);
	SXMainWndElem::EditParamColorTextB->Enable(false);

	SXMainWndElem::EditParamColorTextBKR->Enable(false);
	SXMainWndElem::EditParamColorTextBKG->Enable(false);
	SXMainWndElem::EditParamColorTextBKB->Enable(false);

	SXMainWndElem::ComboBoxParamTransparentText->Enable(false);
}

void SXUpdateParam::UnLockColorTextBlock()
{
	SXMainWndElem::StaticParamColorText->Enable(true);
	SXMainWndElem::StaticParamColorBKText->Enable(true);
	SXMainWndElem::StaticParamTransparentText->Enable(true);

	SXMainWndElem::EditParamColorTextR->Enable(true);
	SXMainWndElem::EditParamColorTextG->Enable(true);
	SXMainWndElem::EditParamColorTextB->Enable(true);

	SXMainWndElem::EditParamColorTextBKR->Enable(true);
	SXMainWndElem::EditParamColorTextBKG->Enable(true);
	SXMainWndElem::EditParamColorTextBKB->Enable(true);

	SXMainWndElem::ComboBoxParamTransparentText->Enable(true);
}

void SXUpdateParam::LockFontBlock()
{
	SXMainWndElem::StaticParamFont->Enable(false);
	SXMainWndElem::StaticParamFontParent->Enable(false);
	SXMainWndElem::ComboBoxParamParentFont->Enable(false);
	SXMainWndElem::ButtonParamSelectFont->Enable(false);
}

void SXUpdateParam::UnLockFontBlock()
{
	SXMainWndElem::StaticParamFont->Enable(true);
	SXMainWndElem::StaticParamFontParent->Enable(true);
	SXMainWndElem::ComboBoxParamParentFont->Enable(true);
	SXMainWndElem::ButtonParamSelectFont->Enable(true);
}

void SXUpdateParam::LockPosSizeBlock()
{
	SXMainWndElem::StaticParamPosX->Enable(false);
	SXMainWndElem::StaticParamPosY->Enable(false);
	SXMainWndElem::StaticParamWidth->Enable(false);
	SXMainWndElem::StaticParamHeight->Enable(false);

	SXMainWndElem::EditParamPosX->Enable(false);
	SXMainWndElem::EditParamPosY->Enable(false);
	SXMainWndElem::EditParamWidth->Enable(false);
	SXMainWndElem::EditParamHeight->Enable(false);
}

void SXUpdateParam::UnLockPosSizeBlock()
{
	SXMainWndElem::StaticParamPosX->Enable(true);
	SXMainWndElem::StaticParamPosY->Enable(true);
	SXMainWndElem::StaticParamWidth->Enable(true);
	SXMainWndElem::StaticParamHeight->Enable(true);

	SXMainWndElem::EditParamPosX->Enable(true);
	SXMainWndElem::EditParamPosY->Enable(true);
	SXMainWndElem::EditParamWidth->Enable(true);
	SXMainWndElem::EditParamHeight->Enable(true);
}

void SXUpdateParam::LockImgButton()
{
	SXMainWndElem::StaticParamImgBLoadImg->Enable(false);
	SXMainWndElem::ButtomParamImgBLoadImg->Enable(false);
}

void SXUpdateParam::UnLockImgButton()
{
	SXMainWndElem::StaticParamImgBLoadImg->Enable(true);
	SXMainWndElem::ButtomParamImgBLoadImg->Enable(true);
}

void SXUpdateParam::UnlockAll()
{
	SXUpdateParam::UnLockPosSizeBlock();
	SXUpdateParam::UnLockFontBlock();
	SXUpdateParam::UnLockColorBlock();
	SXUpdateParam::UnLockColorTextBlock();
}

void SXUpdateParam::UpdateAll()
{
	//
		if(SXMainWndElem::NumActiveElement != -1)
		{
			char ClassName[256];
			int error = GetClassName(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHWND(),ClassName,256);
				//обычная кнопка
				if (strcmp(ClassName, "SXGUIGROUPBOX") == 0) //strcmp(ClassName, "Button") == 0 && !(GetWindowLong(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHWND(), GWL_STYLE) & BS_GROUPBOX))
				{
					SXUpdateParam::UnlockAll();
					//SXUpdateParam::LockColorBlock();
					SXUpdateParam::LockImgButton();
				}
				else if(strcmp(ClassName,TOOLBARCLASSNAME) == 0 || strcmp(ClassName,UPDOWN_CLASS) == 0 || strcmp(ClassName,TRACKBAR_CLASS) == 0 || strcmp(ClassName,PROGRESS_CLASS) == 0)
				{
					SXUpdateParam::UnlockAll();
					SXUpdateParam::LockColorBlock();
					SXUpdateParam::LockFontBlock();
					SXUpdateParam::LockImgButton();
				}
				else if(strcmp(ClassName,STATUSCLASSNAME) == 0)
				{
					SXUpdateParam::UnlockAll();
					SXUpdateParam::LockColorBlock();
					SXUpdateParam::LockImgButton();
				}
				else if(
						strcmp(ClassName,"Static") == 0 || 
						(strcmp(ClassName,"Button") == 0 && GetWindowLong(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHWND(),GWL_STYLE) & BS_GROUPBOX) ||
						strcmp(ClassName,"ListBox") == 0 || 
						strcmp(ClassName,"Edit") == 0 ||
						strcmp(ClassName,"ComboBox") == 0
						)
				{
					SXUpdateParam::UnlockAll();
					SXUpdateParam::LockImgButton();
				}
				else if(strcmp(ClassName,"SXGUIBUTTONIMG") == 0)
				{
					SXUpdateParam::UnlockAll();
					SXUpdateParam::UnLockImgButton();
					SXUpdateParam::LockFontBlock();
					SXUpdateParam::LockColorBlock();
				}
				//иначе это окно
				else
				{
					SXUpdateParam::UnlockAll();
					SXUpdateParam::LockFontBlock();
					SXUpdateParam::LockColorTextBlock();
					SXUpdateParam::LockImgButton();
				}
			SXUpdateParam::PosSize();
			
			SXUpdateParam::VisibleHint();
			SXUpdateParam::HintText();
			SXUpdateParam::ColorText();
			SXUpdateParam::ColorTextBK();
			SXUpdateParam::ColorBK();
			SXUpdateParam::TransparentTextBk();
			SXUpdateParam::ParentFont();
			SXUpdateParam::Font();
		}
		else
		{
			
			SXUpdateParam::LockColorTextBlock();
			SXUpdateParam::PosSize();
			SXUpdateParam::VisibleHint();
			SXUpdateParam::HintText();
			SXUpdateParam::ColorBK();
		}
	SXUpdateParam::Caption();
	SXUpdateParam::VarName();
	SXUpdateParam::Enable();
	SXUpdateParam::Visible();
}

void SXUpdateParam::PosSize()
{
	RECT cr;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->GetClientRect(&cr);
	//GetWindowRect(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement].Object->GetHWND(),&gr);
	char tmp[32];
	sprintf(tmp,"%d",cr.right - cr.left);
	SXMainWndElem::EditParamWidth->SetText(tmp);
	sprintf(tmp,"%d",cr.bottom - cr.top);
	SXMainWndElem::EditParamHeight->SetText(tmp);

	sprintf(tmp,"%d",cr.left);
	SXMainWndElem::EditParamPosX->SetText(tmp);
	sprintf(tmp,"%d",cr.top);
	SXMainWndElem::EditParamPosY->SetText(tmp);
}

void SXUpdateParam::Caption()
{
	char str[256];
	str[0] = 0;
		if(SXMainWndElem::NumActiveElement != -1)
		{
			SXGUIFuctinon::GetText(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object,str,256);
				if(str[0] == 0)
				{
					sprintf(str,"%s",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name);
				}
		}
		else
			SXMainWndElem::JobMainWnd->GetText(str,256);
	SXMainWndElem::EditParamCaption->SetText(str);
}

void SXUpdateParam::VarName()
{
	char str[256];
	str[0] = 0;
		if(SXMainWndElem::NumActiveElement != -1)
		{
				if(str[0] == 0)
				{
					sprintf(str,"%s",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name);
				}
		}
		else
			sprintf(str,"%s",SXMainWndElem::NameJobWnd);
	SXMainWndElem::EditParamVarName->SetText(str);
}

void SXUpdateParam::Enable()
{
	bool enable;
		if(SXMainWndElem::NumActiveElement != -1)
			enable = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->Enable();
		else
			enable = SXMainWndElem::JobMainWnd->Enable();
	
	SXMainWndElem::ComboBoxParamEnabled->SetSel((enable ? 0 : 1));
}

void SXUpdateParam::Visible()
{
	bool visible;
		if(SXMainWndElem::NumActiveElement != -1)
			visible = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->Visible();
		else
			visible = SXMainWndElem::JobMainWnd->Visible();
	
	SXMainWndElem::ComboBoxParamVisible->SetSel((visible ? 0 : 1));
}

void SXUpdateParam::VisibleHint()
{
	bool visible;
		if(SXMainWndElem::NumActiveElement != -1)
			visible = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->ShowHint();
		else
			visible = SXMainWndElem::JobMainWnd->ShowHint();
	
	SXMainWndElem::ComboBoxParamHintVisible->SetSel((visible ? 0 : 1));
}

void SXUpdateParam::HintText()
{
	char str[1024];
	str[0] = 0;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHintText(str);
		else
			SXMainWndElem::JobMainWnd->GetHintText(str);
	SXMainWndElem::EditParamHintText->SetText(str);
}

void SXUpdateParam::ColorText()
{
		if(SXMainWndElem::NumActiveElement != -1)
		{
			char bufr[16],bufg[16],bufb[16];
			DWORD color = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetColorText();
			sprintf(bufr, "%d", GetRValue(color));
			sprintf(bufg, "%d", GetGValue(color));
			sprintf(bufb, "%d", GetBValue(color));
			SXMainWndElem::EditParamColorTextR->SetText(bufr);
			SXMainWndElem::EditParamColorTextG->SetText(bufg);
			SXMainWndElem::EditParamColorTextB->SetText(bufb);
		}
}

void SXUpdateParam::ColorTextBK()
{
		if(SXMainWndElem::NumActiveElement != -1)
		{
			char bufr[16],bufg[16],bufb[16];
			DWORD color = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetColorTextBk();
			sprintf(bufr, "%d", GetRValue(color));
			sprintf(bufg, "%d", GetGValue(color));
			sprintf(bufb, "%d", GetBValue(color));
			SXMainWndElem::EditParamColorTextBKR->SetText(bufr);
			SXMainWndElem::EditParamColorTextBKG->SetText(bufg);
			SXMainWndElem::EditParamColorTextBKB->SetText(bufb);
		}
}

void SXUpdateParam::ColorBK()
{
	char bufr[16],bufg[16],bufb[16];
	DWORD color;
		if(SXMainWndElem::NumActiveElement != -1)
		{
			color = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetColorBrush();
		}
		else
		{
			color = SXMainWndElem::JobMainWnd->GetColorBrush();
		}
		sprintf(bufr, "%d", GetRValue(color));
		sprintf(bufg, "%d", GetGValue(color));
		sprintf(bufb, "%d", GetBValue(color));
	SXMainWndElem::EditParamColorBKR->SetText(bufr);
	SXMainWndElem::EditParamColorBKG->SetText(bufg);
	SXMainWndElem::EditParamColorBKB->SetText(bufb);
}

void SXUpdateParam::TransparentTextBk()
{
	bool ttbk = false;
		if(SXMainWndElem::NumActiveElement != -1)
			ttbk = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetTransparentTextBk();
	
	SXMainWndElem::ComboBoxParamTransparentText->SetSel((ttbk ? 0 : 1));
}

/*void SXUpdateParam::ParentFont()
{
	bool pf = false;
		if(SXMainWndElem::NumActiveElement != -1)
			pf = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->IsParentFont();
	
	SXMainWndElem::ComboBoxParamParentFont->SetSel((pf ? 0 : 1));
}*/

void SXUpdateParam::Font()
{
	
		if(SXMainWndElem::NumActiveElement != -1)
		{
			HFONT hfont = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetFont();
				if(hfont == 0)
				{
					SXMainWndElem::ButtonParamSelectFont->SetText("System font ...");
				}
				else
				{
					LOGFONT lf;
					ZeroMemory(&lf,sizeof(&lf));
					GetObject(hfont, sizeof(LOGFONT), &lf);
					char buf[256];
					HDC hDC = ::GetDC(NULL);
					int font_size = -MulDiv(lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
					font_size = (font_size < 0) ? - font_size : font_size;
					ReleaseDC(NULL, hDC);
					sprintf(buf,"%s%s%d",lf.lfFaceName,", ",font_size);
					SXMainWndElem::ButtonParamSelectFont->SetText(buf);
				}
		}
}

void SXUpdateParam::ParentFont()
{
		if(SXMainWndElem::NumActiveElement != -1)
		{
			HFONT defgui = (HFONT)GetStockObject(/*ANSI_VAR_FONT*/ANSI_VAR_FONT);
			HFONT hfont = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetFont();
			
			/*LOGFONT LogFontDefGUI,LogFontCurrent;
			ZeroMemory(&LogFontDefGUI,sizeof(&LogFontDefGUI));
			GetObject(defgui, sizeof(LOGFONT), &LogFontDefGUI);
			ZeroMemory(&LogFontCurrent,sizeof(&LogFontCurrent));
			GetObject(hfont, sizeof(LOGFONT), &LogFontCurrent);*/
				if(hfont == 0)
				{
					SXMainWndElem::ComboBoxParamParentFont->SetSel(0);
				}
				else if(defgui == hfont
							/*strcmp(LogFontDefGUI.lfFaceName,LogFontCurrent.lfFaceName) == 0 &&
							LogFontDefGUI.lfHeight == LogFontCurrent.lfHeight &&
							LogFontDefGUI.lfWidth == LogFontCurrent.lfWidth &&
							LogFontDefGUI.lfWeight == LogFontCurrent.lfWeight &&
							LogFontDefGUI.lfItalic == LogFontCurrent.lfItalic && 
							LogFontDefGUI.lfUnderline == LogFontCurrent.lfUnderline &&
							LogFontDefGUI.lfStrikeOut == LogFontCurrent.lfStrikeOut*/
							)
				{
					SXMainWndElem::ComboBoxParamParentFont->SetSel(1);
				}
				else
				{
					SXMainWndElem::ComboBoxParamParentFont->SetSel(2);
				}
			//DeleteObject(hfont);
			DeleteObject(defgui);
		}
}

void SXUpdateParam::MenuWindow()
{

}

///////////////////////////////

void SXUpdateParam::SetPosX()
{
	RECT cr;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->GetClientRect(&cr);
	RECT dtrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &dtrect, 0);
	char posxtext[16];
	SXMainWndElem::EditParamPosX->GetText(posxtext,16);
	int posxnum = 0;
	sscanf(posxtext,"%d",&posxnum);
	int tmpright = (cr.right - cr.left) + posxnum;
	int tmpleft = posxnum;
		if(tmpright > dtrect.right)
		{
			tmpright = dtrect.right;
			tmpleft = tmpright - (cr.right - cr.left);
		}
	cr.right = tmpright;
	cr.left = tmpleft;

		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetClientRect(&cr,true);
		else
			SXMainWndElem::JobMainWnd->SetClientRect(&cr,true);
}

void SXUpdateParam::SetPosY()
{
	RECT cr;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->GetClientRect(&cr);
	RECT dtrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &dtrect, 0);
	char posytext[16];
	SXMainWndElem::EditParamPosY->GetText(posytext,16);
	int posynum = 0;
	sscanf(posytext,"%d",&posynum);
	int tmpbottom = (cr.bottom - cr.top) + posynum;
	int tmptop = posynum;
		if(tmpbottom > dtrect.bottom)
		{
			tmpbottom = dtrect.bottom;
			tmptop = tmpbottom - (cr.bottom - cr.top);
		}
	cr.bottom = tmpbottom;
	cr.top = tmptop;

		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetClientRect(&cr,true);
		else
			SXMainWndElem::JobMainWnd->SetClientRect(&cr,true);
}

void SXUpdateParam::SetWidth()
{
	RECT cr;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->GetClientRect(&cr);
	RECT dtrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &dtrect, 0);
	char widthtext[16];
	SXMainWndElem::EditParamWidth->GetText(widthtext,16);
	int widthnum = 0;
	sscanf(widthtext,"%d",&widthnum);
		if(widthnum + cr.left > dtrect.right)
			widthnum = dtrect.right - cr.left;
	cr.right = widthnum + cr.left;

		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetClientRect(&cr,true);
		else
			SXMainWndElem::JobMainWnd->SetClientRect(&cr,true);
}

void SXUpdateParam::SetHeight()
{
	RECT cr;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->GetClientRect(&cr);
	RECT dtrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &dtrect, 0);
	char heighttext[16];
	SXMainWndElem::EditParamHeight->GetText(heighttext,16);
	int heightnum = 0;
	sscanf(heighttext,"%d",&heightnum);
		if(heightnum + cr.top > dtrect.bottom)
			heightnum = dtrect.bottom - cr.top;
	cr.bottom = heightnum + cr.top;

		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetClientRect(&cr,true);
		else
			SXMainWndElem::JobMainWnd->SetClientRect(&cr,true);
}

void SXUpdateParam::SetVisible()
{
	int sel = SXMainWndElem::ComboBoxParamVisible->GetSel();
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->Visible(sel==0 ? true : false);
		else
			SXMainWndElem::JobMainWnd->Visible(sel==0 ? true : false);
}

void SXUpdateParam::SetEnable()
{
	int sel = SXMainWndElem::ComboBoxParamEnabled->GetSel();
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->Enable(sel==0 ? true : false);
		else
			SXMainWndElem::JobMainWnd->Enable(sel==0 ? true : false);
}

void SXUpdateParam::SetCaption()
{
	char str[256];
	str[0] = 0;
	SXMainWndElem::EditParamCaption->GetText(str,256);
		//если юзер удалил все символы из названия
		//он творит однозначную херню
		//то присваиваем старое имя
		if(str[0] == 0)
			SXUpdateParam::Caption();
		else
		{
				if(SXMainWndElem::NumActiveElement != -1)
				{
					SXGUIFuctinon::SetText(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object,str);
					/*sprintf(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name,"%s",str);
					char tmpAddItem[256];
					sprintf(tmpAddItem,"%s%s%s%s%s",str, " - ",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName," : ",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SysClassName);
					SXMainWndElem::ListBoxAllElements->SetTextItem(SXMainWndElem::NumActiveElement,tmpAddItem);
					SXMainWndElem::ListBoxAllElements->SetSel(SXMainWndElem::NumActiveElement);*/
				}
				else
					SXMainWndElem::JobMainWnd->SetText(str);
		}
}

void SXUpdateParam::SetVarName()
{
	char str[256];
	str[0] = 0;
	SXMainWndElem::EditParamVarName->GetText(str,256);
		//если юзер удалил все символы из названия
		//он творит однозначную херню
		//то присваиваем старое имя
		if(str[0] == 0)
			SXUpdateParam::VarName();
		else
		{
				if(SXMainWndElem::NumActiveElement != -1)
				{
					//SXGUIFuctinon::SetText(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object,str);
					sprintf(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name,"%s",str);
					char tmpAddItem[256];
					sprintf(tmpAddItem,"%s%s%s%s%s",str, " - ",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName," : ",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SysClassName);
					SXMainWndElem::ListBoxAllElements->SetTextItem(SXMainWndElem::NumActiveElement,tmpAddItem);
					SXMainWndElem::ListBoxAllElements->SetSel(SXMainWndElem::NumActiveElement);
				}
				else
					sprintf(SXMainWndElem::NameJobWnd,"%s",str);//SXMainWndElem::JobMainWnd->SetText(str);
		}
}

void SXUpdateParam::SetVisibleHint()
{
	int sel = SXMainWndElem::ComboBoxParamHintVisible->GetSel();
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->ShowHint(sel==0 ? true : false);
		else
			SXMainWndElem::JobMainWnd->ShowHint(sel==0 ? true : false);
}

void SXUpdateParam::SetHintText()
{
	char str[1024];
	SXMainWndElem::EditParamHintText->GetText(str,1024);
	int sel = SXMainWndElem::ComboBoxParamHintVisible->GetSel();
		if(SXMainWndElem::NumActiveElement != -1)
		{
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetHintText(str);
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->ShowHint(sel==0 ? true : false);
		}
		else
		{
			SXMainWndElem::JobMainWnd->SetHintText(str);
			SXMainWndElem::JobMainWnd->ShowHint(sel==0 ? true : false);
		}
}

void SXUpdateParam::SetColorText()
{
	char bufr[16],bufg[16],bufb[16];
	int numr,numg,numb;
	SXMainWndElem::EditParamColorTextR->GetText(bufr,4);
	SXMainWndElem::EditParamColorTextG->GetText(bufg,4);
	SXMainWndElem::EditParamColorTextB->GetText(bufb,4);
	sscanf(bufr,"%d",&numr);
	sscanf(bufg,"%d",&numg);
	sscanf(bufb,"%d",&numb);
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetColorText(numr,numg,numb);
}

void SXUpdateParam::SetColorTextBK()
{
	char bufr[16],bufg[16],bufb[16];
	int numr,numg,numb;
	SXMainWndElem::EditParamColorTextBKR->GetText(bufr,4);
	SXMainWndElem::EditParamColorTextBKG->GetText(bufg,4);
	SXMainWndElem::EditParamColorTextBKB->GetText(bufb,4);
	sscanf(bufr,"%d",&numr);
	sscanf(bufg,"%d",&numg);
	sscanf(bufb,"%d",&numb);
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetColorTextBk(numr,numg,numb);
}

void SXUpdateParam::SetColorBK()
{
	char bufr[16],bufg[16],bufb[16];
	int numr,numg,numb;
	SXMainWndElem::EditParamColorBKR->GetText(bufr,4);
	SXMainWndElem::EditParamColorBKG->GetText(bufg,4);
	SXMainWndElem::EditParamColorBKB->GetText(bufb,4);
	sscanf(bufr,"%d",&numr);
	sscanf(bufg,"%d",&numg);
	sscanf(bufb,"%d",&numb);

		if(SXMainWndElem::NumActiveElement != -1)
		{
				if(strcmp(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName,"SXGUIBaseWnd") == 0)
					dynamic_cast<ISXGUIBaseWnd*>(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object)->SetColorBrush(numr, numg, numb);
				else
					SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetColorBrush(numr,numg,numb);
		}
		else
			SXMainWndElem::JobMainWnd->SetColorBrush(numr,numg,numb);
}

void SXUpdateParam::SetTransparentTextBk()
{
	int sel = SXMainWndElem::ComboBoxParamTransparentText->GetSel();
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetTransparentTextBk(sel==0 ? true : false);
}

void SXUpdateParam::SetParentFont()
{
	int sel = SXMainWndElem::ComboBoxParamParentFont->GetSel();
		if(SXMainWndElem::NumActiveElement != -1 && sel == 0)
		{	
			HFONT ParentFont = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->ParentFont();

			SendMessage(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHWND(),WM_SETFONT,WPARAM(ParentFont),1);
			DeleteObject(ParentFont);

			SXUpdateParam::Font();
		}
		else if(SXMainWndElem::NumActiveElement != -1 && sel == 1)
		{
			HFONT DefaultFont = (HFONT) GetStockObject(ANSI_VAR_FONT);

			SendMessage(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetHWND(),WM_SETFONT,WPARAM(DefaultFont),1);
			DeleteObject(DefaultFont);

			SXUpdateParam::Font();
		}
		else if(SXMainWndElem::NumActiveElement != -1 && sel == 2)
		{
			SXUpdateParam::Font();
		}
}

void SXUpdateParam::SetFont()
{
	HFONT hfont;
		if(SXMainWndElem::NumActiveElement != -1)
			hfont = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->GetFont();
	
	CHOOSEFONT cf;

	LOGFONT lf;
	ZeroMemory(&lf,sizeof(&lf));
	GetObject(hfont, sizeof(LOGFONT), &lf);

	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof (CHOOSEFONT);
	cf.hwndOwner = 0;
	cf.lpLogFont = &lf;
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;

	SXMainWndElem::MainWnd->Enable(false);
	SXMainWndElem::JobMainWnd->Enable(false);
	SXMainWndElem::ParamWnd->Enable(false);
	SXMainWndElem::WndLog->Enable(false);
		if(ChooseFont(&cf))
		{
				if(SXMainWndElem::NumActiveElement != -1)
					SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->SetFont(lf.lfFaceName,lf.lfHeight,lf.lfWidth,lf.lfWeight,lf.lfItalic,lf.lfUnderline,lf.lfStrikeOut);
			char buf[256];
			HDC hDC = ::GetDC(NULL);
			int font_size = -MulDiv(lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
			font_size = (font_size < 0) ? - font_size : font_size;
			ReleaseDC(NULL, hDC);
			sprintf(buf,"%s%s%d",lf.lfFaceName,", ",font_size);
			SXMainWndElem::ButtonParamSelectFont->SetText(buf);
		}
	SXMainWndElem::MainWnd->Enable(true);
	SXMainWndElem::JobMainWnd->Enable(true);
	SXMainWndElem::ParamWnd->Enable(true);
	SXMainWndElem::WndLog->Enable(true);
}

void SXUpdateParam::SetImgButton()
{
	OPENFILENAME ofn;
	char szFileName[256];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "BMP image(.bmp)\0*.bmp\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	SXMainWndElem::MainWnd->Enable(false);
	SXMainWndElem::JobMainWnd->Enable(false);
	SXMainWndElem::ParamWnd->Enable(false);
	SXMainWndElem::WndLog->Enable(false);
		if(GetOpenFileName(&ofn) == TRUE)
		{
				if(SXMainWndElem::NumActiveElement != -1 && strcmp(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName,"SXGUIButtonImg") == 0)
				{
					ISXGUIButtonImg* bimg = dynamic_cast<ISXGUIButtonImg*>(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object);
					bimg->ReInitImage(szFileName);
					InvalidateRect(bimg->GetHWND(),0,1);
					SXMainWndElem::ButtomParamImgBLoadImg->SetText(szFileTitle);
				}
		}
	SXMainWndElem::MainWnd->Enable(true);
	SXMainWndElem::JobMainWnd->Enable(true);
	SXMainWndElem::ParamWnd->Enable(true);
	SXMainWndElem::WndLog->Enable(true);
}

void SXUpdateParam::SetMenuWindow()
{
	int check = SXMainWndElem::CheckBoxParamWinMenu->GetCheck();
		if(check == 1)
		{
			SXMainWndElem::MenuWindow->SetToWindow(SXMainWndElem::JobMainWnd->GetHWND());
		}
		else
			SetMenu(SXMainWndElem::JobMainWnd->GetHWND(),0);
}

void SaveFile(const char* path)
{
	FILE* file = 0;
	file = fopen(path,"w");

	WORD CountSave = SXMainWndElem::CreateElements.size();

	//записываем sys class, sx class, caption
	//{
	fprintf(file, "[JobWindow]\n");
	char classname[32];
	GetClassName(SXMainWndElem::JobMainWnd->GetHWND(),classname,31);
	fprintf(file, "SysClassName = %s\n", classname);
	fprintf(file, "Name = %s\n", SXMainWndElem::NameJobWnd);
	char textwnd[256];
	SXMainWndElem::JobMainWnd->GetText(textwnd,255);
	fprintf(file, "CaptionElement = %s\n", textwnd);
	
	//}

	//записываем позицию и размеры
	//{
	RECT GClientRect;
	long Width,Height;
	SXMainWndElem::JobMainWnd->GetClientRect(&GClientRect);

	fprintf(file, "left = %d\n", GClientRect.left);
	fprintf(file, "top = %d\n", GClientRect.top);
	Width = GClientRect.right - GClientRect.left;
	Height = GClientRect.bottom - GClientRect.top;
	fprintf(file, "Width = %d\n", Width);
	fprintf(file, "Height = %d\n", Height);
	//}

	char FontName[256];
	long WidthFont,HeightFont,WeightFont;
	bool Italic,Underline,StrikeOut;


	DWORD ColorText,ColorTextBK,ColorBK;
	bool TransparentTextBK;

	//записываем задний фон
	//{
	ColorBK = SXMainWndElem::JobMainWnd->GetColorBrush();
	fprintf(file, "ColorBK = %d\n", ColorBK);
	//}

	//записываем использование меню
	bool MenuWindow = (SXMainWndElem::CheckBoxParamWinMenu->GetCheck() == 1) ? true : false;
	fprintf(file, "MenuWindow = %d\n", MenuWindow);

	//записываем информацию о подсказках
	//{
	bool HintVisible = SXMainWndElem::JobMainWnd->ShowHint();
	fprintf(file, "HintVisible = %d\n", HintVisible);
	char HintText[1024];
	HintText[0] = 0;
	SXMainWndElem::JobMainWnd->GetHintText(HintText);
	fprintf(file, "HintText = %s\n", HintText);
	//}

	//параметры видимости и действительности
	bool Visible = SXMainWndElem::JobMainWnd->Visible();
	fprintf(file, "Visible = %d\n", Visible);
	bool Enable = SXMainWndElem::JobMainWnd->Enable();
	fprintf(file, "Enable = %d\n", Enable);

		for(int i=0;i<CountSave;i++)
		{
			char CaptionElement[256];
			CaptionElement[0] = 0;

			fprintf(file, "\n[elem_%d]\n", i);
			fprintf(file, "SysClassName = %s\n", SXMainWndElem::CreateElements[i]->SysClassName);
			fprintf(file, "SXClassName = %s\n", SXMainWndElem::CreateElements[i]->SXClassName);
			fprintf(file, "Name = %s\n", SXMainWndElem::CreateElements[i]->Name);

				if(
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,UPDOWN_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TRACKBAR_CLASS) != 0 && 
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIComboBox") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIListBox") != 0 /*&& 
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName, STATUSCLASSNAME) != 0*/
					)
				{
					ISXGUITextual* textual = dynamic_cast<ISXGUITextual*>(SXMainWndElem::CreateElements[i]->Object);
					textual->GetText(CaptionElement,256);
					fprintf(file, "CaptionElement = %s\n", CaptionElement);
			}

			//записываем позицию и размеры
			//{
			memset(&GClientRect,0,sizeof(RECT));
			Width = Height = 0;

			SXMainWndElem::CreateElements[i]->Object->GetClientRect(&GClientRect);

			fprintf(file, "left = %d\n", GClientRect.left);
			fprintf(file, "top = %d\n", GClientRect.top);

			Width = GClientRect.right - GClientRect.left;
			Height = GClientRect.bottom - GClientRect.top;

			fprintf(file, "Width = %d\n", Width);
			fprintf(file, "Height = %d\n", Height);
			//}

				if(
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,UPDOWN_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TRACKBAR_CLASS) != 0 && 
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIBaseWnd") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButton") != 0
					)
				{
					HFONT hfont = SXMainWndElem::CreateElements[i]->Object->GetFont();
					LOGFONT LogFont;
						if(hfont == 0)
						{
							FontName[0] = 0;
							WidthFont = -1;
							HeightFont = -1;
							WeightFont = -1;

							Italic = false;
							Underline = false;
							StrikeOut = false;
						}
						else
						{
							ZeroMemory(&LogFont,sizeof(&LogFont));
							GetObject(hfont, sizeof(LogFont), &LogFont);

							sprintf(FontName,"%s",LogFont.lfFaceName);
							WidthFont = LogFont.lfWidth;
							HeightFont = LogFont.lfHeight;
							WeightFont = LogFont.lfWeight;

							Italic = LogFont.lfItalic == 1 ? true : false;
							Underline = LogFont.lfUnderline == 1 ? true : false;
							StrikeOut = LogFont.lfStrikeOut == 1 ? true : false;
						}

					fprintf(file, "FontName = %s\n", FontName);
					fprintf(file, "WidthFont = %d\n", WidthFont);
					fprintf(file, "HeightFont = %d\n", HeightFont);
					fprintf(file, "WeightFont = %d\n", WeightFont);
					fprintf(file, "Italic = %d\n", Italic);
					fprintf(file, "Underline = %d\n", Underline);
					fprintf(file, "StrikeOut = %d\n", StrikeOut);
				}

			ColorText = ColorTextBK = ColorBK = 0;

				if(
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,UPDOWN_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButton") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TRACKBAR_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,STATUSCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIBaseWnd") != 0
					)
				{
					ColorText = SXMainWndElem::CreateElements[i]->Object->GetColorText();
					ColorTextBK = SXMainWndElem::CreateElements[i]->Object->GetColorTextBk();

					fprintf(file, "ColorText = %d\n", ColorText);
					fprintf(file, "ColorTextBK = %d\n", ColorTextBK);
					TransparentTextBK = SXMainWndElem::CreateElements[i]->Object->GetTransparentTextBk();
					fprintf(file, "TransparentTextBK = %d\n", TransparentTextBK);
				}

			//записываем задний фон
			//{
				if(
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,UPDOWN_CLASS) != 0 && 
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButton") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TRACKBAR_CLASS) != 0 && 
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,STATUSCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg")
					)
				{
					ColorBK = SXMainWndElem::CreateElements[i]->Object->GetColorBrush();
					fprintf(file, "ColorBK = %d\n", ColorBK);
				}
			//}

			//записываем информацию о подсказках
			//{
			bool HintVisible = SXMainWndElem::CreateElements[i]->Object->ShowHint();
			fprintf(file, "HintVisible = %d\n", HintVisible);
			char HintText[1024];
			HintText[0] = 0;
			SXMainWndElem::CreateElements[i]->Object->GetHintText(HintText);
			fprintf(file, "HintText = %s\n", HintText);
			//}

			//параметры видимости и действительности
			bool Visible = SXMainWndElem::CreateElements[i]->Object->Visible();
			fprintf(file, "Visible = %d\n", Visible);
			bool Enable = SXMainWndElem::CreateElements[i]->Object->Enable();
			fprintf(file, "Enable = %d\n", Enable);

				if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") == 0)
				{
					char PathForImg[1024];
					PathForImg[0] = 0;
					ISXGUIButtonImg* tmpImgButton = dynamic_cast<ISXGUIButtonImg*>(SXMainWndElem::CreateElements[i]->Object);
					tmpImgButton->GetPathForImg(PathForImg);
					fprintf(file, "PathForImg = %s\n", PathForImg);
				}
		}

	fclose(file);
}

void LoadFile(const char* path)
{
	FILE * fp = fopen(path, "rb");

	ISXLConfig* tmpconfig = Core_OpLConfig(path);

	char CaptionElement[256];
	CaptionElement[0] = 0;

	char classname[256];
	sprintf(classname,"%s",tmpconfig->GetKey("JobWindow", "SysClassName"));
	
	sprintf(SXMainWndElem::NameJobWnd, "%s", tmpconfig->GetKey("JobWindow", "Name"));
	sprintf(CaptionElement, "%s", tmpconfig->GetKey("JobWindow", "CaptionElement"));

	SXMainWndElem::JobMainWnd->SetText(CaptionElement);
	RECT GClientRect;
	long Width,Height;
	
	sscanf(tmpconfig->GetKey("JobWindow", "left"), "%d", &GClientRect.left);
	sscanf(tmpconfig->GetKey("JobWindow", "top"), "%d", &GClientRect.top);
	sscanf(tmpconfig->GetKey("JobWindow", "Width"), "%d", &Width);
	sscanf(tmpconfig->GetKey("JobWindow", "Height"), "%d", &Height);

	char FontName[256];
	long WidthFont,HeightFont,WeightFont;
	BOOL Italic,Underline,StrikeOut;

	GClientRect.right = Width + GClientRect.left;
	GClientRect.bottom = Height + GClientRect.top;
	SXMainWndElem::JobMainWnd->SetClientRect(&GClientRect,false);

	DWORD ColorText,ColorTextBK,ColorBK;
	BOOL TransparentTextBK;

	sscanf(tmpconfig->GetKey("JobWindow", "ColorBK"), "%d", &ColorBK);
	SXMainWndElem::JobMainWnd->SetColorBrush(ColorBK);

	BOOL MenuWindow;
	sscanf(tmpconfig->GetKey("JobWindow", "MenuWindow"), "%d", &MenuWindow);
	SXMainWndElem::CheckBoxParamWinMenu->SetCheck(MenuWindow ? SXGUI_CHECKBOX_CHECKED : SXGUI_CHECKBOX_UNCHECKED);
	InvalidateRect(SXMainWndElem::CheckBoxParamWinMenu->GetHWND(),0,1);
	SXUpdateParam::SetMenuWindow();

	BOOL HintVisible;
	sscanf(tmpconfig->GetKey("JobWindow", "HintVisible"), "%d", &HintVisible);
	SXMainWndElem::JobMainWnd->ShowHint(HintVisible);
	char HintText[1024];
	HintText[0] = 0;
	sprintf(HintText, "%s", tmpconfig->GetKey("JobWindow", "HintText"));
	SXMainWndElem::JobMainWnd->SetHintText(HintText);

	BOOL Visible;
	sscanf(tmpconfig->GetKey("JobWindow", "Visible"), "%d", &Visible);
	SXMainWndElem::JobMainWnd->Visible(Visible);
	BOOL Enable;
	sscanf(tmpconfig->GetKey("JobWindow", "Enable"), "%d", &Enable);
	SXMainWndElem::JobMainWnd->Enable(Enable);

	int CountObject = 0;
	WORD CountSave = tmpconfig->GetSectionCount() - 1;
		for(int i=0;i<CountSave;i++)
		{
			char sectname[64];
			sprintf(sectname, "elem_%d", i);
			if (!tmpconfig->SectionExists(sectname))
				continue;

			char CaptionElement[256];
			CaptionElement[0] = 0;
			SXCreateElement* Object = new SXCreateElement();
			Object->SysClassName[0] = 0;
			Object->SXClassName[0] = 0;
			char NameElem[256];

			//считываем sys class, sx class, caption
			//{

			sprintf(Object->SysClassName, "%s", tmpconfig->GetKey(sectname, "SysClassName"));
			sprintf(Object->SXClassName, "%s", tmpconfig->GetKey(sectname, "SXClassName"));
			sprintf(Object->Name, "%s", tmpconfig->GetKey(sectname, "Name"));
			if (tmpconfig->KeyExists(sectname, "CaptionElement"))
				sprintf(CaptionElement, "%s", tmpconfig->GetKey(sectname, "CaptionElement"));
			//}

			//считываем позицию и размеры
			//{
			memset(&GClientRect,0,sizeof(RECT));
			Width = Height = 0;

			sscanf(tmpconfig->GetKey(sectname, "left"), "%d", &GClientRect.left);
			sscanf(tmpconfig->GetKey(sectname, "top"), "%d", &GClientRect.top);
			sscanf(tmpconfig->GetKey(sectname, "Width"), "%d", &Width);
			sscanf(tmpconfig->GetKey(sectname, "Height"), "%d", &Height);

			GClientRect.right = Width + GClientRect.left;
			GClientRect.bottom = Height + GClientRect.top;
			//}

				//инициализируем объект
				//{
				if(strcmp(Object->SXClassName,"SXGUIButton") == 0)
				{
					CountObject = 0;
					Object->Object = SXGUICrButton(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXGUI_BUTTON_IMAGE_NONE, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIButtonImg") == 0)
				{
					CountObject = 1;
					ISXGUIButtonImg* tmpButtonImg = SXGUICrButtonImgLoad("",GClientRect.left,GClientRect.top,Width,Height,RGB(0,0,0),RGB(100,100,100),SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
					tmpButtonImg->SetColorFrame(100, 100, 100);
					tmpButtonImg->SetEnable(true);
					tmpButtonImg->SetEnableActive(true);

					Object->Object = tmpButtonImg;
				}
				else if(strcmp(Object->SXClassName,"SXGUIEdit") == 0)
				{
					CountObject = 2;
					Object->Object = SXGUICrEdit(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUICheckBox") == 0)
				{
					CountObject = 3;
					Object->Object = SXGUICrCheckBox(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0, true);
				}
				else if(strcmp(Object->SXClassName,"SXGUIComboBox") == 0)
				{
					CountObject = 4;
					Object->Object = SXGUICrComboBox(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIGroupBox") == 0)
				{
					CountObject = 5;
					Object->Object = SXGUICrGroupBox(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIListBox") == 0)
				{
					CountObject = 6;
					Object->Object = SXGUICrListBox(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0, true);
					SendMessage(Object->Object->GetHWND(),LB_SETITEMHEIGHT,0,1);
				}
				else if(strcmp(Object->SXClassName,"SXGUIMemo") == 0)
				{
					CountObject = 7;
					Object->Object = SXGUICrMemo(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIProgressBar") == 0)
				{
					CountObject = 8;
					Object->Object = SXGUICrProgressBar(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0, true, true);
				}
				else if(strcmp(Object->SXClassName,"SXGUIRadioButton") == 0)
				{
					CountObject = 9;
					Object->Object = SXGUICrRadioButton(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIStatic") == 0)
				{
					CountObject = 10;
					Object->Object = SXGUICrStatic(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUITrackBar") == 0)
				{
					CountObject = 11;
					Object->Object = SXGUICrTrackBar(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIStatusBar") == 0)
				{
					CountObject = 12;
					Object->Object = SXGUICrStatusBar(CaptionElement, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIToolBar") == 0)
				{
					CountObject = 13;
					Object->Object = SXGUICrToolBar(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIUpDown") == 0)
				{
					CountObject = 14;
					Object->Object = SXGUICrUpDown(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->GetHWND(), WndProcChildJob, 0, 0, 0);
				}
				else if(strcmp(Object->SXClassName,"SXGUIBaseWnd") == 0)
				{
					CountObject = 15;
					Object->Object = SXGUICrBaseWnd(Object->Name, CaptionElement, 0,
													0,GClientRect.left,GClientRect.top,Width,Height,
													0,0,CreateSolidBrush(RGB(200,200,200)),
													0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,
													SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob);
				}
				//}

			//обнуляем данные для шрифта
			FontName[0] = 0;
			WidthFont = HeightFont = WeightFont = -1;
			Italic = Underline = StrikeOut = false;

				//считываем данные о шрифте
				//{
				if(
					strcmp(Object->SysClassName,UPDOWN_CLASS) != 0 && 
					strcmp(Object->SysClassName,TRACKBAR_CLASS) != 0 && 
					strcmp(Object->SysClassName,TOOLBARCLASSNAME) != 0 && 
					strcmp(Object->SysClassName,PROGRESS_CLASS) != 0 && 
					strcmp(Object->SXClassName,"SXGUIButtonImg") != 0 && 
					strcmp(Object->SXClassName,"SXGUIBaseWnd") != 0 &&
					strcmp(Object->SXClassName,"SXGUIButton") != 0
					)
				{
					sprintf(FontName, "%s", tmpconfig->GetKey(sectname, "FontName"));

						//если первый символ конец строки то используем системный шрифт
						if(FontName[0] == 0)
						{
							Object->Object->SetFont(0);
						}
						else
						{
							sscanf(tmpconfig->GetKey(sectname, "WidthFont"), "%d", &WidthFont);
							sscanf(tmpconfig->GetKey(sectname, "HeightFont"), "%d", &HeightFont);
							sscanf(tmpconfig->GetKey(sectname, "WeightFont"), "%d", &WeightFont);

							sscanf(tmpconfig->GetKey(sectname, "Italic"), "%d", &Italic);
							sscanf(tmpconfig->GetKey(sectname, "Underline"), "%d", &Underline);
							sscanf(tmpconfig->GetKey(sectname, "StrikeOut"), "%d", &StrikeOut);

							Object->Object->SetFont(FontName,HeightFont,WidthFont,WeightFont,Italic,Underline,StrikeOut);
						}
				}
				//}

			ColorText = ColorTextBK = ColorBK = 0;

				//считываем данные о цвете шрифта, цвете заднего фона шрифта
				//или используем прозрачный задний фон текста
				//{
				if(
					strcmp(Object->SysClassName,UPDOWN_CLASS) != 0 && 
					strcmp(Object->SXClassName,"SXGUIButton") != 0 &&
					strcmp(Object->SysClassName,TRACKBAR_CLASS) != 0 &&
					strcmp(Object->SysClassName,STATUSCLASSNAME) != 0 &&
					strcmp(Object->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(Object->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(Object->SXClassName,"SXGUIButtonImg") != 0 &&
					strcmp(Object->SXClassName,"SXGUIBaseWnd") != 0
					)
				{
					sscanf(tmpconfig->GetKey(sectname, "ColorText"), "%d", &ColorText);
					sscanf(tmpconfig->GetKey(sectname, "ColorTextBK"), "%d", &ColorTextBK);
					sscanf(tmpconfig->GetKey(sectname, "TransparentTextBK"), "%d", &TransparentTextBK);

					Object->Object->SetColorText(ColorText);
					Object->Object->SetColorTextBk(ColorTextBK);
					Object->Object->SetTransparentTextBk(TransparentTextBK);
				}
				//}

			//считываем цвет заднего фона
			//{
				if(
					strcmp(Object->SysClassName,UPDOWN_CLASS) != 0 && 
					strcmp(Object->SXClassName,"SXGUIButton") != 0 && 
					strcmp(Object->SysClassName,TRACKBAR_CLASS) != 0 &&
					strcmp(Object->SysClassName,STATUSCLASSNAME) != 0 &&
					strcmp(Object->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(Object->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(Object->SXClassName,"SXGUIButtonImg")
					)
				{
					sscanf(tmpconfig->GetKey(sectname, "ColorBK"), "%d", &ColorBK);
					Object->Object->SetColorBrush(ColorBK);
				}
			//}

			BOOL HintVisible;
			sscanf(tmpconfig->GetKey(sectname, "HintVisible"), "%d", &HintVisible);
			Object->Object->ShowHint(HintVisible);
			char HintText[1024];
			HintText[0] = 0;
			sprintf(HintText, "%s", tmpconfig->GetKey(sectname, "HintText"));
			Object->Object->SetHintText(HintText);

			BOOL Visible;
			sscanf(tmpconfig->GetKey(sectname, "Visible"), "%d", &Visible);
			Object->Object->Visible(Visible);
			BOOL Enable;
			sscanf(tmpconfig->GetKey(sectname, "Enable"), "%d", &Enable);
			Object->Object->Enable(Enable);

				if(strcmp(Object->SXClassName,"SXGUIButtonImg") == 0)
				{
					char PathForImg[1024];
					PathForImg[0] = 0;

					sprintf(PathForImg, "%s", tmpconfig->GetKey(sectname, "PathForImg"));
					ISXGUIButtonImg* tmpImgButton = dynamic_cast<ISXGUIButtonImg*>(Object->Object);
					tmpImgButton->ReInitImage(PathForImg);
				}

			SXMainWndElem::CountCreateNewElem++;
			//устанавливаем активным элементов вновь созданный
			SXMainWndElem::CreateElements.push_back(Object);
			//увеличиваем счетчик количества созданных элементов
			SXMainWndElem::NumSourceElements[CountObject]++;
			InLog("%s%s%s%s%s","Created element sx_class [",Object->SXClassName,"], sys_class [",Object->SysClassName,"]");

			//далее добавляем новую строку в list box
			//устнавливаем в юзердату строки порядковый номер созданного элемента
			char tmpAddItem[256];
			sprintf(tmpAddItem,"%s%s%s%s%s",Object->Name, " - ",Object->SXClassName," : ",Object->SysClassName);
			SXMainWndElem::ListBoxAllElements->AddItem(tmpAddItem);

			int countitem = SXMainWndElem::ListBoxAllElements->GetCountItem();
			SXMainWndElem::ListBoxAllElements->SetItemData(SXMainWndElem::ListBoxAllElements->GetCountItem()-1,(LPARAM)SXMainWndElem::CreateElements.size()-1);
			SXMainWndElem::ListBoxAllElements->SetSel(SXMainWndElem::NumActiveElement);

			InLog("%s","Parameters ParamWnd overwritten, serial number of elements writed (userdata32)");
		}

		for(int e=0;e<SXMainWndElem::CreateElements.size();e++)
		{
			InvalidateRect(SXMainWndElem::CreateElements[e]->Object->GetHWND(),0,1);
		}
		
	InvalidateRect(SXMainWndElem::JobMainWnd->GetHWND(),0,TRUE);
}



void CreateNew(bool reestablish)
{
	InLog("%s","start to delete all and start a new project ...");
	SXMainWndElem::NumActiveElement = -1;
	int size = SXMainWndElem::CreateElements.size();
		for(int i=0;i<size;i++)
		{
			InLog("%s%s%s","Deletion of object [",SXMainWndElem::CreateElements[0]->Name,"] ...");
			SXMainWndElem::CountDeleteElem++;
			SXMainWndElem::ListBoxAllElements->DeleteItem(0);
			delete SXMainWndElem::CreateElements[0]->Object;
			SXMainWndElem::CreateElements.erase(0);
			InLog("%s","Deleting of object completed");
		}
	InLog("%s","Clearing is completed, a new project is ready!");

		if(reestablish)
		{
			RECT wrect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
			SXMainWndElem::JobMainWnd->SetText("JobWindow");
			RECT rc;// = {100,250,wrect.right - 456,wrect.bottom-350};
			rc.left = 100;
			rc.top = 250;
			rc.right = rc.left + (wrect.right - 456);
			rc.bottom = rc.top + (wrect.bottom - 350);
			SXMainWndElem::JobMainWnd->SetClientRect(&rc,true);
		}
	//говорим что проект пустой
	SXMainWndElem::IsCreatedEl = SXMainWndElem::IsLoadedEl = false;
}

void CallCreateNew()
{
		if(SXMainWndElem::IsLoadedEl || SXMainWndElem::IsCreatedEl)
		{
				if(MessageBox(0,"Now all will be deleted! \nВы действительно хотите все удалить и начать новый проект?","Внимание!",MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
					CreateNew(true);
		}
		else
			CreateNew(true);
}

void OutputInFile(const char* path,const char* name_space,bool in_ns,bool constructors,bool destructors,bool comments,bool include)
{
	FILE* file = 0;
	file = fopen(path,"w");
	char NameSpacePP[256];
	NameSpacePP[0] = 0;

		if(include)
		{
				if(comments)
					fprintf(file,"//include the header file and the lib file\n");
			fprintf(file,"#include <SXGUIWinApi\\sxgui.h>\n#pragma comment(lib, \"SXGUIWinApi.lib\")\n\n");
		}

		if(destructors)
		{
				if(comments)
					fprintf(file,"//the marco definition for simple release the memory\n");
			fprintf(file,"#define mem_delete(p) if(p) {delete (p);(p) = NULL;}\n");
		}

		if(in_ns)
		{
			sprintf(NameSpacePP,"%s%s",name_space,"::");
		}

		if(in_ns)
		{
				if(comments)
					fprintf(file,"%s","\n//the marco definition for simple release the memory");
			fprintf(file,"%s%s%s","\nnamespace ",name_space,"\n{\n");
		}

	char NameJobWnd[256];
	SXMainWndElem::JobMainWnd->GetText(NameJobWnd,256);
		for(int i=0;i<strlen(NameJobWnd);i++)
		{
				if(NameJobWnd[i] == ' ')
					NameJobWnd[i] = '_';
		}

	fprintf(file,"%sI%s%s%s%s",(in_ns ? "\t" : ""),"SXGUIBaseWnd","* ",SXMainWndElem::NameJobWnd,";\n");
	
		for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
		{
			fprintf(file,"%sI%s%s%s%s",(in_ns ? "\t" : ""),SXMainWndElem::CreateElements[i]->SXClassName,"* ",SXMainWndElem::CreateElements[i]->Name,";\n");
		}

		if(constructors)
		{
				if(comments)
					fprintf(file,"%s%s%s","\n",(in_ns ? "\t" : ""),"//function for initialize all the elements");
			fprintf(file,"%s%s%s","\n",(in_ns ? "\t" : ""),"void InitAllElements();\n");
		}

		if(destructors)
		{
				if(comments)
					fprintf(file,"%s%s%s","\n",(in_ns ? "\t" : ""),"//function for delete all the elements");
			fprintf(file,"%s%s%s","\n",(in_ns ? "\t" : ""),"void DeleteAllElements();\n");
		}

		if(in_ns)
			fprintf(file,"%s","\n};\n");

	fprintf(file,"%s","\n");

	
		if(constructors)
		{
			fprintf(file,"%s%s%s%s","void ",(in_ns ? name_space : ""),(in_ns ? "::" : ""),"InitAllElements()\n{\n");

			RECT GClientRect;
			long Width,Height;

			fprintf(file,"\t%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd," = SXGUICrBaseWnd(");
			SXMainWndElem::JobMainWnd->GetClientRect(&GClientRect);
			Width = GClientRect.right - GClientRect.left;
			Height = GClientRect.bottom - GClientRect.top;

			fprintf(file,"%s%s%s","\"",NameJobWnd,"\",");
			fprintf(file,"%s%s%s","\"",NameJobWnd,"\",");
			fprintf(file,"%d%s",0,",");
			fprintf(file,"%d%s",0,",");
			fprintf(file,"%d%s",GClientRect.left,",");
			fprintf(file,"%d%s",GClientRect.top,",");
			fprintf(file,"%d%s",Width,",");
			fprintf(file,"%d%s",Height,",");
			fprintf(file,"%d%s",0,",");
			fprintf(file,"%d%s",0,",");

			ISXGUIBaseWnd* tmpwnd = dynamic_cast<ISXGUIBaseWnd*>(SXMainWndElem::JobMainWnd);
			DWORD SXColorBK;
			SXColorBK = tmpwnd->GetColorBrush();
			fprintf(file, "%s%d%s%d%s%d%s", "CreateSolidBrush(RGB(", GetRValue(SXColorBK), ",", GetGValue(SXColorBK), ",", GetBValue(SXColorBK), ")),"); //CreateSolidBrush(RGB(200,200,200))

			fprintf(file,"%d%s",0,",");
			fprintf(file,"%s%s","CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION",",");
			fprintf(file,"%s%s","0",",");
			fprintf(file,"%s","WndProcAllDefault);\n");

			fprintf(file, "\tSXGUIBaseHandlers::InitHandlerMsg(%s%s);\n\n", NameSpacePP, SXMainWndElem::NameJobWnd);

			char CaptionElement[256];
				for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
				{
					CaptionElement[0] = 0;
					
						if(
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName,UPDOWN_CLASS) != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TRACKBAR_CLASS) != 0 && 
							//strcmp(SXMainWndElem::CreateElements[i]->SysClassName, STATUSCLASSNAME) != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName,PROGRESS_CLASS) != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIComboBox") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIListBox") != 0
							)
						{
							//char* tmpstr = 0;
							//tmpstr = SXMainWndElem::CreateElements[i]->SysClassName;
							ISXGUITextual* textual = dynamic_cast<ISXGUITextual*>(SXMainWndElem::CreateElements[i]->Object);
							textual->GetText(CaptionElement,256);
						}

					fprintf(file,"%s%s%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name," = SXGUICr",SXMainWndElem::CreateElements[i]->SXClassName+5,"(");
					
					SXMainWndElem::CreateElements[i]->Object->GetClientRect(&GClientRect);
					Width = GClientRect.right - GClientRect.left;
					Height = GClientRect.bottom - GClientRect.top;

						if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButton") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") == 0)
						{
							ISXGUIButtonImg* tmpButtonImg = dynamic_cast<ISXGUIButtonImg*>(SXMainWndElem::CreateElements[i]->Object);
							char path[1024];
							tmpButtonImg->GetPathForImg(path);
							fprintf(file,"%s%s%s","\"",path,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s","RGB(0,0,0)",",");
							fprintf(file,"%s%s","RGB(100,100,100)",",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,");\n");

							fprintf(file,"%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->SetColorFrame(100,100,100);\n");
							fprintf(file,"%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->SetEnableActive(true);\n");
							fprintf(file,"%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->SetEnable(true);\n");
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIEdit") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							
							//Object->Object = new SXGUIEdit(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUICheckBox") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%s%s","true",");\n");
							//Object->Object = new SXGUICheckBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0,true);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIComboBox") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUIComboBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIGroupBox") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUIGroupBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIListBox") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%s%s","true",");\n");
							//Object->Object = new SXGUIListBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0,true);
							//SendMessage(Object->Object->GetHWND(),LB_SETITEMHEIGHT,0,1);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIMemo") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUIMemo(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIProgressBar") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%s%s","true",",");
							fprintf(file,"%s%s","true",");\n");
							//Object->Object = new SXGUIProgressBar(GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0,true,true);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIRadioButton") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUIRadioButton(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIStatic") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUIStatic(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUITrackBar") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUITrackBar(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIStatusBar") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUIStatusBar(Object->Name,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIToolBar") == 0)
						{
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUIToolBar(Object->Name,GClientRect.left,GClientRect.top,Width,Height,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIUpDown") == 0)
						{
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,");\n");
							//Object->Object = new SXGUIUpDown(GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob,0,0,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIBaseWnd") == 0)
						{
							fprintf(file,"%s%s%s","\"",SXMainWndElem::CreateElements[i]->Name,"\",");
							fprintf(file,"%s%s%s","\"",CaptionElement,"\",");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",GClientRect.left,",");
							fprintf(file,"%d%s",GClientRect.top,",");
							fprintf(file,"%d%s",Width,",");
							fprintf(file,"%d%s",Height,",");
							fprintf(file,"%d%s",0,",");
							fprintf(file,"%d%s",0,",");

							ISXGUIBaseWnd* tmpwnd = dynamic_cast<ISXGUIBaseWnd*>(SXMainWndElem::CreateElements[i]->Object);
							DWORD SXColorBK = tmpwnd->GetColorBrush();
							fprintf(file, "%s%d%s%d%s%d%s", "CreateSolidBrush(RGB(", GetRValue(SXColorBK), ",", GetGValue(SXColorBK), ",", GetBValue(SXColorBK), ")),"); //CreateSolidBrush(RGB(200,200,200))

							fprintf(file,"%d%s",0,",");
							fprintf(file,"%s%s","CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER",",");
							fprintf(file,"%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd,"->GetHWND(),");
							fprintf(file,"%d%s",0,");\n");
							/*Object->Object = new SXGUIBaseWnd(Object->Name,Object->Name,0,
															0,GClientRect.left,GClientRect.top,Width,Height,
															0,0,CreateSolidBrush(RGB(200,200,200)),
															0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,
															SXMainWndElem::JobMainWnd->GetHWND(),WndProcChildJob);*/
						}


						if(
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIUpDown") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUITrackBar") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIToolBar") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIProgressBar") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIBaseWnd") != 0
							)
						{
							HFONT hfont = SXMainWndElem::CreateElements[i]->Object->GetFont();

								if(hfont != 0)
								{
									LOGFONT LogFont;
									ZeroMemory(&LogFont,sizeof(&LogFont));
									GetObject(hfont, sizeof(LogFont), &LogFont);
									fprintf(file,"%s%s%s%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->SetFont(\"",LogFont.lfFaceName,"\",",LogFont.lfHeight,",",LogFont.lfWidth,",",LogFont.lfWeight,",",LogFont.lfItalic,",",LogFont.lfUnderline,",",LogFont.lfStrikeOut,");\n");
								}
						}

						if(
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIUpDown") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButton") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUITrackBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIStatusBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIProgressBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIToolBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIBaseWnd") != 0
							)
						{
							DWORD SXColorText = SXMainWndElem::CreateElements[i]->Object->GetColorText();
							DWORD SXColorTextBK = SXMainWndElem::CreateElements[i]->Object->GetColorTextBk();

							bool TransparentTextBK = SXMainWndElem::CreateElements[i]->Object->GetTransparentTextBk();

							fprintf(file, "%s%s%s%s%d%s%d%s%d%s", "\t", NameSpacePP, SXMainWndElem::CreateElements[i]->Name, "->SetColorText(", GetRValue(SXColorText), ",", GetGValue(SXColorText), ",", GetBValue(SXColorText), ");\n");
							fprintf(file, "%s%s%s%s%d%s%d%s%d%s", "\t", NameSpacePP, SXMainWndElem::CreateElements[i]->Name, "->SetColorTextBk(", GetRValue(SXColorTextBK), ",", GetGValue(SXColorTextBK), ",", GetBValue(SXColorTextBK), ");\n");
							fprintf(file,"%s%s%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->SetTransparentTextBk(",(TransparentTextBK ? "true" : "false"),");\n");
						}

					//записываем задний фон
					//{
						if(
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIUpDown") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButton") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUITrackBar") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIStatusBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIProgressBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIToolBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIButtonImg") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName,"SXGUIBaseWnd") != 0
							)
						{
							DWORD SXColorBrush = SXMainWndElem::CreateElements[i]->Object->GetColorBrush();
							fprintf(file, "%s%s%s%s%d%s%d%s%d%s", "\t", NameSpacePP, SXMainWndElem::CreateElements[i]->Name, "->SetColorBrush(", GetRValue(SXColorBrush), ",", GetGValue(SXColorBrush), ",", GetBValue(SXColorBrush), ");\n");
						}
					//}

					//{
						if(SXMainWndElem::CreateElements[i]->Object->ShowHint())
						{
							fprintf(file,"%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->ShowHint(true);\n");
							char HintText[1024];
							HintText[0] = 0;
							SXMainWndElem::CreateElements[i]->Object->GetHintText(HintText);
							fprintf(file,"%s%s%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->SetHintText(",HintText,");\n");
						}
						else
						{
							char HintText[1024];
							HintText[0] = 0;
							SXMainWndElem::CreateElements[i]->Object->GetHintText(HintText);
								if(HintText[0] != 0)
								{
									fprintf(file,"%s%s%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->SetHintText(",HintText,");\n");
									fprintf(file,"%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->ShowHint(false);\n");
								}
						}
					
					//}

						if(!SXMainWndElem::CreateElements[i]->Object->Visible())
						{
							fprintf(file,"%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->Visible(false);\n");
						}

						if(!SXMainWndElem::CreateElements[i]->Object->Enable())
						{
							fprintf(file,"%s%s%s%s","\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,"->Enable(false);\n");
						}
				}

			fprintf(file,"%s","}\n");
		}

		if(destructors)
		{
			fprintf(file,"%s%s%s","void ",NameSpacePP,"DeleteAllElements()\n{\n");

				for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
				{
					fprintf(file,"%s%s%s%s%s","\t","mem_delete(",NameSpacePP,SXMainWndElem::CreateElements[i]->Name,");\n");
				}
				fprintf(file, "%s%s%s%s%s", "\t", "mem_delete(", NameSpacePP, SXMainWndElem::NameJobWnd, ");\n");

			fprintf(file,"%s","}\n");
		}

	fclose(file);
}