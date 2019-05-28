#include "ResourceModel.h"
#include "ResourceModelStatic.h"
#include "ResourceModelAnimated.h"
#include "ResourceManager.h"
#include "ModelPhysbox.h"

CResourceModel::CResourceModel(CResourceManager *pMgr):
	m_pManager(pMgr)
{
}

CResourceModel::~CResourceModel()
{
	for(UINT i = 0, l = m_aPhysBoxes.size(); i < l; ++i)
	{
		mem_release(m_aPhysBoxes[i].pPhysbox);
	}

	for(UINT i = 0, l = m_asGibs.size(); i < l; ++i)
	{
		mem_release(m_asGibs[i]);
	}
}

void XMETHODCALLTYPE CResourceModel::Release()
{
	if(m_uRefCount == 1)
	{
		m_pManager->onResourceModelRelease(this);
	}
	IXResourceModel::Release();
}

void CResourceModel::setFileName(const char *szFilename)
{
	m_szFileName = szFilename;
}

const char *CResourceModel::getFileName() const
{
	return(m_szFileName);
}

UINT XMETHODCALLTYPE CResourceModel::getPhysboxCount() const
{
	return(m_aPhysBoxes.size());
}
const IModelPhysbox * XMETHODCALLTYPE CResourceModel::getPhysbox(UINT uPart) const
{
	assert(uPart < m_aPhysBoxes.size());

	return(m_aPhysBoxes[uPart].pPhysbox);
}
int XMETHODCALLTYPE CResourceModel::getPhysboxBone(UINT uPart) const
{
	assert(uPart < m_aPhysBoxes.size());

	return(m_aPhysBoxes[uPart].iBone);
}
void XMETHODCALLTYPE CResourceModel::addPhysbox(IModelPhysbox *pPhysbox, int iBone)
{
	assert(pPhysbox);

	pPhysbox->AddRef();

	m_aPhysBoxes.push_back({pPhysbox, iBone});
}


UINT XMETHODCALLTYPE CResourceModel::getMaterialCount() const
{
	return(m_uMaterialCount);
}
UINT XMETHODCALLTYPE CResourceModel::getSkinCount() const
{
	return(m_uSkinCount);
}
const char * XMETHODCALLTYPE CResourceModel::getMaterial(UINT uMaterial, UINT uSkin) const
{
	assert(uMaterial < m_uMaterialCount);
	assert(uSkin < m_uSkinCount);

	return(m_aasMaterials[uSkin][uMaterial].c_str());
}
void XMETHODCALLTYPE CResourceModel::setMaterialCount(UINT uMaterialCount, UINT uSkinCount)
{
	m_uMaterialCount = uMaterialCount;
	m_uSkinCount = uSkinCount;

	m_aasMaterials.resize(m_uSkinCount);

	for(UINT i = 0, l = m_uSkinCount; i < l; ++i)
	{
		m_aasMaterials[i].resize(uMaterialCount);
	}
}
void XMETHODCALLTYPE CResourceModel::setMaterial(UINT uMaterial, UINT uSkin, const char *szName)
{
	assert(uMaterial < m_uMaterialCount);
	assert(uSkin < m_uSkinCount);

	m_aasMaterials[uSkin][uMaterial] = szName;
}


float XMETHODCALLTYPE CResourceModel::getMass() const
{
	return(m_fMass);
}
void XMETHODCALLTYPE CResourceModel::setMass(float fMass)
{
	assert(fMass >= 0.0f);

	m_fMass = fMass;
}


UINT XMETHODCALLTYPE CResourceModel::getGibsCount() const
{
	return(m_asGibNames.size());
}
const char * XMETHODCALLTYPE CResourceModel::getGibName(UINT uIndex) const
{
	assert(uIndex < m_asGibNames.size());

	return(m_asGibNames[uIndex].c_str());
}
UINT XMETHODCALLTYPE CResourceModel::addGibName(const char *szFileName)
{
	m_asGibNames.push_back(szFileName);
	m_asGibs.push_back(NULL);

	return(m_asGibNames.size() - 1);
}

const IXResourceModel * XMETHODCALLTYPE CResourceModel::getGib(UINT uIndex) const
{
	assert(uIndex < m_asGibs.size());

	return(m_asGibs[uIndex]);
}
void XMETHODCALLTYPE CResourceModel::setGib(UINT uIndex, IXResourceModel *pResource)
{
	assert(uIndex < m_asGibs.size());
	assert(pResource);

	pResource->AddRef();

	mem_release(m_asGibs[uIndex]);

	m_asGibs[uIndex] = pResource;
}

const IXResourceModelStatic * XMETHODCALLTYPE CResourceModel::asStatic() const
{
	if(getType() == XMT_STATIC)
	{
		return((const CResourceModelStatic*)this);
	}
	return(NULL);
}
const IXResourceModelAnimated * XMETHODCALLTYPE CResourceModel::asAnimated() const
{
	if(getType() == XMT_ANIMATED)
	{
		return((const CResourceModelAnimated*)this);
	}
	return(NULL);
}
IXResourceModelStatic * XMETHODCALLTYPE CResourceModel::asStatic()
{
	if(getType() == XMT_STATIC)
	{
		return((CResourceModelStatic*)this);
	}
	return(NULL);
}
IXResourceModelAnimated * XMETHODCALLTYPE CResourceModel::asAnimated()
{
	if(getType() == XMT_ANIMATED)
	{
		return((CResourceModelAnimated*)this);
	}
	return(NULL);
}

IModelPhysboxBox * XMETHODCALLTYPE CResourceModel::newPhysboxBox() const
{
	return(new CModelPhysboxBox());
}
IModelPhysboxSphere * XMETHODCALLTYPE CResourceModel::newPhysboxSphere() const
{
	return(new CModelPhysboxSphere());
}
IModelPhysboxConvex * XMETHODCALLTYPE CResourceModel::newPhysboxConvex() const
{
	return(new CModelPhysboxConvex());
}
