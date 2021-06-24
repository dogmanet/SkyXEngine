#ifndef __GUI_H
#define __GUI_H

#include <xcommon/gui/IXGUI.h>

class CGUI: public IXUnknownImplementation<IXGUI>
{
public:
#if 0
	SX_ALIGNED_OP_MEM();

	CGUI(IGXDevice *pDev, IXMaterialSystem *pMaterialSystem, IFileSystem *pFileSystem);
	~CGUI();

	IGXDevice* getDevice();

	struct shader_s
	{
		ID m_idVS = -1;
		ID m_idPS = -1;
		ID m_idShaderKit = -1;
	};
	struct shaders_s
	{
		shader_s m_baseTexturedColored;
		shader_s m_baseTexturedTextransformColored;
		shader_s m_baseColored;
	};
	const shaders_s* getShaders()
	{
		return(&m_shaders);
	}

	struct dsstate_s
	{
		IGXDepthStencilState *m_pDefault;
		IGXDepthStencilState *m_pStencilIncr;
		IGXDepthStencilState *m_pStencilDecr;
		IGXDepthStencilState *m_pStencilKeep;
	};
	const dsstate_s *getDepthStencilStates()
	{
		return(&m_depthStencilStates);
	}

	struct bstate_s
	{
		IGXBlendState *m_pDefault;
		IGXBlendState *m_pDesktop;
		IGXBlendState *m_pNoColorWrite;
	};
	const bstate_s* getBlendStates()
	{
		return(&m_blendStates);
	}

	IGXRasterizerState* getDefaultRasterizerState()
	{
		return(m_pDefaultRState);
	}

	IGXSamplerState* getDefaultSamplerState()
	{
		return(m_pDefaultSamplerState);
	}

	struct vdecl_s
	{
		IGXVertexDeclaration *m_pXYZ;
		IGXVertexDeclaration *m_pXYZTex;
	};
	const vdecl_s* getVertexDeclarations()
	{
		return(&m_vertexDeclarations);
	}

	IGXIndexBuffer* getQuadIndexBuffer()
	{
		return(m_pQuadIndexes);
	}
#endif
	bool XMETHODCALLTYPE newDesktopStack(const char *szResPath, UINT uWidth, UINT uHeight, IXDesktopStack **pOut) override;

protected:
#if 0
	IGXDevice *m_pDevice;

	shaders_s m_shaders;
	dsstate_s m_depthStencilStates;
	bstate_s m_blendStates;
	IGXRasterizerState *m_pDefaultRState;
	vdecl_s m_vertexDeclarations;

	IGXIndexBuffer *m_pQuadIndexes;
	IGXSamplerState *m_pDefaultSamplerState;
#endif
};

#endif
