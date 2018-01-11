#include "Editor.h"

#include <Commdlg.h>

#include <ShlObj.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#include <gcore/sxgcore.h>

void msgbx(int level, const char* format, ...)
{
	va_list va;
	char buf[REPORT_MSG_MAX_LEN];
	va_start(va, format);
	vsprintf_s(buf, REPORT_MSG_MAX_LEN, format, va);
	va_end(va);

	char * title;
	UINT flags = MB_OK;

	switch(level)
	{
	case REPORT_MSG_LEVEL_ERROR:
		title = "Error";
		flags |= MB_ICONSTOP;
		break;
	case REPORT_MSG_LEVEL_WARNING:
		title = "Warning";
		flags |= MB_ICONWARNING;
		break;
	default:
		title = "Notice";
		flags |= MB_ICONINFORMATION;
		break;
	}
	if(REPORT_MSG_LEVEL_NOTICE != level)
	{
		MessageBoxA(NULL, buf, title, flags);
	}
	if(REPORT_MSG_LEVEL_ERROR == level)
	{
		exit(-1);
	}
}

report_func reportf = msgbx;

Editor::Editor():
m_bCamMove(false),
m_iCurIdx(-1),
m_pHitboxesPart(NULL),
m_currentAxe(HA_NONE),
m_bIsDragging(false),
m_htype(HT_MOVE),
m_bIsDraggingStop(false),
m_iActiveHitbox(0),
m_bDirty(false)
{
	GetModuleFileNameA(NULL, m_szGamesourceDir, sizeof(m_szGamesourceDir));
	canonize_path(m_szGamesourceDir);
	dirname(m_szGamesourceDir);
	strcat(m_szGamesourceDir, "gamesource/");
	SetCurrentDirectoryA(m_szGamesourceDir);

	InitUI();
	InitD3D();

	m_pvActivities = &((TabActivities*)m_pTM->m_pTabActivities)->m_vItems;

	m_pEditor = this;
	m_szAnimFilter[0] = 0;
	m_szEditFile[0] = 0;

	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlGB->AddHandler(AnimTBProc, WM_HSCROLL);
	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgressTrack->AddHandler(AnimTBProc, WM_SETFOCUS);
	//((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgress->AddHandler(AnimTBProc, WM_SETFOCUS);
	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgressTrack->AddHandler(AnimTBProc, WM_KILLFOCUS);
	//((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgress->AddHandler(AnimTBProc, WM_KILLFOCUS);


	/*m_szGamesourceDir[0] = 0;
	if(!GetRegGSdir())
	{
		
	}*/
	


	m_pAnimMgr = new AnimationManager(m_pd3dDevice);
	m_pCurAnim = new Animation(m_pAnimMgr);
	m_pCurAnim->setCallback((AnimStateCB)AnimPlayCB);
	m_pCurAnim->setProgressCB((::AnimProgressCB)AnimProgressCB);
	//m_pCurAnim->setModel("C:/DSe/SX/project/gamesource/models/krovosos/krovososa.dse");
	//m_pCurAnim->setModel("C:/DSe/SX/project/gamesource/models/spas/spasa.dse");
	//m_pCurAnim->setModel("C:/DSe/SX/project/gamesource/models/pm/pma.dse");
	//m_pCurAnim->setModel("C:/DSe/SX/project/gamesource/models/ak74/ak74a.dse");
	//m_pCurAnim->playAnimation("reload", 0);

	//addModel("C:/revo/build/gamesource/models/krovosos/krovososa.dse");//ak74_ref.dse
	//addModel("C:/revo/build/gamesource/models/ak74/ak74_ref.dse");
	//addModel("C:/revo/build/gamesource/models/ak74/hands.dse");
	//addModel("C:/revo/build/gamesource/models/ak74/reload.dse");

	/*ModelPart mp;
	mp.attachDesc.type = MA_BONE;
	strcpy(mp.attachDesc.szBone, "bip01_r_hand");
	mp.uImportFlags = MI_ALL;
	mp.pMdl = m_pAnimMgr->loadModel("C:/revo/build/gamesource/models/ak74/ak74.dse", true);
	m_pCurAnim->addModel(&mp);*/

	//bip01_r_hand
	//addModel("F:/revo/build/gamesource/models/ak74/ak74.dse");
	//addModel("F:/revo/build/gamesource/models/ak74/idle.dse");
	//addModel("C:/revo/build/gamesource/models/ak74/reload.dse");
	//addModel("C:/revo/build/gamesource/models/ak74/shoot.dse");
	//m_pCurAnim->assembly();
	//if(m_pCurAnim->m_pMdl)
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
	//	RenderAnimList();
	//	RenderBoneList();
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
			edt->MenuSave(hwnd);
			break;
		case ID_FILE_SAVEAS:
			edt->MenuSaveAs(hwnd);
			break;

		case ID_FILE_IMPORT:
			edt->MenuBrowseImport(hwnd);
			break;

		case ID_FILE_NEW:
			edt->MenuNew(hwnd);
			break;

		case ID_FILE_EXIT:
			if(edt->m_bDirty && MessageBoxA(hwnd, "All unsaved changes will be lost", "Exit?", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
			{
				return(0);
			}
			PostQuitMessage(0);
			break;

		case IDC_ANIM_PAUSE:
			edt->m_pCurAnim->stop();
			break;

		case IDC_ANIM_PLAY:
			edt->m_pCurAnim->resume();
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

		case IDC_ANIM_ACT_NAME:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				ISXGUIComboBox * pList = ((TabAnimation*)edt->m_pTM->m_pTabAnimation)->AnimPropActCmb;
				
				m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->activity = pList->GetSel();
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
					edt->delModel(sel);
				}
			}
			break;

		case IDC_PT_ADD:
			{
				m_pEditor->addModel("");
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
			edt->m_pCurAnim->assembly();
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

		case IDC_HB_DEL:
			{
				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				int len = MODEL_MAX_NAME;
				int sel = pList->GetSel();
				if(sel < 0)
				{
					break;
				}
				sel = pList->GetItemData(sel);

				if(Tools::DlgConfirm())
				{
					edt->DelHitbox(sel);
				}
			}
			break;

		case IDC_HB_ADD:
			{
				if(!m_pEditor->m_pHitboxesPart)
				{
					m_pEditor->m_pHitboxesPart = m_pEditor->addModel("!hitboxes");
				}
				ModelHitbox hb;
				memset(&hb, 0, sizeof(hb));
				strcpy(hb.name, "New hitbox");

				m_pEditor->m_pHitboxesPart->AddHitbox(&hb);
				m_pEditor->m_pCurAnim->assembly();
				m_pEditor->UpdateHitboxList(m_pEditor->m_pHitboxesPart, false);

				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				pList->SetSel(pList->GetCountItem() - 1);
				m_pEditor->OnHitboxListSelChg();
			}
			break;

		case IDC_HB_LB:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				edt->OnHitboxListSelChg();
				break;
			}
			break;

		case IDC_HB_NAME:
			if(HIWORD(wParam) == EN_CHANGE)
			{
				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				int sel = pList->GetSel();
				if(sel < 0 || (sel = pList->GetItemData(sel)) < 0)
				{
					break;
				}
				
				HitboxItem * hbx = &m_pEditor->m_vHitboxes[sel];
				
				int len = GetWindowTextLengthA((HWND)lParam);

				GetWindowTextA((HWND)lParam, hbx->hb->name, MODEL_MAX_NAME);
				char tmpSN[MODEL_MAX_NAME + 32];
				sprintf(tmpSN, "[%c] %s", hbx->isImported ? 'I' : '_', hbx->hb->name);
				pList->SetTextItem(pList->GetSel(), tmpSN);
				//m_pEditor->RenderHitboxList();
			}
			break;

		case IDC_HB_BONE:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				int sel = pList->GetSel();
				if(sel < 0 || (sel = pList->GetItemData(sel)) < 0)
				{
					break;
				}
				HitboxItem * hbx = &m_pEditor->m_vHitboxes[sel];
				GetWindowTextA((HWND)lParam, hbx->hb->bone, MODEL_BONE_MAX_NAME);
			}
			break;

		case IDC_HB_BODYPART:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				ISXGUIComboBox * cmb = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->CBBodyPart;
				int sel = pList->GetSel();
				if(sel < 0 || (sel = pList->GetItemData(sel)) < 0)
				{
					break;
				}
				HitboxItem * hbx = &m_pEditor->m_vHitboxes[sel];
				hbx->hb->part = (HITBOX_BODYPART)cmb->GetItemData(cmb->GetSel());
			}
			break;

		case IDC_HB_TYPE:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				ISXGUIComboBox * cmb = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->CBType;
				int sel = pList->GetSel();
				if(sel < 0 || (sel = pList->GetItemData(sel)) < 0)
				{
					break;
				}
				HitboxItem * hbx = &m_pEditor->m_vHitboxes[sel];
				hbx->hb->type = (HITBOX_TYPE)cmb->GetItemData(cmb->GetSel());
			}
			break;

		case IDC_HB_ED_L:
		case IDC_HB_ED_W:
		case IDC_HB_ED_H:
		case IDC_HB_ED_POS_X:
		case IDC_HB_ED_POS_Y:
		case IDC_HB_ED_POS_Z:
		case IDC_HB_ED_ROT_X:
		case IDC_HB_ED_ROT_Y:
		case IDC_HB_ED_ROT_Z:
			if(HIWORD(wParam) == EN_CHANGE)
			{
				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				int sel = pList->GetSel();
				if(sel < 0 || (sel = pList->GetItemData(sel)) < 0)
				{
					break;
				}
				HitboxItem * hbx = &m_pEditor->m_vHitboxes[sel];

				int len = GetWindowTextLengthA((HWND)lParam);
				char * txt = (char*)alloca(len + 1);
				GetWindowTextA((HWND)lParam, txt, len + 1);
				float f;
				if(sscanf(txt, "%f", &f) < 1)
				{
					SetWindowTextA((HWND)lParam, "0");
				}
				else
				{
					switch(LOWORD(wParam))
					{
					case IDC_HB_ED_L:
						hbx->hb->lwh.x = f;
						break;

					case IDC_HB_ED_W:
						hbx->hb->lwh.y = f;
						break;

					case IDC_HB_ED_H:
						hbx->hb->lwh.z = f;
						break;

					case IDC_HB_ED_POS_X:
						hbx->hb->pos.x = f;
						break;

					case IDC_HB_ED_POS_Y:
						hbx->hb->pos.y = f;
						break;

					case IDC_HB_ED_POS_Z:
						hbx->hb->pos.z = f;
						break;

					case IDC_HB_ED_ROT_X:
						hbx->hb->rot.x = SMToRadian(f);
						break;

					case IDC_HB_ED_ROT_Y:
						hbx->hb->rot.y = SMToRadian(f);
						break;

					case IDC_HB_ED_ROT_Z:
						hbx->hb->rot.z = SMToRadian(f);
						break;

					}
				}
			}
			break;

		case IDC_HB_TRANS:
			m_pEditor->m_htype = HT_MOVE;
			break;
		case IDC_HB_ROT:
			m_pEditor->m_htype = HT_ROTATE;
			break;
		case IDC_HB_SCALE:
			m_pEditor->m_htype = HT_SCALE;
			break;
		}
		break;

	case WM_CLOSE:
		if(m_pEditor->m_bDirty && MessageBoxA(hwnd, "All unsaved changes will be lost", "Quit?", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
		{
			return(0);
		}
		return(DefWindowProc(hwnd, msg, wParam, lParam));

	/*case WM_KEYDOWN:
		if(wParam == 'S' && (GetKeyState(VK_CONTROL) & 0x80))
		{
			edt->MenuSave(hwnd);
			return(1);
		}
		break;*/

	case EM_LOADACTIVITIES:
		TabAnimation * tAnim = (TabAnimation*)edt->m_pTM->m_pTabAnimation;
		tAnim->AnimPropActCmb->Clear();
		tAnim->AnimPropActCmb->AddItem("");
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
			edt->m_cam.move(Camera::CAMERA_MOVE_FORWARD, 1);
			break;
		case 'S':
			edt->m_cam.move(Camera::CAMERA_MOVE_BACKWARD, 1);
			break;
		case 'A':
			edt->m_cam.move(Camera::CAMERA_MOVE_LEFT, 1);
			break;
		case 'D':
			edt->m_cam.move(Camera::CAMERA_MOVE_RIGHT, 1);
			break;
		}
		break;
	case WM_KEYUP:
		switch(wParam)
		{
		case 'W':
			edt->m_cam.move(Camera::CAMERA_MOVE_FORWARD, 0);
			break;
		case 'S':
			edt->m_cam.move(Camera::CAMERA_MOVE_BACKWARD, 0);
			break;
		case 'A':
			edt->m_cam.move(Camera::CAMERA_MOVE_LEFT, 0);
			break;
		case 'D':
			edt->m_cam.move(Camera::CAMERA_MOVE_RIGHT, 0);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		m_pEditor->OnMouseDown(((int)(short)LOWORD(lParam)), ((int)(short)HIWORD(lParam)));
		break;

	case WM_LBUTTONDOWN:
		m_pEditor->m_bIsDragging = true;
		m_pEditor->m_bIsDraggingStart = true;
		break;

	case WM_LBUTTONUP:
		m_pEditor->m_bIsDragging = false;
		m_pEditor->m_bIsDraggingStop = true;
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
			edt->m_pCurAnim->play(edt->m_vAnims[seqi].seq->name, 100);
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
	ModelSequence const * seq = m_pEditor->m_pCurAnim->getCurAnim(slot);
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
		m_pEditor->m_pCurAnim->setProgress(pos / 1000.0f);
		break;
	case WM_SETFOCUS:
		m_pEditor->m_pCurAnim->setAdvance(false);
		break;
	case WM_KILLFOCUS:
		m_pEditor->m_pCurAnim->setAdvance(true);
		break;
	}
	return(0);
}

