#include "EditorExtension.h"
#include "EditorObject.h"
#include "Editable.h"

CEditorExtension::CEditorExtension(CEditable *pEditable, IXEditor *pEditor):
	m_pEditable(pEditable),
	m_pBrushTool(new CEditorBrushTool(pEditable, pEditor))
{
}
CEditorExtension::~CEditorExtension()
{
	mem_release(m_pBrushTool);
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
	return(1);
}
bool XMETHODCALLTYPE CEditorExtension::getTool(UINT uId, IXEditorTool **ppOut)
{
	switch(uId)
	{
	case 0:
		add_ref(m_pBrushTool);
		*ppOut = m_pBrushTool;
		return(true);
	}
	return(false);
}

void XMETHODCALLTYPE CEditorExtension::render(bool is3D)
{
	m_pBrushTool->render(is3D);
}

//void CEditorExtension::onSelectionChanged(CEditorObject *pObject)
//{
//	auto &aObjects = m_pEditable->getObjects();
//
//	static UINT s_uLastSelected = 0;
//
//	UINT uSelected = 0;
//	for(UINT i = 0, l = aObjects.size(); i < l; ++i)
//	{
//		if(aObjects[i]->isSelected())
//		{
//			++uSelected;
//		}
//	}
//
//	if(uSelected != s_uLastSelected)
//	{
//		s_uLastSelected = uSelected;
//		
//		m_pOutputsTab->setEnabled(uSelected != 0);
//	}
//
//	if(uSelected != 0)
//	{
//		m_pOutputsTab->onSelectionChanged();
//	}
//}
