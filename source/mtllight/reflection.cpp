
#include "reflection.h"

Reflection::Reflection()
{
	IsComNow = true;
	SurfaceReflect = 0;
	TextureReflect = 0;
	TextureCubeReflect = 0;
	SurfaceZBuffer = 0;
	BackBuffer = 0;
	LastSurfaceZBuffer = 0;
	ZNear = 0.0f;
	CountUpdate = 0;

	for (int i = 0; i < 6; ++i)
	{
		//IDArr[i] = -1;
		ReflectFrustum[i] = 0;
		CubeReflectSurface[i] = 0;
	}
}

Reflection::~Reflection()
{
	for (int i = 0; i < 6; ++i)
	{
		mem_release_del(ReflectFrustum[i]);
	}

	mem_release_del(BackBuffer);
	mem_release_del(SurfaceZBuffer);
	mem_release_del(LastSurfaceZBuffer);
	mem_release_del(TextureReflect);
	mem_release_del(SurfaceReflect);

	for (int i = 0; i < 6; i++)
	{
		mem_release_del(CubeReflectSurface[i]);
	}

	mem_release_del(TextureCubeReflect);
}

void Reflection::SetIDArr(ID id, int cube, ID idarr)
{
	if (!(cube >= 0 && cube < 6))
		return;

	if (id >= IDArr.size())
	{
		ID* tmparr = new ID[6];
		for (int i = 0; i < 6; ++i)
			tmparr[i] = -1;
		IDArr[id] = tmparr;
	}

	IDArr[id][cube] = idarr;
}

int Reflection::GetCountIDArrs()
{
	return IDArr.size();
}

ID Reflection::GetIDArr(ID id, int cube)
{
	if (!(cube >= 0 && cube < 6))
		return -1;

	if (id < 0 || !(cube >= 0 && cube < 6) || !(id < IDArr.size()))
		return -2;

	return IDArr[id][cube];
}

void Reflection::OnLostDevice()
{
	mem_release_del(TextureReflect);
	mem_release_del(TextureCubeReflect);
	mem_release_del(SurfaceZBuffer);

	mem_release_del(SurfaceReflect);

	mem_release_del(CubeReflectSurface[0]);
	mem_release_del(CubeReflectSurface[1]);
	mem_release_del(CubeReflectSurface[2]);
	mem_release_del(CubeReflectSurface[3]);
	mem_release_del(CubeReflectSurface[4]);
	mem_release_del(CubeReflectSurface[5]);
}

void Reflection::OnResetDevice()
{
	if (TypeRef == MTLTYPE_REFLECT_PLANE)
	{
		D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureReflect);
		MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &SurfaceZBuffer, NULL);
	}
	else if (TypeRef == MTLTYPE_REFLECT_CUBE_STATIC || TypeRef == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureReflect);
		D3DXCreateCubeTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureCubeReflect);
		//MLSet::DXDevice->CreateCubeTexture(MLSet::SizeTexReflection.x, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureCubeReflect, NULL);
		MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.x, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &SurfaceZBuffer, NULL);
		//TextureCubeReflect->SetAutoGenFilterType(D3DTEXF_LINEAR);
	}

	CountUpdate = 0;
}

void Reflection::Init(MTLTYPE_REFLECT howref)
{
	TypeRef = howref;
	if (TypeRef == MTLTYPE_REFLECT_PLANE)
	{
		if (!ReflectFrustum[0])
			ReflectFrustum[0] = SGCore_CrFrustum();

		if (!TextureReflect)
			D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureReflect);

		if (!SurfaceZBuffer)
			MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &SurfaceZBuffer, NULL);
	
	}
	else if (TypeRef == MTLTYPE_REFLECT_CUBE_STATIC || TypeRef == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (!ReflectFrustum[i])
				ReflectFrustum[i] = SGCore_CrFrustum();
		}

		D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureReflect);

		if (!TextureCubeReflect)
			D3DXCreateCubeTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &TextureCubeReflect);
			//MLSet::DXDevice->CreateCubeTexture(MLSet::SizeTexReflection.x, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureCubeReflect, NULL);
		
		if (!SurfaceZBuffer)
			MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.x, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &SurfaceZBuffer, NULL);
	
	}
	else
	{

	}
	

	CubeReflectSurface[0] = 0;
	CubeReflectSurface[1] = 0;
	CubeReflectSurface[2] = 0;
	CubeReflectSurface[3] = 0;
	CubeReflectSurface[4] = 0;
	CubeReflectSurface[5] = 0;

	CountUpdate = 0;
}

