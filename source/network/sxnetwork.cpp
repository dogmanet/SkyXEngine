
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxnetwork.h"
#include "Server.h"
#include "Client.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

CServer *g_pServer = NULL;
CClient *g_pClient = NULL;

//##########################################################################


//#define SN_PRECOND(ret) if(!g_pLevel){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxlevel is not init", GEN_MSG_LOCATION);return ret;}

//##########################################################################

SX_LIB_API void SNetwork_Dbg_Set(report_func fnFunc)
{
	g_fnReportf = fnFunc;
}

SX_LIB_API void SNetwork_0Create()
{
	Core_SetOutPtr();
	Core_0RegisterConcmdArg("connect", [](int argc, const char ** argv){
		if(SNetwork_IsConnected())
		{
			SNetwork_Disconnect();
		}

		if(argc < 2)
		{
			printf("Usage: connect <ip:port>\n");
			return;
		}

		int len = strlen(argv[1]);
		char *szAddr = (char*)alloca(len + 1);
		memcpy(szAddr, argv[1], len + 1);
		char *szAddrParts[2];
		int iPartsCount = parse_str(szAddr, szAddrParts, 2, ':');
		if(iPartsCount != 2)
		{
			printf("Invalid address\n");
			return;
		}
		if(!strcmp(szAddrParts[0], "localhost"))
		{
			strcpy(szAddrParts[0], "127.0.0.1");
		}

		uint16_t usPort;
		if(!sscanf(szAddrParts[1], "%hu", &usPort))
		{
			printf("Invalid port\n");
			return;
		}

		SNetwork_Connect(szAddrParts[0], usPort);
	}, "Connect to game server");

	Core_0RegisterConcmd("disconnect", []()
	{
		if(SNetwork_IsConnected())
		{
			SNetwork_Disconnect();
		}
		else
		{
			printf("Not connected\n");
			return;
		}
	}, "Connect to game server");
}

SX_LIB_API void SNetwork_AKill()
{
	//SN_PRECOND(_VOID);

	//mem_delete(g_pNet);
}

SX_LIB_API void SNetwork_Update()
{
	if(g_pServer)
	{
		g_pServer->update();
	}
	if(g_pClient)
	{
		g_pClient->update();
	}
}

//##########################################################################

SX_LIB_API void SNetwork_InitServer(unsigned short usPort, const char *szIp)
{
	assert(!g_pServer);

	if(!szIp)
	{
		szIp = "0.0.0.0";
	}

	g_pServer = new CServer(szIp, usPort);
}

SX_LIB_API void SNetwork_FinishServer()
{
	assert(g_pServer);

	mem_delete(g_pServer);
}


SX_LIB_API void SNetwork_Connect(const char *szIp, unsigned short usPort)
{
	assert(!g_pClient);

	g_pClient = new CClient();
	g_pClient->connect(szIp, usPort);
}

SX_LIB_API void SNetwork_Disconnect()
{
	assert(g_pClient);

	g_pClient->disconnect();
	mem_delete(g_pClient);
}

SX_LIB_API bool SNetwork_IsConnected()
{
	return(g_pClient != NULL);
}
