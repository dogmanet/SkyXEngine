
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __LOADERTEXTURES_H
#define __LOADERTEXTURES_H

#include <gdefines.h>
#include <graphix/graphix.h>
#include <common/array.h>
#include <common/String.h>
#include "sxgcore.h"
#include <common/ConcurrentQueue.h>

void LibReport(int iLevel, const char *szFormat, ...);
extern IGXContext *g_pDevice;

//! загрузчик текстур
class CLoaderTextures
{
public:
	CLoaderTextures();
	~CLoaderTextures();

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
	ID create(const char *szName, IGXTexture2D *pTex);

	//! перезагрузить текстуру name
	ID update(const char *szName, LOAD_TEXTURE_TYPE type);

	//! перезагрузить текстуру по id
	void update(ID id);

	//! загрузка всех текстур поставленных в очередь
	void loadTextures();	

	//! получить текстуру по id
	IGXTexture2D* getTexture2d(ID id);
	void setTexture2d(ID id, IGXTexture2D *pTexture);

	//! получить текстуру по id
	IGXTextureCube* getTextureCube(ID id);

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
		IGXTexture2D *m_pTex2d;

		//! кубическая текстура
		IGXTextureCube *m_pTexCube;

		//! глобальный id директории
		ID m_idDir;
	};

	//**********************************************************************

	//! массив путей с текстурами
	Array<CPath*> m_aPathes;

	int CurrFirstFree = 0;

	//! массив текстур, ключ == идентификатор
	Array<CTexture*> m_aTextures;

	//! массив id текстур на загрузку
	CConcurrentQueue<ID> m_aQueueToLoadIDs;

	//**********************************************************************

	//! загрузка 2d текстуры
	IGXTexture2D* loadTexture2d(const char *szPath);

	//! загрузка кубической текстуры
	IGXTextureCube* loadTextureCube(const char *szPath);
};

#endif