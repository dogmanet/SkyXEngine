

#include <gcore/sxgcore.h>

#define MAIN_WINDOW_CLASS         "X Main Window"
#define RENDER_WINDOW_CLASS		  "X Viewport Window"
#define WIDTH_ADJUST			2

#define	WINDOW_WIDTH			800
#define	WINDOW_HEIGHT			600

#define	SPLITTER_BAR_WIDTH		2

#define MARGIN_TOP              30
#define MARGIN_BOTTOM           22
#define MARGIN_LEFT             50
#define MARGIN_RIGHT            150
#define AB_BUTTON_HEIGHT        40

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

extern ICamera *g_pTopRightCamera;
extern ICamera *g_pBottomLeftCamera;
extern ICamera *g_pBottomRightCamera;

enum X_2D_VIEW
{
	X2D_TOP,
	X2D_FRONT,
	X2D_SIDE
};

extern X_2D_VIEW g_x2DTopRightView;
extern X_2D_VIEW g_x2DBottomLeftView;
extern X_2D_VIEW g_x2DBottomRightView;

#define X2D_TOP_POS float3(0.0f, 1000.0f, 0.0f)
#define X2D_TOP_ROT SMQuaternion(-SM_PIDIV2, 'x')
#define X2D_FRONT_POS float3(0.0f, 0.0f, 1000.0f)
#define X2D_FRONT_ROT SMQuaternion(-SM_PI, 'y')
#define X2D_SIDE_POS float3(1000.0f, 0.0f, 0.0f)
#define X2D_SIDE_ROT SMQuaternion(-SM_PIDIV2, 'y')

extern BOOL g_is3DRotating;
extern BOOL g_is3DPanning;

