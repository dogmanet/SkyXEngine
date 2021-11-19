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

extern HINSTANCE g_hInstance;

//##########################################################################

CEditorBrushTool::CEditorBrushTool(CEditable *pEditable, IXEditor *pEditor):
	m_pEditable(pEditable),
	m_pEditor(pEditor)
{
	m_hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_AB_BLOCK));

	IXRenderUtils *pUtils = (IXRenderUtils*)pEditable->getCore()->getPluginManager()->getInterface(IXRENDERUTILS_GUID);
	pUtils->newGizmoRenderer(&m_pRenderer);
}
CEditorBrushTool::~CEditorBrushTool()
{
	DeleteBitmap(m_hBitmap);
	//mem_release(m_pCurrentCommand);

	mem_release(m_pRenderer);
	mem_delete(m_pNewOutline);

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
	m_isActive = true;
	SAFE_CALL(m_pNewOutline, activate);
}
void XMETHODCALLTYPE CEditorBrushTool::deactivate()
{
	m_isActive = false;
	SAFE_CALL(m_pNewOutline, deactivate);
}




bool XMETHODCALLTYPE CEditorBrushTool::onMouseDown(bool isPrimary)
{
	if(m_pEditor->getViewForWindow(m_pEditor->getState()->activeWindow) == X2D_NONE)
	{
		return(false);
	}
	if(!isPrimary)
	{
		if(m_pNewOutline)
		{
			m_pNewOutline->closePath();
		}
		return(false);
	}
	if(!m_pNewOutline)
	{
		m_pNewOutline = new COutline(m_pEditor, m_pRenderer);
		m_pNewOutline->setPlane(m_pEditor->getState()->vBestPlaneNormal);
	}

	m_isMouseDown = true;

	return(true);
}
bool XMETHODCALLTYPE CEditorBrushTool::onMouseMove()
{
	if(m_pEditor->getViewForWindow(m_pEditor->getState()->activeWindow) == X2D_NONE)
	{
		return(false);
	}

	if(m_pNewOutline)
	{
		m_pNewOutline->setMouse(m_pEditor->getState()->vResolvedWorldMousePos);

		return(true);
	}
	return(false);
}
void XMETHODCALLTYPE CEditorBrushTool::onMouseUp(bool isPrimary)
{
	if(m_isMouseDown)
	{
		m_isMouseDown = false;

		m_pNewOutline->setMouse(m_pEditor->getState()->vResolvedWorldMousePos);

		m_pNewOutline->addPoint();
	}
}

bool XMETHODCALLTYPE CEditorBrushTool::onKeyDown(UINT uKey)
{
	if(uKey == SIK_DELETE)
	{
		if(m_pNewOutline)
		{
			m_pNewOutline->deleteSelected();
			return(true);
		}
	}
	else if(uKey == SIK_ESCAPE)
	{
		if(m_pNewOutline)
		{
			if(!m_pNewOutline->isClosed())
			{
				m_pNewOutline->closePath();
				if(!m_pNewOutline->isClosed())
				{
					mem_delete(m_pNewOutline);
				}
			}
			else
			{
				mem_delete(m_pNewOutline);
			}
			return(true);
		}
	}
	else if(uKey == SIK_ENTER || uKey == SIK_NUMPADENTER)
	{
		if(m_pNewOutline && m_pNewOutline->isClosed() && m_pNewOutline->isValid())
		{
			const char *szMat = m_pEditor->getMaterialBrowser()->getCurrentMaterial();
			// create brush with default height

			float fHeight = m_pEditor->getGridStep();
			if(fHeight <= 0.0f)
			{
				fHeight = 0.2f;
			}

			CEditorObject *pObject = new CEditorObject(m_pEditable);
			for(UINT i = 0, l = m_pNewOutline->getContourCount(); i < l; ++i)
			{
				pObject->addBrush(new CBrushMesh(m_pEditable->getCore(), m_pNewOutline, i, szMat, fHeight));
			}
			mem_delete(m_pNewOutline);
			
			pObject->fixPos();

			CCommandCreate *pCmd = new CCommandCreate(m_pEditor, pObject);
			m_pEditor->execCommand(pCmd);

			return(true);
		}
	}

	return(false);
}
void XMETHODCALLTYPE CEditorBrushTool::onKeyUp(UINT uKey)
{
}

void CEditorBrushTool::render(bool is3D)
{
	if(m_isActive)
	{
		SAFE_CALL(m_pNewOutline, render, is3D);
	}
}
