#ifndef Editor_H
#define Editor_H

#include <SXGUIWinApi/sxguielements.h>
#include "../../SkyXEngine/animeditor/resource.h"

#include "TabManager.h"

#include <anim/ModelFile.h>
//#include <anim/animated.h>

#include "Camera.h"


#ifdef _DEBUG
#	define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
//#include <d3dcompiler.h>
//#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d9.lib")

//#ifdef _DEBUG
//#	pragma comment(lib, "d3dx9d.lib")
//#else
//#	pragma comment(lib, "d3dx9.lib")
//#endif

/**@TODO:
	- Добавить окошко лодов
	- Добавить окошко хитбоксов
*/
class Editor
{
public:
	Editor();
	~Editor();


	static LRESULT MenuCmd(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CamInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	void MenuBrowse(HWND hwnd);
	void MenuSave();
	void MenuSaveAs(HWND hwnd);
	
	void Update();

protected:
	void InitUI();
	void DestroyUI();

	void InitD3D();
	void DestroyD3D();

	void DrawAxis();
	void CenterMouse();

	ISXGUIBaseWnd* MainWindow;
	ISXGUIBaseWnd* D3DWindow;
	ISXGUIListBox* AnimList;
	ISXGUIGroupBox* AnimationsGB;
	ISXGUIEdit* AnimFilter;
	ISXGUIStatic* Static1;

	ISXGUIStatic* CurAnimName;

	ISXGUIMenu * Menu;

	TabManager * m_pTM;

	bool m_bDirty;

	IDirect3D9 * m_pD3D = NULL;
	IDirect3DDevice9 * m_pd3dDevice = NULL;
	IDirect3DSwapChain9 * m_pSwapChain = NULL;

	struct VShaderInputCamera
	{
		SMMATRIX mRes;
		SMMATRIX mWorld;
	};

	SMMATRIX m_mWorldMat;
	SMMATRIX m_mViewMat;
	SMMATRIX m_mProjMat;

	Camera m_cam;

	UINT m_uWidth;
	UINT m_uHeight;

	bool m_bCamMove;
};

#endif
