
#include <gcore\sxgcore.h>

//флаги компил€ции шейдеров
#define SHADER_DEBUG D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3DXSHADER_AVOID_FLOW_CONTROL | D3DXSHADER_SKIPOPTIMIZATION
#define SHADER_RELEASE D3DXSHADER_OPTIMIZATION_LEVEL3 | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3DXSHADER_PARTIALPRECISION | D3DXSHADER_PREFER_FLOW_CONTROL

//определ€ем флаг компилции шейдеров
#if defined(DEBUG) || defined(_DEBUG) 
#define SHADER_FLAGS SHADER_DEBUG 
#else 
#define SHADER_FLAGS SHADER_RELEASE 
#endif

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

IDirect3DDevice9* DXDevice = 0;
D3DPRESENT_PARAMETERS D3DAPP;
IDirect3D9* d3d9 = 0;
ID3DXFont* FPSText = 0;

#include <string\string.cpp>
#include <gcore\shader.cpp>
#include <gcore\creatortextures.cpp>
#include <gcore\loadertextures.cpp>
#include <gcore\baseobject.cpp>
#include <gcore\workmodel.cpp>
#include <gcore\camera.cpp>
#include <gcore\\loader_static.cpp>

#define SXGCORE_VERSION 1

ShaderManager* MShaders = 0;
CreatorTextures* MRenderTargets = 0;
LoaderTextures* MTextures = 0;


#include <gcore\dxdevice.cpp>

long SGCore_0GetVersion()
{
	return SXGCORE_VERSION;
}

void SGCore_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void SGCore_0Create(const char* name, HWND hwnd, int width, int heigth, bool windowed, DWORD create_device_flags, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				reportf(-1, "%s - none unic name, sxgcore", gen_msg_location);
			}
			else
			{
				int cerr = InitD3D(hwnd, windowed, width, heigth, create_device_flags);
				if (cerr == SXGC_ERR_NON_DETECTED_D3D)
					reportf(-1, "%s - none detected d3d, sxgcore", gen_msg_location);
				else if (cerr == SXGC_ERR_FAILED_INIT_D3D)
					reportf(-1, "%s - failed initialized d3d, sxgcore", gen_msg_location);

				MShaders = new ShaderManager();
				MRenderTargets = new CreatorTextures();
				MTextures = new LoaderTextures();
			}
		}
		else
		{
			int cerr = InitD3D(hwnd, windowed, width, heigth, create_device_flags);
			if (cerr == SXGC_ERR_NON_DETECTED_D3D)
				reportf(-1, "%s - none detected d3d, sxgcore", gen_msg_location);
			else if (cerr == SXGC_ERR_FAILED_INIT_D3D)
				reportf(-1, "%s - failed initialized d3d, sxgcore", gen_msg_location);
			
			MShaders = new ShaderManager();
			MRenderTargets = new CreatorTextures();
			MTextures = new LoaderTextures();
		}
	}
	else
		reportf(-1, "%s - not init argument [name], sxgcore", gen_msg_location);
}

IDirect3DDevice9* SGCore_GetDXDevice()
{
	return DXDevice;
}

void SGCore_DbgMsg(const char* format, ...)
{
	if (!DXDevice || !FPSText)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);
	
	va_list va;
	char buf[SXGC_STR_SIZE_DBG_MSG];
	va_start(va, format);
	vsprintf_s(buf, SXGC_STR_SIZE_DBG_MSG, format, va);
	va_end(va);

	RECT rect;
	rect.top = 10;
	rect.left = 10;
	rect.right = D3DAPP.BackBufferWidth - 10;
	rect.bottom = D3DAPP.BackBufferHeight - 10;
	FPSText->DrawText(0, buf, -1, &rect, 0, 0xff000000);
}

void SGCore_OnLostDevice()
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	FPSText->OnLostDevice();
	MRenderTargets->OnLostDevice();
}

int SGCore_OnDeviceReset(int width, int heigth, bool windewed)
{
	D3DAPP.BackBufferWidth = width;
	D3DAPP.BackBufferHeight = heigth;
	D3DAPP.Windowed = windewed;
	return (FAILED(DXDevice->Reset(&D3DAPP)));
}

