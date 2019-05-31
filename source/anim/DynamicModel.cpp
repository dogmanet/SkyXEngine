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
	m_qRotation = qRot;
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
	return(m_pShared->getLocalBoundMin());
}
float3 XMETHODCALLTYPE CDynamicModel::getLocalBoundMax() const
{
	return(m_pShared->getLocalBoundMax());
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

	m_pShared->render(m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition), m_uSkin, uLod, m_vColor);
}
