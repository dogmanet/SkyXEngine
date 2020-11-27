#ifndef _COMMAND_PROPERTIES_H_
#define _COMMAND_PROPERTIES_H_

#include <xcommon/editor/IXEditorExtension.h>
#include "terrax.h"

#include <common/assotiativearray.h>
#include <common/string.h>

class CCommandProperties final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	~CCommandProperties()
	{
		for(UINT i = 0, l = m_aCustomTabCommands.size(); i < l; ++i)
		{
			mem_release(m_aCustomTabCommands[i]);
		}
	}

	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	bool XMETHODCALLTYPE isEmpty() override
	{
		if(!m_isEmpty)
		{
			return(false);
		}

		for(UINT i = 0, l = m_aCustomTabCommands.size(); i < l; ++i)
		{
			if(!m_aCustomTabCommands[i]->isEmpty())
			{
				return(false);
			}
		}

		return(true);
	}

	const char* XMETHODCALLTYPE getText() override
	{
		return("change props");
	}

	void addObject(ID idObject);
	void setKV(const char *szKey, const char *szValue);

	void addInnerCommand(IXEditorCommand *pCmd)
	{
		m_aCustomTabCommands.push_back(pCmd);
	}

protected:

	struct _prop_kv
	{
		String sOld;
		String sNew;
		bool isChanged = false;
	};

	struct _prop_obj
	{
		IXEditorObject *pObject;
		AssotiativeArray<String, _prop_kv> mKeyValues;
	};
	Array<_prop_obj> m_aObjects;

	Array<IXEditorCommand*> m_aCustomTabCommands;

	bool m_isEmpty = true;
};

#endif
