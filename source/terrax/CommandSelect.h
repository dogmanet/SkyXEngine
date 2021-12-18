#ifndef _COMMAND_SELECT_H_
#define _COMMAND_SELECT_H_

#include <xcommon/editor/IXEditorExtension.h>
#include "terrax.h"

class CCommandSelect final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	~CCommandSelect();

	enum IGNORE_GROUP_MODE
	{
		IGM_NONE,
		IGM_ENABLE,
		IGM_DISABLE
	};

	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override;

	void addSelected(IXEditorObject *pObj);
	void addDeselected(IXEditorObject *pObj);

	void setIGMode(IGNORE_GROUP_MODE mode)
	{
		m_igmode = mode;
	}

	bool XMETHODCALLTYPE isEmpty() override;

protected:
	struct SelObj
	{
		IXEditorObject *pObj;
		bool bToSelect;
	};

	Array<SelObj> m_aObjects;
	IGNORE_GROUP_MODE m_igmode = IGM_NONE;
};

#endif
