#include "Editor.h"

#include <ShlObj.h>


Editor::Editor():
m_bCamMove(false),
m_iCurIdx(-1)
{
	InitUI();
	InitD3D();

	m_pvActivities = &((TabActivities*)m_pTM->m_pTabActivities)->m_vItems;

	m_pEditor = this;
	m_szAnimFilter[0] = 0;

	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlGB->AddHandler(AnimTBProc, WM_HSCROLL);
	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgressTrack->AddHandler(AnimTBProc, WM_SETFOCUS);
	//((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgress->AddHandler(AnimTBProc, WM_SETFOCUS);
	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgressTrack->AddHandler(AnimTBProc, WM_KILLFOCUS);
	//((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgress->AddHandler(AnimTBProc, WM_KILLFOCUS);


	/*m_szGamesourceDir[0] = 0;
	if(!GetRegGSdir())
	{
		
	}*/
	GetModuleFileNameA(NULL, m_szGamesourceDir, sizeof(m_szGamesourceDir));
	canonize_path(m_szGamesourceDir);
	dirname(m_szGamesourceDir);
	strcat(m_szGamesourceDir, "gamesource/");
	SetCurrentDirectoryA(m_szGamesourceDir);


	m_pAnimMgr = new AnimationManager(m_pd3dDevice);
	m_pCurAnim = new Animation(m_pAnimMgr);
	m_pCurAnim->SetCallback(AnimPlayCB);
	m_pCurAnim->SetProgressCB(AnimProgressCB);
	//m_pCurAnim->SetModel("C:/DSe/SX/project/gamesource/models/krovosos/krovososa.dse");
	//m_pCurAnim->SetModel("C:/DSe/SX/project/gamesource/models/spas/spasa.dse");
	//m_pCurAnim->SetModel("C:/DSe/SX/project/gamesource/models/pm/pma.dse");
	//m_pCurAnim->SetModel("C:/DSe/SX/project/gamesource/models/ak74/ak74a.dse");
	//m_pCurAnim->PlayAnimation("reload", 0);

	//AddModel("C:/revo/build/gamesource/models/krovosos/krovososa.dse");//ak74_ref.dse
	//AddModel("C:/revo/build/gamesource/models/ak74/ak74_ref.dse");
	//AddModel("C:/revo/build/gamesource/models/ak74/hands.dse");
	//AddModel("C:/revo/build/gamesource/models/ak74/reload.dse");

	/*ModelPart mp;
	mp.attachDesc.type = MA_BONE;
	strcpy(mp.attachDesc.szBone, "bip01_r_hand");
	mp.uImportFlags = MI_ALL;
	mp.pMdl = m_pAnimMgr->LoadModel("C:/revo/build/gamesource/models/ak74/ak74.dse", true);
	m_pCurAnim->AddModel(&mp);*/

	//bip01_r_hand
	AddModel("F:/revo/build/gamesource/models/ak74/ak74.dse");
	AddModel("F:/revo/build/gamesource/models/ak74/idle.dse");
	//AddModel("C:/revo/build/gamesource/models/ak74/reload.dse");
	//AddModel("C:/revo/build/gamesource/models/ak74/shoot.dse");
	m_pCurAnim->Assembly();
	if(m_pCurAnim->m_pMdl)
	{
		/*UINT c = m_pCurAnim->m_pMdl->GetSequenceCount();
		AnimItem ai;
		for(UINT i = 0; i < c; ++i)
		{
			//m_vAnims
			ai.seq = (ModelSequence*)m_pCurAnim->m_pMdl->GetSequence(i);
			//	((ModelSequence*)ai.seq)->bLooped = true;
			if(!(i % 2))
			{
				ai.seq->framerate *= -1;
			}
			ai.mdl = m_pCurAnim->m_pMdl;
			ai.isImported = false;
			m_vAnims.push_back(ai);
		}*/
		RenderAnimList();
		RenderBoneList();
	}
}
bool Editor::GetRegGSdir()
{
	HKEY hKey;
	LONG lRes = RegOpenKeyExA(HKEY_CURRENT_USER, EDITOR_REG_TREE, 0, KEY_READ, &hKey);
	if(lRes == ERROR_SUCCESS)
	{
		DWORD dwBufferSize = sizeof(m_szGamesourceDir);
		ULONG nError = RegQueryValueExA(hKey, EDITOR_REG_KEY_GSDIR, 0, NULL, (LPBYTE)m_szGamesourceDir, &dwBufferSize);
		if(ERROR_SUCCESS == nError)
		{
			
		}
	}

	RegCloseKey(hKey);
	return(m_szGamesourceDir[0] != 0);
}

void Editor::SetRegGSdir()
{
	HKEY hKey;
	LONG lRes = RegCreateKeyExA(HKEY_CURRENT_USER, EDITOR_REG_TREE, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hKey, NULL);
	if(lRes == ERROR_SUCCESS)
	{
		DWORD dwBufferSize = strlen(m_szGamesourceDir);
		ULONG nError = RegSetValueExA(hKey, EDITOR_REG_KEY_GSDIR, 0, REG_SZ, (LPBYTE)m_szGamesourceDir, dwBufferSize);
		if(ERROR_SUCCESS == nError)
		{

		}
	}

	RegCloseKey(hKey);
}

Editor::~Editor()
{
	mem_del(m_pCurAnim);
	mem_del(m_pAnimMgr);

	DestroyD3D();
	DestroyUI();
}

