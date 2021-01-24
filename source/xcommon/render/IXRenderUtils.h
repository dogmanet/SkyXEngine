#ifndef __IXRENDERUTILS_H
#define __IXRENDERUTILS_H

#include <gdefines.h>
#include <graphix/graphix.h>

enum XGIZMO_POINT_MODE
{
	XGPM_SQUARE,
	XGPM_ROUND
};

//! 
class IXGizmoRenderer: public IXUnknown
{
public:
	//! 
	virtual void XMETHODCALLTYPE reset() = 0;
	//! 
	virtual void XMETHODCALLTYPE render(bool isOrtho) = 0;

	//! 
	virtual void XMETHODCALLTYPE setLineWidth(float fWidth) = 0;

	//! 
	virtual void XMETHODCALLTYPE setColor(const float4 &vColor) = 0;

	//! 
	virtual void XMETHODCALLTYPE setTexture(IGXBaseTexture *pTexture) = 0;

	//! 
	virtual void XMETHODCALLTYPE jumpTo(const float3 &vPos) = 0;
	//! 
	virtual void XMETHODCALLTYPE lineTo(const float3 &vPos) = 0;


	//! 
	virtual void XMETHODCALLTYPE setPointSize(float fWidth) = 0;
	//! 
	virtual void XMETHODCALLTYPE setPointMode(XGIZMO_POINT_MODE pointMode) = 0;
	//! 
	virtual void XMETHODCALLTYPE drawPoint(const float3 &vPos) = 0;

	//! 
	virtual void XMETHODCALLTYPE drawAABB(const SMAABB &aabb) = 0;

	//! 
	virtual void XMETHODCALLTYPE drawEllipsoid(const float3 &vPos, const float3 vSize) = 0;
};



// {D4025713-43EC-4A55-8989-0639379F981F}
#define IXRENDERUTILS_GUID DEFINE_XGUID(0xd4025713, 0x43ec, 0x4a55, 0x89, 0x89, 0x6, 0x39, 0x37, 0x9f, 0x98, 0x1f)
#define IXRENDERUTILS_VERSION 1

//! 
class IXRenderUtils: public IXUnknown
{
public:
	//! 
	virtual void XMETHODCALLTYPE newGizmoRenderer(IXGizmoRenderer **ppOut) const = 0;
};


#endif
