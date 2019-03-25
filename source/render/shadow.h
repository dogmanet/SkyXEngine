
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SHADOW_H
#define __SHADOW_H

#include <gdefines.h>

#include <common/Math.h>
#include <common/Array.h>
#include <graphix/graphix.h>
#include <gcore/sxgcore.h>
#include <light/IXLight.h>

class CShadowMap
{
public:
	CShadowMap(IGXContext *pContext);
	~CShadowMap();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(UINT uSize);
	
	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	void genShadow(IGXTexture2D *shadowmap);
	
private:
	IGXContext *m_pDevice;

	IGXTexture2D *m_pDepthMap = NULL;
	IGXTexture2D *m_pNormalMap = NULL;
	IGXTexture2D *m_pFluxMap = NULL;

	float4x4 m_mScaleBiasMat;
	float m_fBias = 0.0001f;
	float m_fBlurPixel = 0.5f;

	float m_fSize;

	//-------------------------------------------


	float3 m_vPosition;
	float3 m_vDirection;
	float3 m_vAngleNearFar;

	IGXSurface *m_pDepthSurface;
	
	IFrustum* m_pFrustum;

	float4x4 m_mView;
	float4x4 m_mProj;
};

//##########################################################################


#endif
