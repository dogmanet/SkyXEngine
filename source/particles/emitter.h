
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __EMITTER_H
#define __EMITTER_H

#include <gdefines.h>

//#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>

#include "sxparticles.h"
#include "pe_data.h"

extern g_particles_phy_collision g_fnParticlesPhyCollision;

//структура описывающая партикл
struct CommonParticle
{
	SX_ALIGNED_OP_MEM

	CommonParticle()
	{
		Age = 0;
		TimeLife = 0;
		AnimTexRateMls = 0;
		AnimTexCurrentMls = 0;
		AnimTexCurrentCadr = 0;
		AlphaAgeDependCoef = 1;
		AlphaDeath = 1;
		IsAlife = false;
		AnimTexSizeCadrAndBias = float4(1, 1, 0, 0);

		Ambient = float4(1, 1, 1, 1);

		rot_1.x = sinf(0);
		rot_1.y = cosf(0);

		CharacterRotateAngleZ = 0;
		CharacterDeviationCountDelayMls2 = 0;

		LightingIntens = 1;

		Track = false;
		TrackTime = 0;
	}

	float3 PosCreate;	//позиция создания
	float3 Pos;			//текущая позиция
	DWORD Age;			//возраст частицы в млсек
	DWORD TimeLife;		//время в млсек которое проживет частица

	float4 Ambient;

	float2_t Size;
	float4 AnimTexSizeCadrAndBias;

	WORD AnimTexCurrentCadr;
	DWORD AnimTexRateMls;
	DWORD AnimTexCurrentMls;

	float CharacterRotateAngleZ;
	DWORD CharacterDeviationCountDelayMls2;

	float2_t rot_1;

	float3 DeviationVector;

	float3 Velocity;		//скорость
	float3 Acceleration;	//ускорение

	float AlphaAgeDependCoef;
	float AlphaDeath;
	float LightingIntens;
	bool IsAlife;


	bool Track;
	float3 TrackPos;
	float3 TrackNormal;
	DWORD TrackTime;
};

//основное представление частицы в шейдере
struct CommonParticleDecl
{
	CommonParticleDecl(float posx, float posy, float posz, float texx, float texy)
	{
		pos.x = posx;
		pos.y = posy;
		pos.z = posz;

		tex.x = texx;
		tex.y = texy;
	}

	float3_t pos;
	float2_t tex;
};

//представление частицы для инстансинга в шейдере
struct CommonParticleDecl2
{
	float3_t pos;
	float4_t tex;
	float size;
	float alpha;

	float2_t rot_1;

	float lighting;
};



//#############################################################################

class CEmitter
{
public:

	CEmitter();
	CEmitter(CEmitter &oPart);
	~CEmitter();

	SX_ALIGNED_OP_MEM

	void onLostDevice();
	void onResetDevice();

	void init(const CParticlesData *pData);
	CParticlesData* getData();

	void setName(const char *szName);
	void getName(char *szName);

	void setCount(int iCount);
	int getCount();
	int getCountLife();

	void setEnable(bool isEnable);
	bool getEnable();

	void createGeomData();
	
	void compute(const float4x4 *pMat);
	void computeLighting();
	void render(DWORD timeDelta, const float4x4 *mRot, const float4x4 *mPos);

	void setTexture(const char *szTex);
	void setTextureID(ID idTex);
	ID getTextureID();
	void getTexture(char *szTex);

	void setTextureTrack(const char *szTex);
	void setTextureTrackID(ID idTex);
	ID getTextureTrackID();
	void getTextureTrack(char *szTex);

	void initAnimTexData();

	void setAlife(bool isAlife);
	bool getAlife();

	int getTrackCount();
	int getTrackPos(float3 **ppArr, int iCount);

	float3_t m_vCurrMin;
	float3_t m_vCurrMax;
	float m_fSizeAdd;

protected:

	void initNulling();

	char m_szName[OBJECT_NAME_MAX_LEN];

	bool isPointInCone(const float3 *pPoint);
	bool isPointInSphere(const float3 *pPoint);
	bool isPointInBox(const float3 *pPoint);

	void createParticles();
	void reCreateParticles(ID id);

	void modifyVertexBuff();

	void updateAnimTex(ID idParticle, DWORD dwTime);

	//размер текстуры
	float2_t m_vAnimTexSize;	

	//размер одного кадра, xy - в пикселях, zw - в процентном соотношении к размеру текстуры
	float4 m_vAnimSizeCadr;

	bool m_isTexInit;

	//! id текстуры, общей на все партиклы текущего организатора
	ID m_idTex;			

	ID m_idTexTrack;		//id текстуры следа, общей на все партиклы текущего организатора

	DWORD m_dwOldTime;		//прошлое время с которого началось обработка
	DWORD m_dwTimeNextSpawnParticle;	//время спавна будущего партикла
	
	//сколько переродили
	int m_iCountReCreate2;

	//количество живых частиц
	int m_iCountLifeParticle;	

	//живо ли сие? если false то все партиклы умерли
	bool m_isAlife;					
	float GTransparency;
	DWORD m_dwTimerDeath;
	bool m_isEnable;

	CommonParticle *m_pArr;
	int m_iCount;

	IDirect3DVertexBuffer9 *m_pTransVertBuf;

	IDirect3DVertexBuffer9 *m_pVertexBuff;
	IDirect3DIndexBuffer9 *m_pIndexBuff;

	IDirect3DVertexBuffer9 *m_pVertexBuffQuad;
	IDirect3DIndexBuffer9 *m_pIndexBuffQuad;

	CParticlesData m_oData;

	float2_t m_vOldSize;
};

#endif
