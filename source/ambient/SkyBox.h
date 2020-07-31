
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SKY_H
#define __SKY_H

#include <gdefines.h>
#include <graphix/graphix.h>
#include <common/array.h>
#include <common/String.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>

//##########################################################################

//! структура вершин для скайбокса
struct CSkyBoxVertex
{
	CSkyBoxVertex(float fX, float fY, float fZ, float fU, float fV, float fW)
	{
		x = fX;  y = fY;  z = fZ; u = fU; v = fV; w = fW;
	}
	float x, y, z;
	float u, v, w;
};

//##########################################################################

class CRFarCvarListener;

//! скайбокс, куб, на который накалывдается кубическая текстура с изображением неба
class CSkyBox
{
	friend class CRFarCvarListener;
public:
	SX_ALIGNED_OP_MEM2();

	CSkyBox(IXCore *pCore);
	~CSkyBox();

	void setDevice(IGXDevice *pDevice);
	void setMaterialSystem(IXMaterialSystem *pMaterialSystem);

	//! загрузка текстуры
	void setTexture(const char *szTexture);

	void getTexture(IXTexture **ppTexture);

	//! используется ли?
	bool isEnabled();
	//! установить использование
	void enable(bool yesNo);

	//! замена текстуры
	void changeTexture(const char *szTexture);

	//! установить угол поворота
	void setRotation(float fAngle);
	//! возвращает угол поворота
	float getRotation();

	//! установить цвет, w компонента это коэффициент цвета [0,1]
	void setColor(const float4 &vColor);
	//! в pColor записывает цвет
	const float4_t& getColor();

	//! рендер
	void render();

	void update(float fDT);

protected:
	void updateBuffers();

	IXCore *m_pCore;

	//! декларация вершин
	IGXVertexDeclaration *m_pVertexDeclarationSkyBox = NULL;
	
	//! угол поворота
	float m_fRotaionY = 0.0f;

	//! матрица поворота
	float4x4 m_mMatRotation;

	//! цвет
	float4_t m_vColor = float4_t(0.0f, 0.0f, 0.0f, 0.0f);

	//! коэфициент смешивания текстур
	float m_fBlendFactor = 0.0f;

	//! происходит ли смена текстур
	bool m_isChanging = false;

	//! используется ли?
	bool m_isEnabled = true;

	
	//! первая (основная текстура)
	IXMaterial *m_pSky1 = NULL;
	//! вторая (заменяет первую при #changeTexture)
	IXMaterial *m_pSky2 = NULL;
	
	//! индексный буфер
	IGXIndexBuffer *m_pIndeces = NULL;
	IGXRenderBuffer *m_pRenderBuffer = NULL;

	IGXDevice *m_pDevice = NULL;
	IXMaterialSystem *m_pMaterialSystem = NULL;

	XVertexShaderHandler *m_pVertexShaderHandler = NULL;

	CRFarCvarListener *m_pRFarCvarListener = NULL;

	IEventChannel<XEventSkyboxChanged> *m_pEventChannel = NULL;
};

#if 0
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
	IGXVertexDeclaration *m_pVertexDeclarationClouds;

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
	IGXVertexBuffer *m_pVertices;

	//! индексный буфер
	IGXIndexBuffer *m_pIndeces;

	IGXRenderBuffer *m_pRenderBuffer;

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

#endif
