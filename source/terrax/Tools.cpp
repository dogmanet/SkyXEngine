
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "Tools.h"

#include <windows.h>
#include "resource.h"

struct PromptPrm
{
	char * pOut;
	int * pMaxlen;
	const char * szText; 
	const char * szTitle;
	const char * szDefault;
};

INT_PTR CALLBACK DlgPromptProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	PromptPrm * prm = (PromptPrm*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			prm = (PromptPrm*)lParam;
			HWND hEdit = GetDlgItem(hwndDlg, IDC_PROMPT);
			SetDlgItemTextA(hwndDlg, IDC_PROMPT_TEXT, prm->szText);
			SetWindowTextA(hEdit, prm->szDefault);
			SetWindowTextA(hwndDlg, prm->szTitle);
			SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam);
			//SetWindowLongPtr(hEdit, GWLP_USERDATA, SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)InpSelAllWndProc));
			SetFocus(hEdit);
			return(0);
		}
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return(1);

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			*prm->pMaxlen = GetDlgItemTextA(hwndDlg, IDC_PROMPT, prm->pOut, *prm->pMaxlen);
			EndDialog(hwndDlg, 1);
			return(1);
		case IDCANCEL:
			EndDialog(hwndDlg, 0);
			return(1);
		/*case IDC_EDIT1:
			if(HIWORD(wParam) == WM_KEYDOWN)
			{
				OutputDebugStringA("WM_KEYDOWN\n");
			}*/
		}
		break;
	}
	return(0);
}

int Tools::DlgPrompt(char * pOut, int * pMaxlen, const char * szText, const char * szTitle, const char * szDefault)
{
	PromptPrm prm;
	prm.pOut = pOut;
	prm.pMaxlen = pMaxlen;
	prm.szText = szText;
	prm.szTitle = szTitle;
	prm.szDefault = szDefault;
	return(DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_PROPT), GetForegroundWindow(), DlgPromptProc, (LPARAM)&prm));
}

int Tools::DlgConfirm(const char * szText, const char * szTitle, int def)
{
	return(MessageBoxA(NULL, szText, szTitle, MB_OKCANCEL | MB_ICONQUESTION | (def ? MB_DEFBUTTON1 : MB_DEFBUTTON2)) == IDOK ? 1 : 0);
}
