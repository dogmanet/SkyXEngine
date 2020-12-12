
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

void output_t::fire(CBaseEntity *pInflictor, CBaseEntity *pActivator, inputdata_t *pInputData)
{
	inputdata_t data = {0};
	data.pActivator = pActivator;
	data.pInflictor = pInflictor;
	for(int i = 0; i < iOutCount; ++i)
	{
		data.type = PDF_NONE;
		if(pOutputs[i].fDelay == 0.0f && !pOutputs[i].useRandomDelay)
		{
			for(int j = 0; j < pOutputs[i].iOutCount; ++j)
			{
				data.type = pOutputs[i].pOutputs[j].data.type;
				
				if(data.type == PDF_STRING)
				{
					data.parameter.str = NULL;
				}

				if(pOutputs[i].pOutputs[j].useOverrideData)
				{
					data.setParameter(pOutputs[i].pOutputs[j].data);
				}
				else if(pInputData)
				{
					data.setParameter(*pInputData);
				}

				(pOutputs[i].pOutputs[j].pTarget->*(pOutputs[i].pOutputs[j].fnInput))(&data);

				if(data.type == PDF_STRING && data.parameter.str != GetEmptyString())
				{
					delete[] data.parameter.str;
				}
			}
		}
		else
		{
			if(pInputData)
			{
				data.type = pInputData->type;
				data.setParameter(*pInputData);
			}
			pActivator->getManager()->setOutputTimeout(&pOutputs[i], &data);
		}
	}
}


void _setStrVal(const char **to, const char *value)
{
	char * str = (char*)*to;
	if(str && !fstrcmp(str, value))
	{
		return;
	}
	if(!value[0])
	{
		if(str && str != GetEmptyString())
		{
			delete[] str;
		}
		*to = GetEmptyString();
	}
	else
	{
		size_t len = strlen(value);
		if(!str || strlen(str) < len)
		{
			if(str && str != GetEmptyString())
			{
				delete[] str;
			}
			str = new char[len + 1];
		}
		memcpy(str, value, sizeof(char)* (len + 1));
		*to = str;
	}
}

void inputdata_t::setParameter(const inputdata_t &other)
{
	if(type == other.type)
	{
		if(type == PDF_VECTOR)
		{
			v3Parameter = other.v3Parameter;
		}
		else if(type == PDF_VECTOR4)
		{
			v4Parameter = other.v4Parameter;
		}
		else if(type == PDF_STRING)
		{
			_setStrVal(&parameter.str, other.parameter.str);
		}
		else
		{
			parameter = other.parameter;
		}
		return;
	}

	switch(type)
	{
	case PDF_BOOL:
		switch(other.type)
		{
		case PDF_INT:
			parameter.b = other.parameter.i != 0;
			break;
		case PDF_FLOAT:
			parameter.b = other.parameter.f != 0.0f;
			break;
		case PDF_VECTOR:
			parameter.b = SMVector3Length2(other.v3Parameter) > 0.0f;
			break;
		case PDF_VECTOR4:
			parameter.b = SMVector4Length(other.v4Parameter) > 0.0f;
			break;
		case PDF_STRING:
			parameter.b = other.parameter.str
				&& other.parameter.str[0]
				&& fstrcmp(other.parameter.str, "false")
				&& fstrcmp(other.parameter.str, "0")
				&& fstrcmp(other.parameter.str, "null");
			break;
		}
		break;

	case PDF_INT:
		switch(other.type)
		{
		case PDF_BOOL:
			parameter.i = other.parameter.b ? 1 : 0;
			break;
		case PDF_FLOAT:
			parameter.i = (int)other.parameter.f;
			break;
		case PDF_VECTOR:
			parameter.i = (int)other.v3Parameter.x;
			break;
		case PDF_VECTOR4:
			parameter.i = (int)other.v4Parameter.x;
			break;
		case PDF_STRING:
			if(!sscanf(other.parameter.str, "%d", &parameter.i))
			{
				parameter.i = 0;
			}
			break;
		}
		break;

	case PDF_FLOAT:
		switch(other.type)
		{
		case PDF_BOOL:
			parameter.f = other.parameter.b ? 1.0f : 0.0f;
			break;
		case PDF_INT:
			parameter.f = (float)other.parameter.i;
			break;
		case PDF_VECTOR:
			parameter.f = other.v3Parameter.x;
			break;
		case PDF_VECTOR4:
			parameter.f = other.v4Parameter.x;
			break;
		case PDF_STRING:
			if(!sscanf(other.parameter.str, "%f", &parameter.f))
			{
				parameter.f = 0.0f;
			}
			break;
		}
		break;

	case PDF_VECTOR:
		switch(other.type)
		{
		case PDF_BOOL:
			v3Parameter = float3(other.parameter.b ? 1.0f : 0.0f);
			break;
		case PDF_INT:
			v3Parameter = float3((float)other.parameter.i);
			break;
		case PDF_FLOAT:
			v3Parameter = float3(other.parameter.f);
			break;
		case PDF_VECTOR4:
			v3Parameter = float3(other.v4Parameter);
			break;
		case PDF_STRING:
			if(!sscanf(other.parameter.str, "%f %f %f", &v3Parameter.x, &v3Parameter.y, &v3Parameter.z))
			{
				v3Parameter = float3(0.0f);
			}
			break;
		}
		break;

	case PDF_VECTOR4:
		switch(other.type)
		{
		case PDF_BOOL:
			v4Parameter = float4(other.parameter.b ? 1.0f : 0.0f);
			break;
		case PDF_INT:
			v4Parameter = float4((float)other.parameter.i);
			break;
		case PDF_FLOAT:
			v4Parameter = float4(other.parameter.f);
			break;
		case PDF_VECTOR:
			v4Parameter = float4(other.v3Parameter);
			break;
		case PDF_STRING:
			if(!sscanf(other.parameter.str, "%f %f %f %f", &v4Parameter.x, &v4Parameter.y, &v4Parameter.z, &v4Parameter.w))
			{
				v4Parameter = float4(0.0f);
			}
			break;
		}
		break;

	case PDF_STRING:
		char tmp[64];

		switch(other.type)
		{
		case PDF_BOOL:
			sprintf(tmp, "%d", other.parameter.b ? 1 : 0);
			break;
		case PDF_INT:
			sprintf(tmp, "%d", other.parameter.i);
			break;
		case PDF_FLOAT:
			sprintf(tmp, "%f", other.parameter.f);
			break;
		case PDF_VECTOR:
			sprintf(tmp, "%f %f %f", other.v3Parameter.x, other.v3Parameter.y, other.v3Parameter.z);
			break;
		case PDF_VECTOR4:
			sprintf(tmp, "%f %f %f %f", other.v4Parameter.x, other.v4Parameter.y, other.v4Parameter.z, other.v4Parameter.w);
			break;
		}

		_setStrVal(&parameter.str, tmp);
		break;
	}
}
