#ifndef __RESOURCEMODEL_H
#define __RESOURCEMODEL_H

#include <xcommon/resource/IXResourceModel.h>
#include <common/string.h>

class CResourceManager;
class CResourceModel: public virtual IXResourceModel
{
public:
	CResourceModel(CResourceManager *pMgr);
	~CResourceModel();

	void XMETHODCALLTYPE Release();

	UINT XMETHODCALLTYPE getPhysboxCount() const override;
	const IModelPhysbox * XMETHODCALLTYPE getPhysbox(UINT uPart) const override;
	int XMETHODCALLTYPE getPhysboxBone(UINT uPart) const override;
	void XMETHODCALLTYPE addPhysbox(IModelPhysbox *pPhysbox, int iBone = -1) override;


	UINT XMETHODCALLTYPE getMaterialCount() const override;
	UINT XMETHODCALLTYPE getSkinCount() const override;
	const char * XMETHODCALLTYPE getMaterial(UINT uMaterial, UINT uSkin = 0) const override;
	void XMETHODCALLTYPE setMaterialCount(UINT uMaterialCount, UINT uSkinCount) override;
	void XMETHODCALLTYPE setMaterial(UINT uMaterial, UINT uSkin, const char *szName) override;


	float XMETHODCALLTYPE getMass() const override;
	void XMETHODCALLTYPE setMass(float fMass) override;


	UINT XMETHODCALLTYPE getGibsCount() const override;
	const char * XMETHODCALLTYPE getGibName(UINT uIndex) const override;
	UINT XMETHODCALLTYPE addGibName(const char *szFileName) override;

	const IXResourceModel * XMETHODCALLTYPE getGib(UINT uIndex) const override;
	void XMETHODCALLTYPE setGib(UINT uIndex, IXResourceModel *pResource) override;

	const IXResourceModelStatic * XMETHODCALLTYPE asStatic() const override;
	const IXResourceModelAnimated * XMETHODCALLTYPE asAnimated() const override;
	IXResourceModelStatic * XMETHODCALLTYPE asStatic() override;
	IXResourceModelAnimated * XMETHODCALLTYPE asAnimated() override;

	IModelPhysboxBox    * XMETHODCALLTYPE newPhysboxBox() const override;
	IModelPhysboxSphere * XMETHODCALLTYPE newPhysboxSphere() const override;
	IModelPhysboxConvex * XMETHODCALLTYPE newPhysboxConvex() const override;
	IModelPhysboxCylinder * XMETHODCALLTYPE newPhysboxCylinder() const override;
	IModelPhysboxCapsule * XMETHODCALLTYPE newPhysboxCapsule() const override;

	void XMETHODCALLTYPE setLocalAABB(const float3 &vMin, const float3 &vMax) override;

	void setFileName(const char *szFilename);
	const char *getFileName() const;

	void XMETHODCALLTYPE makeReadOnly() override
	{
		m_isReadOnly = true;
	}
protected:
	CResourceManager *m_pManager;
	const char *m_szFileName = NULL;

	struct _phys_box
	{
		IModelPhysbox *pPhysbox;
		int iBone;
	};

	Array<_phys_box> m_aPhysBoxes;
	UINT m_uMaterialCount = 0;
	UINT m_uSkinCount = 0;
	Array<Array<String>> m_aasMaterials;
	float m_fMass = 0.0f;
	Array<String> m_asGibNames;
	Array<IXResourceModel*> m_asGibs;

	bool m_isReadOnly = false;
};

#endif
