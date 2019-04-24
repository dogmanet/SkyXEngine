#ifndef _SXGUI_H_
#define _SXGUI_H_

#include <Windows.h>
#include <graphix/graphix.h>

#if 0
#ifndef SXGUI_LIB
#	ifdef _DEBUG
#		pragma comment(lib,"SXGUId.lib")
#	else
#		pragma comment(lib,"SXGUI.lib")
#	endif
#endif
#endif

#include "guimain.h"

namespace gui
{
	class CGUI: public IGUI
	{
	public:
		SX_ALIGNED_OP_MEM2();

		CGUI(IGXContext * pDev, const char * szResPath, HWND hWnd);

		BOOL putMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		void update();
		void syncronize();

		void render();

		void onResetDevice();
		void onLostDevice();

		void release();


		IDesktop * createDesktopA(const char * name, const char * file);
		IDesktop * createDesktopW(const WCHAR * name, const WCHAR * file);

		void setActiveDesktop(IDesktop * d, BOOL clearStack = TRUE);
		void setActiveDesktopA(const char * name);
		void setActiveDesktopW(const WCHAR * name);

		IDesktop * getActiveDesktop();

		IDesktop * getDesktopA(const char * name);
		IDesktop * getDesktopW(const WCHAR * name);

		void registerCallback(const char * cbName, GUI_CALLBACK cb);

		void registerCallbackDefault(GUI_CALLBACK_WC cb, BOOL greedy = FALSE);

		GUI_CALLBACK getCallbackByName(const char * cbName);
		GUI_CALLBACK getCallbackByName(const StringW & cbName);

		void updateScreenSize();
		void showCursor(BOOL bShow);

		WCHAR * getResourceDir();
		IGXContext * getDevice();

		UINT getScreenWidth();
		UINT getScreenHeight();

		void messageBox(const WCHAR * title, const WCHAR * text, ...);

		void pushDesktop(IDesktop *);
		IDesktop * popDesktop();

		void destroyDesktop(IDesktop * dp);

		void execCallback(const StringW cmd, IEvent * ev);

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
		const shaders_s *getShaders()
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
		const bstate_s *getBlendStates()
		{
			return(&m_blendStates);
		}

		struct vdecl_s
		{
			IGXVertexDeclaration *m_pXYZ;
			IGXVertexDeclaration *m_pXYZTex;
		};
		const vdecl_s *getVertexDeclarations()
		{
			return(&m_vertexDeclarations);
		}

		IGXIndexBuffer *getQuadIndexBuffer()
		{
			return(m_pQuadIndexes);
		}

		void setTransformWorld(const SMMATRIX &mat)
		{
			m_mTransformWorld = mat;
		}
		void setTransformViewProj(const SMMATRIX &mat)
		{
			m_mTransformViewProj = mat;
		}

		const SMMATRIX &getTransformWorld()
		{
			return(m_mTransformWorld);
		}
		const SMMATRIX &getTransformViewProj()
		{
			return(m_mTransformViewProj);
		}

		void updateTransformShader()
		{
			m_pVSTransformConstant->update(&SMMatrixTranspose(m_mTransformWorld * m_mTransformViewProj));
			m_pDevice->setVertexShaderConstant(m_pVSTransformConstant, 0);
		}

		IGXRenderBuffer *getQuadRenderBufferXYZ(float3_t *pVertices);
		IGXRenderBuffer *getQuadRenderBufferXYZTex16(float *pVertices);

		IFont *getFont(const WCHAR * szName, UINT size, IFont::STYLE style, int iBlurRadius);

	protected:
		SMMATRIX m_mTransformWorld;
		SMMATRIX m_mTransformViewProj;

		IGXContext * m_pDevice;
		WCHAR * m_szResourceDir = NULL;

		UINT m_iScreenWidth = 0;
		UINT m_iScreenHeight = 0;

		HWND m_hWnd;


		IGXDepthStencilSurface * m_pOldDepthStencilSurface = NULL;
		IGXSurface * m_pDepthStencilSurface = NULL;

		IDesktop * m_pActiveDesktop = NULL;
		Array<IDesktop*> m_mDesktopStack;

		bool m_bShowCursor = true;
		AssotiativeArray<StringW, IDesktop*> m_mDesktops;

		AssotiativeArray<StringW, /* Array< */GUI_CALLBACK/* > */> m_mCallbacks;
		Array<GUI_CALLBACK_WC> m_mCallbacksDefaults;
		Array<GUI_CALLBACK_WC> m_mCallbacksDefaultsWC;

		bool m_bDeviceLost = false;

		shaders_s m_shaders;
		dsstate_s m_depthStencilStates;
		bstate_s m_blendStates;
		IGXRasterizerState *m_pDefaultRState;
		vdecl_s m_vertexDeclarations;

		IGXIndexBuffer *m_pQuadIndexes;
		IGXSamplerState *m_pDefaultSamplerState;

		IGXVertexBuffer *m_pQuadVerticesXYZ;
		IGXVertexBuffer *m_pQuadVerticesXYZTex16;

		IGXRenderBuffer *m_pQuadRenderXYZ;
		IGXRenderBuffer *m_pQuadRenderXYZTex16;

		IGXConstantBuffer *m_pVSTransformConstant = NULL;
	};

	CGUI * GetGUI();
};

EXTERN_C __declspec(dllexport) gui::IGUI * InitInstance(IGXContext * pDev, const char * szResPath, HWND hWnd);



#endif
