#ifndef __XEDITORGIZMO_H
#define __XEDITORGIZMO_H

#include <gdefines.h>

class IXEditorGizmo: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE setPos(const float3_t &vPos) = 0;
	virtual const float3_t& XMETHODCALLTYPE getPos() = 0;

	// virtual void XMETHODCALLTYPE setColor(const float4_t &vColor) = 0;
};


class IXEditorGizmoRadius;
class IXEditorGizmoRadiusCallback
{
public:
	virtual void XMETHODCALLTYPE onChange(float fNewRadius, IXEditorGizmoRadius *pGizmo) = 0;
};

//! Имеет заданый радиус
class IXEditorGizmoRadius: public IXEditorGizmo
{
public:
	virtual void XMETHODCALLTYPE setRadius(float fRadius) = 0;
	virtual float XMETHODCALLTYPE getRadius() = 0;

	virtual void XMETHODCALLTYPE setMinRadius(float fRadius) = 0;
	virtual void XMETHODCALLTYPE setMaxRadius(float fRadius) = 0;

	virtual void XMETHODCALLTYPE setCallback(IXEditorGizmoRadiusCallback *pCallback) = 0;
};

class IXEditorGizmoHandle;
class IXEditorGizmoHandleCallback
{
public:
	virtual void XMETHODCALLTYPE moveTo(const float3 &vNewPos, IXEditorGizmoHandle *pGizmo) = 0;
};

//! Перемещается в 2d, либо в 3d в заданной плоскости, либо вдоль заданного вектора
class IXEditorGizmoHandle: public IXEditorGizmo
{
public:
	virtual void XMETHODCALLTYPE lockInPlane(const float3_t &vPlaneNormal) = 0;
	virtual void XMETHODCALLTYPE lockInDir(const float3_t &vDir) = 0;
	virtual void XMETHODCALLTYPE unLock() = 0;

	virtual void XMETHODCALLTYPE setCallback(IXEditorGizmoHandleCallback *pCallback) = 0;
};

class IXEditorGizmoMove: public IXEditorGizmo
{
public:
};

class IXEditorGizmoRotate: public IXEditorGizmo
{
public:
};

class IXEditorGizmoScale: public IXEditorGizmo
{
public:
};

#endif
