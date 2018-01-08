
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
#include "sxgcore.h"

extern report_func g_fnReportf;
extern IDirect3DDevice9 *g_pDXDevice;
extern D3DPRESENT_PARAMETERS g_oD3DAPP;

//! базовый класс шейдера
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

	//! имя шейдера
	char m_szName[SXGC_SHADER_MAX_SIZE_NAME];

	//! имя файла шейдера
	char m_szPath[SXGC_SHADER_MAX_SIZE_DIR];

	//! количество переменных
	int m_iCountVar;

	//! описание переменных
	D3DXCONSTANT_DESC m_aVarDesc[SXGC_SHADER_VAR_MAX_COUNT];

	//! массив макросов (данные последнего макроса должны быть NULL)
	D3DXMACRO m_aMacros[SXGC_SHADER_COUNT_MACRO];

	//! буфер с бинарным кодом шейдера
	ID3DXBuffer *m_pCode;
};

//! вершинный шейдер
struct CShaderVS : public CShader
{
	CShaderVS(){ m_pVertexShader = 0; };
	~CShaderVS(){ mem_release(m_pVertexShader); };

	IDirect3DVertexShader9 *m_pVertexShader;
};

//! пиксельный шейдер
struct CShaderPS : public CShader
{
	CShaderPS(){ m_pPixelShader = 0; };
	~CShaderPS(){ mem_release(m_pPixelShader); };

	IDirect3DPixelShader9 *m_pPixelShader;
};

//! кэшированный шейдер
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

	//! время последнего изменения оригинального шейдера
	uint32_t m_uiDate;
};

//**************************************************************************

//! создать кэш шейдера
CShaderFileCache* CreateShaderFileCacheFormShader(CShader *pShader);

//! сохранить кэшированный шейдер
void SaveShaderFileCache(CShaderFileCache *pShaderFileCache);

//! создать кэшированный шейдер из файла
CShaderFileCache* CreateShaderFileCacheFormFile(const char *szPath);

//! возвращает время последнего изменения оригинального шейдера на основании данных в его кэше
uint32_t GetTimeShaderFileCache(const char *szPath);

//**************************************************************************

//! загрузка вершинного шейдера
void LoadVertexShader(
	const char *szPath,		//!< абсолютный путь до файла шейдера
	CShaderVS *pShader,		//!< инициализированная структура CShaderVS
	D3DXMACRO *aMacro = 0	//!< массив дефайнов
	);

//загрузка пиксельного шейдера
void LoadPixelShader(
	const char *szPath,		//!< абсолютный путь до файла шейдера
	CShaderPS *pShader,		//!< инициализированная структура CShaderPS
	D3DXMACRO *aMacro = 0	//!< массив дефайнов
	);

//**************************************************************************

//! менеджер шейдеров
class CShaderManager
{
public:
	CShaderManager();
	~CShaderManager();

	//! существует ли файл name в папке с шейдерами
	bool existsFile(const char *szPath);

	//! загрузка шейдера
	ID load(SHADER_TYPE type, const char *szPath, const char *szName, SHADER_CHECKDOUBLE check_double, D3DXMACRO *aMacros = 0);

	//! обновление шейдера по имени
	void update(SHADER_TYPE type, const char *szName);

	//! обновление шейдера по id
	void update(SHADER_TYPE type, ID id);

	//! перезагрузить все шейдеры, с учетом макросов
	void reloadAll();

	//! получить идентификатор шейдера по имени
	ID getID(SHADER_TYPE type, const char *szName);

	//! бинд шейдера по имени
	void bind(SHADER_TYPE type, const char *szName);

	//! бинд шейдера по id
	void bind(SHADER_TYPE type, ID idShader);

	//! обнуление биндов шейдеров
	void unbind();


	//! передача float значений в шейдер по имени
	void setValueRegisterF(SHADER_TYPE type, const char *szNameShader, const char *szNameVar, void *pData, int iCountFloat4 = 0);

	//! передача float значений в шейдер по ID
	void setValueRegisterF(SHADER_TYPE type, ID idShader, const char *szNameVar, void *data, int iCountFloat4 = 0);

	//! передача int значений в шейдер по имени
	void setValueRegisterI(SHADER_TYPE type, const char *szNameShader, const char *szNameVar, void *pData, int iCountInt4 = 0);

	//! передача int значений в шейдер по ID
	void setValueRegisterI(SHADER_TYPE type, ID idShader, const char *szNameVar, void *data, int iCountInt4 = 0);

	//! передача bool значений в шейдер по имени
	void setValueRegisterB(SHADER_TYPE type, const char *szNameShader, const char *szNameVar, void *pData, int iCountBool4 = 0);

	//! передача bool значений в шейдер по ID
	void setValueRegisterB(SHADER_TYPE type, ID idShader, const char *szNameVar, void *data, int iCountBool4 = 0);


	//! существует ли шейдер с именем файла и расширением name, если да то возвращает id
	ID existsPath(SHADER_TYPE type, const char *szPath);

	//! существует ли шейдер с пользовательским именем name, если да то возвращает id
	ID existsName(SHADER_TYPE type, const char *szName);

	//! загружен ли шейдер с данным id
	bool isValidated(SHADER_TYPE type, ID id);

	bool isValidateTypeName(SHADER_TYPE type, const char *szName);


	//! записывает имя шейдер с расширением в path
	void getPath(SHADER_TYPE type, ID idShader, char *szPath);

	//! записывает пользовательское имя шейдера в name
	void getName(SHADER_TYPE type, ID idShader, char *szName);

protected:
	Array<CShaderVS*> m_aVS;
	Array<CShaderPS*> m_aPS;
};

#endif