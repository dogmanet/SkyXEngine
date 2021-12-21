#include "BrushCreatorFree.h"
#include "EditorExtension.h"
#include "EditorObject.h"
#include "Editable.h"
#include "CommandCreate.h"

//##########################################################################

CBrushCreatorFree::CBrushCreatorFree(CEditable *pEditable, IXEditor *pEditor, IXGizmoRenderer *pRenderer):
	m_pEditable(pEditable),
	m_pEditor(pEditor),
	m_pRenderer(pRenderer)
{
	add_ref(pRenderer);

	m_pEditable->getCore()->getConsole()->registerCVar("csg_free_auto_create", false, "Finish free contour creation without ENTER");
}
CBrushCreatorFree::~CBrushCreatorFree()
{
	mem_release(m_pRenderer);
	mem_delete(m_pNewOutline);
}

bool CBrushCreatorFree::isInitiated()
{
	return(m_pNewOutline != NULL);
}
void CBrushCreatorFree::init(const float3 &vNormal)
{
	if(m_pEditor->getViewForWindow(m_pEditor->getState()->activeWindow) == X2D_NONE)
	{
		return;
	}

	assert(!m_pNewOutline);
	mem_delete(m_pNewOutline);

	m_pNewOutline = new COutline(m_pEditor, m_pRenderer);
	m_pNewOutline->setPlane(vNormal);
}

void CBrushCreatorFree::activate()
{
	SAFE_CALL(m_pNewOutline, activate);
}
void CBrushCreatorFree::deactivate()
{
	SAFE_CALL(m_pNewOutline, deactivate);
}

bool CBrushCreatorFree::onMouseDown(bool isPrimary)
{
	if(m_pEditor->getViewForWindow(m_pEditor->getState()->activeWindow) == X2D_NONE)
	{
		return(false);
	}
	if(!isPrimary)
	{
		SAFE_CALL(m_pNewOutline, closePath);

		static const bool *csg_free_auto_create = m_pEditable->getCore()->getConsole()->getPCVarBool("csg_free_auto_create");
		if(*csg_free_auto_create)
		{
			m_pNewOutline->buildContours();
			onKeyDown(SIK_ENTER);
		}

		return(false);
	}
	
	m_isMouseDown = true;

	return(true);
}
bool CBrushCreatorFree::onMouseMove()
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
void CBrushCreatorFree::onMouseUp(bool isPrimary)
{
	if(m_isMouseDown)
	{
		m_isMouseDown = false;

		m_pNewOutline->setMouse(m_pEditor->getState()->vResolvedWorldMousePos);

		m_pNewOutline->addPoint();
	}
}

bool CBrushCreatorFree::onKeyDown(UINT uKey)
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
void CBrushCreatorFree::onKeyUp(UINT uKey)
{
}

void CBrushCreatorFree::render(bool is3D)
{
	SAFE_CALL(m_pNewOutline, render, is3D);
}

UINT CBrushCreatorFree::getClassCount()
{
	return(1);
}

const char* CBrushCreatorFree::getClassAt(UINT idx)
{
	switch(idx)
	{
	case 0:
		return("Free draw");
	}
	assert(!"Invalid class idx");
	return(NULL);
}

bool CBrushCreatorFree::useClass(const char *szClassName)
{
	return(true);
}