void Editor::MenuNew(HWND hwnd)
{
	if(m_bDirty && MessageBoxA(hwnd, "All unsaved changes will be lost", "New?", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
	{
		return;
	}

	while(m_pEditor->m_vMdlParts.size())
	{
		m_pEditor->delModel(0);
	}
	m_bDirty = false;
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
		strcpy(m_szEditFile, ofn.lpstrFile);
		//wprintf(L"File: %s\n", ofn.lpstrFile);

		//unload all parts
		while(m_pEditor->m_vMdlParts.size())
		{
			m_pEditor->delModel(0);
		}
		m_pEditor->addModel(ofn.lpstrFile, MI_ALL, false, true);
		m_pEditor->m_pCurAnim->assembly();
		RenderBoneList();
		m_bDirty = false;
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
			m_pEditor->addModel(ofn.lpstrFile, iflags);
			m_pEditor->m_pCurAnim->assembly();
			RenderBoneList();
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

void Editor::MenuSave(HWND hwnd)
{
	if(m_szEditFile[0])
	{
		if(SaveTo(m_szEditFile))
		{
			m_bDirty = false;
		}
		else
		{
			MessageBoxA(hwnd, "Unable to write file", "Error!", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MenuSaveAs(hwnd);
	}
}
void Editor::MenuSaveAs(HWND hwnd)
{
	OPENFILENAMEA ofn;
	char szFile[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Model file (*.dse)\0*.dse\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	wchar_t bf[256];
	GetCurrentDirectoryW(256, bf);

	if(GetSaveFileNameA(&ofn) == TRUE)
	{
		SetCurrentDirectoryW(bf);
		

		if(SaveTo(ofn.lpstrFile))
		{
			strcpy(m_szEditFile, ofn.lpstrFile);
			m_bDirty = false;
		}
		else
		{
			MessageBoxA(hwnd, "Unable to write file", "Error!", MB_OK | MB_ICONSTOP);
		}
	}
}

bool Editor::SaveTo(char * to)
{
	if(strcmpi(to + strlen(to) - 4, ".dse"))
	{
		strcat(to, ".dse");
	}

	Animation pTmpAnim(m_pAnimMgr);
	ModelPart mp;
	memset(&mp, 0, sizeof(mp));
	strcpy(mp.file, "!bones");
	strcpy(mp.name, "!bones");
	mp.pMdl = m_pCurAnim->m_pMdl;
	mp.attachDesc.type = MA_SKIN;
	pTmpAnim.addModel(&mp);
	int c = 0;
	for(int i = 0, l = m_vMdlParts.size(); i < l; ++i)
	{
		if(!(m_vMdlParts[i]->uFlags & MP_IMPORTED))
		{
			pTmpAnim.addModel(m_vMdlParts[i]);
			++c;
		}
	}
	pTmpAnim.assembly();

	pTmpAnim.m_pMdl->m_hdr2.iActivitiesTableCount = m_pvActivities->size();
	pTmpAnim.m_pMdl->pActivities = new ModelActivity[pTmpAnim.m_pMdl->m_hdr2.iActivitiesTableCount];
	for(uint32_t i = 0; i < pTmpAnim.m_pMdl->m_hdr2.iActivitiesTableCount; ++i)
	{
		strcpy(pTmpAnim.m_pMdl->pActivities[i].szName, m_pvActivities[0][i].act.c_str());
	}

	pTmpAnim.m_pMdl->m_hdr2.iDepsCount = m_vMdlParts.size() - c;
	pTmpAnim.m_pMdl->m_pParts = new ModelPart[pTmpAnim.m_pMdl->m_hdr2.iDepsCount];
	ModelPart * pmp = pTmpAnim.m_pMdl->m_pParts;
	for(int i = 0, l = m_vMdlParts.size(); i < l; ++i)
	{
		if(m_vMdlParts[i]->uFlags & MP_IMPORTED)
		{
			if(m_vMdlParts[i]->file[0] == '@')
			{
				--pTmpAnim.m_pMdl->m_hdr2.iDepsCount;
			}
			else
			{
				*(pmp++) = *m_vMdlParts[i];
			}
		}
	}

	return(pTmpAnim.m_pMdl->Save(to));
}

Editor * Editor::GetInstance()
{
	return(m_pEditor);
}

Editor * Editor::m_pEditor;

void Editor::InitUI()
{
	MainWindow = SXGUICrBaseWnd("MainWindow", "MainWindow", 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, 1320, 730, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(MainWindow);
	MainWindow->AddHandler(MenuCmd, WM_COMMAND);
	MainWindow->AddHandler(MenuCmd, WM_CLOSE, 0, 0, 0, 0, 1);
	MainWindow->AddHandler(MenuCmd, WM_PARENTNOTIFY);
	MainWindow->AddHandler(MenuCmd, EM_LOADACTIVITIES);
	//MainWindow->AddHandler(MenuCmd, WM_KEYDOWN);
	MainWindow->MinSizeX = MAINWIN_SIZE_X;
	MainWindow->MinSizeY = MAINWIN_SIZE_Y;


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
	D3DWindow->AddHandler(CamInput, WM_LBUTTONDOWN);
	D3DWindow->AddHandler(CamInput, WM_LBUTTONUP);
	D3DWindow->AddHandler(CamInput, WM_MOUSEMOVE);

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
	
	SGCore_0Create("SXAnimEditor", hWnd, width, height, 1, 0, true);
	SGCore_Dbg_Set(msgbx);

	char tmp[260];

	sprintf(tmp, "%stextures/", m_szGamesourceDir);
	//SGCore_LoadTexStdPath(tmp);
	sprintf(tmp, "%sshaders/", m_szGamesourceDir);
	SGCore_ShaderSetStdPath(tmp);

	m_pd3dDevice = SGCore_GetDXDevice();
	/*
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
	pp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
	pp.EnableAutoDepthStencil = true;
	pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &pp, &m_pd3dDevice);
	if(FAILED(hr))
	{
		return;
	}

	*/
	m_pd3dDevice->GetSwapChain(0, &m_pSwapChain);

	m_pVSH = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs", "stdr_skin.vs", SHADER_CHECKDOUBLE_PATH);
	m_pPSH = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin.ps", SHADER_CHECKDOUBLE_PATH);

	m_pVSHs = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs", "stdr_geom.vs", SHADER_CHECKDOUBLE_PATH);
	m_pPSHs = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom.ps", SHADER_CHECKDOUBLE_PATH);

	m_mProjMat = SMMatrixPerspectiveFovLH(50.0f / 180.0f * SM_PI, (float)width / (float)height, 0.1f, 10000.0f);

	//load shaders
	//create camera

	m_mViewMat = SMMatrixLookAtLH(float3(10.0f, 10.0f, 10.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
	m_mHelperMat = SMMatrixTranslation(10.0f, 10.0f, 10.0f);
}

void Editor::DestroyD3D()
{
	SGCore_AKill();
	//mem_release(m_pd3dDevice);
	//mem_release(m_pD3D);
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
		m_cam.advance();
	}
	m_mViewMat = m_cam.GetMatrix();

	m_pd3dDevice->BeginScene();

	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(16, 32, 48), 1.0f, 0);
	static VShaderInputCamera VSICData;
	VSICData.mRes = SMMatrixTranspose(m_mWorldMat * m_mViewMat * m_mProjMat);
	VSICData.mWorld = SMMatrixTranspose(m_mWorldMat);
	m_pd3dDevice->SetVertexShaderConstantF(0, (float*)&VSICData, sizeof(VSICData) / sizeof(float) / 4);

	m_pd3dDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&m_mViewMat);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&m_mProjMat);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixIdentity()));

	DrawAxis();

	m_pd3dDevice->SetPixelShader(NULL);

	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

	if(m_pCurAnim->m_pMdl && (m_pCurAnim->m_pMdl->m_hdr.iFlags & MODEL_FLAG_STATIC))
	{
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, m_pVSHs);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, m_pPSHs);
	}
	else
	{
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, m_pVSH);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, m_pPSH);
	}
	m_pAnimMgr->render();
	SGCore_ShaderUnBind();

	if(((TabHitboxes*)m_pTM->m_pTabHitboxes)->m_bShown)
	{
		DrawHitboxes();
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, 0);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixIdentity()));
		m_pCurAnim->RenderSkeleton(m_iActiveHitbox > 0 ? m_pCurAnim->getBone(m_vHitboxes[m_iActiveHitbox].hb->bone) : -1);
		if(m_vHitboxes.size() > m_iActiveHitbox)
		{
			HitboxItem * hbi = &m_vHitboxes[m_iActiveHitbox];
			SMMATRIX mBone = (hbi->hb->bone[0] ? m_pCurAnim->getBoneTransform(m_pCurAnim->getBone(hbi->hb->bone)) : SMMatrixIdentity());

			//m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, 0);

			m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(m_mHelperMat * mBone));
			switch(m_htype)
			{
			case HT_MOVE:
				DrawHandlerMove();
				break;

			case HT_ROTATE:
				DrawHandlerRotate();
				break;

			case HT_SCALE:
				DrawHandlerScale();
				break;
			}

		}
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, 1);
	}
	m_pd3dDevice->EndScene();

	m_pAnimMgr->update();
	m_pAnimMgr->sync();

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
	cmb_2->AddItem("");
	for(int i = 0, l = m_pCurAnim->getBoneCount(); i < l; ++i)
	{
		m_pCurAnim->getBoneName(i, tmp, sizeof(tmp));
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
		if(par->file[0] != '!')
		{
			pList->AddItem(par->name);
			pList->SetItemData(pList->GetCountItem() - 1, (LPARAM)i);
		}
		
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

		m_pCurAnim->play(item->seq->name, 100);

		tab->AnimPropActCmb->SetSel(item->seq->activity);
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

ModelFile * Editor::addModel(const char * mdl, UINT flags, bool forceImport, bool forceLocal)
{
	char * mdlFile = (char*)alloca(strlen(mdl) + 2) + 1;
	strcpy(mdlFile, mdl);
	canonize_path(mdlFile);
	const char * localPath = strip_prefix(mdlFile, m_szGamesourceDir);
	if(forceLocal)
	{
		*(char*)(--localPath) = '@';
	}
	ModelFile * pMdl = localPath[0] ? (ModelFile*)m_pAnimMgr->loadModel(localPath, true) : NULL;

	bool bIsImported = forceImport && !forceLocal;
	if(pMdl)
	{
		bIsImported = ((pMdl->m_hdr.iFlags & MODEL_FLAG_COMPILED) || forceImport) && !forceLocal;

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

			for(int i = 0, l = pMdl->m_hdr2.iActivitiesTableCount; i < l; ++i)
			{
				TabActivities::ActivityItem ai;
				ai.act = pMdl->pActivities[i].szName;
				ai.mdl = pMdl;
				ai.isImported = bIsImported;
				m_pvActivities->push_back(ai);
		}
			((TabActivities*)m_pTM->m_pTabActivities)->RenderList();
		}

		if(flags & MI_HITBOXES)
		{
			UpdateHitboxList(pMdl, bIsImported);
		}
	}
	//init all sections from mdl data

	m_pCurAnim->addModel(pMdl, flags);


	m_vMdlParts.push_back(m_pCurAnim->GetPart(m_pCurAnim->GetPartCount() - 1));
	if(bIsImported)
	{
		m_vMdlParts[m_vMdlParts.size() - 1]->uFlags |= MP_IMPORTED;
	}

	RenderPartList();

	if(pMdl && forceLocal)
	{
		for(uint32_t i = 0; i < pMdl->m_hdr2.iDepsCount; ++i)
		{
			ModelPart * mpSrc = &pMdl->m_pParts[i];
			addModel(mpSrc->file, mpSrc->uImportFlags, true);
			ModelPart * mp = m_vMdlParts[m_vMdlParts.size() - 1];
			mpSrc->pMdl = mp->pMdl;
			*mp = *mpSrc;
		}
	}

	return(pMdl);
}

