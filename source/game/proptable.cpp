
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <cstdarg>
#include <memory>

#include "proptable.h"

#include "BaseEntity.h"

prop_editor_t _GetEditorCombobox(int start, ...)
{
	prop_editor_t out;
	out.type = PDE_COMBOBOX;
	editor_kv kvs[ED_COMBO_MAXELS];

	va_list va;
	va_start(va, start);

	const char * el;
	int i = 0;
	while(i < ED_COMBO_MAXELS)
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

prop_editor_t _GetEditorFilefield(int start, ...)
{
	prop_editor_t out;
	out.type = PDE_FILEFIELD;
	editor_kv kvs[ED_COMBO_MAXELS];

	va_list va;
	va_start(va, start);

	const char * el;
	int i = 0;
	while(i < ED_COMBO_MAXELS)
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

void output_t::fire(CBaseEntity *pInflictor, CBaseEntity *pActivator)
{
	inputdata_t data = {0};
	data.pActivator = pActivator;
	data.pInflictor = pInflictor;
	for(int i = 0; i < iOutCount; ++i)
	{
		if(pOutputs[i].fDelay == 0.0f)
		{
			for(int j = 0; j < pOutputs[i].iOutCount; ++j)
			{
				data.parameter = pOutputs[i].pOutputs[j].data.parameter;
				data.type = pOutputs[i].pOutputs[j].data.type;
				data.v3Parameter = pOutputs[i].pOutputs[j].data.v3Parameter;

				(pOutputs[i].pOutputs[j].pTarget->*(pOutputs[i].pOutputs[j].fnInput))(&data);
			}
		}
		else
		{
			pActivator->getManager()->setOutputTimeout(&pOutputs[i], &data);
		}
	}
}
