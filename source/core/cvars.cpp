#include "cvars.h"

AssotiativeArray<String, CVar> g_mCVars;

extern report_func reportf;

SX_LIB_API void Core_0RegisterCVarString(const char * name, const char * value, const char * desc, int flags)
{
	if(g_mCVars.KeyExists(name))
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVar cv;
	cv.default_val.c = new char[strlen(value) + 1];
	strcpy((char*)cv.default_val.c, value);

	cv.value.c = new char[strlen(value) + 1];
	strcpy((char*)cv.value.c, value);

	if(desc)
	{
		cv.desc = new char[strlen(desc) + 1];
		strcpy((char*)cv.desc, desc);
	}
	
	cv.type = CVAR_STRING;
	cv.flags = flags;

	g_mCVars[name] = cv;
}

SX_LIB_API void Core_0RegisterCVarInt(const char * name, int value, const char * desc, int flags)
{
	if(g_mCVars.KeyExists(name))
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVar cv;
	cv.default_val.i = value;

	cv.value.i = value;

	if(desc)
	{
		cv.desc = new char[strlen(desc) + 1];
		strcpy((char*)cv.desc, desc);
	}

	cv.type = CVAR_INT;
	cv.flags = flags;
	g_mCVars[name] = cv;
}

SX_LIB_API void Core_0RegisterCVarFloat(const char * name, float value, const char * desc, int flags)
{
	if(g_mCVars.KeyExists(name))
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVar cv;
	cv.default_val.f = value;

	cv.value.f = value;

	if(desc)
	{
		cv.desc = new char[strlen(desc) + 1];
		strcpy((char*)cv.desc, desc);
	}

	cv.type = CVAR_FLOAT;
	cv.flags = flags;
	g_mCVars[name] = cv;
}

SX_LIB_API void Core_0RegisterCVarBool(const char * name, bool value, const char * desc, int flags)
{
	if(g_mCVars.KeyExists(name))
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "CVar '%s' already registered. Skipping.\n", name);
		return;
	}

	//!@TODO: make some kind of pool for default values
	CVar cv;
	cv.default_val.b = value;

	cv.value.b = value;

	if(desc)
	{
		cv.desc = new char[strlen(desc) + 1];
		strcpy((char*)cv.desc, desc);
	}

	cv.type = CVAR_BOOL;
	cv.flags = flags;
	g_mCVars[name] = cv;
}

SX_LIB_API const char ** Core_0GetPCVarString(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_STRING)
	{
		return(&pNode->Val->value.c);
	}
	return(NULL);
}

SX_LIB_API const int * Core_0GetPCVarInt(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_INT)
	{
		return(&pNode->Val->value.i);
	}
	return(NULL);
}

SX_LIB_API const float * Core_0GetPCVarFloat(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_FLOAT)
	{
		return(&pNode->Val->value.f);
	}
	return(NULL);
}

SX_LIB_API const bool * Core_0GetPCVarBool(const char * name)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_BOOL)
	{
		return(&pNode->Val->value.b);
	}
	return(NULL);
}


SX_LIB_API void Core_0SetCVarString(const char * name, const char * value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_STRING)
	{
		CVar * cv = pNode->Val;
		int len = strlen(value);
		if(len > strlen(cv->value.c))
		{
			mem_delete_a(cv->value.c);
			cv->value.c = new char[len + 1];
		}
		strcpy((char*)cv->value.c, value);
		Core_0ConsoleExecCmd("on_%s_change", name);
	}
}

SX_LIB_API void Core_0SetCVarInt(const char * name, int value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_INT)
	{
		CVar * cv = pNode->Val;
		cv->value.i = value;
		Core_0ConsoleExecCmd("on_%s_change", name);
	}
}

SX_LIB_API void Core_0SetCVarFloat(const char * name, float value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_FLOAT)
	{
		CVar * cv = pNode->Val;
		cv->value.f = value;
		Core_0ConsoleExecCmd("on_%s_change", name);
	}
}

SX_LIB_API void Core_0SetCVarBool(const char * name, bool value)
{
	const AssotiativeArray<String, CVar>::Node * pNode;
	if(g_mCVars.KeyExists(name, &pNode) && pNode->Val->type == CVAR_BOOL)
	{
		CVar * cv = pNode->Val;
		cv->value.b = value;
		Core_0ConsoleExecCmd("on_%s_change", name);
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
		Core_0SetCVarBool(name, strcmp(value, "0") && strcmpi(value, "false"));
		break;
	}
}