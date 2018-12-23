
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

//! структура вершин для скайбокса
struct CSkyBoxVertex
{
	CSkyBoxVertex(float fX,float fY,float fZ,float fU,float fV,float fW)
	{
		x = fX;  y = fY;  z = fZ; u = fU; v = fV; w = fW;
	}
	float x, y, z;
	float u, v, w;
};

//**************************************************************************

//! скайбокс, куб, на который накалывдается кубическая текстура с изображением неба
class CSkyBox
{
public:
	CSkyBox();
	~CSkyBox();

	SX_ALIGNED_OP_MEM

	//! загрузка текстуры
	void loadTexture(const char *szTexture);

	//! загружена ли текстура
	bool isLoadTex();

	//! используется ли?
	bool getUse();

	//! установить использование
	void setUse(bool isUse);

	//! замена текстуры
	void changeTexture(const char *szTexture);

	//! в szTexture запишет имя текущей активной текстуры
	void getActiveTexture(char *szTexture);

	//! в szTexture запишет имя следующей текстуры
	void getSecondTexture(char *szTexture);

	//! установить угол поворота
	void setRotation(float fAngle);

	//! возвращает угол поворота
	float getRotation();

	//! установить цвет, w компонента это коэффициент цвета [0,1]
	void setColor(const float4_t *pColor);

	//! в pColor записывает цвет
	void getColor(float4_t *pColor);

	//! рендер
	void render(float timeDelta, const float3 *pPos,bool isShadow);

protected:

	//! декларация вершин
	IDirect3DVertexDeclaration9* m_pVertexDeclarationSkyBox;
	
	//! угол поворота
	float m_fRotaionY;

	//! матрица поворота
	float4x4 m_mMatRotation;

	//! цвет
	float4_t m_vColor;

	//! коэфициент смешивания текстур
	float m_fFactorBlend;

	//! происходит ли смена текстур
	bool m_isChange;

	//! используется ли?
	bool m_isUse;

	
	//bool m_isChangeMainTex;

	//! первая (основная текстура)
	ID m_idTex1;

	//! вторая (заменяет первую при #changeTexture)
	ID m_idTex2;

	//! вершинный буфер
	IDirect3DVertexBuffer9 *m_pVertices;

	//! индексный буфер
	IDirect3DIndexBuffer9  *m_pIndeces;

	//! вершинный шейдер
	ID m_idVS;

	//! пиксельный шейдер
	ID m_idPS;
};

//##########################################################################

//! структура вершин облаков
struct CSkyCloudsVertex
{
	CSkyCloudsVertex(float fX,float fY,float fZ,float fU,float fV)
	{
		x = fX;  y = fY;  z = fZ; u = fU; v = fV;
	}
	float x, y, z;
	float u, v;
};

//**************************************************************************

//! облака
class CSkyClouds
{
public:
	CSkyClouds();
	~CSkyClouds();

	SX_ALIGNED_OP_MEM

	//! установить параметры облаков
	void setWidthHeightPos(float iWidth,float iHeight, const float3 *pPos);

	//! загрузка текстуры
	void loadTexture(const char *szTexture);

	//! загружены ли текстура
	bool isLoadTex();

	//! используется ли?
	bool getUse();

	//! установить использование
	void setUse(bool isUse);

	//! заменить текстуру
	void changeTexture(const char *szTexture);

	//Установить поворот
	void setRotation(float fAngle);

	//! возвращает поворот
	float getRotation();

	//! устанавливает прозрачность
	void setAlpha(float fAngle);

	//! возвращает прозрачность
	float getAlpha();

	//! установаить цвет, w компонента это коэффициент цвета [0,1]
	void setColor(const float4_t *pColor);

	//! в pColor записывает цвет
	void getColor(float4_t *pColor);

	//! установить скорость движения
	void setSpeed(float fSpeed);

	//! возвращает скорость движения
	float getSpeed();

	//! рендер
	void render(DWORD timeDetlta, const float3 *pPos, bool isShadow);

private:

	//! декларация вершин
	IDirect3DVertexDeclaration9 *m_pVertexDeclarationClouds;

	//! коэфициент прозарчности
	float m_fAlpha;

	//! поворот по оси Y
	float m_fRotaionY;

	//! матрица поворота
	float4x4 m_mMatRotation;

	//! используется ли?
	bool m_isUse;

	//! цвет облаков
	float4_t m_vColor;

	//! коэфициент смешивания предыдущей текстуры с новой
	float m_fFactorBlend;

	//! размеры квада облаков
	float2_t m_vWidthHeight;

	//! происходит ли сейчас смена текстур
	bool m_isChange;

	//bool m_isChangeMainTex;

	//! скорость движения облаков
	float m_fSpeed;

	//! текущее смещение в текстурных координатах
	float m_fBias;

	//! вершинный буфер
	IDirect3DVertexBuffer9 *m_pVertices;

	//! индексный буфер
	IDirect3DIndexBuffer9 *m_pIndeces;

	//! первая (основная текстура)
	ID m_idTex1;

	//! вторая (заменяет первую при #changeTexture)
	ID m_idTex2;

	//! вершинный шейдер
	ID m_idVS;

	//! пиксельный шейдер (отрисовка для камеры)
	ID m_idPS;

	//! пиксельный шейдер (отрисовка для теней)
	ID m_idPS_Shadow;
};

#endif
