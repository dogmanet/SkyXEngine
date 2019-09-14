#include "CollisionShape.h"

btCollisionShape* GetCollisionShape(ICollisionShape *pShape)
{
	btCollisionShape *pBtShape = NULL;
	switch(pShape->getType())
	{
	case XSHAPE_INVALID:
		assert(!"Invalid shape type!");
		break;
	case XSHAPE_BOX:
		pBtShape = ((CBoxShape*)pShape->asBox())->getBtShape();
		break;
	case XSHAPE_CONVEX_HULL:
		pBtShape = ((CConvexHullShape*)pShape->asConvexHull())->getBtShape();
		break;
	case XSHAPE_SPHERE:
		pBtShape = ((CSphereShape*)pShape->asSphere())->getBtShape();
		break;
	case XSHAPE_CAPSULE:
		pBtShape = ((CCapsuleShape*)pShape->asCapsule())->getBtShape();
		break;
		//case XSHAPE_CONE:
		//break;
		//case XSHAPE_CONVEX:
		//break;
	case XSHAPE_CYLINDER:
		pBtShape = ((CCylinderShape*)pShape->asCylinder())->getBtShape();
		break;
	case XSHAPE_TRIANGLE_MESH:
		pBtShape = ((CTrimeshShape*)pShape->asTrimesh())->getBtShape();
		break;
		//case XSHAPE_TERRAIN:
		//	pBtShape = ((CTerrainShape*)pShape->asTerrain())->getBtShape();
		//	break;
	case XSHAPE_STATIC_PLANE:
		pBtShape = ((CStaticPlaneShape*)pShape->asStaticPlane())->getBtShape();
		break;
	case XSHAPE_COMPOUND:
		pBtShape = ((CCompoundShape*)pShape->asCompound())->getBtShape();
		break;
	default:
		assert(!"Unknown shape type!");
		break;
	}
	assert(pBtShape);

	return(pBtShape);
}

//#############################################################################

CCompoundShape::CCompoundShape(UINT uShapes):
	BaseClass(XSHAPE_COMPOUND)
{
	m_pShape = new btCompoundShape(true, uShapes);
	setShape(m_pShape);
}
CCompoundShape::~CCompoundShape()
{
	for(UINT i = 0, l = m_pShape->getNumChildShapes(); i < l; ++i)
	{
		((ICollisionShape*)m_pShape->getChildShape(i)->getUserPointer())->Release();
	}
	mem_delete(m_pShape);
}

void XMETHODCALLTYPE CCompoundShape::addChildShape(ICollisionShape *pShape, const float3 &vLocalPos, const SMQuaternion &qLocalRot)
{
	pShape->AddRef();
	btCollisionShape *pBtShape = GetCollisionShape(pShape);

	btTransform xForm(Q4_BTQUAT(qLocalRot), F3_BTVEC(vLocalPos));
	m_pShape->addChildShape(xForm, pBtShape);
}

UINT XMETHODCALLTYPE CCompoundShape::getChildCount() const
{
	return(m_pShape->getNumChildShapes());
}
ICollisionShape* XMETHODCALLTYPE CCompoundShape::getShape(UINT uIndex)
{
	btCollisionShape *pShape = m_pShape->getChildShape(uIndex);
	if(!pShape)
	{
		return(NULL);
	}

	ICollisionShape *pOut = (ICollisionShape*)pShape->getUserPointer();
	if(pOut)
	{
		pOut->AddRef();
	}
	return(pOut);
}

void XMETHODCALLTYPE CCompoundShape::setChildTransform(UINT uChildIndex, const float3 &vLocalPos, const SMQuaternion &qLocalRot, bool shouldRecalculateLocalAabb)
{
	btTransform xForm(Q4_BTQUAT(qLocalRot), F3_BTVEC(vLocalPos));
	m_pShape->updateChildTransform(uChildIndex, xForm, shouldRecalculateLocalAabb);
}
void XMETHODCALLTYPE CCompoundShape::recalculateLocalAabb()
{
	m_pShape->recalculateLocalAabb();
}

//#############################################################################

CBoxShape::CBoxShape(const float3 &vHalfExtents):
	BaseClass(XSHAPE_BOX)
{
	m_pShape = new btBoxShape(F3_BTVEC(vHalfExtents));
	setShape(m_pShape);
}
CBoxShape::~CBoxShape()
{
	mem_delete(m_pShape);
}

float3 XMETHODCALLTYPE CBoxShape::getHalfExtents(bool useMargin) const
{
	if(useMargin)
	{
		return(BTVEC_F3(m_pShape->getHalfExtentsWithMargin()));
	}
	else
	{
		return(BTVEC_F3(m_pShape->getHalfExtentsWithoutMargin()));
	}
}

