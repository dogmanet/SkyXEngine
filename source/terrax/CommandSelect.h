#ifndef _COMMAND_SELECT_H_
#define _COMMAND_SELECT_H_

#include <xcommon/editor/IXEditorExtension.h>
#include "terrax.h"

class CCommandSelect final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override;

	void addSelected(ID idObject);
	void addDeselected(ID idObject);

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(false);
	}

protected:
	Array<ID> m_aidSelected;
	Array<ID> m_aidDeselected;
};

#endif
