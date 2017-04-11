
#include <SXGUIWinApi\SXGUI_toolbar_sx.h>

#pragma once

SXGUIToolBarSX::SXGUIToolBarSX()
{

}

SXGUIToolBarSX::SXGUIToolBarSX(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,const char* path_button_ctrl,DWORD id_ctrl_button)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"SXGUITOOLBAR",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler != 0 ? handler : WndProcToolBarDefault));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();

	Caption = caption;

	::GetClientRect(this->GetHWND(),&MaxRect);

	MinRect.left = MaxRect.left;
	MinRect.right = MaxRect.right;
	MinRect.top = MaxRect.top;
	MinRect.bottom = MinRect.top + 16;

	RECT rect;
	::GetClientRect(this->GetHWND(),&rect);
	ImgButton = new SXGUIButtonImg(path_button_ctrl,(rect.right /*+ x*/) - 12,/*y+*/4,8,8,RGB(255,255,255),RGB(128,128,128),/*this->ParentHandle*/this->GetHWND(),0,id_ctrl_button);
	ImgButton->EnableBf = true;

	ColorTop = RGB(128,128,128);
	ColorClient = RGB(169,169,169);
	
	CountChildToolBar = 0;
		for(WORD i=0;i<1024;i++)
		{
			ArrChildToolBar[i] = 0;
		}
}

SXGUIToolBarSX::SXGUIToolBarSX(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,WORD button_res,DWORD id_ctrl_button)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"SXGUITOOLBAR",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler != 0 ? handler : WndProcToolBarDefault));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();

	Caption = caption;

	::GetClientRect(this->GetHWND(),&MaxRect);

	MinRect.left = MaxRect.left;
	MinRect.right = MaxRect.right;
	MinRect.top = MaxRect.top;
	MinRect.bottom = MinRect.top + 16;

	RECT rect;
	::GetClientRect(this->GetHWND(),&rect);
	ImgButton = new SXGUIButtonImg(button_res,rect.right - 12,4,8,8,RGB(255,255,255),RGB(128,128,128),/*this->ParentHandle*/this->GetHWND(),0,id_ctrl_button);
	ImgButton->EnableBf = true;

	ImgButton->GAlign.left = false;
	ImgButton->GAlign.right = true;
	ImgButton->GAlign.top = false;
	ImgButton->GAlign.bottom = false;

	ColorTop = RGB(128,128,128);
	ColorClient = RGB(169,169,169);

	CountChildToolBar = 0;
		for(WORD i=0;i<1024;i++)
		{
			ArrChildToolBar[i] = 0;
		}
}

SXGUIToolBarSX::~SXGUIToolBarSX()
{
	mem_delete(ImgButton);
}


BOOL CALLBACK EnumChildComCountToolBar(HWND hwnd,LPARAM lParam)
{
	SXGUIToolBarSX* ParentToolBar = (SXGUIToolBarSX *)GetWindowLong((HWND)lParam,GWL_USERDATA);
	char ClassName[256];

		if(GetClassName(hwnd,ClassName,256))
		{
				if(strcmp(ClassName,"SXGUITOOLBAR") == 0)
				{
					SXGUIToolBarSX *ToolBar = (SXGUIToolBarSX *)GetWindowLong(hwnd,GWL_USERDATA);
					ParentToolBar->ArrChildToolBar[ParentToolBar->CountChildToolBar] = ToolBar;
					ParentToolBar->CountChildToolBar++;
				}
		}
		
	return TRUE;
}