void Reflection::PreRenderRefPlane(D3DXPLANE* plane)
{
	if (!plane)
	{
		g_fnReportf(REPORT_MSG_LEVEL_WARNING, "%s - sxmtllight [reflection]: plane is NULL", GEN_MSG_LOCATION);
		return;
	}

	Plane = *plane;

	float4x4 viewmat;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &viewmat);

	D3DXMATRIX matReflect, matView;
	D3DXMatrixReflect(&matReflect, &Plane);
	
	viewmat = float4x4(matReflect) * viewmat;
	viewmat._12 = -viewmat._12;
	viewmat._22 = -viewmat._22;
	viewmat._32 = -viewmat._32;
	viewmat._42 = -viewmat._42;

	ReflectFrustum[0]->Update(&viewmat, &(MLSet::RefMProjPlane));

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &OldMatView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &OldMatProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &OldMatViewProj);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &viewmat);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &MLSet::RefMProjPlane);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(viewmat * MLSet::RefMProjPlane));

	MLSet::DXDevice->GetRenderTarget(0, &BackBuffer);

	mem_release_del(SurfaceReflect);
	TextureReflect->GetSurfaceLevel(0, &SurfaceReflect);
	MLSet::DXDevice->SetRenderTarget(0, SurfaceReflect);
	//MLSet::DXDevice->GetDepthStencilSurface(&LastSurfaceZBuffer);
	//MLSet::DXDevice->SetDepthStencilSurface(SurfaceZBuffer);
	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0L);
}

void Reflection::PostRenderRefPlane()
{
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &OldMatView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &OldMatProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &OldMatViewProj);

	MLSet::DXDevice->SetRenderTarget(0, BackBuffer);
	//MLSet::DXDevice->SetDepthStencilSurface(LastSurfaceZBuffer);

	mem_release_del(BackBuffer);
	mem_release_del(LastSurfaceZBuffer);
	mem_release_del(SurfaceReflect);

	if (GetAsyncKeyState(VK_NUMPAD9))
	{
		char tmpstr[256];
		sprintf(tmpstr, "C:\\1\\reflectionreflection.png");
		D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_PNG, SurfaceReflect, NULL, 0);
	}
}

IDirect3DTexture9* Reflection::GetRefPlaneTex()
{
	return TextureReflect;
}

////////////////////////

void Reflection::SetMinMax(float3_t* min, float3_t* max)
{
	//если новые размеры отличаются от старых значит надо обновить отражения, если они были статические
	if (PosMax.x != max->x || PosMax.y != max->y || PosMax.z != max->z || PosMin.x != min->x || PosMin.y != min->y || PosMin.z != min->z)
		CountUpdate = 0;

	PosMax = *max;
	PosMin = *min;
}

void Reflection::BeginRenderRefCube(float3_t* center)
{
	if (!center)
	{
		g_fnReportf(REPORT_MSG_LEVEL_WARNING, "%s - sxmtllight [reflection]: position center is NULL", GEN_MSG_LOCATION);
		return;
	}

	Position = *center;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &OldMatView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &OldMatProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &OldMatViewProj);

	MLSet::DXDevice->GetRenderTarget(0, &BackBuffer);
	//MLSet::DXDevice->GetDepthStencilSurface(&LastSurfaceZBuffer);
	//MLSet::DXDevice->SetDepthStencilSurface(SurfaceZBuffer);
}

