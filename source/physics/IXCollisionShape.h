#ifndef __IXCOLLISIONSHAPE_H
#define __IXCOLLISIONSHAPE_H

#include <gdefines.h>

enum XSHAPE_TYPE
{
	XSHAPE_INVALID,
	XSHAPE_BOX,
	XSHAPE_CONVEX_HULL,
	XSHAPE_SPHERE,
	XSHAPE_CAPSULE,
	//XSHAPE_CONE,
	//XSHAPE_CONVEX,
	XSHAPE_CYLINDER,
	XSHAPE_TRIANGLE_MESH,
	XSHAPE_TERRAIN,
	XSHAPE_STATIC_PLANE,
	XSHAPE_COMPOUND
};

class IXCompoundShape;
class IXBoxShape;
class IXSphereShape;
class IXCapsuleShape;
class IXCylinderShape;
class IXCylinderShape;
class IXStaticPlaneShape;
class IXConvexHullShape;
class IXTrimeshShape;

class IXCollisionShape: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE setLocalScaling(const float3 &vScaling) = 0;
	virtual float3 XMETHODCALLTYPE getLocalScaling() const = 0;
	virtual float3 XMETHODCALLTYPE calculateLocalInertia(float fMass) const = 0;

	virtual SMAABB XMETHODCALLTYPE getAABB() const = 0;

	virtual void XMETHODCALLTYPE setMargin(float fMargin) = 0;
	virtual float XMETHODCALLTYPE getMargin() const = 0;

	virtual void XMETHODCALLTYPE setUserPointer(void *pUser) = 0;
	virtual void* XMETHODCALLTYPE getUserPointer() const = 0;

	virtual void XMETHODCALLTYPE setUserTypeId(int iUser) = 0;
	virtual int XMETHODCALLTYPE getUserTypeId() const = 0;

	virtual void XMETHODCALLTYPE setUserIndex(int iUser) = 0;
	virtual int XMETHODCALLTYPE getUserIndex() const = 0;

	virtual XSHAPE_TYPE XMETHODCALLTYPE getType() const = 0;

	virtual IXCompoundShape* XMETHODCALLTYPE asCompound() = 0;
	virtual IXBoxShape* XMETHODCALLTYPE asBox() = 0;
	virtual IXSphereShape* XMETHODCALLTYPE asSphere() = 0;
	virtual IXCapsuleShape* XMETHODCALLTYPE asCapsule() = 0;
	virtual IXCylinderShape* XMETHODCALLTYPE asCylinder() = 0;
	virtual IXStaticPlaneShape* XMETHODCALLTYPE asStaticPlane() = 0;
	virtual IXConvexHullShape* XMETHODCALLTYPE asConvexHull() = 0;
	virtual IXTrimeshShape* XMETHODCALLTYPE asTrimesh() = 0;
};

//#############################################################################

class IXConcaveShape: public IXCollisionShape
{
public:
	// virtual void	processAllTriangles(ITriangleCallback* callback, const float3 &aabbMin, const float3 &aabbMax) const = 0;
};

//#############################################################################

class IXConvexShape: public IXCollisionShape
{
public:

};

//#############################################################################

class IXCompoundShape: public IXCollisionShape
{
public:
	virtual void XMETHODCALLTYPE addChildShape(IXCollisionShape *pShape, const float3 &vLocalPos, const SMQuaternion &qLocalRot) = 0;

	virtual UINT XMETHODCALLTYPE getChildCount() const = 0;
	virtual IXCollisionShape* XMETHODCALLTYPE getShape(UINT uIndex) = 0;

	virtual void XMETHODCALLTYPE setChildTransform(UINT uChildIndex, const float3 &vLocalPos, const SMQuaternion &qLocalRot, bool shouldRecalculateLocalAabb = true) = 0;
	virtual void XMETHODCALLTYPE recalculateLocalAabb() = 0;

	// virtual void XMETHODCALLTYPE removeChild(UINT uIndex) = 0;
};

//#############################################################################

class IXBoxShape: public IXConvexShape
{
public:
	virtual float3 XMETHODCALLTYPE getHalfExtents(bool useMargin = false) const = 0;
};

//#############################################################################

class IXSphereShape: public IXConvexShape
{
public:
	virtual float XMETHODCALLTYPE getRadius() const = 0;
};

//#############################################################################

class IXCapsuleShape: public IXConvexShape
{
public:
	virtual float XMETHODCALLTYPE getRadius() const = 0;
	virtual float XMETHODCALLTYPE getHeight() const = 0;
};

//#############################################################################

class IXCylinderShape: public IXConvexShape
{
public:
	virtual float XMETHODCALLTYPE getRadius() const = 0;
	virtual float XMETHODCALLTYPE getHeight() const = 0;
};

//#############################################################################

class IXStaticPlaneShape: public IXConcaveShape
{
public:
	virtual SMPLANE XMETHODCALLTYPE getPlane() const = 0;
};

//#############################################################################

/*class IXTerrainShape: public IXConcaveShape
{
public:

};*/

//#############################################################################

class IXConvexHullShape: public IXConvexShape
{
public:

};

//#############################################################################

class IXTrimeshShape: public IXConcaveShape
{
public:

};

#endif
