
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __CREATORTEXTURES_H
#define __CREATORTEXTURES_H

#include <gdefines.h>
#include <stdio.h>
#include <d3d9.h>
#include <common/array.h>

extern IDirect3DDevice9 *g_pDXDevice;
extern D3DCAPS9 g_dxCaps;
extern D3DPRESENT_PARAMETERS g_oD3DAPP;

//структура описывающая движковые текстуры
struct CreatedTexture
{
	char Name[64];	//имя
	IDirect3DTexture9* Texture;	//текстура
	D3DSURFACE_DESC Desc;		//описание текстуры, для восстановления
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