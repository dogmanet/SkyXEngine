
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LoaderTextures.h"

СLoaderTextures::СLoaderTextures()
{
	CurrFirstFree = -1;
}

СLoaderTextures::~СLoaderTextures()
{
	for (int i = 0; i < m_aTextures.size(); i++)
	{
		mem_delete(m_aTextures[i]);
	}
	m_aTextures.clear();

	for (int i = 0; i < m_aPathes.size(); i++)
	{
		mem_delete(m_aPathes[i]);
	}
	m_aPathes.clear();
}

bool СLoaderTextures::fileExists(const char* name)
{
	char tmppath[SXGC_LOADTEX_MAX_SIZE_FULLPATH];
	char tmp_path[SXGC_LOADTEX_MAX_SIZE_DIR];
	char tmp_name[SXGC_LOADTEX_MAX_SIZE_NAME];
	
	bool IsTruePath = false;
	//обрезаем имя текстуры и папку
	for (int i = 0; i<strlen(name); i++)
	{
		if (name[i] == '_')
		{
			memcpy(tmp_path, name, sizeof(char)*i);
			tmp_path[i] = 0;
			sprintf(tmp_name, "%s", name + i + 1);
			IsTruePath = true;
			break;
		}
	}

	if (!IsTruePath)
	{
		//LibReport(REPORT_MSG_LEVEL_ERROR, "%s - wrong texture name [%s]!!!", GEN_MSG_LOCATION, name);
		return false;
	}

	sprintf(tmppath, "%s%s\\%s", Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), tmp_path, name);
	return FileExistsFile(tmppath);
}

void СLoaderTextures::clearLoaded()
{
	int tmpcountdel = 0;
	for (int i = 0; i < m_aPathes.size(); ++i)
	{
		for (int k = 0; k < m_aPathes[i]->m_aTextures.size(); ++k)
		{
			if (m_aPathes[i]->m_aTextures[k]->m_type == LOAD_TEXTURE_TYPE_LOAD)
			{
				mem_release_del(m_aTextures[m_aPathes[i]->m_aTextures[k]->m_id]->m_pTex2d);
				mem_release_del(m_aTextures[m_aPathes[i]->m_aTextures[k]->m_id]->m_pTexCube);
				++tmpcountdel;
				if (CurrFirstFree > m_aPathes[i]->m_aTextures[k]->m_id)
					CurrFirstFree = m_aPathes[i]->m_aTextures[k]->m_id;

				mem_delete(m_aPathes[i]->m_aTextures[k]);
				m_aPathes[i]->m_aTextures.erase(k);
				--k;
			}
		}
	}

	int qwert = 0;
}

void СLoaderTextures::deleteTexture(ID id)
{
	if (!(id < m_aTextures.size() && id >= 0))
		return;

	if (m_aTextures[id]->m_pTex2d == 0 || m_aTextures[id]->m_pTexCube == 0)
		return;

	mem_release_del(m_aTextures[id]->m_pTex2d);
	mem_release_del(m_aTextures[id]->m_pTexCube);

	if (CurrFirstFree > id)
		CurrFirstFree = id;

	CPath* tmpp = m_aPathes[m_aTextures[id]->m_idDir];

	for (int i = 0; i < tmpp->m_aTextures.size(); ++i)
	{
		if (tmpp->m_aTextures[i] && tmpp->m_aTextures[i]->m_id == id)
		{
			mem_delete(tmpp->m_aTextures[i]);
			tmpp->m_aTextures.erase(i);
			break;
		}
	}
}

