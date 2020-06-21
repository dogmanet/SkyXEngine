
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "cvars.h"
#include <xcommon/XEvents.h>

AssotiativeArray<String, CVar> g_mCVars;
AssotiativeArray<String, CVarPtr> g_mCVarPtrs;
IEventChannel<XEventCvarChanged> *g_pNotifyEventChannel = NULL;

SX_LIB_API XDEPRECATED void Core_0RegisterCVarString(const char * name, const char * value, const char * desc, int flags)
{
	if(g_mCVars.KeyExists(name))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVar cv;
	cv.default_val.c = new char[strlen(value) + 1];
	strcpy((char*)cv.default_val.c, value);

	cv.value.c = new char[strlen(value) + 1];
	strcpy((char*)cv.value.c, value);
	cv.desc = NULL;

	if(desc)
	{
		cv.desc = new char[strlen(desc) + 1];
		strcpy((char*)cv.desc, desc);
	}
	
	cv.type = CVAR_STRING;
	cv.flags = flags;

	g_mCVars[name] = cv;
}

SX_LIB_API XDEPRECATED void Core_0RegisterCVarInt(const char * name, int value, const char * desc, int flags)
{
	if(g_mCVars.KeyExists(name))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVar cv;
	cv.default_val.i = value;

	cv.value.i = value;
	cv.desc = NULL;

	if(desc)
	{
		cv.desc = new char[strlen(desc) + 1];
		strcpy((char*)cv.desc, desc);
	}

	cv.type = CVAR_INT;
	cv.flags = flags;
	g_mCVars[name] = cv;
}

SX_LIB_API XDEPRECATED void Core_0RegisterCVarFloat(const char * name, float value, const char * desc, int flags)
{
	if(g_mCVars.KeyExists(name))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVar cv;
	cv.default_val.f = value;

	cv.value.f = value;
	cv.desc = NULL;
	if(desc)
	{
		cv.desc = new char[strlen(desc) + 1];
		strcpy((char*)cv.desc, desc);
	}

	cv.type = CVAR_FLOAT;
	cv.flags = flags;
	g_mCVars[name] = cv;
}

SX_LIB_API XDEPRECATED void Core_0RegisterCVarBool(const char * name, bool value, const char * desc, int flags)
{
	if(g_mCVars.KeyExists(name))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVar cv;
	cv.default_val.b = value;

	cv.value.b = value;
	cv.desc = NULL;

	if(desc)
	{
		cv.desc = new char[strlen(desc) + 1];
		strcpy((char*)cv.desc, desc);
	}

	cv.type = CVAR_BOOL;
	cv.flags = flags;
	g_mCVars[name] = cv;
}

SX_LIB_API XDEPRECATED void Core_0RegisterCVarPointer(const char * name, UINT_PTR value)
{
	if(g_mCVarPtrs.KeyExists(name))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVarPtr cv;

	cv.value = value;

	g_mCVarPtrs[name] = cv;
}


SX_LIB_API XDEPRECATED const char ** Core_0GetPCVarString(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_STRING)
	{
		return(&pNode->Val->value.c);
	}
	return(NULL);
}

SX_LIB_API XDEPRECATED const int * Core_0GetPCVarInt(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_INT)
	{
		return(&pNode->Val->value.i);
	}
	return(NULL);
}

SX_LIB_API XDEPRECATED const float * Core_0GetPCVarFloat(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_FLOAT)
	{
		return(&pNode->Val->value.f);
	}
	return(NULL);
}

SX_LIB_API XDEPRECATED const bool * Core_0GetPCVarBool(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_BOOL)
	{
		return(&pNode->Val->value.b);
	}
	return(NULL);
}

SX_LIB_API XDEPRECATED UINT_PTR * Core_0GetPCVarPointer(const char * name)
{
	const AssotiativeArray<String, CVarPtr>::Node * pNode;
	if(g_mCVarPtrs.KeyExists(name, &pNode))
	{
		return(&pNode->Val->value);
	}
	return(NULL);
}


