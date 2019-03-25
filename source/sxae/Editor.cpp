
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

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

//report_func g_fnReportf = SkyXEngine_PrintfLog;// msgbx;

extern report_func g_fnReportf;

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

	SkyXEngine_InitOutLog();
	SkyXEngine_InitPaths();

	InitD3D();

	Core_0Create("sxcore", "GenPreview", false);
	Core_Dbg_Set(SkyXEngine_PrintfLog);
	Core_SetOutPtr();

	m_pvActivities = &((TabActivities*)m_pTM->m_pTabActivities)->m_vItems;

	m_pEditor = this;
	m_szAnimFilter[0] = 0;
	m_szEditFile[0] = 0;

	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlGB->addHandler(AnimTBProc, WM_HSCROLL);
	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgressTrack->addHandler(AnimTBProc, WM_SETFOCUS);
	//((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgress->addHandler(AnimTBProc, WM_SETFOCUS);
	((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgressTrack->addHandler(AnimTBProc, WM_KILLFOCUS);
	//((TabAnimation*)m_pTM->m_pTabAnimation)->AnimCtlProgress->addHandler(AnimTBProc, WM_KILLFOCUS);


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

HWND Editor::getRenderHWND()
{
	if (D3DWindow)
		return D3DWindow->getHWND();

	return 0;
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
	Editor * edt = (Editor*)cmp->getUserPtr();
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
				
				m_pEditor->m_vAnims[m_pEditor->m_iCurIdx].seq->activity = pList->getSel();
			}
			break;

		case IDC_ATTACH_RB_BONE:
			{
				TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
				tab->AttachBone->setEnable(1);
			}
			break;

		case IDC_ATTACH_RB_SKIN:
			{
				TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
				tab->AttachBone->setEnable(0);
			}
			break;

		case IDC_PT_REN:
			{
				ISXGUIListBox * pList = ((TabAttachments*)edt->m_pTM->m_pTabAttachments)->AttachmentsList;
				int len = MODEL_MAX_NAME;
				int sel = pList->getSel();
				if(sel < 0)
				{
					break;
				}
				sel = pList->getItemData(sel);
				
				Tools::DlgPrompt(edt->m_vMdlParts[sel]->name, &len, "New name", "Rename", edt->m_vMdlParts[sel]->name);
				edt->RenderPartList();
			}
			break;

		case IDC_PT_DEL:
			{
				ISXGUIListBox * pList = ((TabAttachments*)edt->m_pTM->m_pTabAttachments)->AttachmentsList;
				int len = MODEL_MAX_NAME;
				int sel = pList->getSel();
				if(sel < 0)
				{
					break;
				}
				sel = pList->getItemData(sel);

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
				pList->setSel(pList->getItemCount() - 1);
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
				int sel = pList->getSel();
				if(sel < 0)
				{
					break;
				}
				sel = pList->getItemData(sel);

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
				pList->setSel(pList->getItemCount() - 1);
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
				int sel = pList->getSel();
				if(sel < 0 || (sel = pList->getItemData(sel)) < 0)
				{
					break;
				}
				
				HitboxItem * hbx = &m_pEditor->m_vHitboxes[sel];
				
				int len = GetWindowTextLengthA((HWND)lParam);

				GetWindowTextA((HWND)lParam, hbx->hb->name, MODEL_MAX_NAME);
				char tmpSN[MODEL_MAX_NAME + 32];
				sprintf(tmpSN, "[%c] %s", hbx->isImported ? 'I' : '_', hbx->hb->name);
				pList->setItemText(pList->getSel(), tmpSN);
				//m_pEditor->RenderHitboxList();
			}
			break;

		case IDC_HB_BONE:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				int sel = pList->getSel();
				if(sel < 0 || (sel = pList->getItemData(sel)) < 0)
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
				int sel = pList->getSel();
				if(sel < 0 || (sel = pList->getItemData(sel)) < 0)
				{
					break;
				}
				HitboxItem * hbx = &m_pEditor->m_vHitboxes[sel];
				hbx->hb->part = (HITBOX_BODYPART)cmb->getItemData(cmb->getSel());
			}
			break;

		case IDC_HB_TYPE:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				ISXGUIListBox * pList = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->HBList;
				ISXGUIComboBox * cmb = ((TabHitboxes*)edt->m_pTM->m_pTabHitboxes)->CBType;
				int sel = pList->getSel();
				if(sel < 0 || (sel = pList->getItemData(sel)) < 0)
				{
					break;
				}
				HitboxItem * hbx = &m_pEditor->m_vHitboxes[sel];
				hbx->hb->type = (HITBOX_TYPE)cmb->getItemData(cmb->getSel());
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
				int sel = pList->getSel();
				if(sel < 0 || (sel = pList->getItemData(sel)) < 0)
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
		tAnim->AnimPropActCmb->clear();
		tAnim->AnimPropActCmb->addItem("");
		for(int i = 0, l = edt->m_pvActivities->size(); i < l; ++i)
		{
			tAnim->AnimPropActCmb->addItem(edt->m_pvActivities[0][i].act.c_str());
		}
		break;
	}
	return(0);
}

