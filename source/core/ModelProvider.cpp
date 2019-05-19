#include "ModelProvider.h"

CModelProvider::CModelProvider(CCore *pCore):
	m_pCore(pCore)
{
}

bool CModelProvider::createStaticModel(const IXResourceModel *pResource, IXStaticModel **ppModel)
{
	if(pResource->getType() == XMT_STATIC)
	{
		IXStaticModelProvider *pProvider = (IXStaticModelProvider*)m_pCore->getPluginManager()->getInterface(IXSTATICMODELPROVIDER_GUID);
		if(pProvider)
		{
			return(pProvider->createModel(pResource->asStatic(), ppModel));
		}
	}

	IXDynamicModelProvider *pProvider = (IXDynamicModelProvider*)m_pCore->getPluginManager()->getInterface(IXDYNAMICMODELPROVIDER_GUID);
	if(pProvider)
	{
		IXDynamicModel *pModel = NULL;
		bool result = pProvider->createModel(pResource->asStatic(), &pModel);
		*ppModel = pModel;
		return(result);
	}

	return(false);
}

bool CModelProvider::createDynamicModel(const IXResourceModel *pResource, IXDynamicModel **ppModel)
{
	if(pResource->getType() == XMT_ANIMATED)
	{
		IXAnimatedModelProvider *pProvider = (IXAnimatedModelProvider*)m_pCore->getPluginManager()->getInterface(IXANIMATEDMODELPROVIDER_GUID);
		if(pProvider)
		{
			IXAnimatedModel *pModel = NULL;
			const IXResourceModelAnimated *pResourceAnimated = pResource->asAnimated();
			bool result = pProvider->createModel(1, &pResourceAnimated, &pModel);
			*ppModel = pModel;
			return(result);
		}
	}

	IXDynamicModelProvider *pProvider = (IXDynamicModelProvider*)m_pCore->getPluginManager()->getInterface(IXDYNAMICMODELPROVIDER_GUID);
	if(pProvider)
	{
		IXDynamicModel *pModel = NULL;
		bool result = pProvider->createModel(pResource->asStatic(), &pModel);
		*ppModel = pModel;
		return(result);
	}

	return(false);
}
