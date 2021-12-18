//#include "EditorExtension.h"
//#include "resource.h"
#include <xcommon/resource/IXResourceManager.h>
#include "EditorModel.h"
#include "Editable.h"
#include "EditorObject.h"
//#include "CommandCreate.h"


//##########################################################################

CEditorModel::CEditorModel(CEditable *pEditable, IXCore *pCore):
	m_pEditable(pEditable),
	m_pCore(pCore)
{
	XCreateGUID(&m_guid);

	auto &aObj = m_pEditable->getObjects();
	fora(i, aObj)
	{
		CEditorObject *pObj = aObj[i];
		if(pObj->isSelected())
		{
			assert(!pObj->getModel());

			add_ref(pObj);
			pObj->setModel(this);
			m_aObjects.push_back(pObj);
		}
	}
}
CEditorModel::CEditorModel(CEditable *pEditable, IXCore *pCore, const XGUID &guid):
	m_pEditable(pEditable),
	m_pCore(pCore),
	m_guid(guid)
{
}
CEditorModel::~CEditorModel()
{
	fora(i, m_aObjects)
	{
		assert(m_aObjects[i]->getModel() == this);
		m_aObjects[i]->setModel(NULL);
		mem_release(m_aObjects[i]);
	}

	m_pEditable->onModelDestroy(this);
}

const XGUID* XMETHODCALLTYPE CEditorModel::getGUID()
{
	return(&m_guid);
}

void XMETHODCALLTYPE CEditorModel::getResource(IXResourceModel **ppOut)
{
	IXResourceManager *pRM = m_pCore->getResourceManager();
	IXResourceModelStatic *pResource = pRM->newResourceModelStatic();

	CMeshBuilder meshBuilder;
	fora(i, m_aObjects)
	{
		m_aObjects[i]->buildMesh(&meshBuilder);
		m_aObjects[i]->buildPhysbox(pResource);
	}

	meshBuilder.buildResource(pResource);

	*ppOut = pResource;
}

UINT XMETHODCALLTYPE CEditorModel::getObjectCount()
{
	return(m_aObjects.size());
}
bool XMETHODCALLTYPE CEditorModel::getObject(UINT id, IXEditorObject **ppObj)
{
	if(id >= m_aObjects.size())
	{
		return(false);
	}

	*ppObj = m_aObjects[id];
	add_ref(*ppObj);

	return(true);
}

void XMETHODCALLTYPE CEditorModel::addObject(IXEditorObject *pObj)
{
	auto &aObj = m_pEditable->getObjects();
	int idx = aObj.indexOf(pObj, [](const CEditorObject *a, const IXEditorObject *b){
		return(a == b);
	});
	assert(idx >= 0);
	if(idx >= 0)
	{
		CEditorObject *pObj = aObj[idx];

		assert(!pObj->getModel());

		add_ref(pObj);
		pObj->setModel(this);
		m_aObjects.push_back(pObj);

		SAFE_CALL(m_pCallback, onChanged, this, NULL);
	}
}
void XMETHODCALLTYPE CEditorModel::removeObject(IXEditorObject *pObj)
{
	int idx = m_aObjects.indexOf(pObj, [](const CEditorObject *a, const IXEditorObject *b){
		return(a == b);
	});
	assert(idx >= 0);
	if(idx >= 0)
	{
		assert(m_aObjects[idx]->getModel() == this);
		m_aObjects[idx]->setModel(NULL);
		mem_release(m_aObjects[idx]);
		m_aObjects.erase(idx);

		SAFE_CALL(m_pCallback, onChanged, this, NULL);
	}
}

void XMETHODCALLTYPE CEditorModel::setCallback(IXEditorModelCallback *pCallback)
{
	m_pCallback = pCallback;
}

void XMETHODCALLTYPE CEditorModel::destroy()
{
	fora(i, m_aObjects)
	{
		assert(m_aObjects[i]->getModel() == this);
		m_aObjects[i]->setModel(NULL);
		mem_release(m_aObjects[i]);
	}
	m_aObjects.clearFast();

	SAFE_CALL(m_pCallback, onChanged, this, NULL);
	m_pCallback = NULL;

	m_pEditable->onModelDestroy(this);
}

void XMETHODCALLTYPE CEditorModel::restore()
{
	m_pEditable->onModelRestored(this);
}

const char* XMETHODCALLTYPE CEditorModel::getTypeName()
{
	return(m_pEditable->getName());
}

void CEditorModel::onObjectChanged(CEditorObject *pObj)
{
	SAFE_CALL(m_pCallback, onChanged, this, pObj);
}
void CEditorModel::onObjectAdded(CEditorObject *pObj)
{
	SAFE_CALL(m_pCallback, onObjectAdded, this, pObj);
}
void CEditorModel::onObjectRemoved(CEditorObject *pObj)
{
	SAFE_CALL(m_pCallback, onObjectRemoved, this, pObj);
}
