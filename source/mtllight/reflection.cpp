
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "reflection.h"

CReflection::CReflection()
{
	m_pSurface = 0;
	m_pTexPlaneRef = 0;
	m_pTexCubeRef = 0;
	m_pBackBuffer = 0;
	m_iCountUpdate = 0;

	for (int i = 0; i < 6; ++i)
	{
		m_aFrustums[i] = 0;
	}
}

CReflection::~CReflection()
{
	for (int i = 0; i < 6; ++i)
	{
		mem_release_del(m_aFrustums[i]);
	}

	mem_release_del(m_pBackBuffer);
	mem_release_del(m_pTexPlaneRef);
	mem_release_del(m_pSurface);

	mem_release_del(m_pTexCubeRef);
}

void CReflection::setIDArr(ID id, ID idFace, ID idArr)
{
	if (!(idFace >= 0 && idFace < 6))
		return;

	if (id >= m_aIDsArr.size())
	{
		ID* tmparr = new ID[6];
		for (int i = 0; i < 6; ++i)
			tmparr[i] = -1;
		m_aIDsArr[id] = tmparr;
	}

	m_aIDsArr[id][idFace] = idArr;
}

int CReflection::getCountIDArrs()
{
	return m_aIDsArr.size();
}

ID CReflection::getIDArr(ID id, ID idFace)
{
	if (!(idFace >= 0 && idFace < 6))
		return -1;

	if (id < 0 || !(idFace >= 0 && idFace < 6) || !(id < m_aIDsArr.size()))
		return -2;

	return m_aIDsArr[id][idFace];
}

void CReflection::onLostDevice()
{
	mem_release_del(m_pTexWork);
	mem_release_del(m_pTexPlaneRef);
	mem_release_del(m_pTexCubeRef);

	mem_release_del(m_pSurface);
}

void CReflection::onResetDevice()
{
	D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexWork);

	if (type_reflection == MTLTYPE_REFLECT_PLANE)
	{
		D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexPlaneRef);
	}
	else if (type_reflection == MTLTYPE_REFLECT_CUBE_STATIC || type_reflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		D3DXCreateCubeTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexCubeRef);
		m_pTexCubeRef->SetAutoGenFilterType(D3DTEXF_LINEAR);
	}

	m_iCountUpdate = 0;
}

void CReflection::init(MTLTYPE_REFLECT howref)
{
	type_reflection = howref;

	D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexWork);

	if (type_reflection == MTLTYPE_REFLECT_PLANE)
	{
		if (!m_aFrustums[0])
			m_aFrustums[0] = SGCore_CrFrustum();

		if (!m_pTexPlaneRef)
			D3DXCreateTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, MLSet::SizeTexReflection.y, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexPlaneRef);
	}
	else if (type_reflection == MTLTYPE_REFLECT_CUBE_STATIC || type_reflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (!m_aFrustums[i])
				m_aFrustums[i] = SGCore_CrFrustum();
		}

		if (!m_pTexCubeRef)
			D3DXCreateCubeTexture(MLSet::DXDevice, MLSet::SizeTexReflection.x, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &m_pTexCubeRef);
	}
	else
	{

	}

	m_iCountUpdate = 0;
}

MTLTYPE_REFLECT CReflection::getTypeReflect()
{ 
	return type_reflection; 
}

const IFrustum* CReflection::getFrustum(ID id)
{ 
	return m_aFrustums[id]; 
}

bool CReflection::updateCountUpdate(const float3_t* viewpos)
{
	if (!viewpos)
		return false;

	if (type_reflection == MTLTYPE_REFLECT_CUBE_STATIC)
	{
		if (m_iCountUpdate < MTL_REF_UPDATE_MAX_COUNT_FOR_STATIC)
		{
			++(m_iCountUpdate);
			return true;
		}
		else
			return false;
	}

	float dist = SMVector3Distance(m_vPosition, (*viewpos)) - SMVector3Length((m_vMin - m_vMax) *0.5f);
	if (dist < MTL_REF_UPDATE_L0_DIST)
		m_iCountUpdate = -1;
	else if (dist < MTL_REF_UPDATE_L1_DIST && dist > MTL_REF_UPDATE_L0_DIST)
	{
		if (m_iCountUpdate > 1)
			m_iCountUpdate = -1;
	}
	else if (dist < MTL_REF_UPDATE_L2_DIST && dist > MTL_REF_UPDATE_L1_DIST)
	{
		if (m_iCountUpdate > 2)
			m_iCountUpdate = -1;
	}
	else if (dist > MTL_REF_UPDATE_L2_DIST)
	{
		m_iCountUpdate = 4;
	}

	++m_iCountUpdate;

	return (m_iCountUpdate == 0);
}

bool CReflection::allowedRender()
{
	if (type_reflection == MTLTYPE_REFLECT_CUBE_STATIC)
	{
		if (m_iCountUpdate <= MTL_REF_UPDATE_MAX_COUNT_FOR_STATIC)
		{
			++(m_iCountUpdate);
			return true;
		}
		else
			return false;
	}

	return (m_iCountUpdate == 0);
}

void CReflection::nullingCountUpdate()
{
	m_iCountUpdate = 0;
}

IDirect3DCubeTexture9* CReflection::getRefCubeTex()
{
	return m_pTexCubeRef;
}

//##########################################################################

