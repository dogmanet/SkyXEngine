
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 
#include <skyxengine.h>
#include <io.h>

#include "terrax.h"
#include "Grid.h"

#include <xcommon/editor/IXEditorObject.h>
#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include "UndoManager.h"

extern HWND g_hWndMain;
CGrid *g_pGrid = NULL;
CTerraXRenderStates g_xRenderStates;

ATOM XRegisterClass(HINSTANCE hInstance);
BOOL XInitInstance(HINSTANCE, int);

Array<IXEditable*> g_pEditableSystems;
Array<IXEditorObject*> g_pLevelObjects;
//SGeom_GetCountModels()

static IGXVertexBuffer *g_pBorderVertexBuffer;
static IGXRenderBuffer *g_pBorderRenderBuffer;

CUndoManager *g_pUndoManager = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
//	SkyXEngine_PreviewCreate();
//	SXGUIinit();


	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES;
	InitCommonControlsEx(&icex);

	XRegisterClass(hInstance);
	g_pUndoManager = new CUndoManager();

	if(!XInitInstance(hInstance, nCmdShow))
	{
		return(1);
	}

	SkyXEngine_Init(g_hTopLeftWnd, g_hWndMain, lpCmdLine);

	IPluginManager *pPluginManager = Core_GetIXCore()->getPluginManager();

	IXMaterialSystem *pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);

	GXCOLOR w = GXCOLOR_ARGB(255, 255, 255, 255);
	GXCOLOR t = GXCOLOR_ARGB(0, 255, 255, 255);
	GXCOLOR colorData[] = {
		t, t, t, w, w, w,
		w, t, t, t, w, w,
		w, w, t, t, t, w,
		w, w, w, t, t, t,
		t, w, w, w, t, t,
		t, t, w, w, w, t
	};
	IGXTexture2D* pDashedMaterial = SGCore_GetDXDevice()->createTexture2D(6, 6, 1, 0, GXFMT_A8R8G8B8, colorData);
	pMaterialSystem->addTexture("dev_dashed", pDashedMaterial);
	mem_release(pDashedMaterial);

	GXCOLOR tmpColor = GXCOLOR_ARGB(255, 255, 255, 255);
	IGXTexture2D* pWhiteMaterial = SGCore_GetDXDevice()->createTexture2D(1, 1, 1, 0, GXFMT_A8R8G8B8, &tmpColor);
	pMaterialSystem->addTexture("dev_white", pWhiteMaterial);
	mem_release(pDashedMaterial);

	UINT ic = 0;
	IXEditable *pEditable;
	while((pEditable = (IXEditable*)pPluginManager->getInterface(IXEDITABLE_GUID, ic++)))
	{
		if(pEditable->getVersion() == IXEDITABLE_VERSION)
		{
			g_pEditableSystems.push_back(pEditable);
			pEditable->startup(SGCore_GetDXDevice());
		}
	}
	
	//SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyBoxLoadTex("sky_hdr.dds");
	//SGCore_SkyBoxLoadTex("sky_test_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyBoxSetUse(false);
	SGCore_SkyCloudsSetUse(false);

	g_xConfig.m_pViewportCamera[XWP_TOP_RIGHT] = SGCore_CrCamera();
	g_xConfig.m_pViewportCamera[XWP_TOP_RIGHT]->setPosition(&X2D_TOP_POS);
	g_xConfig.m_pViewportCamera[XWP_TOP_RIGHT]->setOrientation(&X2D_TOP_ROT);

	g_xConfig.m_pViewportCamera[XWP_BOTTOM_LEFT] = SGCore_CrCamera();
	g_xConfig.m_pViewportCamera[XWP_BOTTOM_LEFT]->setPosition(&X2D_SIDE_POS);
	g_xConfig.m_pViewportCamera[XWP_BOTTOM_LEFT]->setOrientation(&X2D_SIDE_ROT);

	g_xConfig.m_pViewportCamera[XWP_BOTTOM_RIGHT] = SGCore_CrCamera();
	g_xConfig.m_pViewportCamera[XWP_BOTTOM_RIGHT]->setPosition(&X2D_FRONT_POS);
	g_xConfig.m_pViewportCamera[XWP_BOTTOM_RIGHT]->setOrientation(&X2D_FRONT_ROT);


