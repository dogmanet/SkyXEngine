
#ifndef __LOADERTEXTURES_H
#define __LOADERTEXTURES_H

#include <gdefines.h>
#include <d3d9.h>
#include <common/array.h>
#include <common/String.h>
#include "sxgcore.h"

extern report_func g_fnReportf;
extern IDirect3DDevice9* DXDevice;
extern D3DPRESENT_PARAMETERS D3DAPP;

class LoaderTextures
{
public:
	LoaderTextures();
	~LoaderTextures();

	bool FileExists(const char* name);

	void ClearLoaded();

	void Delete(ID id);	//удалить текстуру id

	ID AddName(const char* name, LoadTexType type, ID* iddir = 0, ID* idname = 0);	//добавляем имя текстуры, взамен получаем на нее ID (поставить в очередь)
	ID GetID(const char* name);		//получить id по имени
	void GetName(ID id, char* name);//получить имя по id

	ID Create(const char* name, IDirect3DTexture9* tex);	//создать место для текстуры tex
	ID Update(const char* name, LoadTexType type);			//перезагрузить текстуру name
	void Update(ID id);

	void LoadTextures();	//загрузка всех текстур поставленных в очередь

	IDirect3DTexture9* GetTexture(ID id);//получить текстуру по id

private:

	//структура описывающая папку и все текстуры в ней, у каждой свой id для доступа
	struct TLPath
	{
		TLPath(){}
		~TLPath()
		{
			for (int i = 0; i < ArrTex.size(); ++i)
			{
				mem_delete(ArrTex[i]);
			}
		}

		String Path;	//имя папки

		struct TLTex
		{
			TLTex(){ id = -1; type = LoadTexType::ltt_load; }
			TLTex(ID _id, const char* _name, LoadTexType _type){ id = _id; name = _name; type = _type; }
			
			ID id;
			String name;
			LoadTexType type;
		};
		
		Array<TLTex*> ArrTex;
	};

	struct TexAndName
	{
		TexAndName(){ tex = 0; IDDir = -1; }
		~TexAndName(){ mem_release(tex); }
		String name;
		IDirect3DTexture9* tex;
		ID IDDir;
		
	};

	Array<TLPath*> Arr;
	int CurrFirstFree;

	Array<TexAndName*> ArrTextures;
	Array<ID> ArrIDsLoad;
};

#endif