LRESULT Editor::MenuCmd(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!m_pEditor)
	{
		return(0);
	}
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	Editor * edt = (Editor*)cmp->GetUserPtr();
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_FILE_OPEN:
			edt->MenuBrowse(hwnd);
			break;
		case ID_FILE_SAVE:
			edt->MenuSave();
			break;
		case ID_FILE_SAVEAS:
			edt->MenuSaveAs(hwnd);
			break;

		case ID_FILE_IMPORT:
			edt->MenuBrowseImport(hwnd);
			break;

		case ID_FILE_EXIT:
			if(edt->m_bDirty && MessageBoxA(hwnd, "All unsaved changes will be lost", "Exit?", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
			{
				return(0);
			}
			PostQuitMessage(0);
			break;

		case IDC_ANIM_PAUSE:
			edt->m_pCurAnim->Stop();
			break;

		case IDC_ANIM_PLAY:
			edt->m_pCurAnim->Resume();
			break;

		case IDC_ANIM_LOOPED:
			if(m_pEditor->m_iCurIdx >= 0)
			{
				byte lp = SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) > 0 ? 1 : 0;
				m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->bLooped = lp;
				((ModelSequence*)m_pEditor->m_pCurAnim->m_pMdl->GetSequence(m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->name))->bLooped = lp;
				m_pEditor->RenderAnimList();
			}
			break;

		case IDC_ANIM_SPEED:
			switch(HIWORD(wParam))
			{
			case EN_CHANGE:
				if(m_pEditor->m_iCurIdx >= 0)
				{
					int speed = 0;
					int len = GetWindowTextLengthA((HWND)lParam);
					char * txt = (char*)alloca(len + 1);
					GetWindowTextA((HWND)lParam, txt, len + 1);
					sscanf(txt, "%d", &speed);

					m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->framerate = speed;
					((ModelSequence*)m_pEditor->m_pCurAnim->m_pMdl->GetSequence(m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->name))->framerate = speed;

				}
			}
			break;

		case IDC_ANIM_NAME:
			if(HIWORD(wParam) == EN_CHANGE && m_pEditor->m_iCurIdx >= 0)
			{
				ModelSequence * ms = ((ModelSequence*)m_pEditor->m_pCurAnim->m_pMdl->GetSequence(m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->name));
				GetWindowTextA((HWND)lParam, m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->name, MODEL_MAX_NAME);
				strcpy(ms->name, m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->name);
				m_pEditor->RenderAnimList();
				m_pEditor->m_pCurAnim->SyncAnims();
			}
			break;

		case IDC_ANIM_ACT_CNANCE:
			if(HIWORD(wParam) == EN_CHANGE && m_pEditor->m_iCurIdx >= 0)
			{
				int len = GetWindowTextLengthA((HWND)lParam);
				char * txt = (char*)alloca(len + 1);
				GetWindowTextA((HWND)lParam, txt, len + 1);
				int i;
				if(sscanf(txt, "%d", &i) < 1)
				{
					SetWindowTextA((HWND)lParam, "0");
				}
				else
				{
					m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->act_chance = i;
					((ModelSequence*)m_pEditor->m_pCurAnim->m_pMdl->GetSequence(m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->name))->act_chance = i;
				}
			}
			break;

		case IDC_ATTACH_RB_BONE:
			{
				TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
				tab->AttachBone->Enable(1);
			}
			break;

		case IDC_ATTACH_RB_SKIN:
			{
				TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
				tab->AttachBone->Enable(0);
			}
			break;

		case IDC_PT_REN:
			{
				ISXGUIListBox * pList = ((TabAttachments*)edt->m_pTM->m_pTabAttachments)->AttachmentsList;
				int len = MODEL_MAX_NAME;
				int sel = pList->GetSel();
				if(sel < 0)
				{
					break;
				}
				sel = pList->GetItemData(sel);
				
				Tools::DlgPrompt(edt->m_vMdlParts[sel]->name, &len, "New name", "Rename", edt->m_vMdlParts[sel]->name);
				edt->RenderPartList();
			}
			break;

		case IDC_PT_DEL:
			{
				ISXGUIListBox * pList = ((TabAttachments*)edt->m_pTM->m_pTabAttachments)->AttachmentsList;
				int len = MODEL_MAX_NAME;
				int sel = pList->GetSel();
				if(sel < 0)
				{
					break;
				}
				sel = pList->GetItemData(sel);

				if(Tools::DlgConfirm())
				{
					edt->DelModel(sel);
				}
			}
			break;

		case IDC_PT_ADD:
			{
				m_pEditor->AddModel("");
				ISXGUIListBox * pList = ((TabAttachments*)edt->m_pTM->m_pTabAttachments)->AttachmentsList;
				pList->SetSel(pList->GetCountItem() - 1);
				m_pEditor->OnPartListSelChg();
			}
			break;

		case IDC_PT_LB:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				edt->OnPartListSelChg();
				break;
			}
			break;

		case IDC_PT_CB_HIDDEN:
			edt->SetPartFlag(MP_HIDDEN, (byte)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0));
			edt->m_pCurAnim->Assembly();
			break;

		case IDC_PT_CB_COLLIDE:
			edt->SetPartFlag(MP_COLLISIONS, (byte)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0));
			break;

		case IDC_PT_CB_RAYTRACE:
			edt->SetPartFlag(MP_RAYTRACE, (byte)SendMessage((HWND)lParam, BM_GETCHECK, 0, 0));
			break;

		case IDC_PT_BROWSE:
			edt->MenuBrowseImport(hwnd, false);
			break;

		case IDC_PT_APPLY:
			edt->OnPartApply();
			break;

		}
		break;

	case WM_CLOSE:
		if(m_pEditor->m_bDirty && MessageBoxA(hwnd, "All unsaved changes will be lost", "Quit?", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
		{
			return(0);
		}
		return(DefWindowProc(hwnd, msg, wParam, lParam));

	case EM_LOADACTIVITIES:
		TabAnimation * tAnim = (TabAnimation*)edt->m_pTM->m_pTabAnimation;
		tAnim->AnimPropActCmb->Clear();
		for(int i = 0, l = edt->m_pvActivities->size(); i < l; ++i)
		{
			tAnim->AnimPropActCmb->AddItem(edt->m_pvActivities[0][i].act.c_str());
		}
		break;
	}
	return(0);
}

