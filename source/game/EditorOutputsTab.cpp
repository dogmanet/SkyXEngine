#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <Commdlg.h>

#include "EditorExtension.h"
#include "resource.h"
#include "EditorObject.h"
#include "Editable.h"

extern HINSTANCE g_hInstance;


CEditorOutputsTab::CEditorCommand::~CEditorCommand()
{
	for(UINT i = 0, l = m_aEditorObjects.size(); i < l; ++i)
	{
		mem_release(m_aEditorObjects[i]);
	}
}

bool XMETHODCALLTYPE CEditorOutputsTab::CEditorCommand::exec()
{
	printf("CEditorCommand::exec()\n");

	compile();

	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		Object &obj = m_aObjects[i];
		CBaseEntity *pEnt = obj.pObject->getEnt();
		if(pEnt)
		{
			for(UINT j = 0, jl = obj.aOutputs.size(); j < jl; ++j)
			{
				Output &out = obj.aOutputs[j];
				pEnt->setKV(out.szOutput, out.sValue.c_str());
			}
		}
	}

	return(true);
}
bool XMETHODCALLTYPE CEditorOutputsTab::CEditorCommand::undo()
{
	printf("CEditorCommand::undo()\n");

	for(UINT i = 0, l = m_aPrevObjects.size(); i < l; ++i)
	{
		Object &obj = m_aPrevObjects[i];
		CBaseEntity *pEnt = obj.pObject->getEnt();
		if(pEnt)
		{
			for(UINT j = 0, jl = obj.aOutputs.size(); j < jl; ++j)
			{
				Output &out = obj.aOutputs[j];
				pEnt->setKV(out.szOutput, out.sValue.c_str());
			}
		}
	}
	return(true);
}

void CEditorOutputsTab::CEditorCommand::compile()
{
	if(m_isCompiled)
	{
		return;
	}
	m_isCompiled = true;

	for(UINT i = 0, l = m_aPrevObjects.size(); i < l; ++i)
	{
		Object &obj = m_aPrevObjects[i];
		Object &obj2 = m_aObjects[m_aObjects.size()];
		obj2.pObject = obj.pObject;

		for(UINT j = 0, jl = obj.aOutputs.size(); j < jl; ++j)
		{
			obj2.aOutputs[obj2.aOutputs.size()].szOutput = obj.aOutputs[j].szOutput;
		}
	}

	char tmp[1024];
	Row *pRow;
	CEditorObject *pObj;
	for(UINT i = 0, l = m_aRows.size(); i < l; ++i)
	{
		pRow = &m_aRows[i];
		for(UINT j = 0, jl = pRow->aObjects.size(); j < jl; ++j)
		{
			pObj = pRow->aObjects[j];
			int idx = m_aObjects.indexOf(pObj, [](const Object &a, CEditorObject *pB){
				return(a.pObject == pB);
			});
			if(idx < 0)
			{
				idx = m_aObjects.size();
				m_aObjects[idx].pObject = pObj;
			}

			Object &obj = m_aObjects[idx];

			idx = obj.aOutputs.indexOf(pRow->szOutput, [](const Output &a, const char *b){
				return(!fstrcmp(a.szOutput, b));
			});
			if(idx < 0)
			{
				idx = obj.aOutputs.size();
				obj.aOutputs[idx].szOutput = pRow->szOutput;
			}

			Output &out = obj.aOutputs[idx];

			char *ptr = tmp;
			ptr += sprintf(ptr, "%s%s:%s:%g", out.sValue.length() ? "," : "", pRow->sTarget.c_str(), pRow->sInput.c_str(), pRow->fTime);
			if(pRow->fTimeTo > 0.0f)
			{
				ptr += sprintf(ptr, "-%g", pRow->fTimeTo);
			}
			ptr += sprintf(ptr, ":%s:%d", pRow->sParam.c_str(), pRow->isOnceOnly ? 1 : -1);

			out.sValue += tmp;
		}
	}
}

void CEditorOutputsTab::CEditorCommand::addObjectOutput(CEditorObject *pObject, const char *szOutput)
{
	int idx = m_aPrevObjects.indexOf(pObject, [](const Object &a, CEditorObject *pB){
		return(a.pObject == pB);
	});
	if(idx < 0)
	{
		idx = m_aPrevObjects.size();
		m_aPrevObjects[idx].pObject = pObject;
	}

	Object &obj = m_aPrevObjects[idx];

	idx = obj.aOutputs.indexOf(szOutput, [](const Output &a, const char *b){
		return(!fstrcmp(a.szOutput, b));
	});
	if(idx < 0)
	{
		idx = obj.aOutputs.size();
		obj.aOutputs[idx].szOutput = szOutput;
	}
}

