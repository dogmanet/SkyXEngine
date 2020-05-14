#ifndef __OCCLUSIONCULLER_H
#define __OCCLUSIONCULLER_H

#include <xcommon/render/IXOcclusionCuller.h>
#include <light/IXLightSystem.h>

#define OCCLUSION_BUFFER_WIDTH 16
#define OCCLUSION_BUFFER_HEIGHT 16

class COcclusionCuller: public IXUnknownImplementation<IXOcclusionCuller>
{
public:
	COcclusionCuller();
	~COcclusionCuller();

	void XMETHODCALLTYPE update(ICamera *pCamera, float fFOV, float fAspectRatio, float fFarPlane) override;

	bool XMETHODCALLTYPE isPointVisible(const float3 &vPoint) const override;

	bool XMETHODCALLTYPE isPolyVisible(const float3 &vPoint1, const float3 &vPoint2, const float3 &vPoint3) const override;

	bool XMETHODCALLTYPE isSphereVisible(const float3 &vOrigin, float fRadius) const override;

	bool XMETHODCALLTYPE isAABBvisible(const float3 &vMin, const float3 &vMax) const override;

protected:
	ICamera *m_pCamera = NULL;
	float m_fFOV = 1.0f;
	float m_fAspectRatio = 1.0f;
	float m_fFarPlane = 1.0f;

	float m_ppfOcclusionBuffer[OCCLUSION_BUFFER_WIDTH][OCCLUSION_BUFFER_HEIGHT];

	void _update();

	void _print();

	IXLightSystem *m_pLightSystem = NULL;
	Array<IXLightPoint*> m_apTemp;
};

#endif