LRESULT CALLBACK WndProcToolBarDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//SXGUIToolBarSX *ToolBar = (SXGUIToolBarSX *)GetWindowLong(hwnd,GWL_USERDATA);
	ISXGUIComponent *Component = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	SXGUIToolBarSX *ToolBar = dynamic_cast<SXGUIToolBarSX*>(Component);
		switch(msg)
		{
			case WM_MOUSEMOVE:
				{
					POINT p;
					GetCursorPos(&p);
					RECT rect;
					GetWindowRect(ToolBar->ImgButton->GetHWND(),&rect);

						if(((p.x >= rect.left && p.x <= rect.right) && (p.y >= rect.top && p.y <= rect.bottom)))
						{
							SendMessage(ToolBar->ImgButton->GetHWND(),WM_MOUSEMOVE,wParam,lParam);
						}
						else
						{
							SendMessage(ToolBar->ImgButton->GetHWND(),WM_NCMOUSEMOVE,wParam,lParam);
						}

						EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcMouseMove,lParam);
				}

					for(int i=0;i<ToolBar->CountChildToolBar;i++)
					{
						RECT *rect_first_tool_bar = ToolBar->ArrChildToolBar[i]->GetClientRect();

						int k=i+1;
							if(ToolBar->ArrChildToolBar[k])
							{
								int count_tab_pix = 0;
								RECT *rect_second_tool_bar = ToolBar->ArrChildToolBar[k]->GetClientRect();

									//if(rect_first_tool_bar->top <= rect_second_tool_bar->top && rect_first_tool_bar->bottom >= rect_second_tool_bar->bottom)
									//{
										count_tab_pix = (rect_first_tool_bar->bottom - rect_second_tool_bar->top) + 5;
									//}
									//else
									//{	
										//count_tab_pix = (rect_first_tool_bar->bottom - rect_second_tool_bar->top) + 25;
									//}
								rect_second_tool_bar->top += count_tab_pix;
								rect_second_tool_bar->bottom += count_tab_pix;
										
								ToolBar->ArrChildToolBar[k]->SetClientRect(rect_second_tool_bar,false);
							}
						::GetWindowRect(ToolBar->ArrChildToolBar[i]->Parent(),&(ToolBar->ArrChildToolBar[i]->ParentRect));
						::GetWindowRect(ToolBar->ArrChildToolBar[i]->ImgButton->Parent(),&(ToolBar->ArrChildToolBar[i]->ImgButton->ParentRect));
					}

					
					
				/*ToolBar->CountChildToolBar = 0;
				EnumChildWindows(hwnd,(WNDENUMPROC)&EnumChildComCountToolBar,LPARAM(hwnd));*/
				//MessageBox(0,ToPointChar(ToolBar->CountChildToolBar),0,0);
				break;

			case WM_NCMOUSEMOVE:
					for(int i=0;i<ToolBar->CountChildToolBar;i++)
					{
						RECT *rect_first_tool_bar = ToolBar->ArrChildToolBar[i]->GetClientRect();

						int k=i+1;
							if(ToolBar->ArrChildToolBar[k])
							{
								int count_tab_pix = 0;
								RECT *rect_second_tool_bar = ToolBar->ArrChildToolBar[k]->GetClientRect();

									//if(rect_first_tool_bar->top <= rect_second_tool_bar->top && rect_first_tool_bar->bottom >= rect_second_tool_bar->bottom)
									//{
										count_tab_pix = (rect_first_tool_bar->bottom - rect_second_tool_bar->top) + 5;
									//}
									//else
									//{	
										//count_tab_pix = (rect_first_tool_bar->bottom - rect_second_tool_bar->top) + 25;
									//}
								rect_second_tool_bar->top += count_tab_pix;
								rect_second_tool_bar->bottom += count_tab_pix;
										
								ToolBar->ArrChildToolBar[k]->SetClientRect(rect_second_tool_bar,false);
							}
						::GetWindowRect(ToolBar->ArrChildToolBar[i]->Parent(),&(ToolBar->ArrChildToolBar[i]->ParentRect));
						::GetWindowRect(ToolBar->ArrChildToolBar[i]->ImgButton->Parent(),&(ToolBar->ArrChildToolBar[i]->ImgButton->ParentRect));
					}
			break;

			case WM_CTLCOLORSTATIC:
				{
					SXGUIComponent *Component = (SXGUIComponent *)GetWindowLong((HWND)lParam,GWL_USERDATA);

						if(Component)
						{
							//MessageBox(0,SXGUIFuctinon::GetText(Component),0,0);
								if(Component->GetTransparentTextBk())
									SetBkMode((HDC)wParam,TRANSPARENT);
								else
									SetBkColor((HDC)wParam, Component->GetColorTextBk());
							SetTextColor((HDC)wParam, Component->GetColorTextBk());
			
							return (long)Component->GetBrush();
						}
				}
				break;

			case WM_COMMAND:
					if(LOWORD(wParam) == GetWindowLong(ToolBar->ImgButton->GetHWND(),GWL_ID))
					{
							if(ToolBar->ImgButton->EnableState)
							{
								RECT *client_rect = ToolBar->GetClientRect();

								//ToolBar->MaxRect.bottom = client_rect->bottom;

								client_rect->right = client_rect->left + (ToolBar->MinRect.right);
								client_rect->bottom = client_rect->top + (ToolBar->MinRect.bottom);
								ToolBar->SetClientRect(client_rect,false);
								//MessageBox(0,ToPointChar(client_rect->bottom),"max",0);
							}
							else
							{
								RECT *client_rect = ToolBar->GetClientRect();
								client_rect->right = client_rect->left + (ToolBar->MaxRect.right);
								client_rect->bottom = client_rect->top + (ToolBar->MaxRect.bottom);
								ToolBar->SetClientRect(client_rect,false);
								//MessageBox(0,ToPointChar(client_rect->bottom),"min",0);
							}
					}
				
				break;

			case WM_PAINT:
				{
					
					RECT rect;
					HBRUSH brush_top_rect;
					HBRUSH brush_client_rect;
					GetClientRect(hwnd,&rect);

					PAINTSTRUCT PaintStruct;
					HDC hDC = BeginPaint(hwnd, &PaintStruct);

					RECT top_rect;
					top_rect.left = rect.left;
					top_rect.top = rect.top;
					top_rect.right = rect.right;
					top_rect.bottom = rect.top + 15;

					brush_top_rect = CreateSolidBrush(ToolBar->ColorTop);
					FillRect(hDC,&top_rect,brush_top_rect);


					RECT client_rect;
					client_rect.left = rect.left;
					client_rect.top = rect.top + 15;
					client_rect.right = rect.right;
					client_rect.bottom = rect.bottom;

					brush_client_rect = CreateSolidBrush(ToolBar->ColorClient);
					FillRect(hDC,&client_rect,brush_client_rect);

					SetBkMode(hDC,TRANSPARENT);
					SetTextColor(hDC,RGB(200,200,200));
					SelectObject (hDC, ToolBar->GetFont());
					DrawText (hDC, ToolBar->Caption, -1, &top_rect, DT_CENTER);

					EndPaint(hwnd, &PaintStruct);

					DeleteObject(brush_top_rect);
					DeleteObject(brush_client_rect);
				}
			break;
		}
	
	return ToolBar->OldProc(hwnd, msg, wParam, lParam);
}