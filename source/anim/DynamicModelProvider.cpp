#include "DynamicModelProvider.h"
#include <xcommon/IPluginManager.h>

CDynamicModelProvider::CDynamicModelProvider(IXCore *pCore):
	m_pCore(pCore)
{
}

CDynamicModelProvider::~CDynamicModelProvider()
{
	mem_release(m_pVertexDeclaration);
}

IGXVertexDeclaration *CDynamicModelProvider::getVertexDeclaration()
{
	return(m_pVertexDeclaration);
}

IGXContext *CDynamicModelProvider::getDevice()
{
	return(m_pRenderContext);
}
void CDynamicModelProvider::setDevice(IGXContext *pDevice)
{
	m_pRenderContext = pDevice;


	GXVertexElement layoutStaticEx[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		{0, 20, GXDECLTYPE_FLOAT3, GXDECLUSAGE_NORMAL},
		{0, 32, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TANGENT},
		{0, 44, GXDECLTYPE_FLOAT3, GXDECLUSAGE_BINORMAL},
		GX_DECL_END()
	};

	m_pVertexDeclaration = pDevice->createVertexDeclaration(layoutStaticEx);
}

bool XMETHODCALLTYPE CDynamicModelProvider::createModel(IXResourceModel *pResource, IXDynamicModel **ppModel)
{
	CDynamicModelShared *pShared = NULL;

	const AssotiativeArray<IXResourceModel*, CDynamicModelShared*>::Node *pNode;
	if(m_mModels.KeyExists(pResource, &pNode))
	{
		pShared = *(pNode->Val);
	}

	if(!pShared)
	{
		if(pResource->getType() != XMT_STATIC)
		{
			return(false);
		}

		pShared = new CDynamicModelShared(this);
		if(!pShared->init(pResource->asStatic()))
		{
			mem_delete(pShared);
			return(false);
		}
		m_mModels[pResource] = pShared;
	}

	CDynamicModel *pModel = new CDynamicModel(this, pShared);
	m_apModels.push_back(pModel);

	*ppModel = pModel;
	return(true);
}

void CDynamicModelProvider::onSharedModelRelease(CDynamicModelShared *pShared)
{
	m_mModels[pShared->getResource()] = NULL;
}

void CDynamicModelProvider::onModelRelease(CDynamicModel *pModel)
{
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		if(m_apModels[i] == pModel)
		{
			m_apModels.erase(i);
			return;
		}
	}

	assert(!"Something wrong! Should never get here!");
}

IXMaterialSystem *CDynamicModelProvider::getMaterialSystem()
{
	return((IXMaterialSystem*)m_pCore->getPluginManager()->getInterface(IXMATERIALSYSTEM_GUID));
}

void CDynamicModelProvider::render(CRenderableVisibility *pVisibility)
{
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		if(pVisibility)
		{
			auto pItem = pVisibility->getItemDynamic(i);
			if(pItem->isVisible)
			{
				m_apModels[i]->render(pItem->uLod);
			}
		}
		else
		{
			m_apModels[i]->render(0);
		}
	}
}

void CDynamicModelProvider::computeVisibility(const IFrustum *pFrustum, CRenderableVisibility *pVisibility, CRenderableVisibility *pReference)
{
	pVisibility->setItemCountDynamic(m_apModels.size());

	CDynamicModel *pMdl;
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		pMdl = m_apModels[i];
		if(pMdl->isEnabled())
		{
			float3 vDelta = pMdl->getPosition();
			pVisibility->getItemDynamic(i)->isVisible = (pReference ? pReference->getItemDynamic(i)->isVisible : true)
				&& pFrustum->boxInFrustum(&float3(pMdl->getLocalBoundMin() + vDelta), &float3(pMdl->getLocalBoundMax() + vDelta));
		}
		else
		{
			pVisibility->getItemDynamic(i)->isVisible = false;
		}
	}
}

void CDynamicModelProvider::getLevelSize(const XEventLevelSize *pData)
{
	CDynamicModel *pMdl;
	float3 vMax, vMin;
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		pMdl = m_apModels[i];
		if(pMdl->isEnabled())
		{
			float3 vDelta = pMdl->getPosition();
			vMin = pMdl->getLocalBoundMin() + vDelta;
			vMax = pMdl->getLocalBoundMax() + vDelta;

			if(pData->vMax == pData->vMin)
			{
				pData->vMax = vMax;
				pData->vMin = vMin;
			}
			else
			{
				pData->vMax = SMVectorMax(pData->vMax, vMax);
				pData->vMin = SMVectorMin(pData->vMin, vMin);
			}

		}
	}
}
