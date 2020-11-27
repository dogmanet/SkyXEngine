
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "gcore_utils.h"


class CGXLogger: public IGXLogger
{
public:
	void logInfo(const char *szString) override
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "%s", szString);
	}
	void logWarning(const char *szString) override
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s", szString);
	}
	void logError(const char *szString) override
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s", szString);
	}
};

CGXLogger g_gxLogger;

//##########################################################################

void InitDevice(SXWINDOW hWnd, int iWidth, int iHeight, bool isWindowed)
{
	char szModuleName[64];
	sprintf_s(szModuleName, "gxgapi%s.dll", Core_0GetCommandLineArg("gapi", "dx11"));
	m_hLibGXAPI = LoadLibrary(szModuleName);
	if(!m_hLibGXAPI)
	{
		const char *szMsg = "";
		if(GetLastError() == ERROR_MOD_NOT_FOUND && !fstrcmp(szModuleName, "gxgapidx11.dll"))
		{
			szMsg = "Please check you have d3dx11_43.dll and D3DCompiler_47.dll in your system!\n";
		}
		LibReport(REPORT_MSG_LEVEL_FATAL, "%s - unable to load GX: %s; Error: %lu\n%s", GEN_MSG_LOCATION, szModuleName, GetLastError(), szMsg);
		return;
	}

	IGXDevice* (*libGXGetInstance)();
	libGXGetInstance = (IGXDevice*(*)())GetProcAddress(m_hLibGXAPI, "GetInstance");
	if(!libGXGetInstance)
	{
		LibReport(REPORT_MSG_LEVEL_FATAL, "%s - %s: Not a GX module!; Error: %lu\n", GEN_MSG_LOCATION, szModuleName, GetLastError());
		return;
	}

	g_pDevice = libGXGetInstance();
	if(!g_pDevice)
	{
		LibReport(REPORT_MSG_LEVEL_FATAL, "%s - %s: Cannot spawn GX context!\n", GEN_MSG_LOCATION, szModuleName);
		return;
	}
	g_pDevice->setLogger(&g_gxLogger);

	if(!g_pDevice->initContext(hWnd, iWidth, iHeight, isWindowed))
	{
		LibReport(REPORT_MSG_LEVEL_FATAL, "%s - %s: Cannot init GX context!\n", GEN_MSG_LOCATION, szModuleName);
		return;
	}
}

void InitFullScreenQuad()
{
	mem_release(g_pScreenTextureRB);
	GXVertexElement oLayoutQuad[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD},
		GX_DECL_END()
	};

	IGXVertexDeclaration *pVD = g_pDevice->createVertexDeclaration(oLayoutQuad);

	struct VERTEX_SCREEN_TEXTURE
	{
		float x, y, z, tx, ty, tz;
	};

	static int * r_win_width = (int*)GET_PCVAR_INT("r_win_width");
	static int * r_win_height = (int*)GET_PCVAR_INT("r_win_height");

	const float fOffsetPixelX = 1.0f / float(*r_win_width);
	const float fOffsetPixelY = 1.0f / float(*r_win_height);

	VERTEX_SCREEN_TEXTURE aVertices[] =
	{
		{-1.0f - fOffsetPixelX, -1.0f + fOffsetPixelY, 1.0f, 0.0f, 1.0f, 0},
		{-1.0f - fOffsetPixelX, 1.0f + fOffsetPixelY, 1.0f, 0.0f, 0.0f, 1},
		{1.0f - fOffsetPixelX, 1.0f + fOffsetPixelY, 1.0f, 1.0f, 0.0f, 2},

		{-1.0f - fOffsetPixelX, -1.0f + fOffsetPixelY, 1.0f, 0.0f, 1.0f, 0},
		{1.0f - fOffsetPixelX, 1.0f + fOffsetPixelY, 1.0f, 1.0f, 0.0f, 2},
		{1.0f - fOffsetPixelX, -1.0f + fOffsetPixelY, 1.0f, 1.0f, 1.0f, 3},
	};


	IGXVertexBuffer *pVB = g_pDevice->createVertexBuffer(sizeof(VERTEX_SCREEN_TEXTURE)* 6, GXBUFFER_USAGE_STATIC, aVertices);
	g_pScreenTextureRB = g_pDevice->createRenderBuffer(1, &pVB, pVD);
	mem_release(pVD);
	mem_release(pVB);
}

void InitArrModes()
{
	//получение всех возможных разрешений монитора
	DEVMODE oDevMode;
	int iNum = 0;
	while (EnumDisplaySettings(NULL, iNum, &oDevMode))
	{
		iNum++;
		if (oDevMode.dmBitsPerPel >= 32 && oDevMode.dmPelsWidth >= 800 && oDevMode.dmPelsHeight >= 600)
		{
			bool isUnic = true;
			for (int i = 0, il = g_aModes.size(); i < il; ++i)
			{
				if (g_aModes[i].dmPelsWidth == oDevMode.dmPelsWidth && g_aModes[i].dmPelsHeight == oDevMode.dmPelsHeight)
				{
					isUnic = false;
					break;
				}
			}

			if (isUnic)
				g_aModes.push_back(oDevMode);
		}
	}
}

