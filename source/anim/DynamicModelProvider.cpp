#include "DynamicModelProvider.h"
#include <xcommon/IPluginManager.h>
#include <chrono>

/*
#if 1
#	include <core/sxcore.h>
#	ifdef _DEBUG
#		pragma comment(lib, "sxcore_d.lib")
#	else
#		pragma comment(lib, "sxcore.lib")
#	endif
#endif
*/

CMaterialChangedEventListener::CMaterialChangedEventListener(CDynamicModelProvider *pProvider):
	m_pProvider(pProvider)
{
}
void CMaterialChangedEventListener::onEvent(const XEventMaterialChanged *pData)
{
	if(pData->type == XEventMaterialChanged::TYPE_TRANSPARENCY)
	{
		m_pProvider->onMaterialTransparencyChanged(pData->pMaterial);
	}
}
/*
//##########################################################################

class CRenderTask: public IParallelForBody
{
public:
	CRenderTask(CRenderableVisibility *pVisibility, Array<CDynamicModel*> *pModels, IGXDevice *pDevice, IGXContextState *pState):
		m_pVisibility(pVisibility),
		m_pModels(pModels),
		m_pDevice(pDevice),
		m_pState(pState)
	{
	}

	void forLoop(int iStart, int iEnd) const
	{
		//printf("%d %d\n", iStart, iEnd);
		//printf("%d\n", Core_MGetThreadID());
		m_pDevice->getThreadContext()->setFullState(m_pState);

		m_pDevice->getThreadContext()->beginIndirect();

		for(UINT i = iStart, l = iEnd; i < l; ++i)
		{
			//if(iStart != 279 && iStart != 558 && iStart != 837)
			//{
			//	break;
			//}
			//if(iStart == 558)
			//{
			//	//break;
			//}
			//if(iStart == 837)
			//{
			//	//break;
			//}
			if(m_pVisibility)
			{
				auto pItem = m_pVisibility->getItemDynamic(i, false);
				if(pItem && pItem->isVisible)
				{
					(*m_pModels)[i]->render(pItem->uLod, false);
					//break;
				}
			}
			else
			{
				(*m_pModels)[i]->render(0, false);
			}
		}

		//Sleep(2);
		m_pDevice->getThreadContext()->endIndirect();
	};

protected:
	CRenderableVisibility *m_pVisibility;
	Array<CDynamicModel*> *m_pModels;
	IGXDevice *m_pDevice;
	IGXContextState *m_pState;
};
*/

//##########################################################################

CDynamicModelProvider::CDynamicModelProvider(IXCore *pCore):
	m_pCore(pCore)
{
	m_pMaterialChangedEventListener = new CMaterialChangedEventListener(this);
	pCore->getEventChannel<XEventMaterialChanged>(EVENT_MATERIAL_CHANGED_GUID)->addListener(m_pMaterialChangedEventListener);
	m_pModelChangedEventChannel = pCore->getEventChannel<XEventModelChanged>(EVENT_MODEL_CHANGED_GUID);
}

CDynamicModelProvider::~CDynamicModelProvider()
{
	m_pCore->getEventChannel<XEventMaterialChanged>(EVENT_MATERIAL_CHANGED_GUID)->removeListener(m_pMaterialChangedEventListener);
	mem_delete(m_pMaterialChangedEventListener);
	mem_release(m_pVertexDeclaration);
}

IGXVertexDeclaration *CDynamicModelProvider::getVertexDeclaration()
{
	return(m_pVertexDeclaration);
}

IGXDevice* CDynamicModelProvider::getDevice()
{
	return(m_pRenderContext);
}

IXCore* CDynamicModelProvider::getCore()
{
	return(m_pCore);
}

void CDynamicModelProvider::setDevice(IGXDevice *pDevice)
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

	m_pMaterialSystem = (IXMaterialSystem*)m_pCore->getPluginManager()->getInterface(IXMATERIALSYSTEM_GUID);
	XVertexFormatHandler *pFormat = m_pMaterialSystem->getVertexFormat("xSceneGeneric");
	m_pVertexShaderHandler = m_pMaterialSystem->registerVertexShader(pFormat, "base/static.vs");
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
	else
	{
		pShared->AddRef();
	}

	CDynamicModel *pModel = new CDynamicModel(this, pShared);
	m_apModels.push_back(pModel);

	pShared->Release();

	*ppModel = pModel;
	return(true);
}

void CDynamicModelProvider::onSharedModelReady(CDynamicModelShared *pShared)
{
	// CDynamicModel *pTmp;
	// for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	// {
	// 	pTmp = m_apModels[i];
	// 	if(pTmp->getShared() == pShared)
	// 	{
	// 		notifyModelCreated(pTmp);
	// 	}
	// }
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

	assert(!"Something went wrong! Should never get here!");
}

IXMaterialSystem *CDynamicModelProvider::getMaterialSystem()
{
	return((IXMaterialSystem*)m_pCore->getPluginManager()->getInterface(IXMATERIALSYSTEM_GUID));
}

void CDynamicModelProvider::bindVertexFormat()
{
	m_pMaterialSystem->bindVS(m_pVertexShaderHandler);
}