void SGCore_OnResetDevice()
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	FPSText->OnResetDevice();
	MRenderTargets->OnResetDevice();
}

DWORD SGCore_ShaderLoad(int type_shader, const char* path, const char* name, int is_check_double, D3DXMACRO* macro)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->Load(type_shader, path, name, is_check_double, macro);
}

void SGCore_ShaderUpdateN(int type_shader, const char* name, D3DXMACRO macro[])
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	MShaders->Update(type_shader, name, macro);
}

void SGCore_ShaderUpdate(int type_shader, DWORD id, D3DXMACRO macro[])
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	MShaders->Update(type_shader, id, macro);
}

void SGCore_ShaderReloadAll()
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	MShaders->ReloadAll();
}

DWORD SGCore_ShaderGetID(int type_shader, const char* shader)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->GetID(type_shader, shader);
}

void SGCore_ShaderBindN(int type_shader, const char* shader)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->Bind(type_shader, shader);
}

void SGCore_ShaderBind(int type_shader, DWORD shader)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->Bind(type_shader, shader);
}


void SGCore_ShaderUnBind()
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->UnBind();
}

void SGCore_ShaderSetVRFN(int type_shader, const char* name_shader, const char* name_var, void* data)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->SetValueRegisterF(type_shader, name_shader, name_var, data);
}

void SGCore_ShaderSetVRF(int type_shader, DWORD num_shader, const char* name_var, void* data)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->SetValueRegisterF(type_shader, num_shader, name_var, data);
}

void SGCore_ShaderSetVRIN(int type_shader, const char* name_shader, const char* name_var, void* data)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->SetValueRegisterI(type_shader, name_shader, name_var, data);
}

void SGCore_ShaderSetVRI(int type_shader, DWORD num_shader, const char* name_var, void* data)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->SetValueRegisterI(type_shader, num_shader, name_var, data);
}


DWORD SGCore_ShaderIsExist(int type_shader, const char* name)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->IsExist(type_shader, name);
}

bool SGCore_ShaderIsValidate(int type_shader, DWORD id)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MShaders->IsValidate(type_shader, id);
}

void SGCore_ShaderGetPath(int type_shader, DWORD id, char* path)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	MShaders->GetPath(type_shader, id, path);
}

void SGCore_ShaderGetName(int type_shader, DWORD id, char* name)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	MShaders->GetName(type_shader, id, name);
}

void SGCore_ShaderSetStdPath(const char* path)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	MShaders->SetStdPath(path);
}

void SGCore_ShaderGetStdPath(char* path)
{
	if (!DXDevice || !MShaders)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	MShaders->GetStdPath(path);
}

////////////////////////

void SGCore_LoadTexStdPath(const char* path)
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	MTextures->SetStdPath(path);
}

DWORD SGCore_LoadTexAddName(const char* name)
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MTextures->AddName(name);
}

DWORD SGCore_LoadTexGetID(const char* name)
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MTextures->GetID(name);
}

void SGCore_LoadTexGetName(DWORD id, char* name)
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MTextures->GetName(id,name);
}

DWORD SGCore_LoadTexCreate(const char* name, IDirect3DTexture9* tex)
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MTextures->Create(name, tex);
}

DWORD SGCore_LoadTexUpdateN(const char* name)
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MTextures->Update(name);
}

void SGCore_LoadTexUpdate(DWORD id)
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MTextures->Update(id);
}

IDirect3DTexture9* SGCore_GetTex(DWORD id)
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MTextures->GetTexture(id);
}

void SGCore_LoadTexLoadTextures()
{
	if (!DXDevice || !MTextures)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MTextures->LoadTextures();
}

//////////////////////////

DWORD SGCore_RTAdd(UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, const char* name, float coeffullscreen)
{
	if (!DXDevice || !MRenderTargets)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MRenderTargets->Add(width, height, levels, usage, format, pool, name, coeffullscreen);
}

void SGCore_RTDeleteN(const char* text)
{
	if (!DXDevice || !MRenderTargets)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MRenderTargets->Delete(text);
}

void SGCore_RTDelete(DWORD num)
{
	if (!DXDevice || !MRenderTargets)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MRenderTargets->Delete(num);
}

DWORD SGCore_RTGetNum(const char* text)
{
	if (!DXDevice || !MRenderTargets)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MRenderTargets->GetNum(text);
}

