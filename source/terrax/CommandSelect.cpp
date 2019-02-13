#include "CommandSelect.h"

bool CCommandSelect::exec()
{
	for(UINT i = 0, l = m_aidSelected.size(); i < l; ++i)
	{
		g_pLevelObjects[m_aidSelected[i]]->setSelected(true);
	}
	for(UINT i = 0, l = m_aidDeselected.size(); i < l; ++i)
	{
		g_pLevelObjects[m_aidDeselected[i]]->setSelected(false);
	}
	return(true);
}
bool CCommandSelect::undo()
{
	for(UINT i = 0, l = m_aidSelected.size(); i < l; ++i)
	{
		g_pLevelObjects[m_aidSelected[i]]->setSelected(false);
	}
	for(UINT i = 0, l = m_aidDeselected.size(); i < l; ++i)
	{
		g_pLevelObjects[m_aidDeselected[i]]->setSelected(true);
	}

	return(true);
}

const char *CCommandSelect::getText()
{
	return("select");
}

void CCommandSelect::addSelected(ID idObject)
{
	for(UINT i = 0, l = m_aidDeselected.size(); i < l; ++i)
	{
		if(m_aidDeselected[i] == idObject)
		{
			m_aidDeselected.erase(i);
			break;
		}
	}
	for(UINT i = 0, l = m_aidSelected.size(); i < l; ++i)
	{
		if(m_aidSelected[i] == idObject)
		{
			return;
		}
	}
	m_aidSelected.push_back(idObject);
}
void CCommandSelect::addDeselected(ID idObject)
{
	for(UINT i = 0, l = m_aidSelected.size(); i < l; ++i)
	{
		if(m_aidSelected[i] == idObject)
		{
			m_aidSelected.erase(i);
			break;
		}
	}
	for(UINT i = 0, l = m_aidDeselected.size(); i < l; ++i)
	{
		if(m_aidDeselected[i] == idObject)
		{
			return;
		}
	}
	m_aidDeselected.push_back(idObject);
}
