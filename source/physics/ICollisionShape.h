#ifndef __ICOLLISIONSHAPE_H
#define __ICOLLISIONSHAPE_H

#include <gdefines.h>

enum XSHAPE_TYPE
{
	XSHAPE_INVALID,
	XSHAPE_BOX,
	XSHAPE_CONVEX_HULL,
	XSHAPE_SPHERE,
	XSHAPE_CAPSULE,
	XSHAPE_CONE,
	XSHAPE_CONVEX,
	XSHAPE_CYLINDER,
	XSHAPE_TRIANGLE_MESH,
	XSHAPE_TERRAIN,
	XSHAPE_STATIC_PLANE,
	XSHAPE_COMPOUND
};

class ICollisionShape: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE setLocalScaling(const float3 &vScaling) = 0;
	virtual float3 XMETHODCALLTYPE getLocalScaling() const = 0;
	//virtual void XMETHODCALLTYPE calculateLocalInertia(float fMass, float3 &vInertia) const = 0;

	virtual void XMETHODCALLTYPE setMargin(float fMargin) = 0;
	virtual float XMETHODCALLTYPE getMargin() const = 0;

	virtual void XMETHODCALLTYPE setUserPointer(void *pUser) = 0;
	virtual void* XMETHODCALLTYPE getUserPointer() const = 0;

	virtual XSHAPE_TYPE XMETHODCALLTYPE getType() const = 0;

	virtual ICompoundShape* XMETHODCALLTYPE asCompound() = 0;
	virtual IBoxShape* XMETHODCALLTYPE asBox() = 0;
	virtual ISphereShape* XMETHODCALLTYPE asSphere() = 0;
	virtual ICapsuleShape* XMETHODCALLTYPE asCapsule() = 0;
	virtual IStaticPlaneShape* XMETHODCALLTYPE asStaticPlane() = 0;
	virtual IConvexHullShape* XMETHODCALLTYPE asConvexHull() = 0;
	virtual ITrimeshShape* XMETHODCALLTYPE asTrimesh() = 0;
};

class IConcaveShape: public ICollisionShape
{
public:
	// virtual void	processAllTriangles(ITriangleCallback* callback, const float3 &aabbMin, const float3 &aabbMax) const = 0;
};

class IConvexShape: public ICollisionShape
{
public:

};

class ICompoundShape: public ICollisionShape
{
public:
	virtual void XMETHODCALLTYPE addChildShape(ICollisionShape *pShape, const float3 &vLocalPos, const SMQuaternion &qLocalRot) = 0;

	virtual UINT XMETHODCALLTYPE getChildCount() const = 0;
	virtual ICollisionShape *getShape(UINT uIndex) = 0;

	virtual void XMETHODCALLTYPE setChildTransform(UINT uChildIndex, const float3 &vLocalPos, const SMQuaternion &qLocalRot, bool shouldRecalculateLocalAabb = true) = 0;
	virtual void XMETHODCALLTYPE recalculateLocalAabb() = 0;

	// virtual void XMETHODCALLTYPE removeChild(UINT uIndex) = 0;
};



class IBoxShape: public IConvexShape
{
public:
	virtual float3 XMETHODCALLTYPE getHalfExtents(bool useMargin = false) const = 0;
};

class ISphereShape: public IConvexShape
{
public:
	virtual float XMETHODCALLTYPE getRadius() const = 0;
};

class ICapsuleShape: public IConvexShape
{
public:
	virtual float XMETHODCALLTYPE getRadius() const = 0;
	virtual float XMETHODCALLTYPE getHeight() const = 0;
};

class IStaticPlaneShape: public IConcaveShape
{
public:
	virtual SMPLANE XMETHODCALLTYPE getPlane() const = 0;
};

/*class ITerrainShape: public IConcaveShape
{
public:

};*/

class IConvexHullShape: public IConvexShape
{
public:

};

class ITrimeshShape: public IConcaveShape
{
public:

};

#endif
