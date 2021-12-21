#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <Commdlg.h>

#include "EditorExtension.h"
#include "resource.h"
#include "EditorObject.h"
#include "Editable.h"
#include "CommandCreate.h"

#include <xcommon/IPluginManager.h>

#include "BrushCreatorBox.h"
#include "BrushCreatorFree.h"

extern HINSTANCE g_hInstance;

//##########################################################################

CEditorBrushTool::CEditorBrushTool(CEditable *pEditable, IXEditor *pEditor):
	m_pEditable(pEditable),
	m_pEditor(pEditor)
{
	m_hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_AB_BLOCK));

	IXRenderUtils *pUtils = (IXRenderUtils*)pEditable->getCore()->getPluginManager()->getInterface(IXRENDERUTILS_GUID);
	pUtils->newGizmoRenderer(&m_pRenderer);

	registerCreator(new CBrushCreatorBox(pEditable, pEditor, m_pRenderer));
	registerCreator(new CBrushCreatorFree(pEditable, pEditor, m_pRenderer));
}
CEditorBrushTool::~CEditorBrushTool()
{
	DeleteBitmap(m_hBitmap);
	//mem_release(m_pCurrentCommand);

	mem_release(m_pRenderer);

	fora(i, m_aCreators)
	{
		mem_release(m_aCreators[i].pCreator);
	}
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
	m_isActive = true;
	m_pCreator->activate();
}
void XMETHODCALLTYPE CEditorBrushTool::deactivate()
{
	m_isActive = false;
	m_pCreator->deactivate();
}




bool XMETHODCALLTYPE CEditorBrushTool::onMouseDown(bool isPrimary)
{
	if(!m_pCreator->isInitiated())
	{
		m_pCreator->init(m_pEditor->getState()->vBestPlaneNormal);
	}
	return(m_pCreator->onMouseDown(isPrimary));
}
bool XMETHODCALLTYPE CEditorBrushTool::onMouseMove()
{
	return(m_pCreator->onMouseMove());
}
void XMETHODCALLTYPE CEditorBrushTool::onMouseUp(bool isPrimary)
{
	m_pCreator->onMouseUp(isPrimary);
}

bool XMETHODCALLTYPE CEditorBrushTool::onKeyDown(UINT uKey)
{
	return(m_pCreator->onKeyDown(uKey));
}
void XMETHODCALLTYPE CEditorBrushTool::onKeyUp(UINT uKey)
{
	m_pCreator->onKeyUp(uKey);
}

void CEditorBrushTool::render(bool is3D)
{
	if(m_isActive)
	{
		m_pCreator->render(is3D);
	}
}

UINT XMETHODCALLTYPE CEditorBrushTool::getClassCount()
{
	return(m_aCreators.size());
}

const char* XMETHODCALLTYPE CEditorBrushTool::getClassAt(UINT idx)
{
	assert(idx < m_aCreators.size());
	if(idx < m_aCreators.size())
	{
		return(m_aCreators[idx].szClassName);
	}
	return(NULL);
	switch(idx)
	{
	case 0:
		return("Block");
	case 1:
		return("Cylinder");
	case 2:
		return("Spike");
	case 3:
		return("Free draw");
	}
	assert(!"Invalid class idx");
	return(NULL);
}

bool XMETHODCALLTYPE CEditorBrushTool::useClass(const char *szClassName)
{
	int idx = m_aCreators.indexOf(szClassName, [](const Creator &a, const char *b){
		return(!strcmp(a.szClassName, b));
	});

	if(idx >= 0)
	{
		IBrushCreator *pNewCreator = m_aCreators[idx].pCreator;

		if(!m_pCreator || m_pCreator == pNewCreator || !m_pCreator->isInitiated())
		{
			m_pCreator = pNewCreator;
			return(m_pCreator->useClass(szClassName));
		}
	}

	return(false);
}

void CEditorBrushTool::registerCreator(IBrushCreator *pCreator)
{
	for(UINT i = 0, l = pCreator->getClassCount(); i < l; ++i)
	{
		add_ref(pCreator);
		m_aCreators.push_back({pCreator, pCreator->getClassAt(i)});
	}
	mem_release(pCreator);
}
