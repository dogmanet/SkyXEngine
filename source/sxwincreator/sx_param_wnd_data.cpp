
#include "sx_param_wnd_data.h"

void SXUpdateParam::SetDefaultValue()
{

}

void SXUpdateParam::LockColorBlock()
{
	SXMainWndElem::StaticParamColorText->setEnable(false);
	SXMainWndElem::StaticParamColorBK->setEnable(false);
	SXMainWndElem::StaticParamColorBKText->setEnable(false);
	SXMainWndElem::StaticParamTransparentText->setEnable(false);

	SXMainWndElem::EditParamColorTextR->setEnable(false);
	SXMainWndElem::EditParamColorTextG->setEnable(false);
	SXMainWndElem::EditParamColorTextB->setEnable(false);

	SXMainWndElem::EditParamColorTextBKR->setEnable(false);
	SXMainWndElem::EditParamColorTextBKG->setEnable(false);
	SXMainWndElem::EditParamColorTextBKB->setEnable(false);

	SXMainWndElem::EditParamColorBKR->setEnable(false);
	SXMainWndElem::EditParamColorBKG->setEnable(false);
	SXMainWndElem::EditParamColorBKB->setEnable(false);

	SXMainWndElem::ComboBoxParamTransparentText->setEnable(false);
}

void SXUpdateParam::UnLockColorBlock()
{
	SXMainWndElem::StaticParamColorText->setEnable(true);
	SXMainWndElem::StaticParamColorBK->setEnable(true);
	SXMainWndElem::StaticParamColorBKText->setEnable(true);
	SXMainWndElem::StaticParamTransparentText->setEnable(true);

	SXMainWndElem::EditParamColorTextR->setEnable(true);
	SXMainWndElem::EditParamColorTextG->setEnable(true);
	SXMainWndElem::EditParamColorTextB->setEnable(true);

	SXMainWndElem::EditParamColorTextBKR->setEnable(true);
	SXMainWndElem::EditParamColorTextBKG->setEnable(true);
	SXMainWndElem::EditParamColorTextBKB->setEnable(true);

	SXMainWndElem::EditParamColorBKR->setEnable(true);
	SXMainWndElem::EditParamColorBKG->setEnable(true);
	SXMainWndElem::EditParamColorBKB->setEnable(true);

	SXMainWndElem::ComboBoxParamTransparentText->setEnable(true);
}

void SXUpdateParam::LockColorTextBlock()
{
	SXMainWndElem::StaticParamColorText->setEnable(false);
	SXMainWndElem::StaticParamColorBKText->setEnable(false);
	SXMainWndElem::StaticParamTransparentText->setEnable(false);

	SXMainWndElem::EditParamColorTextR->setEnable(false);
	SXMainWndElem::EditParamColorTextG->setEnable(false);
	SXMainWndElem::EditParamColorTextB->setEnable(false);

	SXMainWndElem::EditParamColorTextBKR->setEnable(false);
	SXMainWndElem::EditParamColorTextBKG->setEnable(false);
	SXMainWndElem::EditParamColorTextBKB->setEnable(false);

	SXMainWndElem::ComboBoxParamTransparentText->setEnable(false);
}

void SXUpdateParam::UnLockColorTextBlock()
{
	SXMainWndElem::StaticParamColorText->setEnable(true);
	SXMainWndElem::StaticParamColorBKText->setEnable(true);
	SXMainWndElem::StaticParamTransparentText->setEnable(true);

	SXMainWndElem::EditParamColorTextR->setEnable(true);
	SXMainWndElem::EditParamColorTextG->setEnable(true);
	SXMainWndElem::EditParamColorTextB->setEnable(true);

	SXMainWndElem::EditParamColorTextBKR->setEnable(true);
	SXMainWndElem::EditParamColorTextBKG->setEnable(true);
	SXMainWndElem::EditParamColorTextBKB->setEnable(true);

	SXMainWndElem::ComboBoxParamTransparentText->setEnable(true);
}

void SXUpdateParam::LockFontBlock()
{
	SXMainWndElem::StaticParamFont->setEnable(false);
	SXMainWndElem::StaticParamFontParent->setEnable(false);
	SXMainWndElem::ComboBoxParamParentFont->setEnable(false);
	SXMainWndElem::ButtonParamSelectFont->setEnable(false);
}

void SXUpdateParam::UnLockFontBlock()
{
	SXMainWndElem::StaticParamFont->setEnable(true);
	SXMainWndElem::StaticParamFontParent->setEnable(true);
	SXMainWndElem::ComboBoxParamParentFont->setEnable(true);
	SXMainWndElem::ButtonParamSelectFont->setEnable(true);
}

void SXUpdateParam::LockPosSizeBlock()
{
	SXMainWndElem::StaticParamPosX->setEnable(false);
	SXMainWndElem::StaticParamPosY->setEnable(false);
	SXMainWndElem::StaticParamWidth->setEnable(false);
	SXMainWndElem::StaticParamHeight->setEnable(false);

	SXMainWndElem::EditParamPosX->setEnable(false);
	SXMainWndElem::EditParamPosY->setEnable(false);
	SXMainWndElem::EditParamWidth->setEnable(false);
	SXMainWndElem::EditParamHeight->setEnable(false);
}

void SXUpdateParam::UnLockPosSizeBlock()
{
	SXMainWndElem::StaticParamPosX->setEnable(true);
	SXMainWndElem::StaticParamPosY->setEnable(true);
	SXMainWndElem::StaticParamWidth->setEnable(true);
	SXMainWndElem::StaticParamHeight->setEnable(true);

	SXMainWndElem::EditParamPosX->setEnable(true);
	SXMainWndElem::EditParamPosY->setEnable(true);
	SXMainWndElem::EditParamWidth->setEnable(true);
	SXMainWndElem::EditParamHeight->setEnable(true);
}

void SXUpdateParam::LockImgButton()
{
	SXMainWndElem::StaticParamImgBLoadImg->setEnable(false);
	SXMainWndElem::ButtomParamImgBLoadImg->setEnable(false);
}

void SXUpdateParam::UnLockImgButton()
{
	SXMainWndElem::StaticParamImgBLoadImg->setEnable(true);
	SXMainWndElem::ButtomParamImgBLoadImg->setEnable(true);
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
			int error = GetClassName(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHWND(),ClassName,256);
				//обычная кнопка
				if (strcmp(ClassName, "SXGUIGROUPBOX") == 0) //strcmp(ClassName, "Button") == 0 && !(GetWindowLong(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHWND(), GWL_STYLE) & BS_GROUPBOX))
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
						strcmp(ClassName, "Static") == 0 || 
						(strcmp(ClassName, "Button") == 0 && GetWindowLong(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHWND(),GWL_STYLE) & BS_GROUPBOX) ||
						strcmp(ClassName, "ListBox") == 0 || 
						strcmp(ClassName, "Edit") == 0 ||
						strcmp(ClassName, "ComboBox") == 0
						)
				{
					SXUpdateParam::UnlockAll();
					SXUpdateParam::LockImgButton();
				}
				else if(strcmp(ClassName, "SXGUIBUTTONIMG") == 0)
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
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->getClientRect(&cr);
	//GetWindowRect(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement].Object->getHWND(),&gr);
	char tmp[32];
	sprintf(tmp, "%d",cr.right - cr.left);
	SXMainWndElem::EditParamWidth->setText(tmp);
	sprintf(tmp, "%d",cr.bottom - cr.top);
	SXMainWndElem::EditParamHeight->setText(tmp);

	sprintf(tmp, "%d",cr.left);
	SXMainWndElem::EditParamPosX->setText(tmp);
	sprintf(tmp, "%d",cr.top);
	SXMainWndElem::EditParamPosY->setText(tmp);
}