void Editor::delModel(UINT id)
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
			m_pCurAnim->stopAll();
		}

		for(int i = 0, l = m_vHitboxes.size(); i < l; ++i)
		{
			if(m_vHitboxes[i].mdl == mdl)
			{
				m_vHitboxes.erase(i);
				--i;
				--l;
			}
		}
		RenderHitboxList();
		OnHitboxListSelChg();

		for(int i = 0, l = m_pvActivities->size(); i < l; ++i)
		{
			if(m_pvActivities[0][i].mdl == mdl)
			{
				m_pvActivities->erase(i);
				--i; --l;
			}
		}
		((TabActivities*)m_pTM->m_pTabActivities)->RenderList();

		if(mdl == m_pHitboxesPart)
		{
			m_pHitboxesPart = 0;
		}
	}
	m_pCurAnim->delModel(pt);
	m_pCurAnim->assembly();

	m_pAnimMgr->unloadModel(mdl);


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
				m_pCurAnim->stopAll();
			}

			for(int i = 0, l = m_vHitboxes.size(); i < l; ++i)
			{
				if(m_vHitboxes[i].mdl == mdl)
				{
					m_vHitboxes.erase(i);
					--i;
					--l;
				}
			}
			RenderHitboxList();
			OnHitboxListSelChg();

			for(int i = 0, l = m_pvActivities->size(); i < l; ++i)
			{
				if(m_pvActivities[0][i].mdl == mdl)
				{
					m_pvActivities->erase(i);
					--i; --l;
				}
			}
			((TabActivities*)m_pTM->m_pTabActivities)->RenderList();


			m_pAnimMgr->unloadModel(mdl);
			pt->pMdl = NULL;
		}

		//load new mdl
		strcpy(pt->file, szFile);
		UINT flags = tab->m_iflags;
		ModelFile * pMdl = (ModelFile*)m_pAnimMgr->loadModel(szFile, true);
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

			for(int i = 0, l = pMdl->m_hdr2.iActivitiesTableCount; i < l; ++i)
			{
				TabActivities::ActivityItem ai;
				ai.act = pMdl->pActivities[i].szName;
				ai.mdl = pMdl;
				ai.isImported = bIsImported;
				m_pvActivities->push_back(ai);
			}
			((TabActivities*)m_pTM->m_pTabActivities)->RenderList();
		}

		if(flags & MI_HITBOXES)
		{
			UpdateHitboxList(pMdl, bIsImported);
		}

	}

	m_pCurAnim->assembly();
	RenderBoneList();
	//
}