void CEditorOutputsTab::CEditorCommand::prepare()
{
	char tmp[1024];
	Row *pRow;
	CEditorObject *pObj;
	for(UINT i = 0, l = m_aRows.size(); i < l; ++i)
	{
		pRow = &m_aRows[i];
		for(UINT j = 0, jl = pRow->aObjects.size(); j < jl; ++j)
		{
			pObj = pRow->aObjects[j];
			int idx = m_aPrevObjects.indexOf(pObj, [](const Object &a, CEditorObject *pB){
				return(a.pObject == pB);
			});
			if(idx < 0)
			{
				idx = m_aPrevObjects.size();
				m_aPrevObjects[idx].pObject = pObj;
			}

			Object &obj = m_aPrevObjects[idx];

			idx = obj.aOutputs.indexOf(pRow->szOutput, [](const Output &a, const char *b){
				return(!fstrcmp(a.szOutput, b));
			});
			if(idx < 0)
			{
				idx = obj.aOutputs.size();
				obj.aOutputs[idx].szOutput = pRow->szOutput;
			}

			Output &out = obj.aOutputs[idx];

			char *ptr = tmp;
			ptr += sprintf(ptr, "%s%s:%s:%g", out.sValue.length() ? "," : "", pRow->sTarget.c_str(), pRow->sInput.c_str(), pRow->fTime);
			if(pRow->fTimeTo > 0.0f)
			{
				ptr += sprintf(ptr, "-%g", pRow->fTimeTo);
			}
			ptr += sprintf(ptr, ":%s:%d", pRow->sParam.c_str(), pRow->isOnceOnly ? 1 : -1);

			out.sValue += tmp;
		}
	}
}

//##########################################################################

WNDPROC CEditorOutputsTab::m_pfnEditProc = NULL;
WNDPROC CEditorOutputsTab::m_pfnFilteredComboBoxOldWndproc = NULL;
WNDPROC CEditorOutputsTab::m_pfnButtonOldWndproc = NULL;

CEditorOutputsTab::CEditorOutputsTab(CEditable *pEditable):
	m_pEditable(pEditable)
{}
CEditorOutputsTab::~CEditorOutputsTab()
{
	mem_release(m_pCurrentCommand);

	if(m_hDlg)
	{
		DestroyWindow(m_hDlg);
	}

	if(m_pCallback)
	{
		m_pCallback->onRemove();
	}
}

void XMETHODCALLTYPE CEditorOutputsTab::init(HWND hWndParent, int X, int Y, int cx, int cy)
{
	assert(!m_hDlg);

	m_hDlg = CreateDialogParamA(g_hInstance, MAKEINTRESOURCE(IDD_OP_OUTPUTS), hWndParent, DlgProc, (LPARAM)this);
	SetWindowPos(m_hDlg, HWND_TOP, X, Y, cx, cy, 0);
}
const char* XMETHODCALLTYPE CEditorOutputsTab::getTitle()
{
	return("Outputs");
}

void XMETHODCALLTYPE CEditorOutputsTab::setCallback(IXEditorPropertyTabCallback *pCallback)
{
	m_pCallback = pCallback;
}

void XMETHODCALLTYPE CEditorOutputsTab::show()
{
	ShowWindow(m_hDlg, SW_SHOWNORMAL);

	updateTable();
}
void XMETHODCALLTYPE CEditorOutputsTab::hide()
{
	ShowWindow(m_hDlg, SW_HIDE);
}

IXEditorCommand* XMETHODCALLTYPE CEditorOutputsTab::startNewCommand()
{
	mem_release(m_pCurrentCommand);

	m_pCurrentCommand = new CEditorCommand();
	m_pCurrentCommand->AddRef();

	initSelection();

	return(m_pCurrentCommand);
}

bool XMETHODCALLTYPE CEditorOutputsTab::isEnabled()
{
	return(m_isEnabled);
}

void CEditorOutputsTab::setEnabled(bool isEnabled)
{
	if(m_isEnabled == isEnabled)
	{
		return;
	}

	m_isEnabled = isEnabled;

	if(m_pCallback)
	{
		if(isEnabled)
		{
			m_pCallback->onAdd();
		}
		else
		{
			m_pCallback->onRemove();
			hide();
		}
	}
}

void CEditorOutputsTab::onSelectionChanged()
{}

