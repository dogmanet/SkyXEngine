#include "ModelPhysbox.h"

XPHYSBOXTYPE CModelPhysboxBox::getType() const
{
	return(XPBT_BOX);
}

const IModelPhysboxBox *CModelPhysboxBox::asBox() const
{
	return(this);
}

float3 CModelPhysboxBox::getSize() const
{
	return(m_vSize);
}
void CModelPhysboxBox::setSize(const float3 &vSize)
{
	m_vSize = vSize;
}

//##########################################################################

XPHYSBOXTYPE CModelPhysboxSphere::getType() const
{
	return(XPBT_SPHERE);
}

const IModelPhysboxSphere *CModelPhysboxSphere::asSphere() const
{
	return(this);
}

float CModelPhysboxSphere::getRadius() const
{
	return(m_fRadius);
}
void CModelPhysboxSphere::setRadius(float fRadius)
{
	m_fRadius = fRadius;
}

//##########################################################################

CModelPhysboxConvex::~CModelPhysboxConvex()
{
	mem_delete_a(m_pData);
}

XPHYSBOXTYPE CModelPhysboxConvex::getType() const
{
	return(XPBT_CONVEX);
}

const IModelPhysboxConvex *CModelPhysboxConvex::asConvex() const
{
	return(this);
}

UINT CModelPhysboxConvex::getVertexCount() const
{
	return(m_uVertexCount);
}
const float3_t *CModelPhysboxConvex::getData() const
{
	return(m_pData);
}
float3_t *CModelPhysboxConvex::getData()
{
	return(m_pData);
}

void CModelPhysboxConvex::initData(UINT uVertexCount, const float3_t *pData)
{
	mem_delete_a(m_pData);
	m_uVertexCount = uVertexCount;
	m_pData = new float3_t[uVertexCount];

	if(pData)
	{
		memcpy(m_pData, pData, sizeof(float3_t) * uVertexCount);
	}
}
