
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "gcore_utils.h"

static int g_iWidth;
static int g_iHeight;

void InitDevice(SXWINDOW hWnd, int iWidth, int iHeight, bool isWindowed)
{
	char szModuleName[64];
	sprintf_s(szModuleName, "gxgapi%s.dll", Core_0GetCommandLineArg("gapi", "dx9"));
	m_hLibGXAPI = LoadLibrary(szModuleName);
	if(!m_hLibGXAPI)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unable to load GX: %s", GEN_MSG_LOCATION, szModuleName);
		return;
	}

	IGXContext * (*libGXGetInstance)();
	libGXGetInstance = (IGXContext*(*)())GetProcAddress(m_hLibGXAPI, "GetInstance");
	if(!libGXGetInstance)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - %s: Not a GX module!", GEN_MSG_LOCATION, szModuleName);
		return;
	}

	g_pDevice = libGXGetInstance();
	if(!g_pDevice)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - %s: Cannot spawn GX context!", GEN_MSG_LOCATION, szModuleName);
		return;
	}

	if(!g_pDevice->initContext(hWnd, iWidth, iHeight, isWindowed))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - %s: Cannot init GX context!", GEN_MSG_LOCATION, szModuleName);
		return;
	}

	g_iWidth = iWidth;
	g_iHeight = iHeight;
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
	GXVERTEXELEMENT oLayoutQuad[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD},
		GXDECL_END()
	};

	IGXVertexDeclaration *pVD = g_pDevice->createVertexDeclaration(oLayoutQuad);

	struct VERTEX_SCREEN_TEXTURE { float x, y, z, tx, ty, tz; };

	// const float fOffsetPixelX = 1.0f / float(g_oD3DAPP.BackBufferWidth);
	// const float fOffsetPixelY = 1.0f / float(g_oD3DAPP.BackBufferHeight);

	VERTEX_SCREEN_TEXTURE aVertices[] =
	{
		{-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0},
		{-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1},
		{1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2},

		{-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0},
		{1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2},
		{1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 3},
	};

	for(UINT i = 0; i < 6; ++i)
	{
		aVertices[i].x -= 1.0f / (float)g_iWidth;
		aVertices[i].y += 1.0f / (float)g_iHeight;
	}


	IGXVertexBuffer *pVB = g_pDevice->createVertexBuffer(sizeof(VERTEX_SCREEN_TEXTURE)* 6, GX_BUFFER_USAGE_STATIC | GX_BUFFER_WRITEONLY, aVertices);
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
	gcore_data::rt_id::idColorScene = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8, "ds_color");
	//номрали + микрорельеф
	gcore_data::rt_id::idNormalScene = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8/*D3DFMT_A2R10G10B10*/, "ds_normal");
	//параметры освещени¤
	gcore_data::rt_id::idParamsScene = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8, "ds_param");

	gcore_data::rt_id::idDepthScene = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_R32F, "ds_depth");
	gcore_data::rt_id::idDepthScene0 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_R32F, "ds_depth_0");
	gcore_data::rt_id::idDepthScene1 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_R32F, "ds_depth_1");

	gcore_data::rt_id::idLightAmbientDiff = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A16B16G16R16F, "ds_ambient");
	gcore_data::rt_id::idLightSpecular = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_R16F, "ds_specdiff");


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
		gcore_data::rt_id::aToneMaps[tmpcount] = SGCore_RTAdd(tmpsize, tmpsize, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_R16F, szNameRT);
		gcore_data::rt_id::aSurfToneMap[tmpcount] = 0;
		++tmpcount;
	}
	gcore_data::rt_id::iCountArrToneMaps = tmpcount;

	gcore_data::rt_id::idAdaptLumCurr = SGCore_RTAdd(1, 1, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_R16F, "adapted_lum_curr");
	gcore_data::rt_id::idAdaptLumLast = SGCore_RTAdd(1, 1, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_R16F, "adapted_lum_last");

	gcore_data::rt_id::idLigthCom = SGCore_RTAdd(*r_win_width, *r_win_height, 1,  GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A16B16G16R16F, "ds_lightcom");
	gcore_data::rt_id::idLigthCom2 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A16B16G16R16F, "ds_lightcom2");
	gcore_data::rt_id::idLigthCom3 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8, "ds_lightcom3");

	gcore_data::rt_id::idLigthComScaled = SGCore_RTAdd(*r_win_width / 4, *r_win_height / 4, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A16B16G16R16F, "ds_lightcomscaled");


	gcore_data::ps_id::idCalcAdaptedLum = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_calc_adapted_lum.ps", "pptm_calc_adapted_lum.ps", SHADER_CHECKDOUBLE_PATH);
	gcore_data::ps_id::idSampleLumInit = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_lum_init.ps", "pptm_lum_init.ps", SHADER_CHECKDOUBLE_PATH);
	gcore_data::ps_id::idSampleLumIterative = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_lum_iterative.ps", "pptm_lum_iterative.ps", SHADER_CHECKDOUBLE_PATH);
}

void LoadShaders()
{
	gcore_data::vs_id::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	//MLSet::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);
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
	GXBLEND_DESC blendDesc;
	blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_RED;
	g_pToneMappingBS = g_pDevice->createBlendState(&blendDesc);

	GXSAMPLER_DESC samplerDesc;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	g_pSamplerFilterLinear = g_pDevice->createSamplerState(&samplerDesc);

	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	g_pSamplerFilterPoint = g_pDevice->createSamplerState(&samplerDesc);
}