void Editor::RenderHitboxList()
{
	UINT c = m_vHitboxes.size();
	HitboxItem * hbi;
	char tmpSN[MODEL_MAX_NAME + 32];

	TabHitboxes * tab = (TabHitboxes*)m_pEditor->m_pTM->m_pTabHitboxes;
	ISXGUIListBox * pList = tab->HBList;


	int cur = pList->GetSel();
	if(cur < 0)
	{
		cur = 0;
	}
	pList->Clear();

	for(UINT i = 0; i < c; ++i)
	{
		hbi = &m_vHitboxes[i];
			
		sprintf(tmpSN, "[%c] %s", hbi->isImported ? 'I' : '_', hbi->hb->name);
		pList->AddItem(tmpSN);
		pList->SetItemData(pList->GetCountItem() - 1, (LPARAM)i);
	}
	pList->SetSel(cur);

	OnHitboxListSelChg();
}

void Editor::OnHitboxListSelChg()
{
	TabHitboxes * tab = (TabHitboxes*)m_pEditor->m_pTM->m_pTabHitboxes;
	ISXGUIListBox * pList = tab->HBList;

	int sel = pList->GetSel();
	HitboxItem * hbx;
	int idx = pList->GetItemData(sel);
	if(idx < 0)
	{
		tab->CBBodyPart->Enable(0);
		tab->CBBone->Enable(0);
		tab->CBType->Enable(0);

		tab->EdL->Enable(0);
		tab->EdW->Enable(0);
		tab->EdH->Enable(0);

		tab->EdName->Enable(0);

		tab->EdPosX->Enable(0);
		tab->EdPosY->Enable(0);
		tab->EdPosZ->Enable(0);

		tab->EdRotX->Enable(0);
		tab->EdRotY->Enable(0);
		tab->EdRotZ->Enable(0);

		tab->BtnDel->Enable(0);
	}
	else
	{
		hbx = &m_vHitboxes[idx];
		m_iActiveHitbox = idx;
		tab->CBBodyPart->Enable(!hbx->isImported);
		tab->CBBone->Enable(!hbx->isImported);
		tab->CBType->Enable(!hbx->isImported);

		tab->EdL->Enable(!hbx->isImported);
		tab->EdW->Enable(!hbx->isImported);
		tab->EdH->Enable(!hbx->isImported);

		tab->EdName->Enable(!hbx->isImported);

		tab->EdPosX->Enable(!hbx->isImported);
		tab->EdPosY->Enable(!hbx->isImported);
		tab->EdPosZ->Enable(!hbx->isImported);

		tab->EdRotX->Enable(!hbx->isImported);
		tab->EdRotY->Enable(!hbx->isImported);
		tab->EdRotZ->Enable(!hbx->isImported);

		tab->BtnDel->Enable(1);

		tab->EdName->SetText(hbx->hb->name);

		tab->CBBodyPart->SetSel(hbx->hb->part);
		tab->CBType->SetSel(hbx->hb->type);
		char tmp[MODEL_BONE_MAX_NAME];
		for(int i = 0, l = tab->CBBone->GetCount(); i < l; ++i)
		{
			tab->CBBone->GetItemText(i, tmp);
			if(!strcmp(tmp, hbx->hb->bone))
			{
				tab->CBBone->SetSel(i);
			}
		}

		sprintf(tmp, "%f", hbx->hb->lwh.x); tab->EdL->SetText(tmp);
		sprintf(tmp, "%f", hbx->hb->lwh.y); tab->EdW->SetText(tmp);
		sprintf(tmp, "%f", hbx->hb->lwh.z); tab->EdH->SetText(tmp);

		sprintf(tmp, "%f", hbx->hb->pos.x); tab->EdPosX->SetText(tmp);
		sprintf(tmp, "%f", hbx->hb->pos.y); tab->EdPosY->SetText(tmp);
		sprintf(tmp, "%f", hbx->hb->pos.z); tab->EdPosZ->SetText(tmp);

		sprintf(tmp, "%f", SMToAngle(hbx->hb->rot.x)); tab->EdRotX->SetText(tmp);
		sprintf(tmp, "%f", SMToAngle(hbx->hb->rot.y)); tab->EdRotY->SetText(tmp);
		sprintf(tmp, "%f", SMToAngle(hbx->hb->rot.z)); tab->EdRotZ->SetText(tmp);
		
	}
}

