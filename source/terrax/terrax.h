#ifndef _TERRAX_H_
#define _TERRAX_H_

// "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'" 
#include <gcore/sxgcore.h>
#include <xcommon/editor/IXEditorExtension.h>
#include <xcommon/editor/IXEditorObject.h>
#include <common/assotiativearray.h>
#include <xEngine/IXEngine.h>

#define MAIN_WINDOW_TITLE      "TerraX"
#define MAIN_WINDOW_CLASS      "X Main Window"
#define RENDER_WINDOW_CLASS    "X Viewport Window"
#define WIDTH_ADJUST			2

#define	WINDOW_WIDTH			800
#define	WINDOW_HEIGHT			600

#define	SPLITTER_BAR_WIDTH		2

#define MARGIN_TOP              30
#define MARGIN_BOTTOM           22
#define MARGIN_LEFT             50
#define MARGIN_RIGHT            150
#define AB_BUTTON_HEIGHT        40
#define OBJECT_TREE_HEIGHT      300


#define WM_SETTITLEASYNC (WM_USER + 1)

#include "Grid.h"
#include "MaterialBrowser.h"

enum X_VIEWPORT_LAYOUT
{
	XVIEW_2X2,
	XVIEW_1X2,
	XVIEW_2X1,
	XVIEW_3DONLY
};

extern HWND g_hTopRightWnd;
extern HWND g_hTopLeftWnd;
extern HWND g_hBottomRightWnd;
extern HWND g_hBottomLeftWnd;

enum X_2D_VIEW
{
	X2D_NONE = -1,
	X2D_TOP,   // x/z
	X2D_FRONT, // x/y
	X2D_SIDE   // z/y
};


enum X_2DXFORM_TYPE
{
	X2DXF_SCALE = 0,
	X2DXF_ROTATE,


	X2DXF__LAST
};

enum X_DIR
{
	XDIR_X_POS = 0x00000001,
	XDIR_X_NEG = 0x00000002,
	XDIR_Y_POS = 0x00000004,
	XDIR_Y_NEG = 0x00000008,
	XDIR_Z_POS = 0x00000010,
	XDIR_Z_NEG = 0x00000020,
};
DEFINE_ENUM_FLAG_OPERATORS(X_DIR);

struct CTerraXConfig
{
	CTerraXConfig()
	{
		m_x2DView[0] = X2D_NONE;
		m_x2DView[1] = X2D_TOP;
		m_x2DView[2] = X2D_SIDE;
		m_x2DView[3] = X2D_FRONT;

		m_fViewportScale[0] = 
			m_fViewportScale[1] = 
			m_fViewportScale[2] = 
			m_fViewportScale[3] = 1.0f;

		memset(m_pViewportCamera, 0, sizeof(m_pViewportCamera));
	};

	X_2D_VIEW m_x2DView[4];
	float m_fViewportScale[4];
	ICamera *m_pViewportCamera[4];

	GRID_STEP m_gridStep = GRID_STEP_1M;
	bool m_bShowGrid = true;
	bool m_bSnapGrid = true;
	bool m_bDottedGrid = false;
	float m_fGridOpacity = 0.5f;

	X_VIEWPORT_LAYOUT m_xViewportLayout = XVIEW_2X2;
};

struct CTerraXState
{
	X_WINDOW_POS activeWindow = XWP_TOP_LEFT;
	float2_t vMousePos;
	float2_t vWorldMousePos;
	float4_t m_vViewportBorders[4];

	bool isFrameSelect = false;
	float2_t vFrameSelectStart;

	bool bHasSelection = false;
	float3_t vSelectionBoundMin;
	float3_t vSelectionBoundMax;

	X_2DXFORM_TYPE xformType = X2DXF_SCALE;


	bool bCreateMode = false;
	float3 vCreateOrigin;
};

#define X_MAX_HANDLERS_PER_DIP 512

struct CTerraXRenderStates
{
	IGXBlendState *pBlendColorFactor = NULL;
	IGXBlendState *pBlendAlpha = NULL;

	IGXRasterizerState *pRSWireframe = NULL;
	IGXRasterizerState *pRSSolidNoCull = NULL;

	ID idTexturedShaderVS = -1;
	ID idTexturedShaderPS = -1;
	ID idTexturedShaderKit = -1;

	ID idColoredShaderVS = -1;
	ID idColoredShaderPS = -1;
	ID idColoredShaderKit = -1;

	IGXVertexBuffer *pHandlerVB;
	IGXIndexBuffer *pHandlerIB;
	IGXVertexBuffer *pHandlerInstanceVB;
	IGXRenderBuffer *pHandlerRB;
	ID idHandlerShaderVS = -1;
	ID idHandlerShaderKit = -1;
	ID idBoundShaderKit = -1;

	IGXIndexBuffer *pHandler3DIB;
	IGXRenderBuffer *pHandler3DRB;

	IGXVertexBuffer *pIconVB;
	IGXIndexBuffer *pIcon3DIB;
	IGXRenderBuffer *pIcon3DRB;
	ID idIconShaderKit = -1;

	IGXVertexBuffer *pTransformHandlerVB;
	IGXRenderBuffer *pTransformHandlerRB;
	IGXIndexBuffer *pTransformHandlerScaleIB;
	IGXIndexBuffer *pTransformHandlerRotateIB;


	IGXRenderBuffer *pCreateCrossRB;

	IGXSamplerState *pSamplerLinearClamp;
};
extern CTerraXRenderStates g_xRenderStates;

extern CTerraXConfig g_xConfig;

extern CTerraXState g_xState;

extern CMaterialBrowser *g_pMaterialBrowser;

#define X2D_TOP_POS float3(0.0f, 1000.0f, 0.0f)
#define X2D_TOP_ROT SMQuaternion(-SM_PIDIV2, 'x')
#define X2D_FRONT_POS float3(0.0f, 0.0f, -1000.0f)
#define X2D_FRONT_ROT SMQuaternion()
#define X2D_SIDE_POS float3(1000.0f, 0.0f, 0.0f)
#define X2D_SIDE_ROT SMQuaternion(SM_PIDIV2, 'y')

extern BOOL g_is3DRotating;
extern BOOL g_is3DPanning;
extern BOOL g_is2DPanning;

void XResetLevel();
bool XSaveLevel(const char *szNewName=NULL, bool bForcePrompt = false);
void XLoadLevel(const char *szName);
void XRender3D();
void XRender2D(X_2D_VIEW view, float fScale, bool preScene);

void XFrameRun(float fDeltaTime);

#define XSELECT_STEP_DELAY 0.5f

struct XBorderVertex
{
	float3_t vPos;
	float2_t vTex;
};

extern Array<IXEditorObject*> g_pLevelObjects;

void XDrawBorder(GXCOLOR color, const float3_t &vA, const float3_t &vB, const float3_t &vC, const float3_t &vD, float fViewportScale = 0.01f);

bool XExecCommand(IXEditorCommand *pCommand);

void XUpdateSelectionBound();

bool XRayCast(X_WINDOW_POS wnd);
bool XIsMouseInSelection(X_WINDOW_POS wnd);

void XUpdatePropWindow();

float3 XSnapToGrid(const float3 &vPos);

extern IXEngine *g_pEngine;

#endif
