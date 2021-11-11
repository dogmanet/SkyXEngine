#ifndef _UNDO_MANAGER_H_
#define _UNDO_MANAGER_H_

#include <common/array.h>
#include <xcommon/editor/IXEditorExtension.h>

class CUndoManager
{
public:
	~CUndoManager();

	bool canUndo();
	bool canRedo();

	const char *getUndoText();
	const char *getRedoText();

	bool execCommand(IXEditorCommand *pCommand);
	bool undo();
	bool redo();

	void reset();
	void flushRedo();

	bool isDirty();
	void makeClean();

	bool isInCommandContext();
protected:
	Array<IXEditorCommand*> m_stackUndo;
	Array<IXEditorCommand*> m_stackRedo;

	int m_iLastSaveIndex = 0;

	bool m_isInCommandContext = false;
};

#endif
