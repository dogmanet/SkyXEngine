
#include <SXGUIWinApi\SXGUI_img_button.h>

#pragma once

SXGUIButtonImg::SXGUIButtonImg()
{

}

SXGUIButtonImg::SXGUIButtonImg(const char* path,WORD x,WORD y,WORD width,WORD heigth,DWORD alpha_color,DWORD bk_color,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"SXGUIBUTTONIMG",
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler != 0 ? handler : WndProcAllDefault));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();

	/*this->AddHandler(WndProcButtonImgDefault,WM_MOUSEMOVE,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_KILLFOCUS,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_NCMOUSEMOVE,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_LBUTTONDOWN,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_LBUTTONUP,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_PAINT,0,0,0,0,true);*/

	AlphaColor = alpha_color;
	BkColor = bk_color;
	
		if(path[0] != 0)
		{
			this->IsImages = true;
			HBitMap = LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			GetObject(HBitMap, sizeof(BITMAP), &Bitmap);
		}
		else
		{
			this->IsImages = false;
		}

	this->Pos = SXGUI_BI_POS_FREE;
	this->EnableState = false;
	this->EnableBf = false;
	this->FrameColor = RGB(255,255,255);

	sprintf(this->PathForImg,"%s",path);
}

SXGUIButtonImg::SXGUIButtonImg(WORD button,WORD x,WORD y,WORD width,WORD heigth,DWORD alpha_color,DWORD bk_color,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"SXGUIBUTTONIMG",
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler != 0 ? handler : WndProcAllDefault));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();

	/*this->AddHandler(WndProcButtonImgDefault,WM_MOUSEMOVE,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_KILLFOCUS,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_NCMOUSEMOVE,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_LBUTTONDOWN,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_LBUTTONUP,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_PAINT,0,0,0,0,true);*/

	AlphaColor = alpha_color;
	BkColor = bk_color;
	
	HBitMap = LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(button));
	this->IsImages = true;

	GetObject(HBitMap, sizeof(BITMAP), &Bitmap);

	this->Pos = SXGUI_BI_POS_FREE;
	this->EnableState = false;
	this->EnableBf = false;
	this->FrameColor = RGB(255,255,255);
}

void SXGUIButtonImg::ReInitImage(const char* path)
{
	DeleteObject(HBitMap);
	HBitMap = LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	this->IsImages = true;

	GetObject(HBitMap, sizeof(BITMAP), &Bitmap);
	sprintf(this->PathForImg,"%s",path);
}

void SXGUIButtonImg::InitCallBack()
{
	this->AddHandler(WndProcButtonImgDefault,WM_MOUSEMOVE,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_KILLFOCUS,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_NCMOUSEMOVE,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_LBUTTONDOWN,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_LBUTTONUP,0,0,0,0,true);
	this->AddHandler(WndProcButtonImgDefault,WM_PAINT,0,0,0,0,true);
}

SXGUIButtonImg::~SXGUIButtonImg()
{
	DeleteObject(HBitMap);
}

void SXGUIButtonImg::GetPathForImg(char* buf)
{
	sprintf(buf,"%s",this->PathForImg);
}