//	SkyXEngine_RunGenPreview();
	//	Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
	Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);

	SRender_EditorSetRenderGrid(true);
	SRender_EditorSetRenderAxesStatic(true);

	g_pGrid = new CGrid();
	g_pGrid->setOpacity(0.7f);
	
	int countgc = SGame_EntGetClassListCount();
	const char** listgc = new const char*[countgc];

	SGame_EntGetClassList(listgc, countgc);
	for(int i = 0; i < countgc; ++i)
	{
	//	level_editor::pComboBoxGameClass->addItem(listgc[i]);
	}
	mem_delete_a(listgc);

	g_xRenderStates.idColoredShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "terrax_colored.vs");
	g_xRenderStates.idColoredShaderPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "terrax_colored.ps");
	g_xRenderStates.idColoredShaderKit = SGCore_ShaderCreateKit(g_xRenderStates.idColoredShaderVS, g_xRenderStates.idColoredShaderPS);
//	SkyXEngine_PreviewKill();
	IGXContext *pDevice = SGCore_GetDXDevice();

	GXBLEND_DESC blendDesc;
	blendDesc.renderTarget[0].bBlendEnable = true;
	blendDesc.renderTarget[0].srcBlend = blendDesc.renderTarget[0].srcBlendAlpha = GXBLEND_BLEND_FACTOR;
	blendDesc.renderTarget[0].destBlend = blendDesc.renderTarget[0].destBlendAlpha = GXBLEND_INV_BLEND_FACTOR;
	g_xRenderStates.pBlendColorFactor = pDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].srcBlend = blendDesc.renderTarget[0].srcBlendAlpha = GXBLEND_SRC_ALPHA;
	blendDesc.renderTarget[0].destBlend = blendDesc.renderTarget[0].destBlendAlpha = GXBLEND_INV_SRC_ALPHA;
	g_xRenderStates.pBlendAlpha = pDevice->createBlendState(&blendDesc);

	GXRASTERIZER_DESC rsDesc; 
	rsDesc.cullMode = GXCULL_NONE;
	g_xRenderStates.pRSSolidNoCull = SGCore_GetDXDevice()->createRasterizerState(&rsDesc);

	rsDesc.fillMode = GXFILL_WIREFRAME;
	g_xRenderStates.pRSWireframe = SGCore_GetDXDevice()->createRasterizerState(&rsDesc);

	g_xRenderStates.idTexturedShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "terrax_textured.vs");
	g_xRenderStates.idTexturedShaderPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "terrax_textured.ps");
	g_xRenderStates.idTexturedShaderKit = SGCore_ShaderCreateKit(g_xRenderStates.idTexturedShaderVS, g_xRenderStates.idTexturedShaderPS);

	GXVERTEXELEMENT oLayout[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		GXDECL_END()
	};
	IGXVertexDeclaration *pVD = pDevice->createVertexDeclaration(oLayout);
	g_pBorderVertexBuffer = pDevice->createVertexBuffer(sizeof(XBorderVertex) * 5, GX_BUFFER_USAGE_STREAM | GX_BUFFER_WRITEONLY);
	g_pBorderRenderBuffer = pDevice->createRenderBuffer(1, &g_pBorderVertexBuffer, pVD);
	mem_release(pVD);

	// Object handlers
	GXVERTEXELEMENT oLayoutHandler[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION, GXDECLSPEC_PER_VERTEX_DATA},
		{1, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD, GXDECLSPEC_PER_INSTANCE_DATA},
		GXDECL_END()
	};
	pVD = pDevice->createVertexDeclaration(oLayoutHandler);
	g_xRenderStates.pHandlerVB = pDevice->createVertexBuffer(sizeof(float3_t) * 8, GX_BUFFER_USAGE_STREAM | GX_BUFFER_WRITEONLY);
	g_xRenderStates.pHandlerInstanceVB = pDevice->createVertexBuffer(sizeof(float3_t) * X_MAX_HANDLERS_PER_DIP, GX_BUFFER_USAGE_STREAM | GX_BUFFER_WRITEONLY);
	IGXVertexBuffer *ppVB[] = {g_xRenderStates.pHandlerVB, g_xRenderStates.pHandlerInstanceVB};
	g_xRenderStates.pHandlerRB = pDevice->createRenderBuffer(2, ppVB, pVD);
	mem_release(pVD);
	g_xRenderStates.idHandlerShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "terrax_handler.vs");
	g_xRenderStates.idHandlerShaderKit = SGCore_ShaderCreateKit(g_xRenderStates.idHandlerShaderVS, g_xRenderStates.idColoredShaderPS);
	USHORT pHandlerIndices[] = {0, 1, 2, 3, 4, 5, 6, 7};
	g_xRenderStates.pHandlerIB = pDevice->createIndexBuffer(sizeof(USHORT)* 8, GX_BUFFER_USAGE_STATIC | GX_BUFFER_WRITEONLY, GXIT_USHORT, pHandlerIndices);

	{
		// Transform handlers
		GXVERTEXELEMENT oLayoutHandler[] =
		{
			{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION, GXDECLSPEC_PER_VERTEX_DATA},
			GXDECL_END()
		};
		pVD = pDevice->createVertexDeclaration(oLayoutHandler);
		g_xRenderStates.pTransformHandlerVB = pDevice->createVertexBuffer(sizeof(float3_t)* 32, GX_BUFFER_USAGE_STREAM | GX_BUFFER_WRITEONLY);
		g_xRenderStates.pTransformHandlerRB = pDevice->createRenderBuffer(1, &g_xRenderStates.pTransformHandlerVB, pVD);
		mem_release(pVD);
		USHORT pHandlerIndices[] = {
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14, 12, 14, 15,

			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23,
			24, 25, 26, 24, 26, 27,
			28, 29, 30, 28, 30, 31
		};
		g_xRenderStates.pTransformHandlerScaleIB = pDevice->createIndexBuffer(sizeof(USHORT)* 48, GX_BUFFER_USAGE_STATIC | GX_BUFFER_WRITEONLY, GXIT_USHORT, pHandlerIndices);
		
		USHORT pHandlerRotateIndices[] = {
			0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7,
			8, 9, 10, 8, 10, 11, 8, 11, 12, 8, 12, 13, 8, 13, 14, 8, 14, 15,
			16, 17, 18, 16, 18, 19, 16, 19, 20, 16, 20, 21, 16, 21, 22, 16, 22, 23,
			24, 25, 26, 24, 26, 27, 24, 27, 28, 24, 28, 29, 24, 29, 30, 24, 30, 31
		};
		g_xRenderStates.pTransformHandlerRotateIB = pDevice->createIndexBuffer(sizeof(USHORT)* 72, GX_BUFFER_USAGE_STATIC | GX_BUFFER_WRITEONLY, GXIT_USHORT, pHandlerRotateIndices);

	}

	int result = SkyXEngine_CycleMain();

	for(UINT ic = 0, il = g_pEditableSystems.size(); ic < il; ++ic)
	{
		g_pEditableSystems[ic]->shutdown();
	}

	mem_delete(g_pGrid);
	mem_delete(g_pUndoManager);
	SkyXEngine_Kill();
	return result;
}