SX_LIB_API XDEPRECATED void Core_0SetCVarString(const char * name, const char * value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_STRING && !(pNode->Val->flags & FCVAR_READONLY))
	{
		CVar * cv = pNode->Val;
		size_t len = strlen(value);
		if(len > strlen(cv->value.c))
		{
			mem_delete_a(cv->value.c);
			cv->value.c = new char[len + 1];
		}
		strcpy((char*)cv->value.c, value);
		if(pNode->Val->flags & FCVAR_NOTIFY_OLD)
		{
			Core_0ConsoleExecCmd("on_%s_change", name);
		}
		if(pNode->Val->flags & FCVAR_NOTIFY)
		{
			XEventCvarChanged ev;
			ev.type = XEventCvarChanged::TYPE_STRING;
			ev.szName = name;
			ev.pCvar = &cv->value.c;
			g_pNotifyEventChannel->broadcastEvent(&ev);
		}
	}
}

SX_LIB_API XDEPRECATED void Core_0SetCVarInt(const char * name, int value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_INT && !(pNode->Val->flags & FCVAR_READONLY))
	{
		CVar * cv = pNode->Val;
		cv->value.i = value;
		if(pNode->Val->flags & FCVAR_NOTIFY_OLD)
		{
			Core_0ConsoleExecCmd("on_%s_change", name);
		}
		if(pNode->Val->flags & FCVAR_NOTIFY)
		{
			XEventCvarChanged ev;
			ev.type = XEventCvarChanged::TYPE_INT;
			ev.szName = name;
			ev.pCvar = &cv->value.i;
			g_pNotifyEventChannel->broadcastEvent(&ev);
		}
	}
}

SX_LIB_API XDEPRECATED void Core_0SetCVarFloat(const char * name, float value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_FLOAT && !(pNode->Val->flags & FCVAR_READONLY))
	{
		CVar * cv = pNode->Val;
		cv->value.f = value;
		if(pNode->Val->flags & FCVAR_NOTIFY_OLD)
		{
			Core_0ConsoleExecCmd("on_%s_change", name);
		}
		if(pNode->Val->flags & FCVAR_NOTIFY)
		{
			XEventCvarChanged ev;
			ev.type = XEventCvarChanged::TYPE_FLOAT;
			ev.szName = name;
			ev.pCvar = &cv->value.f;
			g_pNotifyEventChannel->broadcastEvent(&ev);
		}
	}
}

SX_LIB_API XDEPRECATED void Core_0SetCVarBool(const char * name, bool value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_BOOL && !(pNode->Val->flags & FCVAR_READONLY))
	{
		CVar * cv = pNode->Val;
		cv->value.b = value;
		if(pNode->Val->flags & FCVAR_NOTIFY_OLD)
		{
			Core_0ConsoleExecCmd("on_%s_change", name);
		}
		if(pNode->Val->flags & FCVAR_NOTIFY)
		{
			XEventCvarChanged ev;
			ev.type = XEventCvarChanged::TYPE_BOOL;
			ev.szName = name;
			ev.pCvar = &cv->value.b;
			g_pNotifyEventChannel->broadcastEvent(&ev);
		}
	}
}

SX_LIB_API XDEPRECATED void Core_0GetCVarAsString(const char * name, char * szOut, int iMaxLength)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(!g_mCVars.KeyExists(name, &pNode))
	{
		sprintf_s(szOut, iMaxLength, "");
		return;
	}

	switch(pNode->Val->type)
	{
	case CVAR_STRING:
		sprintf_s(szOut, iMaxLength, "%s", pNode->Val->value.c);
		break;
	case CVAR_INT:
		sprintf_s(szOut, iMaxLength, "%d", pNode->Val->value.i);
		break;
	case CVAR_FLOAT:
		sprintf_s(szOut, iMaxLength, "%f", pNode->Val->value.f);
		break;
	case CVAR_BOOL:
		sprintf_s(szOut, iMaxLength, "%s", pNode->Val->value.b ? "1" : "0");
		break;
	}
}

