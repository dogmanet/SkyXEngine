#include <cstdarg>
#include <memory>

#include "proptable.h"

prop_editor_t _GetEditorCombobox(int start, ...)
{
	prop_editor_t out;
	out.type = PDE_COMBOBOX;
	editor_kv kvs[ED_COMBO_MAXELS];

	va_list va;
	va_start(va, start);

	const char * el;
	int i = 0;
	while(1)
	{
		if(!(el = va_arg(va, const char *)))
		{
			break;
		}
		kvs[i].key = el;
		if(!(el = va_arg(va, const char *)))
		{
			break;
		}
		kvs[i].value = el;
		++i;
	}
	kvs[i].value = kvs[i].key = 0;
	++i;

	va_end(va);

	out.pData = new editor_kv[i];
	memcpy(out.pData, kvs, sizeof(editor_kv)* i);
	return(out);
}

const char * GetEmptyString()
{
	static const char * str = "";
	return(str);
}
