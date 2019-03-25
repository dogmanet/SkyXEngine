
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "shader.h"

bool g_useCache = true;

CShaderFileCache* CreateShaderFileCacheFormShader(CShader *pShader)
{
	CShaderFileCache *pSFC = new CShaderFileCache();
	strcpy(pSFC->m_szName, pShader->m_szName);
	strcpy(pSFC->m_szPath, pShader->m_szPath);
	pSFC->m_pCode = pShader->m_pCode;
	memcpy(pSFC->m_aMacros, pShader->m_aMacros, sizeof(GXMACRO) * SXGC_SHADER_COUNT_MACRO);

	char szFullPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szDir[SXGC_SHADER_MAX_SIZE_DIR];
	szDir[0] = 0;
	char szName[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(pSFC->m_szName, szDir, szName);
	if (szDir[0] != 0)
		sprintf(szFullPath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szDir, pSFC->m_szPath);
	else
	{
		sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), pSFC->m_szPath);
		LibReport(REPORT_MSG_LEVEL_ERROR, "unresolved shader in root directory [%s]\n", szFullPath);
	}

	pSFC->m_uiDate = FileGetTimeLastModify(szFullPath);

	return pSFC;
}

void SaveShaderFileCache(CShaderFileCache *pShaderFileCache)
{
	char szDirPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	
	char szFullPathCache[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szDir[SXGC_SHADER_MAX_SIZE_DIR];
	szDir[0] = 0;
	char szName[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(pShaderFileCache->m_szName, szDir, szName);
	if (szDir[0] != 0)
	{
		sprintf(szDirPath, "%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, szDir);
		sprintf(szFullPathCache, "%s%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, szDir, pShaderFileCache->m_szName);
	}
	else
	{
		sprintf(szDirPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH);
		sprintf(szFullPathCache, "%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, pShaderFileCache->m_szName);

		char szFullPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
		sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), pShaderFileCache->m_szPath);
		LibReport(REPORT_MSG_LEVEL_ERROR, "unresolved shader in root directory [%s]\n", szFullPath);
	}

	CreateDirectory(szDirPath, 0);
	//SHCreateDirectory

	FILE *pFile = fopen(szFullPathCache, "wb");

	if(!pFile)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to open file '%s' for writing", szFullPathCache);
		return;
	}

	fwrite(&(pShaderFileCache->m_uiDate), sizeof(uint32_t), 1, pFile);

	uint32_t iLen = strlen(pShaderFileCache->m_szName);
	fwrite(&(iLen), sizeof(uint32_t), 1, pFile);
	fwrite((pShaderFileCache->m_szName), sizeof(char)* iLen, 1, pFile);

	uint32_t iCountMacros = 0;
	for (int i = 0; i < SXGC_SHADER_COUNT_MACRO; ++i)
	{
		if ((pShaderFileCache->m_aMacros[i].szName && strlen(pShaderFileCache->m_aMacros[i].szName) > 0))
			++iCountMacros;
	}

	fwrite(&iCountMacros, sizeof(uint32_t), 1, pFile);

	if (iCountMacros > 0)
	{
		for (int i = 0; i < iCountMacros; ++i)
		{
			if (!(pShaderFileCache->m_aMacros[i].szName && strlen(pShaderFileCache->m_aMacros[i].szName) > 0))
				break;

			uint32_t iLen = strlen(pShaderFileCache->m_aMacros[i].szName);
			fwrite(&(iLen), sizeof(uint32_t), 1, pFile);
			fwrite((pShaderFileCache->m_aMacros[i].szName), sizeof(char)* iLen, 1, pFile);

			iLen = strlen(pShaderFileCache->m_aMacros[i].szDefinition);
			fwrite(&(iLen), sizeof(uint32_t), 1, pFile);
			fwrite((pShaderFileCache->m_aMacros[i].szDefinition), sizeof(char)* iLen, 1, pFile);
		}
	}

	uint32_t iSizeBinCode = pShaderFileCache->m_uiCodeSize;
	fwrite(&(pShaderFileCache->m_uiCodeSize), sizeof(uint32_t), 1, pFile);
	fwrite(pShaderFileCache->m_pCode, iSizeBinCode, 1, pFile);

	fclose(pFile);
}

CShaderFileCache* CreateShaderFileCacheFormFile(const char *szPath)
{
	if (!FileExistsFile(szPath))
		return 0;

	CShaderFileCache *pSFC = new CShaderFileCache();

	FILE *pFile = fopen(szPath, "rb");

	uint32_t tLastModify = 0;

	fread(&(pSFC->m_uiDate), sizeof(uint32_t), 1, pFile);

	uint32_t iLen = 0;
	fread(&(iLen), sizeof(uint32_t), 1, pFile);
	fread((pSFC->m_szName), sizeof(char)* iLen, 1, pFile);

	uint32_t iCountMacros = 0;
	fread(&iCountMacros, sizeof(uint32_t), 1, pFile);

	for (int i = 0; i < iCountMacros; ++i)
	{
		uint32_t iLen = 0;
		fread(&(iLen), sizeof(uint32_t), 1, pFile);

		pSFC->m_aMacros[i].szName = new const char[iLen + 1];
		ZeroMemory((void*)(pSFC->m_aMacros[i].szName), iLen + 1);

		fread((void*)(pSFC->m_aMacros[i].szName), sizeof(char)* iLen, 1, pFile);


		iLen = 0;
		fread(&(iLen), sizeof(uint32_t), 1, pFile);

		pSFC->m_aMacros[i].szDefinition = new const char[iLen + 1];
		ZeroMemory((void*)(pSFC->m_aMacros[i].szDefinition), iLen + 1);

		fread((void*)(pSFC->m_aMacros[i].szDefinition), sizeof(char)* iLen, 1, pFile);
	}

	uint32_t iSizeBinCode;
	fread(&iSizeBinCode, sizeof(uint32_t), 1, pFile);

	pSFC->m_pCode = new BYTE[iSizeBinCode];

	fread(pSFC->m_pCode, iSizeBinCode, 1, pFile);

	fclose(pFile);

	return pSFC;
}

uint32_t GetTimeShaderFileCache(const char *szPath)
{
	if (!FileExistsFile(szPath))
		return 0;

	FILE *pFile = fopen(szPath, "rb");

	uint32_t tLastModify = 0;
	fread(&tLastModify, sizeof(uint32_t), 1, pFile);
	fclose(pFile);

	return tLastModify;
}

//**************************************************************************

int LoadVertexShader(const char *szPath, CShaderVS *pShader, GXMACRO *aMacro)
{
	char szFullPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szFullPathCache[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szDir[SXGC_SHADER_MAX_SIZE_DIR];
	szDir[0] = 0;
	char szName[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(szPath, szDir, szName);
	if (szDir[0] != 0)
	{
		sprintf(szFullPath, "%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szDir, szPath);
		sprintf(szFullPathCache, "%s%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, szDir, pShader->m_szName);
	}
	else
	{
		sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szPath);
		sprintf(szFullPathCache, "%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, pShader->m_szName);
	}

#if !defined(_DEBUG) && 0 
	if (
		//если юзаем кэш и файл кэша существует
		(g_useCache && FileExistsFile(szFullPathCache)) && 
		//если время изменения кэша такое же как и время изменения файла шейдера или файла шейдера нет
		(GetTimeShaderFileCache(szFullPathCache) == FileGetTimeLastModify(szFullPath) || !FileExistsFile(szFullPath))
		)
	{
		CShaderFileCache *pSFC = CreateShaderFileCacheFormFile(szFullPathCache);

		pShader->m_iCountVar = pSFC->m_iCountVar;
		//strcpy(pShader->m_szName, pSFC->m_szName);
		memcpy(pShader->m_aVarDesc, pSFC->m_aVarDesc, sizeof(D3DXCONSTANT_DESC) * SXGC_SHADER_VAR_MAX_COUNT);
		memcpy(pShader->m_aMacros, pSFC->m_aMacros, sizeof(GXMACRO) * SXGC_SHADER_COUNT_MACRO);
		pShader->m_pCode = pSFC->m_pCode;
		pShader->m_pCode->AddRef();

		HRESULT hr = g_pDevice->CreateVertexShader(
			(DWORD*)pShader->m_pCode->GetBufferPointer(),
			&(pShader->m_pVertexShader)
			);
		if (FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error creating vertex shader [%s]\n", GEN_MSG_LOCATION, szPath);
			return LOAD_SHADER_FAIL;
		}

		mem_delete(pSFC);
		return LOAD_SHADER_CACHE;
	}
	else
	{
#endif

		IGXVertexShader *pVertexShader = g_pDevice->createVertexShader(szFullPath, aMacro);

		pShader->m_pCode = 0;
		pVertexShader->getData(pShader->m_pCode, &pShader->m_uiCodeSize);
		pShader->m_pCode = new BYTE[pShader->m_uiCodeSize];
		pVertexShader->getData(pShader->m_pCode, &pShader->m_uiCodeSize);

		pShader->m_pVertexShader = pVertexShader;

#if !defined(_DEBUG) && 0 
		CShaderFileCache *pSFC = CreateShaderFileCacheFormShader(pShader);
		SaveShaderFileCache(pSFC);
		mem_delete(pSFC);
	}
#endif

	return LOAD_SHADER_COMPLETE;
}

int LoadPixelShader(const char *szPath, CShaderPS *pShader,GXMACRO *aMacro)
{
	char szFullPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szFullPathCache[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szDir[SXGC_SHADER_MAX_SIZE_DIR];
	szDir[0] = 0;
	char szName[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(szPath, szDir, szName);
	if (szDir[0] != 0)
	{
		sprintf(szFullPath, "%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szDir, szPath);
		sprintf(szFullPathCache, "%s%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, szDir, pShader->m_szName);
	}
	else
	{
		sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szPath);
		sprintf(szFullPathCache, "%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, pShader->m_szName);
	}

#if !defined(_DEBUG) && 0 
	UINT uiTimeFileCahce = GetTimeShaderFileCache(szFullPathCache);
	UINT uiTimeFileSahder = FileGetTimeLastModify(szFullPath);
	if (
		//если юзаем кэш и файл кэша существует
		(g_useCache && FileExistsFile(szFullPathCache)) &&
		//если время изменения кэша такое же как и время изменения файла шейдера или файла шейдера нет
		(uiTimeFileCahce == uiTimeFileSahder || !FileExistsFile(szFullPath))
		)
	{
		CShaderFileCache *pSFC = CreateShaderFileCacheFormFile(szFullPathCache);

		pShader->m_iCountVar = pSFC->m_iCountVar;
		memcpy(pShader->m_aVarDesc, pSFC->m_aVarDesc, sizeof(D3DXCONSTANT_DESC)* SXGC_SHADER_VAR_MAX_COUNT);
		memcpy(pShader->m_aMacros, pSFC->m_aMacros, sizeof(GXMACRO)* SXGC_SHADER_COUNT_MACRO);
		pShader->m_pCode = pSFC->m_pCode;
		pShader->m_pCode->AddRef();

		HRESULT hr = g_pDevice->CreatePixelShader(
			(DWORD*)pShader->m_pCode->GetBufferPointer(),
			&(pShader->m_pPixelShader)
			);
		if (FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error creating pixel shader [%s]", GEN_MSG_LOCATION, szPath);
			return LOAD_SHADER_FAIL;
		}

		mem_delete(pSFC);

		return LOAD_SHADER_CACHE;
	}
	else
	{
#endif
		IGXPixelShader *pPixelShader = NULL;
		do
		{
			pPixelShader = g_pDevice->createPixelShader(szFullPath, aMacro);
		}
		while(!pPixelShader && MessageBoxA(NULL, "Unable to compile pixel shader. Want to retry?", "PS Shader error", MB_OKCANCEL | MB_ICONSTOP) == IDOK);

		pShader->m_pCode = 0;
		pPixelShader->getData(pShader->m_pCode, &pShader->m_uiCodeSize);
		pShader->m_pCode = new BYTE[pShader->m_uiCodeSize];
		pPixelShader->getData(pShader->m_pCode, &pShader->m_uiCodeSize);

		pShader->m_pPixelShader = pPixelShader;

#if !defined(_DEBUG) && 0 
		CShaderFileCache *pSFC = CreateShaderFileCacheFormShader(pShader);
		SaveShaderFileCache(pSFC);
		mem_delete(pSFC);
	}
#endif

	return LOAD_SHADER_COMPLETE;
}

int LoadGeometryShader(const char *szPath, CShaderGS *pShader, GXMACRO *aMacro)
{
	char szFullPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szFullPathCache[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szDir[SXGC_SHADER_MAX_SIZE_DIR];
	szDir[0] = 0;
	char szName[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(szPath, szDir, szName);
	if(szDir[0] != 0)
	{
		sprintf(szFullPath, "%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szDir, szPath);
		sprintf(szFullPathCache, "%s%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, szDir, pShader->m_szName);
	}
	else
	{
		sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szPath);
		sprintf(szFullPathCache, "%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, pShader->m_szName);
	}

#if !defined(_DEBUG) && 0 
	UINT uiTimeFileCahce = GetTimeShaderFileCache(szFullPathCache);
	UINT uiTimeFileSahder = FileGetTimeLastModify(szFullPath);
	if(
		//если юзаем кэш и файл кэша существует
		(g_useCache && FileExistsFile(szFullPathCache)) &&
		//если время изменения кэша такое же как и время изменения файла шейдера или файла шейдера нет
		(uiTimeFileCahce == uiTimeFileSahder || !FileExistsFile(szFullPath))
		)
	{
		CShaderFileCache *pSFC = CreateShaderFileCacheFormFile(szFullPathCache);

		pShader->m_iCountVar = pSFC->m_iCountVar;
		memcpy(pShader->m_aVarDesc, pSFC->m_aVarDesc, sizeof(D3DXCONSTANT_DESC)* SXGC_SHADER_VAR_MAX_COUNT);
		memcpy(pShader->m_aMacros, pSFC->m_aMacros, sizeof(GXMACRO)* SXGC_SHADER_COUNT_MACRO);
		pShader->m_pCode = pSFC->m_pCode;
		pShader->m_pCode->AddRef();

		HRESULT hr = g_pDevice->CreatePixelShader(
			(DWORD*)pShader->m_pCode->GetBufferPointer(),
			&(pShader->m_pPixelShader)
			);
		if(FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error creating pixel shader [%s]", GEN_MSG_LOCATION, szPath);
			return LOAD_SHADER_FAIL;
		}

		mem_delete(pSFC);

		return LOAD_SHADER_CACHE;
	}
	else
	{
#endif
		IGXGeometryShader *pGeometryShader = g_pDevice->createGeometryShader(szFullPath, aMacro);

		pShader->m_pCode = 0;
		pGeometryShader->getData(pShader->m_pCode, &pShader->m_uiCodeSize);
		pShader->m_pCode = new BYTE[pShader->m_uiCodeSize];
		pGeometryShader->getData(pShader->m_pCode, &pShader->m_uiCodeSize);

		pShader->m_pGeometryShader = pGeometryShader;

#if !defined(_DEBUG) && 0 
		CShaderFileCache *pSFC = CreateShaderFileCacheFormShader(pShader);
		SaveShaderFileCache(pSFC);
		mem_delete(pSFC);
	}
#endif

	return LOAD_SHADER_COMPLETE;
}

//**************************************************************************

CShaderManager::CShaderManager()
{
	m_canInfo4FailSend = true;

	updateDataInfo4FailSend();

	g_useCache = true;
	m_iLastAllLoadVS = 0;
	m_iLastAllLoadPS = 0;

	testDirCache();

	testIncludeCache(false);
}

void CShaderManager::testIncludeCache(bool hasReport)
{
	testDirCache();
	loadCacheInclude();

	Array<String> aIncludes = FileGetListRec(Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), FILE_LIST_TYPE_FILES, "h");

	g_useCache = true;

	if (aIncludes.size() != m_aIncludes.size())
	{
		g_useCache = false;
		if (hasReport)
			LibReport(REPORT_MSG_LEVEL_WARNING, "h shaders is modified (%d!=%d), shader cache disabled\n", aIncludes.size(), m_aIncludes.size());
	}
	else
	{
		for (int i = 0, il = m_aIncludes.size(); i < il; ++i)
		{
			if (
				!(stricmp(m_aIncludes[i].m_szFile, aIncludes[i].c_str()) == 0 &&
				FileGetTimeLastModify((String(Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS)) + aIncludes[i]).c_str()) == m_aIncludes[i].m_uiDate)
				)
			{
				g_useCache = false;
				if (hasReport)
					LibReport(REPORT_MSG_LEVEL_WARNING, "h shaders is modified, shader cache disabled\n");
				break;
			}
		}
	}

	if (!g_useCache)
	{
		m_aIncludes.clearFast();
		for (int i = 0, il = aIncludes.size(); i < il; ++i)
		{
			sprintf(m_aIncludes[i].m_szFile, "%s", aIncludes[i].c_str());
			m_aIncludes[i].m_uiDate = FileGetTimeLastModify((String(Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS)) + aIncludes[i]).c_str());
		}

		saveCacheInclude();
	}
}

void CShaderManager::testDirCache()
{
	char szFullPathCache[1024];
	sprintf(szFullPathCache, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH);

	if (!FileExistsDir(szFullPathCache))
		FileCreateDir(szFullPathCache);
}

CShaderManager::~CShaderManager()
{
	for (int i = 0; i < m_aVS.size(); ++i)
	{
		mem_delete(m_aVS[i]);
	}


	for(int i = 0; i < m_aPS.size(); ++i)
	{
		mem_delete(m_aPS[i]);
	}

	for(int i = 0; i < m_aGS.size(); ++i)
	{
		mem_delete(m_aGS[i]);
	}
			
	m_aVS.clear();
	m_aPS.clear();
	m_aGS.clear();
}

String CShaderManager::getTextResultLoad(int iResult)
{
	String sStr = "unknown";
	if (iResult == LOAD_SHADER_FAIL)
		sStr = "fail";
	else if (iResult == LOAD_SHADER_COMPLETE)
		sStr = "complete";
	else if (iResult == LOAD_SHADER_CACHE)
		sStr = "load from cache";
	else
		sStr = "unknown";

	return sStr;
}


void CShaderManager::loadCacheInclude()
{
	char szFullPathCacheInc[1024];
	sprintf(szFullPathCacheInc, "%s%s/includes", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH);

	if (!FileExistsFile(szFullPathCacheInc))
		return;

	m_aIncludes.clear();

	FILE *pFile = fopen(szFullPathCacheInc, "rb");

	int32_t iCount = 0;

	fread(&iCount, sizeof(int32_t), 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		CShaderInclude oInc;
		fread(&oInc, sizeof(CShaderInclude), 1, pFile);
		m_aIncludes.push_back(oInc);
	}

	fclose(pFile);
}

void CShaderManager::saveCacheInclude()
{
	char szFullPathCacheInc[1024];
	sprintf(szFullPathCacheInc, "%s%s/includes", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH);

	FILE *pFile = fopen(szFullPathCacheInc, "wb");

	int32_t iCount = m_aIncludes.size();

	fwrite(&iCount, sizeof(int32_t), 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		fwrite(&(m_aIncludes[i]), sizeof(CShaderInclude), 1, pFile);
	}

	fclose(pFile);
}

void CShaderManager::reloadAll()
{
	LibReport(REPORT_MSG_LEVEL_NOTICE, "reload shaders, cache %s ...\n", (g_useCache ? "enable" : "disable"));

	for (int i = 0, il = m_aShaderKit.size(); i < il; ++i)
	{
		if (m_aShaderKit[i])
		{
			mem_release(m_aShaderKit[i]->m_pShaderKit);
		}
	}

	testIncludeCache();

	String sResult;

	for (int i = 0; i<m_aVS.size(); i++)
	{
		CShaderVS *pShader = m_aVS[i];
		sResult = getTextResultLoad(LoadVertexShader(pShader->m_szPath, pShader, pShader->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  VS id [%d], file [%s], name [%s], result [%s] \n", i, pShader->m_szPath, pShader->m_szName, sResult.c_str());
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for(int i = 0; i<m_aPS.size(); i++)
	{
		CShaderPS *pShader = m_aPS[i];
		sResult = getTextResultLoad(LoadPixelShader(m_aPS[i]->m_szPath, m_aPS[i], m_aPS[i]->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  PS id [%d], file[%s], name[%s], result [%s] \n", i, pShader->m_szPath, pShader->m_szName, sResult.c_str());
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for(int i = 0; i<m_aGS.size(); i++)
	{
		CShaderGS *pShader = m_aGS[i];
		sResult = getTextResultLoad(LoadGeometryShader(m_aGS[i]->m_szPath, m_aGS[i], m_aGS[i]->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  GS id [%d], file[%s], name[%s], result [%s] \n", i, pShader->m_szPath, pShader->m_szName, sResult.c_str());
	}

	for (int i = 0, il = m_aShaderKit.size(); i < il; ++i)
	{
		if (m_aShaderKit[i])
		{
			ID idVS = m_aShaderKit[i]->m_idVertexShader;
			ID idPS = m_aShaderKit[i]->m_idPixelShader;
			ID idGS = m_aShaderKit[i]->m_idGeometryShader;

			m_aShaderKit[i]->m_pShaderKit = g_pDevice->createShader(ID_VALID(idVS) ? m_aVS[idVS]->m_pVertexShader : NULL, ID_VALID(idPS) ? m_aPS[idPS]->m_pPixelShader : NULL, ID_VALID(idGS) ? m_aGS[idGS]->m_pGeometryShader : NULL);
		}
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "all reloaded shaders\n");
}

ID CShaderManager::existsPath(SHADER_TYPE type, const char *szPath)
{
	if (type == SHADER_TYPE_VERTEX)
	{
		for (int i = 0; i<m_aVS.size(); ++i)
		{
			if (strcmp(m_aVS[i]->m_szPath, szPath) == 0)
				return i;
		}
	}
	else if(type == SHADER_TYPE_PIXEL)
	{
		for(int i = 0; i<m_aPS.size(); ++i)
		{
			if(strcmp(m_aPS[i]->m_szPath, szPath) == 0)
				return i;
		}
	}
	else if(type == SHADER_TYPE_GEOMETRY)
	{
		for(int i = 0; i<m_aGS.size(); ++i)
		{
			if(strcmp(m_aGS[i]->m_szPath, szPath) == 0)
				return i;
		}
	}
	return -1;
}

ID CShaderManager::existsName(SHADER_TYPE type_shader, const char *szName)
{
	if (!isValidateTypeName(type_shader, szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szName);
		return -1;
	}

	if (type_shader == SHADER_TYPE_VERTEX)
	{
		for (int i = 0; i<m_aVS.size(); ++i)
		{
			if (strcmp(m_aVS[i]->m_szName, szName) == 0)
				return i;
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		for(int i = 0; i<m_aPS.size(); ++i)
		{
			if(strcmp(m_aPS[i]->m_szName, szName) == 0)
				return i;
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		for(int i = 0; i<m_aGS.size(); ++i)
		{
			if(strcmp(m_aGS[i]->m_szName, szName) == 0)
				return i;
		}
	}
	return -1;
}

bool CShaderManager::isValidateTypeName(SHADER_TYPE type, const char *szName)
{
	char szEndStr[4];
	sprintf(szEndStr, "%s", szName + (strlen(szName) - 3));
	szEndStr[3] = 0;

	return (type == SHADER_TYPE_VERTEX && stricmp(szEndStr, ".vs") == 0) || (type == SHADER_TYPE_PIXEL && stricmp(szEndStr, ".ps") == 0 || (type == SHADER_TYPE_GEOMETRY && stricmp(szEndStr, ".gs") == 0));
}

bool CShaderManager::existsFile(const char *szPath)
{
	char tmppath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char tmpspath[SXGC_SHADER_MAX_SIZE_DIR];
	tmpspath[0] = 0;
	char tmpname[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(szPath, tmpspath, tmpname);

	if (tmpspath[0] != 0)
		sprintf(tmppath, "%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), tmpspath, szPath);
	else
		sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szPath);

	return FileExistsFile(tmppath);
}

ID CShaderManager::preLoad(SHADER_TYPE type, const char *szPath, const char *szName, SHADER_CHECKDOUBLE check_double, GXMACRO *aMacros)
{
	if (!isValidateTypeName(type, szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szName);
		return -1;
	}

	ID id = -1;
	bool isUnic = true;

	if (check_double == SHADER_CHECKDOUBLE_PATH)
		id = existsPath(type, szPath);
	else if (check_double == SHADER_CHECKDOUBLE_NAME)
		id = existsName(type, szName);

	if (id >= 0)
		isUnic = false;

	CShader *pShader = 0;

	if (type == SHADER_TYPE_VERTEX)
	{
		if (id == -1)
		{
			CShaderVS *pVS = new CShaderVS();
			sprintf(pVS->m_szName, "%s", szName);
			sprintf(pVS->m_szPath, "%s", szPath);
			//sprintf(pVS->m_szFrom, "%s", szFrom);
			m_aVS.push_back(pVS);
			id = m_aVS.size() -1;
			pShader = pVS;
		}
	}
	else if(type == SHADER_TYPE_PIXEL)
	{
		if(id == -1)
		{
			CShaderPS *pPS = new CShaderPS();
			sprintf(pPS->m_szName, "%s", szName);
			sprintf(pPS->m_szPath, "%s", szPath);
			//sprintf(pPS->m_szFrom, "%s", szFrom);
			m_aPS.push_back(pPS);
			id = m_aPS.size() - 1;
			pShader = pPS;
		}
	}
	else if(type == SHADER_TYPE_GEOMETRY)
	{
		if(id == -1)
		{
			CShaderGS *pGS = new CShaderGS();
			sprintf(pGS->m_szName, "%s", szName);
			sprintf(pGS->m_szPath, "%s", szPath);
			//sprintf(pPS->m_szFrom, "%s", szFrom);
			m_aGS.push_back(pGS);
			id = m_aGS.size() - 1;
			pShader = pGS;
		}
	}

	if (aMacros && isUnic)
	{
		int iCountMacros = 0;
		for (int i = 0; i < SXGC_SHADER_COUNT_MACRO; i++)
		{
			if (aMacros[i].szName == 0)
			{
				iCountMacros = i;
				break;
			}
		}

		for (int i = 0; i < iCountMacros; i++)
		{
			pShader->m_aMacros[i] = aMacros[i];
		}

		pShader->m_aMacros[iCountMacros].szName = 0;
		pShader->m_aMacros[iCountMacros].szDefinition = 0;
	}

	return id;
}

void CShaderManager::allLoad()
{
	if(m_aVS.size() == m_iLastAllLoadVS && m_aPS.size() == m_iLastAllLoadPS && m_aGS.size() == m_iLastAllLoadGS)
		return;

	DWORD dwTime = GetTickCount();
	LibReport(REPORT_MSG_LEVEL_NOTICE, "load shaders, cache %s ...\n", (g_useCache ? "enable" : "disable"));

	String sResult;
	
	for (int i = 0, il = m_aVS.size(); i < il; ++i)
	{
		CShaderVS *pShader = m_aVS[i];
		if (pShader->m_pVertexShader)
			continue;
		sResult = getTextResultLoad(LoadVertexShader(pShader->m_szPath, pShader, pShader->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  VS id [%d], file [%s], name [%s], result [%s] \n", i, pShader->m_szPath, pShader->m_szName, sResult.c_str());
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for (int i = 0, il = m_aPS.size(); i < il; ++i)
	{
		CShaderPS *pShader = m_aPS[i];
		if (pShader->m_pPixelShader)
			continue;
		sResult = getTextResultLoad(LoadPixelShader(pShader->m_szPath, pShader, pShader->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  PS id [%d], file[%s], name[%s], result [%s] \n", i, pShader->m_szPath, pShader->m_szName, sResult.c_str());
	}
	
	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for(int i = 0, il = m_aGS.size(); i < il; ++i)
	{
		CShaderGS *pShader = m_aGS[i];
		if(pShader->m_pGeometryShader)
			continue;
		sResult = getTextResultLoad(LoadGeometryShader(pShader->m_szPath, pShader, pShader->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  GS id [%d], file[%s], name[%s], result [%s] \n", i, pShader->m_szPath, pShader->m_szName, sResult.c_str());
	}

	m_iLastAllLoadVS = m_aVS.size();
	m_iLastAllLoadPS = m_aPS.size();
	m_iLastAllLoadGS = m_aGS.size();

	for (int i = 0, il = m_aShaderKit.size(); i < il; ++i)
	{
		if (m_aShaderKit[i] && !(m_aShaderKit[i]->m_pShaderKit))
		{
			ID idVS = m_aShaderKit[i]->m_idVertexShader;
			ID idPS = m_aShaderKit[i]->m_idPixelShader;
			ID idGS = m_aShaderKit[i]->m_idGeometryShader;

			m_aShaderKit[i]->m_pShaderKit = g_pDevice->createShader(ID_VALID(idVS) ? m_aVS[idVS]->m_pVertexShader : NULL, ID_VALID(idPS) ? m_aPS[idPS]->m_pPixelShader : NULL, ID_VALID(idGS) ? m_aGS[idGS]->m_pGeometryShader : NULL);
		}
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "all loaded shaders, time %d\n", GetTickCount() - dwTime);
}

void CShaderManager::update(SHADER_TYPE type_shader, const char *szName)
{
	if (!isValidateTypeName(type_shader, szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szName);
		return;
	}

	bool isUpdate = false;
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		for (int i = 0; i<m_aVS.size(); i++)
		{
			if (strcmp(m_aVS[i]->m_szName, szName) == 0)
			{
				CShaderVS *pShader = m_aVS[i];
				mem_release(m_aVS[i]->m_pVertexShader);
				LoadVertexShader(m_aVS[i]->m_szPath, pShader, m_aVS[i]->m_aMacros);
				isUpdate = true;
			}
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		for(int i = 0; i<m_aPS.size(); i++)
		{
			if(strcmp(m_aPS[i]->m_szName, szName) == 0)
			{
				CShaderPS *pShader = m_aPS[i];
				mem_release(m_aPS[i]->m_pPixelShader);
				LoadPixelShader(m_aPS[i]->m_szPath, pShader, m_aPS[i]->m_aMacros);
				isUpdate = true;
			}
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		for(int i = 0; i<m_aGS.size(); i++)
		{
			if(strcmp(m_aGS[i]->m_szName, szName) == 0)
			{
				CShaderGS *pShader = m_aGS[i];
				mem_release(m_aGS[i]->m_pGeometryShader);
				LoadGeometryShader(m_aGS[i]->m_szPath, pShader, m_aGS[i]->m_aMacros);
				isUpdate = true;
			}
		}
	}

	if (!isUpdate)
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - shader [%s] is not detected", GEN_MSG_LOCATION, szName);
	/*else
		LibReport(REPORT_MSG_LEVEL_NOTICE, "shader [%s] is update", name);*/
}

void CShaderManager::update(SHADER_TYPE type_shader, ID id)
{
	bool isUpdate = false;
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		if (m_aVS.size() > id && m_aVS[id])
		{
			CShaderVS *pShader = m_aVS[id];
			mem_release(m_aVS[id]->m_pVertexShader);
			LoadVertexShader(m_aVS[id]->m_szPath, pShader, pShader->m_aMacros);
			isUpdate = true;
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		if(m_aPS.size() > id  && m_aPS[id])
		{
			CShaderPS *pShader = m_aPS[id];
			mem_release(m_aPS[id]->m_pPixelShader);
			LoadPixelShader(m_aPS[id]->m_szPath, pShader, m_aPS[id]->m_aMacros);
			isUpdate = true;
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		if(m_aGS.size() > id  && m_aGS[id])
		{
			CShaderGS *pShader = m_aGS[id];
			mem_release(m_aGS[id]->m_pGeometryShader);
			LoadGeometryShader(m_aGS[id]->m_szPath, pShader, m_aGS[id]->m_aMacros);
			isUpdate = true;
		}
	}

	if (!isUpdate)
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - shader_id [%s] is not detected", GEN_MSG_LOCATION, id);
	/*else
		LibReport(REPORT_MSG_LEVEL_NOTICE, "shader [%s] is update", m_aPS[id]->m_szName);*/
}

ID CShaderManager::getID(SHADER_TYPE type_shader, const char *szName)
{
	if (!isValidateTypeName(type_shader, szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szName);
		return -1;
	}

	if (type_shader == SHADER_TYPE_VERTEX)
	{
		for (int i = 0; i < m_aVS.size(); ++i)
		{
			if (strcmp(m_aVS[i]->m_szName, szName) == 0)
			{
				return i;
			}
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		for(int i = 0; i<m_aPS.size(); ++i)
		{
			if(strcmp(m_aPS[i]->m_szName, szName) == 0)
			{
				return i;
			}
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		for(int i = 0; i<m_aGS.size(); ++i)
		{
			if(strcmp(m_aGS[i]->m_szName, szName) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}

ID CShaderManager::createKit(ID idVertexShader, ID idPixelShader, ID idGeometryShader)
{
	if(!(idVertexShader >= 0 && idVertexShader < m_aVS.size()) && !(idPixelShader >= 0 && idPixelShader < m_aPS.size()) && !(idGeometryShader >= 0 && idGeometryShader < m_aGS.size()))
		return -1;

	ID idShaderKit = -1;

	for (int i = 0, il = m_aShaderKit.size(); i < il; ++i)
	{
		if(m_aShaderKit[i] && m_aShaderKit[i]->m_idVertexShader == idVertexShader && m_aShaderKit[i]->m_idPixelShader == idPixelShader && m_aShaderKit[i]->m_idGeometryShader == idGeometryShader)
		{
			idShaderKit = i;
			break;
		}
	}

	if (idShaderKit == -1)
	{
		CShaderKit *pShaderKit = new CShaderKit();
		pShaderKit->m_idVertexShader = idVertexShader;
		pShaderKit->m_idPixelShader = idPixelShader;
		pShaderKit->m_idGeometryShader = idGeometryShader;

		if((!ID_VALID(idVertexShader) || m_aVS[idVertexShader]->m_pVertexShader) && (!ID_VALID(idPixelShader) || m_aPS[idPixelShader]->m_pPixelShader) && (!ID_VALID(idPixelShader) || m_aGS[idPixelShader]->m_pGeometryShader))
			pShaderKit->m_pShaderKit = g_pDevice->createShader(ID_VALID(idVertexShader) ? m_aVS[idVertexShader]->m_pVertexShader : NULL, ID_VALID(idPixelShader) ? m_aPS[idPixelShader]->m_pPixelShader : NULL, ID_VALID(idGeometryShader) ? m_aGS[idGeometryShader]->m_pGeometryShader : NULL);

		m_aShaderKit.push_back(pShaderKit);
		idShaderKit = m_aShaderKit.size() - 1;
	}

	return idShaderKit;
}

void CShaderManager::bind(ID idShaderKit)
{
	if (!(idShaderKit >= 0 && idShaderKit < m_aShaderKit.size() && m_aShaderKit[idShaderKit]))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "shader kit not bind, id [%d]\n", idShaderKit);
		return;
	}

	g_pDevice->setShader(m_aShaderKit[idShaderKit]->m_pShaderKit);
}

void CShaderManager::unbind()
{
	g_pDevice->setShader(0);
}


void CShaderManager::setValueRegisterF(SHADER_TYPE type_shader, const char *szNameShader, const char *szNameVar, void *pData, int iCountFloat4)
{
	updateDataInfo4FailSend();

	if (!isValidateTypeName(type_shader, szNameShader))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szNameShader);
		return;
	}

	if (type_shader == SHADER_TYPE_VERTEX)
	{
		ID idShader = -1;
		
		for (int i = 0; i<m_aVS.size(); i++)
		{
			if (strcmp(m_aVS[i]->m_szName, szNameShader) == 0)
			{
				idShader = i;
				break;
			}
		}


		if (idShader >= 0)
		{
			IGXVertexShader *pVertexShader = m_aVS[idShader]->m_pVertexShader;
			UINT uConstantLocation = pVertexShader->getConstantLocation(szNameVar);
			
			if (uConstantLocation == GX_SHADER_CONSTANT_FAIL)
			{
				if (m_canInfo4FailSend)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
			}
			else
			{
				pVertexShader->setConstantF(uConstantLocation, (float*)pData, (iCountFloat4 == 0 ? pVertexShader->getConstantSizeV4(szNameVar) : iCountFloat4));
			}
		}
		else
		{
			if (m_canInfo4FailSend)
			{
				if (idShader == -1)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, shader not found, type [%d], id [%d]\n", szNameVar, type_shader, szNameShader);
			}
		}
	}
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		ID idShader = -1;
		
		for (int i = 0; i<m_aPS.size(); i++)
		{
			if (strcmp(m_aPS[i]->m_szName, szNameShader) == 0)
			{
				idShader = i;
				break;
			}
		}

		if (idShader != -1)
		{
			IGXPixelShader *pPixelShader = m_aPS[idShader]->m_pPixelShader;
			UINT uConstantLocation = pPixelShader->getConstantLocation(szNameVar);

			if (uConstantLocation == GX_SHADER_CONSTANT_FAIL)
			{
				if (m_canInfo4FailSend)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
			}
			else
			{
				pPixelShader->setConstantF(uConstantLocation, (float*)pData, (iCountFloat4 == 0 ? pPixelShader->getConstantSizeV4(szNameVar) : iCountFloat4));
			}
		}
		else
		{
			if (m_canInfo4FailSend)
			{
				if (idShader == -1)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set pixel shader constant [%s] is failed, shader not found, type [%d], id [%d]\n", szNameVar, type_shader, szNameShader);
			}
		}
	}
}

void CShaderManager::setValueRegisterF(SHADER_TYPE type_shader, ID idShader, const char *szNameVar, void *pData, int iCountFloat4)
{
	updateDataInfo4FailSend();

	if (isValidated(type_shader, idShader))
	{
		if (type_shader == SHADER_TYPE_VERTEX)
		{
			IGXVertexShader *pVertexShader = m_aVS[idShader]->m_pVertexShader;
			UINT uConstantLocation = pVertexShader->getConstantLocation(szNameVar);

			if (uConstantLocation == GX_SHADER_CONSTANT_FAIL)
			{
				if (m_canInfo4FailSend)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
			}
			else
			{
				pVertexShader->setConstantF(uConstantLocation, (float*)pData, (iCountFloat4 == 0 ? pVertexShader->getConstantSizeV4(szNameVar) : iCountFloat4));
			}
		}
		else if (type_shader == SHADER_TYPE_PIXEL)
		{
			IGXPixelShader *pPixelShader = m_aPS[idShader]->m_pPixelShader;
			UINT uConstantLocation = pPixelShader->getConstantLocation(szNameVar);

			if (uConstantLocation == GX_SHADER_CONSTANT_FAIL)
			{
				if (m_canInfo4FailSend)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
			}
			else
			{
				pPixelShader->setConstantF(uConstantLocation, (float*)pData, (iCountFloat4 == 0 ? pPixelShader->getConstantSizeV4(szNameVar) : iCountFloat4));
			}
		}
	}
	else
	{
		if (m_canInfo4FailSend)
			LibReport(REPORT_MSG_LEVEL_WARNING, "set shader constant [%s] is failed, shader not validate, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
	}
}

void CShaderManager::setValueRegisterI(SHADER_TYPE type_shader, const char *szNameShader, const char *szNameVar, void *pData, int iCountInt4)
{
	updateDataInfo4FailSend();

	if (!isValidateTypeName(type_shader, szNameShader))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szNameShader);
		return;
	}

	if (type_shader == SHADER_TYPE_VERTEX)
	{
		ID idShader = -1;

		for (int i = 0; i<m_aVS.size(); i++)
		{
			if (strcmp(m_aVS[i]->m_szName, szNameShader) == 0)
			{
				idShader = i;
				break;
			}
		}


		if (idShader >= 0)
		{
			IGXVertexShader *pVertexShader = m_aVS[idShader]->m_pVertexShader;

			if (pVertexShader->getConstantLocation(szNameVar) == GX_SHADER_CONSTANT_FAIL)
			{
				if (m_canInfo4FailSend)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
			}
			else
			{
				pVertexShader->setConstantI(pVertexShader->getConstantLocation(szNameVar), (int*)pData, (iCountInt4 == 0 ? pVertexShader->getConstantSizeV4(szNameVar) : iCountInt4));
			}
		}
		else
		{
			if (m_canInfo4FailSend)
			{
				if (idShader == -1)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, shader not found, type [%d], id [%d]\n", szNameVar, type_shader, szNameShader);
			}
		}
	}
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		ID idShader = -1;

		for (int i = 0; i<m_aPS.size(); i++)
		{
			if (strcmp(m_aPS[i]->m_szName, szNameShader) == 0)
			{
				idShader = i;
				break;
			}
		}

		if (idShader != -1)
		{
			IGXPixelShader *pPixelShader = m_aPS[idShader]->m_pPixelShader;

			if (pPixelShader->getConstantLocation(szNameVar) == GX_SHADER_CONSTANT_FAIL)
			{
				if (m_canInfo4FailSend)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
			}
			else
			{
				pPixelShader->setConstantI(pPixelShader->getConstantLocation(szNameVar), (int*)pData, (iCountInt4 == 0 ? pPixelShader->getConstantSizeV4(szNameVar) : iCountInt4));
			}
		}
		else
		{
			if (m_canInfo4FailSend)
			{
				if (idShader == -1)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set pixel shader constant [%s] is failed, shader not found, type [%d], id [%d]\n", szNameVar, type_shader, szNameShader);
			}
		}
	}
}

void CShaderManager::setValueRegisterI(SHADER_TYPE type_shader, ID idShader, const char *szNameVar, void *pData, int iCountInt4)
{
	updateDataInfo4FailSend();

	if (isValidated(type_shader, idShader))
	{
		if (type_shader == SHADER_TYPE_VERTEX)
		{
			IGXVertexShader *pVertexShader = m_aVS[idShader]->m_pVertexShader;
			UINT uConstantLocation = pVertexShader->getConstantLocation(szNameVar);

			if (uConstantLocation == GX_SHADER_CONSTANT_FAIL)
			{
				if (m_canInfo4FailSend)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
			}
			else
			{
				pVertexShader->setConstantI(uConstantLocation, (int*)pData, (iCountInt4 == 0 ? pVertexShader->getConstantSizeV4(szNameVar) : iCountInt4));
			}
		}
		else if (type_shader == SHADER_TYPE_PIXEL)
		{
			IGXPixelShader *pPixelShader = m_aPS[idShader]->m_pPixelShader;
			UINT uConstantLocation = pPixelShader->getConstantLocation(szNameVar);

			if (uConstantLocation == GX_SHADER_CONSTANT_FAIL)
			{
				if (m_canInfo4FailSend)
					LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
			}
			else
			{
				pPixelShader->setConstantI(uConstantLocation, (int*)pData, (iCountInt4 == 0 ? pPixelShader->getConstantSizeV4(szNameVar) : iCountInt4));
			}
		}
	}
	else
	{
		if (m_canInfo4FailSend)
			LibReport(REPORT_MSG_LEVEL_WARNING, "set shader constant [%s] is failed, shader not validate, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
	}
}


bool CShaderManager::isValidated(SHADER_TYPE type_shader, ID idShader)
{
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		if (idShader < m_aVS.size())
		{
			if (m_aVS[idShader]->m_pVertexShader)
				return true;
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		if(idShader < m_aPS.size())
		{
			if(m_aPS[idShader]->m_pPixelShader)
				return true;
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		if(idShader < m_aGS.size())
		{
			if(m_aGS[idShader]->m_pGeometryShader)
				return true;
		}
	}
	return false;
}

void CShaderManager::getPath(SHADER_TYPE type_shader, ID idShader, char *szPath)
{
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		if (idShader < m_aVS.size())
		{
			//if (m_aVS[idShader]->m_pVertexShader)
				sprintf(szPath, "%s", m_aVS[idShader]->m_szPath);
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		if(idShader < m_aPS.size())
		{
			//if (m_aPS[idShader]->m_pPixelShader)
			sprintf(szPath, "%s", m_aPS[idShader]->m_szPath);
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		if(idShader < m_aGS.size())
		{
			//if (m_aGS[idShader]->m_pPixelShader)
			sprintf(szPath, "%s", m_aGS[idShader]->m_szPath);
		}
	}
}

void CShaderManager::getName(SHADER_TYPE type_shader, ID idShader, char *szName)
{
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		if (idShader < m_aVS.size())
		{
			if (m_aVS[idShader]->m_pVertexShader)
				sprintf(szName, "%s", m_aVS[idShader]->m_szName);
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		if(idShader < m_aPS.size())
		{
			if(m_aPS[idShader]->m_pPixelShader)
				sprintf(szName, "%s", m_aPS[idShader]->m_szName);
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		if(idShader < m_aGS.size())
		{
			if(m_aGS[idShader]->m_pGeometryShader)
				sprintf(szName, "%s", m_aGS[idShader]->m_szName);
		}
	}
}


void CShaderManager::updateDataInfo4FailSend()
{
	static const bool *r_shader_sendfail_info = GET_PCVAR_BOOL("r_shader_sendfail_info");

	if (!r_shader_sendfail_info)
		r_shader_sendfail_info = GET_PCVAR_BOOL("r_shader_sendfail_info");

	if (r_shader_sendfail_info)
		m_canInfo4FailSend = *r_shader_sendfail_info;
}
