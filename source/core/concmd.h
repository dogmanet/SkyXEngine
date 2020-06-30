
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __CONCMD_H
#define __CONCMD_H

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
	ConCmdStub *cls;
	union
	{
		void *cmd;
		SXCONCMDCLS clscmd;
		SXCONCMDCLSARG clscmdarg;
	} cmd;
	char *szDesc;
	bool isAlias = false;
};

bool ConsoleConnect(const char *szName = 0, bool bNewInstance = false);
void ConsoleDisconnect();
void ConsoleRegisterCmds();

void CvarInitSystem(IXCore *pCore);

#endif
