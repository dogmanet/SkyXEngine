#ifndef _TERRAX_H_
#define _TERRAX_H_

// "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'" 
#include <gcore/sxgcore.h>
#include "Command.h"
#include "XObject.h"

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

#include "Grid.h"

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

enum X_WINDOW_POS
{
	XWP_TOP_LEFT,
	XWP_TOP_RIGHT,
	XWP_BOTTOM_LEFT,
	XWP_BOTTOM_RIGHT
};

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

	bool isFrameSelect = false;
	float2_t vFrameSelectStart;

	bool bHasSelection = false;
	float3_t vSelectionBoundMin;
	float3_t vSelectionBoundMax;
};

struct CTerraXRenderStates
{
	IGXBlendState *pBlendColorFactor = NULL;
	IGXBlendState *pBlendAlpha = NULL;

	IGXRasterizerState *pRSWireframe = NULL;

	ID idTexturedShaderVS = -1;
	ID idTexturedShaderPS = -1;
	ID idTexturedShaderKit = -1;
};
extern CTerraXRenderStates g_xRenderStates;

extern CTerraXConfig g_xConfig;

extern CTerraXState g_xState;


#define X2D_TOP_POS float3(0.0f, 1000.0f, 0.0f)
#define X2D_TOP_ROT SMQuaternion(-SM_PIDIV2, 'x')
#define X2D_FRONT_POS float3(0.0f, 0.0f, -1000.0f)
#define X2D_FRONT_ROT SMQuaternion()
#define X2D_SIDE_POS float3(1000.0f, 0.0f, 0.0f)
#define X2D_SIDE_ROT SMQuaternion(SM_PIDIV2, 'y')

extern BOOL g_is3DRotating;
extern BOOL g_is3DPanning;

void XResetLevel();
bool XSaveLevel(const char *szNewName=NULL);
void XLoadLevel(const char *szName);
void XRender3D();
void XRender2D(X_2D_VIEW view, float fScale, bool preScene);

struct XBorderVertex
{
	float3_t vPos;
	float2_t vTex;
};

extern Array<CXObject*> g_pLevelObjects;

void XDrawBorder(GXCOLOR color, const float3_t &vA, const float3_t &vB, const float3_t &vC, const float3_t &vD, float fViewportScale = 0.01f);

bool XExecCommand(CCommand *pCommand);

void XUpdateSelectionBound();

#endif