LRESULT Editor::CamInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	Editor * edt = (Editor*)cmp->GetUserPtr();
	switch(msg)
	{
	case WM_RBUTTONDOWN:
		edt->CenterMouse();
		edt->m_bCamMove = true;
		ShowCursor(0);
		SetFocus(edt->MainWindow->GetHWND());
		break;
	case WM_RBUTTONUP:
		edt->m_bCamMove = false;
		ShowCursor(1);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'W':
			edt->m_cam.Move(Camera::CAMERA_MOVE_FORWARD, 1);
			break;
		case 'S':
			edt->m_cam.Move(Camera::CAMERA_MOVE_BACKWARD, 1);
			break;
		case 'A':
			edt->m_cam.Move(Camera::CAMERA_MOVE_LEFT, 1);
			break;
		case 'D':
			edt->m_cam.Move(Camera::CAMERA_MOVE_RIGHT, 1);
			break;
		}
		break;
	case WM_KEYUP:
		switch(wParam)
		{
		case 'W':
			edt->m_cam.Move(Camera::CAMERA_MOVE_FORWARD, 0);
			break;
		case 'S':
			edt->m_cam.Move(Camera::CAMERA_MOVE_BACKWARD, 0);
			break;
		case 'A':
			edt->m_cam.Move(Camera::CAMERA_MOVE_LEFT, 0);
			break;
		case 'D':
			edt->m_cam.Move(Camera::CAMERA_MOVE_RIGHT, 0);
			break;
		}
		break;
		
	}
	return(0);
}

LRESULT Editor::AnimListCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	Editor * edt = (Editor*)cmp->GetUserPtr();
	ISXGUIListBox * lb = edt->AnimList;
	int curSel = lb->GetSel();
	switch(msg)
	{
	case WM_LBUTTONDBLCLK:
		if(curSel != -1)
		{
			UINT seqi = (UINT)lb->GetItemData(curSel);
			edt->m_pCurAnim->Play(edt->m_vAnims[seqi].seq->name, 100);
		}
		break;
	}

	return(CallWindowProc(cmp->OldProc, hwnd, msg, wParam, lParam));
}

LRESULT Editor::AnimFilterCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYUP:
		m_pEditor->AnimFilter->GetText(m_pEditor->m_szAnimFilter, sizeof(m_pEditor->m_szAnimFilter) - 1);
		m_pEditor->RenderAnimList();
		break;
	}
	return(0);
}

void Editor::AnimPlayCB(int slot, ANIM_STATE state, Animation * pAnim)
{
	ModelSequence const * seq = m_pEditor->m_pCurAnim->GetCurAnim(slot);
	TabAnimation * ta = (TabAnimation*)m_pEditor->m_pTM->m_pTabAnimation;
	switch(state)
	{
	case AS_STOP:
		m_pEditor->CurAnimName->SetText("");
		ta->AnimCtlPauseBtn->Enable(0);
		ta->AnimCtlPlayBtn->Enable(1);
		break;
	case AS_PLAY:
		char name[MODEL_MAX_NAME + 16];
		sprintf(name, "Animation: %s", seq->name);
		m_pEditor->CurAnimName->SetText(name);

		ta->AnimCtlPauseBtn->Enable(1);
		ta->AnimCtlPlayBtn->Enable(0);
		break;
	}
	
}
void Editor::AnimProgressCB(int slot, float progress, Animation * pAnim)
{
	TabAnimation * ta = (TabAnimation*)m_pEditor->m_pTM->m_pTabAnimation;
	ta->AnimCtlProgress->SetText(String(progress).c_str());

	ta->AnimCtlProgressTrack->SetPos((int)(progress * 1000));
}

LRESULT Editor::AnimTBProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int pos;
	switch(msg)
	{
	case WM_HSCROLL:
		pos = ((TabAnimation*)m_pEditor->m_pTM->m_pTabAnimation)->AnimCtlProgressTrack->GetPos();
		m_pEditor->m_pCurAnim->SetProgress(pos / 1000.0f);
		break;
	case WM_SETFOCUS:
		m_pEditor->m_pCurAnim->SetAdvance(false);
		break;
	case WM_KILLFOCUS:
		m_pEditor->m_pCurAnim->SetAdvance(true);
		break;
	}
	return(0);
}

void Editor::MenuBrowse(HWND hwnd)
{
	OPENFILENAMEA ofn;
	char szFile[260];

	if(m_bDirty && MessageBoxA(hwnd, "All unsaved changes will be lost", "Open?", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
	{
		return;
	}

	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Model file (*.dse)\0*.dse\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	wchar_t bf[256];
	GetCurrentDirectoryW(256, bf);

	if(GetOpenFileNameA(&ofn) == TRUE)
	{
		SetCurrentDirectoryW(bf);

		//wprintf(L"File: %s\n", ofn.lpstrFile);

		//unload all parts
		while(m_pEditor->m_vMdlParts.size())
		{
			m_pEditor->DelModel(0);
		}
		m_pEditor->AddModel(ofn.lpstrFile, MI_ALL, false, true);
		m_pEditor->m_pCurAnim->Assembly();
	}
}
void Editor::MenuBrowseImport(HWND hwnd, bool use)
{
	OPENFILENAMEA ofn;
	char szFile[260];

	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Model file (*.dse)\0*.dse\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	wchar_t bf[256];
	GetCurrentDirectoryW(256, bf);

	if(GetOpenFileNameA(&ofn) == TRUE)
	{
		SetCurrentDirectoryW(bf);
		//wprintf(L"File: %s\n", ofn.lpstrFile);
		UINT iflags = DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_IMPORT), hwnd, DlgImportProc, (LPARAM)&ofn.lpstrFile);
		if(use)
		{
			m_pEditor->AddModel(ofn.lpstrFile, iflags);
			m_pEditor->m_pCurAnim->Assembly();
		}
		else
		{
			TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
			canonize_path(ofn.lpstrFile);
			ofn.lpstrFile = (char*)strip_prefix(ofn.lpstrFile, m_pEditor->m_szGamesourceDir);
			tab->AttachFileField->SetText(ofn.lpstrFile);
			tab->m_iflags = iflags;
		}
	}
}