void CEditorOutputsTab::initSelection()
{
	auto &aObjects = m_pEditable->getObjects();

	Array<const char*> aClasses;

	CEditorObject *pObj;

	for(UINT i = 0, l = aObjects.size(); i < l; ++i)
	{
		pObj = aObjects[i];
		if(pObj->isSelected())
		{
			if(aClasses.indexOf(pObj->getClassName()) < 0)
			{
				aClasses.push_back(pObj->getClassName());
			}
		}
	}
	
	Array<const char*> aTmpOutputs;
	m_aOutputs.clearFast();
	bool isFirst = true;
	for(UINT i = 0, l = aClasses.size(); i < l; ++i)
	{
		proptable_t *pTable = CEntityFactoryMap::GetInstance()->getPropTable(aClasses[i]);

		while(pTable)
		{
			for(int j = 0; j < pTable->numFields; ++j)
			{
				propdata_t *pPropData = &pTable->pData[j];

				if(pPropData->type == PDF_OUTPUT)
				{
					if((isFirst ? m_aOutputs : aTmpOutputs).indexOf(pPropData->szKey, [](const char *a, const char *b){
						return(!fstrcmp(a, b));
					}) < 0)
					{
						(isFirst ? m_aOutputs : aTmpOutputs).push_back(pPropData->szKey);
					}
				}
			}

			pTable = pTable->pBaseProptable;
		}

		if(!isFirst)
		{
			for(UINT j = 0, jl = m_aOutputs.size(); j < jl; ++j)
			{
				if(aTmpOutputs.indexOf(m_aOutputs[i], [](const char *a, const char *b){
					return(!fstrcmp(a, b));
				}) < 0)
				{
					m_aOutputs.erase(j);
					--j;
					--jl;
				}
			}

			aTmpOutputs.clearFast();
		}

		isFirst = false;

		if(!m_aOutputs.size())
		{
			break;
		}
	}

	ComboBox_ResetContent(m_hOutputCmb);
	for(UINT i = 0, l = m_aOutputs.size(); i < l; ++i)
	{
		int idx = ComboBox_AddString(m_hOutputCmb, m_aOutputs[i]);
		ComboBox_SetItemData(m_hOutputCmb, idx, m_aOutputs[i]);
	}

	//**************************************************************************

	m_aNames.clearFast();
	m_aNames.push_back(ENT_SPECIAL_NAME_THIS);
	m_aNames.push_back(ENT_SPECIAL_NAME_PARENT);
	CBaseEntity *pEnt;
	const char *szName;
	for(int i = 0, l = GameData::m_pMgr->getCount(); i < l; ++i)
	{
		pEnt = GameData::m_pMgr->getById(i);
		if(pEnt)
		{
			szName = pEnt->getName();
			if(szName && szName[0])
			{
				if(m_aNames.indexOf(szName, [](const char *a, const char *b){
					return(!fstrcmp(a, b));
				}) < 0)
				{
					m_aNames.push_back(szName);
				}
			}
		}
	}

	ComboBox_ResetContent(m_hTargetCmb);
	for(UINT i = 0, l = m_aNames.size(); i < l; ++i)
	{
		int idx = ComboBox_AddString(m_hTargetCmb, m_aNames[i]);
		ComboBox_SetItemData(m_hTargetCmb, idx, m_aNames[i]);
	}

	//**************************************************************************

	// char tmpBuf[10240];
	for(UINT i = 0, l = aObjects.size(); i < l; ++i)
	{
		pObj = aObjects[i];
		if(pObj->isSelected() && pObj->getEnt())
		{
			m_pCurrentCommand->m_aEditorObjects.push_back(pObj);
			pObj->AddRef();

			CBaseEntity *pEnt = pObj->getEnt();
			proptable_t *pTable = CEntityFactoryMap::GetInstance()->getPropTable(pObj->getClassName());

			while(pTable)
			{
				for(int j = 0; j < pTable->numFields; ++j)
				{
					propdata_t *pPropData = &pTable->pData[j];

					if(pPropData->type == PDF_OUTPUT)
					{
						m_pCurrentCommand->addObjectOutput(pObj, pPropData->szKey);
						propdata_t *pField = pEnt->getField(pPropData->szKey);
						output_t *pOutput = &(pEnt->*((output_t CBaseEntity::*)pField->pField));
						
						for(int i = 0, l = pOutput->aOutputs.size(); i < l; ++i)
						{
							const named_output_t &namedOutput = pOutput->aOutputs[i];

							int idx = m_pCurrentCommand->m_aRows.indexOf(namedOutput, [&](const Row &row, const named_output_t &out){
								return(
									!fstrcmp(row.szOutput, pPropData->szKey)
									&& !fstrcmp(row.sTarget.c_str(), out.szTargetName)
									&& !fstrcmp(row.sInput.c_str(), out.szTargetInput)
									&& (out.szTargetData && !fstrcmp(row.sParam.c_str(), out.szTargetData)
									|| !out.szTargetData && !fstrcmp(row.sParam.c_str(), ENT_OUTPUT_PARAM_NONE))
									&& row.fTime == out.fDelay
									&& (out.useRandomDelay && row.fTimeTo == out.fDelayTo
									|| !out.useRandomDelay && row.fTimeTo <= 0.0f)
									// && row.isOnceOnly && out.iMaxFire == 1
									);
							});

							if(idx < 0)
							{
								idx = m_pCurrentCommand->m_aRows.size();
								Row &row = m_pCurrentCommand->m_aRows[idx];
								row.szOutput = pPropData->szKey;
								row.sTarget = namedOutput.szTargetName;
								row.sInput = namedOutput.szTargetInput;
								if(namedOutput.szTargetData)
								{
									row.sParam = namedOutput.szTargetData;
								}
								row.fTime = namedOutput.fDelay;
								if(namedOutput.useRandomDelay)
								{
									row.fTimeTo = namedOutput.fDelayTo;
								}
							}
							m_pCurrentCommand->m_aRows[idx].aObjects.push_back(pObj);
						}
					}
				}

				pTable = pTable->pBaseProptable;
			}
		}
	}

	m_pCurrentCommand->prepare();

	updateTable();
}

INT_PTR CALLBACK CEditorOutputsTab::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CEditorOutputsTab *pThis = (CEditorOutputsTab*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(msg == WM_INITDIALOG)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)lParam);
		pThis = (CEditorOutputsTab*)lParam;
		pThis->m_hDlg = hWnd;
	}
	if(pThis)
	{
		return(pThis->dlgProc(hWnd, msg, wParam, lParam));
	}
	return(FALSE);
}

