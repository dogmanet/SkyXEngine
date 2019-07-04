
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*
добавить дефайн ~0 для сравнения
добавить переделывание наборов шейдеров в случае перезагрузки шейдеров
*/

#ifndef __SHADERS_H
#define __SHADERS_H

#define SHADERS_CACHE_PATH "cache/"

#include <gdefines.h>
#include <stdio.h>
#include <time.h>
//#include <direct.h>
#include <graphix/graphix.h>
#include <common/array.h>
#include <common/String.h>
#include <common/file_utils.h>
#include "sxgcore.h"

extern IGXContext *g_pDevice;

//! используется ли в данный момент кэш шейдеров?
extern bool g_useCache;

//! базовый класс шейдера
struct CShader
{
	CShader()
	{ 
		m_szName[0] = 0; 
		m_szPath[0] = 0; 

		ZeroMemory(m_aMacros, sizeof(GXMacro)* SXGC_SHADER_COUNT_MACRO);
	}

	~CShader()
	{
		mem_delete(m_pCode);
	}

	//! имя шейдера
	char m_szName[SXGC_SHADER_MAX_SIZE_NAME];

	//! имя файла шейдера
	char m_szPath[SXGC_SHADER_MAX_SIZE_DIR];

	//! массив макросов (данные последнего макроса должны быть NULL)
	GXMacro m_aMacros[SXGC_SHADER_COUNT_MACRO];

	//! буфер с бинарным кодом шейдера
	BYTE *m_pCode = NULL;
	UINT m_uiCodeSize;
};

template<class T>
struct CShaderImpl: public CShader
{
	CShaderImpl<T>()
	{
		m_pGXShader = 0;
	};
	CShaderImpl<T>(const CShaderImpl<T> &pOther) :
		m_pGXShader(pOther.m_pGXShader)
	{
		if(m_pGXShader)
		{
			m_pGXShader->AddRef();
		}
	};
	~CShaderImpl<T>()
	{
		mem_release(m_pGXShader);
	};
	CShaderImpl<T> &operator=(const CShaderImpl<T> &pOther)
	{
		m_pGXShader = pOther.m_pGXShader;
		if(m_pGXShader)
		{
			m_pGXShader->AddRef();
		}
	}

	T *m_pGXShader;
};

//! вершинный шейдер
typedef CShaderImpl<IGXVertexShader> CShaderVS;

//! пиксельный шейдер
typedef CShaderImpl<IGXPixelShader> CShaderPS;

//! геометрический шейдер
typedef CShaderImpl<IGXGeometryShader> CShaderGS;

//! вычислительный шейдер
typedef CShaderImpl<IGXComputeShader> CShaderCS;



struct CShaderKit
{
	CShaderKit(){ m_idVertexShader = m_idPixelShader = -1; m_pShaderKit = 0; }
	CShaderKit(ID idVertexShader, ID idPixelShader, IGXShaderSet *pShaderKit)
	{
		m_idVertexShader = idVertexShader;
		m_idPixelShader = idPixelShader; 
		m_pShaderKit = pShaderKit;
	}
	CShaderKit(const CShaderKit &pOther):
		m_idVertexShader(pOther.m_idVertexShader),
		m_idPixelShader(pOther.m_idPixelShader),
		m_idGeometryShader(pOther.m_idGeometryShader),
		m_idComputeShader(pOther.m_idComputeShader),
		m_pShaderKit(pOther.m_pShaderKit)
	{
		if(m_pShaderKit)
		{
			m_pShaderKit->AddRef();
		}
	}
	~CShaderKit()
	{
		mem_release(m_pShaderKit);
	}
	
	CShaderKit &operator=(const CShaderKit &pOther)
	{
		m_idVertexShader = pOther.m_idVertexShader;
		m_idPixelShader = pOther.m_idPixelShader;
		m_idGeometryShader = pOther.m_idGeometryShader;
		m_idComputeShader = pOther.m_idComputeShader;
		m_pShaderKit = pOther.m_pShaderKit;
		if(m_pShaderKit)
		{
			m_pShaderKit->AddRef();
		}
		return(*this);
	}

	ID m_idVertexShader = -1;
	ID m_idPixelShader = -1;
	ID m_idGeometryShader = -1;
	ID m_idComputeShader = -1;
	IGXShaderSet *m_pShaderKit;
};

//! кэшированный шейдер
struct CShaderFileCache : public CShader
{
	CShaderFileCache()
	{
		m_szName[0] = 0;
		m_szPath[0] = 0; 
		m_uiDate = 0; 
		m_pCode = 0; 
		ZeroMemory(m_aMacros, sizeof(GXMacro)* SXGC_SHADER_COUNT_MACRO);
		int qwert = 0;
	}

	~CShaderFileCache()
	{
		mem_delete(m_pCode);
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
	GXMacro *aMacro = 0	//!< массив дефайнов
	);

//загрузка пиксельного шейдера
int LoadPixelShader(
	const char *szPath,		//!< абсолютный путь до файла шейдера
	CShaderPS *pShader,		//!< инициализированная структура CShaderPS
	GXMacro *aMacro = 0	//!< массив дефайнов
	);

//загрузка геометрического шейдера
int LoadGeometryShader(
	const char *szPath,		//!< абсолютный путь до файла шейдера
	CShaderGS *pShader,		//!< инициализированная структура CShaderGS
	GXMacro *aMacro = 0	//!< массив дефайнов
	);

//загрузка вычислительного шейдера
int LoadComputeShader(
	const char *szPath,		//!< абсолютный путь до файла шейдера
	CShaderCS *pShader,		//!< инициализированная структура CShaderCS
	GXMacro *aMacro = 0	//!< массив дефайнов
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
	ID preLoad(SHADER_TYPE type, const char *szPath, const char *szName, GXMacro *aMacros = 0);

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

	ID createKit(ID idVertexShader, ID idPixelShader, ID idGeometryShader, ID idComputeShader);

	//! бинд шейдеров по id
	void bind(ID idShaderKit);

	//! обнуление биндов шейдеров
	void unbind();


	//! существует ли шейдер с именем файла и расширением name, если да то возвращает id
	//@DEPRECATED: 
	ID existsPath(SHADER_TYPE type, const char *szPath);

	//! существует ли шейдер с именем файла и набором макросов, если да то возвращает id
	ID existsPathMacro(SHADER_TYPE type, const char *szPath, GXMacro *aMacros);

	//! существует ли шейдер с пользовательским именем name, если да то возвращает id
	//@DEPRECATED: 
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
	Array<CShaderGS*> m_aGS;	//!< массивы gs шейдеров
	Array<CShaderCS*> m_aCS;	//!< массивы cs шейдеров

	Array<CShaderKit*> m_aShaderKit;
	
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
	int m_iLastAllLoadGS;		//! общее количество загруженных gs шейдеров, с прошлого раза
	int m_iLastAllLoadCS;		//! общее количество загруженных gs шейдеров, с прошлого раза
};

#endif
