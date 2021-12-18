#include "CommandSelect.h"
#include "resource.h"

CCommandSelect::~CCommandSelect()
{
	fora(i, m_aObjects)
	{
		mem_release(m_aObjects[i].pObj);
	}
}

bool XMETHODCALLTYPE CCommandSelect::exec()
{
	fora(i, m_aObjects)
	{
		m_aObjects[i].pObj->setSelected(m_aObjects[i].bToSelect);
	}
	XUpdatePropWindow();
	return(true);
}
bool XMETHODCALLTYPE CCommandSelect::undo()
{
	if(m_igmode != IGM_NONE)
	{
		g_xConfig.m_bIgnoreGroups = m_igmode == IGM_ENABLE;
		CheckToolbarButton(ID_IGNORE_GROUPS, g_xConfig.m_bIgnoreGroups);
	}

	forar(i, m_aObjects)
	{
		m_aObjects[i].pObj->setSelected(!m_aObjects[i].bToSelect);
	}

	XUpdatePropWindow();
	return(true);
}

const char* CCommandSelect::getText()
{
	return("select");
}

void CCommandSelect::addSelected(IXEditorObject *pObj)
{
	int idx = m_aObjects.indexOf(pObj, [](const SelObj &a, IXEditorObject *b){
		return(a.pObj == b);
	});
	if(idx >= 0)
	{
		m_aObjects[idx].bToSelect = true;
	}
	else
	{
		add_ref(pObj);
		m_aObjects.push_back({pObj, true});
	}
}
void CCommandSelect::addDeselected(IXEditorObject *pObj)
{
	int idx = m_aObjects.indexOf(pObj, [](const SelObj &a, IXEditorObject *b){
		return(a.pObj == b);
	});
	if(idx >= 0)
	{
		m_aObjects[idx].bToSelect = false;
	}
	else
	{
		add_ref(pObj);
		m_aObjects.push_back({pObj, false});
	}
}

bool XMETHODCALLTYPE CCommandSelect::isEmpty()
{
	return(m_aObjects.size() == 0 && m_igmode == IGM_NONE);
}