INT_PTR CALLBACK CEditorOutputsTab::dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
	{
		m_hOutputsList = GetDlgItem(hWnd, IDC_LIST_OUTPUTS);
		m_hOutputCmb = GetDlgItem(hWnd, IDC_COMBO_OUTPUT);
		m_hTargetCmb = GetDlgItem(hWnd, IDC_COMBO_TARGET);
		m_hInputCmb = GetDlgItem(hWnd, IDC_COMBO_INPUT);
		m_hParameterCmb = GetDlgItem(hWnd, IDC_COMBO_PARAMETER);
		m_hTimeEd = GetDlgItem(hWnd, IDC_EDIT_DELAY);
		m_hTimeToEd = GetDlgItem(hWnd, IDC_EDIT_DELAY_TO);
		m_hFireOnceCb = GetDlgItem(hWnd, IDC_CHECK_FIRE_ONCE);

		m_hAddBtn = GetDlgItem(hWnd, IDC_BUTTON_ADD);
		m_hCopyBtn = GetDlgItem(hWnd, IDC_BUTTON_COPY);
		m_hPasteBtn = GetDlgItem(hWnd, IDC_BUTTON_PASTE);
		m_hDeleteBtn = GetDlgItem(hWnd, IDC_BUTTON_DELETE);

		m_pfnEditProc = (WNDPROC)GetWindowLongPtr(m_hTimeEd, GWLP_WNDPROC);
		SetWindowLongPtr(m_hTimeEd, GWLP_WNDPROC, (LPARAM)EditEnterDlgProc);
		SetWindowLongPtr(m_hTimeToEd, GWLP_WNDPROC, (LPARAM)EditEnterDlgProc);

		ComboBox_AddString(m_hParameterCmb, ENT_OUTPUT_PARAM_NONE);

		HWND aComboBoxes[] = {
			m_hOutputCmb,
			m_hTargetCmb,
			m_hInputCmb,
			m_hParameterCmb
		};
		for(UINT i = 0, l = ARRAYSIZE(aComboBoxes); i < l; ++i)
		{
			COMBOBOXINFO info = {sizeof(COMBOBOXINFO)};
			GetComboBoxInfo(aComboBoxes[i], &info);
			SetWindowLongPtr(info.hwndItem, GWLP_USERDATA, (LONG_PTR)this);
			if(!m_pfnFilteredComboBoxOldWndproc)
			{
				m_pfnFilteredComboBoxOldWndproc = (WNDPROC)GetWindowLongPtr(info.hwndItem, GWLP_WNDPROC);
			}
			SetWindowLongPtr(info.hwndItem, GWLP_WNDPROC, (LONG_PTR)FilteredComboBoxWndProc);
		}

		HWND aButtons[] = {
			m_hCopyBtn,
			m_hDeleteBtn,
			m_hPasteBtn,
			m_hAddBtn
		};
		for(UINT i = 0, l = ARRAYSIZE(aButtons); i < l; ++i)
		{
			SetWindowLongPtr(aButtons[i], GWLP_USERDATA, (LONG_PTR)this);
			if(!m_pfnButtonOldWndproc)
			{
				m_pfnButtonOldWndproc = (WNDPROC)GetWindowLongPtr(aButtons[i], GWLP_WNDPROC);
			}
			SetWindowLongPtr(aButtons[i], GWLP_WNDPROC, (LONG_PTR)ButtonWndProc);
		}

		// m_pfnButtonOldWndproc

		ListView_SetExtendedListViewStyle(m_hOutputsList, ListView_GetExtendedListViewStyle(m_hOutputsList) | LVS_EX_FULLROWSELECT);

		LV_COLUMNA lvColumn;
		memset(&lvColumn, 0, sizeof(lvColumn));

		RECT rc;
		GetClientRect(m_hOutputsList, &rc);

		lvColumn.mask = LVCF_WIDTH | LVCF_TEXT;

		char *aszColumns[] = {
			"My output",
			"Target entity",
			"Target input",
			"Parameter",
			"Delay",
			"Only once"
		};
		int iCount = ARRAYSIZE(aszColumns);

		lvColumn.cx = (rc.right - rc.left) / iCount;
		for(int i = 0; i < iCount; ++i)
		{
			lvColumn.pszText = aszColumns[i];
			lvColumn.cchTextMax = (int)strlen(lvColumn.pszText);
			ListView_InsertColumn(m_hOutputsList, i, &lvColumn);
		}

		break;
	}
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case LVN_ITEMCHANGED:
			{
				LPNMLISTVIEW pNMLV = (LPNMLISTVIEW)lParam;
				if(pNMLV->hdr.idFrom == IDC_LIST_OUTPUTS && !m_isSkipUpdate && (pNMLV->uChanged & LVIF_STATE) && ((pNMLV->uNewState ^ pNMLV->uOldState) & LVIS_SELECTED))
				{
					Row *pRow = &m_pCurrentCommand->m_aRows[pNMLV->iItem];
					pRow->isSelected = (pNMLV->uNewState & LVIS_SELECTED) != 0;

					updateButtons();
				}
			}
			break;
		}
		break;

	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case CBN_EDITCHANGE:
			{
				HWND hCombo = (HWND)lParam;
				int iLen = ComboBox_GetTextLength(hCombo) + 1;
				char *szTemp = (char*)alloca(sizeof(char)* iLen);
				ComboBox_GetText(hCombo, szTemp, iLen);

				switch(LOWORD(wParam))
				{
				case IDC_COMBO_OUTPUT:
					filterOutputsList(szTemp);
					break;
				case IDC_COMBO_TARGET:
					filterTargetsList(szTemp);
					break;
				case IDC_COMBO_INPUT:
					filterInputsList(szTemp);
					break;
				}
			}
			break;

		case CBN_SELCHANGE:
			{
				HWND hCombo = (HWND)lParam;
				int iSel = ComboBox_GetCurSel(hCombo);
				if(iSel >= 0)
				{
					switch(LOWORD(wParam))
					{
					case IDC_COMBO_OUTPUT:
						{
							const char *szValue = (const char*)ComboBox_GetItemData(hCombo, iSel);
							
							for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
							{
								if(m_pCurrentCommand->m_aRows[i].isSelected)
								{
									m_pCurrentCommand->m_aRows[i].szOutput = szValue;
								}
							}

							m_pCurrentCommand->m_isDirty = true;

							updateTable();
						}
						break;

					case IDC_COMBO_INPUT:
						{
							const char *szValue = (const char*)ComboBox_GetItemData(hCombo, iSel);

							for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
							{
								if(m_pCurrentCommand->m_aRows[i].isSelected)
								{
									m_pCurrentCommand->m_aRows[i].sInput = szValue;
								}
							}

							m_pCurrentCommand->m_isDirty = true;

							m_isSkipUpdateInputs = true;
							updateTable();
							m_isSkipUpdateInputs = false;
						}
						break;

					// case IDC_COMBO_INPUT:
					// case IDC_COMBO_TARGET:
					// case IDC_COMBO_PARAMETER:
					// 	break;
					}
				}
			}
			break;

		case CBN_KILLFOCUS:
		case EN_KILLFOCUS:
			switch(LOWORD(wParam))
			{
			case IDC_EDIT_DELAY:
			case IDC_EDIT_DELAY_TO:
				{
					int iValLen = Edit_GetTextLength((HWND)lParam) + 1;
					char *szValue = (char*)alloca(sizeof(char) * iValLen);
					Edit_GetText((HWND)lParam, szValue, iValLen);

					float fVal = 0.0f;
					if(!sscanf(szValue, "%f", &fVal) && LOWORD(wParam) == IDC_EDIT_DELAY_TO)
					{
						fVal = -1.0f;
					}
					Row *pRow;
					for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
					{
						pRow = &m_pCurrentCommand->m_aRows[i];
						if(pRow->isSelected)
						{
							if(LOWORD(wParam) == IDC_EDIT_DELAY)
							{
								pRow->fTime = fVal;
							}
							else
							{
								pRow->fTimeTo = fVal;
							}

							if(pRow->fTimeTo > 0.0f && pRow->fTimeTo <= pRow->fTime)
							{
								pRow->fTimeTo = -1.0f;
							}
						}
					}

					m_pCurrentCommand->m_isDirty = true;

					updateTable();
				}
				break;

			case IDC_COMBO_OUTPUT:
				for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
				{
					if(m_pCurrentCommand->m_aRows[i].isSelected)
					{
						ComboBox_SetText((HWND)lParam, m_pCurrentCommand->m_aRows[i].szOutput);
						break;
					}
				}
				break;

			case IDC_COMBO_INPUT:
				for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
				{
					if(m_pCurrentCommand->m_aRows[i].isSelected)
					{
						ComboBox_SetText((HWND)lParam, m_pCurrentCommand->m_aRows[i].sInput.c_str());
						break;
					}
				}
				break;

			case IDC_COMBO_TARGET:
				{
					HWND hCombo = (HWND)lParam;
					int iLen = ComboBox_GetTextLength(hCombo) + 1;
					char *szTemp = (char*)alloca(sizeof(char)* iLen);
					ComboBox_GetText(hCombo, szTemp, iLen);

					for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
					{
						if(m_pCurrentCommand->m_aRows[i].isSelected)
						{
							m_pCurrentCommand->m_aRows[i].sTarget = szTemp;
						}
					}

					m_pCurrentCommand->m_isDirty = true;

					updateTable();
				}
				break;

			case IDC_COMBO_PARAMETER:
				{
					HWND hCombo = (HWND)lParam;
					int iLen = ComboBox_GetTextLength(hCombo) + 1;
					char *szTemp = (char*)alloca(sizeof(char)* iLen);
					ComboBox_GetText(hCombo, szTemp, iLen);

					for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
					{
						if(m_pCurrentCommand->m_aRows[i].isSelected)
						{
							m_pCurrentCommand->m_aRows[i].sParam = szTemp;
						}
					}

					m_pCurrentCommand->m_isDirty = true;

					m_isSkipUpdateInputs = true;
					updateTable();
					m_isSkipUpdateInputs = false;
				}
				break;
			}
			break;
		}

		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			{
				for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
				{
					m_pCurrentCommand->m_aRows[i].isSelected = false;
				}

				m_pCurrentCommand->m_isDirty = true;

				Row row;
				row.aObjects = m_pCurrentCommand->m_aEditorObjects;
				row.isSelected = true;
				m_pCurrentCommand->m_aRows.push_back(row);
				updateTable();
			}
			break;
		case IDC_BUTTON_DELETE:
			{
				for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
				{
					if(m_pCurrentCommand->m_aRows[i].isSelected)
					{
						m_pCurrentCommand->m_aRows.erase(i);
						--i;
						--l;
					}
				}

				m_pCurrentCommand->m_isDirty = true;

				updateTable();
			}
			break;
		case IDC_BUTTON_COPY:
			copySelection();
			break;
		case IDC_BUTTON_PASTE:
			pasteCopied();
			break;

		case IDC_CHECK_FIRE_ONCE:
			{
				bool isChecked = Button_GetCheck(m_hFireOnceCb) == BST_CHECKED;
				for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
				{
					if(m_pCurrentCommand->m_aRows[i].isSelected)
					{
						m_pCurrentCommand->m_aRows[i].isOnceOnly = isChecked;
					}
				}

				m_pCurrentCommand->m_isDirty = true;

				updateTable();
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		SetFocus(GetParent(m_hDlg));
		break;

	default:
		return(FALSE);
	}
	return(TRUE);
}