INT_PTR CALLBACK Editor::DlgImportProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hwndDlg, IDC_CI_ALL, BM_SETCHECK, 1, 0);
		SendDlgItemMessage(hwndDlg, IDC_CI_MESH, BM_SETCHECK, 1, 0);
		SendDlgItemMessage(hwndDlg, IDC_CI_ANIMATIONS, BM_SETCHECK, 1, 0);
		SendDlgItemMessage(hwndDlg, IDC_CI_SKINS, BM_SETCHECK, 1, 0);
		SendDlgItemMessage(hwndDlg, IDC_CI_CONTROLLERS, BM_SETCHECK, 1, 0);
		SendDlgItemMessage(hwndDlg, IDC_CI_HITBOXES, BM_SETCHECK, 1, 0);

		SetWindowLong(hwndDlg, GWL_USERDATA, MI_ALL);
		return(0);
	}
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return(1);

	case WM_COMMAND:
		LONG gwlp = GetWindowLong(hwndDlg, GWL_USERDATA);
		switch(LOWORD(wParam))
		{
		case IDOK:
			//*prm->pMaxlen = GetDlgItemTextA(hwndDlg, IDC_EDIT1, prm->pOut, *prm->pMaxlen);
			EndDialog(hwndDlg, gwlp);
			return(1);
		case IDCANCEL:
			EndDialog(hwndDlg, 0);
			return(1);


		case IDC_CI_MESH:
			DlgImpCheckAll(hwndDlg);
			SetWindowLong(hwndDlg, GWL_USERDATA, SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) ? gwlp | MI_MESH : gwlp & ~MI_MESH);
			break;
		case IDC_CI_ANIMATIONS:
			DlgImpCheckAll(hwndDlg);
			SetWindowLong(hwndDlg, GWL_USERDATA, SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) ? gwlp | MI_ANIMATIONS : gwlp & ~MI_ANIMATIONS);
			break;
		case IDC_CI_SKINS:
			DlgImpCheckAll(hwndDlg);
			SetWindowLong(hwndDlg, GWL_USERDATA, SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) ? gwlp | MI_SKINS : gwlp & ~MI_SKINS);
			break;
		case IDC_CI_CONTROLLERS:
			DlgImpCheckAll(hwndDlg);
			SetWindowLong(hwndDlg, GWL_USERDATA, SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) ? gwlp | MI_CONTROLLERS : gwlp & ~MI_CONTROLLERS);
			break;
		case IDC_CI_HITBOXES:
			DlgImpCheckAll(hwndDlg);
			SetWindowLong(hwndDlg, GWL_USERDATA, SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) ? gwlp | MI_HITBOXES : gwlp & ~MI_HITBOXES);
			break;


		case IDC_CI_ALL:
			byte checked = SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) > 0 ? 1 : 0;
			SendDlgItemMessage(hwndDlg, IDC_CI_MESH, BM_SETCHECK, checked, 0);
			SendDlgItemMessage(hwndDlg, IDC_CI_ANIMATIONS, BM_SETCHECK, checked, 0);
			SendDlgItemMessage(hwndDlg, IDC_CI_SKINS, BM_SETCHECK, checked, 0);
			SendDlgItemMessage(hwndDlg, IDC_CI_CONTROLLERS, BM_SETCHECK, checked, 0);
			SendDlgItemMessage(hwndDlg, IDC_CI_HITBOXES, BM_SETCHECK, checked, 0);
			SetWindowLong(hwndDlg, GWL_USERDATA, checked ? gwlp | MI_ALL : gwlp & ~MI_ALL);
			break;
		}
		break;
	}
	return(0);
}

void Editor::DlgImpCheckAll(HWND hwndDlg)
{
	bool checked = SendDlgItemMessage(hwndDlg, IDC_CI_MESH, BM_GETCHECK, 0, 0)
	&& SendDlgItemMessage(hwndDlg, IDC_CI_ANIMATIONS, BM_GETCHECK, 0, 0)
	&& SendDlgItemMessage(hwndDlg, IDC_CI_SKINS, BM_GETCHECK, 0, 0)
	&& SendDlgItemMessage(hwndDlg, IDC_CI_CONTROLLERS, BM_GETCHECK, 0, 0)
	&& SendDlgItemMessage(hwndDlg, IDC_CI_HITBOXES, BM_GETCHECK, 0, 0);

	SendDlgItemMessage(hwndDlg, IDC_CI_ALL, BM_SETCHECK, checked, 0);
}

void Editor::MenuSave()
{
	m_bDirty = false;
}
void Editor::MenuSaveAs(HWND hwnd)
{
	m_bDirty = false;
}

Editor * Editor::GetInstance()
{
	return(m_pEditor);
}

Editor * Editor::m_pEditor;

