
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SHADERS_H
#define __SHADERS_H

#include <gdefines.h>
#include <stdio.h>
#include <time.h>

#include <graphix/graphix.h>
#include <common/array.h>
#include <common/String.h>
#include <common/string_utils.h>
#include "sxgcore.h"

extern IGXDevice *g_pDevice;

//! базовый класс шейдера
struct CShader
{
	CShader()
	{ 
		m_szPath[0] = 0; 

		ZeroMemory(m_aMacros, sizeof(GXMacro)* SXGC_SHADER_COUNT_MACRO);
	}

	//! имя файла шейдера
	char m_szPath[SXGC_SHADER_MAX_SIZE_DIR];

	//! массив макросов (данные последнего макроса должны быть NULL)
	GXMacro m_aMacros[SXGC_SHADER_COUNT_MACRO];
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
	CShaderKit() = default;
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
	IGXShaderSet *m_pShaderKit = NULL;
};


//**************************************************************************

#define LOAD_SHADER_FAIL 0
#define LOAD_SHADER_COMPLETE 1
#define LOAD_SHADER_CACHE 2

//**************************************************************************

class CShaderPreprocessor;
//! менеджер шейдеров
class CShaderManager
{
public:
	CShaderManager();
	~CShaderManager();

	//! существует ли файл name в папке с шейдерами
	bool existsFile(const char *szPath);

	//! добавление шейдера в очередь
	ID preLoad(SHADER_TYPE type, const char *szPath, GXMacro *aMacros = 0);

	//! загрузка всех шейдеров
	void allLoad(bool bReload = false);

	//! перезагрузить все шейдеры, с учетом макросов
	void reloadAll();

	ID createKit(ID idVertexShader, ID idPixelShader, ID idGeometryShader, ID idComputeShader);

	//! бинд шейдеров по id
	void bind(ID idShaderKit);

	//! обнуление биндов шейдеров
	void unbind();

	//! существует ли шейдер с именем файла и набором макросов, если да то возвращает id
	ID existsPathMacro(SHADER_TYPE type, const char *szPath, GXMacro *aMacros);

	//! загружен ли шейдер с данным id
	bool isLoaded(SHADER_TYPE type, ID id);
	
	//! записывает имя шейдер с расширением в path
	void getPath(SHADER_TYPE type, ID idShader, char *szPath);

protected:
	const char* getTextResultLoad(int iResult);

	Array<CShaderVS*> m_aVS;	//!< массивы vs шейдеров
	Array<CShaderPS*> m_aPS;	//!< массивы ps шейдеров
	Array<CShaderGS*> m_aGS;	//!< массивы gs шейдеров
	Array<CShaderCS*> m_aCS;	//!< массивы cs шейдеров

	Array<CShaderKit*> m_aShaderKit;
	
	mutex m_mxLock;

	int m_iLastAllLoadVS = 0;		//! общее количество загруженных vs шейдеров, с прошлого раза
	int m_iLastAllLoadPS = 0;		//! общее количество загруженных ps шейдеров, с прошлого раза
	int m_iLastAllLoadGS = 0;		//! общее количество загруженных gs шейдеров, с прошлого раза
	int m_iLastAllLoadCS = 0;		//! общее количество загруженных gs шейдеров, с прошлого раза

	CShaderPreprocessor *m_pPreprocessor = NULL;
};

#endif