void SXUpdateParam::Caption()
{
	char str[256];
	str[0] = 0;
		if(SXMainWndElem::NumActiveElement != -1)
		{
			gui_func::text::GetText(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object,str,256);
				if(str[0] == 0)
				{
					sprintf(str, "%s",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name);
				}
		}
		else
			SXMainWndElem::JobMainWnd->getText(str,256);
	SXMainWndElem::EditParamCaption->setText(str);
}

void SXUpdateParam::VarName()
{
	char str[256];
	str[0] = 0;
		if(SXMainWndElem::NumActiveElement != -1)
		{
				if(str[0] == 0)
				{
					sprintf(str, "%s",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name);
				}
		}
		else
			sprintf(str, "%s",SXMainWndElem::NameJobWnd);
	SXMainWndElem::EditParamVarName->setText(str);
}

void SXUpdateParam::Enable()
{
	bool enable;
		if(SXMainWndElem::NumActiveElement != -1)
			enable = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getEnable();
		else
			enable = SXMainWndElem::JobMainWnd->getEnable();
	
	SXMainWndElem::ComboBoxParamEnabled->setSel((enable ? 0 : 1));
}

void SXUpdateParam::Visible()
{
	bool visible;
		if(SXMainWndElem::NumActiveElement != -1)
			visible = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getVisible();
		else
			visible = SXMainWndElem::JobMainWnd->getVisible();
	
	SXMainWndElem::ComboBoxParamVisible->setSel((visible ? 0 : 1));
}

void SXUpdateParam::VisibleHint()
{
	bool visible;
		if(SXMainWndElem::NumActiveElement != -1)
			visible = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getShowHint();
		else
			visible = SXMainWndElem::JobMainWnd->getShowHint();
	
	SXMainWndElem::ComboBoxParamHintVisible->setSel((visible ? 0 : 1));
}

void SXUpdateParam::HintText()
{
	const char *str = 0;
		if(SXMainWndElem::NumActiveElement != -1)
			str = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHintText();
		else
			str = SXMainWndElem::JobMainWnd->getHintText();
	SXMainWndElem::EditParamHintText->setText(str);
}

void SXUpdateParam::ColorText()
{
		if(SXMainWndElem::NumActiveElement != -1)
		{
			char bufr[16],bufg[16],bufb[16];
			DWORD color = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getColorText();
			sprintf(bufr, "%d", GetRValue(color));
			sprintf(bufg, "%d", GetGValue(color));
			sprintf(bufb, "%d", GetBValue(color));
			SXMainWndElem::EditParamColorTextR->setText(bufr);
			SXMainWndElem::EditParamColorTextG->setText(bufg);
			SXMainWndElem::EditParamColorTextB->setText(bufb);
		}
}

void SXUpdateParam::ColorTextBK()
{
		if(SXMainWndElem::NumActiveElement != -1)
		{
			char bufr[16],bufg[16],bufb[16];
			DWORD color = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getColorTextBk();
			sprintf(bufr, "%d", GetRValue(color));
			sprintf(bufg, "%d", GetGValue(color));
			sprintf(bufb, "%d", GetBValue(color));
			SXMainWndElem::EditParamColorTextBKR->setText(bufr);
			SXMainWndElem::EditParamColorTextBKG->setText(bufg);
			SXMainWndElem::EditParamColorTextBKB->setText(bufb);
		}
}

void SXUpdateParam::ColorBK()
{
	char bufr[16],bufg[16],bufb[16];
	DWORD color;
		if(SXMainWndElem::NumActiveElement != -1)
		{
			color = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getColorBrush();
		}
		else
		{
			color = SXMainWndElem::JobMainWnd->getColorBrush();
		}
		sprintf(bufr, "%d", GetRValue(color));
		sprintf(bufg, "%d", GetGValue(color));
		sprintf(bufb, "%d", GetBValue(color));
	SXMainWndElem::EditParamColorBKR->setText(bufr);
	SXMainWndElem::EditParamColorBKG->setText(bufg);
	SXMainWndElem::EditParamColorBKB->setText(bufb);
}

void SXUpdateParam::TransparentTextBk()
{
	bool ttbk = false;
		if(SXMainWndElem::NumActiveElement != -1)
			ttbk = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getTransparentTextBk();
	
	SXMainWndElem::ComboBoxParamTransparentText->setSel((ttbk ? 0 : 1));
}

/*void SXUpdateParam::ParentFont()
{
	bool pf = false;
		if(SXMainWndElem::NumActiveElement != -1)
			pf = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->IsParentFont();
	
	SXMainWndElem::ComboBoxParamParentFont->setSel((pf ? 0 : 1));
}*/

void SXUpdateParam::Font()
{
	
		if(SXMainWndElem::NumActiveElement != -1)
		{
			HFONT hfont = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHFont();
				if(hfont == 0)
				{
					SXMainWndElem::ButtonParamSelectFont->setText("System font ...");
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
					sprintf(buf, "%s%s%d",lf.lfFaceName, ", ",font_size);
					SXMainWndElem::ButtonParamSelectFont->setText(buf);
				}
		}
}

void SXUpdateParam::ParentFont()
{
		if(SXMainWndElem::NumActiveElement != -1)
		{
			HFONT defgui = (HFONT)GetStockObject(/*ANSI_VAR_FONT*/ANSI_VAR_FONT);
			HFONT hfont = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHFont();
			
			/*LOGFONT LogFontDefGUI,LogFontCurrent;
			ZeroMemory(&LogFontDefGUI,sizeof(&LogFontDefGUI));
			GetObject(defgui, sizeof(LOGFONT), &LogFontDefGUI);
			ZeroMemory(&LogFontCurrent,sizeof(&LogFontCurrent));
			GetObject(hfont, sizeof(LOGFONT), &LogFontCurrent);*/
				if(hfont == 0)
				{
					SXMainWndElem::ComboBoxParamParentFont->setSel(0);
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
					SXMainWndElem::ComboBoxParamParentFont->setSel(1);
				}
				else
				{
					SXMainWndElem::ComboBoxParamParentFont->setSel(2);
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
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->getClientRect(&cr);
	RECT dtrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &dtrect, 0);
	char posxtext[16];
	SXMainWndElem::EditParamPosX->getText(posxtext,16);
	int posxnum = 0;
	sscanf(posxtext, "%d",&posxnum);
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
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setClientRect(&cr,true);
		else
			SXMainWndElem::JobMainWnd->setClientRect(&cr,true);
}

void SXUpdateParam::SetPosY()
{
	RECT cr;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->getClientRect(&cr);
	RECT dtrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &dtrect, 0);
	char posytext[16];
	SXMainWndElem::EditParamPosY->getText(posytext,16);
	int posynum = 0;
	sscanf(posytext, "%d",&posynum);
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
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setClientRect(&cr,true);
		else
			SXMainWndElem::JobMainWnd->setClientRect(&cr,true);
}

void SXUpdateParam::SetWidth()
{
	RECT cr;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->getClientRect(&cr);
	RECT dtrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &dtrect, 0);
	char widthtext[16];
	SXMainWndElem::EditParamWidth->getText(widthtext,16);
	int widthnum = 0;
	sscanf(widthtext, "%d",&widthnum);
		if(widthnum + cr.left > dtrect.right)
			widthnum = dtrect.right - cr.left;
	cr.right = widthnum + cr.left;

		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setClientRect(&cr,true);
		else
			SXMainWndElem::JobMainWnd->setClientRect(&cr,true);
}

