#ifndef __EDITOR_OUTPUTS_TAB_H
#define __EDITOR_OUTPUTS_TAB_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>
#include "GameData.h"

class CEditable;
class CEditorObject;

class CEditorOutputsTab final: public IXUnknownImplementation<IXEditorPropertyTab>
{
public:
	CEditorOutputsTab(CEditable *pEditable);
	~CEditorOutputsTab();

	void XMETHODCALLTYPE init(HWND hWndParent, int X, int Y, int cx, int cy) override;
	const char* XMETHODCALLTYPE getTitle() override;

	void XMETHODCALLTYPE setCallback(IXEditorPropertyTabCallback *pCallback) override;

	void XMETHODCALLTYPE show() override;
	void XMETHODCALLTYPE hide() override;

	IXEditorCommand* XMETHODCALLTYPE startNewCommand() override;

	bool XMETHODCALLTYPE isEnabled() override;

	void setEnabled(bool isEnabled);

	void onSelectionChanged();

protected:
	INT_PTR CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK EditEnterDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static WNDPROC m_pfnEditProc;

	static LRESULT CALLBACK FilteredComboBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static WNDPROC m_pfnFilteredComboBoxOldWndproc;

	static LRESULT CALLBACK ButtonWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static WNDPROC m_pfnButtonOldWndproc;

	void updateTable();
	void updateButtons();
	void updateInputs();

	void filterOutputsList(const char *szFilter);
	void filterTargetsList(const char *szFilter);
	void filterInputsList(const char *szFilter);

	void initSelection();

	void copySelection();
	void pasteCopied();

	struct Row
	{
		const char *szOutput = "";
		String sTarget;
		String sInput;
		String sParam = ENT_OUTPUT_PARAM_NONE;
		float fTime = 0.0f;
		float fTimeTo = 0.0f;
		bool isOnceOnly = false;

		bool isSelected = false;
		Array<CEditorObject*> aObjects;
	};

	class CEditorCommand final: public IXUnknownImplementation<IXEditorCommand>
	{
	public:
		~CEditorCommand();

		bool XMETHODCALLTYPE exec() override;
		bool XMETHODCALLTYPE undo() override;

		bool XMETHODCALLTYPE isEmpty() override
		{
			return(!m_isDirty);
		}

		const char* XMETHODCALLTYPE getText() override
		{
			return("");
		}

		Array<CEditorObject*> m_aEditorObjects;
		Array<Row> m_aRows;

		bool m_isDirty = false;

		void prepare();
		void addObjectOutput(CEditorObject *pObject, const char *szOutput);

	protected:
		void compile();

	private:

		bool m_isCompiled = false;

		struct Output
		{
			const char *szOutput;
			String sValue;
		};

		struct Object
		{
			CEditorObject *pObject;
			Array<Output> aOutputs;
		};

		Array<Object> m_aObjects;
		Array<Object> m_aPrevObjects;
	};

private:
	CEditable *m_pEditable;

	HWND m_hDlg = NULL;


	IXEditorPropertyTabCallback *m_pCallback = NULL;

	CEditorCommand *m_pCurrentCommand = NULL;

	bool m_isEnabled = false;

	Array<const char*> m_aOutputs;
	Array<const char*> m_aNames;
	Array<const char*> m_aInputs;

	HWND m_hOutputsList = NULL;

	HWND m_hOutputCmb = NULL;
	HWND m_hTargetCmb = NULL;
	HWND m_hInputCmb = NULL;
	HWND m_hParameterCmb = NULL;
	HWND m_hTimeEd = NULL;
	HWND m_hTimeToEd = NULL;
	HWND m_hFireOnceCb = NULL;

	HWND m_hAddBtn = NULL;
	HWND m_hCopyBtn = NULL;
	HWND m_hPasteBtn = NULL;
	HWND m_hDeleteBtn = NULL;

	bool m_isSkipUpdate = false;
	bool m_isSkipUpdateInputs = false;

	Array<Row> m_aClipBoard;
};

#endif