void Editor::UpdateHitboxList(ModelFile * pMdl, bool bIsImported)
{
	for(int i = 0, l = m_vHitboxes.size(); i < l; ++i)
	{
		if(m_vHitboxes[i].mdl == pMdl)
		{
			bIsImported = m_vHitboxes[i].isImported;
			m_vHitboxes.erase(i);
			--i;
			--l;
		}
	}

	uint32_t c = pMdl->getHitboxCount();
	HitboxItem hbi;
	for(uint32_t i = 0; i < c; ++i)
	{
		hbi.hb = (ModelHitbox*)pMdl->getHitbox(i);
		hbi.mdl = pMdl;
		hbi.isImported = bIsImported;
		hbi.id = i;

		m_vHitboxes.push_back(hbi);
	}
	RenderHitboxList();
}

void Editor::DelHitbox(UINT id)
{
	if(id >= m_vHitboxes.size())
	{
		return;
	}

	HitboxItem * hbx = &m_vHitboxes[id];
	
	((ModelFile*)hbx->mdl)->DelHitbox(hbx->id);
	m_pCurAnim->assembly();
	
	UpdateHitboxList((ModelFile*)hbx->mdl, false);
	OnHitboxListSelChg();
}

void Editor::DrawHitboxes()
{
	m_pd3dDevice->SetVertexShader(NULL);
	m_pd3dDevice->SetPixelShader(NULL);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	m_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pd3dDevice->SetTexture(0, NULL);
	
	for(int i = 0, l = m_vHitboxes.size(); i < l; ++i)
	{
		HitboxItem * hbi = &m_vHitboxes[i];
		SMMATRIX mBone = (hbi->hb->bone[0] ? m_pCurAnim->getBoneTransform(m_pCurAnim->getBone(hbi->hb->bone)) : SMMatrixIdentity());
		m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&((m_iActiveHitbox == i && m_bIsDragging ? m_mHitboxMat :
			SMMatrixRotationX(hbi->hb->rot.x)
			* SMMatrixRotationY(hbi->hb->rot.y)
			* SMMatrixRotationZ(hbi->hb->rot.z)
			* SMMatrixTranslation(hbi->hb->pos))
			* mBone
			));

		if(m_iActiveHitbox == i && !m_bIsDragging)
		{
			switch(m_htype)
			{
			case HT_MOVE:
				m_mHelperMat = SMMatrixTranslation(hbi->hb->pos/* + float3(mBone._41, mBone._42, mBone._43)*/);
				break;
			case HT_ROTATE:
			case HT_SCALE:
				m_mHelperMat = SMMatrixRotationX(hbi->hb->rot.x)
					* SMMatrixRotationY(hbi->hb->rot.y)
					* SMMatrixRotationZ(hbi->hb->rot.z)
					* SMMatrixTranslation(hbi->hb->pos/* + float3(mBone._41, mBone._42, mBone._43)*/);
				break;
			}
		}

		DWORD color = m_iActiveHitbox == i ? 0xFFFFFF00 : 0xFFFFFFFF;
		switch(hbi->hb->type)
		{
		case HT_BOX:
			DrawBox(hbi->hb->lwh, color);
			break;
		case HT_CAPSULE:
			DrawCapsule(hbi->hb->lwh, color);
			break;
		case HT_ELIPSOID:
			DrawSphere(hbi->hb->lwh, color);
			break;
		case HT_CYLINDER:
			DrawCylinder(hbi->hb->lwh, color);
			break;
		}
	}
	//DrawCapsule(float3_t(10.0f, 20.0f, 30.0f));
}

void Editor::DrawBox(const float3_t & lwh, DWORD color)
{
	vert l[] = {
		{lwh * float3_t(-0.5f, -0.5f, -0.5f), color},
		{lwh * float3_t(0.5f, -0.5f, -0.5f), color},
		{lwh * float3_t(-0.5f, -0.5f, -0.5f), color},
		{lwh * float3_t(-0.5f, 0.5f, -0.5f), color},
		{lwh * float3_t(-0.5f, -0.5f, -0.5f), color},
		{lwh * float3_t(-0.5f, -0.5f, 0.5f), color},

		{lwh * float3_t(0.5f, -0.5f, -0.5f), color},
		{lwh * float3_t(0.5f, 0.5f, -0.5f), color},
		{lwh * float3_t(0.5f, -0.5f, -0.5f), color},
		{lwh * float3_t(0.5f, -0.5f, 0.5f), color},

		{lwh * float3_t(-0.5f, -0.5f, 0.5f), color},
		{lwh * float3_t(0.5f, -0.5f, 0.5f), color},
		{lwh * float3_t(-0.5f, -0.5f, 0.5f), color},
		{lwh * float3_t(-0.5f, 0.5f, 0.5f), color},

		{lwh * float3_t(-0.5f, 0.5f, -0.5f), color},
		{lwh * float3_t(0.5f, 0.5f, -0.5f), color},
		{lwh * float3_t(-0.5f, 0.5f, -0.5f), color},
		{lwh * float3_t(-0.5f, 0.5f, 0.5f), color},

		{lwh * float3_t(0.5f, 0.5f, 0.5f), color},
		{lwh * float3_t(0.5f, 0.5f, -0.5f), color},
		{lwh * float3_t(0.5f, 0.5f, 0.5f), color},
		{lwh * float3_t(0.5f, -0.5f, 0.5f), color},
		{lwh * float3_t(0.5f, 0.5f, 0.5f), color},
		{lwh * float3_t(-0.5f, 0.5f, 0.5f), color}
	};
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, sizeof(l) / sizeof(vert) / 2, l, sizeof(vert));
}

void Editor::DrawHemiSphere(float3_t lwh, bool up, DWORD color)
{
	const int segs = 16;
	const int hsegs = 8;

	lwh = (float3)(lwh * 0.5f);
	Array<vert> l;
	l.reserve(segs * segs * 4);
	float deg1, deg2, hdeg1, hdeg2, ymod, xzmod;
	for(int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;
		for(int j = 0; j < hsegs; ++j)
		{
			hdeg1 = ((float)j / (float)hsegs) * SM_PIDIV2;
			hdeg2 = ((float)(j + 1) / (float)hsegs) * SM_PIDIV2;

			ymod = sinf(hdeg1);
			xzmod = cosf(hdeg1);

			l.push_back({lwh * float3_t(sinf(deg1) * xzmod, ymod * (up ? 1.0f : -1.0f), cosf(deg1) * xzmod), color});
			l.push_back({lwh * float3_t(sinf(deg2) * xzmod, ymod * (up ? 1.0f : -1.0f), cosf(deg2) * xzmod), color});

			l.push_back({lwh * float3_t(sinf(deg1) * xzmod, ymod * (up ? 1.0f : -1.0f), cosf(deg1) * xzmod), color});
			l.push_back({lwh * float3_t(sinf(deg1) * cosf(hdeg2), sinf(hdeg2) * (up ? 1.0f : -1.0f), cosf(deg1) * cosf(hdeg2)), color});
		}
	}
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, l.size() / 2, &l[0], sizeof(vert));
}