void Editor::InitUI()
{
	MainWindow = SXGUICrBaseWnd("MainWindow", "MainWindow", 0, 0, 256, 199, 1320, 730, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(MainWindow);
	MainWindow->AddHandler(MenuCmd, WM_COMMAND);
	MainWindow->AddHandler(MenuCmd, WM_CLOSE, 0, 0, 0, 0, 1);
	MainWindow->AddHandler(MenuCmd, WM_PARENTNOTIFY);
	MainWindow->AddHandler(MenuCmd, EM_LOADACTIVITIES);


	MainWindow->SetUserPtr(this);

	Menu = SXGUICrMenuEx(IDR_MENU1);
	Menu->SetToWindow(MainWindow->GetHWND());

	D3DWindow = SXGUICrBaseWnd("Window1", "Window1", 0, 0, 279, 6, 1023, 473, 0, 0, CreateSolidBrush(RGB(200, 200, 200)), 0, CS_HREDRAW | CS_VREDRAW, WS_CHILD | WS_VISIBLE | WS_BORDER, MainWindow->GetHWND(), 0);
	D3DWindow->GAlign = {true, true, true, true};
	D3DWindow->SetUserPtr(this);

	D3DWindow->AddHandler(CamInput, WM_RBUTTONDOWN);
	D3DWindow->AddHandler(CamInput, WM_RBUTTONUP);
	MainWindow->AddHandler(CamInput, WM_KEYDOWN);
	MainWindow->AddHandler(CamInput, WM_KEYUP);

	m_pTM = new TabManager(MainWindow);

	AnimationsGB = SXGUICrGroupBox("Animations", 3, 0, 275, 669, MainWindow->GetHWND(), 0, 0);
	AnimationsGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimationsGB->SetColorText(0, 0, 0);
	AnimationsGB->SetColorTextBk(255, 255, 255);
	AnimationsGB->SetTransparentTextBk(true);
	AnimationsGB->SetColorBrush(255, 255, 255);
	AnimationsGB->GAlign = {true, true, true, false};
	AnimationsGB->SetUserPtr(this);
	AnimationsGB->AddHandler(AnimGBProc, WM_COMMAND);

	AnimList = SXGUICrListBoxEx("", 2, 43, 270, 623, 0, WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | LBS_NOTIFY/* | LBS_SORT*/, AnimationsGB->GetHWND(), 0, IDC_LISTBOX);
	AnimList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimList->SetColorText(0, 0, 0);
	AnimList->SetColorTextBk(255, 255, 255);
	AnimList->SetTransparentTextBk(true);
	AnimList->SetColorBrush(255, 255, 255);
	AnimList->GAlign = {true, true, true, true};
	//AnimList->SetUserPtr(this);
	//AnimList->AddHandler(AnimListCB, WM_LBUTTONDBLCLK);

	AnimFilter = SXGUICrEdit("", 44, 16, 228, 23, AnimationsGB->GetHWND(), 0, 0);
	AnimFilter->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimFilter->SetColorText(0, 0, 0);
	AnimFilter->SetColorTextBk(255, 255, 255);
	AnimFilter->SetTransparentTextBk(true);
	AnimFilter->SetColorBrush(255, 255, 255);

	AnimFilter->AddHandler(AnimFilterCB, WM_KEYUP);

	Static1 = SXGUICrStatic("Filter:", 8, 18, 32, 20, AnimationsGB->GetHWND(), 0, 0);
	Static1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static1->SetColorText(0, 0, 0);
	Static1->SetColorTextBk(255, 255, 255);
	Static1->SetTransparentTextBk(true);
	Static1->SetColorBrush(255, 255, 255);


	CurAnimName = SXGUICrStatic("Animation: idle", 1149, 649, 145, 19, MainWindow->GetHWND(), 0, 0);
	CurAnimName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CurAnimName->SetColorText(0, 0, 0);
	CurAnimName->SetColorTextBk(255, 255, 255);
	CurAnimName->SetTransparentTextBk(true);
	CurAnimName->SetColorBrush(255, 255, 255);
	CurAnimName->GAlign = {false, false, true, true};
}
void Editor::DestroyUI()
{
	mem_delete(m_pTM);

	mem_delete(D3DWindow);
	mem_delete(AnimList);
	mem_delete(AnimationsGB);
	mem_delete(AnimFilter);
	mem_delete(Static1);
	mem_delete(CurAnimName);
	mem_delete(MainWindow);
}

void Editor::InitD3D()
{
	HWND hWnd = D3DWindow->GetHWND();
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = m_uWidth = rc.right - rc.left;
	UINT height = m_uHeight = rc.bottom - rc.top;


	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_pD3D)
	{
		return;
	}

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.Windowed = 1;
	pp.BackBufferFormat = D3DFMT_UNKNOWN;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferHeight = height;
	pp.BackBufferWidth = width;
	pp.BackBufferCount = 1;
	pp.hDeviceWindow = hWnd;
	pp.MultiSampleQuality = 0;
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	pp.EnableAutoDepthStencil = true;
	pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &pp, &m_pd3dDevice);
	if(FAILED(hr))
	{
		return;
	}

	m_pd3dDevice->GetSwapChain(0, &m_pSwapChain);


	m_mProjMat = SMMatrixPerspectiveFovLH(50.0f / 180.0f * SM_PI, (float)width / (float)height, 0.1f, 10000.0f);

	//load shaders
	//create camera

	m_mViewMat = SMMatrixLookAtLH(float3(10.0f, 10.0f, 10.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
}

void Editor::DestroyD3D()
{
	mem_release(m_pd3dDevice);
	mem_release(m_pD3D);
}

void Editor::DrawAxis()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	m_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pd3dDevice->SetTexture(0, NULL);
	struct vert
	{
		float3_t pos;
		DWORD color;
	};
	float len = 20;
	static vert l[] = {
		{float3_t(0, 0, 0), 0xFFFF0000},
		{float3_t(len, 0, 0), 0xFFFF0000},
		{float3_t(0, 0, 0), 0xFF00FF00},
		{float3_t(0, len, 0), 0xFF00FF00},
		{float3_t(0, 0, 0), 0xFF0000FF},
		{float3_t(0, 0, len), 0xFF0000FF},
	};
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 3, l, sizeof(vert));
}