void XRender3D()
{
	for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
	{
		if(g_pLevelObjects[i]->isSelected())
		{
			g_pLevelObjects[i]->renderSelection(true);
		}
	}

	if(g_xState.isFrameSelect)
	{
		X_2D_VIEW xCurView = g_xConfig.m_x2DView[g_xState.activeWindow];

		float3_t va, vb, vc, vd;

		switch(xCurView)
		{
		case X2D_TOP:
			va = float3_t(g_xState.vFrameSelectStart.x, 0.0f, g_xState.vFrameSelectStart.y);
			vb = float3_t(g_xState.vWorldMousePos.x, 0.0f, g_xState.vFrameSelectStart.y);
			vc = float3_t(g_xState.vWorldMousePos.x, 0.0f, g_xState.vWorldMousePos.y);
			vd = float3_t(g_xState.vFrameSelectStart.x, 0.0f, g_xState.vWorldMousePos.y);
			break;
		case X2D_FRONT:
			va = float3_t(g_xState.vFrameSelectStart.x, g_xState.vFrameSelectStart.y, 0.0f);
			vb = float3_t(g_xState.vWorldMousePos.x, g_xState.vFrameSelectStart.y, 0.0f);
			vc = float3_t(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, 0.0f);
			vd = float3_t(g_xState.vFrameSelectStart.x, g_xState.vWorldMousePos.y, 0.0f);
			break;
		case X2D_SIDE:
			va = float3_t(0.0f, g_xState.vFrameSelectStart.y, g_xState.vFrameSelectStart.x);
			vb = float3_t(0.0f, g_xState.vFrameSelectStart.y, g_xState.vWorldMousePos.x);
			vc = float3_t(0.0f, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
			vd = float3_t(0.0f, g_xState.vWorldMousePos.y, g_xState.vFrameSelectStart.x);
			break;
		}

		XDrawBorder(GXCOLOR_ARGB(255, 255, 255, 0), va, vb, vc, vd);
	}
}

void XRender2D(X_2D_VIEW view, float fScale, bool preScene)
{
	IGXContext *pDevice = SGCore_GetDXDevice();

	if(preScene)
	{
		if(g_xConfig.m_bShowGrid)
		{
			GRID_STEP step = g_xConfig.m_gridStep;
			GRID_STEP minStep = CGrid::GetMinStepForScale(fScale);
			if(step < minStep)
			{
				step = minStep;
			}
			switch(view)
			{
			case X2D_FRONT:
				Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixRotationX(SM_PIDIV2));
				break;
			case X2D_SIDE:
				Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixRotationZ(SM_PIDIV2));
				break;
			}
			g_pGrid->setOpacity(g_xConfig.m_fGridOpacity);
			g_pGrid->setDottedMode(g_xConfig.m_bDottedGrid);
			g_pGrid->render(step);

			Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

		}
	}
	else
	{
		for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
		{
			if(g_pLevelObjects[i]->isSelected())
			{
				g_pLevelObjects[i]->renderSelection(false);
			}
		}

		// Draw handlers
#if 0
		if(g_pLevelObjects.size())
		{
			SMMATRIX mViewProj;
			Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mViewProj);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, g_xRenderStates.idHandlerShaderVS, "g_mVP", &SMMatrixTranspose(mViewProj), 4);
			pDevice->setPrimitiveTopology(GXPT_LINELIST);
			SGCore_ShaderBind(g_xRenderStates.idHandlerShaderKit);
			pDevice->setIndexBuffer(g_xRenderStates.pHandlerIB);

			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, g_xRenderStates.idColoredShaderPS, "g_vColor", &float4(0.0f, 1.0f, 0.0f, 1.0f), 1);

			float3_t *pvData;
			float fPtSize = 3.5f * fScale;
			if(g_xRenderStates.pHandlerVB->lock((void**)&pvData, GXBL_WRITE))
			{
				pvData[0] = float3_t(fPtSize, fPtSize, fPtSize);
				pvData[1] = (float3)(pvData[0] * -1.0f);

				pvData[2] = float3_t(fPtSize, fPtSize, -fPtSize);
				pvData[3] = (float3)(pvData[2] * -1.0f);

				pvData[4] = float3_t(fPtSize, -fPtSize, fPtSize);
				pvData[5] = (float3)(pvData[4] * -1.0f);

				pvData[6] = float3_t(-fPtSize, fPtSize, fPtSize);
				pvData[7] = (float3)(pvData[6] * -1.0f);

				g_xRenderStates.pHandlerVB->unlock();
			}
			pDevice->setRenderBuffer(g_xRenderStates.pHandlerRB);
			float4 fViewportBorders = g_xState.m_vViewportBorders[g_xState.activeWindow];
			for(bool isSelected = false;; isSelected = true)
			{
				UINT uHandlerCount = 0;
				pvData = NULL;
				for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
				{
					float3_t vPos = g_pLevelObjects[i]->getPos();
					//@TODO: Add visibility check
					/*if(fViewportBorders.x > vPos.x || fViewportBorders.z < vPos.x || fViewportBorders.y < vPos.z) // not visible
					{
						continue;
					}*/
					if(isSelected != g_pLevelObjects[i]->isSelected())
					{
						continue;
					}
					if(!pvData && !g_xRenderStates.pHandlerInstanceVB->lock((void**)&pvData, GXBL_WRITE))
					{
						break;
					}

					pvData[uHandlerCount++] = vPos;
					if(uHandlerCount == X_MAX_HANDLERS_PER_DIP)
					{
						g_xRenderStates.pHandlerInstanceVB->unlock();
						pDevice->drawIndexedInstanced(uHandlerCount, 8, 4, 0, 0);
						pvData = NULL;
						uHandlerCount = 0;
					}
				}
				if(pvData)
				{
					g_xRenderStates.pHandlerInstanceVB->unlock();
				}
				if(uHandlerCount)
				{
					pDevice->drawIndexedInstanced(uHandlerCount, 8, 4, 0, 0);
				}
				if(isSelected)
				{
					break;
				}
				SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, g_xRenderStates.idColoredShaderPS, "g_vColor", &float4(1.0f, 0.0f, 0.0f, 1.0f), 1);
			}

			SGCore_ShaderUnBind();
		}