void CReflection::preRenderRefPlane(const D3DXPLANE* plane)
{
	if (!plane)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - [reflection]: plane is NULL", GEN_MSG_LOCATION);
		return;
	}

	//m_oPlane = *plane;

	float4x4 viewmat;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &viewmat);

	D3DXMATRIX matReflect, matView;
	D3DXMatrixReflect(&matReflect, plane);
	
	viewmat = float4x4(matReflect) * viewmat;
	viewmat._12 = -viewmat._12;
	viewmat._22 = -viewmat._22;
	viewmat._32 = -viewmat._32;
	viewmat._42 = -viewmat._42;

	m_aFrustums[0]->update(&viewmat, &(MLSet::RefMProjPlane));

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &m_mOldMatView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &m_mOldMatProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &m_mOldMatViewProj);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &viewmat);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &MLSet::RefMProjPlane);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(viewmat * MLSet::RefMProjPlane));

	MLSet::DXDevice->GetRenderTarget(0, &m_pBackBuffer);

	mem_release_del(m_pSurface);
	m_pTexWork->GetSurfaceLevel(0, &m_pSurface);
	MLSet::DXDevice->SetRenderTarget(0, m_pSurface);
	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0L);
}

void CReflection::postRenderRefPlane()
{
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mOldMatView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mOldMatProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &m_mOldMatViewProj);

	mem_release_del(m_pSurface);

	DWORD alphablend, alphatest, zenable, zwriteenable;

	MLSet::DXDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &alphablend);
	MLSet::DXDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &alphatest);
	MLSet::DXDevice->GetRenderState(D3DRS_ZENABLE, &zenable);
	MLSet::DXDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zwriteenable);

	MLSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	m_pTexPlaneRef->GetSurfaceLevel(0, &m_pSurface);
	MLSet::DXDevice->SetRenderTarget(0, m_pSurface);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::ScreenOut);

	MLSet::DXDevice->SetTexture(0, m_pTexWork);
	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	MLSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, alphablend);
	MLSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, alphatest);
	MLSet::DXDevice->SetRenderState(D3DRS_ZENABLE, zenable);
	MLSet::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, zwriteenable);

	mem_release_del(m_pSurface);

	MLSet::DXDevice->SetRenderTarget(0, m_pBackBuffer);
	mem_release_del(m_pBackBuffer);

	/*if (GetAsyncKeyState(VK_NUMPAD9))
	{
		char tmpstr[256];
		sprintf(tmpstr, "C:\\1\\reflectionreflection.png");
		D3DXSaveTextureToFile(tmpstr, D3DXIFF_JPG, m_pTexPlaneRef, NULL);
	}*/

	//m_pTexPlaneRef->GenerateMipSubLevels();
}

IDirect3DTexture9* CReflection::getRefPlaneTex()
{
	return m_pTexPlaneRef;
}

//**************************************************************************

void CReflection::setMinMax(const float3_t *pMin, const float3_t *pMax)
{
	//если новые размеры отличаются от старых значит надо обновить отражения, если они были статические
	if (m_vMin.x != pMax->x || m_vMin.y != pMax->y || m_vMin.z != pMax->z || m_vMax.x != pMin->x || m_vMax.y != pMin->y || m_vMax.z != pMin->z)
		m_iCountUpdate = 0;

	m_vMin = *pMax;
	m_vMax = *pMin;
}

void CReflection::beginRenderRefCube(const float3_t* pCenter)
{
	if (!pCenter)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - [reflection]: position center is NULL", GEN_MSG_LOCATION);
		return;
	}

	m_vPosition = *pCenter;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &m_mOldMatView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &m_mOldMatProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &m_mOldMatViewProj);

	MLSet::DXDevice->GetRenderTarget(0, &m_pBackBuffer);
}

void CReflection::preRenderRefCube(ID idFace, const float4x4 *pWorld)
{
	//MLSet::DXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)MLSet::RefMProjCube));

	if (pWorld)
		m_vPosition = SMVector3Transform(m_vPosition, *pWorld);

	m_mView = SMMatrixLookAtLH(
		m_vPosition,
		((MLSet::OrientedCube[idFace] + m_vPosition)),
		MLSet::UpVectorsCube[idFace]);

	/*MLSet::DXDevice->SetTransform(D3DTS_VIEW, &m_mView.operator D3DXMATRIX());

	MLSet::DXDevice->SetTransform(D3DTS_WORLD1, &((D3DXMATRIX)(m_mView * MLSet::RefMProjCube)));*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &MLSet::RefMProjCube);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(m_mView * MLSet::RefMProjCube));

	m_aFrustums[idFace]->update(&float4x4(m_mView), &MLSet::RefMProjCube);

	mem_release_del(m_pSurface);
	m_pTexWork->GetSurfaceLevel(0, &m_pSurface);
	MLSet::DXDevice->SetRenderTarget(0, m_pSurface);

	MLSet::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0);
}

void CReflection::postRenderRefCube(ID idFace)
{
	mem_release(m_pSurface);

	DWORD alphablend, alphatest, zenable, zwriteenable;

	MLSet::DXDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &alphablend);
	MLSet::DXDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &alphatest);
	MLSet::DXDevice->GetRenderState(D3DRS_ZENABLE, &zenable);
	MLSet::DXDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zwriteenable);

	MLSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	MLSet::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	m_pTexCubeRef->GetCubeMapSurface((D3DCUBEMAP_FACES)idFace, 0, &m_pSurface);
	MLSet::DXDevice->SetRenderTarget(0, m_pSurface);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::ScreenOut);

	MLSet::DXDevice->SetTexture(0, m_pTexWork);
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
	
	mem_release_del(m_pSurface);
}

void CReflection::endRenderRefCube(const float3_t *pViewPos)
{
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mOldMatView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mOldMatProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &m_mOldMatViewProj);

	MLSet::DXDevice->SetRenderTarget(0, m_pBackBuffer);
	mem_release_del(m_pBackBuffer);

	updateCountUpdate(pViewPos);
}

