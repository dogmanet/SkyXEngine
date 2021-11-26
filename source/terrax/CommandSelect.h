#ifndef _COMMAND_SELECT_H_
#define _COMMAND_SELECT_H_

#include <xcommon/editor/IXEditorExtension.h>
#include "terrax.h"

class CCommandSelect final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	~CCommandSelect();

	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override;

	void addSelected(IXEditorObject *pObj);
	void addDeselected(IXEditorObject *pObj);

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(false);
	}

protected:
	Array<IXEditorObject*> m_aidSelected;
	Array<IXEditorObject*> m_aidDeselected;
};

#endif