LRESULT Editor::CamInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	Editor * edt = (Editor*)cmp->getUserPtr();
	switch(msg)
	{
	case WM_RBUTTONDOWN:
		edt->CenterMouse();
		edt->m_bCamMove = true;
		ShowCursor(0);
		SetFocus(edt->MainWindow->getHWND());
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
	Editor * edt = (Editor*)cmp->getUserPtr();
	ISXGUIListBox * lb = edt->AnimList;
	int curSel = lb->getSel();
	switch(msg)
	{
	case WM_LBUTTONDBLCLK:
		if(curSel != -1)
		{
			UINT seqi = (UINT)lb->getItemData(curSel);
			edt->m_pCurAnim->play(edt->m_vAnims[seqi].seq->name, 100);
		}
		break;
	}

	return(CallWindowProc(cmp->getPrevWndProc(), hwnd, msg, wParam, lParam));
}

LRESULT Editor::AnimFilterCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYUP:
		m_pEditor->AnimFilter->getText(m_pEditor->m_szAnimFilter, sizeof(m_pEditor->m_szAnimFilter) - 1);
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
		m_pEditor->CurAnimName->setText("");
		ta->AnimCtlPauseBtn->setEnable(0);
		ta->AnimCtlPlayBtn->setEnable(1);
		break;
	case AS_PLAY:
		char name[MODEL_MAX_NAME + 16];
		sprintf(name, "Animation: %s", seq->name);
		m_pEditor->CurAnimName->setText(name);

		ta->AnimCtlPauseBtn->setEnable(1);
		ta->AnimCtlPlayBtn->setEnable(0);
		break;
	}
	
}
void Editor::AnimProgressCB(int slot, float progress, Animation * pAnim)
{
	TabAnimation * ta = (TabAnimation*)m_pEditor->m_pTM->m_pTabAnimation;
	ta->AnimCtlProgress->setText(String(progress).c_str());

	ta->AnimCtlProgressTrack->setPos((int)(progress * 1000));
}