#endif

		if(g_xState.isFrameSelect)
		{
			X_2D_VIEW xCurView = g_xConfig.m_x2DView[g_xState.activeWindow];
			float3_t va, vb, vc, vd;
			switch(xCurView)
			{
			case X2D_TOP:
				va = float3_t(g_xState.vFrameSelectStart.x, 0.0f, g_xState.vFrameSelectStart.y);
				vb = float3_t(g_xState.vWorldMousePos.x, 0.0f, g_xState.vFrameSelectStart.y);
				vc = float3_t(g_xState.vWorldMousePos.x, 0.0f, g_xState.vWorldMousePos.y);
				vd = float3_t(g_xState.vFrameSelectStart.x, 0.0f, g_xState.vWorldMousePos.y);
				break;
			case X2D_FRONT:
				va = float3_t(g_xState.vFrameSelectStart.x, g_xState.vFrameSelectStart.y, 0.0f);
				vb = float3_t(g_xState.vWorldMousePos.x, g_xState.vFrameSelectStart.y, 0.0f);
				vc = float3_t(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, 0.0f);
				vd = float3_t(g_xState.vFrameSelectStart.x, g_xState.vWorldMousePos.y, 0.0f);
				break;
			case X2D_SIDE:
				va = float3_t(0.0f, g_xState.vFrameSelectStart.y, g_xState.vFrameSelectStart.x);
				vb = float3_t(0.0f, g_xState.vFrameSelectStart.y, g_xState.vWorldMousePos.x);
				vc = float3_t(0.0f, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
				vd = float3_t(0.0f, g_xState.vWorldMousePos.y, g_xState.vFrameSelectStart.x);
				break;
			}
			XDrawBorder(GXCOLOR_ARGB(255, 255, 255, 0), va, vb, vc, vd, fScale);
		}
		if(g_xState.bHasSelection)
		{
			float3_t va, vb, vc, vd;
			float4 vBorder;
			float2 vSelectionCenter;
			switch(view)
			{
			case X2D_TOP:
				va = float3_t(g_xState.vSelectionBoundMin.x, 0.0f, g_xState.vSelectionBoundMin.z);
				vb = float3_t(g_xState.vSelectionBoundMax.x, 0.0f, g_xState.vSelectionBoundMin.z);
				vc = float3_t(g_xState.vSelectionBoundMax.x, 0.0f, g_xState.vSelectionBoundMax.z);
				vd = float3_t(g_xState.vSelectionBoundMin.x, 0.0f, g_xState.vSelectionBoundMax.z);
				vBorder = float4(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.z,
					g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.z);
				vSelectionCenter = float2(g_xState.vSelectionBoundMin.x + g_xState.vSelectionBoundMax.x,
					g_xState.vSelectionBoundMin.z + g_xState.vSelectionBoundMax.z) * 0.5f;
				break;
			case X2D_FRONT:
				va = float3_t(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.y, 0.0f);
				vb = float3_t(g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMin.y, 0.0f);
				vc = float3_t(g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.y, 0.0f);
				vd = float3_t(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMax.y, 0.0f);
				vBorder = float4(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.y,
					g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.y);
				vSelectionCenter = float2(g_xState.vSelectionBoundMin.x + g_xState.vSelectionBoundMax.x,
					g_xState.vSelectionBoundMin.y + g_xState.vSelectionBoundMax.y) * 0.5f;
				break;
			case X2D_SIDE:
				va = float3_t(0.0f, g_xState.vSelectionBoundMin.y, g_xState.vSelectionBoundMin.z);
				vb = float3_t(0.0f, g_xState.vSelectionBoundMin.y, g_xState.vSelectionBoundMax.z);
				vc = float3_t(0.0f, g_xState.vSelectionBoundMax.y, g_xState.vSelectionBoundMax.z);
				vd = float3_t(0.0f, g_xState.vSelectionBoundMax.y, g_xState.vSelectionBoundMin.z);
				vBorder = float4(g_xState.vSelectionBoundMin.z, g_xState.vSelectionBoundMin.y,
					g_xState.vSelectionBoundMax.z, g_xState.vSelectionBoundMax.y);
				vSelectionCenter = float2(g_xState.vSelectionBoundMin.z + g_xState.vSelectionBoundMax.z,
					g_xState.vSelectionBoundMin.y + g_xState.vSelectionBoundMax.y) * 0.5f;
				break;
			}
			XDrawBorder(GXCOLOR_ARGB(255, 255, 0, 0), va, vb, vc, vd, fScale);

			float3_t *pvData;
			float fPtSize = 3.0f * fScale;
			float fPtMargin = 7.0f * fScale;
			if(g_xRenderStates.pTransformHandlerVB->lock((void**)&pvData, GXBL_WRITE))
			{
				UINT uCV = 0;

				if(g_xState.xformType == X2DXF_SCALE)
				{
					float3 vCenter(vSelectionCenter.x, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vSelectionCenter.y);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vSelectionCenter.x, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.x - fPtMargin, 0.0f, vSelectionCenter.y);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.x - fPtMargin, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));

					vCenter = float3(vBorder.x - fPtMargin, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, fPtSize));
				}
				else if(g_xState.xformType == X2DXF_ROTATE)
				{
					float fPtSize2 = fPtSize * sqrtf(2.0f) * 0.5f;

					float3 vCenter = float3(vBorder.x - fPtMargin, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, fPtSize2));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vBorder.y - fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, fPtSize2));

					vCenter = float3(vBorder.z + fPtMargin, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, fPtSize2));

					vCenter = float3(vBorder.x - fPtMargin, 0.0f, vBorder.w + fPtMargin);
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, -fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, -fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize, 0.0f, 0.0f));
					pvData[uCV++] = (float3)(vCenter + float3(fPtSize2, 0.0f, fPtSize2));
					pvData[uCV++] = (float3)(vCenter + float3(0.0f, 0.0f, fPtSize));
					pvData[uCV++] = (float3)(vCenter + float3(-fPtSize2, 0.0f, fPtSize2));
				}

				g_xRenderStates.pTransformHandlerVB->unlock();
			}

			SMMATRIX mViewProj;
			SMMATRIX mWorld;
			switch(view)
			{
			case X2D_FRONT:
				mWorld = SMMatrixRotationX(-SM_PIDIV2);
				break;
			case X2D_SIDE:
				mWorld = SMMatrixRotationY(-SM_PIDIV2) * SMMatrixRotationZ(-SM_PIDIV2);
				break;
			}
			IGXRasterizerState *pOldRS = pDevice->getRasterizerState();
			pDevice->setRasterizerState(g_xRenderStates.pRSSolidNoCull);
			Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mViewProj);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, g_xRenderStates.idColoredShaderVS, "g_mWVP", &SMMatrixTranspose(mWorld * mViewProj), 4);
			pDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);
			SGCore_ShaderBind(g_xRenderStates.idColoredShaderKit);
			pDevice->setRenderBuffer(g_xRenderStates.pTransformHandlerRB);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, g_xRenderStates.idColoredShaderPS, "g_vColor", &float4(1.0f, 1.0f, 1.0f, 1.0f), 1);
			if(g_xState.xformType == X2DXF_SCALE)
			{
				pDevice->setIndexBuffer(g_xRenderStates.pTransformHandlerScaleIB);
				pDevice->drawIndexed(32, 16, 0, 0);
			}
			else if(g_xState.xformType == X2DXF_ROTATE)
			{
				pDevice->setIndexBuffer(g_xRenderStates.pTransformHandlerRotateIB);
				pDevice->drawIndexed(32, 24, 0, 0);
			}
			SGCore_ShaderUnBind();
			pDevice->setRasterizerState(pOldRS);
			mem_release(pOldRS);
		}
	}
}