void SGCore_RTOnLostDevice()
{
	if (!DXDevice || !MRenderTargets)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MRenderTargets->OnLostDevice();
}

void SGCore_RTOnResetDevice()
{
	if (!DXDevice || !MRenderTargets)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MRenderTargets->OnResetDevice();
}

IDirect3DTexture9* SGCore_RTGetTextureN(const char* text)
{
	if (!DXDevice || !MRenderTargets)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MRenderTargets->GetTexture(text);
}

IDirect3DTexture9* SGCore_RTGetTexture(DWORD num)
{
	if (!DXDevice || !MRenderTargets)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	return MRenderTargets->GetTexture(num);
}

////////////

void SGCore_FCreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, UINT iSideCount)
{
	if (!DXDevice)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	CreateCone(fTopRadius, fBottomRadius, fHeight, ppMesh, DXDevice, iSideCount);
}

void SGCore_FCompBoundBox(IDirect3DVertexBuffer9* vertex_buffer, ISXBound** bound, DWORD count_vert, DWORD bytepervert)
{
	if (!DXDevice)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	ComputeBoundingBox(vertex_buffer, bound, count_vert, bytepervert);
}

void SGCore_FCompBoundBox2(IDirect3DVertexBuffer9* vertex_buffer, ISXBound* bound, DWORD count_vert, DWORD bytepervert)
{
	if (!DXDevice)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	ComputeBoundingBox2(vertex_buffer, bound, count_vert, bytepervert);
}

void SGCore_FCreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh)
{
	if (!DXDevice)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);

	CreateBoundingBoxMesh(min, max, bbmesh,DXDevice);
}

void SGCore_OptimizeIndecesInSubsetWord(WORD* ib, WORD numFaces, WORD numVerts)
{
	if (!DXDevice)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);
	OptimizeIndecesInSubsetWord(ib, numFaces, numVerts);
}

void SGCore_OptimizeIndecesInSubsetDword(DWORD* ib, DWORD numFaces, DWORD numVerts)
{
	if (!DXDevice)
		reportf(-1, "%s - sxcore is not init", gen_msg_location);
	OptimizeIndecesInSubsetDword(ib, numFaces, numVerts);
}


bool SGCore_0InPos2D(float3* min, float3* max, float3* pos)
{
	return InPosition2D(min, max, pos);
}

bool SGCore_0InPosAbs2D(float3* min, float3* max, float3* pos)
{
	return InPositionAbs2D(min, max, pos);
}

int SGCore_0CountPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPoints2D(min, max, p1, p2, p3);
}

int SGCore_0CountPosPointsAbs2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPointsAbs2D(min, max, p1, p2, p3);
}

bool SGCore_0InPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return InPositionPoints2D(min, max, p1, p2, p3);
}

bool SGCore_0InPos3D(float3* min, float3* max, float3* pos)
{
	return InPosition3D(min, max, pos);
}

bool SGCore_0InPosAbs3D(float3* min, float3* max, float3* pos)
{
	return InPositionAbs3D(min, max, pos);
}

int SGCore_0CountPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPoints3D(min, max, p1, p2, p3);
}

int SGCore_0CountPosPointsAbs3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPointsAbs3D(min, max, p1, p2, p3);
}

bool SGCore_0InPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return InPositionPoints3D(min, max, p1, p2, p3);
}

void SGCore_0ComBoundBoxArr8(ISXBound* bound, ISXBound** bound_arr)
{
	ComputeBoundingBoxArr8(bound, bound_arr);
}

void SGCore_0ComBoundBoxArr4(ISXBound* bound, ISXBound** bound_arr)
{
	ComputeBoundingBoxArr4(bound, bound_arr);
}

////////////

ISXFrustum* SXGCore_CrFrustum()
{
	return new Frustum();
}

ISXCamera* SXGCore_CrCamera()
{
	return new Camera();
}

ISXTransObject* SXGCore_CrTransObject()
{
	return new SXTransObject();
}

ISXBound* SXGCore_CrBound()
{
	return new SXBound();
}

//////////////////////

ISXCamera* SGCore_CrCamera()
{
	return new Camera();
}

ISXBound* SGCore_CrBound()
{
	return new SXBound();
}