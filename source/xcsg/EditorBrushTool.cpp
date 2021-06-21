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

//##########################################################################

CEditorBrushTool::CEditorBrushTool(CEditable *pEditable):
	m_pEditable(pEditable)
{
	m_hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
}
CEditorBrushTool::~CEditorBrushTool()
{
	DeleteBitmap(m_hBitmap);
	//mem_release(m_pCurrentCommand);

	//if(m_hDlg)
	//{
	//	DestroyWindow(m_hDlg);
	//}

	//if(m_pCallback)
	//{
	//	m_pCallback->onRemove();
	//}
}

void* XMETHODCALLTYPE CEditorBrushTool::getIcon()
{
	return(m_hBitmap);
}
const char* XMETHODCALLTYPE CEditorBrushTool::getTitle()
{
	return("Brush tool");
}
XAccelItem XMETHODCALLTYPE CEditorBrushTool::getAccel()
{
	XAccelItem xaf = {XAF_SHIFT | XAF_VIRTKEY, 'B'};
	return(xaf);
}

void XMETHODCALLTYPE CEditorBrushTool::activate()
{
}
void XMETHODCALLTYPE CEditorBrushTool::deactivate()
{
}

bool XMETHODCALLTYPE CEditorBrushTool::wantMouse2D()
{
	return(false);
}
bool XMETHODCALLTYPE CEditorBrushTool::wantMouse3D()
{
	return(false);
}


bool XMETHODCALLTYPE CEditorBrushTool::onMouseDown()
{
	return(false);
}
bool XMETHODCALLTYPE CEditorBrushTool::onMouseMove()
{
	return(false);
}
void XMETHODCALLTYPE CEditorBrushTool::onMouseUp()
{
}