void Editor::CenterMouse()
{
	POINT p = {0, 0};
	ClientToScreen(D3DWindow->GetHWND(), &p);
	SetCursorPos(m_uWidth / 2 + p.x, m_uHeight / 2 + p.y);
}

void Editor::Update()
{
	if(m_bCamMove)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(D3DWindow->GetHWND(), &pt);
		
		float sens = 0.003f;
		m_cam.Rotate(((int)m_uWidth / 2 - pt.x) * sens, ((int)m_uHeight / 2 - pt.y) * sens);
		if((m_uWidth / 2 - pt.x) || (m_uHeight / 2 - pt.y))
		{
			CenterMouse();
		}
		m_cam.Advance();
	}
	m_mViewMat = m_cam.GetMatrix();

	m_pd3dDevice->BeginScene();

	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(16, 32, 48), 1.0f, 0);
	static VShaderInputCamera VSICData;
	VSICData.mRes = SMMatrixTranspose(m_mWorldMat * m_mViewMat * m_mProjMat);
	VSICData.mWorld = SMMatrixTranspose(m_mWorldMat);
	m_pd3dDevice->SetVertexShaderConstantF(1, (float*)&VSICData, sizeof(VSICData) / sizeof(float) / 4);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&m_mViewMat);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&m_mProjMat);


	DrawAxis();

	m_pd3dDevice->SetPixelShader(NULL);

	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

	m_pAnimMgr->Render();

	m_pd3dDevice->EndScene();

	m_pAnimMgr->Update();

	m_pSwapChain->Present(NULL, NULL, NULL, NULL, D3DSWAPEFFECT_DISCARD);
}

bool filterStr(char const * str1, char const * str2)
{
	const char* p1 = str1;
	const char* p2 = str2;
	const char* r = *p2 == 0 ? str1 : 0;

	while(*p1 != 0 && *p2 != 0)
	{
		if(tolower(*p1) == tolower(*p2))
		{
			if(r == 0)
			{
				r = p1;
			}

			p2++;
		}
		else
		{
			p2 = str2;
			if(tolower(*p1) == tolower(*p2))
			{
				r = p1;
				p2++;
			}
			else
			{
				r = 0;
			}
		}

		p1++;
	}

	return(*p2 == 0);
}

void Editor::RenderAnimList()
{
	UINT c = m_vAnims.size();
	AnimItem * ai;
	char tmpSN[MODEL_MAX_NAME + 32];
	bool filt = m_szAnimFilter[0] != 0;
	int cur = AnimList->GetSel();
	if(cur < 0)
	{
		cur = 0;
	}
	AnimList->Clear();
	for(UINT i = 0; i < c; ++i)
	{
		ai = &m_vAnims[i];
		if(!filt || filterStr(ai->seq->name, m_szAnimFilter))
		{
			sprintf(tmpSN, "[%c%c] %s", ai->isImported ? 'I' : '_', ai->seq->bLooped ? 'L' : '_', ai->seq->name); //I|_, L|_
			AnimList->AddItem(tmpSN);
			AnimList->SetItemData(AnimList->GetCountItem() - 1, (LPARAM)i);
		}
	}
	AnimList->SetSel(cur);
}

void Editor::RenderBoneList()
{
	ISXGUIComboBox * cmb = ((TabAttachments*)(m_pTM->m_pTabAttachments))->AttachBone;
	ISXGUIComboBox * cmb_2 = ((TabHitboxes*)(m_pTM->m_pTabHitboxes))->CBBone;
	int sel = cmb->GetSel();
	char * text = (char*)alloca(sizeof(char) * (cmb->GetItemTextLength(sel) + 1));
	cmb->GetItemText(sel, text);

	int sel2 = cmb_2->GetSel();
	char * text2 = (char*)alloca(sizeof(char)* (cmb_2->GetItemTextLength(sel2) + 1));
	cmb_2->GetItemText(sel2, text2);

	char tmp[MODEL_BONE_MAX_NAME + 1];
	cmb->Clear();
	cmb_2->Clear();
	for(int i = 0, l = m_pCurAnim->GetBoneCount(); i < l; ++i)
	{
		m_pCurAnim->GetBoneName(i, tmp, sizeof(tmp));
		if(!strcmp(tmp, text))
		{
			sel = i;
		}
		if(!strcmp(tmp, text2))
		{
			sel2 = i;
		}
		cmb->AddItem(tmp);
		cmb_2->AddItem(tmp);
	}

	cmb->SetSel(sel);
	cmb_2->SetSel(sel2);

	/*UINT c = m_vAnims.size();
	AnimItem * ai;
	char tmpSN[MODEL_MAX_NAME + 32];
	bool filt = m_szAnimFilter[0] != 0;
	int cur = AnimList->GetSel();
	if(cur < 0)
	{
		cur = 0;
	}
	AnimList->Clear();
	for(UINT i = 0; i < c; ++i)
	{
		ai = &m_vAnims[i];
		if(!filt || filterStr(ai->seq->name, m_szAnimFilter))
		{
			sprintf(tmpSN, "[%c%c] %s", ai->isImported ? 'I' : '_', ai->seq->bLooped ? 'L' : '_', ai->seq->name); //I|_, L|_
			AnimList->AddItem(tmpSN);
			AnimList->SetItemData(AnimList->GetCountItem() - 1, (LPARAM)i);
		}
	}
	AnimList->SetSel(cur);*/
}