LRESULT WndProcButtonImgDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//SXGUIButtonImg *Button = (SXGUIButtonImg *)GetWindowLong(hwnd,GWL_USERDATA);
	ISXGUIComponent *Component = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	ISXGUIButtonImg *Button = dynamic_cast<ISXGUIButtonImg*>(Component);
	switch(msg)
	{
		case WM_MOUSEMOVE:
			{
				SetCapture(hwnd);
				POINT global_pos_cursor;
				GetCursorPos(&global_pos_cursor);
				RECT* grect = Button->GetWinRect();
					if(global_pos_cursor.x >= grect->left && global_pos_cursor.x <= grect->right && global_pos_cursor.y >= grect->top && global_pos_cursor.y <= grect->bottom)
					{
							if(Button->Pos != SXGUI_BI_POS_MOUSE_CLICK && !(Button->EnableBf && Button->EnableState))
							{
								Button->Pos = SXGUI_BI_POS_MOUSE_MOVE;
								Button->PosBf[0] = false;
								Button->PosBf[1] = true;
								Button->PosBf[2] = false;
								InvalidateRect(hwnd,0,TRUE);
							}
					}
					else
					{
							if(Button->Pos != SXGUI_BI_POS_FREE)
							{
								Button->Pos = SXGUI_BI_POS_FREE;
								Button->PosBf[0] = true;
								Button->PosBf[1] = false;
								Button->PosBf[2] = false;
								InvalidateRect(hwnd,0,TRUE);
							}
						ReleaseCapture();
					}
			}
			break;

		case WM_KILLFOCUS:
			{
				if(Button->EnableGroup && lParam == -1)
				{
					Button->Pos = SXGUI_BI_POS_FREE;
					Button->PosBf[0] = true;
					Button->PosBf[1] = false;
					Button->PosBf[2] = false;
					InvalidateRect(hwnd,0,TRUE);
					Button->EnableState = false;
				}
			}
			break;

		case WM_NCMOUSEMOVE:
				if(Button->Pos != SXGUI_BI_POS_FREE)
				{
					Button->Pos = SXGUI_BI_POS_FREE;
					Button->PosBf[0] = true;
					Button->PosBf[1] = false;
					Button->PosBf[2] = false;
					InvalidateRect(hwnd,0,TRUE);
				}
			break;

		case WM_LBUTTONDOWN:
				if(!(Button->EnableBf && Button->EnableState))
				{
					Button->Pos = SXGUI_BI_POS_MOUSE_CLICK;
					Button->PosBf[0] = false;
					Button->PosBf[1] = false;
					Button->PosBf[2] = true;
					InvalidateRect(hwnd,0,TRUE);
				}
			break;

		case WM_LBUTTONUP:
			{
					//если кнопка состоит в группе
					//если кнопка была неактивна значит она сейчас станет активной, посылаем соощение о дезактивации остальных кнопок
					if(!Button->EnableState && Button->EnableGroup)
						EnumChildWindows(Button->Parent(),(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcUpdateImgButton,-1);
				POINT p;
				GetCursorPos(&p);
				RECT rect;
				GetWindowRect(hwnd,&rect);

					if((p.x >= rect.left && p.x <= rect.right) && (p.y >= rect.top && p.y <= rect.bottom))
					{
						Button->Pos = SXGUI_BI_POS_MOUSE_MOVE;
						Button->PosBf[0] = false;
						Button->PosBf[1] = true;
						Button->PosBf[2] = false;
					}
					else
					{
						Button->Pos = SXGUI_BI_POS_FREE;
						Button->PosBf[0] = true;
						Button->PosBf[1] = false;
						Button->PosBf[2] = false;
					}
				Button->EnableState = !Button->EnableState;
				InvalidateRect(hwnd,0,TRUE);
			}
				if(Button->EnableState)
					Button->SetFocus();
			
			SendMessage(Button->Parent(),WM_COMMAND,MAKEWPARAM(GetWindowLong(hwnd,GWL_ID),0),(LPARAM)hwnd);
			break;

		case WM_PAINT:
			{
				RECT rect;
				GetClientRect(hwnd,&rect);

				PAINTSTRUCT PaintStruct;
				HDC hDC = BeginPaint(hwnd, &PaintStruct);
				HBRUSH color_rect_white = CreateSolidBrush(Button->FrameColor);

				HDC hCompatibleDC = CreateCompatibleDC(hDC);

				HBRUSH color_bk = CreateSolidBrush(Button->BkColor);
				FillRect(hDC,&rect,color_bk);

				HGDIOBJ hOldBitmap;
					if(Button->IsImages)
					{
						hOldBitmap = SelectObject(hCompatibleDC, Button->HBitMap);
						TransparentBlt(hDC, 0, 0, rect.right, rect.bottom, hCompatibleDC, 0, 0, Button->Bitmap.bmWidth, Button->Bitmap.bmHeight, Button->AlphaColor);
					}
					else
					{
						
					}
				RECT atc;

					if(Button->Pos == SXGUI_BI_POS_FREE && Button->PosBf[0])
					{
							if(Button->EnableState && Button->EnableBf)
							{
								atc.left = rect.left;
								atc.top = rect.top;
								atc.right = rect.left + SXGUI_BI_SIZE_LINE;
								atc.bottom = rect.bottom;

								FillRect(hDC,&atc,color_rect_white);

								atc.left = rect.left;
								atc.top = rect.top;
								atc.right = rect.right;
								atc.bottom = rect.top + SXGUI_BI_SIZE_LINE;

								FillRect(hDC,&atc,color_rect_white);

								atc.left = rect.right - SXGUI_BI_SIZE_LINE;
								atc.top = rect.top;
								atc.right = rect.right;
								atc.bottom = rect.bottom;

								FillRect(hDC,&atc,color_rect_white);

								atc.left = rect.left;
								atc.top = rect.bottom - SXGUI_BI_SIZE_LINE;
								atc.right = rect.right;
								atc.bottom = rect.bottom;

								FillRect(hDC,&atc,color_rect_white);
							}
						//MessageBox(0,ToPointChar(ToString(Button->Pos) + "|" + ToString(Button->PosBf[0])),"SXGUI_BI_POS_FREE",0);
						Button->PosBf[0] = false;
					}
					else if(Button->Pos == SXGUI_BI_POS_MOUSE_MOVE && Button->PosBf[1])
					{
						atc.left = rect.left;
						atc.top = rect.top;
						atc.right = rect.left + SXGUI_BI_SIZE_LINE;
						atc.bottom = rect.bottom;

						FillRect(hDC,&atc,color_rect_white);

						atc.left = rect.left;
						atc.top = rect.top;
						atc.right = rect.right;
						atc.bottom = rect.top + SXGUI_BI_SIZE_LINE;

						FillRect(hDC,&atc,color_rect_white);

						Button->PosBf[1] = false;
						//MessageBox(0,ToPointChar(ToString(Button->Pos) + "|" + ToString(Button->PosBf[1])),"SXGUI_BI_POS_MOUSE_MOVE",0);
					}
					else if(Button->Pos == SXGUI_BI_POS_MOUSE_CLICK && Button->PosBf[2])
					{
						atc.left = rect.right - SXGUI_BI_SIZE_LINE;
						atc.top = rect.top;
						atc.right = rect.right;
						atc.bottom = rect.bottom;

						FillRect(hDC,&atc,color_rect_white);

						atc.left = rect.left;
						atc.top = rect.bottom - SXGUI_BI_SIZE_LINE;
						atc.right = rect.right;
						atc.bottom = rect.bottom;

						FillRect(hDC,&atc,color_rect_white);
						Button->PosBf[2] = false;

						//MessageBox(0,ToPointChar(ToString(Button->Pos) + "|" + ToString(Button->PosBf[2])),"SXGUI_BI_POS_MOUSE_CLICK",0);
					}

					if(Button->IsImages)
					{
						SelectObject(hCompatibleDC, hOldBitmap);
					}
				//DeleteObject(hBitmap);
				DeleteDC(hCompatibleDC);
				DeleteObject(color_bk);
				DeleteObject(color_rect_white);
				EndPaint(hwnd, &PaintStruct);
				//return 0;
				//MessageBox(0,"button paint",0,0);
			}
		//break;
	}
	return CallWindowProc(Button->OldProc, hwnd, msg, wParam, lParam);
}