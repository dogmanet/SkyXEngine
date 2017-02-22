
#include <mtl_ligth\\reflection.h>

Reflection::Reflection()
{
	IsComNow = true;
	SurfaceReflect = 0;
	TextureReflect = 0;
	TextureCubeReflect = 0;
	SurfaceZBuffer = 0;
	BackBuffer = 0;
	LastSurfaceZBuffer = 0;

	CubeReflectSurface[0] = 0;
	CubeReflectSurface[1] = 0;
	CubeReflectSurface[2] = 0;
	CubeReflectSurface[3] = 0;
	CubeReflectSurface[4] = 0;
	CubeReflectSurface[5] = 0;

	for (int i = 0; i < 6; ++i)
		IDArr[i] = -1;
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

	for (int i = 0; i<6; i++)
		mem_release_del(CubeReflectSurface[i]);

	mem_release_del(TextureCubeReflect);
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
	if (TypeRef == 0)
	{
		D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureReflect);
		MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &SurfaceZBuffer, NULL);
	}
	else if (TypeRef == 1)
	{
		MLSet::DXDevice->CreateCubeTexture(MLSet::SizeTexReflection.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureCubeReflect, NULL);
		MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.x, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &SurfaceZBuffer, NULL);
	}
}

void Reflection::Init(int howref)
{
	TypeRef = howref;
	if (TypeRef == 0)
	{
		ReflectFrustum[0] = SGCore_CrFrustum();
		D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureReflect);
		MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &SurfaceZBuffer, NULL);
	
	}
	else if (TypeRef == 1)
	{
		for (int i = 0; i < 6;++i)
			ReflectFrustum[i] = SGCore_CrFrustum();
		MLSet::DXDevice->CreateCubeTexture(MLSet::SizeTexReflection.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureCubeReflect, NULL);
		MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.x, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &SurfaceZBuffer, NULL);
	}

	

	CubeReflectSurface[0] = 0;
	CubeReflectSurface[1] = 0;
	CubeReflectSurface[2] = 0;
	CubeReflectSurface[3] = 0;
	CubeReflectSurface[4] = 0;
	CubeReflectSurface[5] = 0;
}

void Reflection::PreRenderRefPlane(float4x4* world)
{
	D3DXMATRIX MatrixView;
	PositionReflect = SMVector3Transform(Position, *world);
	//PositionReflect = SkyXEngine::Core::Data::ConstCurrentCameraPosition;
	float4x4 viewmat;
	//Core::Data::ObjCamera->GetViewMatrix(&viewmat);
	GData::DXDevice->GetTransform(D3DTS_VIEW, &MatrixView);
	viewmat = MatrixView;

	float4x4 tmpworld;
	tmpworld = *world;
	tmpworld._41 = tmpworld._42 = tmpworld._43 = tmpworld._14 = tmpworld._24 = tmpworld._34 = 0;
	tmpworld._44 = 1.f;

	D3DXMATRIX matReflect, matView;
	D3DXPLANE tmpplane;
	D3DXPlaneTransform(&tmpplane, &Plane, &(tmpworld.operator D3DXMATRIX()));
	D3DXMatrixReflect(&matReflect, &tmpplane);

	D3DXMatrixMultiply(&MatrixView, &matReflect, &(viewmat.operator D3DXMATRIX()));

	MatrixView._12 = -MatrixView._12;
	MatrixView._22 = -MatrixView._22;
	MatrixView._32 = -MatrixView._32;
	MatrixView._42 = -MatrixView._42;

	MLSet::DXDevice->GetTransform(D3DTS_VIEW, &OldMatView);
	MLSet::DXDevice->GetTransform(D3DTS_PROJECTION, &OldMatProj);
	MLSet::DXDevice->GetTransform(D3DTS_WORLD1, &OldMatViewProj);

	float4x4 tmpproj = SMMatrixPerspectiveFovLH(MLSet::ProjFov, MLSet::ProjRatio, 0.25, 100);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION, &(tmpproj.operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_VIEW, &MatrixView);

	MLSet::DXDevice->SetTransform(D3DTS_WORLD1, &((float4x4(MatrixView) * tmpproj).operator D3DXMATRIX()));

	ReflectFrustum[0]->Update(&float4x4(MatrixView), &(tmpproj));

	MLSet::DXDevice->GetRenderTarget(0, &BackBuffer);

	mem_release_del(SurfaceReflect);
	TextureReflect->GetSurfaceLevel(0, &SurfaceReflect);
	MLSet::DXDevice->SetRenderTarget(0, SurfaceReflect);
	MLSet::DXDevice->GetDepthStencilSurface(&LastSurfaceZBuffer);
	MLSet::DXDevice->SetDepthStencilSurface(SurfaceZBuffer);
	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0L);
}

void Reflection::PostRenderRefPlane()
{
	MLSet::DXDevice->SetTransform(D3DTS_VIEW, &OldMatView);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION, &OldMatProj);
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1, &OldMatViewProj);
	MLSet::DXDevice->SetRenderTarget(0, BackBuffer);
	MLSet::DXDevice->SetDepthStencilSurface(LastSurfaceZBuffer);

	if (GetAsyncKeyState(VK_NUMPAD9))
	{
		char tmpstr[256];
		sprintf(tmpstr, "C:\\1\\reflectionreflection.png");
		D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_JPG, SurfaceReflect, NULL, 0);
	}

	mem_release_del(BackBuffer);
	mem_release_del(LastSurfaceZBuffer);
}

