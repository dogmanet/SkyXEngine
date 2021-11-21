#ifndef __COLLISIONSHAPE_H
#define __COLLISIONSHAPE_H

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include "IXCollisionObject.h"

btCollisionShape* GetCollisionShape(IXCollisionShape *pShape);

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

	SMAABB XMETHODCALLTYPE getAABB() const override
	{
		btTransform xForm;
		btVector3 bvMin, bvMax;
		xForm.setIdentity();
		m_pCollisionShape->getAabb(xForm, bvMin, bvMax);
		return(SMAABB(BTVEC_F3(bvMin), BTVEC_F3(bvMax)));
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

	void XMETHODCALLTYPE setUserTypeId(int iUser) override
	{
		m_iUserTypeId = iUser;
	}
	int XMETHODCALLTYPE getUserTypeId() const override
	{
		return(m_iUserTypeId);
	}

	void XMETHODCALLTYPE setUserIndex(int iUser) override
	{
		m_iUserIndex = iUser;
	}
	int XMETHODCALLTYPE getUserIndex() const override
	{
		return(m_iUserIndex);
	}
	
	XSHAPE_TYPE XMETHODCALLTYPE getType() const override
	{
		return(m_type);
	}

	IXCompoundShape* XMETHODCALLTYPE asCompound() override
	{
		return(NULL);
	}
	IXBoxShape* XMETHODCALLTYPE asBox() override
	{
		return(NULL);
	}
	IXSphereShape* XMETHODCALLTYPE asSphere() override
	{
		return(NULL);
	}
	IXCapsuleShape* XMETHODCALLTYPE asCapsule() override
	{
		return(NULL);
	}
	IXCylinderShape* XMETHODCALLTYPE asCylinder() override
	{
		return(NULL);
	}
	IXStaticPlaneShape* XMETHODCALLTYPE asStaticPlane() override
	{
		return(NULL);
	}
	IXConvexHullShape* XMETHODCALLTYPE asConvexHull() override
	{
		return(NULL);
	}
	IXTrimeshShape* XMETHODCALLTYPE asTrimesh() override
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
		pCollisionShape->setUserPointer((IXCollisionShape*)this);
		pCollisionShape->setUserIndex(2);
	}

private:
	XSHAPE_TYPE m_type;
	void *m_pUserPointer = NULL;
	btCollisionShape *m_pCollisionShape = NULL;
	int m_iUserTypeId = -1;
	int m_iUserIndex = -1;
};

//#############################################################################

class CCompoundShape: public CCollisionShape<IXCompoundShape>
{
public:
	CCompoundShape(UINT uShapes = 0);
	~CCompoundShape();

	void XMETHODCALLTYPE addChildShape(IXCollisionShape *pShape, const float3 &vLocalPos, const SMQuaternion &qLocalRot) override;

	UINT XMETHODCALLTYPE getChildCount() const override;
	IXCollisionShape* XMETHODCALLTYPE getShape(UINT uIndex) override;

	void XMETHODCALLTYPE setChildTransform(UINT uChildIndex, const float3 &vLocalPos, const SMQuaternion &qLocalRot, bool shouldRecalculateLocalAabb = true) override;
	void XMETHODCALLTYPE recalculateLocalAabb() override;

	IXCompoundShape* XMETHODCALLTYPE asCompound() override
	{
		return(this);
	}

private:
	btCompoundShape *m_pShape;
};

//#############################################################################

class CBoxShape: public CCollisionShape<IXBoxShape>
{
public:
	CBoxShape(const float3 &vHalfExtents);
	~CBoxShape();

	float3 XMETHODCALLTYPE getHalfExtents(bool useMargin = false) const override;

	IXBoxShape* XMETHODCALLTYPE asBox() override
	{
		return(this);
	}

private:
	btBoxShape *m_pShape;
};

//#############################################################################

class CSphereShape: public CCollisionShape<IXSphereShape>
{
public:
	CSphereShape(float fRadius);
	~CSphereShape();

	float XMETHODCALLTYPE getRadius() const override;

	IXSphereShape* XMETHODCALLTYPE asSphere() override
	{
		return(this);
	}

private:
	btSphereShape *m_pShape;
};

//#############################################################################

class CCapsuleShape: public CCollisionShape<IXCapsuleShape>
{
public:
	CCapsuleShape(float fRadius, float fHeight);
	~CCapsuleShape();

	float XMETHODCALLTYPE getRadius() const override;
	float XMETHODCALLTYPE getHeight() const override;

	IXCapsuleShape* XMETHODCALLTYPE asCapsule() override
	{
		return(this);
	}
private:
	btCapsuleShape *m_pShape;
};

//#############################################################################

class CCylinderShape: public CCollisionShape<IXCylinderShape>
{
public:
	CCylinderShape(float fRadius, float fHeight);
	~CCylinderShape();

	float XMETHODCALLTYPE getRadius() const override;
	float XMETHODCALLTYPE getHeight() const override;

	IXCylinderShape* XMETHODCALLTYPE asCylinder() override
	{
		return(this);
	}

private:
	btCylinderShape *m_pShape;
};

//#############################################################################

class CStaticPlaneShape: public CCollisionShape<IXStaticPlaneShape>
{
public:
	CStaticPlaneShape(const SMPLANE &plane);
	~CStaticPlaneShape();

	SMPLANE XMETHODCALLTYPE getPlane() const override;

	IXStaticPlaneShape* XMETHODCALLTYPE asStaticPlane() override
	{
		return(this);
	}

private:
	btStaticPlaneShape *m_pShape;
};

//#############################################################################

/*class CTerrainShape: public CCollisionShape<ITerrainShape>
{
public:

};*/

//#############################################################################

class CConvexHullShape: public CCollisionShape<IXConvexHullShape>
{
public:
	CConvexHullShape(UINT uPoints, const float3_t *pPoints, byte u8Stride = sizeof(float3_t), bool bOptimize = true);
	~CConvexHullShape();

	IXConvexHullShape* XMETHODCALLTYPE asConvexHull() override
	{
		return(this);
	}
private:
	btConvexHullShape *m_pShape;
};

//#############################################################################

class CTrimeshShape: public CCollisionShape<IXTrimeshShape>
{
public:
	CTrimeshShape(UINT uVertices, const float3_t *pVertices, UINT uIndices, UINT *pIndices, byte u8Stride = sizeof(float3_t));
	~CTrimeshShape();
	
	IXTrimeshShape* XMETHODCALLTYPE asTrimesh() override
	{
		return(this);
	}

private:
	btTriangleMesh *m_pMesh;
	btBvhTriangleMeshShape *m_pShape;
};

#endif
