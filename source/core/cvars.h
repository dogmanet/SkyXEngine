
#ifndef SXCVARS_H
#define SXCVARS_H

#include "sxcore.h"
#include <common/AssotiativeArray.h>
#include <common/String.h>

//! Типы кваров
enum CVAR_TYPE
{
	CVAR_STRING, //!< Строковый
	CVAR_INT,    //!< Целый
	CVAR_FLOAT,  //!< Дробный
	CVAR_BOOL    //!< Логический
};

union CVarValue
{
	int i;
	float f;
	const char * c;
	bool b;
};

struct CVar
{
	CVAR_TYPE type;
	CVarValue value;
	CVarValue default_val;
	const char * desc;
	int flags;
};
struct CVarPtr
{
	UINT_PTR value;
};

void PrintCVar(const char * name);
void SetCVar(const char * name, const char * value);

#endif