void XLoadLevel(const char *szName)
{
	XResetLevel();

	SLevel_Load(szName, false);
	char szCaption[256];
	sprintf(szCaption, "%s - [%s] | %s", MAIN_WINDOW_TITLE, szName, SKYXENGINE_VERSION4EDITORS);
	SetWindowText(g_hWndMain, szCaption);

	for(UINT ic = 0, il = g_pEditableSystems.size(); ic < il; ++ic)
	{
		IXEditable *pEditable = g_pEditableSystems[ic];
		for(ID i = 0, l = pEditable->getObjectCount(); i < l; ++i)
		{
			g_pLevelObjects.push_back(pEditable->getObject(i));
		}
	}

	
}

void XResetLevel()
{
	SetWindowText(g_hWndMain, MAIN_WINDOW_TITLE " | " SKYXENGINE_VERSION4EDITORS);

	SLevel_Clear();
#if 0
	ID gid = SLight_GetGlobal();
	if(ID_VALID(gid))
	{
		SLight_DeleteLight(gid);
	}
#endif

	for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
	{
		mem_release(g_pLevelObjects[i]);
	}
	g_pLevelObjects.clear();
}

bool XSaveLevel(const char *szNewName)
{
	if(szNewName)
	{
		SLevel_Save(szNewName);
		return(true);
	}
	
	if(SLevel_GetName()[0])
	{
		return(XSaveLevel(SLevel_GetName()));
	}
		
	char szPath[1024];
	szPath[0] = 0;
	char szName[1024];
	/*if(gui_func::dialogs::SelectDirOwn(szName, szPath, Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), "Save level", false, true, 0))
	{
		return(XSaveLevel(szName));
	}*/
	
	return(false);
}

