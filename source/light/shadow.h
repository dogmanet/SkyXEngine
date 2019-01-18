
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SHADOW_H
#define __SHADOW_H

#include <gdefines.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <common/Array.h>
#include <common/string.h>
#include <d3d9.h>
#include <gcore/sxgcore.h>
#include "sxlight.h"
#include "ml_data.h"

//##########################################################################

class CShadowGlobal
{
public:

	CShadowGlobal();
	~CShadowGlobal();

	void onLostDevice();
	void onResetDevice();

	SX_ALIGNED_OP_MEM

	void init();

	void updateFrustums(int iSplit, const float3 *pPosCam, const float3 *pDirCam);
	
	void preRender(int iSplit);
	void begin();
	void end();

	void genShadow(IDirect3DTexture9 *pShadowMap);
	void genShadowAll(IDirect3DTexture9 *pShadowMap);
	
	void setPosition(const float3 *pPos);
	void getPosition(float3 *pPos);

	void setBlurPixel(float fBlurPixel);
	bool getBlurPixel();

	void set4Or3Splits(bool is4);
	bool get4Or3Splits();

	void setIdVisCalcObj(ID id, int iSplit, ID idVisCalcObj);
	int getCountVisCalcObj();
	ID getIdVisCalcObj(ID id, int iSplit);

	IFrustum *m_aFrustums[5];

	int m_aIsUpdate[5];
	float2 m_aNearFar[5];
	ID m_aIdDepthMap[5];

protected:

	Array<ID*> m_aVisCalcObjs;
	float3 m_vPosition;

	void flickering(float4x4 *mLVP, float fSizeX, float fSizeY);

	float m_fBlurPixel;
	
	float2 m_vFovRatio;

	//генерация теней для 4 сплитов, иначе для 3
	bool m_useGenerating4Slits;
	
	IDirect3DSurface9 *m_pDepthSurface;
	float4x4 m_aViews[5];
	float4x4 m_aProjections[5];
	float4x4 m_aViewProj[5];
	float4x4 m_mOldView, m_mOldProj, m_mOldViewProj;
	float4x4 ScaleBiasMat;
	
	IDirect3DSurface9 *m_pOldDepthStencilSurface;
	IDirect3DSurface9 *m_pOldColorSurface;
};

//##########################################################################

class CShadowMap
{
public:
	CShadowMap();
	~CShadowMap();

	void onLostDevice();
	void onResetDevice();

	SX_ALIGNED_OP_MEM

	void init();

	void setBias(float bias);
	float getBias();

	void setBlurPixel(float blur_pixel);
	float getBlurPixel();

	void begin();
	void end();

	void genShadow(IDirect3DTexture9 *pTexture);

	void setPosition(const float3 *pPos);
	void getPosition(float3 *pPos);

	void setDirection(const float3 *pDir);
	void getDirection(float3 *pDir);

	void setAngleNearFar(const float3 * anf);
	void getAngleNearFar(float3* anf);

	void setFar(float sfar);
	float getFar();

	void setNear(float snear);
	float getNear();

	void setAngle(float sangle);
	float getAngle();

	void setIdVisCalcObj(ID id, ID idVisCalcObj);
	int getCountVisCalcObj();
	ID getIdVisCalcObj(ID id);


	IFrustum *m_pFrustum;

	float4x4 m_mView;
	float4x4 m_mProjection;

private:

	Array<ID> m_aVisCalcObjs;
	float m_fBias;
	float m_fBlurPixel;

	float3 m_vPosition;
	float3 m_vDirection;
	float3 m_vAngleNearFar;

	ID m_idDepth;
	IDirect3DSurface9 *m_pDepthSurface;
	
	float4x4 m_mOldView, m_mOldProj, m_mOldViewProj;
	float4x4 m_mScaleBiasMat;

	IDirect3DSurface9 *m_pOldDepthStencilSurface;
	IDirect3DSurface9 *m_pOldColorSurface;
};

//##########################################################################

class CShadowMapCube
{
public:
	CShadowMapCube();
	~CShadowMapCube();

	void onLostDevice();
	void onResetDevice();

	SX_ALIGNED_OP_MEM

	void setEnableCubeEdge(int iEdge, bool isEnable);
	bool getEnableCubeEdge(int iEdge);

	void setBias(float bias);
	float getBias();

	void setBlurPixel(float blur_pixel);
	bool getBlurPixel();

	void init();

	void setPosition(const float3 *pPos);
	void getPosition(float3 *pPos);

	void setNearFar(const float2 *pNearFar);
	void getNearFar(float2 *pNearFar);

	void setNear(float fNear);
	float getNear();

	void setFar(float fFar);
	float getFar();
	
	void begin();
	void pre(int iCube);
	void post(int iCube);
	void end();

	void genShadow(IDirect3DTexture9 *pTexture);

	void setIdVisCalcObj(ID id, int iSplit, ID idVisCalcObj);
	int getCountVisCalcObj();
	ID getIdVisCalcObj(ID id, int iSplit);

	IFrustum *m_aFrustums[6];

private:

	Array<ID*> m_aVisCalcObjs;
	float m_fBias;
	float m_fBlurPixel;
	ID m_idDepthCubeMap;
	IDirect3DSurface9 *m_pDepthSurface;
	float4x4 m_aViews[6];
	float4x4 m_aProjections[6];

	bool EnableEdge[6];
	bool EnableEdgeNulled[6];

	float4x4 m_mOldView, m_mOldProj, m_mOldViewProj;

	IDirect3DSurface9 *m_pOldDepthStencilSurface;
	IDirect3DSurface9 *m_pOldColorSurface;

	float3 m_vPosition;
	float2 m_vNearFar;
};

#endif