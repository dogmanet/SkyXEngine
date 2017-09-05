
#ifndef SXCONCMD_H
#define SXCONCMD_H

#include "sxcore.h"
#include <common/AssotiativeArray.h>


enum ConCmdType
{
	CMD_DEFAULT = 0,
	CMD_ARG,
	CMD_CLS,
	CMD_CLS_ARG
};

struct ConCmd
{
	ConCmdType type;
	ConCmdStub * cls;
	union
	{
		void * cmd;
		SXCONCMDCLS clscmd;
		SXCONCMDCLSARG clscmdarg;
	} cmd;
	char * szDesc;
};

bool ConsoleConnect(bool bNewInstance = false);
void ConsoleDisconnect();
void ConsoleRegisterCmds();

#endif
