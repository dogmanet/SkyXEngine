#ifndef __MATERIALSYSTEM_H
#define __MATERIALSYSTEM_H

#include "IXMaterialSystem.h"
#include <common/string.h>
#include <xcommon/IXTextureProxy.h>
#include <xcommon/resource/IXResourceTexture.h>
#include <common/ConcurrentQueue.h>

#include "LogicExpression.h"

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

	XRenderPassHandler* XMETHODCALLTYPE registerRenderPass(const char *szName, const char *szShaderFile, XRenderPassTexturesElement *pTextures, XRenderPassSamplersElement *pSamplers, XRenderPassOutputElement *pOutput) override;
	XRenderPassHandler* XMETHODCALLTYPE getRenderPass(const char *szName) override;

	XMaterialShaderHandler* XMETHODCALLTYPE registerMaterialShader(const char *szName, XVertexFormatHandler *pVertexFormat, XMaterialShaderPass *pPasses, XMaterialProperty *pGenericProperties) override;
	XMaterialShaderHandler* XMETHODCALLTYPE getMaterialShader(const char *szName) override;

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
	struct GeometryShader: public XGeometryShaderHandler
	{
		const char *szShaderFile;
		//VertexFormatData *pVertexFormat;
		Array<GXMacro> aDefines;
		Array<const char*> aszRequiredParameters;
	};
	struct GeometryShaderData
	{
		bool isSkipped;
		Array<GXMacro> aDefines;
		int iCommonDefines;
		ID idShader;
	};

	struct VertexFormatData: public XVertexFormatHandler
	{
		Array<XVertexOutputElement> aDecl;
		Array<VertexShaderData*> aVS;
		Array<GeometryShaderData*> aGS;
	};

	struct RenderPass: public XRenderPassHandler
	{
		const char *szName;
		const char *szShaderFile;
		Array<XRenderPassTexturesElement> aTextures;
		Array<XRenderPassSamplersElement> aSamplers;
		Array<XRenderPassOutputElement> aOutput;
		Array<GXMacro> aDefines;
	};

	struct MaterialShaderSamplerData
	{
		const char *szKey;
		IGXSamplerState *pSampler;
	};

	struct MaterialProperty
	{
		XMaterialProperty prop;
		CLogicExpression *pCondition;
	};

	struct MaterialDefine
	{
		const char *szName;
		CLogicExpression *pCondition;
	};

	struct MaterialVariant
	{
		bool isReady;
		ID idShader;
	};
	
	struct MaterialShaderPassData
	{
		RenderPass *pRenderPass;
		const char *szShaderFile;
		const char *szEntryPoint;
		Array<GXMacro> aDefines;
		Array<MaterialShaderSamplerData> aSamplers;
		Array<MaterialProperty> aProperties;
		bool isDirty;

		Array<MaterialShaderSamplerData> aTotalSamplers;

		Array<MaterialVariant> aVariants;
	};

	struct MaterialShader: public XMaterialShaderHandler
	{
		const char *szName;
		VertexFormatData *pVertexFormat;
		Array<MaterialShaderPassData> aPasses;
		Array<MaterialProperty> aProperties;
	};

	struct MaterialShaderConstants
	{
		const char *szKey;
		GXDECLTYPE type;
		UINT uGroup;
	};

	AssotiativeArray<String, VertexFormatData> m_mVertexFormats;
	MemAlloc<VertexShaderData> m_poolVSdata;
	MemAlloc<GeometryShader> m_poolGSdata;
	Array<GeometryShader*> m_aGeometryShaders;
	MemAlloc<GeometryShaderData> m_aGeometryShadersData;


	AssotiativeArray<String, RenderPass> m_mRenderPasses;
	AssotiativeArray<String, MaterialShader> m_mMaterialShaders;

	VertexShaderData *m_pCurrentVS = NULL;
	GeometryShader *m_pCurrentGS = NULL;

	friend void CopyProps(XMaterialProperty *pProperties, Array<CMaterialSystem::MaterialProperty> &aTarget, const char *szShaderName);
	friend void GetAllDefines(Array<MaterialDefine> &aAllDefines, Array<MaterialProperty> &aProperties);
	friend void ParseTexturesConstants(Array<MaterialProperty> &aProperties, Array<MaterialDefine*> &aStaticList,
		Array<const char*> &aTextures, Array<MaterialShaderConstants> &aConstants);
	friend bool EvalCondition(CLogicExpression *pExpr, Array<MaterialDefine*> &aStaticList);

	const char* getHLSLType(GXDECLTYPE type)
	{
		switch(type)
		{
		case GXDECLTYPE_FLOAT1:
			return("float");
		case GXDECLTYPE_FLOAT2:
			return("float2");
		case GXDECLTYPE_FLOAT3:
			return("float3");
		case GXDECLTYPE_FLOAT4:
			return("float4");
		}
		assert(!"Unknown type");
		return("");
	}
	int getTypeSize(GXDECLTYPE type)
	{
		switch(type)
		{
		case GXDECLTYPE_FLOAT1:
			return(1);
		case GXDECLTYPE_FLOAT2:
			return(2);
		case GXDECLTYPE_FLOAT3:
			return(3);
		case GXDECLTYPE_FLOAT4:
			return(4);
		}
		assert(!"Unknown type");
		return(0);
	}
	const char* getHLSLSemantic(GXDECLUSAGE usage)
	{
		switch(usage)
		{
		case GXDECLUSAGE_POSITION:
			return("POSITION0");
		case GXDECLUSAGE_TEXCOORD:
			return("TEXCOORD0");
		case GXDECLUSAGE_TEXCOORD1:
			return("TEXCOORD1");
		case GXDECLUSAGE_TEXCOORD2:
			return("TEXCOORD2");
		case GXDECLUSAGE_TEXCOORD3:
			return("TEXCOORD3");
		case GXDECLUSAGE_TEXCOORD4:
			return("TEXCOORD4");
		case GXDECLUSAGE_TEXCOORD5:
			return("TEXCOORD5");
		case GXDECLUSAGE_TEXCOORD6:
			return("TEXCOORD6");
		case GXDECLUSAGE_TEXCOORD7:
			return("TEXCOORD7");
		case GXDECLUSAGE_BINORMAL:
			return("BINORMAL0");
		case GXDECLUSAGE_BLENDINDICES:
			return("BLENDINDICES0");
		case GXDECLUSAGE_BLENDWEIGHT:
			return("BLENDWEIGHT0");
		case GXDECLUSAGE_COLOR:
			return("COLOR0");
		case GXDECLUSAGE_NORMAL:
			return("NORMAL0");
		case GXDECLUSAGE_TANGENT:
			return("TANGENT0");
		case GXDECLUSAGE_TESSFACTOR:
			return("TESSFACTOR0");
		}
		assert(!"Unknown usage");
		return("");
	}
};

#endif
