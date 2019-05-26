#include "AnimatedModelProvider.h"
#include <xcommon/IPluginManager.h>

#if 1
#include <gcore/sxgcore.h>
#endif

CAnimatedModelProvider::CAnimatedModelProvider(IXCore *pCore):
	m_pCore(pCore)
{
	GXVERTEXELEMENT layoutDynamicEx[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		{0, 20, GXDECLTYPE_FLOAT3, GXDECLUSAGE_NORMAL},
		{0, 32, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TANGENT},
		{0, 44, GXDECLTYPE_FLOAT3, GXDECLUSAGE_BINORMAL},
		{0, 56, GXDECLTYPE_UBYTE4, GXDECLUSAGE_BLENDINDICES},
		{0, 60, GXDECLTYPE_FLOAT4, GXDECLUSAGE_BLENDWEIGHT},
		GXDECL_END()
	};

	m_pVertexDeclaration = getDevice()->createVertexDeclaration(layoutDynamicEx);
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
	return(SGCore_GetDXDevice());
}

bool CAnimatedModelProvider::createModel(UINT uResourceCount, const IXResourceModelAnimated **ppResources, IXAnimatedModel **ppModel)
{
	const IXResourceModelAnimated *pMinPtr = ppResources[0];
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

void CAnimatedModelProvider::render()
{
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		m_apModels[i]->render(0);
	}
}