LRESULT Editor::AnimTBProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int pos;
	switch(msg)
	{
	case WM_HSCROLL:
		pos = ((TabAnimation*)m_pEditor->m_pTM->m_pTabAnimation)->AnimCtlProgressTrack->getPos();
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
	//OPENFILENAMEA ofn;
	char szFile[260];

	if(m_bDirty && MessageBoxA(hwnd, "All unsaved changes will be lost", "Open?", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
	{
		return;
	}

	//ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ZeroMemory(szFile, sizeof(szFile));
	/*ofn.lStructSize = sizeof(OPENFILENAMEA);
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
	GetCurrentDirectoryW(256, bf);*/

	if (gui_func::dialogs::SelectFileOwn(0, szFile, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS), "dse", "Open file", true, Core_RStringGet(G_RI_STRING_PATH_GS_MODELS), MainWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo)/*GetOpenFileNameA(&ofn) == TRUE*/)
	{
		//SetCurrentDirectoryW(bf);
		//strcpy(m_szEditFile, ofn.lpstrFile);
		//wprintf(L"File: %s\n", ofn.lpstrFile);

		//unload all parts
		while(m_pEditor->m_vMdlParts.size())
		{
			m_pEditor->delModel(0);
		}
		m_pEditor->addModel(szFile, MI_ALL, false, true);
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
			tab->AttachFileField->setText(ofn.lpstrFile);
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
	MainWindow = SXGUICrBaseWndEx("MainWindow", "MainWindow", CW_USEDEFAULT, CW_USEDEFAULT, 1320, 730, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	gui_func::base_handlers::InitHandlerMsg(MainWindow);
	MainWindow->addHandler(MenuCmd, WM_COMMAND);
	MainWindow->addHandler(MenuCmd, WM_CLOSE, 0, 0, 0, 0, 1);
	MainWindow->addHandler(MenuCmd, WM_PARENTNOTIFY);
	MainWindow->addHandler(MenuCmd, EM_LOADACTIVITIES);
	//MainWindow->addHandler(MenuCmd, WM_KEYDOWN);
	MainWindow->setMixSize(MAINWIN_SIZE_X, MAINWIN_SIZE_Y);


	MainWindow->setUserPtr(this);

	Menu = SXGUICrMenuWindowEx(IDR_MENU1);
	Menu->setToWindow(MainWindow->getHWND());

	D3DWindow = SXGUICrBaseWndEx("Window1", "Window1", 279, 6, 1023, 473, 0, 0, CreateSolidBrush(RGB(200, 200, 200)), 0, CS_HREDRAW | CS_VREDRAW, WS_CHILD | WS_VISIBLE | WS_BORDER, MainWindow->getHWND(), 0);
	D3DWindow->setFollowParentSides(true, true, true, true);
	D3DWindow->setUserPtr(this);

	D3DWindow->addHandler(CamInput, WM_RBUTTONDOWN);
	D3DWindow->addHandler(CamInput, WM_RBUTTONUP);
	MainWindow->addHandler(CamInput, WM_KEYDOWN);
	MainWindow->addHandler(CamInput, WM_KEYUP);
	D3DWindow->addHandler(CamInput, WM_LBUTTONDOWN);
	D3DWindow->addHandler(CamInput, WM_LBUTTONUP);
	D3DWindow->addHandler(CamInput, WM_MOUSEMOVE);

	m_pTM = new TabManager(MainWindow);

	AnimationsGB = SXGUICrGroupBox("Animations", 3, 0, 275, 669, MainWindow->getHWND(), 0, 0);
	AnimationsGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimationsGB->setColorText(RGB(0, 0, 0));
	AnimationsGB->setColorTextBk(RGB(255, 255, 255));
	AnimationsGB->setTransparentTextBk(true);
	AnimationsGB->setColorBrush(RGB(255, 255, 255));
	AnimationsGB->setFollowParentSides(true, true, true, false);
	AnimationsGB->setUserPtr(this);
	AnimationsGB->addHandler(AnimGBProc, WM_COMMAND);

	AnimList = SXGUICrListBoxEx(2, 43, 270, 623, 0, WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | LBS_NOTIFY/* | LBS_SORT*/, AnimationsGB->getHWND(), 0, IDC_LISTBOX);
	AnimList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimList->setColorText(RGB(0, 0, 0));
	AnimList->setColorTextBk(RGB(255, 255, 255));
	AnimList->setTransparentTextBk(true);
	AnimList->setColorBrush(RGB(255, 255, 255));
	AnimList->setFollowParentSides(true, true, true, true);
	//AnimList->setUserPtr(this);
	//AnimList->addHandler(AnimListCB, WM_LBUTTONDBLCLK);

	AnimFilter = SXGUICrEdit("", 44, 16, 228, 23, AnimationsGB->getHWND(), 0, 0);
	AnimFilter->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimFilter->setColorText(RGB(0, 0, 0));
	AnimFilter->setColorTextBk(RGB(255, 255, 255));
	AnimFilter->setTransparentTextBk(true);
	AnimFilter->setColorBrush(RGB(255, 255, 255));

	AnimFilter->addHandler(AnimFilterCB, WM_KEYUP);

	Static1 = SXGUICrStatic("Filter:", 8, 18, 32, 20, AnimationsGB->getHWND(), 0, 0);
	Static1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static1->setColorText(RGB(0, 0, 0));
	Static1->setColorTextBk(RGB(255, 255, 255));
	Static1->setTransparentTextBk(true);
	Static1->setColorBrush(RGB(255, 255, 255));


	CurAnimName = SXGUICrStatic("Animation: idle", 1149, 649, 145, 19, MainWindow->getHWND(), 0, 0);
	CurAnimName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CurAnimName->setColorText(RGB(0, 0, 0));
	CurAnimName->setColorTextBk(RGB(255, 255, 255));
	CurAnimName->setTransparentTextBk(true);
	CurAnimName->setColorBrush(RGB(255, 255, 255));
	CurAnimName->setFollowParentSides(false, false, true, true);


	MainWindow->setVisible(true);
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
	HWND hWnd = D3DWindow->getHWND();
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = m_uWidth = rc.right - rc.left;
	UINT height = m_uHeight = rc.bottom - rc.top;
	
	SGCore_0Create("SXAnimEditor", hWnd, width, height, 1, 0, true);
	SGCore_Dbg_Set(SkyXEngine_PrintfLog/*msgbx*/);

	/*char tmp[260];

	sprintf(tmp, "%stextures/", m_szGamesourceDir);
	//SGCore_LoadTexStdPath(tmp);
	sprintf(tmp, "%sshaders/", m_szGamesourceDir);
	SGCore_ShaderSetStdPath(tmp);*/

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

	m_pVSH = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs");
	m_pPSH = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps");

	m_pVSHs = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs");
	m_pPSHs = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps");

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
	ClientToScreen(D3DWindow->getHWND(), &p);
	SetCursorPos(m_uWidth / 2 + p.x, m_uHeight / 2 + p.y);
}

void Editor::Update()
{
	SGCore_ShaderAllLoad();
	SGCore_LoadTexAllLoad();
	if(m_bCamMove)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(D3DWindow->getHWND(), &pt);
		
		float sens = 0.003f;
		m_cam.Rotate(((int)m_uWidth / 2 - pt.x) * sens, ((int)m_uHeight / 2 - pt.y) * sens);
		if((m_uWidth / 2 - pt.x) || (m_uHeight / 2 - pt.y))
		{
			CenterMouse();
		}
		m_cam.advance();
	}
	m_mViewMat = m_cam.GetMatrix();

	m_pd3dDevice->beginFrame();

	m_pd3dDevice->setClearColor(float4_t(0.0625f, 0.125f, 0.1875f, 1.0f));
	m_pd3dDevice->clearTarget();
	m_pd3dDevice->clearDepth(1.0f);
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
	m_pd3dDevice->endFrame();

	m_pAnimMgr->update();
	m_pAnimMgr->sync();

	m_pd3dDevice->swapBuffers();
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
	int cur = AnimList->getSel();
	if(cur < 0)
	{
		cur = 0;
	}
	AnimList->clear();
	for(UINT i = 0; i < c; ++i)
	{
		ai = &m_vAnims[i];
		if(!filt || filterStr(ai->seq->name, m_szAnimFilter))
		{
			sprintf(tmpSN, "[%c%c] %s", ai->isImported ? 'I' : '_', ai->seq->bLooped ? 'L' : '_', ai->seq->name); //I|_, L|_
			AnimList->addItem(tmpSN);
			AnimList->setItemData(AnimList->getItemCount() - 1, (LPARAM)i);
		}
	}
	AnimList->setSel(cur);
}

void Editor::RenderBoneList()
{
	ISXGUIComboBox * cmb = ((TabAttachments*)(m_pTM->m_pTabAttachments))->AttachBone;
	ISXGUIComboBox * cmb_2 = ((TabHitboxes*)(m_pTM->m_pTabHitboxes))->CBBone;
	int sel = cmb->getSel();
	char * text = (char*)alloca(sizeof(char) * (cmb->getItemTextLength(sel) + 1));
	cmb->getItemText(sel, text);

	int sel2 = cmb_2->getSel();
	char * text2 = (char*)alloca(sizeof(char)* (cmb_2->getItemTextLength(sel2) + 1));
	cmb_2->getItemText(sel2, text2);

	char tmp[MODEL_BONE_MAX_NAME + 1];
	cmb->clear();
	cmb_2->clear();
	cmb_2->addItem("");
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
		cmb->addItem(tmp);
		cmb_2->addItem(tmp);
	}

	cmb->setSel(sel);
	cmb_2->setSel(sel2);

	/*UINT c = m_vAnims.size();
	AnimItem * ai;
	char tmpSN[MODEL_MAX_NAME + 32];
	bool filt = m_szAnimFilter[0] != 0;
	int cur = AnimList->getSel();
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
			AnimList->SetItemData(AnimList->getItemCount() - 1, (LPARAM)i);
		}
	}
	AnimList->setSel(cur);*/
}

void Editor::RenderPartList()
{
	UINT c = m_vMdlParts.size();
	ModelPart * par;
	//char tmpSN[MODEL_MAX_NAME + 32];

	ISXGUIListBox * pList = ((TabAttachments*)m_pTM->m_pTabAttachments)->AttachmentsList;
	
	int cur = pList->getSel();
	if(cur < 0)
	{
		cur = 0;
	}
	pList->clear();

	for(UINT i = 0; i < c; ++i)
	{
		par = m_vMdlParts[i];
	
		//sprintf(tmpSN, "[%c%c] %s", ai->isImported ? 'I' : '_', ai->seq->bLooped ? 'L' : '_', ai->seq->name); //I|_, L|_
		if(par->file[0] != '!')
		{
			pList->addItem(par->name);
			pList->setItemData(pList->getItemCount() - 1, (LPARAM)i);
		}
		
	}
	pList->setSel(cur);
	OnPartListSelChg();
}

LRESULT Editor::AnimGBProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	Editor * self = (Editor*)cmp->getUserPtr();

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
	int sel = AnimList->getSel();
	AnimItem * item;
	int idx = AnimList->getItemData(sel);
	TabAnimation * tab = (TabAnimation*)m_pEditor->m_pTM->m_pTabAnimation;
	if(idx < 0)
	{
		tab->AnimPropActChance->setEnable(0);
		tab->AnimPropName->setEnable(0);
		tab->AnimPropLoopCB->setEnable(0);
		tab->AnimPropSpeed->setEnable(0);
		tab->AnimPropActCmb->setEnable(0);
		tab->AnimCtlPlayBtn->setEnable(0);
		tab->AnimCtlPauseBtn->setEnable(0);
	}
	else
	{
		item = &m_vAnims[idx];
		m_iCurIdx = idx;

		m_pCurAnim->play(item->seq->name, 100);

		tab->AnimPropActCmb->setSel(item->seq->activity);
		tab->AnimPropActChance->setText(String((DWORD)item->seq->act_chance).c_str());
		tab->AnimPropName->setText(item->seq->name);
		tab->AnimPropLoopCB->setCheck(item->seq->bLooped);
		tab->AnimPropSpeed->setText(String(item->seq->framerate).c_str());

		tab->AnimPropActChance->setEnable(!item->isImported);
		tab->AnimPropName->setEnable(!item->isImported);
		tab->AnimPropLoopCB->setEnable(!item->isImported);
		tab->AnimPropSpeed->setEnable(!item->isImported);
		tab->AnimPropActCmb->setEnable(!item->isImported);
	}
}

