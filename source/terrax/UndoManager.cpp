#include "UndoManager.h"

void XUpdateWindowTitle();

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
		mem_release(m_stackRedo[i]);
	}
	m_stackRedo.clearFast();

	for(UINT i = 0, l = m_stackUndo.size(); i < l; ++i)
	{
		mem_release(m_stackUndo[i]);
	}
	m_stackUndo.clearFast();

	m_iLastSaveIndex = 0;

	XUpdateWindowTitle();
}
void CUndoManager::flushRedo()
{
	for(UINT i = 0, l = m_stackRedo.size(); i < l; ++i)
	{
		mem_release(m_stackRedo[i]);
	}
	m_stackRedo.clearFast();

	if(m_iLastSaveIndex > m_stackUndo.size())
	{
		m_iLastSaveIndex = -1;
	}
}

bool CUndoManager::execCommand(IXEditorCommand *pCommand)
{
	m_isInCommandContext = true;
	if(!pCommand->isEmpty() && pCommand->exec())
	{
		m_isInCommandContext = false;
		flushRedo();
		m_stackUndo.push_back(pCommand);
		XUpdateWindowTitle();
		return(true);
	}
	m_isInCommandContext = false;
	mem_release(pCommand);
	return(false);
}
bool CUndoManager::undo()
{
	if(!canUndo() || isInCommandContext())
	{
		return(false);
	}

	IXEditorCommand *pCmd = m_stackUndo[m_stackUndo.size() - 1];
	m_isInCommandContext = true;
	if(pCmd->undo())
	{
		m_isInCommandContext = false;
		m_stackUndo.erase(m_stackUndo.size() - 1);
		m_stackRedo.push_back(pCmd);
		XUpdateWindowTitle();
		return(true);
	}
	m_isInCommandContext = false;
	return(false);
}
bool CUndoManager::redo()
{
	if(!canRedo() || isInCommandContext())
	{
		return(false);
	}

	IXEditorCommand *pCmd = m_stackRedo[m_stackRedo.size() - 1];
	m_isInCommandContext = true;
	if(pCmd->exec())
	{
		m_isInCommandContext = false;
		m_stackRedo.erase(m_stackRedo.size() - 1);
		m_stackUndo.push_back(pCmd);
		XUpdateWindowTitle();
		return(true);
	}
	m_isInCommandContext = false;
	return(false);
}

bool CUndoManager::isDirty()
{
	return(m_stackUndo.size() != m_iLastSaveIndex);
}

void CUndoManager::makeClean()
{
	m_iLastSaveIndex = m_stackUndo.size();
	XUpdateWindowTitle();
}

bool CUndoManager::isInCommandContext()
{
	return(m_isInCommandContext);
}