void CDynamicModelProvider::render(bool isTransparent, CRenderableVisibility *pVisibility)
{
	bindVertexFormat();
	//if(isTransparent/* || m_apModels.size() < 1000*/)
	//{
		for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
		{
			if(pVisibility)
			{
				auto pItem = pVisibility->getItemDynamic(i);
				if(pItem->isVisible)
				{
					if(isTransparent && !pItem->isTransparent)
					{
						continue;
					}
					m_apModels[i]->render(pItem->uLod, isTransparent);
				}
			}
			else
			{
				m_apModels[i]->render(0, isTransparent);
			}
		}
	//}
	/*else
	{
		IGXContextState *pState = m_pRenderContext->getThreadContext()->getCurrentState();
		CRenderTask task(pVisibility, &m_apModels, m_pRenderContext, pState);

		int iThreads = Core_MGetThreadCount();
		int iChunkSize = ((int)m_apModels.size() + (iThreads - 1)) / iThreads;

		ID idTask = Core_MForLoop(iChunkSize, m_apModels.size(), &task);

		for(UINT i = 0, l = iChunkSize; i < l; ++i)
		{
			if(pVisibility)
			{
				auto pItem = pVisibility->getItemDynamic(i);
				if(pItem->isVisible)
				{
					if(isTransparent && !pItem->isTransparent)
					{
						continue;
					}
					m_apModels[i]->render(pItem->uLod, isTransparent);
				}
			}
			else
			{
				m_apModels[i]->render(0, isTransparent);
			}
		}
		
		Core_MWaitFor(idTask);

		mem_release(pState);

		m_pRenderContext->executeThreadContexts();
		//printf("\n");
	}*/
	m_pMaterialSystem->bindVS(NULL);
}

void CDynamicModelProvider::computeVisibility(const IFrustum *pFrustum, CRenderableVisibility *pVisibility, CRenderableVisibility *pReference)
{
	pVisibility->setItemCountDynamic(m_apModels.size());
	pVisibility->resetItemTransparentDynamic();

	CDynamicModel *pMdl;
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		pMdl = m_apModels[i];
		auto *pItem = pVisibility->getItemDynamic(i);
		if(pMdl->isEnabled())
		{
			float3 vDelta = pMdl->getPosition();
			pItem->isVisible = (pReference ? pReference->getItemDynamic(i)->isVisible : true)
				&& pFrustum->boxInFrustum(&float3(pMdl->getLocalBoundMin() + vDelta), &float3(pMdl->getLocalBoundMax() + vDelta));

			if(pItem->isVisible)
			{
				pItem->isTransparent = pMdl->hasTransparentSubsets(pItem->uLod);
				IXMaterial *pMaterial = pMdl->getTransparentMaterial(pItem->uLod);
				if(pItem->isTransparent)
				{
					UINT uPSPcount = pMdl->getPSPcount(pItem->uLod);
					if(uPSPcount)
					{
						pVisibility->addItemTransparentDynamic({pMdl, true, pMdl->getPSP(pItem->uLod, 0), 0, pMaterial});
					}
					else
					{
						pVisibility->addItemTransparentDynamic({pMdl, false, SMPLANE(), 0, pMaterial});
					}
				}
				mem_release(pMaterial);
			}
		}
		else
		{
			pItem->isVisible = false;
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

void CDynamicModelProvider::update()
{
	typedef std::chrono::high_resolution_clock::time_point time_point;
	time_point tStart = std::chrono::high_resolution_clock::now();

	CDynamicModelShared *pShared;
	while(m_queueGPUinitShared.tryPop(pShared))
	{
		pShared->initGPUresources();
		mem_release(pShared);

		if(((float)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - tStart).count() / 1000000.0f) > 0.0001f)
		{
			break;
		}
	}

	CDynamicModel *pModel;
	while(m_queueGPUinitModel.tryPop(pModel))
	{
		pModel->initGPUresources();
		mem_release(pModel);

		if(((float)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - tStart).count() / 1000000.0f) > 0.0001f)
		{
			break;
		}
	}
}

void CDynamicModelProvider::scheduleSharedGPUinit(CDynamicModelShared *pShared)
{
	pShared->AddRef();
	m_queueGPUinitShared.push(pShared);
}

void CDynamicModelProvider::scheduleModelGPUinit(CDynamicModel *pModel)
{
	pModel->AddRef();
	m_queueGPUinitModel.push(pModel);
}

void CDynamicModelProvider::onMaterialTransparencyChanged(const IXMaterial *pMaterial)
{
	for(auto i = m_mModels.begin(); i; i++)
	{
		if(*i.second)
		{
			(*i.second)->onMaterialTransparencyChanged(pMaterial);
		}
	}
}

UINT CDynamicModelProvider::getTransparentCount(CRenderableVisibility *pVisibility)
{
	return(pVisibility->getItemTransparentDynamicCount());
}
void CDynamicModelProvider::getTransparentObject(CRenderableVisibility *pVisibility, UINT uIndex, XTransparentObject *pObject)
{
	CRenderableVisibility::TransparentModel *pMdl = pVisibility->getItemTransparentDynamic(uIndex);
	pObject->hasPSP = pMdl->hasPSP;
	if(pMdl->hasPSP)
	{
		pObject->psp = pMdl->psp;
	}
	pObject->pMaterial = pMdl->pMaterial;
	//! @fixme use transparent local bound
	pObject->vMin = pMdl->pReferenceMdl->getLocalBoundMin() + pMdl->pReferenceMdl->getPosition();
	pObject->vMax = pMdl->pReferenceMdl->getLocalBoundMax() + pMdl->pReferenceMdl->getPosition();
}
void CDynamicModelProvider::renderTransparentObject(CRenderableVisibility *pVisibility, UINT uIndex, UINT uSplitPlanes)
{
	m_pMaterialSystem->bindVS(m_pVertexShaderHandler);
	CRenderableVisibility::TransparentModel *pMdl = pVisibility->getItemTransparentDynamic(uIndex);
	pMdl->pReferenceMdl->render(pMdl->uLod, true);
}

void CDynamicModelProvider::notifyModelChanged(CDynamicModel *pModel, XEventModelChanged::TYPE type)
{
	XEventModelChanged ev;
	ev.type = type;
	ev.pModel = pModel;
	m_pModelChangedEventChannel->broadcastEvent(&ev);
}
