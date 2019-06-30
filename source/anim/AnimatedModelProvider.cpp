#include "AnimatedModelProvider.h"
#include <xcommon/IPluginManager.h>

CAnimatedModelProvider::CAnimatedModelProvider(IXCore *pCore):
	m_pCore(pCore)
{
	
}

CAnimatedModelProvider::~CAnimatedModelProvider()
{
	mem_release(m_pVertexDeclaration);
}

IGXVertexDeclaration *CAnimatedModelProvider::getVertexDeclaration()
{
	return(m_pVertexDeclaration);
}

IGXContext *CAnimatedModelProvider::getDevice()
{
	return(m_pRenderContext);
}

void CAnimatedModelProvider::setDevice(IGXContext *pDevice)
{
	m_pRenderContext = pDevice;

	GXVertexElement layoutDynamicEx[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		{0, 20, GXDECLTYPE_FLOAT3, GXDECLUSAGE_NORMAL},
		{0, 32, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TANGENT},
		{0, 44, GXDECLTYPE_FLOAT3, GXDECLUSAGE_BINORMAL},
		{0, 56, GXDECLTYPE_UBYTE4, GXDECLUSAGE_BLENDINDICES},
		{0, 60, GXDECLTYPE_FLOAT4, GXDECLUSAGE_BLENDWEIGHT},
		GX_DECL_END()
	};

	m_pVertexDeclaration = getDevice()->createVertexDeclaration(layoutDynamicEx);
}

bool XMETHODCALLTYPE CAnimatedModelProvider::createModel(UINT uResourceCount, IXResourceModelAnimated **ppResources, IXAnimatedModel **ppModel)
{
	IXResourceModelAnimated *pMinPtr = ppResources[0];
	for(UINT i = 1; i < uResourceCount; ++i)
	{
		if(ppResources[i] < pMinPtr)
		{
			pMinPtr = ppResources[i];
		}
	}

	auto &aModels = m_mModels[pMinPtr];

	CAnimatedModelShared *pShared = NULL;
	for(UINT i = 0, l = aModels.size(); i < l; ++i)
	{
		if(aModels[i]->isSame(uResourceCount, ppResources))
		{
			pShared = aModels[i];
			break;
		}
	}

	if(!pShared)
	{
		pShared = new CAnimatedModelShared(this);
		aModels.push_back(pShared);
		if(!pShared->init(uResourceCount, ppResources))
		{
			mem_delete(pShared);
			return(false);
		}
	}

	CAnimatedModel *pModel = new CAnimatedModel(this, pShared);
	m_apModels.push_back(pModel);

	*ppModel = pModel;
	return(true);
}

void CAnimatedModelProvider::onSharedModelRelease(CAnimatedModelShared *pShared)
{
	auto &aModels = m_mModels[pShared->getResources()[0]];

	for(UINT i = 0, l = aModels.size(); i < l; ++i)
	{
		if(aModels[i] == pShared)
		{
			aModels.erase(i);
			return;
		}
	}

	assert(!"Something wrong! Should never get here!");
}

void CAnimatedModelProvider::onModelRelease(CAnimatedModel *pModel)
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

IXMaterialSystem *CAnimatedModelProvider::getMaterialSystem()
{
	return((IXMaterialSystem*)m_pCore->getPluginManager()->getInterface(IXMATERIALSYSTEM_GUID));
}

void CAnimatedModelProvider::update(float fDT)
{
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		m_apModels[i]->update(fDT);
	}
}

void CAnimatedModelProvider::sync()
{
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		m_apModels[i]->sync();
	}
}

void CAnimatedModelProvider::render(CRenderableVisibility *pVisibility)
{
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		if(pVisibility)
		{
			auto pItem = pVisibility->getItem(i);
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

void CAnimatedModelProvider::computeVisibility(const IFrustum *pFrustum, CRenderableVisibility *pVisibility, CRenderableVisibility *pReference)
{
	pVisibility->setItemCount(m_apModels.size());

	CAnimatedModel *pMdl;
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		pMdl = m_apModels[i];
		if(pMdl->isEnabled())
		{
			float3 vDelta = pMdl->getPosition();
			pVisibility->getItem(i)->isVisible = (pReference ? pReference->getItem(i)->isVisible : true)
				&& pFrustum->boxInFrustum(&float3(pMdl->getLocalBoundMin() + vDelta), &float3(pMdl->getLocalBoundMax() + vDelta));
		}
		else
		{
			pVisibility->getItem(i)->isVisible = false;
		}
	}
}

void CAnimatedModelProvider::getLevelSize(const XEventLevelSize *pData)
{
	CAnimatedModel *pMdl;
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
