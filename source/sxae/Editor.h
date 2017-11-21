#ifndef Editor_H
#define Editor_H

#define MAINWIN_SIZE_X	1320
#define MAINWIN_SIZE_Y	730

#define EDITOR_REG_TREE "SOFTWARE\\DogmaNet\\SkyXEngine\\Editor"
#define EDITOR_REG_KEY_GSDIR "gs_dir"

#include <SXGUIWinApi/sxgui.h>
#include "resource.h"

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
#include "TabHitboxes.h"

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
	
	void MenuNew(HWND hwnd);
	void MenuBrowse(HWND hwnd);
	void MenuBrowseImport(HWND hwnd, bool use=true);
	void MenuSave(HWND hwnd);
	void MenuSaveAs(HWND hwnd);

	bool SaveTo(char * to);
	
	void Update();

	ModelFile * AddModel(const char * mdl, UINT flags = MI_ALL, bool forceImport = false, bool forceLocal = false);
	void DelModel(UINT id);

	void DelHitbox(UINT id);

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
	void DrawHitboxes();
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
	SMMATRIX m_mHelperMat;

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

	Array<ModelPart*> m_vMdlParts;

	EActivityItems * m_pvActivities;

	int m_iCurIdx;

	char m_szGamesourceDir[MODEL_MAX_FILE];

	struct HitboxItem
	{
		ModelHitbox * hb;
		ModelFile const * mdl;
		bool isImported;
		UINT id;
	};
	Array<HitboxItem> m_vHitboxes;

	ModelFile * m_pHitboxesPart;

	char m_szEditFile[MAX_PATH];


	ID m_pVSH;
	ID m_pPSH;

	ID m_pVSHs;
	ID m_pPSHs;

private:
	void RenderAnimList();
	void RenderBoneList();
	void RenderPartList();
	void RenderHitboxList();

	void OnAnimListSelChg();
	void OnPartListSelChg();
	void OnPartApply();
	void SetPartFlag(MODEL_PART_FLAGS f, byte v);
	void OnHitboxListSelChg();

	static void DlgImpCheckAll(HWND hwndDlg);

	bool GetRegGSdir();
	void SetRegGSdir();

	void UpdateHitboxList(ModelFile * mdl, bool bIsImported);

	struct vert
	{
		float3_t pos;
		DWORD color;
	};

	void DrawBox(const float3_t & lwh, DWORD color = 0xFFFFFFFF);
	void DrawHemiSphere(float3_t lwh, bool up=true, DWORD color = 0xFFFFFFFF);
	void DrawSphere(float3_t lwh, DWORD color = 0xFFFFFFFF);
	void DrawCapsule(float3_t lwh, DWORD color = 0xFFFFFFFF);
	void DrawCylinder(float3_t lwh, DWORD color = 0xFFFFFFFF);

	enum HANDLER_AXE
	{
		HA_NONE = 0x00,
		HA_X    = 0x01,
		HA_Y    = 0x02,
		HA_XY   = 0x03,
		HA_Z    = 0x04,
		HA_XZ   = 0x05,
		HA_YZ   = 0x06,
		HA_XYZ  = 0x07
	};

	enum HANDLER_TYPE
	{
		HT_NONE,
		HT_MOVE,
		HT_ROTATE,
		HT_SCALE
	};


	HANDLER_AXE m_currentAxe;
	HANDLER_TYPE m_htype;
	bool m_bIsDragging;
	bool m_bIsDraggingStart;
	bool m_bIsDraggingStop;
	float3 m_fStartDragPos;
	SMMATRIX m_mOldDragMat;
	SMMATRIX m_mOldHitboxMat;
	SMMATRIX m_mHitboxMat;
	float3 m_fOldHitboxLWH;

	UINT m_iActiveHitbox;

	void DrawHandlerMove();
	void DrawHandlerRotate();
	void DrawHandlerScale();
	void OnMouseDown(int x, int y);
	void HandlerIntersectMove(const float3 & start, const float3 & dir);
	void HandlerIntersectRotate(const float3 & start, const float3 & dir);
	void HandlerIntersectScale(const float3 & start, const float3 & dir);
};

#endif
