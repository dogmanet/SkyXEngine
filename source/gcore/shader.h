
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SHADER_H
#define __SHADER_H

#define SHADERS_CACHE_PATH "cache/"

#include <gdefines.h>
#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <graphix/graphix.h>
#include <common/array.h>
#include <common/String.h>
#include <common/file_utils.h>
#include "sxgcore.h"

extern IGXContext *g_pDXDevice;

//! используется ли в данный момент кэш шейдеров?
extern bool g_useCache;

//! базовый класс шейдера
struct CShader
{
	CShader()
	{ 
		m_szName[0] = 0; 
		m_szPath[0] = 0; 
		m_iCountVar = 0;

		ZeroMemory(m_aMacros, sizeof(GXMACRO)* SXGC_SHADER_COUNT_MACRO);
	}

	~CShader()
	{
		
	}

	//! имя шейдера
	char m_szName[SXGC_SHADER_MAX_SIZE_NAME];

	//! имя файла шейдера
	char m_szPath[SXGC_SHADER_MAX_SIZE_DIR];

	//! где был загружен этот шейдер (впервые)
	//char m_szFrom[SXGC_SHADER_MAX_SIZE_FULLPATH];

	//! количество переменных
	//int m_iCountVar;

	//! массив макросов (данные последнего макроса должны быть NULL)
	GXMACRO m_aMacros[SXGC_SHADER_COUNT_MACRO];
};

//! вершинный шейдер
struct CShaderVS : public CShader
{
	CShaderVS(){ m_pVertexShader = 0; };
	~CShaderVS(){ mem_release(m_pVertexShader); };

	IGXVertexShader *m_pVertexShader;
};

//! пиксельный шейдер
struct CShaderPS : public CShader
{
	CShaderPS(){ m_pPixelShader = 0; };
	~CShaderPS(){ mem_release(m_pPixelShader); };

	IGXPixelShader *m_pPixelShader;
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
		ZeroMemory(m_aMacros, sizeof(GXMACRO)* SXGC_SHADER_COUNT_MACRO);
		int qwert = 0;
	}

	~CShaderFileCache()
	{
	}

	//! время последнего изменения оригинального шейдера
	uint32_t m_uiDate;
};

//! представление инлюда шейдеров
struct CShaderInclude
{
	CShaderInclude()
	{
		m_szFile[0] = 0;
		m_uiDate = 0;
	}

	//! имя файла
	char m_szFile[SXGC_SHADER_MAX_SIZE_DIRNAME];

	//! время последнего изменения
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

#define LOAD_SHADER_FAIL 0
#define LOAD_SHADER_COMPLETE 1
#define LOAD_SHADER_CACHE 2

//! загрузка вершинного шейдера
int LoadVertexShader(
	const char *szPath,		//!< абсолютный путь до файла шейдера
	CShaderVS *pShader,		//!< инициализированная структура CShaderVS
	GXMACRO *aMacro = 0	//!< массив дефайнов
	);

//загрузка пиксельного шейдера
int LoadPixelShader(
	const char *szPath,		//!< абсолютный путь до файла шейдера
	CShaderPS *pShader,		//!< инициализированная структура CShaderPS
	GXMACRO *aMacro = 0	//!< массив дефайнов
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

	//! добавление шейдера в очередь
	ID preLoad(SHADER_TYPE type, const char *szPath, const char *szName, SHADER_CHECKDOUBLE check_double, GXMACRO *aMacros = 0);

	//! загрузка всех шейдеров
	void allLoad();

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

	//! валидное ли имя у шейдера (расширение должно совпадать с типом)
	bool isValidateTypeName(SHADER_TYPE type, const char *szName);


	//! записывает имя шейдер с расширением в path
	void getPath(SHADER_TYPE type, ID idShader, char *szPath);

	//! записывает пользовательское имя шейдера в name
	void getName(SHADER_TYPE type, ID idShader, char *szName);

protected:

	Array<CShaderVS*> m_aVS;	//!< массивы vs шейдеров
	Array<CShaderPS*> m_aPS;	//!< массивы ps шейдеров

	//! массив всех инклюдов
	Array<CShaderInclude> m_aIncludes;

	String getTextResultLoad(int iResult);

	//! проверка директории кэша, если не существует то создаст
	void testDirCache();

	//! загрузка и проверка кэша инклюдов
	void testIncludeCache(bool hasReport=true);

	//! загрузить текущий кэш инклюдов
	void loadCacheInclude();

	//! сохранить текущий кэш инклюдов
	void saveCacheInclude();

	//bool m_useCache;

	int m_iLastAllLoadVS;		//! общее количество загруженных vs шейдеров, с прошлого раза
	int m_iLastAllLoadPS;		//! общее количество загруженных ps шейдеров, с прошлого раза
};

#endif