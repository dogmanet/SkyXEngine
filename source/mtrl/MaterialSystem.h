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
	void XMETHODCALLTYPE getMainTexture(IXTexture **ppTexture) override;
	ID getId();
	bool XMETHODCALLTYPE isTransparent() override;
	bool XMETHODCALLTYPE isRefractive() override;
	bool XMETHODCALLTYPE isBlurred() override;

	void XMETHODCALLTYPE setFlag(const char *szFlag, bool isSet) override;
	bool XMETHODCALLTYPE getFlag(const char *szFlag) override;

	void XMETHODCALLTYPE setParam(const char *szFlag, float fValue) override;
	float XMETHODCALLTYPE getParam(const char *szFlag) override;


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

	XVertexFormatHandler* XMETHODCALLTYPE registerVertexFormat(const char *szName, XVertexOutputElement *pDecl) override;
	void XMETHODCALLTYPE unregisterVertexFormat(const char *szName) override;
	XVertexFormatHandler* XMETHODCALLTYPE getVertexFormat(const char *szName) override;

	XVertexShaderHandler* XMETHODCALLTYPE registerVertexShader(XVertexFormatHandler *pVertexFormat, const char *szShaderFile, GXMacro *pDefines = NULL) override;
	void XMETHODCALLTYPE bindVS(XVertexShaderHandler *pVertexShader);

	XGeometryShaderHandler* XMETHODCALLTYPE registerGeometryShader(const char *szShaderFile, const char **aszRequiredParameters, GXMacro *pDefines = NULL) override;
	void XMETHODCALLTYPE bindGS(XGeometryShaderHandler *pGeometryShader) override;

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

	void updateReferences();
	void cleanData();

	struct VertexFormatData;
	struct VertexShaderData: public XVertexShaderHandler
	{
		const char *szShaderFile; //!< @fixme: нужно ли это хранить?
		ID idShader;
		VertexFormatData *pVertexFormat;
		Array<GXMacro> aDefines; //!< @fixme: нужно ли это хранить?
	};
	struct GeometryShaderData: public XGeometryShaderHandler
	{
		const char *szShaderFile;
		//ID idShader;
		//VertexFormatData *pVertexFormat;
		Array<GXMacro> aDefines;
		Array<const char*> aszRequiredParameters;
	};

	struct VertexFormatData: public XVertexFormatHandler
	{
		Array<XVertexOutputElement> aDecl;
		Array<VertexShaderData*> aVS;
		Array<GeometryShaderData*> aGS;
	};

	AssotiativeArray<String, VertexFormatData> m_mVertexFormats;
	MemAlloc<VertexShaderData> m_poolVSdata;
	MemAlloc<GeometryShaderData> m_poolGSdata;
	Array<GeometryShaderData*> m_aGeometryShaders;

	VertexShaderData *m_pCurrentVS = NULL;
	GeometryShaderData *m_pCurrentGS = NULL;
};

#endif
