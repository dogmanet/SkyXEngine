
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
	pSFC->m_iCountVar = pShader->m_iCountVar;
	pSFC->m_pCode = pShader->m_pCode;
	pSFC->m_pCode->AddRef();
	memcpy(pSFC->m_aVarDesc, pShader->m_aVarDesc, sizeof(D3DXCONSTANT_DESC) * SXGC_SHADER_VAR_MAX_COUNT);
	memcpy(pSFC->m_aMacros, pShader->m_aMacros, sizeof(D3DXMACRO) * SXGC_SHADER_COUNT_MACRO);

	return pSFC;
}

void SaveShaderFileCache(CShaderFileCache *pShaderFileCache)
{
	char szDirPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szFullPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szFullPathCache[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szDir[SXGC_SHADER_MAX_SIZE_DIR];
	szDir[0] = 0;
	char szName[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(pShaderFileCache->m_szName, szDir, szName);
	if (szDir[0] != 0)
	{
		sprintf(szDirPath, "%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, szDir);
		sprintf(szFullPath, "%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), szDir, pShaderFileCache->m_szName);
		sprintf(szFullPathCache, "%s%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, szDir, pShaderFileCache->m_szName);
	}
	else
	{
		sprintf(szDirPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH);
		sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), pShaderFileCache->m_szName);
		sprintf(szFullPathCache, "%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH, pShaderFileCache->m_szName);
	}

	CreateDirectory(szDirPath, 0);
	//SHCreateDirectory

	FILE *pFile = fopen(szFullPathCache, "wb");

	if(!pFile)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to open file '%s' for writing", szFullPathCache);
		return;
	}

	uint32_t tLastModify = FileGetTimeLastModify(szFullPath);

	fwrite(&tLastModify, sizeof(uint32_t), 1, pFile);

	uint32_t iLen = strlen(pShaderFileCache->m_szName);
	fwrite(&(iLen), sizeof(uint32_t), 1, pFile);
	fwrite((pShaderFileCache->m_szName), sizeof(char)* iLen, 1, pFile);

	fwrite(&(pShaderFileCache->m_iCountVar), sizeof(uint32_t), 1, pFile);

	for (int i = 0; i < pShaderFileCache->m_iCountVar; ++i)
	{
		uint32_t iLen = strlen(pShaderFileCache->m_aVarDesc[i].Name);
		fwrite(&(iLen), sizeof(uint32_t), 1, pFile);
		fwrite((pShaderFileCache->m_aVarDesc[i].Name), sizeof(char)* iLen, 1, pFile);

		fwrite(&(pShaderFileCache->m_aVarDesc[i].RegisterSet), sizeof(uint32_t), 1, pFile);
		fwrite(&(pShaderFileCache->m_aVarDesc[i].RegisterIndex), sizeof(uint32_t), 1, pFile);
		fwrite(&(pShaderFileCache->m_aVarDesc[i].RegisterCount), sizeof(uint32_t), 1, pFile);
		fwrite(&(pShaderFileCache->m_aVarDesc[i].Class), sizeof(uint32_t), 1, pFile);
		fwrite(&(pShaderFileCache->m_aVarDesc[i].Type), sizeof(uint32_t), 1, pFile);
	}

	uint32_t iCountMacros = 0;
	for (int i = 0; i < SXGC_SHADER_COUNT_MACRO; ++i)
	{
		if ((pShaderFileCache->m_aMacros[i].Name && strlen(pShaderFileCache->m_aMacros[i].Name) > 0))
			++iCountMacros;
	}

	fwrite(&iCountMacros, sizeof(uint32_t), 1, pFile);

	if (iCountMacros > 0)
	{
		for (int i = 0; i < iCountMacros; ++i)
		{
			if (!(pShaderFileCache->m_aMacros[i].Name && strlen(pShaderFileCache->m_aMacros[i].Name) > 0))
				break;

			uint32_t iLen = strlen(pShaderFileCache->m_aMacros[i].Name);
			fwrite(&(iLen), sizeof(uint32_t), 1, pFile);
			fwrite((pShaderFileCache->m_aMacros[i].Name), sizeof(char)* iLen, 1, pFile);

			iLen = strlen(pShaderFileCache->m_aMacros[i].Definition);
			fwrite(&(iLen), sizeof(uint32_t), 1, pFile);
			fwrite((pShaderFileCache->m_aMacros[i].Definition), sizeof(char)* iLen, 1, pFile);
		}
	}

	uint32_t iSizeBinCode = pShaderFileCache->m_pCode->GetBufferSize();
	fwrite(&iSizeBinCode, sizeof(uint32_t), 1, pFile);
	fwrite(pShaderFileCache->m_pCode->GetBufferPointer(), iSizeBinCode, 1, pFile);

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

	fread(&(pSFC->m_iCountVar), sizeof(uint32_t), 1, pFile);

	for (int i = 0; i < pSFC->m_iCountVar; ++i)
	{
		uint32_t iLen = 0;
		fread(&(iLen), sizeof(uint32_t), 1, pFile);
		pSFC->m_aVarDesc[i].Name = new const char[iLen + 1];
		ZeroMemory((void*)(pSFC->m_aVarDesc[i].Name), iLen + 1);
		fread((void*)(pSFC->m_aVarDesc[i].Name), sizeof(char)* iLen, 1, pFile);

		fread(&(pSFC->m_aVarDesc[i].RegisterSet), sizeof(uint32_t), 1, pFile);
		fread(&(pSFC->m_aVarDesc[i].RegisterIndex), sizeof(uint32_t), 1, pFile);
		fread(&(pSFC->m_aVarDesc[i].RegisterCount), sizeof(uint32_t), 1, pFile);
		fread(&(pSFC->m_aVarDesc[i].Class), sizeof(uint32_t), 1, pFile);
		fread(&(pSFC->m_aVarDesc[i].Type), sizeof(uint32_t), 1, pFile);
	}

	uint32_t iCountMacros = 0;
	fread(&iCountMacros, sizeof(uint32_t), 1, pFile);

	for (int i = 0; i < iCountMacros; ++i)
	{
		uint32_t iLen = 0;
		fread(&(iLen), sizeof(uint32_t), 1, pFile);

		pSFC->m_aMacros[i].Name = new const char[iLen + 1];
		ZeroMemory((void*)(pSFC->m_aMacros[i].Name), iLen + 1);

		fread((void*)(pSFC->m_aMacros[i].Name), sizeof(char)* iLen, 1, pFile);


		iLen = 0;
		fread(&(iLen), sizeof(uint32_t), 1, pFile);

		pSFC->m_aMacros[i].Definition = new const char[iLen + 1];
		ZeroMemory((void*)(pSFC->m_aMacros[i].Definition), iLen + 1);

		fread((void*)(pSFC->m_aMacros[i].Definition), sizeof(char)* iLen, 1, pFile);
	}

	uint32_t iSizeBinCode;
	fread(&iSizeBinCode, sizeof(uint32_t), 1, pFile);

	D3DXCreateBuffer(iSizeBinCode, &(pSFC->m_pCode));
	fread(pSFC->m_pCode->GetBufferPointer(), iSizeBinCode, 1, pFile);

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

void LoadVertexShader(const char *szPath, CShaderVS *pShader, D3DXMACRO *aMacro)
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

#ifndef _DEBUG
	if (g_useCache && FileExistsFile(szFullPathCache) && GetTimeShaderFileCache(szFullPathCache) == FileGetTimeLastModify(szFullPath))
	{
		CShaderFileCache *pSFC = CreateShaderFileCacheFormFile(szFullPathCache);

		pShader->m_iCountVar = pSFC->m_iCountVar;
		//strcpy(pShader->m_szName, pSFC->m_szName);
		memcpy(pShader->m_aVarDesc, pSFC->m_aVarDesc, sizeof(D3DXCONSTANT_DESC) * SXGC_SHADER_VAR_MAX_COUNT);
		memcpy(pShader->m_aMacros, pSFC->m_aMacros, sizeof(D3DXMACRO) * SXGC_SHADER_COUNT_MACRO);
		pShader->m_pCode = pSFC->m_pCode;
		pShader->m_pCode->AddRef();

		HRESULT hr = g_pDXDevice->CreateVertexShader(
			(DWORD*)pShader->m_pCode->GetBufferPointer(),
			&(pShader->m_pVertexShader)
			);
		if (FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error creating vertex shader [%s]\n", GEN_MSG_LOCATION, szPath);
			return;
		}

		mem_delete(pSFC);
	}
	else
	{
#endif
		ID3DXBuffer *pBufShader = 0;
		ID3DXBuffer *pBufError = 0;
		ID3DXConstantTable *pConstTable;
		IDirect3DVertexShader9 *pVertexShader;
		D3DXHANDLE aHandles[SXGC_SHADER_VAR_MAX_COUNT];

		HRESULT hr = D3DXCompileShaderFromFile(
			szFullPath,
			aMacro,
			0,
			"main",
			"vs_3_0",
			SHADER_FLAGS,
			&pBufShader,
			&pBufError,
			&pConstTable
			);

		if (pBufError && pBufShader == 0)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed to load vertex shader [%s], msg: %s\n", GEN_MSG_LOCATION, szPath, (char*)pBufError->GetBufferPointer());
			return;
		}

		//!!!проанализировать
		if (FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - download function vertex shader fails, path [%s]\n", GEN_MSG_LOCATION, szPath);
			return;
		}

		hr = g_pDXDevice->CreateVertexShader(
			(DWORD*)pBufShader->GetBufferPointer(),
			&pVertexShader
			);

		if (FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error creating vertex shader [%s]\n", GEN_MSG_LOCATION, szPath);
			return;
		}

		D3DXCONSTANTTABLE_DESC desc;
		pConstTable->GetDesc(&desc);

		if (desc.Constants > SXGC_SHADER_VAR_MAX_COUNT)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error, count variable in vertex shader [%s] more standart [%d]\n", GEN_MSG_LOCATION, szPath, SXGC_SHADER_VAR_MAX_COUNT);
			return;
		}

		for (int i = 0; i < desc.Constants; i++)
		{
			aHandles[i] = pConstTable->GetConstant(0, i);

			UINT count = 0;
			pConstTable->GetConstantDesc(aHandles[i], &(pShader->m_aVarDesc[i]), &count);
			int qwert = 0;
		}

		pShader->m_pCode = pBufShader;
		pShader->m_iCountVar = desc.Constants;
		pShader->m_pVertexShader = pVertexShader;

#ifndef _DEBUG
		CShaderFileCache *pSFC = CreateShaderFileCacheFormShader(pShader);
		SaveShaderFileCache(pSFC);
		mem_delete(pSFC);
	}
#endif
}

void LoadPixelShader(const char *szPath, CShaderPS *pShader,D3DXMACRO *aMacro)
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

#ifndef _DEBUG
	if (g_useCache && FileExistsFile(szFullPathCache) && GetTimeShaderFileCache(szFullPathCache) == FileGetTimeLastModify(szFullPath))
	{
		CShaderFileCache *pSFC = CreateShaderFileCacheFormFile(szFullPathCache);

		pShader->m_iCountVar = pSFC->m_iCountVar;
		memcpy(pShader->m_aVarDesc, pSFC->m_aVarDesc, sizeof(D3DXCONSTANT_DESC)* SXGC_SHADER_VAR_MAX_COUNT);
		memcpy(pShader->m_aMacros, pSFC->m_aMacros, sizeof(D3DXMACRO)* SXGC_SHADER_COUNT_MACRO);
		pShader->m_pCode = pSFC->m_pCode;
		pShader->m_pCode->AddRef();

		HRESULT hr = g_pDXDevice->CreatePixelShader(
			(DWORD*)pShader->m_pCode->GetBufferPointer(),
			&(pShader->m_pPixelShader)
			);
		if (FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error creating pixel shader [%s]", GEN_MSG_LOCATION, szPath);
			return;
		}

		mem_delete(pSFC);
	}
	else
	{
#endif
		ID3DXBuffer *pBufShader = 0;
		ID3DXBuffer *pBufError = 0;
		ID3DXConstantTable *pConstTable;
		IDirect3DPixelShader9 *pPixelShader;
		D3DXHANDLE aHandles[SXGC_SHADER_VAR_MAX_COUNT];

		HRESULT hr = D3DXCompileShaderFromFile(
			szFullPath,
			aMacro,
			0,
			"main",
			"ps_3_0",
			SHADER_FLAGS,
			&pBufShader,
			&pBufError,
			&pConstTable
			);

		if (pBufError && pBufShader == 0)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed to load pixel shader [%s], msg: %s\n", GEN_MSG_LOCATION, szPath, (char*)pBufError->GetBufferPointer());
			return;
		}

		//!!!проанализировать
		if (FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - download function pixel shader fails, path [%s]\n", GEN_MSG_LOCATION, szPath);
			return;
		}

		hr = g_pDXDevice->CreatePixelShader(
			(DWORD*)pBufShader->GetBufferPointer(),
			&pPixelShader
			);

		if (FAILED(hr))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error creating pixel shader [%s]\n", GEN_MSG_LOCATION, szPath);
			return;
		}

		D3DXCONSTANTTABLE_DESC desc;
		pConstTable->GetDesc(&desc);

		if (desc.Constants > SXGC_SHADER_VAR_MAX_COUNT)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error, count variable in pixel shader [%s] more standart [%d]\n", GEN_MSG_LOCATION, szPath, SXGC_SHADER_VAR_MAX_COUNT);
			return;
		}


		for (int i = 0; i < desc.Constants; i++)
		{
			aHandles[i] = pConstTable->GetConstant(0, i);

			UINT count = 0;
			pConstTable->GetConstantDesc(aHandles[i], &(pShader->m_aVarDesc[i]), &count);
		}

		pShader->m_pCode = pBufShader;
		pShader->m_iCountVar = desc.Constants;
		pShader->m_pPixelShader = pPixelShader;