void SXUpdateParam::SetHeight()
{
	RECT cr;
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getClientRect(&cr);
		else
			SXMainWndElem::JobMainWnd->getClientRect(&cr);
	RECT dtrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &dtrect, 0);
	char heighttext[16];
	SXMainWndElem::EditParamHeight->getText(heighttext,16);
	int heightnum = 0;
	sscanf(heighttext, "%d",&heightnum);
		if(heightnum + cr.top > dtrect.bottom)
			heightnum = dtrect.bottom - cr.top;
	cr.bottom = heightnum + cr.top;

		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setClientRect(&cr,true);
		else
			SXMainWndElem::JobMainWnd->setClientRect(&cr,true);
}

void SXUpdateParam::SetVisible()
{
	int sel = SXMainWndElem::ComboBoxParamVisible->getSel();
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setVisible(sel==0 ? true : false);
		else
			SXMainWndElem::JobMainWnd->setVisible(sel==0 ? true : false);
}

void SXUpdateParam::SetEnable()
{
	int sel = SXMainWndElem::ComboBoxParamEnabled->getSel();
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setEnable(sel==0 ? true : false);
		else
			SXMainWndElem::JobMainWnd->setEnable(sel==0 ? true : false);
}

void SXUpdateParam::SetCaption()
{
	char str[256];
	str[0] = 0;
	SXMainWndElem::EditParamCaption->getText(str,256);
		//если юзер удалил все символы из названия
		//он творит однозначную херню
		//то присваиваем старое имя
		if(str[0] == 0)
			SXUpdateParam::Caption();
		else
		{
				if(SXMainWndElem::NumActiveElement != -1)
				{
					gui_func::text::SetText(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object, str);
					/*sprintf(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name, "%s",str);
					char tmpAddItem[256];
					sprintf(tmpAddItem, "%s%s%s%s%s",str, " - ",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName, " : ",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SysClassName);
					SXMainWndElem::ListBoxAllElements->setTextItem(SXMainWndElem::NumActiveElement,tmpAddItem);
					SXMainWndElem::ListBoxAllElements->setSel(SXMainWndElem::NumActiveElement);*/
				}
				else
					SXMainWndElem::JobMainWnd->setText(str);
		}
}

void SXUpdateParam::SetVarName()
{
	char str[256];
	str[0] = 0;
	SXMainWndElem::EditParamVarName->getText(str,256);
		//если юзер удалил все символы из названия
		//он творит однозначную херню
		//то присваиваем старое имя
		if(str[0] == 0)
			SXUpdateParam::VarName();
		else
		{
				if(SXMainWndElem::NumActiveElement != -1)
				{
					//gui_func::text::SetText(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object,str);
					sprintf(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Name, "%s",str);
					char tmpAddItem[256];
					sprintf(tmpAddItem, "%s%s%s%s%s",str, " - ",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName, " : ",SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SysClassName);
					SXMainWndElem::ListBoxAllElements->setItemText(SXMainWndElem::NumActiveElement, tmpAddItem);
					SXMainWndElem::ListBoxAllElements->setSel(SXMainWndElem::NumActiveElement);
				}
				else
					sprintf(SXMainWndElem::NameJobWnd, "%s",str);//SXMainWndElem::JobMainWnd->setText(str);
		}
}

void SXUpdateParam::SetVisibleHint()
{
	int sel = SXMainWndElem::ComboBoxParamHintVisible->getSel();
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setShowHint(sel==0 ? true : false);
		else
			SXMainWndElem::JobMainWnd->setShowHint(sel==0 ? true : false);
}

void SXUpdateParam::SetHintText()
{
	char str[1024];
	SXMainWndElem::EditParamHintText->getText(str,1024);
	int sel = SXMainWndElem::ComboBoxParamHintVisible->getSel();
		if(SXMainWndElem::NumActiveElement != -1)
		{
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setHintText(str);
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setShowHint(sel==0 ? true : false);
		}
		else
		{
			SXMainWndElem::JobMainWnd->setHintText(str);
			SXMainWndElem::JobMainWnd->setShowHint(sel==0 ? true : false);
		}
}

void SXUpdateParam::SetColorText()
{
	char bufr[16],bufg[16],bufb[16];
	int numr,numg,numb;
	SXMainWndElem::EditParamColorTextR->getText(bufr,4);
	SXMainWndElem::EditParamColorTextG->getText(bufg,4);
	SXMainWndElem::EditParamColorTextB->getText(bufb,4);
	sscanf(bufr, "%d",&numr);
	sscanf(bufg, "%d",&numg);
	sscanf(bufb, "%d",&numb);
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setColorText(RGB(numr, numg, numb));
}

void SXUpdateParam::SetColorTextBK()
{
	char bufr[16],bufg[16],bufb[16];
	int numr,numg,numb;
	SXMainWndElem::EditParamColorTextBKR->getText(bufr,4);
	SXMainWndElem::EditParamColorTextBKG->getText(bufg,4);
	SXMainWndElem::EditParamColorTextBKB->getText(bufb,4);
	sscanf(bufr, "%d",&numr);
	sscanf(bufg, "%d",&numg);
	sscanf(bufb, "%d",&numb);
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setColorTextBk(RGB(numr, numg, numb));
}

void SXUpdateParam::SetColorBK()
{
	char bufr[16],bufg[16],bufb[16];
	int numr,numg,numb;
	SXMainWndElem::EditParamColorBKR->getText(bufr,4);
	SXMainWndElem::EditParamColorBKG->getText(bufg,4);
	SXMainWndElem::EditParamColorBKB->getText(bufb,4);
	sscanf(bufr, "%d",&numr);
	sscanf(bufg, "%d",&numg);
	sscanf(bufb, "%d",&numb);

		if(SXMainWndElem::NumActiveElement != -1)
		{
				if(strcmp(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName, "SXGUIBaseWnd") == 0)
					dynamic_cast<ISXGUIBaseWnd*>(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object)->setColorBrush(RGB(numr, numg, numb));
				else
					SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setColorBrush(RGB(numr, numg, numb));
		}
		else
			SXMainWndElem::JobMainWnd->setColorBrush(RGB(numr, numg, numb));
}

void SXUpdateParam::SetTransparentTextBk()
{
	int sel = SXMainWndElem::ComboBoxParamTransparentText->getSel();
		if(SXMainWndElem::NumActiveElement != -1)
			SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setTransparentTextBk(sel==0 ? true : false);
}