//#############################################################################

CSphereShape::CSphereShape(float fRadius):
	BaseClass(XSHAPE_SPHERE)
{
	m_pShape = new btSphereShape(fRadius);
	setShape(m_pShape);
}
CSphereShape::~CSphereShape()
{
	mem_delete(m_pShape);
}

float XMETHODCALLTYPE CSphereShape::getRadius() const
{
	return(m_pShape->getRadius());
}

//#############################################################################

CCapsuleShape::CCapsuleShape(float fRadius, float fHeight):
	BaseClass(XSHAPE_CAPSULE)
{
	m_pShape = new btCapsuleShape(fRadius, fHeight);
	setShape(m_pShape);
}
CCapsuleShape::~CCapsuleShape()
{
	mem_delete(m_pShape);
}

float XMETHODCALLTYPE CCapsuleShape::getRadius() const
{
	return(m_pShape->getRadius());
}
float XMETHODCALLTYPE CCapsuleShape::getHeight() const
{
	return(m_pShape->getHalfHeight() * 2.0f);
}

//#############################################################################

CCylinderShape::CCylinderShape(float fRadius, float fHeight):
	BaseClass(XSHAPE_CYLINDER)
{
	m_pShape = new btCylinderShape(btVector3(fRadius, fHeight * 0.5f, fRadius));
	setShape(m_pShape);
}
CCylinderShape::~CCylinderShape()
{
	mem_delete(m_pShape);
}

float XMETHODCALLTYPE CCylinderShape::getRadius() const
{
	return(m_pShape->getRadius());
}
float XMETHODCALLTYPE CCylinderShape::getHeight() const
{
	return(((btScalar*)&m_pShape->getHalfExtentsWithoutMargin())[m_pShape->getUpAxis()] * 2.0f);
}

//#############################################################################

CStaticPlaneShape::CStaticPlaneShape(const SMPLANE &plane):
	BaseClass(XSHAPE_STATIC_PLANE)
{
	m_pShape = new btStaticPlaneShape(btVector3(plane.x, plane.y, plane.z), plane.w);
	setShape(m_pShape);
}
CStaticPlaneShape::~CStaticPlaneShape()
{
	mem_delete(m_pShape);
}

SMPLANE XMETHODCALLTYPE CStaticPlaneShape::getPlane() const
{
	return(SMPLANE(BTVEC_F3(m_pShape->getPlaneNormal()), m_pShape->getPlaneConstant()));
}

//#############################################################################

/*class CTerrainShape: public CCollisionShape<ITerrainShape>
{
public:

};*/

//#############################################################################

CConvexHullShape::CConvexHullShape(UINT uPoints, const float3_t *pPoints, byte u8Stride, bool bOptimize):
	BaseClass(XSHAPE_CONVEX_HULL)
{
	if(bOptimize)
	{
		btConvexHullShape tmpShape((float*)pPoints, uPoints, u8Stride);
		tmpShape.setMargin(0);
		btShapeHull tmpHull(&tmpShape);
		tmpHull.buildHull(0);

		m_pShape = new btConvexHullShape((float*)tmpHull.getVertexPointer(), tmpHull.numVertices(), sizeof(btVector3));
	}
	else
	{
		m_pShape = new btConvexHullShape((float*)pPoints, uPoints, u8Stride);
	}

	setShape(m_pShape);
}
CConvexHullShape::~CConvexHullShape()
{
	mem_delete(m_pShape);
}

//#############################################################################

CTrimeshShape::CTrimeshShape(UINT uVertices, const float3_t *pVertices, UINT uIndices, UINT *pIndices, byte u8Stride):
	BaseClass(XSHAPE_TRIANGLE_MESH)
{
	m_pMesh = new btTriangleMesh(true, false);
	m_pMesh->preallocateIndices(uIndices);
	m_pMesh->preallocateVertices(uVertices);
	for(UINT i = 0; i < uVertices; ++i)
	{
		m_pMesh->findOrAddVertex(F3_BTVEC(*pVertices), false);
		pVertices = (const float3_t*)(((byte*)pVertices) + u8Stride);
	}
	for(UINT i = 0; i < uIndices; i += 3)
	{
		m_pMesh->addTriangleIndices(pIndices[i], pIndices[i + 1], pIndices[i + 2]);
	}

	m_pShape = new btBvhTriangleMeshShape(m_pMesh, true);
	setShape(m_pShape);
}
CTrimeshShape::~CTrimeshShape()
{
	mem_delete(m_pShape);
	mem_delete(m_pMesh);
}
