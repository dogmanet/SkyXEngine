
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SKY_H
#define __SKY_H

#include <gdefines.h>
#include <d3d9.h>
#include <common/array.h>
#include <common/String.h>
#include "sxgcore.h"

extern IDirect3DDevice9 *g_pDXDevice;
extern D3DPRESENT_PARAMETERS g_oD3DAPP;

//##########################################################################

struct CSkyBoxVertex
{
	CSkyBoxVertex(float x,float y,float z,float u,float v,float w)
	{
		_x  = x;  _y  = y;  _z  = z; _u = u; _v = v; _w = w;
	}
	float _x, _y, _z;
	float _u, _v, _w;
};

//**************************************************************************

class CSkyBox
{
public:
	CSkyBox();
	~CSkyBox();
	void loadTextures(const char *szTexture);
	bool isLoadTex();
	void changeTexture(const char *szTexture);
	void getActiveTexture(char *szTexture);
	void getSecondTexture(char *szTexture);
	void setRotation(float fAngle);
	float getRotation();
	void setColor(const float4_t *pColor);
	void getColor(float4_t *pColor);
	void render(float timeDelta, const float3 *pPos,bool isShadow);

	
	SX_ALIGNED_OP_MEM

protected:

	IDirect3DVertexDeclaration9* m_pVertexDeclarationSkyBox;
	
	char m_szTexActive[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	char m_szTexSecond[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	float m_fRotaionY;
	float4x4 m_mMatRotation;
	float4_t m_vColor;
	float m_fFactorBlend;

	bool m_isChange;
	bool m_isChangeMainTex;

	IDirect3DCubeTexture9  *m_pTexture;
	IDirect3DCubeTexture9  *m_pTexture2;
	IDirect3DVertexBuffer9 *m_pVertices;
	IDirect3DIndexBuffer9  *m_pIndeces;

	ID m_idVS;
	ID m_idPS;
};

//##########################################################################

struct CSkyCloudsVertex
{
	CSkyCloudsVertex(float x,float y,float z,float u,float v)
	{
	_x  = x;  _y  = y;  _z  = z; _u = u; _v = v;
	}
	float _x, _y, _z;
	float _u, _v;
};

//**************************************************************************

class CSkyClouds
{
public:
	CSkyClouds();
	~CSkyClouds();

	//установить параметры облаков
	//если облака отбрасывают тень, то надо шобы облака покрывали почти весь уровень
	void setWidthHeightPos(float iWidth,float iHeight, const float3 *pPos);
	void loadTextures(const char *szTexture);
	bool isLoadTex();
	void changeTexture(const char *szTexture);

	void setRotation(float fAngle);
	float getRotation();
	void setAlpha(float fAngle);
	float getAlpha();
	void setColor(const float4_t *pColor);
	void getColor(float4_t *pColor);

	void setSpeed(float fSpeed);
	float getSpeed();

	void render(DWORD timeDetlta, const float3 *pPos,bool isShadow);

	SX_ALIGNED_OP_MEM
private:

	IDirect3DVertexDeclaration9 *m_pVertexDeclarationClouds;
	float m_fAlpha;
	float m_fRotaionY;
	float4x4 m_mMatRotation;
	float4_t m_vColor;

	float m_fFactorBlend;
	float2_t m_vWidthHeight;
	bool m_isChange;
	bool m_isChangeMainTex;

	float m_fSpeed;
	float m_fBias;
	CSkyCloudsVertex *m_pVertices;
	IDirect3DVertexBuffer9 *m_pSkyCloudsVertices;
	IDirect3DIndexBuffer9 *m_pSkyCloudsIndeces;

	IDirect3DTexture9 *m_pSkyCloudsTex;
	IDirect3DTexture9 *m_pSkyCloudsTex2;

	ID m_idVS;
	ID m_idPS;

	ID m_idPS_Shadow;
};

#endif
