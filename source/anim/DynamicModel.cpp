#include "DynamicModel.h"
#include "DynamicModelProvider.h"

CDynamicModel::CDynamicModel(CDynamicModelProvider *pProvider, CDynamicModelShared *pShared):
	m_pProvider(pProvider),
	m_pShared(pShared),
	m_pDevice(pProvider->getDevice())
{
	pShared->AddRef();

	if(m_pDevice)
	{
		if(m_pProvider->getCore()->isOnMainThread())
		{
			initGPUresources();
		}
		else
		{
			m_pProvider->scheduleModelGPUinit(this);
		}
	}

	m_pSceneObject = m_pProvider->getSceneObjectType()->newObject(getLocalBound() + getPosition(), this);
	onFeaturesChanged();
}
CDynamicModel::~CDynamicModel()
{
	m_pProvider->notifyModelChanged(this, XEventModelChanged::TYPE_BEFORE_REMOVED);
	m_pProvider->onModelRelease(this);
	mem_release(m_pShared);
	mem_release(m_pWorldBuffer);
	mem_release(m_pSceneObject);
}

void CDynamicModel::initGPUresources()
{
	if(m_pWorldBuffer)
	{
		return;
	}
	m_pWorldBuffer = m_pDevice->createConstantBuffer(sizeof(SMMATRIX));

	m_pProvider->notifyModelChanged(this, XEventModelChanged::TYPE_CREATED);
}

bool XMETHODCALLTYPE CDynamicModel::isEnabled() const
{
	return(m_isEnabled);
}
void XMETHODCALLTYPE CDynamicModel::enable(bool yesNo)
{
	if(m_isEnabled == yesNo)
	{
		return;
	}
	m_isEnabled = yesNo;

	if(m_isEnabled && !m_pSceneObject)
	{
		m_pSceneObject = m_pProvider->getSceneObjectType()->newObject(getLocalBound() + getPosition(), this);
		onFeaturesChanged();
	}
	if(!m_isEnabled)
	{
		mem_release(m_pSceneObject);
	}

	m_pProvider->notifyModelChanged(this, XEventModelChanged::TYPE_VISIBILITY);
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
	if((float3)m_vPosition == vPos)
	{
		return;
	}
	m_vPosition = vPos;
	m_isWorldDirty = true;

	if(m_isEnabled)
	{
		m_pProvider->notifyModelChanged(this, XEventModelChanged::TYPE_MOVED);

		m_pSceneObject->update(getLocalBound() + getPosition());
	}
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
	m_isWorldDirty = true;

	if(m_isEnabled)
	{
		m_pProvider->notifyModelChanged(this, XEventModelChanged::TYPE_MOVED);

		m_pSceneObject->update(getLocalBound() + getPosition());
	}
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
	m_isWorldDirty = true;

	if(m_isEnabled)
	{
		m_pProvider->notifyModelChanged(this, XEventModelChanged::TYPE_MOVED);

		m_pSceneObject->update(getLocalBound() + getPosition());
	}
}

UINT XMETHODCALLTYPE CDynamicModel::getSkin() const
{
	return(m_uSkin);
}
void XMETHODCALLTYPE CDynamicModel::setSkin(UINT uSkin)
{
	if(m_uSkin == uSkin)
	{
		return;
	}

	m_uSkin = uSkin;

	m_pProvider->notifyModelChanged(this, XEventModelChanged::TYPE_SKIN);

	onFeaturesChanged();
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
SMAABB XMETHODCALLTYPE CDynamicModel::getLocalBound() const
{
	_updateAABB();

	return(SMAABB(m_vLocalMin, m_vLocalMax));
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
const IModelPhysbox* XMETHODCALLTYPE CDynamicModel::getPhysBox(UINT id, UINT uPartIndex) const
{
	assert(uPartIndex == 0);

	return(m_pShared->getPhysBox(id));
}
const IXResourceModel* XMETHODCALLTYPE CDynamicModel::getResource(UINT uIndex)
{
	assert(uIndex == 0);

	return(m_pShared->getResource());
}

void XMETHODCALLTYPE CDynamicModel::render(UINT uLod, XMODEL_FEATURE bmFeatures)
{
	if(!m_pDevice || !m_isEnabled || !m_pWorldBuffer)
	{
		return;
	}

	if(m_pShared->isInstancing())
	{
		m_pShared->renderInstanced(m_vPosition, m_qRotation, m_fScale, m_vColor);
	}
	else
	{
		if(m_isWorldDirty)
		{
			m_pWorldBuffer->update(&SMMatrixTranspose(SMMatrixScaling(m_fScale) * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition)));
			m_isWorldDirty = false;
		}

		m_pProvider->bindVertexFormat();

		m_pDevice->getThreadContext()->setVSConstant(m_pWorldBuffer, 1 /* SCR_OBJECT */);
		m_pShared->render(m_uSkin, uLod, m_vColor, bmFeatures);
	}
}

CDynamicModelShared* CDynamicModel::getShared()
{
	return(m_pShared);
}

UINT CDynamicModel::getPSPcount(UINT uLod) const
{
	return(m_pShared->getPSPs(m_uSkin, uLod).size());
}
SMPLANE CDynamicModel::getPSP(UINT uLod, UINT uPlane) const
{
	auto &aPlanes = m_pShared->getPSPs(m_uSkin, uLod);
	assert(uPlane < aPlanes.size());

	SMPLANE plane = aPlanes[uPlane];

	plane = SMPlaneTransform(plane, SMMatrixScaling(m_fScale) * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition));

	return(plane);
}

XMODEL_FEATURE CDynamicModel::getFeatures(UINT uLod) const
{
	return(m_pShared->getFeatures(m_uSkin, uLod));
}

IXMaterial* CDynamicModel::getTransparentMaterial(UINT uLod)
{
	return(m_pShared->getTransparentMaterial(m_uSkin, uLod));
}

void CDynamicModel::onFeaturesChanged()
{
	if(m_pSceneObject)
	{
		XMODEL_FEATURE bmFeatures = getFeatures(0);

		IXSceneFeature *ppFeatures[4] = {0};
		int i = 0;

		if(bmFeatures & MF_OPAQUE)
		{
			ppFeatures[i++] = m_pProvider->getFeature(MF_OPAQUE);
		}
		if(bmFeatures & MF_TRANSPARENT)
		{
			ppFeatures[i++] = m_pProvider->getFeature(MF_TRANSPARENT);
		}
		if(bmFeatures & MF_SELFILLUM)
		{
			ppFeatures[i++] = m_pProvider->getFeature(MF_SELFILLUM);
		}

		assert(i < ARRAYSIZE(ppFeatures));

		m_pSceneObject->setFeatures(ppFeatures);
	}
}

void XMETHODCALLTYPE CDynamicModel::FinalRelease()
{
	m_pProvider->enqueueModelDelete(this);
}
