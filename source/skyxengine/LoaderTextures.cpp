
#pragma once

SkyXEngine::Core::Loaders::LoaderTextures::LoaderTextures()
{
	CountArr=0;
	CountIDs=0;
	CountIDsOld=0;
}

SkyXEngine::Core::Loaders::LoaderTextures::~LoaderTextures()
{
	ArrNames.clear();

		for(DWORD i=0;i<ArrTextures.size();i++)
			ndx_release(ArrTextures[i]);
	ArrTextures.clear();
}

DWORD SkyXEngine::Core::Loaders::LoaderTextures::AddName(const char* name)
{
	char tmp_path[256];//папка
	char tmp_name[256];//само имя текстыр с расширением
	DWORD id = -1;
	bool IsTruePath = false;
		//обрезаем имя текстуры и папку
		for(DWORD i=0;i<strlen(name);i++)
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
			InError("Невернео имя текстуры [%s]!!!",name);
		}

	DWORD tmpkey = -1;	//переменная в которой храним ключ от массива в который записываем
		for(DWORD i=0;i<CountArr;i++)
		{
				if(strcmp(Arr[i].Path,tmp_path) == 0)
				{
					tmpkey = i;
					break;
				}
		}

		//если мы не нашли совпадений значит путь уникален ...
		if(tmpkey == -1)
		{
			tmpkey = CountArr;
			CountArr++;
		}
	sprintf(Arr[tmpkey].Path,"%s",tmp_path);
	

	bool isunic = true;
	
		//првоеряем записано ли уже имя текстуры
		for(DWORD i=0;i<Arr[tmpkey].ArrNames.size();i++)
		{
				if(strcmp(Arr[tmpkey].ArrNames[i],tmp_name) == 0)
				{
					isunic = false;
					id = Arr[tmpkey].ArrID[i];
					break;
				}
		}

		//если уникальное то записываем
		if(isunic)
		{
			char* tmpunicname = new char[strlen(tmp_name)+1];
			sprintf(tmpunicname,"%s",tmp_name);
			Arr[tmpkey].ArrNames.push_back(tmpunicname);
			Arr[tmpkey].ArrID.push_back(CountIDs);
			id = CountIDs;
			CountIDs++;
		}
	
	return id;
}

DWORD SkyXEngine::Core::Loaders::LoaderTextures::GetID(const char* name)
{
	char tmp_path[256];//папка
	char tmp_name[256];//само имя текстуры с расширением
	DWORD id = -1;
		//обрезаем имя текстуры и папку
		for(DWORD i=0;i<strlen(name);i++)
		{
				if(name[i] == '_')
				{
					memcpy(tmp_path,name,sizeof(char)*i);
					tmp_path[i] = 0;
					sprintf(tmp_name,"%s",name+i+1);
					break;
				}
		}

		for(DWORD i=0;i<CountArr;i++)
		{
				if(strcmp(Arr[i].Path,tmp_path) == 0)
				{
						for(DWORD k=0;k<Arr[i].ArrNames.size();k++)
						{
								if(strcmp(Arr[i].ArrNames[k],tmp_name) == 0)
								{
									id = Arr[i].ArrID[k];
									i=256;
									k=Arr[i].ArrNames.size();
								}
						}
				}
		}
	return id;
}

void SkyXEngine::Core::Loaders::LoaderTextures::GetName(DWORD id,char* name)
{
		if(id < ArrTextures.size())
		{
			char* tmpname = ArrNames[id];
			sprintf(name,"%s",tmpname);
		}
}

DWORD SkyXEngine::Core::Loaders::LoaderTextures::Create(const char* name,IDirect3DTexture9* tex)
{
	DWORD id = -1;
		if(CountIDsOld == CountIDs)
		{
			id = this->AddName(name);
			ArrTextures[id] = tex;
			CountIDsOld = CountIDs;
		}
	return id;
}

void SkyXEngine::Core::Loaders::LoaderTextures::Update(DWORD id)
{
		if(id < ArrTextures.size())
		{
			Update(ArrNames[id]);
		}
}

