#include "ModelPhysbox.h"

XPHYSBOXTYPE XMETHODCALLTYPE CModelPhysboxBox::getType() const
{
	return(XPBT_BOX);
}

const IModelPhysboxBox* XMETHODCALLTYPE CModelPhysboxBox::asBox() const
{
	return(this);
}

IModelPhysboxBox* XMETHODCALLTYPE CModelPhysboxBox::asBox()
{
	return(this);
}

float3 XMETHODCALLTYPE CModelPhysboxBox::getSize() const
{
	return(m_vSize);
}
void XMETHODCALLTYPE CModelPhysboxBox::setSize(const float3 &vSize)
{
	m_vSize = vSize;
}

//##########################################################################

XPHYSBOXTYPE XMETHODCALLTYPE CModelPhysboxSphere::getType() const
{
	return(XPBT_SPHERE);
}

const IModelPhysboxSphere* XMETHODCALLTYPE CModelPhysboxSphere::asSphere() const
{
	return(this);
}

IModelPhysboxSphere* XMETHODCALLTYPE CModelPhysboxSphere::asSphere()
{
	return(this);
}

float XMETHODCALLTYPE CModelPhysboxSphere::getRadius() const
{
	return(m_fRadius);
}
void XMETHODCALLTYPE CModelPhysboxSphere::setRadius(float fRadius)
{
	m_fRadius = fRadius;
}

//##########################################################################

CModelPhysboxConvex::~CModelPhysboxConvex()
{
	mem_delete_a(m_pData);
}

XPHYSBOXTYPE XMETHODCALLTYPE CModelPhysboxConvex::getType() const
{
	return(XPBT_CONVEX);
}

const IModelPhysboxConvex* XMETHODCALLTYPE CModelPhysboxConvex::asConvex() const
{
	return(this);
}

IModelPhysboxConvex* XMETHODCALLTYPE CModelPhysboxConvex::asConvex()
{
	return(this);
}

UINT XMETHODCALLTYPE CModelPhysboxConvex::getVertexCount() const
{
	return(m_uVertexCount);
}
const float3_t * XMETHODCALLTYPE CModelPhysboxConvex::getData() const
{
	return(m_pData);
}
float3_t * XMETHODCALLTYPE CModelPhysboxConvex::getData()
{
	return(m_pData);
}

void XMETHODCALLTYPE CModelPhysboxConvex::initData(UINT uVertexCount, const float3_t *pData)
{
	mem_delete_a(m_pData);
	m_uVertexCount = uVertexCount;
	m_pData = new float3_t[uVertexCount];

	if(pData)
	{
		memcpy(m_pData, pData, sizeof(float3_t) * uVertexCount);
	}
}

//##########################################################################

XPHYSBOXTYPE XMETHODCALLTYPE CModelPhysboxCylinder::getType() const
{
	return(XPBT_CYLINDER);
}

const IModelPhysboxCylinder* XMETHODCALLTYPE CModelPhysboxCylinder::asCylinder() const
{
	return(this);
}

IModelPhysboxCylinder* XMETHODCALLTYPE CModelPhysboxCylinder::asCylinder()
{
	return(this);
}

float XMETHODCALLTYPE CModelPhysboxCylinder::getRadius() const
{
	return(m_fRadius);
}
void XMETHODCALLTYPE CModelPhysboxCylinder::setRadius(float fRadius)
{
	m_fRadius = fRadius;
}

float XMETHODCALLTYPE CModelPhysboxCylinder::getHeight() const
{
	return(m_fHeight);
}
void XMETHODCALLTYPE CModelPhysboxCylinder::setHeight(float fHeight)
{
	m_fHeight = fHeight;
}

//##########################################################################

XPHYSBOXTYPE XMETHODCALLTYPE CModelPhysboxCapsule::getType() const
{
	return(XPBT_CAPSULE);
}

const IModelPhysboxCapsule* XMETHODCALLTYPE CModelPhysboxCapsule::asCapsule() const
{
	return(this);
}

IModelPhysboxCapsule* XMETHODCALLTYPE CModelPhysboxCapsule::asCapsule()
{
	return(this);
}

float XMETHODCALLTYPE CModelPhysboxCapsule::getRadius() const
{
	return(m_fRadius);
}
void XMETHODCALLTYPE CModelPhysboxCapsule::setRadius(float fRadius)
{
	m_fRadius = fRadius;
}

float XMETHODCALLTYPE CModelPhysboxCapsule::getHeight() const
{
	return(m_fHeight);
}
void XMETHODCALLTYPE CModelPhysboxCapsule::setHeight(float fHeight)
{
	m_fHeight = fHeight;
}