inline IDirect3DTexture9* Reflection::GetRefPlaneTex()
{
	return TextureReflect;
}

////////////////////////

void Reflection::BeginRenderRefCube()
{
	MLSet::DXDevice->GetTransform(D3DTS_VIEW, &OldMatView);
	MLSet::DXDevice->GetTransform(D3DTS_PROJECTION, &OldMatProj);
	MLSet::DXDevice->GetTransform(D3DTS_WORLD1, &OldMatViewProj);
	MLSet::DXDevice->GetRenderTarget(0, &BackBuffer);
	MLSet::DXDevice->GetDepthStencilSurface(&LastSurfaceZBuffer);
	MLSet::DXDevice->SetDepthStencilSurface(SurfaceZBuffer);
}

void Reflection::PreRenderRefCube(int cube, float4x4* world)
{
	float3 tmpmax = SMVector3Transform(PosMax, *world);
	float3 tmpmin = SMVector3Transform(PosMin, *world);
	float znear = 0.25f;
	if (cube == 0 || cube == 1)
		znear = (PosMax.x - PosMin.x)*0.5f;
	else if (cube == 2 || cube == 3)
		znear = (PosMax.y - PosMin.y)*0.5f;
	else if (cube == 4 || cube == 5)
		znear = (PosMax.z - PosMin.z)*0.5f;

	D3DXMATRIX tmpproj;
	D3DXMatrixPerspectiveFovLH(&tmpproj, D3DX_PI / 2.f, 1, znear + 0.25, 100);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION, &tmpproj);

	/*D3DXMATRIXA16 mViewDir;
	float4x4 tmpview;
	Core::Data::ObjCamera->GetViewMatrix(&tmpview);*/
	/*tmpview._14 = tmpview._12 = tmpview._13 = 0.0f;
	tmpview._21 = tmpview._24 = tmpview._23 = 0.0f;
	tmpview._31 = tmpview._32 = tmpview._34 = 0.0f;
	tmpview._41 = tmpview._42 = tmpview._43 = 0.0f;*/

	PositionReflect = SMVector3Transform(Position, *world);
	//float3 tmppos = SMVector3Transform(Position,*world);
	MatrixView = SMMatrixLookAtLH(
		PositionReflect,
		//((Core::Data::OrientedCube[cube]*Core::Data::NearFar.y)+Position),
		((MLSet::OrientedCube[cube] + PositionReflect)),
		MLSet::UpVectorsCube[cube]);
	//MatrixView = tmpview * MatrixView;



	/*float4x4 viewmat;
	Core::Data::ObjCamera->GetViewMatrix(&viewmat);

	D3DXPLANE tmpplane2;
	D3DXPlaneFromPointNormal(&tmpplane2,&Position.operator D3DXVECTOR3(),&Core::Data::OrientedCube[cube].operator D3DXVECTOR3());

	D3DXMATRIX matReflect,matView,MatrixView2;
	D3DXPLANE tmpplane;
	D3DXPlaneTransform(&tmpplane,&tmpplane2,&(world->operator D3DXMATRIX()));
	D3DXMatrixReflect(&matReflect, &tmpplane);

	D3DXMatrixMultiply(&MatrixView2, &matReflect, &(viewmat.operator D3DXMATRIX()) );*/


	MLSet::DXDevice->SetTransform(D3DTS_VIEW, &MatrixView.operator D3DXMATRIX());

	MLSet::DXDevice->SetTransform(D3DTS_WORLD1, &((MatrixView * tmpproj).operator D3DXMATRIX()));

	ReflectFrustum[cube]->Update(&float4x4(MatrixView), &float4x4(tmpproj));

	mem_release_del(CubeReflectSurface[cube]);

	TextureCubeReflect->GetCubeMapSurface((D3DCUBEMAP_FACES)cube, 0, &CubeReflectSurface[cube]);
	MLSet::DXDevice->SetRenderTarget(0, CubeReflectSurface[cube]);

	MLSet::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0L);
	//SkyXEngine::Core::Data::MaterialsManager->Render(SkyXEngine::Core::Data::EngineID::ID_MtrlBase,&SMMatrixIdentity(),0);
}

void Reflection::PostRenderRefCube(int cube)
{
	/*if(GetAsyncKeyState(VK_NUMPAD5))
	{
	char tmpstr[1024];
	sprintf(tmpstr,"C:\\1\\reflectioncube_%d.bmp",cube);
	D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_PNG, CubeReflectSurface[cube], NULL,0);
	}*/
}

void Reflection::EndRenderRefCube()
{
	MLSet::DXDevice->SetTransform(D3DTS_VIEW, &OldMatView);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION, &OldMatProj);
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1, &OldMatViewProj);
	MLSet::DXDevice->SetRenderTarget(0, BackBuffer);
	MLSet::DXDevice->SetDepthStencilSurface(LastSurfaceZBuffer);
	mem_release_del(LastSurfaceZBuffer);
	mem_release_del(BackBuffer);

}

inline IDirect3DCubeTexture9* Reflection::GetRefCubeTex()
{
	return TextureCubeReflect;
}