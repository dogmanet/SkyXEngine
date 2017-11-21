
#ifndef __CREATORTEXTURES_H
#define __CREATORTEXTURES_H

#include <gdefines.h>
#include <stdio.h>
#include <d3d9.h>
#include <common/array.h>

extern report_func g_fnReportf;
extern IDirect3DDevice9* DXDevice;
extern D3DPRESENT_PARAMETERS D3DAPP;

//структура описывающа€ движковые текстуры
struct CreatedTexture
{
	char Name[64];	//им€
	IDirect3DTexture9* Texture;	//текстура
	D3DSURFACE_DESC Desc;		//описание текстуры, дл€ восстановлени€
	UINT Level;
	float CoefFullScreen;		//если меньше 0 то брать размер из Desc, иначе это коэфициент умножение размера экрана на это число
};

class CreatorTextures
{
public:
	CreatorTextures();
	~CreatorTextures();

	ID Add(UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, const char* name, float coeffullscreen);

	void Delete(const char* text);
	void Delete(ID num);

	ID GetNum(const char* text);

	void OnLostDevice();
	void OnResetDevice();

	IDirect3DTexture9* GetTexture(const char* text);
	IDirect3DTexture9* GetTexture(ID num);
private:
	Array<CreatedTexture*> Arr;
};

#endif