void Editor::SetPartFlag(MODEL_PART_FLAGS f, byte v)
{
	TabAttachments * tab = (TabAttachments*)m_pEditor->m_pTM->m_pTabAttachments;
	ISXGUIListBox * pList = tab->AttachmentsList;
	int sel = pList->getSel();
	ModelPart * pt;
	int idx = pList->getItemData(sel);
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
	int sel = pList->getSel();
	ModelPart * pt;
	int idx = pList->getItemData(sel);
	if(idx < 0)
	{
		tab->AttachSkinRB->setEnable(0);
		tab->AttachBoneRB->setEnable(0);
		tab->AttachBone->setEnable(0);

		tab->AttachFileBrowse->setEnable(0);
		tab->AttachFileField->setEnable(0);

		tab->AttachHideCB->setEnable(0);
		tab->AttachEnaCollisionCB->setEnable(0);
		tab->AttachEnaRaytraceCB->setEnable(0);

		tab->AttachXshift->setEnable(0);
		tab->AttachXshiftSpin->setEnable(0);
		tab->AttachYshift->setEnable(0);
		tab->AttachYshiftSpin->setEnable(0);
		tab->AttachZshift->setEnable(0);
		tab->AttachZshiftSpin->setEnable(0);

		tab->AttachFileApply->setEnable(0);
	}
	else
	{
		pt = m_vMdlParts[idx];

		tab->AttachSkinRB->setEnable(1);
		tab->AttachBoneRB->setEnable(1);

		switch(pt->attachDesc.type)
		{
		case MA_SKIN:
			tab->AttachSkinRB->setCheck(1);
			tab->AttachBoneRB->setCheck(0);
			tab->AttachBone->setEnable(0);
			break;
		case MA_BONE:
			tab->AttachBoneRB->setCheck(1);	
			tab->AttachSkinRB->setCheck(0);
			tab->AttachBone->setEnable(1);
			tab->AttachBone->setSel(-1);
			{
				char tmpBone[MODEL_BONE_MAX_NAME + 1];
				for(int i = 0, l = tab->AttachBone->getCount(); i < l; ++i)
				{
					tab->AttachBone->getItemText(i, tmpBone);
					if(!strcmp(pt->attachDesc.szBone, tmpBone))
					{
						tab->AttachBone->setSel(i);
						break;
					}
				}
			}
			break;
		}

		tab->AttachFileApply->setEnable(1);

		tab->AttachHideCB->setEnable(1);
		tab->AttachHideCB->setCheck(pt->uFlags & MP_HIDDEN);

		tab->AttachEnaCollisionCB->setEnable(1);
		tab->AttachEnaCollisionCB->setCheck(pt->uFlags & MP_COLLISIONS);

		tab->AttachEnaRaytraceCB->setEnable(1);
		tab->AttachEnaRaytraceCB->setCheck(pt->uFlags & MP_RAYTRACE);

		tab->AttachFileBrowse->setEnable(1);
		tab->AttachFileField->setEnable(1);

		tab->AttachFileField->setText(pt->file);

		tab->AttachXshift->setEnable(0);
		tab->AttachXshiftSpin->setEnable(0);
		tab->AttachYshift->setEnable(0);
		tab->AttachYshiftSpin->setEnable(0);
		tab->AttachZshift->setEnable(0);
		tab->AttachZshiftSpin->setEnable(0);
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
	int sel = pList->getSel();
	ModelPart * pt;
	int idx = pList->getItemData(sel);
	if(idx < 0)
	{
		return;
	}
	pt = m_vMdlParts[idx];

	MODEL_ATTACH ma = tab->AttachBoneRB->getCheck() ? MA_BONE : MA_SKIN;

	if(ma == MA_BONE)
	{
		int sel = tab->AttachBone->getSel();
		if(sel < 0)
		{
			MessageBoxA(NULL, "Please select bone", "Error!", MB_OK | MB_ICONSTOP);
			return;
		}
		tab->AttachBone->getItemText(sel, pt->attachDesc.szBone);
	}
	pt->attachDesc.type = ma;

	char szFile[MODEL_MAX_FILE];
	tab->AttachFileField->getText(szFile, sizeof(szFile));

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


	int cur = pList->getSel();
	if(cur < 0)
	{
		cur = 0;
	}
	pList->clear();

	for(UINT i = 0; i < c; ++i)
	{
		hbi = &m_vHitboxes[i];
			
		sprintf(tmpSN, "[%c] %s", hbi->isImported ? 'I' : '_', hbi->hb->name);
		pList->addItem(tmpSN);
		pList->setItemData(pList->getItemCount() - 1, (LPARAM)i);
	}
	pList->setSel(cur);

	OnHitboxListSelChg();
}

void Editor::OnHitboxListSelChg()
{
	TabHitboxes * tab = (TabHitboxes*)m_pEditor->m_pTM->m_pTabHitboxes;
	ISXGUIListBox * pList = tab->HBList;

	int sel = pList->getSel();
	HitboxItem * hbx;
	int idx = pList->getItemData(sel);
	if(idx < 0)
	{
		tab->CBBodyPart->setEnable(0);
		tab->CBBone->setEnable(0);
		tab->CBType->setEnable(0);

		tab->EdL->setEnable(0);
		tab->EdW->setEnable(0);
		tab->EdH->setEnable(0);

		tab->EdName->setEnable(0);

		tab->EdPosX->setEnable(0);
		tab->EdPosY->setEnable(0);
		tab->EdPosZ->setEnable(0);

		tab->EdRotX->setEnable(0);
		tab->EdRotY->setEnable(0);
		tab->EdRotZ->setEnable(0);

		tab->BtnDel->setEnable(0);
	}
	else
	{
		hbx = &m_vHitboxes[idx];
		m_iActiveHitbox = idx;
		tab->CBBodyPart->setEnable(!hbx->isImported);
		tab->CBBone->setEnable(!hbx->isImported);
		tab->CBType->setEnable(!hbx->isImported);

		tab->EdL->setEnable(!hbx->isImported);
		tab->EdW->setEnable(!hbx->isImported);
		tab->EdH->setEnable(!hbx->isImported);

		tab->EdName->setEnable(!hbx->isImported);

		tab->EdPosX->setEnable(!hbx->isImported);
		tab->EdPosY->setEnable(!hbx->isImported);
		tab->EdPosZ->setEnable(!hbx->isImported);

		tab->EdRotX->setEnable(!hbx->isImported);
		tab->EdRotY->setEnable(!hbx->isImported);
		tab->EdRotZ->setEnable(!hbx->isImported);

		tab->BtnDel->setEnable(1);

		tab->EdName->setText(hbx->hb->name);

		tab->CBBodyPart->setSel(hbx->hb->part);
		tab->CBType->setSel(hbx->hb->type);
		char tmp[MODEL_BONE_MAX_NAME];
		for(int i = 0, l = tab->CBBone->getCount(); i < l; ++i)
		{
			tab->CBBone->getItemText(i, tmp);
			if(!strcmp(tmp, hbx->hb->bone))
			{
				tab->CBBone->setSel(i);
			}
		}

		sprintf(tmp, "%f", hbx->hb->lwh.x); tab->EdL->setText(tmp);
		sprintf(tmp, "%f", hbx->hb->lwh.y); tab->EdW->setText(tmp);
		sprintf(tmp, "%f", hbx->hb->lwh.z); tab->EdH->setText(tmp);

		sprintf(tmp, "%f", hbx->hb->pos.x); tab->EdPosX->setText(tmp);
		sprintf(tmp, "%f", hbx->hb->pos.y); tab->EdPosY->setText(tmp);
		sprintf(tmp, "%f", hbx->hb->pos.z); tab->EdPosZ->setText(tmp);

		sprintf(tmp, "%f", SMToAngle(hbx->hb->rot.x)); tab->EdRotX->setText(tmp);
		sprintf(tmp, "%f", SMToAngle(hbx->hb->rot.y)); tab->EdRotY->setText(tmp);
		sprintf(tmp, "%f", SMToAngle(hbx->hb->rot.z)); tab->EdRotZ->setText(tmp);
		
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

						sprintf(tmp, "%f", vec.x); tab->EdL->setText(tmp);
						sprintf(tmp, "%f", vec.y); tab->EdW->setText(tmp);
						sprintf(tmp, "%f", vec.z); tab->EdH->setText(tmp);
					}
					m_res = m_res * m_mOldDragMat;
					break;
				}
				SMMATRIX mBone = (hbi->hb->bone[0] ? m_pCurAnim->getBoneTransform(m_pCurAnim->getBone(hbi->hb->bone)) : SMMatrixIdentity());


				m_mHelperMat = m_res;/* * SMMatrixTranslation(float3(mBone._41, mBone._42, mBone._43));
				m_mHitboxMat *= SMMatrixTranslation(float3(mBone._41, mBone._42, mBone._43));*/

				sprintf(tmp, "%f", m_mHitboxMat._41); tab->EdPosX->setText(tmp);
				sprintf(tmp, "%f", m_mHitboxMat._42); tab->EdPosY->setText(tmp);
				sprintf(tmp, "%f", m_mHitboxMat._43); tab->EdPosZ->setText(tmp);

				float3 rot = SMMatrixToEuler(m_mHitboxMat);
				sprintf(tmp, "%f", SMToAngle(-rot.x)); tab->EdRotX->setText(tmp);
				sprintf(tmp, "%f", SMToAngle(rot.y)); tab->EdRotY->setText(tmp);
				sprintf(tmp, "%f", SMToAngle(-rot.z)); tab->EdRotZ->setText(tmp);

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
