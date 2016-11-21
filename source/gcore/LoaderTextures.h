
#ifndef LOADER_TEXTURES_H
#define LOADER_TEXTURES_H

#pragma once
//структура описывающая папку и все текстуры в ней, у каждой свой id джля доступа
struct TLPath
{
	TLPath()
	{
		Path[0] = 0;
	}
	
	char Path[256];//имя папки
	Array<DWORD> ArrID;		//идентификатор
	Array<char*> ArrNames;	//массив с именами текстур которые находятся в данной папке
};

class LoaderTextures
{
public:
	LoaderTextures();
	~LoaderTextures();

	DWORD AddName(const char* name);	//добавляем имя текстуры, взамен получаем на нее ID (поставить в очередь)
	DWORD GetID(const char* name);		//получить id по имени
	void GetName(DWORD id,char* name);	//получить имя по id

	DWORD Create(const char* name,IDirect3DTexture9* tex);	//создать место для текстуры tex
	DWORD Update(const char* name);		//перезагрузить текстуру name (поставить в очередь)
	void Update(DWORD id);

	void LoadTextures();	//загрузка всех текстур поставленных в очередь

	IDirect3DTexture9* GetTexture(DWORD id);//получить текстуру по id

	inline void SetStdPath(const char* path){ strcpy(StdPath, path); }
	inline void GetStdPath(char* path){ if (path)strcpy(path, StdPath); }
//private:
	char StdPath[1024];
	TLPath Arr[256];
	DWORD CountArr;
	DWORD CountIDs;
	DWORD CountIDsOld;

	Array<char*> ArrNames;
	Array<IDirect3DTexture9*> ArrTextures;
};

#endif