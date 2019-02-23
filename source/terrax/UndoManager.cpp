#include "UndoManager.h"

CUndoManager::~CUndoManager()
{
	reset();
}

bool CUndoManager::canUndo()
{
	return(m_stackUndo.size() != 0);
}

bool CUndoManager::canRedo()
{
	return(m_stackRedo.size() != 0);
}

const char *CUndoManager::getUndoText()
{
	if(!canUndo())
	{
		return(NULL);
	}

	return(m_stackUndo[m_stackUndo.size() - 1]->getText());
}
const char *CUndoManager::getRedoText()
{
	if(!canRedo())
	{
		return(NULL);
	}

	return(m_stackRedo[m_stackRedo.size() - 1]->getText());
}

void CUndoManager::reset()
{
	for(UINT i = 0, l = m_stackRedo.size(); i < l; ++i)
	{
		mem_delete(m_stackRedo[i]);
	}
	m_stackRedo.clearFast();

	for(UINT i = 0, l = m_stackUndo.size(); i < l; ++i)
	{
		mem_delete(m_stackUndo[i]);
	}
	m_stackUndo.clearFast();
}
void CUndoManager::flushRedo()
{
	for(UINT i = 0, l = m_stackRedo.size(); i < l; ++i)
	{
		mem_delete(m_stackRedo[i]);
	}
	m_stackRedo.clearFast();
}

bool CUndoManager::execCommand(CCommand *pCommand)
{
	if(pCommand->exec())
	{
		flushRedo();
		m_stackUndo.push_back(pCommand);
		return(true);
	}
	mem_delete(pCommand);
	return(false);
}
bool CUndoManager::undo()
{
	if(!canUndo())
	{
		return(false);
	}

	CCommand *pCmd = m_stackUndo[m_stackUndo.size() - 1];
	if(pCmd->undo())
	{
		m_stackUndo.erase(m_stackUndo.size() - 1);
		m_stackRedo.push_back(pCmd);
		return(true);
	}
	return(false);
}
bool CUndoManager::redo()
{
	if(!canRedo())
	{
		return(false);
	}

	CCommand *pCmd = m_stackRedo[m_stackRedo.size() - 1];
	if(pCmd->exec())
	{
		m_stackRedo.erase(m_stackRedo.size() - 1);
		m_stackUndo.push_back(pCmd);
		return(true);
	}
	return(false);
}
