
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
	Array<ID> ArrID;		//идентификатор
	Array<char*> ArrNames;	//массив с именами текстур которые находятся в данной папке
};

class LoaderTextures
{
public:
	LoaderTextures();
	~LoaderTextures();

	ID AddName(const char* name);	//добавляем имя текстуры, взамен получаем на нее ID (поставить в очередь)
	ID GetID(const char* name);		//получить id по имени
	void GetName(ID id, char* name);	//получить имя по id

	ID Create(const char* name, IDirect3DTexture9* tex);	//создать место для текстуры tex
	ID Update(const char* name);		//перезагрузить текстуру name (поставить в очередь)
	void Update(ID id);

	void LoadTextures();	//загрузка всех текстур поставленных в очередь

	IDirect3DTexture9* GetTexture(ID id);//получить текстуру по id

	inline void SetStdPath(const char* path){ strcpy(StdPath, path); }
	inline void GetStdPath(char* path){ if (path)strcpy(path, StdPath); }
//private:
	char StdPath[1024];
	TLPath Arr[256];
	int CountArr;
	int CountIDs;
	int CountIDsOld;

	Array<char*> ArrNames;
	Array<IDirect3DTexture9*> ArrTextures;
};

#endif