#ifndef _DEBUG
		CShaderFileCache *pSFC = CreateShaderFileCacheFormShader(pShader);
		SaveShaderFileCache(pSFC);
		mem_delete(pSFC);
	}
#endif
}

//**************************************************************************

CShaderManager::CShaderManager()
{
	g_useCache = true;
	m_iLastAllLoadVS = 0;
	m_iLastAllLoadPS = 0;

	char szFullPathCache[1024];
	sprintf(szFullPathCache, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH);

	if (!FileExistsDir(szFullPathCache))
		FileCreateDir(szFullPathCache);

	loadCacheInclude();

	Array<String> aIncludes = FileGetListRec(Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), FILE_LIST_TYPE_FILES, "h");

	if (aIncludes.size() != m_aIncludes.size())
		g_useCache = false;
	else
	{
		for (int i = 0, il = m_aIncludes.size(); i < il; ++i)
		{
			if (
				!(stricmp(m_aIncludes[i].m_szFile, aIncludes[i].c_str()) == 0 &&
				FileGetTimeLastModify((String(Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS)) + aIncludes[i]).c_str()) == m_aIncludes[i].m_uiDate)
				)
				g_useCache = false;
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

	int qwerty = 0;
}

CShaderManager::~CShaderManager()
{
	for (int i = 0; i < m_aVS.size(); ++i)
	{
		mem_delete(m_aVS[i]);
	}
			

	for (int i = 0; i < m_aPS.size(); ++i)
	{
		mem_delete(m_aPS[i]);
	}
			
	m_aVS.clear();
	m_aPS.clear();
}

