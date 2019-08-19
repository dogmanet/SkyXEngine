
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
		LibReport(REPORT_MSG_LEVEL_FATAL, "%s - unable to load GX: %s\n", GEN_MSG_LOCATION, szModuleName);
		return;
	}

	IGXContext* (*libGXGetInstance)();
	libGXGetInstance = (IGXContext*(*)())GetProcAddress(m_hLibGXAPI, "GetInstance");
	if(!libGXGetInstance)
	{
		LibReport(REPORT_MSG_LEVEL_FATAL, "%s - %s: Not a GX module!\n", GEN_MSG_LOCATION, szModuleName);
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

void InitFPStext()
{
	/*
	D3DXFONT_DESC LF;
	ZeroMemory(&LF, sizeof(D3DXFONT_DESC));
	LF.Height = 14;
	LF.Width = 7;
	LF.Weight = 10;
	LF.Italic = 0;
	LF.CharSet = DEFAULT_CHARSET;
	sprintf(LF.FaceName, "Courier New");

	D3DXCreateFontIndirect(g_pDevice, &LF, &g_pFPStext);
	*/
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

void InitRT4Gbuffer()
{
	const int *r_win_width = GET_PCVAR_INT("r_win_width");
	const int *r_win_height = GET_PCVAR_INT("r_win_height");
	
	//цвет (текстуры)
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	gcore_data::rt_id::idColorScene = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8R8G8B8, "ds_color");
	//номрали + микрорельеф
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	gcore_data::rt_id::idNormalScene = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8R8G8B8/*D3DFMT_A2R10G10B10*/, "ds_normal");
	//параметры освещени¤
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	gcore_data::rt_id::idParamsScene = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8R8G8B8, "ds_param");

	//GXFMT_G32R32F; // 64bpp; GXFMT_R32F
	gcore_data::rt_id::idDepthScene = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R32F, "ds_depth");
	gcore_data::rt_id::idDepthScene0 = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R32F, "ds_depth_0");
	gcore_data::rt_id::idDepthScene1 = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R32F, "ds_depth_1");

	gcore_data::rt_id::idLightAmbientDiff = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F, "ds_ambient");
	gcore_data::rt_id::idLightSpecular = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R16F, "ds_specdiff");


	gcore_data::rt_id::aToneMaps.clear();
	gcore_data::rt_id::aSurfToneMap.clear();

	char szNameRT[64];
	int tmpcount = 0;
	while (true)
	{
		int tmpsize = 1 << (2 * tmpcount);
		if (tmpsize >= float(*r_win_width)*0.25 || tmpsize > (*r_win_height)*0.25)
			break;

		sprintf(szNameRT, "tone_map_%dx%d", tmpsize, tmpsize);
		gcore_data::rt_id::aToneMaps[tmpcount] = -1; // SGCore_RTAdd(tmpsize, tmpsize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R16F, szNameRT);
		gcore_data::rt_id::aSurfToneMap[tmpcount] = 0;
		++tmpcount;
	}
	gcore_data::rt_id::iCountArrToneMaps = tmpcount;

	gcore_data::rt_id::idAdaptLumCurr = -1; // SGCore_RTAdd(1, 1, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R16F, "adapted_lum_curr");
	gcore_data::rt_id::idAdaptLumLast = -1; // SGCore_RTAdd(1, 1, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R16F, "adapted_lum_last");

	gcore_data::rt_id::idLigthCom = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1,  GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F, "ds_lightcom");
	gcore_data::rt_id::idLigthCom2 = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F, "ds_lightcom2");
	gcore_data::rt_id::idLigthCom3 = -1; // SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8R8G8B8, "ds_lightcom3");

	gcore_data::rt_id::idLigthComScaled = -1; // SGCore_RTAdd(*r_win_width / 4, *r_win_height / 4, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F, "ds_lightcomscaled");


	gcore_data::ps_id::idCalcAdaptedLum = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_calc_adapted_lum.ps");
	gcore_data::ps_id::idSampleLumInit = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_lum_init.ps");
	gcore_data::ps_id::idSampleLumIterative = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_lum_iterative.ps");
}

void LoadShaders()
{
	gcore_data::vs_id::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs");
	//MLSet::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps");
}

void ToneMappingGetArrDownScale4x4(int iWidth, int iHeight, float2 aDS[])
{
	if (aDS == 0)
		return;

	float tU = 1.0f / float(iWidth);
	float tV = 1.0f / float(iHeight);

	int index = 0;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			aDS[index].x = (x - 1.5f) * tU;
			aDS[index].y = (y - 1.5f) * tV;

			++index;
		}
	}
}

void InitToneMappingStates()
{
	GXBlendDesc blendDesc;
	blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_RED;
	g_pToneMappingBS = g_pDevice->createBlendState(&blendDesc);

	GXSamplerDesc samplerDesc;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	g_pSamplerFilterLinear = g_pDevice->createSamplerState(&samplerDesc);

	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	g_pSamplerFilterPoint = g_pDevice->createSamplerState(&samplerDesc);
}