void XDrawBorder(GXCOLOR color, const float3_t &vA, const float3_t &vB, const float3_t &vC, const float3_t &vD, float fViewportScale)
{
	float2_t ta, tb, tc, td;
	if(vA.x == vB.x && vB.x == vC.x)
	{
		ta = float2_t(vA.y, vA.z);
		tb = float2_t(vB.y, vB.z);
		tc = float2_t(vC.y, vC.z);
		td = float2_t(vD.y, vD.z);
	}
	else if(vA.y == vB.y && vB.y == vC.y)
	{
		ta = float2_t(vA.x, vA.z);
		tb = float2_t(vB.x, vB.z);
		tc = float2_t(vC.x, vC.z);
		td = float2_t(vD.x, vD.z);
	}
	else
	{
		ta = float2_t(vA.x, vA.y);
		tb = float2_t(vB.x, vB.y);
		tc = float2_t(vC.x, vC.y);
		td = float2_t(vD.x, vD.y);
	}

	XBorderVertex *pData;
	if(g_pBorderVertexBuffer->lock((void**)&pData, GXBL_WRITE))
	{
		pData[0] = {vA, ta / fViewportScale / 6.0f};
		pData[1] = {vB, tb / fViewportScale / 6.0f};
		pData[2] = {vC, tc / fViewportScale / 6.0f};
		pData[3] = {vD, td / fViewportScale / 6.0f};
		pData[4] = pData[0];
		g_pBorderVertexBuffer->unlock();
	}

	IGXContext *pDevice = SGCore_GetDXDevice();
	IGXBlendState *pOldBlendState = pDevice->getBlendState();
	SGCore_ShaderBind(g_xRenderStates.idTexturedShaderKit);

	SMMATRIX mViewProj;
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mViewProj);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, g_xRenderStates.idTexturedShaderVS, "g_mWVP", &SMMatrixTranspose(mViewProj), 4);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, g_xRenderStates.idTexturedShaderPS, "g_vColor", &GXCOLOR_COLORVECTOR_ARGB(color), 1);
	pDevice->setTexture(SGCore_LoadTexGetTex(SRender_EditorGetDashedTex()));
	pDevice->setBlendState(g_xRenderStates.pBlendAlpha);
	pDevice->setRenderBuffer(g_pBorderRenderBuffer);
	pDevice->setPrimitiveTopology(GXPT_LINESTRIP);
	pDevice->drawPrimitive(0, 4);
	SGCore_ShaderUnBind();

	pDevice->setBlendState(pOldBlendState);
	mem_release(pOldBlendState);
}