void ToneMappingCom(DWORD timeDelta, float fFactorAdapted)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	ToneMappingGetArrDownScale4x4(*r_win_width, *r_win_height, gcore_data::rt_id::aHDRSampleOffsets);

	IGXSurface *SurfSceneScale, *BackBuf;

	SurfSceneScale = SGCore_RTGetTexture(gcore_data::rt_id::idLigthComScaled)->getMipmap();

	BackBuf = g_pDevice->getColorTarget();
	g_pDevice->setColorTarget(SurfSceneScale);

	static ID s_idShader1 = SGCore_ShaderCreateKit(gcore_data::vs_id::idScreenOut, gcore_data::ps_id::idSampleLumIterative);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumIterative, "g_aOffsets", &(gcore_data::rt_id::aHDRSampleOffsets));
	SGCore_ShaderBind(s_idShader1);

	g_pDevice->setTexture(SGCore_RTGetTexture(gcore_data::rt_id::idLigthCom));
	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(SurfSceneScale);

	int CurrTexture = gcore_data::rt_id::iCountArrToneMaps - 1;

	for (int i = 0; i < gcore_data::rt_id::iCountArrToneMaps; i++)
	{
		IGXTexture2D* tmptex = SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[i]);
		gcore_data::rt_id::aSurfToneMap[i] = tmptex->getMipmap();
	}

	IGXTexture2D* tmptex = SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[CurrTexture]);

	ToneMappingGetArrDownScale4x4(tmptex->getWidth(), tmptex->getHeight(), gcore_data::rt_id::aHDRSampleOffsets);

	g_pDevice->setColorTarget(gcore_data::rt_id::aSurfToneMap[CurrTexture]);
	g_pDevice->setTexture(SGCore_RTGetTexture(gcore_data::rt_id::idLigthComScaled));

	g_pDevice->setBlendState(g_pToneMappingBS);
	//g_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);
	//g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//g_pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//g_pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDevice->setSamplerState(g_pSamplerFilterLinear, 0);
	g_pDevice->setSamplerState(g_pSamplerFilterLinear, 1);

	static ID s_idShader2 = SGCore_ShaderCreateKit(gcore_data::vs_id::idScreenOut, gcore_data::ps_id::idSampleLumInit);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumInit, "g_aOffsets", &(gcore_data::rt_id::aHDRSampleOffsets));
	SGCore_ShaderBind(s_idShader2);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();
	mem_release(gcore_data::rt_id::aSurfToneMap[CurrTexture]);

	--CurrTexture;


	IGXTexture2D *pTex;

	g_pDevice->setSamplerState(g_pSamplerFilterPoint, 0);
	//g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	SGCore_ShaderBind(s_idShader1);

	while(CurrTexture >= 0)
	{
		IGXTexture2D *pTex = SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[CurrTexture + 1]);
		ToneMappingGetArrDownScale4x4(pTex->getWidth(), pTex->getHeight(), gcore_data::rt_id::aHDRSampleOffsets);

		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumIterative, "g_aOffsets", &(gcore_data::rt_id::aHDRSampleOffsets));

		g_pDevice->setColorTarget(gcore_data::rt_id::aSurfToneMap[CurrTexture]);

		g_pDevice->setTexture(pTex);
		SGCore_ScreenQuadDraw();

		CurrTexture--;
	}

	SGCore_ShaderUnBind();

	// char *a = (char*)malloc(sizeof(char) * 5);

	//IGXTexture2D* tmptex = SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[3]);

	for (int i = 0; i < gcore_data::rt_id::iCountArrToneMaps - 1; i++)
	{
		IGXSurface* tmpsurf = gcore_data::rt_id::aSurfToneMap[i];
		mem_release(gcore_data::rt_id::aSurfToneMap[i]);
	}

	tmptex = SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[3]);

	gcore_data::rt_id::IncrAdaptedLum();
	IGXSurface *SurfAdaptedLum = SGCore_RTGetTexture(gcore_data::rt_id::GetCurrAdaptedLum())->getMipmap();

	g_pDevice->setColorTarget(SurfAdaptedLum);
	g_pDevice->setTexture(SGCore_RTGetTexture(gcore_data::rt_id::GetLastAdaptedLum()));
	g_pDevice->setTexture(SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[0]), 1);

	//g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//g_pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//g_pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_pDevice->setSamplerState(g_pSamplerFilterPoint, 0);
	g_pDevice->setSamplerState(g_pSamplerFilterPoint, 1);

	static ID s_idShader3 = SGCore_ShaderCreateKit(gcore_data::vs_id::idScreenOut, gcore_data::ps_id::idCalcAdaptedLum);

	SGCore_ShaderBind(s_idShader3);

	float ElapsedTime = float(timeDelta) * 0.001f * (fFactorAdapted * 1000.f);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gcore_data::ps_id::idCalcAdaptedLum, "g_fElapsedTime", &(ElapsedTime));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();
	mem_release(SurfAdaptedLum);

	g_pDevice->setColorTarget(BackBuf);
	mem_release(BackBuf);

	g_pDevice->setBlendState(NULL);
	//g_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

}
