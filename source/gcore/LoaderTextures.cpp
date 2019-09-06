
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LoaderTextures.h"

CLoaderTextures::CLoaderTextures()
{
}

CLoaderTextures::~CLoaderTextures()
{
	for(int i = 0; i < m_aTextures.size(); i++)
	{
		mem_delete(m_aTextures[i]);
	}
	m_aTextures.clear();

	for(int i = 0; i < m_aPathes.size(); i++)
	{
		mem_delete(m_aPathes[i]);
	}
	m_aPathes.clear();
}

void CLoaderTextures::clearLoaded()
{
	int tmpcountdel = 0;
	for(int i = 0; i < m_aPathes.size(); ++i)
	{
		for(int k = 0; k < m_aPathes[i]->m_aTextures.size(); ++k)
		{
			if(m_aPathes[i]->m_aTextures[k]->m_type == LOAD_TEXTURE_TYPE_LOAD)
			{
				mem_release_del(m_aTextures[m_aPathes[i]->m_aTextures[k]->m_id]->m_pTex2d);
				mem_release_del(m_aTextures[m_aPathes[i]->m_aTextures[k]->m_id]->m_pTexCube);
				++tmpcountdel;
				if(CurrFirstFree > m_aPathes[i]->m_aTextures[k]->m_id)
					CurrFirstFree = m_aPathes[i]->m_aTextures[k]->m_id;

				mem_delete(m_aPathes[i]->m_aTextures[k]);
				m_aPathes[i]->m_aTextures.erase(k);
				--k;
			}
		}
	}

	int qwert = 0;
}

void CLoaderTextures::deleteTexture(ID id)
{
	if(!(id < m_aTextures.size() && id >= 0))
		return;

	if(m_aTextures[id]->m_pTex2d == 0 || m_aTextures[id]->m_pTexCube == 0)
		return;

	mem_release_del(m_aTextures[id]->m_pTex2d);
	mem_release_del(m_aTextures[id]->m_pTexCube);

	if(CurrFirstFree > id)
		CurrFirstFree = id;

	CPath* tmpp = m_aPathes[m_aTextures[id]->m_idDir];

	for(int i = 0; i < tmpp->m_aTextures.size(); ++i)
	{
		if(tmpp->m_aTextures[i] && tmpp->m_aTextures[i]->m_id == id)
		{
			mem_delete(tmpp->m_aTextures[i]);
			tmpp->m_aTextures.erase(i);
			break;
		}
	}
}

ID CLoaderTextures::addName(const char* name, LOAD_TEXTURE_TYPE type, ID* iddir, ID* idname)
{
	char tmp_path[SXGC_LOADTEX_MAX_SIZE_DIR];//папка
	char tmp_name[SXGC_LOADTEX_MAX_SIZE_NAME];//само имя текстыр с расширением
	ID id = -1;
	bool IsTruePath = false;
	//обрезаем имя текстуры и папку
	for(int i = 0; i < strlen(name); i++)
	{
		if(name[i] == '_')
		{
			memcpy(tmp_path, name, sizeof(char)*i);
			tmp_path[i] = 0;
			sprintf(tmp_name, "%s", name + i + 1);
			IsTruePath = true;
			break;
		}
	}

	if(!IsTruePath || strstr(name, ":") || strstr(name, "/") || strstr(name, "\\"))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - wrong texture name [%s]!!!", GEN_MSG_LOCATION, name);
	}

	int tmpkey = -1;	//переменная в которой храним ключ от массива в который записываем

	for(int i = 0; i < m_aPathes.size(); i++)
	{
		if(strcmp(m_aPathes[i]->m_sPath.c_str(), tmp_path) == 0)
		{
			tmpkey = i;
			break;
		}
	}

	//если мы не нашли совпадений значит путь уникален ...
	if(tmpkey == -1)
	{
		tmpkey = m_aPathes.size();
		m_aPathes.push_back(new CPath());
		m_aPathes[tmpkey]->m_sPath = tmp_path;
	}

	bool isunic = true;

	//првоеряем записано ли уже имя текстуры
	for(int i = 0; i < m_aPathes[tmpkey]->m_aTextures.size(); i++)
	{
		if(strcmp(m_aPathes[tmpkey]->m_aTextures[i]->m_sName.c_str(), tmp_name) == 0)
		{
			isunic = false;
			id = m_aPathes[tmpkey]->m_aTextures[i]->m_id;
			if(idname)
				*idname = i;
			break;
		}
	}

	if(iddir)
		*iddir = tmpkey;

	//если уникальное то записываем
	if(isunic)
	{
		if(CurrFirstFree == m_aTextures.size())
		{
			m_aTextures.push_back(new CTexture());
			id = m_aTextures.size() - 1;
			CurrFirstFree = id + 1;
		}
		else
		{
			if(CurrFirstFree < 0 || m_aTextures[CurrFirstFree] != 0)
			{
				if(CurrFirstFree < 0)
					CurrFirstFree = 0;

				for(int i = CurrFirstFree; i < m_aTextures.size(); ++i)
				{
					if(m_aTextures[i] == 0)
					{
						CurrFirstFree = i;
						break;
					}
				}
			}

			id = CurrFirstFree;
			++CurrFirstFree;
		}

		//если тип текстуры самоопределение
		if(type == LOAD_TEXTURE_TYPE_SELF)
			type = LOAD_TEXTURE_TYPE_LOAD;

		m_aPathes[tmpkey]->m_aTextures.push_back(new CPath::CTex(id, tmp_name, type));

		if(idname)
			*idname = m_aPathes[tmpkey]->m_aTextures.size() - 1;

		m_aTextures[id]->m_sName = name;
		m_aTextures[id]->m_idDir = tmpkey;

		if(type != LOAD_TEXTURE_TYPE_CUSTOM)
		{
			m_aQueueToLoadIDs.push(id);
		}
	}

	return id;
}