void XUpdateUndoRedo();
bool XExecCommand(CCommand *pCommand)
{
	if(g_pUndoManager->execCommand(pCommand))
	{
		XUpdateUndoRedo();
		return(true);
	}
	return(false);
}

void XUpdateSelectionBound()
{
	g_xState.bHasSelection = false;
	float3 vMin, vMax;

	for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
	{
		if(g_pLevelObjects[i]->isSelected())
		{
			g_pLevelObjects[i]->getBound(&vMin, &vMax);
			if(!g_xState.bHasSelection)
			{
				g_xState.bHasSelection = true;
				g_xState.vSelectionBoundMax = vMax;
				g_xState.vSelectionBoundMin = vMin;
			}
			else
			{
				g_xState.vSelectionBoundMax = (float3)SMVectorMax(g_xState.vSelectionBoundMax, vMax);
				g_xState.vSelectionBoundMin = (float3)SMVectorMin(g_xState.vSelectionBoundMin, vMin);
			}
		}
	}
}

bool XRayCast(X_WINDOW_POS wnd)
{
	float3 vStart, vEnd, vPos, vCamPos;
	if(!g_xConfig.m_pViewportCamera[wnd])
	{
		return(false);
	}
	g_xConfig.m_pViewportCamera[wnd]->getPosition(&vCamPos);
	switch(g_xConfig.m_x2DView[wnd])
	{
	case X2D_NONE:
		return(false);
	case X2D_TOP:
		vStart = float3(g_xState.vWorldMousePos.x, vCamPos.y, g_xState.vWorldMousePos.y);
		vEnd = float3(0.0f, -2000.0f, 0.0f);
		break;
	case X2D_FRONT:
		vStart = float3(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, vCamPos.z);
		vEnd = float3(0.0f, 0.0f, 2000.0f);
		break;
	case X2D_SIDE:
		vStart = float3(vCamPos.x, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
		vEnd = float3(-2000.0f, 0.0f, 0.0f);
		break;
	}
	vEnd += vStart;
	for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
	{
		if(g_pLevelObjects[i]->isSelected() && g_pLevelObjects[i]->rayTest(vStart, vEnd, &vPos, NULL))
		{
			return(true);
		}
	}
	return(false);
}

bool XIsMouseInSelection(X_WINDOW_POS wnd)
{
	if(!g_xConfig.m_pViewportCamera[wnd] || !g_xState.bHasSelection)
	{
		return(false);
	}
	const float2_t &fMPos = g_xState.vWorldMousePos;
	switch(g_xConfig.m_x2DView[wnd])
	{
	case X2D_TOP:
		return(fMPos.x >= g_xState.vSelectionBoundMin.x && fMPos.x <= g_xState.vSelectionBoundMax.x && 
			fMPos.y >= g_xState.vSelectionBoundMin.z && fMPos.y <= g_xState.vSelectionBoundMax.z);

	case X2D_FRONT:
		return(fMPos.x >= g_xState.vSelectionBoundMin.x && fMPos.x <= g_xState.vSelectionBoundMax.x &&
			fMPos.y >= g_xState.vSelectionBoundMin.y && fMPos.y <= g_xState.vSelectionBoundMax.y);

	case X2D_SIDE:
		return(fMPos.x >= g_xState.vSelectionBoundMin.z && fMPos.x <= g_xState.vSelectionBoundMax.z &&
			fMPos.y >= g_xState.vSelectionBoundMin.y && fMPos.y <= g_xState.vSelectionBoundMax.y);
	}
	return(false);
}