void PrintCVar(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(!g_mCVars.KeyExists(name, &pNode))
	{
		return;
	}

	printf(COLOR_GREEN "---------------------------------\n");
	const char * type = "";
	printf(COLOR_LGREEN);
	switch(pNode->Val->type)
	{
	case CVAR_STRING:
		printf("%s = \"%s\"\n", name, pNode->Val->value.c);
		type = "string";
		break;
	case CVAR_INT:
		printf("%s = \"%d\"\n", name, pNode->Val->value.i);
		type = "int";
		break;
	case CVAR_FLOAT:
		printf("%s = \"%f\"\n", name, pNode->Val->value.f);
		type = "float";
		break;
	case CVAR_BOOL:
		printf("%s = \"%s\"\n", name, pNode->Val->value.b ? "true" : "false");
		type = "bool";
		break;
	}
	printf(COLOR_GREEN);
	printf("-------\n");
	if(pNode->Val->desc)
	{
		printf("Description: %s\n", pNode->Val->desc);
	}
	printf("Type: %s\n", type);
	switch(pNode->Val->type)
	{
	case CVAR_STRING:
		printf("Default: \"%s\"\n", pNode->Val->default_val.c);
		break;
	case CVAR_INT:
		printf("Default: \"%d\"\n", pNode->Val->default_val.i);
		break;
	case CVAR_FLOAT:
		printf("Default: \"%f\"\n", pNode->Val->default_val.f);
		break;
	case CVAR_BOOL:
		printf("Default: \"%s\"\n", pNode->Val->default_val.b ? "true" : "false");
		break;
	}
	printf("---------------------------------\n" COLOR_RESET);
}

void SetCVar(const char * name, const char * value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(!g_mCVars.KeyExists(name, &pNode))
	{
		return;
	}

	switch(pNode->Val->type)
	{
	case CVAR_STRING:
		Core_0SetCVarString(name, value);
		break;
	case CVAR_INT:
		{
			int val;
			if(sscanf(value, "%d", &val))
			{
				Core_0SetCVarInt(name, val);
			}
		}
		break;
	case CVAR_FLOAT:
		{
			float val;
			if(sscanf(value, "%f", &val))
			{
				Core_0SetCVarFloat(name, val);
			}
		}
		break;
	case CVAR_BOOL:
		Core_0SetCVarBool(name, strcmp(value, "0") && strcasecmp(value, "false"));
		break;
	}
}

void DumpCVars()
{
	int iLenName = 0,
		iLenType = 6,
		iLenVal = 10;

	for(AssotiativeArray<String, CVar>::Iterator i = g_mCVars.begin(); i; ++i)
	{
		CVar * pCvar = i.second;
		int len = (int)i.first->length();
		if(iLenName < len)
		{
			iLenName = len;
		}
		if(pCvar->type == CVAR_STRING)
		{
			len = (int)strlen(pCvar->value.c);
			if(iLenVal < len)
			{
				iLenVal = len;
			}
		}
	}
	char szRow[128];
	sprintf(szRow, COLOR_LGREEN " %%%ds " COLOR_GRAY "| " COLOR_MAGENTA "%%%ds " COLOR_GRAY "|" COLOR_YELLOW " %%%ds " COLOR_GRAY "|" COLOR_GREEN " %%s" COLOR_RESET "\n", iLenName, iLenType, iLenVal);
	printf(szRow, "Name", "Type", "Value", "Description");
	char tmp[64];
	char tmpName[64];
	for(AssotiativeArray<String, CVar>::Iterator i = g_mCVars.begin(); i; ++i)
	{
		CVar * pCvar = i.second;
		sprintf(tmpName, "%s", i.first->c_str());
		int j;
		for(j = i.first->length(); j < iLenName; ++j)
		{
			tmpName[j] = ' ';
		}
		tmpName[j] = 0;
		switch(pCvar->type)
		{
		case CVAR_STRING:
			printf(szRow, tmpName, "string", pCvar->value.c, pCvar->desc ? pCvar->desc : "");
			break;
		case CVAR_INT:
			sprintf(tmp, "%d", pCvar->value.i);
			printf(szRow, tmpName, "int", tmp, pCvar->desc ? pCvar->desc : "");
			break;
		case CVAR_FLOAT:
			sprintf(tmp, "%f", pCvar->value.f);
			printf(szRow, tmpName, "float", tmp, pCvar->desc ? pCvar->desc : "");
			break;
		case CVAR_BOOL:
			printf(szRow, tmpName, "bool", pCvar->value.b ? "true" : "false", pCvar->desc ? pCvar->desc : "");
			break;
		}
	}
}

void CvarInitSystem(IXCore *pCore)
{
	g_pNotifyEventChannel = pCore->getEventChannel<XEventCvarChanged>(EVENT_CVAR_CHANGED_GUID);
}
