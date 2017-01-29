
#ifndef CREATOR_TEXTURES_H
#define CREATOR_TEXTURES_H

#pragma once

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

	DWORD Add(UINT width,UINT height,UINT levels,DWORD usage,D3DFORMAT format,D3DPOOL pool,const char* name,float coeffullscreen);

	void Delete(const char* text);
	void Delete(DWORD num);

	DWORD GetNum(const char* text);

	void OnLostDevice();
	void OnResetDevice();

	IDirect3DTexture9* GetTexture(const char* text);
	IDirect3DTexture9* GetTexture(DWORD num);
private:
	Array<CreatedTexture*> Arr;
};

#endif