void Editor::DrawSphere(float3_t lwh, DWORD color)
{
	DrawHemiSphere(lwh, true, color);
	DrawHemiSphere(lwh, false, color);
}

void Editor::DrawCapsule(float3_t lwh, DWORD color)
{
	SMMATRIX mOld;
	m_pd3dDevice->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&mOld);

	m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixTranslation(0.0f, lwh.z * 0.5f, 0.0f) * mOld));
	DrawHemiSphere(float3_t(lwh.y, lwh.y, lwh.y), true, color);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixTranslation(0.0f, -lwh.z * 0.5f, 0.0f) * mOld));
	DrawHemiSphere(float3_t(lwh.y, lwh.y, lwh.y), false, color);

	m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mOld);

	DrawCylinder(float3_t(lwh.y, lwh.z, lwh.y), color);
}

void Editor::DrawCylinder(float3_t lwh, DWORD color)
{
	const int segs = 16;

	lwh = (float3)(lwh * 0.5f);
	Array<vert> l;
	l.reserve(segs * 6);
	float deg1, deg2;
	for(int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;

		l.push_back({lwh * float3_t(sinf(deg1), 1.0f, cosf(deg1)), color});
		l.push_back({lwh * float3_t(sinf(deg2), 1.0f, cosf(deg2)), color});

		l.push_back({lwh * float3_t(sinf(deg1), 1.0f, cosf(deg1)), color});
		l.push_back({lwh * float3_t(sinf(deg1), -1.0f, cosf(deg1)), color});

		l.push_back({lwh * float3_t(sinf(deg1), -1.0f, cosf(deg1)), color});
		l.push_back({lwh * float3_t(sinf(deg2), -1.0f, cosf(deg2)), color});
	}
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, l.size() / 2, &l[0], sizeof(vert));
}