LRESULT CALLBACK CEditorOutputsTab::EditEnterDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CHAR:
		if(wParam == VK_RETURN)
		{
			SetFocus(GetNextDlgTabItem(GetParent(hWnd), hWnd, GetKeyState(VK_SHIFT) < 0));
			return(0);
			// SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWnd), EN_KILLFOCUS), (LPARAM)hWnd);
			// return(0);
		}
	}
	return(CallWindowProc(m_pfnEditProc, hWnd, msg, wParam, lParam));
}

LRESULT CALLBACK CEditorOutputsTab::FilteredComboBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CEditorOutputsTab *pThis = (CEditorOutputsTab*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch(message)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			HWND hCombo = GetParent(hWnd);

			if(hCombo == pThis->m_hTargetCmb && wParam == VK_ESCAPE)
			{
				int iLen = ComboBox_GetTextLength(hCombo) + 1;
				char *szTemp = (char*)alloca(sizeof(char)* iLen);
				ComboBox_GetText(hCombo, szTemp, iLen);

				ComboBox_ShowDropdown(hCombo, FALSE);

				ComboBox_SetText(hCombo, szTemp);
			}
			else
			{
				ComboBox_ShowDropdown(hCombo, FALSE);
			}

			if(hCombo == pThis->m_hTargetCmb && wParam == VK_RETURN)
			{
				int iSel = ComboBox_GetCurSel(hCombo);
				if(iSel >= 0)
				{
					ComboBox_SetText(hCombo, (const char*)ComboBox_GetItemData(hCombo, iSel));
				}
			}

			SetFocus(GetNextDlgTabItem(pThis->m_hDlg, hCombo, GetKeyState(VK_SHIFT) < 0));
			break;
		}
		break;

	case WM_GETDLGCODE:
		if(wParam == VK_RETURN)
		{
			return(DLGC_WANTALLKEYS);
		}
		break;
	}

	return(CallWindowProc(m_pfnFilteredComboBoxOldWndproc, hWnd, message, wParam, lParam));
}