ID CLoaderTextures::getID(const char* name)
{
	char tmp_path[SXGC_LOADTEX_MAX_SIZE_DIR];//папка
	char tmp_name[SXGC_LOADTEX_MAX_SIZE_NAME];//само имя текстуры с расширением
	ID id = -1;
	//обрезаем имя текстуры и папку
	for(int i = 0; i<strlen(name); i++)
	{
		if(name[i] == '_')
		{
			memcpy(tmp_path, name, sizeof(char)*i);
			tmp_path[i] = 0;
			sprintf(tmp_name, "%s", name + i + 1);
			break;
		}
	}

	for(int i = 0; i<m_aPathes.size(); i++)
	{
		if(strcmp(m_aPathes[i]->m_sPath.c_str(), tmp_path) == 0)
		{
			for(int k = 0; k<m_aPathes[i]->m_aTextures.size(); k++)
			{
				if(strcmp(m_aPathes[i]->m_aTextures[k]->m_sName.c_str(), tmp_name) == 0)
				{
					id = m_aPathes[i]->m_aTextures[k]->m_id;
					i = m_aPathes.size();
					break;
				}
			}
		}
	}
	return id;
}

void CLoaderTextures::getName(ID id, char* name)
{
	if(id < m_aTextures.size())
	{
		String tmpname = m_aTextures[id]->m_sName;
		sprintf(name, "%s", tmpname.c_str());
	}
}

ID CLoaderTextures::create(const char* name, IGXTexture2D* tex)
{
	ID tmpkey, tmpKeyName;
	ID id = addName(name, LOAD_TEXTURE_TYPE_CUSTOM, &tmpkey, &tmpKeyName);
	m_aTextures[id]->m_pTex2d = tex;
	return id;
}

void CLoaderTextures::update(ID id)
{
	if(id < m_aTextures.size())
	{
		update(m_aTextures[id]->m_sName.c_str(), LOAD_TEXTURE_TYPE_SELF);
	}
}

ID CLoaderTextures::update(const char* name, LOAD_TEXTURE_TYPE type)
{
	ID tmpkey, tmpKeyName;
	ID id = addName(name, type, &tmpkey, &tmpKeyName);

	//LibReport(0,"update texture [%s] ...",name);

	char tmpPath[SXGC_LOADTEX_MAX_SIZE_FULLPATH];
	char tmpPathFS[MAX_PATH + SXGC_LOADTEX_MAX_SIZE_FULLPATH];
	sprintf(tmpPath, "textures/%s/%s_%s", m_aPathes[tmpkey]->m_sPath.c_str(), m_aPathes[tmpkey]->m_sPath.c_str(), m_aPathes[tmpkey]->m_aTextures[tmpKeyName]->m_sName.c_str());

	if(!Core_GetIXCore()->getFileSystem()->resolvePath(tmpPath, tmpPathFS, sizeof(tmpPathFS)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "  file not found [%s]\n", tmpPath);
	}
	else
	{
		GXTEXTURE_TYPE texType = g_pDevice->getTextureTypeFromFile(tmpPathFS);

		if(texType == GXTEXTURE_TYPE_2D)
		{
			IGXTexture2D *pTex = loadTexture2d(tmpPathFS);

			if(pTex == 0)
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not found texture [%s]", GEN_MSG_LOCATION, tmpPath);
			else
				m_aTextures[m_aPathes[tmpkey]->m_aTextures[tmpKeyName]->m_id]->m_pTex2d = pTex;
		}
		else if(texType == GXTEXTURE_TYPE_CUBE)
		{
			IGXTextureCube *pTex = loadTextureCube(tmpPathFS);

			if(pTex == 0)
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not found texture [%s]", GEN_MSG_LOCATION, tmpPath);
			else
				m_aTextures[m_aPathes[tmpkey]->m_aTextures[tmpKeyName]->m_id]->m_pTexCube = pTex;
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "  file not a texture [%s]\n", tmpPath);
		}
	}

	return id;
}