ID СLoaderTextures::addName(const char* name, LOAD_TEXTURE_TYPE type, ID* iddir, ID* idname)
{
	char tmp_path[SXGC_LOADTEX_MAX_SIZE_DIR];//папка
	char tmp_name[SXGC_LOADTEX_MAX_SIZE_NAME];//само имя текстыр с расширением
	ID id = -1;
	bool IsTruePath = false;
		//обрезаем имя текстуры и папку
		for(int i=0;i<strlen(name);i++)
		{
				if(name[i] == '_')
				{
					memcpy(tmp_path,name,sizeof(char)*i);
					tmp_path[i] = 0;
					sprintf(tmp_name,"%s",name+i+1);
					IsTruePath = true;
					break;
				}
		}

		if(!IsTruePath)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - wrong texture name [%s]!!!", GEN_MSG_LOCATION, name);
		}

	int tmpkey = -1;	//переменная в которой храним ключ от массива в который записываем
	
	for (int i = 0; i<m_aPathes.size(); i++)
	{
		if (strcmp(m_aPathes[i]->m_sPath.c_str(), tmp_path) == 0)
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
		for(int i=0;i<m_aPathes[tmpkey]->m_aTextures.size();i++)
		{
				if(strcmp(m_aPathes[tmpkey]->m_aTextures[i]->m_sName.c_str(),tmp_name) == 0)
				{
					isunic = false;
					id = m_aPathes[tmpkey]->m_aTextures[i]->m_id;
					if (idname)
						*idname = i;
					break;
				}
		}

		if (iddir)
			*iddir = tmpkey;

		//если уникальное то записываем
		if(isunic)
		{
			if (CurrFirstFree + 1 == m_aTextures.size())
			{
				m_aTextures.push_back(new CTexture());
				CurrFirstFree = id = m_aTextures.size() - 1;
			}
			else
			{
				if (CurrFirstFree < 0 || m_aTextures[CurrFirstFree] != 0)
				{
					if (CurrFirstFree < 0)
						CurrFirstFree = 0;

					for (int i = CurrFirstFree; i < m_aTextures.size(); ++i)
					{
						if (m_aTextures[i] == 0)
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
			if (type == LOAD_TEXTURE_TYPE_SELF)
				type = LOAD_TEXTURE_TYPE_LOAD;

			m_aPathes[tmpkey]->m_aTextures.push_back(new CPath::CTex(id, tmp_name, type));

			if (idname)
				*idname = m_aPathes[tmpkey]->m_aTextures.size() - 1;

			m_aTextures[id]->m_sName = name;
			m_aTextures[id]->m_idDir = tmpkey;
			
			if (type != LOAD_TEXTURE_TYPE_CUSTOM)
				m_aQueueToLoadIDs.push_back(id);
		}

	return id;
}

ID СLoaderTextures::getID(const char* name)
{
	char tmp_path[SXGC_LOADTEX_MAX_SIZE_DIR];//папка
	char tmp_name[SXGC_LOADTEX_MAX_SIZE_NAME];//само имя текстуры с расширением
	ID id = -1;
		//обрезаем имя текстуры и папку
		for(int i=0;i<strlen(name);i++)
		{
				if(name[i] == '_')
				{
					memcpy(tmp_path,name,sizeof(char)*i);
					tmp_path[i] = 0;
					sprintf(tmp_name,"%s",name+i+1);
					break;
				}
		}

		for (int i = 0; i<m_aPathes.size(); i++)
		{
				if(strcmp(m_aPathes[i]->m_sPath.c_str(),tmp_path) == 0)
				{
						for(int k=0;k<m_aPathes[i]->m_aTextures.size();k++)
						{
								if(strcmp(m_aPathes[i]->m_aTextures[k]->m_sName.c_str(),tmp_name) == 0)
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

void СLoaderTextures::getName(ID id, char* name)
{
		if(id < m_aTextures.size())
		{
			String tmpname = m_aTextures[id]->m_sName;
			sprintf(name,"%s",tmpname.c_str());
		}
}

ID СLoaderTextures::create(const char* name, IDirect3DTexture9* tex)
{
	ID tmpkey, tmpKeyName;
	ID id = addName(name, LOAD_TEXTURE_TYPE_CUSTOM, &tmpkey, &tmpKeyName);
	m_aTextures[id]->m_pTex2d = tex;
	return id;
}

void СLoaderTextures::update(ID id)
{
	if(id < m_aTextures.size())
	{
		update(m_aTextures[id]->m_sName.c_str(), LOAD_TEXTURE_TYPE_SELF);
	}
}

ID СLoaderTextures::update(const char* name, LOAD_TEXTURE_TYPE type)
{
	ID tmpkey, tmpKeyName;
	ID id = addName(name, type, &tmpkey, &tmpKeyName);

	//LibReport(0,"update texture [%s] ...",name);

	char tmpPath[SXGC_LOADTEX_MAX_SIZE_FULLPATH];
	sprintf(tmpPath, "%s%s%s%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), m_aPathes[tmpkey]->m_sPath.c_str(), "\\", m_aPathes[tmpkey]->m_sPath.c_str(), "_", m_aPathes[tmpkey]->m_aTextures[tmpKeyName]->m_sName.c_str());

	if (getTypeFileTex(tmpPath) == D3DRTYPE_TEXTURE)
	{
		IDirect3DTexture9 *pTex = loadTexture2d(tmpPath);

		if (pTex == 0)
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not found texture [%s]", GEN_MSG_LOCATION, tmpPath);
		else
			m_aTextures[m_aPathes[tmpkey]->m_aTextures[tmpKeyName]->m_id]->m_pTex2d = pTex;
	}
	else if (getTypeFileTex(tmpPath) == D3DRTYPE_CUBETEXTURE)
	{
		IDirect3DCubeTexture9 *pTex = loadTextureCube(tmpPath);

		if (pTex == 0)
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not found texture [%s]", GEN_MSG_LOCATION, tmpPath);
		else
			m_aTextures[m_aPathes[tmpkey]->m_aTextures[tmpKeyName]->m_id]->m_pTexCube = pTex;
	}
	else
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "  file not a texture [%s]\n", tmpPath);
	}
	
	return id;
}

void СLoaderTextures::loadTextures()
{
	if (m_aQueueToLoadIDs.size() <= 0)
		return;
	LibReport(REPORT_MSG_LEVEL_NOTICE, "load textures ...\n");

	char tmpPath[SXGC_LOADTEX_MAX_SIZE_FULLPATH];
	ID tmpiddir;
	int iCountLoaded = 0;
	for (int i = 0; i<m_aQueueToLoadIDs.size(); i++)
	{
		tmpiddir = m_aTextures[m_aQueueToLoadIDs[i]]->m_idDir;
		CPath* tmpdir = m_aPathes[tmpiddir];
		sprintf(tmpPath, "%s%s%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), m_aPathes[tmpiddir]->m_sPath.c_str(), "\\", m_aTextures[m_aQueueToLoadIDs[i]]->m_sName.c_str());
		
		if (getTypeFileTex(tmpPath) == D3DRTYPE_TEXTURE)
		{
			IDirect3DTexture9 *pTex = loadTexture2d(tmpPath);

			if (pTex == 0)
			{
				m_aTextures[m_aQueueToLoadIDs[i]]->m_pTex2d = 0;
				LibReport(REPORT_MSG_LEVEL_WARNING, "  not found [%s]\n", m_aTextures[m_aQueueToLoadIDs[i]]->m_sName.c_str());
			}
			else
			{
				m_aTextures[m_aQueueToLoadIDs[i]]->m_pTex2d = pTex;
				LibReport(REPORT_MSG_LEVEL_NOTICE, "  texture id [%d], [%s]\n", m_aQueueToLoadIDs[i], m_aTextures[m_aQueueToLoadIDs[i]]->m_sName.c_str());
				++iCountLoaded;
			}
		}
		else if (getTypeFileTex(tmpPath) == D3DRTYPE_CUBETEXTURE)
		{
			IDirect3DCubeTexture9 *pTex = loadTextureCube(tmpPath);

			if (pTex == 0)
			{
				m_aTextures[m_aQueueToLoadIDs[i]]->m_pTex2d = 0;
				LibReport(REPORT_MSG_LEVEL_WARNING, "  not found [%s]\n", m_aTextures[m_aQueueToLoadIDs[i]]->m_sName.c_str());
			}
			else
			{
				m_aTextures[m_aQueueToLoadIDs[i]]->m_pTexCube = pTex;
				LibReport(REPORT_MSG_LEVEL_NOTICE, "  texture id [%d], [%s]\n", m_aQueueToLoadIDs[i], m_aTextures[m_aQueueToLoadIDs[i]]->m_sName.c_str());
				++iCountLoaded;
			}
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "  file not a texture [%s]\n", m_aTextures[m_aQueueToLoadIDs[i]]->m_sName.c_str());
		}
	}
	
	m_aQueueToLoadIDs.clear();
	LibReport(REPORT_MSG_LEVEL_NOTICE, "all loaded textures [%d]\n", m_aTextures.size());
}

IDirect3DTexture9* СLoaderTextures::getTexture2d(ID id)
{
		if(id < m_aTextures.size())
		{
			return m_aTextures[id]->m_pTex2d;
		}
		else 
			return 0;
}

IDirect3DCubeTexture9* СLoaderTextures::getTextureCube(ID id)
{
	if (id < m_aTextures.size())
	{
		return m_aTextures[id]->m_pTexCube;
	}
	else
		return 0;
}

bool СLoaderTextures::addConstAllInDir(const char *szDir)
{
	Array<String> aPathes = FileGetListFiles((String(Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES)) + szDir + "/*.dds").c_str());

	for (int i = 0, il = aPathes.size(); i < il; ++i)
	{
		addName(aPathes[i].c_str(), LOAD_TEXTURE_TYPE_CONST);
	}

	return (aPathes.size() > 0);
}

//##########################################################################

IDirect3DTexture9* СLoaderTextures::loadTexture2d(const char *szPath)
{
	IDirect3DTexture9 *pTex = 0;
	D3DXCreateTextureFromFileEx(
		g_pDXDevice,
		szPath,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		SXGC_LOADTEX_COUNT_MIPMAP,
		0,
		SXGC_LOADTEX_FORMAT_TEX,
		D3DPOOL_MANAGED,
		D3DX_FILTER_BOX,
		D3DX_FILTER_BOX, 0, 0, 0, &pTex
		);

	return pTex;
}

IDirect3DCubeTexture9* СLoaderTextures::loadTextureCube(const char *szPath)
{
	IDirect3DCubeTexture9 *pTex = 0;
	D3DXCreateCubeTextureFromFileEx(
		g_pDXDevice,
		szPath,
		D3DX_DEFAULT,
		SXGC_LOADTEX_COUNT_MIPMAP,
		0,
		SXGC_LOADTEX_FORMAT_TEX,
		D3DPOOL_MANAGED,
		D3DX_FILTER_BOX,
		D3DX_FILTER_BOX, 0, 0, 0, &pTex
		);

	return pTex;
}

D3DRESOURCETYPE СLoaderTextures::getTypeFileTex(const char *szPath)
{
	D3DXIMAGE_INFO oTexInfo;
	memset(&oTexInfo, 0, sizeof(D3DXIMAGE_INFO));

	D3DXGetImageInfoFromFile(szPath, &oTexInfo);
	return oTexInfo.ResourceType;
}