LRESULT CALLBACK CEditorOutputsTab::ButtonWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CEditorOutputsTab *pThis = (CEditorOutputsTab*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch(message)
	{
	case WM_GETDLGCODE:
		if(wParam == VK_RETURN)
		{
			return(DLGC_BUTTON);
		}
		break;
	}

	return(CallWindowProc(m_pfnButtonOldWndproc, hWnd, message, wParam, lParam));
}

void CEditorOutputsTab::updateTable()
{
	m_isSkipUpdate = true;

	LVITEMA lvItem;
	memset(&lvItem, 0, sizeof(lvItem));

	UINT uCount = ListView_GetItemCount(m_hOutputsList);
	for(UINT i = uCount, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
	{
		ListView_InsertItem(m_hOutputsList, &lvItem);
	}

	for(UINT i = m_pCurrentCommand->m_aRows.size(), l = uCount; i < l; ++i)
	{
		ListView_DeleteItem(m_hOutputsList, i);
		--i;
		--l;
	}

	char tmp[32];
	for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
	{
		Row *pRow = &m_pCurrentCommand->m_aRows[i];

		ListView_SetItemState(m_hOutputsList, i, pRow->isSelected ? LVIS_SELECTED : 0, LVIS_SELECTED);

		ListView_SetItemText(m_hOutputsList, i, 0, (LPSTR)pRow->szOutput);
		ListView_SetItemText(m_hOutputsList, i, 1, (LPSTR)pRow->sTarget.c_str());
		ListView_SetItemText(m_hOutputsList, i, 2, (LPSTR)pRow->sInput.c_str());
		ListView_SetItemText(m_hOutputsList, i, 3, (LPSTR)pRow->sParam.c_str());

		int iBase = sprintf(tmp, "%.3f", pRow->fTime);
		if(pRow->fTimeTo > 0.0f)
		{
			sprintf(tmp + iBase, "-%.3f", pRow->fTimeTo);
		}
		
		ListView_SetItemText(m_hOutputsList, i, 4, tmp);
		ListView_SetItemText(m_hOutputsList, i, 5, (LPSTR)(pRow->isOnceOnly ? "Yes" : "No"));
	}

	m_isSkipUpdate = false;

	updateButtons();
}

void CEditorOutputsTab::updateButtons()
{
	if(m_isSkipUpdate)
	{
		return;
	}
	int iCount = ListView_GetSelectedCount(m_hOutputsList);
	bool hasSelection = ListView_GetSelectedCount(m_hOutputsList) > 0;
	Button_Enable(m_hCopyBtn, hasSelection);
	Button_Enable(m_hPasteBtn, m_aClipBoard.size() > 0);
	Button_Enable(m_hDeleteBtn, hasSelection);

	bool isOutputEditable = hasSelection;
	bool isTargetEditable = hasSelection;
	bool isInputEditable = hasSelection;
	bool isParameterEditable = hasSelection;
	bool isDelayEditable = hasSelection;
	bool isFireOnceEditable = hasSelection;

	Row *pFirstRow = NULL;
	if(hasSelection)
	{
		int iFirstSelected = -1;
		Row *pRow;
		for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
		{
			pRow = &m_pCurrentCommand->m_aRows[i];
			if(pRow->isSelected)
			{
				if(pFirstRow)
				{
					isOutputEditable = isOutputEditable && !fstrcmp(pFirstRow->szOutput, pRow->szOutput);
					isTargetEditable = isTargetEditable && pFirstRow->sTarget == pRow->sTarget;
					isInputEditable = isInputEditable && pFirstRow->sInput == pRow->sInput;
					isParameterEditable = isParameterEditable && pFirstRow->sParam == pRow->sParam;
					isDelayEditable = isDelayEditable && pFirstRow->fTime == pRow->fTime && pFirstRow->fTimeTo == pRow->fTimeTo;
					isFireOnceEditable = isFireOnceEditable && pFirstRow->isOnceOnly == pRow->isOnceOnly;
				}
				else
				{
					pFirstRow = pRow;
				}
			}
		}
	}

	ComboBox_Enable(m_hOutputCmb, isOutputEditable);
	ComboBox_Enable(m_hTargetCmb, isTargetEditable);
	ComboBox_Enable(m_hInputCmb, isInputEditable);
	ComboBox_Enable(m_hParameterCmb, isParameterEditable);
	Edit_Enable(m_hTimeEd, isDelayEditable);
	Edit_Enable(m_hTimeToEd, isDelayEditable);
	Button_Enable(m_hFireOnceCb, isFireOnceEditable);

	ComboBox_SetText(m_hOutputCmb, isOutputEditable ? pFirstRow->szOutput : "");
	ComboBox_SetText(m_hTargetCmb, isTargetEditable ? pFirstRow->sTarget.c_str() : "");
	ComboBox_SetText(m_hInputCmb, isInputEditable ? pFirstRow->sInput.c_str() : "");
	ComboBox_SetText(m_hParameterCmb, isParameterEditable ? pFirstRow->sParam.c_str() : "");
	
	char tmp[32], tmp2[32];
	tmp[0] = 0;
	tmp2[0] = 0;

	if(isDelayEditable)
	{
		sprintf(tmp, "%.3f", pFirstRow->fTime);
		if(pFirstRow->fTimeTo > 0.0f)
		{
			sprintf(tmp2, "%.3f", pFirstRow->fTimeTo);
		}
	}
	Edit_SetText(m_hTimeEd, tmp);
	Edit_SetText(m_hTimeToEd, tmp2);

	Button_SetCheck(m_hFireOnceCb, isFireOnceEditable && pFirstRow->isOnceOnly ? BST_CHECKED : BST_UNCHECKED);

	if(isInputEditable)
	{
		updateInputs();
	}
}

static void FilterList(const char *szFilter, HWND hCombo, const Array<const char*> &aStrings, const char *szCurSel = NULL)
{
	DWORD dwStart;
	DWORD dwEnd;
	SendMessage(hCombo, CB_GETEDITSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

	int iLen = /*ComboBox_GetTextLength(hCombo) */ dwStart + 1;
	char *szTemp = (char*)alloca(sizeof(char)* iLen);
	ComboBox_GetText(hCombo, szTemp, iLen);
	
	ComboBox_ShowDropdown(hCombo, TRUE);
	SendMessage(hCombo, WM_SETCURSOR, 0, 0);
	ComboBox_ResetContent(hCombo);
	bool isSomethingFound = false;
	const char *szPos = NULL;
	int iStart = -1;
	for(UINT i = 0, l = aStrings.size(); i < l; ++i)
	{
		if(!szFilter[0] || (szPos = strcasestr(aStrings[i], szFilter)))
		{
			iStart = (int)(szPos - aStrings[i]);
			int idx = ComboBox_AddString(hCombo, aStrings[i]);
			ComboBox_SetItemData(hCombo, idx, aStrings[i]);
			isSomethingFound = true;
		}
	}
	if(isSomethingFound && szFilter[0])
	{
		int iSel = 0;
		if(szCurSel)
		{
			iSel = ComboBox_FindItemData(hCombo, -1, szCurSel);
			if(iSel < 0)
			{
				iSel = 0;
			}
		}
		ComboBox_SetCurSel(hCombo, iSel);
		SendMessage(GetParent(hCombo), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hCombo), CBN_SELCHANGE), (LPARAM)hCombo);
	}
	/*else
	{*/
		ComboBox_SetText(hCombo, szTemp);
	/*}*/
	/*if(iStart > 0)
	{
		dwStart += iStart;
	}
	if(szFilter[0])
	{
		SendMessage(hCombo, CB_SETEDITSEL, NULL, MAKELPARAM(dwStart, (DWORD)-1));
	}
	else
	{*/
		SendMessage(hCombo, CB_SETEDITSEL, NULL, MAKELPARAM(dwStart, dwStart));
	/*}*/
}

