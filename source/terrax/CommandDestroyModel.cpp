#include "CommandDestroyModel.h"

CCommandDestroyModel::CCommandDestroyModel(CProxyObject *pObject):
	m_pProxy(pObject)
{
	add_ref(m_pProxy);
	
	m_pEntity = pObject->getTargetObject();
	add_ref(m_pEntity);

	IXEditorModel *pModel;
	IXEditorObject *pObj;
	for(UINT i = 0, l = pObject->getModelCount(); i < l; ++i)
	{
		pModel = pObject->getModel(i);
		add_ref(pModel);
		m_aModels.push_back(pModel);
		
		for(UINT j = 0, jl = pModel->getObjectCount(); j < jl; ++j)
		{
			if(pModel->getObject(j, &pObj))
			{
				aObjModels.push_back({pObj, pModel});
			}
		}
	}

	m_pCommandDelete = new CCommandDelete();
	m_pCommandDelete->addObject(m_pEntity);
}

CCommandDestroyModel::~CCommandDestroyModel()
{
	fora(i, aObjModels)
	{
		mem_release(aObjModels[i].pObj);
	}
	fora(i, m_aModels)
	{
		mem_release(m_aModels[i]);
	}
	mem_release(m_pProxy);
	mem_release(m_pEntity);
	mem_release(m_pCommandDelete);
}

bool XMETHODCALLTYPE CCommandDestroyModel::exec()
{
	int idx = g_apProxies.indexOf(m_pProxy);
	assert(idx >= 0);
	if(idx >= 0)
	{
		mem_release(g_apProxies[idx]);
		g_apProxies.erase(idx);
	}

	//m_pProxy->setSelected(false);

	g_pEditor->removeObject(m_pProxy);

	// destroy proxy
	m_pProxy->reset();

	// destroy models
	fora(i, m_aModels)
	{
		IXEditorModel *pMdl = m_aModels[i];
		assert(g_apLevelModels.KeyExists(*pMdl->getGUID()));
		g_apLevelModels.erase(*pMdl->getGUID());
		pMdl->destroy();
		mem_release(pMdl);
		//mem_release(m_aModels[i]);
	}

	m_pCommandDelete->exec();

	return(true);
}
bool XMETHODCALLTYPE CCommandDestroyModel::undo()
{
	m_pCommandDelete->undo();

	fora(i, m_aModels)
	{
		IXEditorModel *pMdl = m_aModels[i];
		assert(!g_apLevelModels.KeyExists(*pMdl->getGUID()));
		g_apLevelModels[*pMdl->getGUID()] = pMdl;
		add_ref(pMdl);
		pMdl->restore();
	}
	fora(i, aObjModels)
	{
		ObjModel &om = aObjModels[i];
		om.pModel->addObject(om.pObj);
	}
	
	fora(i, m_aModels)
	{
		IXEditorModel *pMdl = m_aModels[i];
		m_pProxy->addSrcModel(*pMdl->getGUID());
	}

	bool res = m_pProxy->setDstObject(*m_pEntity->getGUID());
	assert(res);
	m_pProxy->build();

	g_pEditor->addObject(m_pProxy);

	//m_pProxy->setSelected(true);

	add_ref(m_pProxy);
	g_apProxies.push_back(m_pProxy);

	return(true);
}