void CShaderManager::loadCacheInclude()
{
	char szFullPathCacheInc[1024];
	sprintf(szFullPathCacheInc, "%s%s/includes", Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SHADERS_CACHE_PATH);

	if (!FileExistsFile(szFullPathCacheInc))
		return;

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
	LibReport(REPORT_MSG_LEVEL_NOTICE, "reload shaders ...\n");

	for (int i = 0; i<m_aVS.size(); i++)
	{
		CShaderVS *pShader = m_aVS[i];
		LoadVertexShader(pShader->m_szPath, pShader, pShader->m_aMacros);
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  VS id [%d], file [%s], name [%s], \n", i, pShader->m_szPath, pShader->m_szName);
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for (int i = 0; i<m_aPS.size(); i++)
	{
		CShaderPS *pShader = m_aPS[i];
		LoadPixelShader(m_aPS[i]->m_szPath, m_aPS[i], m_aPS[i]->m_aMacros);
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  PS id [%d], file[%s], name[%s], \n", i, pShader->m_szPath, pShader->m_szName);
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
	else if (type == SHADER_TYPE_PIXEL)
	{
		for (int i = 0; i<m_aPS.size(); ++i)
		{
			if (strcmp(m_aPS[i]->m_szPath, szPath) == 0)
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
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		for (int i = 0; i<m_aPS.size(); ++i)
		{
			if (strcmp(m_aPS[i]->m_szName, szName) == 0)
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

	return (type == SHADER_TYPE_VERTEX && stricmp(szEndStr, ".vs") == 0) || (type == SHADER_TYPE_PIXEL && stricmp(szEndStr, ".ps") == 0);
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

ID CShaderManager::preLoad(SHADER_TYPE type, const char *szPath, const char *szName, SHADER_CHECKDOUBLE check_double, D3DXMACRO *aMacros)
{
	if (!isValidateTypeName(type, szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szName);
		return -1;
	}

	ID id = -1;

	if (check_double == SHADER_CHECKDOUBLE_PATH)
		id = existsPath(type, szPath);
	else if (check_double == SHADER_CHECKDOUBLE_NAME)
		id = existsName(type, szName);

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
	else if (type == SHADER_TYPE_PIXEL)
	{
		if (id == -1)
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

	if (aMacros)
	{
		int iCountMacros = 0;
		for (int i = 0; i < SXGC_SHADER_COUNT_MACRO; i++)
		{
			if (aMacros[i].Name == 0)
			{
				iCountMacros = i;
				break;
			}
		}

		for (int i = 0; i < iCountMacros; i++)
		{
			pShader->m_aMacros[i] = aMacros[i];
		}

		pShader->m_aMacros[iCountMacros].Name = 0;
		pShader->m_aMacros[iCountMacros].Definition = 0;
	}

	return id;
}

void CShaderManager::allLoad()
{
	if (m_aVS.size() == m_iLastAllLoadVS || m_aPS.size() == m_iLastAllLoadPS)
		return;

	DWORD dwTime = GetTickCount();
	LibReport(REPORT_MSG_LEVEL_NOTICE, "load shaders ...\n");
	
	for (int i = 0, il = m_aVS.size(); i < il; ++i)
	{
		CShaderVS *pShader = m_aVS[i];
		LoadVertexShader(pShader->m_szPath, pShader, pShader->m_aMacros);

		LibReport(REPORT_MSG_LEVEL_NOTICE, "  VS id [%d], file [%s], name [%s], \n", i, pShader->m_szPath, pShader->m_szName);
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for (int i = 0, il = m_aPS.size(); i < il; ++i)
	{
		CShaderPS *pShader = m_aPS[i];
		LoadPixelShader(pShader->m_szPath, pShader, pShader->m_aMacros);

		LibReport(REPORT_MSG_LEVEL_NOTICE, "  PS id [%d], file[%s], name[%s], \n", i, pShader->m_szPath, pShader->m_szName);
	}

	m_iLastAllLoadVS = m_aVS.size();
	m_iLastAllLoadPS = m_aPS.size();

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
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		for (int i = 0; i<m_aPS.size(); i++)
		{
			if (strcmp(m_aPS[i]->m_szName, szName) == 0)
			{
				CShaderPS *pShader = m_aPS[i];
				mem_release(m_aPS[i]->m_pPixelShader);
				LoadPixelShader(m_aPS[i]->m_szPath, pShader, m_aPS[i]->m_aMacros);
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
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		if (m_aPS.size() > id  && m_aPS[id])
		{
			CShaderPS *pShader = m_aPS[id];
			mem_release(m_aPS[id]->m_pPixelShader);
			LoadPixelShader(m_aPS[id]->m_szPath, pShader, m_aPS[id]->m_aMacros);
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
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		for (int i = 0; i<m_aPS.size(); ++i)
		{
			if (strcmp(m_aPS[i]->m_szName, szName) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}

void CShaderManager::bind(SHADER_TYPE type_shader, const char *szName)
{
	if (!isValidateTypeName(type_shader, szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szName);
		return;
	}

	bool isbind = false;
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		for (int i = 0; i < m_aVS.size(); ++i)
		{
			if (strcmp(m_aVS[i]->m_szName, szName) == 0)
			{
				g_pDXDevice->SetVertexShader(m_aVS[i]->m_pVertexShader);
				isbind = true;
				break;
			}
		}
	}
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		for (int i = 0; i < m_aPS.size(); ++i)
		{
			if (strcmp(m_aPS[i]->m_szName, szName) == 0)
			{
				g_pDXDevice->SetPixelShader(m_aPS[i]->m_pPixelShader);
				isbind = true;
				break;
			}
		}
	}

	if (!isbind)
		LibReport(REPORT_MSG_LEVEL_WARNING, "shader not bind, type [%d], name [%s]\n", type_shader, szName);
}

void CShaderManager::bind(SHADER_TYPE type_shader, ID idShader)
{
	bool isBind = false;
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		if (idShader < m_aVS.size())
		{
			isBind = true;
			g_pDXDevice->SetVertexShader(m_aVS[idShader]->m_pVertexShader);
		}
	}
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		if (idShader < m_aPS.size())
		{
			isBind = true;
			g_pDXDevice->SetPixelShader(m_aPS[idShader]->m_pPixelShader);
		}
	}

	if (!isBind)
		LibReport(REPORT_MSG_LEVEL_WARNING, "shader not bind, type [%d], id [%d]\n", type_shader, idShader);
}

void CShaderManager::unbind()
{
	g_pDXDevice->SetVertexShader(0);
	g_pDXDevice->SetPixelShader(0);
}


void CShaderManager::setValueRegisterF(SHADER_TYPE type_shader, const char *szNameShader, const char *szNameVar, void *pData, int iCountFloat4)
{
	if (!isValidateTypeName(type_shader, szNameShader))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szNameShader);
		return;
	}

	if (type_shader == SHADER_TYPE_VERTEX)
	{
		int num_shader = -1;
		int num_var = -1;

		for (int i = 0; i<m_aVS.size(); i++)
		{
			if (strcmp(m_aVS[i]->m_szName, szNameShader) == 0)
			{
				num_shader = i;
				i = m_aVS.size();
				for (int k = 0; k<m_aVS[num_shader]->m_iCountVar; k++)
				{
					if (strcmp(m_aVS[num_shader]->m_aVarDesc[k].Name, szNameVar) == 0)
					{
						num_var = k;
						k = m_aVS[num_shader]->m_iCountVar;
					}
				}
			}
		}

		if (num_shader != -1 && num_var != -1)
			g_pDXDevice->SetVertexShaderConstantF(m_aVS[num_shader]->m_aVarDesc[num_var].RegisterIndex, (float*)pData, (iCountFloat4 == 0 ? m_aVS[num_shader]->m_aVarDesc[num_var].RegisterCount : iCountFloat4));
		else
		{
			if (num_shader == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, shader not found, type [%d], id [%d]\n", szNameVar, type_shader, szNameShader);
			else if (num_var == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, shader info: type [%d], id [%d], name [%s]\n", szNameVar, type_shader, szNameShader);
		}
	}
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		int num_shader = -1;
		int num_var = -1;

		for (int i = 0; i<m_aPS.size(); i++)
		{
			if (strcmp(m_aPS[i]->m_szName, szNameShader) == 0)
			{
				num_shader = i;
				i = m_aPS.size();
				for (int k = 0; k<m_aPS[num_shader]->m_iCountVar; k++)
				{
					if (strcmp(m_aPS[num_shader]->m_aVarDesc[k].Name, szNameVar) == 0)
					{
						num_var = k;
						k = m_aPS[num_shader]->m_iCountVar;
					}
				}
			}
		}

		if (num_shader != -1 && num_var != -1)
			g_pDXDevice->SetPixelShaderConstantF(m_aPS[num_shader]->m_aVarDesc[num_var].RegisterIndex, (float*)pData, (iCountFloat4 == 0 ? m_aPS[num_shader]->m_aVarDesc[num_var].RegisterCount : iCountFloat4));
		else
		{
			if (num_shader == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set pixel shader constant [%s] is failed, shader not found, type [%d], id [%d]\n", szNameVar, type_shader, szNameShader);
			else if (num_var == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set pixel shader constant [%s] is failed, constant not found, shader info: type [%d], id [%d], name [%s]\n", szNameVar, type_shader, szNameShader);
		}
	}
}

void CShaderManager::setValueRegisterF(SHADER_TYPE type_shader, ID idShader, const char *szNameVar, void *pData, int iCountFloat4)
{
	if (isValidated(type_shader, idShader))
	{
		if (type_shader == SHADER_TYPE_VERTEX)
		{
			int num_var = -1;

			for (int k = 0; k<m_aVS[idShader]->m_iCountVar; k++)
			{
				if (strcmp(m_aVS[idShader]->m_aVarDesc[k].Name, szNameVar) == 0)
				{
					num_var = k;
					k = m_aVS[idShader]->m_iCountVar;
				}
			}

			if (idShader != -1 && num_var != -1)
				g_pDXDevice->SetVertexShaderConstantF(m_aVS[idShader]->m_aVarDesc[num_var].RegisterIndex, (float*)pData, (iCountFloat4 == 0 ? m_aVS[idShader]->m_aVarDesc[num_var].RegisterCount : iCountFloat4));
			else if (num_var == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
		}
		else if (type_shader == SHADER_TYPE_PIXEL)
		{
			int num_var = -1;

			for (int k = 0; k<m_aPS[idShader]->m_iCountVar; k++)
			{
				if (strcmp(m_aPS[idShader]->m_aVarDesc[k].Name, szNameVar) == 0)
				{
					num_var = k;
					k = m_aPS[idShader]->m_iCountVar;
				}
			}

			if (idShader != -1 && num_var != -1)
				g_pDXDevice->SetPixelShaderConstantF(m_aPS[idShader]->m_aVarDesc[num_var].RegisterIndex, (float*)pData, (iCountFloat4 == 0 ? m_aPS[idShader]->m_aVarDesc[num_var].RegisterCount : iCountFloat4));
			else if (num_var == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set pixel shader constant [%s] is failed, constant not found, type [%d], name [%s]\n", szNameVar, type_shader, m_aPS[idShader]->m_szPath);
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_WARNING, "set shader constant [%s] is failed, shader not validate, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
}

void CShaderManager::setValueRegisterI(SHADER_TYPE type_shader, const char *szNameShader, const char *szNameVar, void *pData, int iCountInt4)
{
	if (!isValidateTypeName(type_shader, szNameShader))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - name of shader [%s] is invalid", GEN_MSG_LOCATION, szNameShader);
		return;
	}

	if (type_shader == SHADER_TYPE_VERTEX)
	{
		int num_shader = -1;
		int num_var = -1;

		for (int i = 0; i<m_aVS.size(); i++)
		{
			if (strcmp(m_aVS[i]->m_szName, szNameShader) == 0)
			{
				num_shader = i;
				i = m_aVS.size();
				for (int k = 0; k<m_aVS[num_shader]->m_iCountVar; k++)
				{
					if (strcmp(m_aVS[num_shader]->m_aVarDesc[k].Name, szNameVar) == 0)
					{
						num_var = k;
						k = m_aVS[num_shader]->m_iCountVar;
					}
				}
			}
		}

		if (num_shader != -1 && num_var != -1)
			g_pDXDevice->SetVertexShaderConstantI(m_aVS[num_shader]->m_aVarDesc[num_var].RegisterIndex, (int*)pData, (iCountInt4 == 0 ? m_aVS[num_shader]->m_aVarDesc[num_var].RegisterCount : iCountInt4));
		else
		{
			if (num_shader == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, shader not found, type [%d], id [%d]\n", szNameVar, type_shader, szNameShader);
			else if (num_var == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, shader info: type [%d], id [%d], name [%s]\n", szNameVar, type_shader, szNameShader);
		}
	}
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		int num_shader = -1;
		int num_var = -1;

		for (int i = 0; i<m_aPS.size(); i++)
		{
			if (strcmp(m_aPS[i]->m_szName, szNameShader) == 0)
			{
				num_shader = i;
				i = m_aPS.size();
				for (int k = 0; k<m_aPS[num_shader]->m_iCountVar; k++)
				{
					if (strcmp(m_aPS[num_shader]->m_aVarDesc[k].Name, szNameVar) == 0)
					{
						num_var = k;
						k = m_aPS[num_shader]->m_iCountVar;
					}
				}
			}
		}

		if (num_shader != -1 && num_var != -1)
			g_pDXDevice->SetPixelShaderConstantI(m_aPS[num_shader]->m_aVarDesc[num_var].RegisterIndex, (int*)pData, (iCountInt4 == 0 ? m_aPS[num_shader]->m_aVarDesc[num_var].RegisterCount : iCountInt4));
		else
		{
			if (num_shader == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set pixel shader constant [%s] is failed, shader not found, type [%d], id [%d]\n", szNameVar, type_shader, szNameShader);
			else if (num_var == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set pixel shader constant [%s] is failed, constant not found, shader info: type [%d], id [%d], name [%s]\n", szNameVar, type_shader, szNameShader);
		}
	}
}

void CShaderManager::setValueRegisterI(SHADER_TYPE type_shader, ID idShader, const char *szNameVar, void *pData, int iCountInt4)
{
	if (isValidated(type_shader, idShader))
	{
		if (type_shader == SHADER_TYPE_VERTEX)
		{
			int num_var = -1;

			for (int k = 0; k<m_aVS[idShader]->m_iCountVar; k++)
			{
				if (strcmp(m_aVS[idShader]->m_aVarDesc[k].Name, szNameVar) == 0)
				{
					num_var = k;
					k = m_aVS[idShader]->m_iCountVar;
				}
			}

			if (idShader != -1 && num_var != -1)
				g_pDXDevice->SetVertexShaderConstantI(m_aVS[idShader]->m_aVarDesc[num_var].RegisterIndex, (int*)pData, (iCountInt4 == 0 ? m_aVS[idShader]->m_aVarDesc[num_var].RegisterCount : iCountInt4));
			else if (num_var == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set vertex shader constant [%s] is failed, constant not found, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
		}
		else if (type_shader == SHADER_TYPE_PIXEL)
		{
			int num_var = -1;

			for (int k = 0; k<m_aPS[idShader]->m_iCountVar; k++)
			{
				if (strcmp(m_aPS[idShader]->m_aVarDesc[k].Name, szNameVar) == 0)
				{
					num_var = k;
					k = m_aPS[idShader]->m_iCountVar;
				}
			}

			if (idShader != -1 && num_var != -1)
				g_pDXDevice->SetPixelShaderConstantI(m_aPS[idShader]->m_aVarDesc[num_var].RegisterIndex, (int*)pData, (iCountInt4 == 0 ? m_aPS[idShader]->m_aVarDesc[num_var].RegisterCount : iCountInt4));
			else if (num_var == -1)
				LibReport(REPORT_MSG_LEVEL_WARNING, "set pixel shader constant [%s] is failed, constant not found, type [%d], name [%s]\n", szNameVar, type_shader, m_aPS[idShader]->m_szPath);
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_WARNING, "set shader constant [%s] is failed, shader not validate, type [%d], id [%d]\n", szNameVar, type_shader, idShader);
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
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		if (idShader < m_aPS.size())
		{
			if (m_aPS[idShader]->m_pPixelShader)
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
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		if (idShader < m_aPS.size())
		{
			//if (m_aPS[idShader]->m_pPixelShader)
				sprintf(szPath, "%s", m_aPS[idShader]->m_szPath);
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
	else if (type_shader == SHADER_TYPE_PIXEL)
	{
		if (idShader < m_aPS.size())
		{
			if (m_aPS[idShader]->m_pPixelShader)
				sprintf(szName, "%s", m_aPS[idShader]->m_szName);
		}
	}
}