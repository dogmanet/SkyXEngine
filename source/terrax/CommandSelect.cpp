#include "CommandSelect.h"

CCommandSelect::~CCommandSelect()
{
	for(UINT i = 0, l = m_aidSelected.size(); i < l; ++i)
	{
		mem_release(m_aidSelected[i]);
	}
	for(UINT i = 0, l = m_aidDeselected.size(); i < l; ++i)
	{
		mem_release(m_aidDeselected[i]);
	}
}

bool XMETHODCALLTYPE CCommandSelect::exec()
{
	for(UINT i = 0, l = m_aidSelected.size(); i < l; ++i)
	{
		m_aidSelected[i]->setSelected(true);
	}
	for(UINT i = 0, l = m_aidDeselected.size(); i < l; ++i)
	{
		m_aidDeselected[i]->setSelected(false);
	}
	XUpdatePropWindow();
	return(true);
}
bool XMETHODCALLTYPE CCommandSelect::undo()
{
	for(UINT i = 0, l = m_aidSelected.size(); i < l; ++i)
	{
		m_aidSelected[i]->setSelected(false);
	}
	for(UINT i = 0, l = m_aidDeselected.size(); i < l; ++i)
	{
		m_aidDeselected[i]->setSelected(true);
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
	add_ref(pObj);

	int idx = m_aidDeselected.indexOf(pObj);
	if(idx >= 0)
	{
		mem_release(m_aidDeselected[idx]);
		m_aidDeselected.erase(idx);
	}
	
	idx = m_aidSelected.indexOf(pObj);
	if(idx < 0)
	{
		m_aidSelected.push_back(pObj);
	}
}
void CCommandSelect::addDeselected(IXEditorObject *pObj)
{
	add_ref(pObj);

	int idx = m_aidSelected.indexOf(pObj);
	if(idx >= 0)
	{
		mem_release(m_aidSelected[idx]);
		m_aidSelected.erase(idx);
	}

	idx = m_aidDeselected.indexOf(pObj);
	if(idx < 0)
	{
		m_aidDeselected.push_back(pObj);
	}
}