void Editor::RenderPartList()
{
	UINT c = m_vMdlParts.size();
	ModelPart * par;
	//char tmpSN[MODEL_MAX_NAME + 32];

	ISXGUIListBox * pList = ((TabAttachments*)m_pTM->m_pTabAttachments)->AttachmentsList;
	
	int cur = pList->GetSel();
	if(cur < 0)
	{
		cur = 0;
	}
	pList->Clear();

	for(UINT i = 0; i < c; ++i)
	{
		par = m_vMdlParts[i];
	
		//sprintf(tmpSN, "[%c%c] %s", ai->isImported ? 'I' : '_', ai->seq->bLooped ? 'L' : '_', ai->seq->name); //I|_, L|_
		pList->AddItem(par->name);
		pList->SetItemData(pList->GetCountItem() - 1, (LPARAM)i);
		
	}
	pList->SetSel(cur);
	OnPartListSelChg();
}

LRESULT Editor::AnimGBProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	Editor * self = (Editor*)cmp->GetUserPtr();

	switch(LOWORD(wParam))
	{
	case IDC_LISTBOX:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			self->OnAnimListSelChg();
			break;
		}
	}

	return(0);
}

void Editor::OnAnimListSelChg()
{
	int sel = AnimList->GetSel();
	AnimItem * item;
	int idx = AnimList->GetItemData(sel);
	TabAnimation * tab = (TabAnimation*)m_pEditor->m_pTM->m_pTabAnimation;
	if(idx < 0)
	{
		tab->AnimPropActChance->Enable(0);
		tab->AnimPropName->Enable(0);
		tab->AnimPropLoopCB->Enable(0);
		tab->AnimPropSpeed->Enable(0);
		tab->AnimPropActCmb->Enable(0);
		tab->AnimCtlPlayBtn->Enable(0);
		tab->AnimCtlPauseBtn->Enable(0);
	}
	else
	{
		item = &m_vAnims[idx];
		m_iCurIdx = idx;

		m_pCurAnim->Play(item->seq->name, 100);

		tab->AnimPropActChance->SetText(String((DWORD)item->seq->act_chance).c_str());
		tab->AnimPropName->SetText(item->seq->name);
		tab->AnimPropLoopCB->SetCheck(item->seq->bLooped);
		tab->AnimPropSpeed->SetText(String(item->seq->framerate).c_str());

		tab->AnimPropActChance->Enable(!item->isImported);
		tab->AnimPropName->Enable(!item->isImported);
		tab->AnimPropLoopCB->Enable(!item->isImported);
		tab->AnimPropSpeed->Enable(!item->isImported);
		tab->AnimPropActCmb->Enable(!item->isImported);
	}
}

void Editor::SetPartFlag(MODEL_PART_FLAGS f, byte v)
{
	TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
	ISXGUIListBox * pList = tab->AttachmentsList;
	int sel = pList->GetSel();
	ModelPart * pt;
	int idx = pList->GetItemData(sel);
	if(idx >= 0)
	{
		pt = m_vMdlParts[idx];
		if(v == 0)
		{
			pt->uFlags &= ~f;
		}
		else
		{
			pt->uFlags |= f;
		}
	}
}

void Editor::OnPartListSelChg()
{
	TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
	ISXGUIListBox * pList = tab->AttachmentsList;
	int sel = pList->GetSel();
	ModelPart * pt;
	int idx = pList->GetItemData(sel);
	if(idx < 0)
	{
		tab->AttachSkinRB->Enable(0);
		tab->AttachBoneRB->Enable(0);
		tab->AttachBone->Enable(0);

		tab->AttachFileBrowse->Enable(0);
		tab->AttachFileField->Enable(0);

		tab->AttachHideCB->Enable(0);
		tab->AttachEnaCollisionCB->Enable(0);
		tab->AttachEnaRaytraceCB->Enable(0);

		tab->AttachXshift->Enable(0);
		tab->AttachXshiftSpin->Enable(0);
		tab->AttachYshift->Enable(0);
		tab->AttachYshiftSpin->Enable(0);
		tab->AttachZshift->Enable(0);
		tab->AttachZshiftSpin->Enable(0);

		tab->AttachFileApply->Enable(0);
	}
	else
	{
		pt = m_vMdlParts[idx];

		tab->AttachSkinRB->Enable(1);
		tab->AttachBoneRB->Enable(1);

		switch(pt->attachDesc.type)
		{
		case MA_SKIN:
			tab->AttachSkinRB->SetCheck(1);
			tab->AttachBoneRB->SetCheck(0);
			tab->AttachBone->Enable(0);
			break;
		case MA_BONE:
			tab->AttachBoneRB->SetCheck(1);	
			tab->AttachSkinRB->SetCheck(0);
			tab->AttachBone->Enable(1);
			tab->AttachBone->SetSel(-1);
			{
				char tmpBone[MODEL_BONE_MAX_NAME + 1];
				for(int i = 0, l = tab->AttachBone->GetCount(); i < l; ++i)
				{
					tab->AttachBone->GetItemText(i, tmpBone);
					if(!strcmp(pt->attachDesc.szBone, tmpBone))
					{
						tab->AttachBone->SetSel(i);
						break;
					}
				}
			}
			break;
		}

		tab->AttachFileApply->Enable(1);

		tab->AttachHideCB->Enable(1);
		tab->AttachHideCB->SetCheck(pt->uFlags & MP_HIDDEN);

		tab->AttachEnaCollisionCB->Enable(1);
		tab->AttachEnaCollisionCB->SetCheck(pt->uFlags & MP_COLLISIONS);

		tab->AttachEnaRaytraceCB->Enable(1);
		tab->AttachEnaRaytraceCB->SetCheck(pt->uFlags & MP_RAYTRACE);

		tab->AttachFileBrowse->Enable(1);
		tab->AttachFileField->Enable(1);

		tab->AttachFileField->SetText(pt->file);

		tab->AttachXshift->Enable(0);
		tab->AttachXshiftSpin->Enable(0);
		tab->AttachYshift->Enable(0);
		tab->AttachYshiftSpin->Enable(0);
		tab->AttachZshift->Enable(0);
		tab->AttachZshiftSpin->Enable(0);
	}
}

