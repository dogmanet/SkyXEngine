#ifndef __MATERIALSYSTEM_H
#define __MATERIALSYSTEM_H

#include "IXMaterialSystem.h"
#include <common/string.h>
#include <xcommon/IXTextureProxy.h>
#include <xcommon/resource/IXResourceTexture.h>
#include <common/ConcurrentQueue.h>

class CMaterialSystem;
class CTexture: public IXTexture
{
public:
	CTexture(CMaterialSystem *pMaterialSystem, IXResourceTexture *m_pResource);
	~CTexture();
	void XMETHODCALLTYPE Release() override;
	void XMETHODCALLTYPE getAPITexture(IGXBaseTexture **ppTexture, UINT uFrame = 0) override;
	bool XMETHODCALLTYPE isReady() const override;

	UINT XMETHODCALLTYPE getNumFrames() const override;
	float XMETHODCALLTYPE getFrameTime() const override;

	GXTEXTURE_TYPE XMETHODCALLTYPE getType() const override;

	UINT XMETHODCALLTYPE getWidth() const override;
	UINT XMETHODCALLTYPE getHeight() const override;
	UINT XMETHODCALLTYPE getDepth() const override;

	void setName(const char *szName);
	const char* getName() const;

	void initGPUresources();
protected:
	CMaterialSystem *m_pMaterialSystem;
	IXResourceTexture *m_pResource;

	IGXBaseTexture **m_ppGXTexture = NULL;
	const char *m_szName = NULL;
	UINT m_uFrameCount = 0;
	float m_fFrameTime = 0.0f;

	GXTEXTURE_TYPE m_type = GXTEXTURE_TYPE_UNKNOWN;

	UINT m_uWidth = 1;
	UINT m_uHeight = 1;
	UINT m_uDepth = 1;
};

class CMaterial: public IXMaterial
{
public:
	CMaterial(ID id);
	void getMainTexture(IXTexture **ppTexture) override;
	ID getId();
	bool isTransparent() override;
	bool isRefractive() override;
	bool isBlurred() override;

	ID getInternalID()
	{
		return(m_id);
	}
protected:
	ID m_id = -1;
};

class CMaterialSystem: public IXMaterialSystem
{
public:
	CMaterialSystem();
	~CMaterialSystem();

	void XMETHODCALLTYPE loadMaterial(const char *szName, IXMaterial **ppMaterial, XSHADER_DEFAULT_DESC *pDefaultShaders, UINT uVariantCount = 0, XSHADER_VARIANT_DESC *pVariantsDesc = NULL) override;
	bool XMETHODCALLTYPE getMaterial(const char *szName, IXMaterial **ppMaterial) override;

	bool XMETHODCALLTYPE loadTexture(const char *szName, IXTexture **ppTexture) override;
	bool XMETHODCALLTYPE getTexture(const char *szName, IXTexture **ppTexture) override;
	//void XMETHODCALLTYPE addTexture(const char *szName, IGXTexture2D *pTexture) override;

	void XMETHODCALLTYPE bindMaterial(IXMaterial *pMaterial, IXShaderVariant *pShaderVariant = NULL) override;
	void XMETHODCALLTYPE bindTexture(IXTexture *pTexture, UINT slot = 0) override;
	void XMETHODCALLTYPE setWorld(const SMMATRIX &mWorld) override;

	void XMETHODCALLTYPE overrideGeometryShader(ID id) override;
	void XMETHODCALLTYPE overridePixelShader(ID id) override;


	void queueTextureUpload(CTexture *pTexture);
	void onTextureRelease(CTexture *pTexture);
	void update(float fDT);
protected:
	struct CObjectData
	{
		SMMATRIX m_mW;
	//	SMMATRIX m_mWV;
	//	SMMATRIX m_mWVP;
	} m_objectData;
	IGXConstantBuffer *m_pObjectConstantBuffer = NULL;

	MemAlloc<CTexture> m_poolTextures;
	Array<IXTextureProxy*> m_aTextureProxies;
	AssotiativeArray<String, CTexture*> m_mpTextures;
	CConcurrentQueue<CTexture*> m_queueTextureToLoad;
};

#endif
