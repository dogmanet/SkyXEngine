
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "gcore_utils.h"

void InitDevice(HWND hWnd, int iWidth, int iHeight, bool isWindowed, DWORD dwFlags)
{
	m_hLibGXAPI;
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

	g_pDXDevice = libGXGetInstance();
	if(!g_pDXDevice)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - %s: Cannot spawn GX context!", GEN_MSG_LOCATION, szModuleName);
		return;
	}

	if(!g_pDXDevice->initContext(hWnd, iWidth, iHeight, isWindowed))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - %s: Cannot init GX context!", GEN_MSG_LOCATION, szModuleName);
		return;
	}
}

void InitFPStext()
{
	D3DXFONT_DESC LF;
	ZeroMemory(&LF, sizeof(D3DXFONT_DESC));
	LF.Height = 14;
	LF.Width = 7;
	LF.Weight = 10;
	LF.Italic = 0;
	LF.CharSet = DEFAULT_CHARSET;
	sprintf(LF.FaceName, "Courier New");

	D3DXCreateFontIndirect(g_pDXDevice, &LF, &g_pFPStext);
}

void InitFullScreenQuad()
{
	GXVERTEXELEMENT oLayoutQuad[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD},
		GXDECL_END()
	};

	D3DXCreateMesh(2, 4, D3DXMESH_MANAGED, oLayoutQuad, g_pDXDevice, &g_pScreenTexture);

	struct VERTEX_SCREEN_TEXTURE { float x, y, z, tx, ty, tz; };

	const float fOffsetPixelX = 1.0f / float(g_oD3DAPP.BackBufferWidth);
	const float fOffsetPixelY = 1.0f / float(g_oD3DAPP.BackBufferHeight);

	VERTEX_SCREEN_TEXTURE aVertices[] =
	{
		{ -1.0f - fOffsetPixelX, -1.0f + fOffsetPixelY, 1.0f, 0.0f, 1.0f, 0 },
		{ -1.0f - fOffsetPixelX, 1.0f + fOffsetPixelY, 1.0f, 0.0f, 0.0f, 1 },
		{ 1.0f - fOffsetPixelX, 1.0f + fOffsetPixelY, 1.0f, 1.0f, 0.0f, 2 },
		{ 1.0f - fOffsetPixelX, -1.0f + fOffsetPixelY, 1.0f, 1.0f, 1.0f, 3 },
	};

	void * pVertices;
	if (!FAILED(g_pScreenTexture->LockVertexBuffer(0, (void**)&pVertices)))
	{
		memcpy(pVertices, aVertices, sizeof(aVertices));
		g_pScreenTexture->UnlockVertexBuffer();
	}

	WORD *pIndeces = 0;
	g_pScreenTexture->LockIndexBuffer(0, (void**)&pIndeces);
	pIndeces[0] = 0; pIndeces[1] = 1; pIndeces[2] = 2;
	pIndeces[3] = 0; pIndeces[4] = 2; pIndeces[5] = 3;
	g_pScreenTexture->UnlockIndexBuffer();
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

	float tmpcoefsizert = 1;
	float2_t tmp_sizert = float2_t(float(*r_win_width) * tmpcoefsizert, (*r_win_height) * tmpcoefsizert);

	//цвет (текстуры)
	gcore_data::rt_id::idColorScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_color", tmpcoefsizert);
	//номрали + микрорельеф
	gcore_data::rt_id::idNormalScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8/*D3DFMT_A2R10G10B10*/, D3DPOOL_DEFAULT, "ds_normal", tmpcoefsizert);
	//параметры освещени¤
	gcore_data::rt_id::idParamsScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_param", tmpcoefsizert);

	gcore_data::rt_id::idDepthScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "ds_depth", tmpcoefsizert);
	gcore_data::rt_id::idDepthScene0 = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "ds_depth_0", tmpcoefsizert);
	gcore_data::rt_id::idDepthScene1 = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "ds_depth_1", tmpcoefsizert);

	gcore_data::rt_id::idLightAmbientDiff = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_ambient", tmpcoefsizert);
	gcore_data::rt_id::idLightSpecular = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_specdiff", tmpcoefsizert);


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
		gcore_data::rt_id::aToneMaps[tmpcount] = SGCore_RTAdd(tmpsize, tmpsize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, szNameRT, 0);
		gcore_data::rt_id::aSurfToneMap[tmpcount] = 0;
		++tmpcount;
	}
	gcore_data::rt_id::iCountArrToneMaps = tmpcount;

	gcore_data::rt_id::idAdaptLumCurr = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "adapted_lum_curr", 0);
	gcore_data::rt_id::idAdaptLumLast = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "adapted_lum_last", 0);

	gcore_data::rt_id::idLigthCom = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcom", 1);
	gcore_data::rt_id::idLigthCom2 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcom2", 1);
	gcore_data::rt_id::idLigthCom3 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_lightcom3", 1);

	gcore_data::rt_id::idLigthComScaled = SGCore_RTAdd(float(*r_win_width)*0.25f, float(*r_win_height)*0.25f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcomscaled", 0.25);


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

void ToneMappingCom(DWORD timeDelta, float fFactorAdapted)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	ToneMappingGetArrDownScale4x4(*r_win_width, *r_win_height, gcore_data::rt_id::aHDRSampleOffsets);

	LPDIRECT3DSURFACE9 SurfSceneScale, BackBuf;

	SGCore_RTGetTexture(gcore_data::rt_id::idLigthComScaled)->GetSurfaceLevel(0, &SurfSceneScale);

	g_pDXDevice->GetRenderTarget(0, &BackBuf);
	g_pDXDevice->SetRenderTarget(0, SurfSceneScale);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, gcore_data::vs_id::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumIterative);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumIterative, "g_aOffsets", &(gcore_data::rt_id::aHDRSampleOffsets));

	g_pDXDevice->SetTexture(0, SGCore_RTGetTexture(gcore_data::rt_id::idLigthCom));
	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(SurfSceneScale);

	g_pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);
	int CurrTexture = gcore_data::rt_id::iCountArrToneMaps - 1;

	for (int i = 0; i < gcore_data::rt_id::iCountArrToneMaps; i++)
	{
		IDirect3DTexture9* tmptex = SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[i]);
		IDirect3DSurface9* tmpsurf = gcore_data::rt_id::aSurfToneMap[i];
		SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[i])->GetSurfaceLevel(0, &gcore_data::rt_id::aSurfToneMap[i]);
		int qwert = 0;
	}

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[CurrTexture])->GetLevelDesc(0, &desc);

	ToneMappingGetArrDownScale4x4(desc.Width, desc.Height, gcore_data::rt_id::aHDRSampleOffsets);

	g_pDXDevice->SetRenderTarget(0, gcore_data::rt_id::aSurfToneMap[CurrTexture]);
	g_pDXDevice->SetTexture(0, SGCore_RTGetTexture(gcore_data::rt_id::idLigthComScaled));
	g_pDXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDXDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDXDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, gcore_data::vs_id::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumInit);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumInit, "g_aOffsets", &(gcore_data::rt_id::aHDRSampleOffsets));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();
	mem_release(gcore_data::rt_id::aSurfToneMap[CurrTexture]);

	--CurrTexture;



	while (CurrTexture >= 0)
	{
		SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[CurrTexture + 1])->GetLevelDesc(0, &desc);
		ToneMappingGetArrDownScale4x4(desc.Width, desc.Height, gcore_data::rt_id::aHDRSampleOffsets);

		g_pDXDevice->SetRenderTarget(0, gcore_data::rt_id::aSurfToneMap[CurrTexture]);
		g_pDXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		g_pDXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, gcore_data::vs_id::idScreenOut);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumIterative);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gcore_data::ps_id::idSampleLumIterative, "g_aOffsets", &(gcore_data::rt_id::aHDRSampleOffsets));

		g_pDXDevice->SetTexture(0, SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[CurrTexture + 1]));
		SGCore_ScreenQuadDraw();

		SGCore_ShaderUnBind();
		CurrTexture--;
	}

	IDirect3DTexture9* tmptex = SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[3]);

	for (int i = 0; i < gcore_data::rt_id::iCountArrToneMaps - 1; i++)
	{
		IDirect3DSurface9* tmpsurf = gcore_data::rt_id::aSurfToneMap[i];
		mem_release(gcore_data::rt_id::aSurfToneMap[i]);
	}

	tmptex = SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[3]);

	gcore_data::rt_id::IncrAdaptedLum();
	LPDIRECT3DSURFACE9 SurfAdaptedLum = NULL;
	SGCore_RTGetTexture(gcore_data::rt_id::GetCurrAdaptedLum())->GetSurfaceLevel(0, &SurfAdaptedLum);

	g_pDXDevice->SetRenderTarget(0, SurfAdaptedLum);
	g_pDXDevice->SetTexture(0, SGCore_RTGetTexture(gcore_data::rt_id::GetLastAdaptedLum()));
	g_pDXDevice->SetTexture(1, SGCore_RTGetTexture(gcore_data::rt_id::aToneMaps[0]));
	g_pDXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_pDXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_pDXDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_pDXDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, gcore_data::vs_id::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, gcore_data::ps_id::idCalcAdaptedLum);

	float ElapsedTime = float(timeDelta) * 0.001f * (fFactorAdapted * 1000.f);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gcore_data::ps_id::idCalcAdaptedLum, "g_fElapsedTime", &(ElapsedTime));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();
	mem_release(SurfAdaptedLum);

	g_pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	g_pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

}