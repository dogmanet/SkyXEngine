#ifndef __COLLISIONSHAPE_H
#define __COLLISIONSHAPE_H

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include "ICollisionObject.h"

btCollisionShape* GetCollisionShape(ICollisionShape *pShape);

template<class T>
class CCollisionShape: public IXUnknownImplementation<T>
{
public:
	typedef CCollisionShape<T> BaseClass;

	CCollisionShape(XSHAPE_TYPE type):
		m_type(type)
	{
	}
	void XMETHODCALLTYPE setLocalScaling(const float3 &vScaling) override
	{
		m_pCollisionShape->setLocalScaling(F3_BTVEC(vScaling));
	}
	float3 XMETHODCALLTYPE getLocalScaling() const override
	{
		return(BTVEC_F3(m_pCollisionShape->getLocalScaling()));
	}
	float3 XMETHODCALLTYPE calculateLocalInertia(float fMass) const override
	{
		btVector3 vInertia;
		m_pCollisionShape->calculateLocalInertia(fMass, vInertia);
		return(BTVEC_F3(vInertia));
	}

	void XMETHODCALLTYPE setMargin(float fMargin) override
	{
		m_pCollisionShape->setMargin(fMargin);
	}
	float XMETHODCALLTYPE getMargin() const override
	{
		return(m_pCollisionShape->getMargin());
	}

	void XMETHODCALLTYPE setUserPointer(void *pUser) override
	{
		m_pUserPointer = pUser;
	}
	void* XMETHODCALLTYPE getUserPointer() const override
	{
		return(m_pUserPointer);
	}

	XSHAPE_TYPE XMETHODCALLTYPE getType() const override
	{
		return(m_type);
	}

	ICompoundShape* XMETHODCALLTYPE asCompound() override
	{
		return(NULL);
	}
	IBoxShape* XMETHODCALLTYPE asBox() override
	{
		return(NULL);
	}
	ISphereShape* XMETHODCALLTYPE asSphere() override
	{
		return(NULL);
	}
	ICapsuleShape* XMETHODCALLTYPE asCapsule() override
	{
		return(NULL);
	}
	ICylinderShape* XMETHODCALLTYPE asCylinder() override
	{
		return(NULL);
	}
	IStaticPlaneShape* XMETHODCALLTYPE asStaticPlane() override
	{
		return(NULL);
	}
	IConvexHullShape* XMETHODCALLTYPE asConvexHull() override
	{
		return(NULL);
	}
	ITrimeshShape* XMETHODCALLTYPE asTrimesh() override
	{
		return(NULL);
	}

	btCollisionShape* getBtShape()
	{
		return(m_pCollisionShape);
	}

protected:

	void setShape(btCollisionShape *pCollisionShape)
	{
		m_pCollisionShape = pCollisionShape;
		pCollisionShape->setUserPointer((ICollisionShape*)this);
	}

private:
	XSHAPE_TYPE m_type;
	void *m_pUserPointer = NULL;
	btCollisionShape *m_pCollisionShape = NULL;
};

class CCompoundShape: public CCollisionShape<ICompoundShape>
{
public:
	CCompoundShape(UINT uShapes = 0);
	~CCompoundShape();

	void XMETHODCALLTYPE addChildShape(ICollisionShape *pShape, const float3 &vLocalPos, const SMQuaternion &qLocalRot) override;

	UINT XMETHODCALLTYPE getChildCount() const override;
	ICollisionShape* XMETHODCALLTYPE getShape(UINT uIndex) override;

	void XMETHODCALLTYPE setChildTransform(UINT uChildIndex, const float3 &vLocalPos, const SMQuaternion &qLocalRot, bool shouldRecalculateLocalAabb = true) override;
	void XMETHODCALLTYPE recalculateLocalAabb() override;

	ICompoundShape* XMETHODCALLTYPE asCompound() override
	{
		return(this);
	}

private:
	btCompoundShape *m_pShape;
};



class CBoxShape: public CCollisionShape<IBoxShape>
{
public:
	CBoxShape(const float3 &vHalfExtents);
	~CBoxShape();

	float3 XMETHODCALLTYPE getHalfExtents(bool useMargin = false) const override;

	IBoxShape* XMETHODCALLTYPE asBox() override
	{
		return(this);
	}

private:
	btBoxShape *m_pShape;
};

class CSphereShape: public CCollisionShape<ISphereShape>
{
public:
	CSphereShape(float fRadius);
	~CSphereShape();

	float XMETHODCALLTYPE getRadius() const override;

	ISphereShape* XMETHODCALLTYPE asSphere() override
	{
		return(this);
	}

private:
	btSphereShape *m_pShape;
};

class CCapsuleShape: public CCollisionShape<ICapsuleShape>
{
public:
	CCapsuleShape(float fRadius, float fHeight);
	~CCapsuleShape();

	float XMETHODCALLTYPE getRadius() const override;
	float XMETHODCALLTYPE getHeight() const override;

	ICapsuleShape* XMETHODCALLTYPE asCapsule() override
	{
		return(this);
	}
private:
	btCapsuleShape *m_pShape;
};

class CCylinderShape: public CCollisionShape<ICylinderShape>
{
public:
	CCylinderShape(float fRadius, float fHeight);
	~CCylinderShape();

	float XMETHODCALLTYPE getRadius() const override;
	float XMETHODCALLTYPE getHeight() const override;

	ICylinderShape* XMETHODCALLTYPE asCylinder() override
	{
		return(this);
	}

private:
	btCylinderShape *m_pShape;
};

class CStaticPlaneShape: public CCollisionShape<IStaticPlaneShape>
{
public:
	CStaticPlaneShape(const SMPLANE &plane);
	~CStaticPlaneShape();

	SMPLANE XMETHODCALLTYPE getPlane() const override;

	IStaticPlaneShape* XMETHODCALLTYPE asStaticPlane() override
	{
		return(this);
	}

private:
	btStaticPlaneShape *m_pShape;
};

/*class CTerrainShape: public CCollisionShape<ITerrainShape>
{
public:

};*/

class CConvexHullShape: public CCollisionShape<IConvexHullShape>
{
public:
	CConvexHullShape(UINT uPoints, const float3_t *pPoints, byte u8Stride = sizeof(float3_t), bool bOptimize = true);
	~CConvexHullShape();

	IConvexHullShape* XMETHODCALLTYPE asConvexHull() override
	{
		return(this);
	}
private:
	btConvexHullShape *m_pShape;
};

class CTrimeshShape: public CCollisionShape<ITrimeshShape>
{
public:
	CTrimeshShape(UINT uVertices, const float3_t *pVertices, UINT uIndices, UINT *pIndices, byte u8Stride = sizeof(float3_t));
	~CTrimeshShape();
	
	ITrimeshShape* XMETHODCALLTYPE asTrimesh() override
	{
		return(this);
	}

private:
	btTriangleMesh *m_pMesh;
	btBvhTriangleMeshShape *m_pShape;
};

#endif