DWORD SkyXEngine::Core::Loaders::LoaderTextures::Update(const char* name)
{
	char tmp_path[256];//папка
	char tmp_name[256];//само имя текстыр с расширением
	DWORD id = -1;
	bool IsTruePath = false;
		//обрезаем имя текстуры и папку
		for(DWORD i=0;i<strlen(name);i++)
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
			InError("Невернео имя текстуры [%s]!!!",name);
		}

	DWORD tmpkey = -1;	//переменная в которой храним ключ от массива в который записываем
		for(DWORD i=0;i<CountArr;i++)
		{
				if(strcmp(Arr[i].Path,tmp_path) == 0)
				{
					tmpkey = i;
					break;
				}
		}

		//если мы не нашли совпадений значит путь уникален ...
		if(tmpkey == -1)
		{
			tmpkey = CountArr;
			CountArr++;
		}
	sprintf(Arr[tmpkey].Path,"%s",tmp_path);
	

	bool isunic = true;
	DWORD tmpKeyName = -1;
	
		//првоеряем записано ли уже имя текстуры
		for(DWORD i=0;i<Arr[tmpkey].ArrNames.size();i++)
		{
				if(strcmp(Arr[tmpkey].ArrNames[i],tmp_name) == 0)
				{
					isunic = false;
					id = Arr[tmpkey].ArrID[i];
					tmpKeyName = i;
					break;
				}
		}

		//если уникальное то записываем
		if(isunic)
		{
			char* tmpunicname = new char[strlen(tmp_name)+1];
			sprintf(tmpunicname,"%s",tmp_name);
			Arr[tmpkey].ArrNames.push_back(tmpunicname);
			Arr[tmpkey].ArrID.push_back(CountIDs);
			tmpKeyName = Arr[tmpkey].ArrID.size() - 1;
			id = CountIDs;
			CountIDs++;
			//ArrTexNames.push_back();
			//sprintf(Arr[tmpkey].Arr[Arr[tmpkey].Arr.size()-1],"%s",tmp_path);
		}

	SkyXEngine::Core::InLog("update texture [%s] ...",name);
	char tmpPath[1024];
	sprintf(tmpPath,"%s%s%s%s%s%s",SkyXEngine::Core::Data::Pathes::Textures,Arr[tmpkey].Path,"\\",Arr[tmpkey].Path,"_",Arr[tmpkey].ArrNames[tmpKeyName]);
	IDirect3DTexture9* tex=0;
		if(FAILED(D3DXCreateTextureFromFileEx(
											SkyXEngine::Core::Data::Device,
											tmpPath,
											D3DX_DEFAULT,
											D3DX_DEFAULT,
											10,
											0,
											D3DFMT_A8R8G8B8, 
											D3DPOOL_MANAGED, 
											D3DX_FILTER_BOX, 
											D3DX_FILTER_BOX,  0, 0, 0, &tex
											)
			))
		{
			SkyXEngine::Core::InLog("not found texture");
		}
		else
		{
			ArrTextures[Arr[tmpkey].ArrID[tmpKeyName]] = tex;
			SkyXEngine::Core::InLog("is ok\n");
		}
	
	return id;
}

void SkyXEngine::Core::Loaders::LoaderTextures::LoadTextures()
{
	SkyXEngine::Core::InLog("load textures ...\n");
	char tmpPathFor[1024];
	char tmpPath[1024];

		for(DWORD i=0;i<CountIDs-CountIDsOld;i++)
		{
			ArrTextures.push_back(0);
			ArrNames.push_back(0);
		}

		for(DWORD i=0;i<CountArr;i++)
		{
				if(Arr[i].Path[0] != 0)
				{
						for(DWORD k=0;k<Arr[i].ArrNames.size();k++)
						{
								//если id текстуры больше чем последний id загруженной текстуры
								if(Arr[i].ArrID[k] >= CountIDsOld)
								{
									sprintf(tmpPath,"%s%s%s%s%s%s",SkyXEngine::Core::Data::Pathes::Textures,Arr[i].Path,"\\",Arr[i].Path,"_",Arr[i].ArrNames[k]);
									IDirect3DTexture9* tex=0;
									//D3DXCreateTextureFromFileEx
										//if(FAILED(D3DXCreateTextureFromFile(SkyXEngine::Core::Data::Device,tmpPath,&tex)))
										if(FAILED(D3DXCreateTextureFromFileEx(
											SkyXEngine::Core::Data::Device,
											tmpPath,
											D3DX_DEFAULT,
											D3DX_DEFAULT,
											10,
											0,
											D3DFMT_A8R8G8B8, 
											D3DPOOL_MANAGED, 
											D3DX_FILTER_BOX, 
											D3DX_FILTER_BOX,  0, 0, 0, &tex
											)))
										{
											//ArrTextures.push_back(tex);
											SkyXEngine::Core::InLog("\tnot found texture [%s_%s]\n",Arr[i].Path,Arr[i].ArrNames[k]);
										}
										else
										{
											ArrTextures[Arr[i].ArrID[k]] = tex;
											//ArrNames[Arr[i].ArrID[k]] = Arr[i].ArrNames[k];
											char* tmpname = new char[256];
											sprintf(tmpname,"%s_%s",Arr[i].Path,Arr[i].ArrNames[k]);
											ArrNames[Arr[i].ArrID[k]] = tmpname;
											SkyXEngine::Core::InLog("\ttexture loaded [%s_%s], id = %d\n",Arr[i].Path,Arr[i].ArrNames[k],Arr[i].ArrID[k]);
										}
								}
						}
				}
		}
	CountIDsOld = CountIDs;
	SkyXEngine::Core::InLog("all loaded textures [%d]\n",ArrTextures.size());
}

IDirect3DTexture9* SkyXEngine::Core::Loaders::LoaderTextures::GetTexture(DWORD id)
{
		if(id < ArrTextures.size())
		{
			IDirect3DTexture9* tex = ArrTextures[id];
			return ArrTextures[id];
		}
		else 
			return 0;
}