void Reflection::PreRenderRefCube(int cube, float4x4* world)
{
	//MLSet::DXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)MLSet::RefMProjCube));

	PositionReflect = SMVector3Transform(Position, *world);

	MatrixView = SMMatrixLookAtLH(
		PositionReflect,
		((MLSet::OrientedCube[cube] + PositionReflect)),
		MLSet::UpVectorsCube[cube]);

	/*MLSet::DXDevice->SetTransform(D3DTS_VIEW, &MatrixView.operator D3DXMATRIX());

	MLSet::DXDevice->SetTransform(D3DTS_WORLD1, &((D3DXMATRIX)(MatrixView * MLSet::RefMProjCube)));*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &MatrixView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &MLSet::RefMProjCube);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(MatrixView * MLSet::RefMProjCube));

	ReflectFrustum[cube]->Update(&float4x4(MatrixView), &MLSet::RefMProjCube);

	mem_release_del(SurfaceReflect);
	TextureReflect->GetSurfaceLevel(0, &SurfaceReflect);
	MLSet::DXDevice->SetRenderTarget(0, SurfaceReflect);

	MLSet::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0);
}

void Reflection::PostRenderRefCube(int cube)
{
	LPDIRECT3DSURFACE9 BackBuf;

	mem_release_del(CubeReflectSurface[cube]);
	mem_release(SurfaceReflect);

	DWORD alphablend, alphatest, zenable, zwriteenable;

	MLSet::DXDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &alphablend);
	MLSet::DXDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &alphatest);
	MLSet::DXDevice->GetRenderState(D3DRS_ZENABLE, &zenable);
	MLSet::DXDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zwriteenable);

	MLSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	TextureCubeReflect->GetCubeMapSurface((D3DCUBEMAP_FACES)cube, 0, &CubeReflectSurface[cube]);
	MLSet::DXDevice->SetRenderTarget(0, CubeReflectSurface[cube]);
	//MLSet::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 255, 0, 0), 1, 0);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::ScreenOut);

	MLSet::DXDevice->SetTexture(0, TextureReflect);
	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	MLSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, alphablend);
	MLSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, alphatest);
	MLSet::DXDevice->SetRenderState(D3DRS_ZENABLE, zenable);
	MLSet::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, zwriteenable);


	/*if(GetAsyncKeyState(VK_NUMPAD5))
	{
		char tmpstr[1024];
		sprintf(tmpstr,"C:\\1\\reflectioncube_%d.png",cube);
		D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_PNG, CubeReflectSurface[cube], NULL, 0);
	}*/
	
	mem_release_del(CubeReflectSurface[cube]);
}

void Reflection::EndRenderRefCube(float3_t* viewpos)
{
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &OldMatView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &OldMatProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &OldMatViewProj);

	MLSet::DXDevice->SetRenderTarget(0, BackBuffer);
	//MLSet::DXDevice->SetDepthStencilSurface(LastSurfaceZBuffer);
	//mem_release_del(LastSurfaceZBuffer);
	mem_release_del(BackBuffer);

	UpdateCountUpdate(viewpos);
}

bool Reflection::UpdateCountUpdate(float3_t* viewpos)
{
	if (!viewpos)
		return false;

	if (TypeRef == MTLTYPE_REFLECT_CUBE_STATIC)
	{
		if (CountUpdate < MTL_REF_UPDATE_MAX_COUNT_FOR_STATIC)
		{
			++(CountUpdate);
			return true;
		}
		else
			return false;
	}

	float dist = SMVector3Distance(PositionReflect, (*viewpos)) - SMVector3Length((PosMax - PosMin) *0.5f);
	if (dist < MTL_REF_UPDATE_L0_DIST)
		CountUpdate = -1;
	else if (dist < MTL_REF_UPDATE_L1_DIST && dist > MTL_REF_UPDATE_L0_DIST)
	{
		if (CountUpdate > 1)
			CountUpdate = -1;
	}
	else if (dist < MTL_REF_UPDATE_L2_DIST && dist > MTL_REF_UPDATE_L1_DIST)
	{
		if (CountUpdate > 2)
			CountUpdate = -1;
	}
	else if (dist > MTL_REF_UPDATE_L2_DIST)
	{
		CountUpdate = 4;
	}

	++CountUpdate;

	return (CountUpdate == 0);
}

bool Reflection::AllowedRender()
{
	if (TypeRef == MTLTYPE_REFLECT_CUBE_STATIC)
	{
		if (CountUpdate <= MTL_REF_UPDATE_MAX_COUNT_FOR_STATIC)
		{
			++(CountUpdate);
			return true;
		}
		else
			return false;
	}

	return (CountUpdate == 0);
}

void Reflection::NullingCountUpdate()
{
	CountUpdate = 0;
}

IDirect3DCubeTexture9* Reflection::GetRefCubeTex()
{
	return TextureCubeReflect;
}
