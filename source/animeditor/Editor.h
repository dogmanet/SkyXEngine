#ifndef Editor_H
#define Editor_H

#include <SXGUIWinApi/sxguielements.h>
#include "../../SkyXEngine/animeditor/resource.h"

#include "TabManager.h"

//#include <anim/ModelFile.h>
#include <anim/animated.h>

#include "Camera.h"
#include "EMessages.h"

#ifdef _DEBUG
#	define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
//#include <d3dcompiler.h>
//#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d9.lib")
#include "TabActivities.h"
#include "TabAnimation.h"
#include "TabAttachments.h"

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
	static LRESULT AnimListCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT AnimFilterCB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT AnimGBProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static LRESULT AnimTBProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static void AnimPlayCB(int slot, ANIM_STATE state, Animation * pAnim);
	static void AnimProgressCB(int slot, float progress, Animation * pAnim);

	static Editor * GetInstance();
	
	void MenuBrowse(HWND hwnd);
	void MenuBrowseImport(HWND hwnd);
	void MenuSave();
	void MenuSaveAs(HWND hwnd);
	
	void Update();

	ModelFile * AddModel(const char * mdl, UINT flags = MI_ALL, bool forceImport=false);

	static INT_PTR CALLBACK DlgImportProc(
		_In_ HWND   hwndDlg,
		_In_ UINT   uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
		);

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

	AnimationManager * m_pAnimMgr;
	Animation * m_pCurAnim;

	static Editor * m_pEditor;

	char m_szAnimFilter[64];

	struct AnimItem
	{
		ModelSequence * seq;
		ModelFile const * mdl;
		bool isImported;
	};

	Array<AnimItem> m_vAnims;

	EActivityItems * m_pvActivities;

	int m_iCurIdx;

private:
	void RenderAnimList();

	static void DlgImpCheckAll(HWND hwndDlg);
};

#endif