void CLoaderTextures::loadTextures()
{
	UINT uSize = m_aQueueToLoadIDs.size();
	if(uSize <= 0)
		return;
	//LibReport(REPORT_MSG_LEVEL_NOTICE, "loading textures ...\n");

	char tmpPath[SXGC_LOADTEX_MAX_SIZE_FULLPATH];
	char tmpPathFS[MAX_PATH + SXGC_LOADTEX_MAX_SIZE_FULLPATH];
	ID tmpiddir;
	int iCountLoaded = 0;
	for(UINT i = 0, l = min(uSize, 2); i < l; ++i)
	{
		ID idTexture = m_aQueueToLoadIDs.pop();
		CTexture *pTexture = m_aTextures[idTexture];

		tmpiddir = pTexture->m_idDir;
		CPath* tmpdir = m_aPathes[tmpiddir];
		sprintf(tmpPath, "textures/%s/%s", m_aPathes[tmpiddir]->m_sPath.c_str(), pTexture->m_sName.c_str());

		if(!Core_GetIXCore()->getFileSystem()->resolvePath(tmpPath, tmpPathFS, sizeof(tmpPathFS)))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "  file not found [%s]\n", pTexture->m_sName.c_str());
			continue;
		}
		GXTEXTURE_TYPE texType = g_pDevice->getTextureTypeFromFile(tmpPathFS);

		if(texType == GXTEXTURE_TYPE_2D)
		{
			IGXTexture2D *pTex = loadTexture2d(tmpPathFS);

			if(pTex == 0)
			{
				pTexture->m_pTex2d = 0;
				LibReport(REPORT_MSG_LEVEL_WARNING, "  not found [" COLOR_LCYAN "%s" COLOR_RESET "]\n", pTexture->m_sName.c_str());
			}
			else
			{
				pTexture->m_pTex2d = pTex;
				LibReport(REPORT_MSG_LEVEL_NOTICE, "  texture id [" COLOR_LCYAN "%d" COLOR_RESET "], [" COLOR_LCYAN "%s" COLOR_RESET "]\n", idTexture, pTexture->m_sName.c_str());
				++iCountLoaded;
			}
		}
		else if(texType == GXTEXTURE_TYPE_CUBE)
		{
			IGXTextureCube *pTex = loadTextureCube(tmpPathFS);

			if(pTex == 0)
			{
				pTexture->m_pTex2d = 0;
				LibReport(REPORT_MSG_LEVEL_WARNING, "  not found [" COLOR_LCYAN "%s" COLOR_RESET "]\n", pTexture->m_sName.c_str());
			}
			else
			{
				pTexture->m_pTexCube = pTex;
				LibReport(REPORT_MSG_LEVEL_NOTICE, "  texture id [" COLOR_LCYAN "%d" COLOR_RESET "], [" COLOR_LCYAN "%s" COLOR_RESET "]\n", idTexture, pTexture->m_sName.c_str());
				++iCountLoaded;
			}
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "  file not a texture [" COLOR_LCYAN "%s" COLOR_RESET "]\n", pTexture->m_sName.c_str());
		}
	}

	//LibReport(REPORT_MSG_LEVEL_NOTICE, "all textures loaded [" COLOR_LCYAN "%d" COLOR_RESET "]\n", m_aTextures.size());
}

IGXTexture2D* CLoaderTextures::getTexture2d(ID id)
{
	if(id < m_aTextures.size())
	{
		return m_aTextures[id]->m_pTex2d;
	}
	
	return 0;
}

void CLoaderTextures::setTexture2d(ID id, IGXTexture2D *pTexture)
{
	if(id < m_aTextures.size())
	{
		mem_release(m_aTextures[id]->m_pTex2d);
		m_aTextures[id]->m_pTex2d = pTexture;
	}
}

IGXTextureCube* CLoaderTextures::getTextureCube(ID id)
{
	if(id < m_aTextures.size())
	{
		return m_aTextures[id]->m_pTexCube;
	}
	else
		return 0;
}

//##########################################################################

IGXTexture2D* CLoaderTextures::loadTexture2d(const char *szPath)
{
	//@TODO: use SXGC_LOADTEX_COUNT_MIPMAP
	IGXTexture2D *pTex = g_pDevice->createTexture2DFromFile(szPath, GX_TEXFLAG_DEFAULT);

	return pTex;
}

IGXTextureCube* CLoaderTextures::loadTextureCube(const char *szPath)
{
	//@TODO: use SXGC_LOADTEX_COUNT_MIPMAP
	IGXTextureCube *pTex = g_pDevice->createTextureCubeFromFile(szPath, GX_TEXFLAG_DEFAULT);

	return pTex;
}
