
/***********************************************************
Copyright � Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SHADER_H
#define __SHADER_H

#define SHADERS_CACHE_PATH "cache/"

#include <gdefines.h>
#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <d3d9.h>
#include <common/array.h>
#include <common/String.h>
#include <common/file_utils.h>
#include "sxgcore.h"

extern IDirect3DDevice9 *g_pDXDevice;
extern D3DCAPS9 g_dxCaps;
extern D3DPRESENT_PARAMETERS g_oD3DAPP;

//! ������������ �� � ������ ������ ��� ��������?
extern bool g_useCache;

//! ������� ����� �������
struct CShader
{
	CShader()
	{ 
		m_szName[0] = 0; 
		m_szPath[0] = 0; 
		m_iCountVar = 0;

		ZeroMemory(m_aVarDesc, sizeof(D3DXCONSTANT_DESC)* SXGC_SHADER_VAR_MAX_COUNT);
		ZeroMemory(m_aMacros, sizeof(D3DXMACRO)* SXGC_SHADER_COUNT_MACRO);
	}

	~CShader()
	{
		mem_release(m_pCode);
	}

	//! ��� �������
	char m_szName[SXGC_SHADER_MAX_SIZE_NAME];

	//! ��� ����� �������
	char m_szPath[SXGC_SHADER_MAX_SIZE_DIR];

	//! ��� ��� �������� ���� ������ (�������)
	//char m_szFrom[SXGC_SHADER_MAX_SIZE_FULLPATH];

	//! ���������� ����������
	int m_iCountVar;

	//! �������� ����������
	D3DXCONSTANT_DESC m_aVarDesc[SXGC_SHADER_VAR_MAX_COUNT];

	//! ������ �������� (������ ���������� ������� ������ ���� NULL)
	D3DXMACRO m_aMacros[SXGC_SHADER_COUNT_MACRO];

	//! ����� � �������� ����� �������
	ID3DXBuffer *m_pCode;
};

//! ��������� ������
struct CShaderVS : public CShader
{
	CShaderVS(){ m_pVertexShader = 0; };
	~CShaderVS(){ mem_release(m_pVertexShader); };

	IDirect3DVertexShader9 *m_pVertexShader;
};

//! ���������� ������
struct CShaderPS : public CShader
{
	CShaderPS(){ m_pPixelShader = 0; };
	~CShaderPS(){ mem_release(m_pPixelShader); };

	IDirect3DPixelShader9 *m_pPixelShader;
};

//! ������������ ������
struct CShaderFileCache : public CShader
{
	CShaderFileCache()
	{
		m_szName[0] = 0;
		m_szPath[0] = 0; 
		m_uiDate = 0; 
		m_iCountVar = 0;
		m_pCode = 0; 
		ZeroMemory(m_aVarDesc, sizeof(D3DXCONSTANT_DESC)* SXGC_SHADER_VAR_MAX_COUNT);
		ZeroMemory(m_aMacros, sizeof(D3DXMACRO)* SXGC_SHADER_COUNT_MACRO); 
		int qwert = 0;
	}

	~CShaderFileCache()
	{
		mem_release(m_pCode);
	}

	//! ����� ���������� ��������� ������������� �������
	uint32_t m_uiDate;
};

//! ������������� ������ ��������
struct CShaderInclude
{
	CShaderInclude()
	{
		m_szFile[0] = 0;
		m_uiDate = 0;
	}

	//! ��� �����
	char m_szFile[SXGC_SHADER_MAX_SIZE_DIRNAME];

	//! ����� ���������� ���������
	uint32_t m_uiDate;
};

//**************************************************************************

//! ������� ��� �������
CShaderFileCache* CreateShaderFileCacheFormShader(CShader *pShader);

//! ��������� ������������ ������
void SaveShaderFileCache(CShaderFileCache *pShaderFileCache);

//! ������� ������������ ������ �� �����
CShaderFileCache* CreateShaderFileCacheFormFile(const char *szPath);

//! ���������� ����� ���������� ��������� ������������� ������� �� ��������� ������ � ��� ����
uint32_t GetTimeShaderFileCache(const char *szPath);

//**************************************************************************

#define LOAD_SHADER_FAIL 0
#define LOAD_SHADER_COMPLETE 1
#define LOAD_SHADER_CACHE 2

//! �������� ���������� �������
int LoadVertexShader(
	const char *szPath,		//!< ���������� ���� �� ����� �������
	CShaderVS *pShader,		//!< ������������������ ��������� CShaderVS
	D3DXMACRO *aMacro = 0	//!< ������ ��������
	);

//�������� ����������� �������
int LoadPixelShader(
	const char *szPath,		//!< ���������� ���� �� ����� �������
	CShaderPS *pShader,		//!< ������������������ ��������� CShaderPS
	D3DXMACRO *aMacro = 0	//!< ������ ��������
	);

//**************************************************************************

//! �������� ��������
class CShaderManager
{
public:
	CShaderManager();
	~CShaderManager();

	//! ���������� �� ���� name � ����� � ���������
	bool existsFile(const char *szPath);

	//! ���������� ������� � �������
	ID preLoad(SHADER_TYPE type, const char *szPath, const char *szName, SHADER_CHECKDOUBLE check_double, D3DXMACRO *aMacros = 0);

	//! �������� ���� ��������
	void allLoad();

	//! ���������� ������� �� �����
	void update(SHADER_TYPE type, const char *szName);

	//! ���������� ������� �� id
	void update(SHADER_TYPE type, ID id);

	//! ������������� ��� �������, � ������ ��������
	void reloadAll();

	//! �������� ������������� ������� �� �����
	ID getID(SHADER_TYPE type, const char *szName);

	//! ���� ������� �� �����
	void bind(SHADER_TYPE type, const char *szName);

	//! ���� ������� �� id
	void bind(SHADER_TYPE type, ID idShader);

	//! ��������� ������ ��������
	void unbind();


	//! �������� float �������� � ������ �� �����
	void setValueRegisterF(SHADER_TYPE type, const char *szNameShader, const char *szNameVar, void *pData, int iCountFloat4 = 0);

	//! �������� float �������� � ������ �� ID
	void setValueRegisterF(SHADER_TYPE type, ID idShader, const char *szNameVar, void *data, int iCountFloat4 = 0);

	//! �������� int �������� � ������ �� �����
	void setValueRegisterI(SHADER_TYPE type, const char *szNameShader, const char *szNameVar, void *pData, int iCountInt4 = 0);

	//! �������� int �������� � ������ �� ID
	void setValueRegisterI(SHADER_TYPE type, ID idShader, const char *szNameVar, void *data, int iCountInt4 = 0);


	//! ���������� �� ������ � ������ ����� � ����������� name, ���� �� �� ���������� id
	ID existsPath(SHADER_TYPE type, const char *szPath);

	//! ���������� �� ������ � ���������������� ������ name, ���� �� �� ���������� id
	ID existsName(SHADER_TYPE type, const char *szName);

	//! �������� �� ������ � ������ id
	bool isValidated(SHADER_TYPE type, ID id);

	//! �������� �� ��� � ������� (���������� ������ ��������� � �����)
	bool isValidateTypeName(SHADER_TYPE type, const char *szName);


	//! ���������� ��� ������ � ����������� � path
	void getPath(SHADER_TYPE type, ID idShader, char *szPath);

	//! ���������� ���������������� ��� ������� � name
	void getName(SHADER_TYPE type, ID idShader, char *szName);

protected:

	Array<CShaderVS*> m_aVS;	//!< ������� vs ��������
	Array<CShaderPS*> m_aPS;	//!< ������� ps ��������

	bool m_canInfo4FailSend;

	void updateDataInfo4FailSend();

	//! ������ ���� ��������
	Array<CShaderInclude> m_aIncludes;

	String getTextResultLoad(int iResult);

	//! �������� ���������� ����, ���� �� ���������� �� �������
	void testDirCache();

	//! �������� � �������� ���� ��������
	void testIncludeCache(bool hasReport=true);

	//! ��������� ������� ��� ��������
	void loadCacheInclude();

	//! ��������� ������� ��� ��������
	void saveCacheInclude();

	//bool m_useCache;

	int m_iLastAllLoadVS;		//! ����� ���������� ����������� vs ��������, � �������� ����
	int m_iLastAllLoadPS;		//! ����� ���������� ����������� ps ��������, � �������� ����
};

#endif