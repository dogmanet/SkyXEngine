#include "GUI.h"
#include "DesktopStack.h"

#if 0
CGUI::CGUI(IGXDevice *pDev, IXMaterialSystem *pMaterialSystem, IFileSystem *pFileSystem):
	m_pDevice(pDev)
{
	g_pGUI = this;

	gui::CKeyMap::init();

	m_shaders.m_baseTexturedColored.m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gui_main.vs");
	m_shaders.m_baseTexturedColored.m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gui_main.ps");
	m_shaders.m_baseTexturedColored.m_idShaderKit = SGCore_ShaderCreateKit(m_shaders.m_baseTexturedColored.m_idVS, m_shaders.m_baseTexturedColored.m_idPS);

	m_shaders.m_baseColored.m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gui_simple.vs");
	m_shaders.m_baseColored.m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gui_simple.ps");
	m_shaders.m_baseColored.m_idShaderKit = SGCore_ShaderCreateKit(m_shaders.m_baseColored.m_idVS, m_shaders.m_baseColored.m_idPS);

	m_shaders.m_baseTexturedTextransformColored.m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gui_main_textransform.vs");
	m_shaders.m_baseTexturedTextransformColored.m_idPS = m_shaders.m_baseTexturedColored.m_idPS;
	m_shaders.m_baseTexturedTextransformColored.m_idShaderKit = SGCore_ShaderCreateKit(m_shaders.m_baseTexturedTextransformColored.m_idVS, m_shaders.m_baseTexturedTextransformColored.m_idPS);

	GXDepthStencilDesc depthStencilDesc;
	depthStencilDesc.useDepthTest = false;
	depthStencilDesc.useDepthWrite = false;
	m_depthStencilStates.m_pDefault = m_pDevice->createDepthStencilState(&depthStencilDesc);

	depthStencilDesc.useStencilTest = true;
	depthStencilDesc.stencilTestFront.cmpFunc = GXCMP_EQUAL;
	depthStencilDesc.stencilTestFront.opPass = GXSTENCIL_OP_INCR;
	m_depthStencilStates.m_pStencilIncr = m_pDevice->createDepthStencilState(&depthStencilDesc);

	depthStencilDesc.stencilTestFront.opPass = GXSTENCIL_OP_KEEP;
	m_depthStencilStates.m_pStencilKeep = m_pDevice->createDepthStencilState(&depthStencilDesc);

	depthStencilDesc.stencilTestFront.opPass = GXSTENCIL_OP_DECR;
	m_depthStencilStates.m_pStencilDecr = m_pDevice->createDepthStencilState(&depthStencilDesc);

	GXBlendDesc blendDesc;
	blendDesc.renderTarget[0].useBlend = true;
	blendDesc.renderTarget[0].blendSrcAlpha = blendDesc.renderTarget[0].blendSrcColor = GXBLEND_SRC_ALPHA;
	blendDesc.renderTarget[0].blendDestAlpha = blendDesc.renderTarget[0].blendDestColor = GXBLEND_INV_SRC_ALPHA;
	m_blendStates.m_pDefault = m_pDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].u8RenderTargetWriteMask = 0;
	m_blendStates.m_pNoColorWrite = m_pDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_ALL;
	blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_ONE;
	blendDesc.renderTarget[0].blendOpAlpha = GXBLEND_OP_MAX;
	m_blendStates.m_pDesktop = m_pDevice->createBlendState(&blendDesc);

	GXRasterizerDesc rasterizerDesc;
	rasterizerDesc.useMultisample = true;
	rasterizerDesc.useAntialiasedLine = true;
	rasterizerDesc.cullMode = GXCULL_NONE;
	m_pDefaultRState = m_pDevice->createRasterizerState(&rasterizerDesc);

	GXVertexElement aVertexElementsXYZTex[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION, GXDECLSPEC_PER_VERTEX_DATA},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD, GXDECLSPEC_PER_VERTEX_DATA},
		GX_DECL_END()
	};
	m_vertexDeclarations.m_pXYZTex = m_pDevice->createVertexDeclaration(aVertexElementsXYZTex);

	GXVertexElement aVertexElementsXYZ[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION, GXDECLSPEC_PER_VERTEX_DATA},
		GX_DECL_END()
	};
	m_vertexDeclarations.m_pXYZ = m_pDevice->createVertexDeclaration(aVertexElementsXYZ);

	USHORT pIdxQuad[] = {
		0, 1, 2, 0, 2, 3
	};
	m_pQuadIndexes = m_pDevice->createIndexBuffer(sizeof(USHORT) * 6, GXBUFFER_USAGE_STATIC, GXIT_UINT16, pIdxQuad);

	GXSamplerDesc samplerDesc;
	samplerDesc.filter = GXFILTER_ANISOTROPIC;
	m_pDefaultSamplerState = m_pDevice->createSamplerState(&samplerDesc);
}

CGUI::~CGUI()
{
	g_pGUI = NULL;
	mem_release(m_pDefaultSamplerState);
	mem_release(m_pQuadIndexes);
	mem_release(m_vertexDeclarations.m_pXYZ);
	mem_release(m_vertexDeclarations.m_pXYZTex);
	mem_release(m_pDefaultRState);
	mem_release(m_blendStates.m_pDesktop);
	mem_release(m_blendStates.m_pNoColorWrite);
	mem_release(m_blendStates.m_pDefault);
	mem_release(m_depthStencilStates.m_pStencilDecr);
	mem_release(m_depthStencilStates.m_pStencilKeep);
	mem_release(m_depthStencilStates.m_pStencilIncr);
	mem_release(m_depthStencilStates.m_pDefault);
}

IGXDevice* CGUI::getDevice()
{
	return(m_pDevice);
}
#endif

bool XMETHODCALLTYPE CGUI::newDesktopStack(const char *szResPath, UINT uWidth, UINT uHeight, IXDesktopStack **ppOut)
{
	CDesktopStack *pStack = new CDesktopStack(/*this, m_pDevice, szResPath, uWidth, uHeight*/);
	*ppOut = pStack;
	return(true);
}
