#include "AnimatedModel.h"
#include "AnimatedModelProvider.h"

CAnimatedModel::CAnimatedModel(CAnimatedModelProvider *pProvider, CAnimatedModelShared *pShared):
	m_pProvider(pProvider),
	m_pShared(pShared)
{
	pShared->AddRef();
}
CAnimatedModel::~CAnimatedModel()
{
	m_pProvider->onModelRelease(this);
	mem_release(m_pShared);
}

IXAnimatedModel *CAnimatedModel::asAnimatedModel()
{
	return(this);
}
IXDynamicModel *CAnimatedModel::asDynamicModel()
{
	return(NULL);
}
IXStaticModel *CAnimatedModel::asStaticModel()
{
	return(NULL);
}

float3 CAnimatedModel::getPosition() const
{
	return(m_vPosition);
}
void CAnimatedModel::setPosition(const float3 &vPos)
{
	m_vPosition = vPos;
}

SMQuaternion CAnimatedModel::getOrientation() const
{
	return(m_qRotation);
}
void CAnimatedModel::setOrientation(const SMQuaternion &qRot)
{
	m_qRotation = qRot;
}

UINT CAnimatedModel::getSkin() const
{
	return(m_uSkin);
}
void CAnimatedModel::setSkin(UINT uSkin)
{
	m_uSkin = uSkin;
}

float3 CAnimatedModel::getLocalBoundMin() const
{
	//@TODO: Implement me
	return(0);
}
float3 CAnimatedModel::getLocalBoundMax() const
{
	//@TODO: Implement me
	return(0);
}

float4 CAnimatedModel::getColor() const
{
	return(m_vColor);
}
void CAnimatedModel::setColor(const float4 &vColor)
{
	m_vColor = vColor;
}

UINT CAnimatedModel::getPhysboxCount(UINT uPartIndex) const
{
	return(m_pShared->getPhysboxCount(uPartIndex));
}
const IModelPhysbox *CAnimatedModel::getPhysBox(UINT id, UINT uPartIndex) const
{
	return(m_pShared->getPhysBox(id, uPartIndex));
}
const IXResourceModel *CAnimatedModel::getResource(UINT uIndex)
{
	return(m_pShared->getResource(uIndex));
}



UINT CAnimatedModel::getPartsCount() const
{
	return(m_pShared->getPartsCount());
}
const char *CAnimatedModel::getPartName(UINT uIndex) const
{
	return(m_pShared->getPartName(uIndex));
}
UINT CAnimatedModel::getPartIndex(const char *szName)
{
	return(m_pShared->getPartIndex(szName));
}
XMODEL_PART_FLAGS CAnimatedModel::getPartFlags(UINT uIndex) const
{
	return(m_pShared->getPartFlags(uIndex));
}
bool CAnimatedModel::isPartEnabled(UINT uIndex) const
{
	//@TODO: Implement me
	return(false);
}
void CAnimatedModel::enablePart(UINT uIndex, bool yesNo)
{
	//@TODO: Implement me
}

UINT CAnimatedModel::getHitboxCount(UINT uPartIndex) const
{
	return(m_pShared->getHitboxCount(uPartIndex));
}
const XResourceModelHitbox *CAnimatedModel::getHitbox(UINT id, UINT uPartIndex) const
{
	return(m_pShared->getHitbox(id, uPartIndex));
}

void CAnimatedModel::play(const char *szName, UINT uFadeTime, UINT uSlot, bool bReplaceActivity)
{
	//@TODO: Implement me
}
void CAnimatedModel::stop(UINT uSlot)
{
	//@TODO: Implement me
}
void CAnimatedModel::resume(UINT uSlot)
{
	//@TODO: Implement me
}
void CAnimatedModel::setProgress(float fProgress, UINT uSlot)
{
	//@TODO: Implement me
}
void CAnimatedModel::startActivity(const char *szName, UINT uFadeTime, UINT uSlot)
{
	//@TODO: Implement me
}
void CAnimatedModel::stopAll()
{
	//@TODO: Implement me
}

float3 CAnimatedModel::getBoneTransformPos(UINT id)
{
	//@TODO: Implement me
	return(0);
}
SMQuaternion CAnimatedModel::getBoneTransformRot(UINT id)
{
	//@TODO: Implement me
	return(SMQuaternion());
}
SMMATRIX CAnimatedModel::getBoneTransform(UINT id)
{
	//@TODO: Implement me
	return(SMMatrixIdentity());
}

UINT CAnimatedModel::getBoneId(const char *szName)
{
	return(m_pShared->getBoneId(szName));
}
UINT CAnimatedModel::getBoneCount() const
{
	return(m_pShared->getBoneCount());
}
const char *CAnimatedModel::getBoneName(UINT id) const
{
	return(m_pShared->getBoneName(id));
}

bool CAnimatedModel::isPlayingAnimations()
{
	//@TODO: Implement me
	return(false);
}
bool CAnimatedModel::isPlayingAnimation(const char *szName)
{
	//@TODO: Implement me
	return(false);
}

void CAnimatedModel::setController(UINT id, float fValue)
{
	//@TODO: Implement me
}

UINT CAnimatedModel::getControllersCount() const
{
	return(m_pShared->getControllersCount());
}
const char *CAnimatedModel::getControllerName(UINT id)
{
	return(m_pShared->getControllerName(id));
}
UINT CAnimatedModel::getControllerId(const char *szName)
{
	return(m_pShared->getControllerId(szName));
}

void CAnimatedModel::setCallback(IAnimationCallback *pCallback)
{
	//@TODO: Implement me
}