void CEditorOutputsTab::filterOutputsList(const char *szFilter)
{
	const char *szCurVal = NULL;
	for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
	{
		if(m_pCurrentCommand->m_aRows[i].isSelected)
		{
			szCurVal = m_pCurrentCommand->m_aRows[i].szOutput;
			break;
		}
	}
	FilterList(szFilter, m_hOutputCmb, m_aOutputs, szCurVal);
}

void CEditorOutputsTab::filterTargetsList(const char *szFilter)
{
	FilterList(szFilter, m_hTargetCmb, m_aNames);
}

void CEditorOutputsTab::filterInputsList(const char *szFilter)
{
	const char *szCurVal = NULL;
	for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
	{
		if(m_pCurrentCommand->m_aRows[i].isSelected)
		{
			szCurVal = m_pCurrentCommand->m_aRows[i].sInput.c_str();
			break;
		}
	}
	FilterList(szFilter, m_hInputCmb, m_aInputs, szCurVal);
}

void CEditorOutputsTab::updateInputs()
{
	if(m_isSkipUpdateInputs)
	{
		return;
	}
	int iLen = ComboBox_GetTextLength(m_hInputCmb) + 1;
	char *szTemp = (char*)alloca(sizeof(char)* iLen);
	ComboBox_GetText(m_hInputCmb, szTemp, iLen);

	Array<const char*> aClasses;

	Row *pRow;
	CBaseEntity *pEnt = NULL;
	const char *szTarget = NULL;
	for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
	{
		pRow = &m_pCurrentCommand->m_aRows[i];
		if(pRow->isSelected)
		{
			szTarget = pRow->sTarget.c_str();
			if(ENT_IS_NAME_SPECIAL(szTarget))
			{
				for(UINT j = 0, jl = pRow->aObjects.size(); j < jl; ++j)
				{
					pEnt = pRow->aObjects[j]->getEnt();
					if(pEnt)
					{
						pEnt = pEnt->getEntByName(szTarget, NULL);
						if(pEnt && aClasses.indexOf(pEnt->getClassName(), [](const char *a, const char *b){
							return(!fstrcmp(a, b));
						}) < 0)
						{
							aClasses.push_back(pEnt->getClassName());
						}
					}
				}
				szTarget = NULL;
			}
			else
			{
				break;
			}
		}
	}

	if(szTarget)
	{
		while((pEnt = GameData::m_pMgr->findEntityByName(szTarget, pEnt)))
		{
			if(aClasses.indexOf(pEnt->getClassName(), [](const char *a, const char *b){
				return(!fstrcmp(a, b));
			}) < 0)
			{
				aClasses.push_back(pEnt->getClassName());
			}
		}
	}

	// list inputs
	m_aInputs.clearFast();
	Array<const char*> aTmpInputs;
	bool isFirst = true;
	for(UINT i = 0, l = aClasses.size(); i < l; ++i)
	{
		proptable_t *pTable = CEntityFactoryMap::GetInstance()->getPropTable(aClasses[i]);

		while(pTable)
		{
			for(int j = 0; j < pTable->numFields; ++j)
			{
				propdata_t *pPropData = &pTable->pData[j];

				if(pPropData->flags & PDFF_INPUT)
				{
					if((isFirst ? m_aInputs : aTmpInputs).indexOf(pPropData->szKey, [](const char *a, const char *b){
						return(!fstrcmp(a, b));
					}) < 0)
					{
						(isFirst ? m_aInputs : aTmpInputs).push_back(pPropData->szKey);
					}
				}
			}

			pTable = pTable->pBaseProptable;
		}

		if(!isFirst)
		{
			for(UINT j = 0, jl = m_aInputs.size(); j < jl; ++j)
			{
				if(aTmpInputs.indexOf(m_aInputs[i], [](const char *a, const char *b){
					return(!fstrcmp(a, b));
				}) < 0)
				{
					m_aInputs.erase(j);
					--j;
					--jl;
				}
			}

			aTmpInputs.clearFast();
		}

		isFirst = false;

		if(!m_aInputs.size())
		{
			break;
		}
	}

	const char *szFound = NULL;
	ComboBox_ResetContent(m_hInputCmb);
	for(UINT i = 0, l = m_aInputs.size(); i < l; ++i)
	{
		int idx = ComboBox_AddString(m_hInputCmb, m_aInputs[i]);
		ComboBox_SetItemData(m_hInputCmb, idx, m_aInputs[i]);

		if(!fstrcmp(szTemp, m_aInputs[i]))
		{
			szFound = m_aInputs[i];
		}
	}

	if(szFound)
	{
		// ComboBox_SetText(m_hInputCmb, szTemp);
		int iSel = ComboBox_FindItemData(m_hInputCmb, -1, szFound);
		ComboBox_SetCurSel(m_hInputCmb, iSel);
	}
}

