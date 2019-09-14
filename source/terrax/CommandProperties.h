#ifndef _COMMAND_PROPERTIES_H_
#define _COMMAND_PROPERTIES_H_

#include "Command.h"
#include "terrax.h"

#include <common/assotiativearray.h>
#include <common/string.h>

class CCommandProperties: public CCommand
{
public:
	bool exec();
	bool undo();

	const char *getText()
	{
		return("change props");
	}

	void addObject(ID idObject);
	void setKV(const char *szKey, const char *szValue);

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
};

#endif
