
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __LOADERTEXTURES_H
#define __LOADERTEXTURES_H

#include <gdefines.h>
#include <d3d9.h>
#include <common/array.h>
#include <common/String.h>
#include <common/file_utils.h>
#include "sxgcore.h"

void LibReport(int iLevel, const char *szFormat, ...);
extern IDirect3DDevice9 *g_pDXDevice;
extern D3DPRESENT_PARAMETERS g_oD3DAPP;

//! загрузчик текстур
class СLoaderTextures
{
public:
	СLoaderTextures();
	~СLoaderTextures();

	//! существует ли текстура на диске
	bool fileExists(const char *szName);

	//! удалить все загруженные текстуры
	void clearLoaded();

	//! удалить текстуру id
	void deleteTexture(ID id);	

	//! добавляем имя текстуры, взамен получаем на нее ID (поставить в очередь)
	ID addName(const char *szName, LOAD_TEXTURE_TYPE type, ID *pIDdir = 0, ID *pIDname = 0);

	//! получить id по имени
	ID getID(const char *szName);

	//получить имя по id
	void getName(ID id, char *szName);

	//! создать место для текстуры tex
	ID create(const char *szName, IDirect3DTexture9 *pTex);

	//! перезагрузить текстуру name
	ID update(const char *szName, LOAD_TEXTURE_TYPE type);

	//! перезагрузить текстуру по id
	void update(ID id);

	//! загрузка всех текстур поставленных в очередь
	void loadTextures();	

	//! получить текстуру по id
	IDirect3DTexture9* getTexture2d(ID id);

	//! получить текстуру по id
	IDirect3DCubeTexture9* getTextureCube(ID id);

	//! загрузить все текстуры из директории szDir (относительно директории текстур) и присвоить им статус константные
	bool addConstAllInDir(const char *szDir);

private:

	//! структура описывающая папку и все текстуры в ней, у каждой свой id для доступа
	struct CPath
	{
		CPath(){}
		~CPath()
		{
			for (int i = 0; i < m_aTextures.size(); ++i)
			{
				mem_delete(m_aTextures[i]);
			}
			m_aTextures.clear();
		}

		//! имя папки
		String m_sPath;	

		//! текущее представление текстуры
		struct CTex
		{
			CTex(){ m_id = -1; m_type = LOAD_TEXTURE_TYPE_LOAD; }
			CTex(ID id, const char *szNname, LOAD_TEXTURE_TYPE type){ m_id = id; m_sName = szNname; m_type = type; }
			
			//! глобальный идентификатор
			ID m_id;

			//! имя текстуры
			String m_sName;

			//! тип #LOAD_TEXTURE_TYPE
			LOAD_TEXTURE_TYPE m_type;
		};
		
		Array<CTex*> m_aTextures;
	};

	//! представление текстуры
	struct CTexture
	{
		CTexture(){ m_pTex2d = 0; m_pTexCube = 0; m_idDir = -1; }
		~CTexture(){ mem_release(m_pTex2d); mem_release(m_pTexCube); }

		//! имя
		String m_sName;

		//! 2д текстура
		IDirect3DTexture9 *m_pTex2d;

		//! кубическая текстура
		IDirect3DCubeTexture9 *m_pTexCube;

		//! глобальный id директории
		ID m_idDir;
	};

	//**********************************************************************

	//! массив путей с текстурами
	Array<CPath*> m_aPathes;

	int CurrFirstFree;

	//! массив текстур, ключ == идентификатор
	Array<CTexture*> m_aTextures;

	//! массив id текстур на загрузку
	Array<ID> m_aQueueToLoadIDs;

	//**********************************************************************

	//! загрузка 2d текстуры
	IDirect3DTexture9* loadTexture2d(const char *szPath);

	//! загрузка кубической текстуры
	IDirect3DCubeTexture9* loadTextureCube(const char *szPath);

	//! возвращает тип файла текстуры
	D3DRESOURCETYPE getTypeFileTex(const char *szPath);
};

#endif