#ifndef _UNDO_MANAGER_H_
#define _UNDO_MANAGER_H_

#include <common/array.h>
#include "Command.h"

class CUndoManager
{
public:
	~CUndoManager();

	bool canUndo();
	bool canRedo();

	const char *getUndoText();
	const char *getRedoText();

	bool execCommand(CCommand *pCommand);
	bool undo();
	bool redo();

	void reset();
	void flushRedo();
protected:
	Array<CCommand*> m_stackUndo;
	Array<CCommand*> m_stackRedo;

};

#endif