void SXUpdateParam::SetParentFont()
{
	int sel = SXMainWndElem::ComboBoxParamParentFont->getSel();
		if(SXMainWndElem::NumActiveElement != -1 && sel == 0)
		{	
			HFONT ParentFont = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getParentHFont();

			SendMessage(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHWND(),WM_SETFONT,WPARAM(ParentFont),1);
			DeleteObject(ParentFont);

			SXUpdateParam::Font();
		}
		else if(SXMainWndElem::NumActiveElement != -1 && sel == 1)
		{
			HFONT DefaultFont = (HFONT) GetStockObject(ANSI_VAR_FONT);

			SendMessage(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHWND(),WM_SETFONT,WPARAM(DefaultFont),1);
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
			hfont = SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->getHFont();
	
	CHOOSEFONT cf;

	LOGFONT lf;
	ZeroMemory(&lf,sizeof(&lf));
	GetObject(hfont, sizeof(LOGFONT), &lf);

	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof (CHOOSEFONT);
	cf.hwndOwner = 0;
	cf.lpLogFont = &lf;
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;

	SXMainWndElem::MainWnd->setEnable(false);
	SXMainWndElem::JobMainWnd->setEnable(false);
	SXMainWndElem::ParamWnd->setEnable(false);
	SXMainWndElem::WndLog->setEnable(false);
		if(ChooseFont(&cf))
		{
				if(SXMainWndElem::NumActiveElement != -1)
					SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object->setFont(lf.lfFaceName,lf.lfHeight,lf.lfWidth,lf.lfWeight,lf.lfItalic,lf.lfUnderline,lf.lfStrikeOut);
			char buf[256];
			HDC hDC = ::GetDC(NULL);
			int font_size = -MulDiv(lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
			font_size = (font_size < 0) ? - font_size : font_size;
			ReleaseDC(NULL, hDC);
			sprintf(buf, "%s%s%d",lf.lfFaceName, ", ",font_size);
			SXMainWndElem::ButtonParamSelectFont->setText(buf);
		}
	SXMainWndElem::MainWnd->setEnable(true);
	SXMainWndElem::JobMainWnd->setEnable(true);
	SXMainWndElem::ParamWnd->setEnable(true);
	SXMainWndElem::WndLog->setEnable(true);
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

	SXMainWndElem::MainWnd->setEnable(false);
	SXMainWndElem::JobMainWnd->setEnable(false);
	SXMainWndElem::ParamWnd->setEnable(false);
	SXMainWndElem::WndLog->setEnable(false);
		if(GetOpenFileName(&ofn) == TRUE)
		{
				if(SXMainWndElem::NumActiveElement != -1 && strcmp(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->SXClassName, "SXGUIButtonImg") == 0)
				{
					ISXGUIButtonImg* bimg = dynamic_cast<ISXGUIButtonImg*>(SXMainWndElem::CreateElements[SXMainWndElem::NumActiveElement]->Object);
					bimg->reInitImage(szFileName);
					InvalidateRect(bimg->getHWND(),0,1);
					SXMainWndElem::ButtomParamImgBLoadImg->setText(szFileTitle);
				}
		}
	SXMainWndElem::MainWnd->setEnable(true);
	SXMainWndElem::JobMainWnd->setEnable(true);
	SXMainWndElem::ParamWnd->setEnable(true);
	SXMainWndElem::WndLog->setEnable(true);
}

void SXUpdateParam::SetMenuWindow()
{
	int check = SXMainWndElem::CheckBoxParamWinMenu->getCheck();
		if(check == 1)
		{
			SXMainWndElem::MenuWindow->setToWindow(SXMainWndElem::JobMainWnd->getHWND());
		}
		else
			SetMenu(SXMainWndElem::JobMainWnd->getHWND(),0);
}

void SaveFile(const char* path)
{
	FILE* file = 0;
	file = fopen(path, "w");

	WORD CountSave = SXMainWndElem::CreateElements.size();

	//записываем sys class, sx class, caption
	//{
	fprintf(file, "[JobWindow]\n");
	char classname[32];
	GetClassName(SXMainWndElem::JobMainWnd->getHWND(),classname,31);
	fprintf(file, "SysClassName = %s\n", classname);
	fprintf(file, "Name = %s\n", SXMainWndElem::NameJobWnd);
	char textwnd[256];
	SXMainWndElem::JobMainWnd->getText(textwnd,255);
	fprintf(file, "CaptionElement = %s\n", textwnd);
	
	//}

	//записываем позицию и размеры
	//{
	RECT GClientRect;
	long Width,Height;
	SXMainWndElem::JobMainWnd->getClientRect(&GClientRect);

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
	ColorBK = SXMainWndElem::JobMainWnd->getColorBrush();
	fprintf(file, "ColorBK = %d\n", ColorBK);
	//}

	//записываем использование меню
	bool MenuWindow = (SXMainWndElem::CheckBoxParamWinMenu->getCheck() == 1) ? true : false;
	fprintf(file, "MenuWindow = %d\n", MenuWindow);

	//записываем информацию о подсказках
	//{
	bool HintVisible = SXMainWndElem::JobMainWnd->getShowHint();
	fprintf(file, "HintVisible = %d\n", HintVisible);
	/*char HintText[1024];
	HintText[0] = 0;*/
	const char *HintText = SXMainWndElem::JobMainWnd->getHintText();
	if (!HintText)
		HintText = "";
	fprintf(file, "HintText = %s\n", HintText);
	//}

	//параметры видимости и действительности
	bool Visible = SXMainWndElem::JobMainWnd->getVisible();
	fprintf(file, "Visible = %d\n", Visible);
	bool Enable = SXMainWndElem::JobMainWnd->getEnable();
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
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIComboBox") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIListBox") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIListView") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName, STATUSCLASSNAME) != 0
					)
				{
					//char* tmptxt = SXMainWndElem::CreateElements[i]->SysClassName;
					ISXGUITextual* textual = dynamic_cast<ISXGUITextual*>(SXMainWndElem::CreateElements[i]->Object);
					textual->getText(CaptionElement,256);
					fprintf(file, "CaptionElement = %s\n", CaptionElement);
			}

			//записываем позицию и размеры
			//{
			memset(&GClientRect,0,sizeof(RECT));
			Width = Height = 0;

			SXMainWndElem::CreateElements[i]->Object->getClientRect(&GClientRect);

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
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIBaseWnd") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButton") != 0
					)
				{
					HFONT hfont = SXMainWndElem::CreateElements[i]->Object->getHFont();
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

							sprintf(FontName, "%s",LogFont.lfFaceName);
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
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButton") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TRACKBAR_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,STATUSCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIBaseWnd") != 0
					)
				{
					ColorText = SXMainWndElem::CreateElements[i]->Object->getColorText();
					ColorTextBK = SXMainWndElem::CreateElements[i]->Object->getColorTextBk();

					fprintf(file, "ColorText = %d\n", ColorText);
					fprintf(file, "ColorTextBK = %d\n", ColorTextBK);
					TransparentTextBK = SXMainWndElem::CreateElements[i]->Object->getTransparentTextBk();
					fprintf(file, "TransparentTextBK = %d\n", TransparentTextBK);
				}

			//записываем задний фон
			//{
				if(
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,UPDOWN_CLASS) != 0 && 
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButton") != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TRACKBAR_CLASS) != 0 && 
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,STATUSCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg")
					)
				{
					ColorBK = SXMainWndElem::CreateElements[i]->Object->getColorBrush();
					fprintf(file, "ColorBK = %d\n", ColorBK);
				}
			//}

			//записываем информацию о подсказках
			//{
			bool HintVisible = SXMainWndElem::CreateElements[i]->Object->getShowHint();
			fprintf(file, "HintVisible = %d\n", HintVisible);
			/*char HintText[1024];
			HintText[0] = 0;*/
			const char *HintText = SXMainWndElem::CreateElements[i]->Object->getHintText();
			if (!HintText)
				HintText = "";
			fprintf(file, "HintText = %s\n", HintText);
			//}

			//параметры видимости и действительности
			bool Visible = SXMainWndElem::CreateElements[i]->Object->getVisible();
			fprintf(file, "Visible = %d\n", Visible);
			bool Enable = SXMainWndElem::CreateElements[i]->Object->getEnable();
			fprintf(file, "Enable = %d\n", Enable);

				if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") == 0)
				{
					char PathForImg[1024];
					PathForImg[0] = 0;
					ISXGUIButtonImg* tmpImgButton = dynamic_cast<ISXGUIButtonImg*>(SXMainWndElem::CreateElements[i]->Object);
					tmpImgButton->getPathForImg(PathForImg);
					fprintf(file, "PathForImg = %s\n", PathForImg);
				}
		}

	fclose(file);
}

