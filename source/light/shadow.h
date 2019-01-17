
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SHADOW_H
#define __SHADOW_H

#include <gdefines.h>

//#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <common/Array.h>
#include <graphix/graphix.h>
#include <gcore/sxgcore.h>
#include "sxlight.h"
#include "ml_data.h"

class PSSM
{
public:

	PSSM();
	~PSSM();

	void onLostDevice();
	void onResetDevice();

	SX_ALIGNED_OP_MEM

	void init();

	void updateFrustums(int iSplit, const float3 *pPosCam, const float3 *pDirCam);
	void preRender(int iSplit);
	void begin();
	void end();

	void genShadow(IGXTexture2D *pShadowMap);
	void genShadowAll(IGXTexture2D *pShadowMap);
	
	void setPosition(const float3 *pPos);
	void getPosition(float3 *pPos);

	void setBlurPixel(float fBlurPixel);
	bool getBlurPixel();

	void set4Or3Splits(bool is4);
	bool get4Or3Splits();

	void setIDArr(ID id, int iSplit, ID idArr);
	long getCountIDArrs();
	ID getIDArr(ID id, int iSplit);

	IFrustum *m_aFrustums[5];

	int m_aIsUpdate[5];
	float2 m_aNearFar[5];
	IGXTexture2D *m_aDepthMaps[5];

protected:

	Array<ID*> IDArr;
	float3 Position;

	void flickering(float4x4 *matLVP,float size_x,float size_y);

	float BlurPixel;
	
	float2 FovRatio;

	//генерация теней для 4 сплитов, иначе для 3
	bool Generating4Slits;
	
	IGXSurface*	DepthSurfaces[5];
	IGXDepthStencilSurface*	DepthStencilSurface;
	float4x4 Views[5];
	float4x4 Projs[5];
	float4x4 ViewProj[5];
	float4x4 OldView,OldProj,OldViewProj;
	float4x4 ScaleBiasMat;
	
	IGXDepthStencilSurface* OldDepthStencilSurface;
	IGXSurface* OldColorSurface;
};

//##########################################################################

class ShadowMapTech
{
public:
	ShadowMapTech();
	~ShadowMapTech();

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

	void genShadow(IGXTexture2D* shadowmap);

	void setPosition(const float3* pos);
	void getPosition(float3* pos);

	void setDirection(const float3* dir);
	void getDirection(float3* dir);

	void setAngleNearFar(const float3* anf);
	void getAngleNearFar(float3* anf);

	void setFar(float sfar);
	float getFar();

	void setNear(float snear);
	float getNear();

	void setAngle(float sangle);
	float getAngle();

	void setIDArr(long id, long idarr);
	long getCountIDArrs();
	long getIDArr(long id);

	IFrustum* Frustum;

	float4x4 View;
	float4x4 Proj;

private:

	Array<long> IDArr;
	float Bias;
	float BlurPixel;

	float3 Position;
	float3 Direction;
	float3 AngleNearFar;

	IGXTexture2D*	DepthMap;
	IGXSurface*	DepthSurface;
	IGXSurface*	DepthStencilSurface;
	
	float4x4 OldView,OldProj,OldViewProj;
	float4x4 ScaleBiasMat;

	IGXDepthStencilSurface* OldDepthStencilSurface;
	IGXDepthStencilSurface*  OldColorSurface;
};

//##########################################################################

class ShadowMapCubeTech
{
public:
	ShadowMapCubeTech();
	~ShadowMapCubeTech();

	void onLostDevice();
	void onResetDevice();

	SX_ALIGNED_OP_MEM

	void setEnableCubeEdge(int edge,bool enable);
	bool getEnableCubeEdge(int edge);

	void setBias(float bias);
	float getBias();

	void setBlurPixel(float blur_pixel);
	bool getBlurPixel();

	void init();

	void setPosition(float3* pos);
	void getPosition(float3* pos);

	void setNearFar(float2* nf);
	void getNearFar(float2* nf);

	void setNear(float val);
	float getNear();

	void setFar(float val);
	float getFar();
	
	void begin();
	void pre(int cube);
	void post(int cube);
	void end();

	void genShadow(IGXTexture2D* shadowmap);

	void setIDArr(long id, int split, long idarr);
	long getCountIDArrs();
	long getIDArr(long id, int split);

	IFrustum *m_aFrustums[6];

private:

	Array<long*> IDArr;
	float Bias;
	float BlurPixel;
	IGXTextureCube*	DepthMap;
	IGXSurface*	DepthSurface[6];
	IGXDepthStencilSurface*	DepthStencilSurface;
	float4x4 View[6];
	float4x4 Proj[6];

	bool EnableEdge[6];
	bool EnableEdgeNulled[6];

	float4x4 OldView,OldProj,OldViewProj;

	IGXDepthStencilSurface* OldDepthStencilSurface;
	IGXSurface*  OldColorSurface;

	float3 Position;
	float2 m_vNearFar;
};

#endif