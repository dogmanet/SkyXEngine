#include "EditorExtension.h"
#include "Editable.h"

extern HINSTANCE g_hInstance;

CEditorExtension::CEditorExtension(CEditable *pEditable, IXEditor *pEditor, IXCore *pCore):
	m_pEditable(pEditable)
{
	m_pEffectBrowserWindow = new CEffectBrowserWindow(g_hInstance, (HWND)pEditor->getMainWindow(), pCore->getFileSystem());
}
CEditorExtension::~CEditorExtension()
{
	mem_delete(m_pEffectBrowserWindow);
}

UINT XMETHODCALLTYPE CEditorExtension::getPropertyTabCount()
{
	return(0);
}
IXEditorPropertyTab* XMETHODCALLTYPE CEditorExtension::getPropertyTab(UINT uId)
{
	return(false);
}

UINT XMETHODCALLTYPE CEditorExtension::getToolCount()
{
	return(0);
}
bool XMETHODCALLTYPE CEditorExtension::getTool(UINT uId, IXEditorTool **ppOut)
{
	return(false);
}

void XMETHODCALLTYPE CEditorExtension::render(bool is3D)
{
}