void Editor::DrawHandlerMove()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	m_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DWORD color_act = 0xFFFFFF00;
	DWORD color_act2 = 0x1FFFFF00;
	
	float len = 20;
	vert l[] = {
		{float3_t(0, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(0, 0, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, 0, 0), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},

		{float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFFFF0000},
		{float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0x3FFF0000},
		{float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFFFF0000},
		{float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0x3FFF0000},

		{float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF00FF00},
		{float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0x3F00FF00},
		{float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFF00FF00},
		{float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0x3F00FF00},

		{float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF0000FF},
		{float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0x3F0000FF},
		{float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFF0000FF},
		{float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0x3F0000FF},
	};
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, sizeof(l) / sizeof(vert) / 2, l, sizeof(vert));

	float asize = 1.0f;
	float a2size = 3.0f;


	vert l2[] = {
		//arrow X
		{float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},

		{float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},

		//arrow Y
		{float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},

		{float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},

		//arrow Z
		{float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},

		{float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},


		//plane XY
		{float3_t(0, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00},
		{float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000},
		{float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00},
		{float3_t(0, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00},
		{float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00},
		{float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00},
		{float3_t(0, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00},
		{float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00},
		{float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000},
		{float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00},
		{float3_t(0, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00},
		{float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00},

		//plane XZ
		{float3_t(0, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF},
		{float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000},
		{float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF},
		{float3_t(0, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF},
		{float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF},
		{float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF},
		{float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF},
		{float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000},
		{float3_t(0, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF},
		{float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF},
		{float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF},

		//plane YZ
		{float3_t(0, 0, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF},
		{float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF},
		{float3_t(0, 0, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF},
		{float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF},
		{float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, 0, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF},
		{float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF},
		{float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, 0, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF},
		{float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF},


		/*
		//inv X

		{float3_t(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},

		//inv Y

		{float3_t(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},

		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},

		{float3_t(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},

		//inv Z

		{float3_t(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},

		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},

		{float3_t(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},*/

	};
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, sizeof(l2) / sizeof(vert) / 3, l2, sizeof(vert));

		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
}

void Editor::DrawHandlerScale()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	m_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DWORD color_act = 0xFFFFFF00;
	DWORD color_act2 = 0x1FFFFF00;

	float len = 20.0f;
	float len05 = len * 0.5f;
	float len075 = len * 0.75f;
	vert l[] = {
		{float3_t(0, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(0, 0, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, 0, 0), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},

		{float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFFFF0000},
		{float3_t(0, len05, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFF00FF00},
		{float3_t(len075, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFFFF0000},
		{float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFF00FF00},

		{float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFFFF0000},
		{float3_t(0, 0, len05), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFF0000FF},
		{float3_t(len075, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFFFF0000},
		{float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFF0000FF},

		{float3_t(0, 0, len05), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF0000FF},
		{float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF00FF00},
		{float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF0000FF},
		{float3_t(0, len075, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF00FF00},
	};
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, sizeof(l) / sizeof(vert) / 2, l, sizeof(vert));

	float asize = 1.0f;
	float a2size = 3.0f;


	vert l2[] = {
		//arrow X
		{float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},

		{float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},
		{float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000},

		//arrow Y
		{float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},

		{float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},
		{float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00},

		//arrow Z
		{float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},

		{float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},
		{float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF},


		//plane XY
		{float3_t(len075, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000},
		{float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000},
		{float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00},
		{float3_t(0, len05, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00},
		{float3_t(len075, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000},
		{float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000},
		{float3_t(0, len05, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00},
		{float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00},

		//plane XZ
		{float3_t(len075, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000},
		{float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000},
		{float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, 0, len05), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF},
		{float3_t(len075, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000},
		{float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000},
		{float3_t(0, 0, len05), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF},

		//plane YZ
		{float3_t(0, len075, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, 0, len05), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, len075, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF},
		{float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, 0, len05), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF},

		//plane XYZ
		{float3_t(0, len05, 0), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1F00FF00},
		{float3_t(0, 0, len05), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1F0000FF},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1FFF0000},
		{float3_t(0, len05, 0), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1F00FF00},
		{float3_t(len05, 0, 0), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1FFF0000},
		{float3_t(0, 0, len05), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1F0000FF},

		/*
		//inv X

		{float3_t(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},

		//inv Y

		{float3_t(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},

		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},

		{float3_t(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f), 0x1F00FFFF},

		//inv Z

		{float3_t(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},

		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},

		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},

		{float3_t(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},
		{float3_t(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f), 0x1F00FFFF},*/

	};
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, sizeof(l2) / sizeof(vert) / 3, l2, sizeof(vert));

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
}

void Editor::DrawHandlerRotate()
{
	DWORD color_act = 0xFFFFFF00;

	SMMATRIX mOld;
	m_pd3dDevice->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&mOld);

	DrawCylinder(float3_t(20.0f, 1.0f, 20.0f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixRotationZ(SM_PIDIV2) * mOld));
	DrawCylinder(float3_t(20.0f, 1.0f, 20.0f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixRotationX(SM_PIDIV2) * mOld));
	DrawCylinder(float3_t(20.0f, 1.0f, 20.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF);


	m_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mOld);
}

bool line_intersect_triangle(const float3 &t1, const float3 &t2, const float3 &t3,
	const float3 &l1, const float3 &l2, float3 &p)
{
	float3 n = SMVector3Normalize(SMVector3Cross((t2 - t1), (t3 - t2)));
	float d1 = SMVector3Dot((l1 - t1), n) / SMVector3Length(n), d2 = SMVector3Dot((l2 - t1), n) / SMVector3Length(n);
	if((d1 > 0 && d2 > 0) || (d1 < 0 && d2 < 0))
		return(false);
	p = l1 + (l2 - l1) * (-d1 / (d2 - d1));
	if(SMVector3Dot(SMVector3Cross((t2 - t1), (p - t1)), n) <= 0.0f) return(false);
	if(SMVector3Dot(SMVector3Cross((t3 - t2), (p - t2)), n) <= 0.0f) return(false);
	if(SMVector3Dot(SMVector3Cross((t1 - t3), (p - t3)), n) <= 0.0f) return(false);
	return(true);
}

void Editor::HandlerIntersectMove(const float3 & start, const float3 & dir)
{
	if(m_bIsDragging)
	{
		return;
	}
	float3 end = dir;
	float len = 20.0f;
	float asize = 1.0f;
	float a2size = 3.0f;

	m_currentAxe = HA_NONE;

	float3 p;

	float mind = FLOAT_INF;

	if(line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, 0), float3_t(len * 0.5f, len * 0.5f, 0), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, len * 0.5f, 0), float3_t(0, len * 0.5f, 0), start, end, p))
	{
		float d = SMVector3Length(p - m_cam.getPos());
		if(d < mind)
		{
			mind = d;
			m_currentAxe = HA_XY;
		}
	}

	if(line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, 0), float3_t(len * 0.5f, 0, len * 0.5f), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, len * 0.5f), float3_t(0, 0, len * 0.5f), start, end, p))
	{
		float d = SMVector3Length(p - m_cam.getPos());
		if(d < mind)
		{
			mind = d;
			m_currentAxe = HA_XZ;
		}
	}
	
	if(line_intersect_triangle(float3(0, 0, 0), float3(0, 0, len * 0.5f), float3_t(0, len * 0.5f, len * 0.5f), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(0, len * 0.5f, len * 0.5f), float3_t(0, len * 0.5f, 0), start, end, p))
	{
		float d = SMVector3Length(p - m_cam.getPos());
		if(d < mind)
		{
			mind = d;
			m_currentAxe = HA_YZ;
		}
	}


	float3 asX[] = {
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f),

		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),

		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),

		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
	};

	float3 asY[] = {
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f), 
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),

		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),

		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),

		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
	};

	float3 asZ[] = {
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),

		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),

		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),

		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
	};

	for(int i = 0, l = sizeof(asX) / sizeof(asX[0]); i < l; i += 3)
	{
		if(line_intersect_triangle(asX[i], asX[i + 1], asX[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_X;
				break;
			}
		}
	}
	for(int i = 0, l = sizeof(asY) / sizeof(asY[0]); i < l; i += 3)
	{
		if(line_intersect_triangle(asY[i], asY[i + 1], asY[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_Y;
				break;
			}
		}
	}
	for(int i = 0, l = sizeof(asZ) / sizeof(asZ[0]); i < l; i += 3)
	{
		if(line_intersect_triangle(asZ[i], asZ[i + 1], asZ[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_Z; 
				break;
			}
		}
	}
}

void Editor::HandlerIntersectRotate(const float3 & start, const float3 & dir)
{
	if(m_bIsDragging)
	{
		return;
	}
	float3 end = dir;
	float rad = 20.0f;
	float h = 1.0f;


	m_currentAxe = HA_NONE;

	float3 p;

	float mind = FLOAT_INF;

	const int segs = 16;

	float3_t lwh(rad, h, rad);

	lwh = (float3)(lwh * 0.5f);
	float deg1, deg2;

	/*
	1--2
	 \
	  \
	3--4
	*/

	//Y
	for(int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;

		
		float3_t p1(lwh * float3_t(sinf(deg1), 1.0f, cosf(deg1)));
		float3_t p2(lwh * float3_t(sinf(deg2), 1.0f, cosf(deg2)));

		float3_t p3(lwh * float3_t(sinf(deg1), -1.0f, cosf(deg1)));
		float3_t p4(lwh * float3_t(sinf(deg2), -1.0f, cosf(deg2)));
		if(line_intersect_triangle(p1, p2, p4, start, end, p)
			|| line_intersect_triangle(p1, p4, p3, start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_Y;
				break;
			}
		}
	}
	lwh = float3_t(lwh.y, lwh.x, lwh.z);
	//X
	for(int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;


		float3_t p1(lwh * float3_t(1.0f, sinf(deg1), cosf(deg1)));
		float3_t p2(lwh * float3_t(1.0f, sinf(deg2), cosf(deg2)));

		float3_t p3(lwh * float3_t(-1.0f, sinf(deg1), cosf(deg1)));
		float3_t p4(lwh * float3_t(-1.0f, sinf(deg2), cosf(deg2)));
		if(line_intersect_triangle(p1, p2, p4, start, end, p)
			|| line_intersect_triangle(p1, p4, p3, start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_X;
				break;
			}
		}
	}
	lwh = float3_t(lwh.y, lwh.z, lwh.x);
	//Z
	for(int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;


		float3_t p1(lwh * float3_t(sinf(deg1), cosf(deg1), 1.0f));
		float3_t p2(lwh * float3_t(sinf(deg2), cosf(deg2), 1.0f));

		float3_t p3(lwh * float3_t(sinf(deg1), cosf(deg1), -1.0f));
		float3_t p4(lwh * float3_t(sinf(deg2), cosf(deg2), -1.0f));
		if(line_intersect_triangle(p1, p2, p4, start, end, p)
			|| line_intersect_triangle(p1, p4, p3, start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_Z;
				break;
			}
		}
	}

}

void Editor::HandlerIntersectScale(const float3 & start, const float3 & dir)
{
	if(m_bIsDragging)
	{
		return;
	}
	float3 end = dir;
	float len = 20.0f;
	float len05 = len * 0.5f;
	float len075 = len * 0.75f;
	float asize = 1.0f;
	float a2size = 3.0f;

	m_currentAxe = HA_NONE;

	float3 p;

	float mind = FLOAT_INF;

	if(line_intersect_triangle(float3(len075, 0, 0), float3(0, len075, 0), float3_t(len05, 0, 0), start, end, p)
		|| line_intersect_triangle(float3(len05, 0, 0), float3(0, len075, 0), float3_t(0, len05, 0), start, end, p))
	{
		float d = SMVector3Length(p - m_cam.getPos());
		if(d < mind)
		{
			mind = d;
			m_currentAxe = HA_XY;
		}
	}

	if(line_intersect_triangle(float3(len075, 0, 0), float3(0, 0, len075), float3_t(len05, 0, 0), start, end, p)
		|| line_intersect_triangle(float3(len05, 0, 0), float3(0, 0, len075), float3_t(0, 0, len05), start, end, p))
	{
		float d = SMVector3Length(p - m_cam.getPos());
		if(d < mind)
		{
			mind = d;
			m_currentAxe = HA_XZ;
		}
	}

	if(line_intersect_triangle(float3(0, len075, 0), float3(0, 0, len075), float3_t(0, len05, 0), start, end, p)
		|| line_intersect_triangle(float3(0, len05, 0), float3(0, 0, len075), float3_t(0, 0, len05), start, end, p))
	{
		float d = SMVector3Length(p - m_cam.getPos());
		if(d < mind)
		{
			mind = d;
			m_currentAxe = HA_YZ;
		}
	}

	if(line_intersect_triangle(float3(0, len05, 0), float3(0, 0, len05), float3_t(len05, 0, 0), start, end, p))
	{
		float d = SMVector3Length(p - m_cam.getPos());
		if(d < mind)
		{
			mind = d;
			m_currentAxe = HA_XYZ;
		}
	}


	float3 asX[] = {
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f),

		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),

		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),

		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
	};

	float3 asY[] = {
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),

		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),

		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),

		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
	};

	float3 asZ[] = {
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),

		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),

		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),

		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
	};

	for(int i = 0, l = sizeof(asX) / sizeof(asX[0]); i < l; i += 3)
	{
		if(line_intersect_triangle(asX[i], asX[i + 1], asX[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_X;
				break;
			}
		}
	}
	for(int i = 0, l = sizeof(asY) / sizeof(asY[0]); i < l; i += 3)
	{
		if(line_intersect_triangle(asY[i], asY[i + 1], asY[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_Y;
				break;
			}
		}
	}
	for(int i = 0, l = sizeof(asZ) / sizeof(asZ[0]); i < l; i += 3)
	{
		if(line_intersect_triangle(asZ[i], asZ[i + 1], asZ[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - m_cam.getPos());
			if(d < mind)
			{
				mind = d;
				m_currentAxe = HA_Z;
				break;
			}
		}
	}
}

void Editor::OnMouseDown(int x, int y)
{
	float f; 
	if(m_vHitboxes.size() > m_iActiveHitbox && ((TabHitboxes*)m_pTM->m_pTabHitboxes)->m_bShown)
	{
		HitboxItem * hbi = &m_vHitboxes[m_iActiveHitbox];
		SMMATRIX mBone = (hbi->hb->bone[0] ? m_pCurAnim->getBoneTransform(m_pCurAnim->getBone(hbi->hb->bone)) : SMMatrixIdentity());

		SMMATRIX revMat = SMMatrixInverse(&f, (m_bIsDragging && !m_bIsDraggingStart ? m_mOldDragMat : m_mHelperMat) * mBone * m_mViewMat * m_mProjMat);

		D3DVIEWPORT9 vp;
		m_pd3dDevice->GetViewport(&vp);

		float px = (((2.0f*x) / vp.Width) - 1.0f) /* / m_mProjMat._11 */;
		float py = (((-2.0f*y) / vp.Height) + 1.0f) /* / m_mProjMat._22 */;

		float3 pos(px, py, -1);
		float3 dir(px, py, 1);
		pos = SMVector3Transform(pos, revMat);
		dir = SMVector3Transform(dir, revMat);
		pos /= pos.w;
		dir /= dir.w;

		//dir -= pos;
		switch(m_htype)
		{
		case HT_MOVE:
			HandlerIntersectMove(pos, dir);
			break;

		case HT_ROTATE:
			HandlerIntersectRotate(pos, dir);
			break;

		case HT_SCALE:
			HandlerIntersectScale(pos, dir);
			break;
		}

		if(m_bIsDraggingStart)
		{
			m_bIsDraggingStart = false;
			m_fStartDragPos = pos;
			m_mOldDragMat = m_mHelperMat;
			if(m_vHitboxes.size() > m_iActiveHitbox)
			{
				HitboxItem * hbi = &m_vHitboxes[m_iActiveHitbox];

				m_mHitboxMat = m_mOldHitboxMat = SMMatrixRotationX(hbi->hb->rot.x)
					* SMMatrixRotationY(hbi->hb->rot.y)
					* SMMatrixRotationZ(hbi->hb->rot.z)
					* SMMatrixTranslation(hbi->hb->pos)
					//* (hbi->hb->bone[0] ? m_pCurAnim->getBoneTransform(m_pCurAnim->getBone(hbi->hb->bone)) : SMMatrixIdentity())
					;
				m_fOldHitboxLWH = hbi->hb->lwh;
			}
		}

		if(m_bIsDragging)
		{
			float3 dv = (pos - m_fStartDragPos) * (1000.0f + SMVector3Length(m_cam.getPos() - float3(m_mHelperMat._41, m_mHelperMat._42, m_mHelperMat._43)) * 10.0f);
			SMMATRIX m_res;
			TabHitboxes * tab = (TabHitboxes*)m_pEditor->m_pTM->m_pTabHitboxes;
			char tmp[64];
			if(m_vHitboxes.size() > m_iActiveHitbox)
			{
				HitboxItem * hbi = &m_vHitboxes[m_iActiveHitbox];

				switch(m_htype)
				{
				case HT_MOVE:
					m_res = SMMatrixTranslation((m_currentAxe & HA_X) ? dv.x : 0, (m_currentAxe & HA_Y) ? dv.y : 0, (m_currentAxe & HA_Z) ? dv.z : 0);
					m_mHitboxMat = m_mOldHitboxMat * m_res;
					m_res = m_mOldDragMat * m_res;
					break;

				case HT_ROTATE:
					m_res = SMMatrixRotationX((m_currentAxe & HA_X) ? dv.x : 0) * SMMatrixRotationY((m_currentAxe & HA_Y) ? dv.y : 0) * SMMatrixRotationZ((m_currentAxe & HA_Z) ? dv.z : 0);
					m_mHitboxMat = m_res * m_mOldHitboxMat;
					m_res = m_res * m_mOldDragMat;
					break;

				case HT_SCALE:
					dv /= 100.0f;
					dv += float3(1.0f, 1.0f, 1.0f);
					if((m_currentAxe & HA_XY) == HA_XY)
					{
						dv.x = dv.z = dv.y;
					}
					else if((m_currentAxe & HA_YZ) == HA_YZ)
					{
						dv.y = dv.z;
					}
					else if((m_currentAxe & HA_XZ) == HA_XZ)
					{
						dv.x = dv.z;
					}
					m_res = SMMatrixScaling((m_currentAxe & HA_X) ? dv.x : 1, (m_currentAxe & HA_Y) ? dv.y : 1, (m_currentAxe & HA_Z) ? dv.z : 1);
					if(m_vHitboxes.size() > m_iActiveHitbox)
					{
						HitboxItem * hbi = &m_vHitboxes[m_iActiveHitbox];
						float3 vec = SMVector3Transform(m_fOldHitboxLWH, m_res);

						sprintf(tmp, "%f", vec.x); tab->EdL->SetText(tmp);
						sprintf(tmp, "%f", vec.y); tab->EdW->SetText(tmp);
						sprintf(tmp, "%f", vec.z); tab->EdH->SetText(tmp);
					}
					m_res = m_res * m_mOldDragMat;
					break;
				}
				SMMATRIX mBone = (hbi->hb->bone[0] ? m_pCurAnim->getBoneTransform(m_pCurAnim->getBone(hbi->hb->bone)) : SMMatrixIdentity());


				m_mHelperMat = m_res;/* * SMMatrixTranslation(float3(mBone._41, mBone._42, mBone._43));
				m_mHitboxMat *= SMMatrixTranslation(float3(mBone._41, mBone._42, mBone._43));*/

				sprintf(tmp, "%f", m_mHitboxMat._41); tab->EdPosX->SetText(tmp);
				sprintf(tmp, "%f", m_mHitboxMat._42); tab->EdPosY->SetText(tmp);
				sprintf(tmp, "%f", m_mHitboxMat._43); tab->EdPosZ->SetText(tmp);

				float3 rot = SMMatrixToEuler(m_mHitboxMat);
				sprintf(tmp, "%f", SMToAngle(-rot.x)); tab->EdRotX->SetText(tmp);
				sprintf(tmp, "%f", SMToAngle(rot.y)); tab->EdRotY->SetText(tmp);
				sprintf(tmp, "%f", SMToAngle(-rot.z)); tab->EdRotZ->SetText(tmp);

			}
		}
		if(m_bIsDraggingStop)
		{
			m_bIsDraggingStop = false;
			if(m_htype == HT_SCALE)
			{
				m_mHelperMat._11 = 1.0f;
				m_mHelperMat._12 = 0.0f;
				m_mHelperMat._13 = 0.0f;

				m_mHelperMat._21 = 0.0f;
				m_mHelperMat._22 = 1.0f;
				m_mHelperMat._23 = 0.0f;

				m_mHelperMat._31 = 0.0f;
				m_mHelperMat._32 = 0.0f;
				m_mHelperMat._33 = 1.0f;
			}
		}
	}
}