void LoadFile(const char* path)
{
	FILE * fp = fopen(path, "rb");

	ISXConfig* tmpconfig = Core_OpConfig(path);

	char CaptionElement[256];
	CaptionElement[0] = 0;

	char classname[256];
	sprintf(classname, "%s",tmpconfig->getKey("JobWindow", "SysClassName"));
	
	sprintf(SXMainWndElem::NameJobWnd, "%s", tmpconfig->getKey("JobWindow", "Name"));
	sprintf(CaptionElement, "%s", tmpconfig->getKey("JobWindow", "CaptionElement"));

	SXMainWndElem::JobMainWnd->setText(CaptionElement);
	RECT GClientRect;
	long Width,Height;
	
	sscanf(tmpconfig->getKey("JobWindow", "left"), "%d", &GClientRect.left);
	sscanf(tmpconfig->getKey("JobWindow", "top"), "%d", &GClientRect.top);
	sscanf(tmpconfig->getKey("JobWindow", "Width"), "%d", &Width);
	sscanf(tmpconfig->getKey("JobWindow", "Height"), "%d", &Height);

	char FontName[256];
	long WidthFont,HeightFont,WeightFont;
	BOOL Italic,Underline,StrikeOut;

	GClientRect.right = Width + GClientRect.left;
	GClientRect.bottom = Height + GClientRect.top;
	SXMainWndElem::JobMainWnd->setClientRect(&GClientRect,false);

	DWORD ColorText,ColorTextBK,ColorBK;
	BOOL TransparentTextBK;

	sscanf(tmpconfig->getKey("JobWindow", "ColorBK"), "%d", &ColorBK);
	SXMainWndElem::JobMainWnd->setColorBrush(ColorBK);

	BOOL MenuWindow;
	sscanf(tmpconfig->getKey("JobWindow", "MenuWindow"), "%d", &MenuWindow);
	SXMainWndElem::CheckBoxParamWinMenu->setCheck(MenuWindow ? SXGUI_CHECKBOX_STATE_CHECKED : SXGUI_CHECKBOX_STATE_UNCHECKED);
	InvalidateRect(SXMainWndElem::CheckBoxParamWinMenu->getHWND(),0,1);
	SXUpdateParam::SetMenuWindow();

	BOOL HintVisible;
	sscanf(tmpconfig->getKey("JobWindow", "HintVisible"), "%d", &HintVisible);
	SXMainWndElem::JobMainWnd->setShowHint(HintVisible);
	char HintText[1024];
	HintText[0] = 0;
	sprintf(HintText, "%s", tmpconfig->getKey("JobWindow", "HintText"));
	if (strlen(HintText) > 0)
		SXMainWndElem::JobMainWnd->setHintText(HintText);

	BOOL Visible;
	sscanf(tmpconfig->getKey("JobWindow", "Visible"), "%d", &Visible);
	SXMainWndElem::JobMainWnd->setVisible(Visible);
	BOOL Enable;
	sscanf(tmpconfig->getKey("JobWindow", "Enable"), "%d", &Enable);
	SXMainWndElem::JobMainWnd->setEnable(Enable);

	int CountObject = 0;
	WORD CountSave = tmpconfig->getSectionCount() - 1;
		for(int i=0;i<CountSave;i++)
		{
			char sectname[64];
			sprintf(sectname, "elem_%d", i);
			if (!tmpconfig->sectionExists(sectname))
				continue;

			char CaptionElement[256];
			CaptionElement[0] = 0;
			SXCreateElement* Object = new SXCreateElement();
			Object->SysClassName[0] = 0;
			Object->SXClassName[0] = 0;
			char NameElem[256];

			//считываем sys class, sx class, caption
			//{

			sprintf(Object->SysClassName, "%s", tmpconfig->getKey(sectname, "SysClassName"));
			sprintf(Object->SXClassName, "%s", tmpconfig->getKey(sectname, "SXClassName"));
			sprintf(Object->Name, "%s", tmpconfig->getKey(sectname, "Name"));
			if (tmpconfig->keyExists(sectname, "CaptionElement"))
				sprintf(CaptionElement, "%s", tmpconfig->getKey(sectname, "CaptionElement"));
			//}

			//считываем позицию и размеры
			//{
			memset(&GClientRect,0,sizeof(RECT));
			Width = Height = 0;

			sscanf(tmpconfig->getKey(sectname, "left"), "%d", &GClientRect.left);
			sscanf(tmpconfig->getKey(sectname, "top"), "%d", &GClientRect.top);
			sscanf(tmpconfig->getKey(sectname, "Width"), "%d", &Width);
			sscanf(tmpconfig->getKey(sectname, "Height"), "%d", &Height);

			GClientRect.right = Width + GClientRect.left;
			GClientRect.bottom = Height + GClientRect.top;
			//}

				//инициализируем объект
				//{
				if(strcmp(Object->SXClassName, "SXGUIButton") == 0)
				{
					CountObject = 0;
					Object->Object = SXGUICrButton(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXGUI_BUTTON_IMAGE_NONE, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIButtonImg") == 0)
				{
					CountObject = 1;
					ISXGUIButtonImg* tmpButtonImg = SXGUICrButtonImgLoad("",GClientRect.left,GClientRect.top,Width,Height,RGB(0,0,0),RGB(100,100,100),SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
					tmpButtonImg->setColorFrame(RGB(100, 100, 100));
					tmpButtonImg->setEnable(true);
					tmpButtonImg->setActive(true);

					Object->Object = tmpButtonImg;
				}
				else if(strcmp(Object->SXClassName, "SXGUIEdit") == 0)
				{
					CountObject = 2;
					Object->Object = SXGUICrEdit(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUICheckBox") == 0)
				{
					CountObject = 3;
					Object->Object = SXGUICrCheckBox(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0, true);
				}
				else if(strcmp(Object->SXClassName, "SXGUIComboBox") == 0)
				{
					CountObject = 4;
					Object->Object = SXGUICrComboBox(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIGroupBox") == 0)
				{
					CountObject = 5;
					Object->Object = SXGUICrGroupBox(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIListBox") == 0)
				{
					CountObject = 6;
					Object->Object = SXGUICrListBox(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0, true);
					SendMessage(Object->Object->getHWND(),LB_SETITEMHEIGHT,0,1);
				}
				else if (strcmp(Object->SXClassName, "SXGUIListView") == 0)
				{
					CountObject = 7;
					Object->Object = SXGUICrListView(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIMemo") == 0)
				{
					CountObject = 8;
					Object->Object = SXGUICrMemo(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIProgressBar") == 0)
				{
					CountObject = 9;
					Object->Object = SXGUICrProgressBar(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0, true, true);
				}
				else if(strcmp(Object->SXClassName, "SXGUIRadioButton") == 0)
				{
					CountObject = 10;
					Object->Object = SXGUICrRadioButton(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIStatic") == 0)
				{
					CountObject = 11;
					Object->Object = SXGUICrStatic(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUITrackBar") == 0)
				{
					CountObject = 12;
					Object->Object = SXGUICrTrackBar(CaptionElement, GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIStatusBar") == 0)
				{
					CountObject = 13;
					Object->Object = SXGUICrStatusBar(CaptionElement, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIToolBar") == 0)
				{
					CountObject = 14;
					Object->Object = SXGUICrToolBar(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIUpDown") == 0)
				{
					CountObject = 15;
					Object->Object = SXGUICrUpDown(GClientRect.left, GClientRect.top, Width, Height, SXMainWndElem::JobMainWnd->getHWND(), WndProcChildJob, 0, 0, 0);
				}
				else if(strcmp(Object->SXClassName, "SXGUIBaseWnd") == 0)
				{
					CountObject = 16;
					Object->Object = SXGUICrBaseWndEx(Object->Name, CaptionElement,
													GClientRect.left,GClientRect.top,Width,Height,
													0,0,CreateSolidBrush(RGB(200,200,200)),
													0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,
													SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob);
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
					strcmp(Object->SXClassName, "SXGUIButtonImg") != 0 && 
					strcmp(Object->SXClassName, "SXGUIBaseWnd") != 0 &&
					strcmp(Object->SXClassName, "SXGUIButton") != 0
					)
				{
					sprintf(FontName, "%s", tmpconfig->getKey(sectname, "FontName"));

						//если первый символ конец строки то используем системный шрифт
						if(FontName[0] == 0)
						{
							Object->Object->setHFont(0);
						}
						else
						{
							sscanf(tmpconfig->getKey(sectname, "WidthFont"), "%d", &WidthFont);
							sscanf(tmpconfig->getKey(sectname, "HeightFont"), "%d", &HeightFont);
							sscanf(tmpconfig->getKey(sectname, "WeightFont"), "%d", &WeightFont);

							sscanf(tmpconfig->getKey(sectname, "Italic"), "%d", &Italic);
							sscanf(tmpconfig->getKey(sectname, "Underline"), "%d", &Underline);
							sscanf(tmpconfig->getKey(sectname, "StrikeOut"), "%d", &StrikeOut);

							Object->Object->setFont(FontName,HeightFont,WidthFont,WeightFont,Italic,Underline,StrikeOut);
						}
				}
				//}

			ColorText = ColorTextBK = ColorBK = 0;

				//считываем данные о цвете шрифта, цвете заднего фона шрифта
				//или используем прозрачный задний фон текста
				//{
				if(
					strcmp(Object->SysClassName,UPDOWN_CLASS) != 0 && 
					strcmp(Object->SXClassName, "SXGUIButton") != 0 &&
					strcmp(Object->SysClassName,TRACKBAR_CLASS) != 0 &&
					strcmp(Object->SysClassName,STATUSCLASSNAME) != 0 &&
					strcmp(Object->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(Object->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(Object->SXClassName, "SXGUIButtonImg") != 0 &&
					strcmp(Object->SXClassName, "SXGUIBaseWnd") != 0
					)
				{
					sscanf(tmpconfig->getKey(sectname, "ColorText"), "%d", &ColorText);
					sscanf(tmpconfig->getKey(sectname, "ColorTextBK"), "%d", &ColorTextBK);
					sscanf(tmpconfig->getKey(sectname, "TransparentTextBK"), "%d", &TransparentTextBK);

					Object->Object->setColorText(ColorText);
					Object->Object->setColorTextBk(ColorTextBK);
					Object->Object->setTransparentTextBk(TransparentTextBK);
				}
				//}

			//считываем цвет заднего фона
			//{
				if(
					strcmp(Object->SysClassName,UPDOWN_CLASS) != 0 && 
					strcmp(Object->SXClassName, "SXGUIButton") != 0 && 
					strcmp(Object->SysClassName,TRACKBAR_CLASS) != 0 &&
					strcmp(Object->SysClassName,STATUSCLASSNAME) != 0 &&
					strcmp(Object->SysClassName,PROGRESS_CLASS) != 0 &&
					strcmp(Object->SysClassName,TOOLBARCLASSNAME) != 0 &&
					strcmp(Object->SXClassName, "SXGUIButtonImg")
					)
				{
					sscanf(tmpconfig->getKey(sectname, "ColorBK"), "%d", &ColorBK);
					Object->Object->setColorBrush(ColorBK);
				}
			//}

			BOOL HintVisible;
			sscanf(tmpconfig->getKey(sectname, "HintVisible"), "%d", &HintVisible);
			Object->Object->setShowHint(HintVisible);
			char HintText[1024];
			HintText[0] = 0;
			sprintf(HintText, "%s", tmpconfig->getKey(sectname, "HintText"));
			if (strlen(HintText) > 0)
				Object->Object->setHintText(HintText);

			BOOL Visible;
			sscanf(tmpconfig->getKey(sectname, "Visible"), "%d", &Visible);
			Object->Object->setVisible(Visible);
			BOOL Enable;
			sscanf(tmpconfig->getKey(sectname, "Enable"), "%d", &Enable);
			Object->Object->setEnable(Enable);

				if(strcmp(Object->SXClassName, "SXGUIButtonImg") == 0)
				{
					char PathForImg[1024];
					PathForImg[0] = 0;

					sprintf(PathForImg, "%s", tmpconfig->getKey(sectname, "PathForImg"));
					ISXGUIButtonImg* tmpImgButton = dynamic_cast<ISXGUIButtonImg*>(Object->Object);
					tmpImgButton->reInitImage(PathForImg);
				}

			SXMainWndElem::CountCreateNewElem++;
			//устанавливаем активным элементов вновь созданный
			SXMainWndElem::CreateElements.push_back(Object);
			//увеличиваем счетчик количества созданных элементов
			SXMainWndElem::NumSourceElements[CountObject]++;
			InLog("%s%s%s%s%s", "Created element sx_class [",Object->SXClassName, "], sys_class [",Object->SysClassName, "]");

			//далее добавляем новую строку в list box
			//устнавливаем в юзердату строки порядковый номер созданного элемента
			char tmpAddItem[256];
			sprintf(tmpAddItem, "%s%s%s%s%s",Object->Name, " - ",Object->SXClassName, " : ",Object->SysClassName);
			SXMainWndElem::ListBoxAllElements->addItem(tmpAddItem);

			int countitem = SXMainWndElem::ListBoxAllElements->getItemCount();
			SXMainWndElem::ListBoxAllElements->setItemData(SXMainWndElem::ListBoxAllElements->getItemCount() - 1, (LPARAM)SXMainWndElem::CreateElements.size() - 1);
			SXMainWndElem::ListBoxAllElements->setSel(SXMainWndElem::NumActiveElement);

			InLog("%s", "Parameters ParamWnd overwritten, serial number of elements writed (userdata32)");
		}

		for(int e=0;e<SXMainWndElem::CreateElements.size();e++)
		{
			InvalidateRect(SXMainWndElem::CreateElements[e]->Object->getHWND(),0,1);
		}
		
	InvalidateRect(SXMainWndElem::JobMainWnd->getHWND(),0,TRUE);
}



void CreateNew(bool reestablish)
{
	InLog("%s", "start to delete all and start a new project ...");
	SXMainWndElem::NumActiveElement = -1;
	int size = SXMainWndElem::CreateElements.size();
		for(int i=0;i<size;i++)
		{
			InLog("%s%s%s", "Deletion of object [",SXMainWndElem::CreateElements[0]->Name, "] ...");
			SXMainWndElem::CountDeleteElem++;
			SXMainWndElem::ListBoxAllElements->deleteItem(0);
			delete SXMainWndElem::CreateElements[0]->Object;
			SXMainWndElem::CreateElements.erase(0);
			InLog("%s", "Deleting of object completed");
		}
	InLog("%s", "Clearing is completed, a new project is ready!");

		if(reestablish)
		{
			RECT wrect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
			SXMainWndElem::JobMainWnd->setText("JobWindow");
			RECT rc;// = {100,250,wrect.right - 456,wrect.bottom-350};
			rc.left = 100;
			rc.top = 250;
			rc.right = rc.left + (wrect.right - 456);
			rc.bottom = rc.top + (wrect.bottom - 350);
			SXMainWndElem::JobMainWnd->setClientRect(&rc,true);
		}
	//говорим что проект пустой
	SXMainWndElem::IsCreatedEl = SXMainWndElem::IsLoadedEl = false;
}

void CallCreateNew()
{
		if(SXMainWndElem::IsLoadedEl || SXMainWndElem::IsCreatedEl)
		{
				if(MessageBox(0, "Now all will be deleted! \nВы действительно хотите все удалить и начать новый проект?", "Внимание!",MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
					CreateNew(true);
		}
		else
			CreateNew(true);
}

void OutputInFile(const char* path,const char* name_space,bool in_ns,bool constructors,bool destructors,bool comments,bool include)
{
	FILE* file = 0;
	file = fopen(path, "w");
	char NameSpacePP[256];
	NameSpacePP[0] = 0;

		/*if(include)
		{
				if(comments)
					fprintf(file, "//include the header file and the lib file\n");
			fprintf(file, "#include <SXGUIWinApi\\sxgui.h>\n#pragma comment(lib, \"SXGUIWinApi.lib\")\n\n");
		}*/

		/*if(destructors)
		{
				if(comments)
					fprintf(file, "//the marco definition for simple release the memory\n");
			fprintf(file, "#define mem_delete(p) if(p) {delete (p);(p) = NULL;}\n");
		}*/

		if(in_ns)
		{
			sprintf(NameSpacePP, "%s%s",name_space, "::");
		}

		if(in_ns)
		{
				if(comments)
					fprintf(file, "%s", "\n//the marco definition for simple release the memory");
			fprintf(file, "%s%s%s", "\nnamespace ",name_space, "\n{\n");
		}

	char NameJobWnd[256];
	SXMainWndElem::JobMainWnd->getText(NameJobWnd,256);
		for(int i=0;i<strlen(NameJobWnd);i++)
		{
				if(NameJobWnd[i] == ' ')
					NameJobWnd[i] = '_';
		}

	fprintf(file, "%sI%s *%s%s",(in_ns ? "\t" : ""), "SXGUIBaseWnd",SXMainWndElem::NameJobWnd, ";\n");
	
		for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
		{
			fprintf(file, "%sI%s *%s%s",(in_ns ? "\t" : ""),SXMainWndElem::CreateElements[i]->SXClassName,SXMainWndElem::CreateElements[i]->Name, ";\n");
		}

		if(constructors)
		{
				if(comments)
					fprintf(file, "%s%s%s", "\n",(in_ns ? "\t" : ""), "//function for initialize all the elements");
			fprintf(file, "%s%s%s", "\n",(in_ns ? "\t" : ""), "void InitAllElements();\n");
		}

		if(destructors)
		{
				if(comments)
					fprintf(file, "%s%s%s", "\n",(in_ns ? "\t" : ""), "//function for delete all the elements");
			fprintf(file, "%s%s%s", "\n",(in_ns ? "\t" : ""), "void DeleteAllElements();\n");
		}

		if(in_ns)
			fprintf(file, "%s", "\n};\n");

	fprintf(file, "%s", "\n");

	
		if(constructors)
		{
			fprintf(file, "%s%s%s%s", "void ",(in_ns ? name_space : ""),(in_ns ? "::" : ""), "InitAllElements()\n{\n");

			RECT GClientRect;
			long Width,Height;

			fprintf(file, "\t%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, " = SXGUICrBaseWndEx(");
			SXMainWndElem::JobMainWnd->getClientRect(&GClientRect);
			Width = GClientRect.right - GClientRect.left;
			Height = GClientRect.bottom - GClientRect.top;

			fprintf(file, "%s%s%s", "\"",NameJobWnd, "\", ");
			fprintf(file, "%s%s%s", "\"",NameJobWnd, "\", ");
			//fprintf(file, "%d%s",0, ", ");
			//fprintf(file, "%d%s",0, ", ");
			fprintf(file, "%d%s",GClientRect.left, ", ");
			fprintf(file, "%d%s",GClientRect.top, ", ");
			fprintf(file, "%d%s",Width, ", ");
			fprintf(file, "%d%s",Height, ", ");
			fprintf(file, "%d%s",0, ", ");
			fprintf(file, "%d%s",0, ", ");

			ISXGUIBaseWnd* tmpwnd = dynamic_cast<ISXGUIBaseWnd*>(SXMainWndElem::JobMainWnd);
			DWORD SXColorBK;
			SXColorBK = tmpwnd->getColorBrush();
			fprintf(file, "%s%d%s%d%s%d%s", "CreateSolidBrush(RGB(", GetRValue(SXColorBK), ", ", GetGValue(SXColorBK), ", ", GetBValue(SXColorBK), ")), "); //CreateSolidBrush(RGB(200,200,200))

			fprintf(file, "%d%s",0, ", ");
			fprintf(file, "%s%s", "CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION", ", ");
			fprintf(file, "%s%s", "0", ", ");
			fprintf(file, "%s", "WndProcAllDefault);\n");

			fprintf(file, "\tgui_func::base_handlers::InitHandlerMsg(%s%s);\n\n", NameSpacePP, SXMainWndElem::NameJobWnd);

			char CaptionElement[256];
				for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
				{
					CaptionElement[0] = 0;
					
						if(
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName,UPDOWN_CLASS) != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TRACKBAR_CLASS) != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName, STATUSCLASSNAME) != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName,PROGRESS_CLASS) != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIComboBox") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SysClassName,TOOLBARCLASSNAME) != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIListBox") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIListView") != 0
							)
						{
							//char* tmpstr = 0;
							//tmpstr = SXMainWndElem::CreateElements[i]->SysClassName;
							ISXGUITextual* textual = dynamic_cast<ISXGUITextual*>(SXMainWndElem::CreateElements[i]->Object);
							textual->getText(CaptionElement,256);
						}

					fprintf(file, "%s%s%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, " = SXGUICr",SXMainWndElem::CreateElements[i]->SXClassName+5, "(");
					
					SXMainWndElem::CreateElements[i]->Object->getClientRect(&GClientRect);
					Width = GClientRect.right - GClientRect.left;
					Height = GClientRect.bottom - GClientRect.top;

						if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButton") == 0)
						{
							fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "SXGUI_BUTTON_IMAGE_NONE, ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") == 0)
						{
							ISXGUIButtonImg* tmpButtonImg = dynamic_cast<ISXGUIButtonImg*>(SXMainWndElem::CreateElements[i]->Object);
							char path[1024];
							tmpButtonImg->getPathForImg(path);
							fprintf(file, "%s%s%s", "\"",path, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s", "RGB(0,0,0)", ", ");
							fprintf(file, "%s%s", "RGB(100,100,100)", ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ");\n");

							fprintf(file, "%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setColorFrame(100,100,100);\n");
							fprintf(file, "%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->seActive(true);\n");
							fprintf(file, "%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setEnable(true);\n");
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIEdit") == 0)
						{
							fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							
							//Object->Object = new SXGUIEdit(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUICheckBox") == 0)
						{
							fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%s%s", "true", ");\n");
							//Object->Object = new SXGUICheckBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0,true);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIComboBox") == 0)
						{
							//fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUIComboBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIGroupBox") == 0)
						{
							fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUIGroupBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIListBox") == 0)
						{
							//fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%s%s", "true", ");\n");
							//Object->Object = new SXGUIListBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0,true);
							//SendMessage(Object->Object->getHWND(),LB_SETITEMHEIGHT,0,1);
						}
						else if (strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIListView") == 0)
						{
							//fprintf(file, "%s%s%s", "\"", CaptionElement, "\", ");
							fprintf(file, "%d%s", GClientRect.left, ", ");
							fprintf(file, "%d%s", GClientRect.top, ", ");
							fprintf(file, "%d%s", Width, ", ");
							fprintf(file, "%d%s", Height, ", ");
							fprintf(file, "%s%s%s", NameSpacePP, SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s", 0, ", ");
							fprintf(file, "%d%s", 0, ");\n");
							//Object->Object = new SXGUIListBox(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0,true);
							//SendMessage(Object->Object->getHWND(),LB_SETITEMHEIGHT,0,1);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIMemo") == 0)
						{
							fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUIMemo(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIProgressBar") == 0)
						{
							//fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%s%s", "true", ", ");
							fprintf(file, "%s%s", "true", ");\n");
							//Object->Object = new SXGUIProgressBar(GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0,true,true);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIRadioButton") == 0)
						{
							fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUIRadioButton(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIStatic") == 0)
						{
							fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUIStatic(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUITrackBar") == 0)
						{
							//fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUITrackBar(Object->Name,GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIStatusBar") == 0)
						{
							//fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUIStatusBar(Object->Name,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIToolBar") == 0)
						{
							//fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUIToolBar(Object->Name,GClientRect.left,GClientRect.top,Width,Height,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIUpDown") == 0)
						{
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ");\n");
							//Object->Object = new SXGUIUpDown(GClientRect.left,GClientRect.top,Width,Height,SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob,0,0,0);
						}
						else if(strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIBaseWnd") == 0)
						{
							fprintf(file, "%s%s%s", "\"",SXMainWndElem::CreateElements[i]->Name, "\", ");
							fprintf(file, "%s%s%s", "\"",CaptionElement, "\", ");
							//fprintf(file, "%d%s",0, ", ");
							//fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",GClientRect.left, ", ");
							fprintf(file, "%d%s",GClientRect.top, ", ");
							fprintf(file, "%d%s",Width, ", ");
							fprintf(file, "%d%s",Height, ", ");
							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%d%s",0, ", ");

							ISXGUIBaseWnd* tmpwnd = dynamic_cast<ISXGUIBaseWnd*>(SXMainWndElem::CreateElements[i]->Object);
							DWORD SXColorBK = tmpwnd->getColorBrush();
							fprintf(file, "%s%d%s%d%s%d%s", "CreateSolidBrush(RGB(", GetRValue(SXColorBK), ", ", GetGValue(SXColorBK), ", ", GetBValue(SXColorBK), ")), "); //CreateSolidBrush(RGB(200,200,200))

							fprintf(file, "%d%s",0, ", ");
							fprintf(file, "%s%s", "CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER", ", ");
							fprintf(file, "%s%s%s",NameSpacePP,SXMainWndElem::NameJobWnd, "->getHWND(), ");
							fprintf(file, "%d%s",0, ");\n");
							/*Object->Object = new SXGUIBaseWnd(Object->Name,Object->Name,0,
															0,GClientRect.left,GClientRect.top,Width,Height,
															0,0,CreateSolidBrush(RGB(200,200,200)),
															0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,
															SXMainWndElem::JobMainWnd->getHWND(),WndProcChildJob);*/
						}


						if(
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIUpDown") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUITrackBar") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIToolBar") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIProgressBar") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIBaseWnd") != 0
							)
						{
							HFONT hfont = SXMainWndElem::CreateElements[i]->Object->getHFont();

								if(hfont != 0)
								{
									LOGFONT LogFont;
									ZeroMemory(&LogFont,sizeof(&LogFont));
									GetObject(hfont, sizeof(LogFont), &LogFont);
									fprintf(file, "%s%s%s%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setFont(\"",LogFont.lfFaceName, "\", ",LogFont.lfHeight, ", ",LogFont.lfWidth, ", ",LogFont.lfWeight, ", ",LogFont.lfItalic, ", ",LogFont.lfUnderline, ", ",LogFont.lfStrikeOut, ");\n");
								}
						}

						if(
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIUpDown") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButton") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUITrackBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIStatusBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIProgressBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIToolBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIBaseWnd") != 0
							)
						{
							DWORD SXColorText = SXMainWndElem::CreateElements[i]->Object->getColorText();
							DWORD SXColorTextBK = SXMainWndElem::CreateElements[i]->Object->getColorTextBk();

							bool TransparentTextBK = SXMainWndElem::CreateElements[i]->Object->getTransparentTextBk();

							fprintf(file, "%s%s%s%s%d%s%d%s%d%s", "\t", NameSpacePP, SXMainWndElem::CreateElements[i]->Name, "->setColorText(RGB(", GetRValue(SXColorText), ", ", GetGValue(SXColorText), ", ", GetBValue(SXColorText), "));\n");
							fprintf(file, "%s%s%s%s%d%s%d%s%d%s", "\t", NameSpacePP, SXMainWndElem::CreateElements[i]->Name, "->setColorTextBk(RGB(", GetRValue(SXColorTextBK), ", ", GetGValue(SXColorTextBK), ", ", GetBValue(SXColorTextBK), "));\n");
							fprintf(file, "%s%s%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setTransparentTextBk(",(TransparentTextBK ? "true" : "false"), ");\n");
						}

					//записываем задний фон
					//{
						if(
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIUpDown") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButton") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUITrackBar") != 0 && 
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIStatusBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIProgressBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIToolBar") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIButtonImg") != 0 &&
							strcmp(SXMainWndElem::CreateElements[i]->SXClassName, "SXGUIBaseWnd") != 0
							)
						{
							DWORD SXColorBrush = SXMainWndElem::CreateElements[i]->Object->getColorBrush();
							fprintf(file, "%s%s%s%s%d%s%d%s%d%s", "\t", NameSpacePP, SXMainWndElem::CreateElements[i]->Name, "->setColorBrush(RGB(", GetRValue(SXColorBrush), ", ", GetGValue(SXColorBrush), ", ", GetBValue(SXColorBrush), "));\n");
						}
					//}

					//{
						if(SXMainWndElem::CreateElements[i]->Object->getShowHint())
						{
							fprintf(file, "%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setShowHint(true);\n");
							/*char HintText[1024];
							HintText[0] = 0;*/
							const char *HintText = SXMainWndElem::CreateElements[i]->Object->getHintText();
							fprintf(file, "%s%s%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setHintText(",HintText, ");\n");
						}
						else
						{
							/*char HintText[1024];
							HintText[0] = 0;*/
							const char *HintText = SXMainWndElem::CreateElements[i]->Object->getHintText();
								if(HintText[0] != 0)
								{
									fprintf(file, "%s%s%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setHintText(",HintText, ");\n");
									fprintf(file, "%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setShowHint(false);\n");
								}
						}
					
					//}

						if(!SXMainWndElem::CreateElements[i]->Object->getVisible())
						{
							fprintf(file, "%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->set(false);\n");
						}

						if(!SXMainWndElem::CreateElements[i]->Object->getEnable())
						{
							fprintf(file, "%s%s%s%s", "\t",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, "->setEnable(false);\n");
						}
				}

			fprintf(file, "%s", "}\n");
		}

		if(destructors)
		{
			fprintf(file, "%s%s%s", "void ",NameSpacePP, "DeleteAllElements()\n{\n");

				for(int i=0;i<SXMainWndElem::CreateElements.size();i++)
				{
					fprintf(file, "%s%s%s%s%s", "\t", "mem_delete(",NameSpacePP,SXMainWndElem::CreateElements[i]->Name, ");\n");
				}
				fprintf(file, "%s%s%s%s%s", "\t", "mem_delete(", NameSpacePP, SXMainWndElem::NameJobWnd, ");\n");

			fprintf(file, "%s", "}\n");
		}

	fclose(file);
}