ModelFile * Editor::AddModel(const char * mdl, UINT flags, bool forceImport, bool forceLocal)
{
	char * mdlFile = (char*)alloca(strlen(mdl) + 1);
	strcpy(mdlFile, mdl);
	canonize_path(mdlFile);
	const char * localPath = strip_prefix(mdlFile, m_szGamesourceDir);

	ModelFile * pMdl = localPath[0] ? (ModelFile*)m_pAnimMgr->LoadModel(localPath, true) : NULL;

	
	if(pMdl)
	{
		bool bIsImported = ((pMdl->m_hdr.iFlags & MODEL_FLAG_COMPILED) || forceImport) && !forceLocal;

		if(flags & MI_ANIMATIONS)
		{
			UINT c = pMdl->GetSequenceCount();
			AnimItem ai;
			for(UINT i = 0; i < c; ++i)
			{
				ai.seq = (ModelSequence*)pMdl->GetSequence(i);
				ai.mdl = pMdl;
				ai.isImported = bIsImported;
				if(!bIsImported)
				{
					if(!ai.seq->name[0])
					{
						const char * bn = basename(localPath);
						strncpy(ai.seq->name, bn, min(strlen(bn) - 4, MODEL_MAX_NAME));
						ai.seq->name[MODEL_MAX_NAME - 1] = 0;
						ai.seq->framerate = 30;
						ai.seq->activity = 0;
						ai.seq->act_chance = 0;
						ai.seq->bLooped = 0;
					}
				}
				m_vAnims.push_back(ai);
			}
			RenderAnimList();
		}
	}
	//init all sections from mdl data

	m_pCurAnim->AddModel(pMdl, flags);


	m_vMdlParts.push_back(m_pCurAnim->GetPart(m_pCurAnim->GetPartCount() - 1));

	RenderPartList();

	return(pMdl);
}

void Editor::DelModel(UINT id)
{
	if(id >= m_vMdlParts.size())
	{
		return;
	}

	ModelPart * pt = m_vMdlParts[id];
	m_vMdlParts.erase(id);

	const ModelFile * mdl = pt->pMdl;
	if(mdl)
	{
		bool hasAnims = false;
		for(int i = 0, l = m_vAnims.size(); i < l; ++i)
		{
			if(m_vAnims[i].mdl == mdl)
			{
				hasAnims = true;
				m_vAnims.erase(i--);
				--l;
			}
		}
		RenderAnimList();
		OnAnimListSelChg();
		if(hasAnims)
		{
			m_pCurAnim->StopAll();
		}
	}
	m_pCurAnim->DelModel(pt);
	m_pCurAnim->Assembly();

	m_pAnimMgr->UnloadModel(mdl);


	RenderPartList();
}

void Editor::OnPartApply()
{
	TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
	ISXGUIListBox * pList = tab->AttachmentsList;
	int sel = pList->GetSel();
	ModelPart * pt;
	int idx = pList->GetItemData(sel);
	if(idx < 0)
	{
		return;
	}
	pt = m_vMdlParts[idx];

	MODEL_ATTACH ma = tab->AttachBoneRB->GetCheck() ? MA_BONE : MA_SKIN;

	if(ma == MA_BONE)
	{
		int sel = tab->AttachBone->GetSel();
		if(sel < 0)
		{
			MessageBoxA(NULL, "Please select bone", "Error!", MB_OK | MB_ICONSTOP);
			return;
		}
		tab->AttachBone->GetItemText(sel, pt->attachDesc.szBone);
	}
	pt->attachDesc.type = ma;

	char szFile[MODEL_MAX_FILE];
	tab->AttachFileField->GetText(szFile, sizeof(szFile));

	if(strcmp(pt->file, szFile))
	{
		//unload old mdl
		const ModelFile * mdl = pt->pMdl;
		if(mdl)
		{
			bool hasAnims = false;
			for(int i = 0, l = m_vAnims.size(); i < l; ++i)
			{
				if(m_vAnims[i].mdl == mdl)
				{
					hasAnims = true;
					m_vAnims.erase(i--);
					--l;
				}
			}
			RenderAnimList();
			OnAnimListSelChg();
			if(hasAnims)
			{
				m_pCurAnim->StopAll();
			}
			m_pAnimMgr->UnloadModel(mdl);
			pt->pMdl = NULL;
		}

		//load new mdl
		strcpy(pt->file, szFile);
		UINT flags = tab->m_iflags;
		ModelFile * pMdl = (ModelFile*)m_pAnimMgr->LoadModel(szFile, true);
		pt->pMdl = pMdl;
		pt->uImportFlags = flags;
		bool bIsImported = (pMdl->m_hdr.iFlags & MODEL_FLAG_COMPILED);

		if(flags & MI_ANIMATIONS)
		{
			UINT c = pMdl->GetSequenceCount();
			AnimItem ai;
			for(UINT i = 0; i < c; ++i)
			{
				ai.seq = (ModelSequence*)pMdl->GetSequence(i);
				ai.mdl = pMdl;
				ai.isImported = bIsImported;
				if(!bIsImported)
				{
					if(!ai.seq->name[0])
					{
						const char * bn = basename(szFile);
						strncpy(ai.seq->name, bn, min(strlen(bn) - 4, MODEL_MAX_NAME));
						ai.seq->name[MODEL_MAX_NAME - 1] = 0;
						ai.seq->framerate = 30;
						ai.seq->activity = 0;
						ai.seq->act_chance = 0;
						ai.seq->bLooped = 0;
					}
				}
				m_vAnims.push_back(ai);
			}
			RenderAnimList();
		}
	}

	m_pCurAnim->Assembly();
	//
}