void CEditorOutputsTab::copySelection()
{
	m_aClipBoard.clearFast();

	for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
	{
		Row &row = m_pCurrentCommand->m_aRows[i];
		if(row.isSelected)
		{
			Row &newRow = m_aClipBoard[m_aClipBoard.size()];

			newRow.szOutput = row.szOutput;
			newRow.sTarget = row.sTarget;
			newRow.sInput = row.sInput;
			newRow.sParam = row.sParam;
			newRow.fTime = row.fTime;
			newRow.fTimeTo = row.fTimeTo;
			newRow.isOnceOnly = row.isOnceOnly;
		}
	}

	updateButtons();
}
void CEditorOutputsTab::pasteCopied()
{
	if(!m_aClipBoard.size())
	{
		return;
	}

	for(UINT i = 0, l = m_pCurrentCommand->m_aRows.size(); i < l; ++i)
	{
		m_pCurrentCommand->m_aRows[i].isSelected = false;
	}

	for(UINT i = 0, l = m_aClipBoard.size(); i < l; ++i)
	{
		Row &row = m_aClipBoard[i];
		Row &newRow = m_pCurrentCommand->m_aRows[m_pCurrentCommand->m_aRows.size()];

		newRow.szOutput = row.szOutput;
		newRow.sTarget = row.sTarget;
		newRow.sInput = row.sInput;
		newRow.sParam = row.sParam;
		newRow.fTime = row.fTime;
		newRow.fTimeTo = row.fTimeTo;
		newRow.isOnceOnly = row.isOnceOnly;

		newRow.aObjects = m_pCurrentCommand->m_aEditorObjects;
		newRow.isSelected = true;
	}

	m_pCurrentCommand->m_isDirty = true;

	updateTable();
}
