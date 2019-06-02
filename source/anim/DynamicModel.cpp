#include "DynamicModel.h"
#include "DynamicModelProvider.h"

CDynamicModel::CDynamicModel(CDynamicModelProvider *pProvider, CDynamicModelShared *pShared):
	m_pProvider(pProvider),
	m_pShared(pShared),
	m_pDevice(pProvider->getDevice())
{
	pShared->AddRef();


}
CDynamicModel::~CDynamicModel()
{
	m_pProvider->onModelRelease(this);
	mem_release(m_pShared);
}

bool XMETHODCALLTYPE CDynamicModel::isEnabled() const
{
	return(m_isEnabled);
}
void XMETHODCALLTYPE CDynamicModel::enable(bool yesNo)
{
	m_isEnabled = yesNo;
}

IXAnimatedModel * XMETHODCALLTYPE CDynamicModel::asAnimatedModel()
{
	return(NULL);
}
IXDynamicModel * XMETHODCALLTYPE CDynamicModel::asDynamicModel()
{
	return(this);
}
IXStaticModel * XMETHODCALLTYPE CDynamicModel::asStaticModel()
{
	return(NULL);
}

float3 XMETHODCALLTYPE CDynamicModel::getPosition() const
{
	return(m_vPosition);
}
void XMETHODCALLTYPE CDynamicModel::setPosition(const float3 &vPos)
{
	m_vPosition = vPos;
}

SMQuaternion XMETHODCALLTYPE CDynamicModel::getOrientation() const
{
	return(m_qRotation);
}
void XMETHODCALLTYPE CDynamicModel::setOrientation(const SMQuaternion &qRot)
{
	if(m_qRotation == qRot)
	{
		return;
	}
	m_qRotation = qRot;
	m_isLocalAABBvalid = false;
}

float XMETHODCALLTYPE CDynamicModel::getScale() const
{
	return(m_fScale);
}
void XMETHODCALLTYPE CDynamicModel::setScale(float fScale)
{
	if(m_fScale == fScale)
	{
		return;
	}
	m_fScale = fScale;
	m_isLocalAABBvalid = false;
}

UINT XMETHODCALLTYPE CDynamicModel::getSkin() const
{
	return(m_uSkin);
}
void XMETHODCALLTYPE CDynamicModel::setSkin(UINT uSkin)
{
	m_uSkin = uSkin;
}

float3 XMETHODCALLTYPE CDynamicModel::getLocalBoundMin() const
{
	_updateAABB();

	return(m_vLocalMin);
}
float3 XMETHODCALLTYPE CDynamicModel::getLocalBoundMax() const
{
	_updateAABB();

	return(m_vLocalMax);
}

void CDynamicModel::_updateAABB() const
{
	if(m_isLocalAABBvalid)
	{
		return;
	}

	float3 vMin = m_pShared->getLocalBoundMin() * m_fScale;
	float3 vMax = m_pShared->getLocalBoundMax() * m_fScale;

	float3 vCurrent[] = {
		m_qRotation * float3(vMin.x, vMin.y, vMin.z),
		m_qRotation * float3(vMin.x, vMin.y, vMax.z),
		m_qRotation * float3(vMin.x, vMax.y, vMin.z),
		m_qRotation * float3(vMin.x, vMax.y, vMax.z),
		m_qRotation * float3(vMax.x, vMin.y, vMin.z),
		m_qRotation * float3(vMax.x, vMin.y, vMax.z),
		m_qRotation * float3(vMax.x, vMax.y, vMin.z),
		m_qRotation * float3(vMax.x, vMax.y, vMax.z)
	};
	
	m_vLocalMin = (float3)SMVectorMin(
		SMVectorMin(SMVectorMin(vCurrent[0], vCurrent[1]), SMVectorMin(vCurrent[2], vCurrent[3])),
		SMVectorMin(SMVectorMin(vCurrent[4], vCurrent[5]), SMVectorMin(vCurrent[6], vCurrent[7]))
		);
	m_vLocalMax = (float3)SMVectorMax(
		SMVectorMax(SMVectorMax(vCurrent[0], vCurrent[1]), SMVectorMax(vCurrent[2], vCurrent[3])),
		SMVectorMax(SMVectorMax(vCurrent[4], vCurrent[5]), SMVectorMax(vCurrent[6], vCurrent[7]))
		);

	m_isLocalAABBvalid = true;
}

float4 XMETHODCALLTYPE CDynamicModel::getColor() const
{
	return(m_vColor);
}
void XMETHODCALLTYPE CDynamicModel::setColor(const float4 &vColor)
{
	m_vColor = vColor;
}

UINT XMETHODCALLTYPE CDynamicModel::getPhysboxCount(UINT uPartIndex) const
{
	assert(uPartIndex == 0);

	return(m_pShared->getPhysboxCount());
}
const IModelPhysbox * XMETHODCALLTYPE CDynamicModel::getPhysBox(UINT id, UINT uPartIndex) const
{
	assert(uPartIndex == 0);

	return(m_pShared->getPhysBox(id));
}
const IXResourceModel * XMETHODCALLTYPE CDynamicModel::getResource(UINT uIndex)
{
	assert(uIndex == 0);

	return(m_pShared->getResource());
}

void CDynamicModel::render(UINT uLod)
{
	if(!m_pDevice || !m_isEnabled)
	{
		return;
	}

	m_pShared->render(SMMatrixScaling(m_fScale) * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition), m_uSkin, uLod, m_vColor);
}
