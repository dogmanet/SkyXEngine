
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

	GXDEPTH_STENCIL_DESC dsDesc;
	dsDesc.useDepthTest = false;
	dsDesc.useDepthWrite = false;
	m_pDSState = mtrl_data::pDXDevice->createDepthStencilState(&dsDesc);
}

CReflection::~CReflection()
{
	for (int i = 0; i < 6; ++i)
	{
		mem_release(m_aFrustums[i]);
	}

	mem_release(m_pDSState);

	mem_release(m_pBackBuffer);
	mem_release(m_pTexPlaneRef);
	mem_release(m_pSurface);

	mem_release(m_pTexCubeRef);
}

void CReflection::setIDvisCalcObj(ID id, ID idFace, ID idArr)
{
	if (!(idFace >= 0 && idFace < 6))
		return;

	if (id >= m_aIDsVisCalcObj.size())
	{
		ID* tmparr = new ID[6];
		for (int i = 0; i < 6; ++i)
			tmparr[i] = -1;
		m_aIDsVisCalcObj[id] = tmparr;
	}

	m_aIDsVisCalcObj[id][idFace] = idArr;
}

int CReflection::getCountIDsVisCalcObj()
{
	return m_aIDsVisCalcObj.size();
}

ID CReflection::getIDvisCalcObj(ID id, ID idFace)
{
	if (!(idFace >= 0 && idFace < 6))
		return -1;

	if (id < 0 || !(idFace >= 0 && idFace < 6) || !(id < m_aIDsVisCalcObj.size()))
		return -2;

	return m_aIDsVisCalcObj[id][idFace];
}

void CReflection::onLostDevice()
{
	//mem_release_del(m_pTexWork);
	//mem_release_del(m_pTexPlaneRef);
	//mem_release_del(m_pTexCubeRef);

	//mem_release_del(m_pSurface);
}

void CReflection::onResetDevice()
{
#if 0
	m_pTexWork = mtrl_data::pDXDevice->createTexture2D(mtrl_data::vSizeTexReflection.x, mtrl_data::vSizeTexReflection.y, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_A8R8G8B8);

	if (typeReflection == MTLTYPE_REFLECT_PLANE)
	{
		m_pTexPlaneRef = mtrl_data::pDXDevice->createTexture2D(mtrl_data::vSizeTexReflection.x, mtrl_data::vSizeTexReflection.y, 0, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTOGENMIPMAPS, GXFMT_A8R8G8B8);
		//D3DXCreateTexture(mtrl_data::pDXDevice, mtrl_data::vSizeTexReflection.x, mtrl_data::vSizeTexReflection.y, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexPlaneRef);
	}
	else if (typeReflection == MTLTYPE_REFLECT_CUBE_STATIC || typeReflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		m_pTexCubeRef = mtrl_data::pDXDevice->createTextureCube(mtrl_data::vSizeTexReflection.x, 0, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTOGENMIPMAPS, GXFMT_A8R8G8B8);
		//D3DXCreateCubeTexture(mtrl_data::pDXDevice, mtrl_data::vSizeTexReflection.x, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexCubeRef);
		//@FIXME: no such api!
		//m_pTexCubeRef->SetAutoGenFilterType(D3DTEXF_LINEAR);
	}
#endif

	m_iCountUpdate = 0;
}

void CReflection::init(MTLTYPE_REFLECT howref)
{
	typeReflection = howref;

	m_pTexWork = mtrl_data::pDXDevice->createTexture2D(mtrl_data::vSizeTexReflection.x, mtrl_data::vSizeTexReflection.y, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_A8R8G8B8);
	//D3DXCreateTexture(mtrl_data::pDXDevice, mtrl_data::vSizeTexReflection.x, mtrl_data::vSizeTexReflection.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexWork);

	if (typeReflection == MTLTYPE_REFLECT_PLANE)
	{
		if (!m_aFrustums[0])
			m_aFrustums[0] = SGCore_CrFrustum();

		if (!m_pTexPlaneRef)
			m_pTexPlaneRef = mtrl_data::pDXDevice->createTexture2D(mtrl_data::vSizeTexReflection.x, mtrl_data::vSizeTexReflection.y, 0, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTOGENMIPMAPS, GXFMT_A8R8G8B8);
			//D3DXCreateTexture(mtrl_data::pDXDevice, mtrl_data::vSizeTexReflection.x, mtrl_data::vSizeTexReflection.y, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexPlaneRef);
	}
	else if (typeReflection == MTLTYPE_REFLECT_CUBE_STATIC || typeReflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (!m_aFrustums[i])
				m_aFrustums[i] = SGCore_CrFrustum();
		}

		if (!m_pTexCubeRef)
			m_pTexCubeRef = mtrl_data::pDXDevice->createTextureCube(mtrl_data::vSizeTexReflection.x, 0, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTOGENMIPMAPS, GXFMT_A8R8G8B8);
			//D3DXCreateCubeTexture(mtrl_data::pDXDevice, mtrl_data::vSizeTexReflection.x, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &m_pTexCubeRef);
	}
	else
	{

	}

	m_iCountUpdate = 0;
}

MTLTYPE_REFLECT CReflection::getTypeReflect()
{ 
	return typeReflection; 
}

const IFrustum* CReflection::getFrustum(ID id)
{ 
	return m_aFrustums[id]; 
}

bool CReflection::updateCountUpdate(const float3_t* viewpos)
{
	if (!viewpos)
		return false;

	if (typeReflection == MTLTYPE_REFLECT_CUBE_STATIC)
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
	if (typeReflection == MTLTYPE_REFLECT_CUBE_STATIC)
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

IGXTextureCube* CReflection::getRefCubeTex()
{
	return m_pTexCubeRef;
}

//##########################################################################

void CReflection::preRenderRefPlane(const SMPLANE* plane)
{
	if (!plane)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - [reflection]: plane is NULL", GEN_MSG_LOCATION);
		return;
	}

	//m_oPlane = *plane;

	float4x4 viewmat;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &viewmat);

	SMMATRIX matReflect, matView;
	matReflect = SMMatrixReflect(*plane);
	
	viewmat = float4x4(matReflect) * viewmat;
	viewmat._12 = -viewmat._12;
	viewmat._22 = -viewmat._22;
	viewmat._32 = -viewmat._32;
	viewmat._42 = -viewmat._42;

	m_aFrustums[0]->update(&viewmat, &(mtrl_data::mRefProjPlane));

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &m_mOldMatView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &m_mOldMatProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &m_mOldMatViewProj);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &viewmat);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &mtrl_data::mRefProjPlane);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(viewmat * mtrl_data::mRefProjPlane));

	m_pBackBuffer = mtrl_data::pDXDevice->getColorTarget();

	mem_release(m_pSurface);
	m_pSurface = m_pTexWork->getMipmap();
	mtrl_data::pDXDevice->setColorTarget(m_pSurface);
	mtrl_data::pDXDevice->clear(GXCLEAR_COLOR | GXCLEAR_DEPTH);
}

void CReflection::postRenderRefPlane()
{
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mOldMatView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mOldMatProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &m_mOldMatViewProj);

	mem_release(m_pSurface);

	DWORD alphablend, alphatest, zenable, zwriteenable;

/*	mtrl_data::pDXDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &alphablend);
	mtrl_data::pDXDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &alphatest);
	mtrl_data::pDXDevice->GetRenderState(D3DRS_ZENABLE, &zenable);
	mtrl_data::pDXDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zwriteenable);
*/
	mtrl_data::pDXDevice->setDepthStencilState(m_pDSState);
	mtrl_data::pDXDevice->setBlendState(NULL);

/*	mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
*/
	m_pSurface = m_pTexPlaneRef->getMipmap();
	mtrl_data::pDXDevice->setColorTarget(m_pSurface);

	static ID s_idShader = SGCore_ShaderCreateKit(mtrl_data::shader_id::vs::idScreenOut, mtrl_data::shader_id::ps::idScreenOut);

	SGCore_ShaderBind(s_idShader);

	mtrl_data::pDXDevice->setTexture(m_pTexWork);
	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

/*	mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, alphablend);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, alphatest);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ZENABLE, zenable);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, zwriteenable);
*/
	mem_release(m_pSurface);

	mtrl_data::pDXDevice->setColorTarget(m_pBackBuffer);
	mem_release(m_pBackBuffer);

	/*if (GetAsyncKeyState(VK_NUMPAD9))
	{
		char tmpstr[256];
		sprintf(tmpstr, "C:\\1\\reflectionreflection.png");
		D3DXSaveTextureToFile(tmpstr, D3DXIFF_JPG, m_pTexPlaneRef, NULL);
	}*/

	//m_pTexPlaneRef->GenerateMipSubLevels();
}

IGXTexture2D* CReflection::getRefPlaneTex()
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

	m_pBackBuffer = mtrl_data::pDXDevice->getColorTarget();
}

void CReflection::preRenderRefCube(ID idFace, const float4x4 *pWorld)
{
	//mtrl_data::pDXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)mtrl_data::RefMProjCube));

	if (pWorld)
		m_vPosition = SMVector3Transform(m_vPosition, *pWorld);

	m_mView = SMMatrixLookAtLH(
		m_vPosition,
		((mtrl_data::vOrientedCube[idFace] + m_vPosition)),
		mtrl_data::vUpVectorsCube[idFace]);

	/*mtrl_data::pDXDevice->SetTransform(D3DTS_VIEW, &m_mView.operator D3DXMATRIX());

	mtrl_data::pDXDevice->SetTransform(D3DTS_WORLD1, &((D3DXMATRIX)(m_mView * mtrl_data::RefMProjCube)));*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &mtrl_data::mRefProjCube);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(m_mView * mtrl_data::mRefProjCube));

	m_aFrustums[idFace]->update(&float4x4(m_mView), &mtrl_data::mRefProjCube);

	mem_release(m_pSurface);
	m_pSurface = m_pTexWork->getMipmap();
	mtrl_data::pDXDevice->setColorTarget(m_pSurface);

	mtrl_data::pDXDevice->clear(GXCLEAR_COLOR | GXCLEAR_DEPTH);
}

void CReflection::postRenderRefCube(ID idFace)
{
	mem_release(m_pSurface);

	DWORD alphablend, alphatest, zenable, zwriteenable;

/*	mtrl_data::pDXDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &alphablend);
	mtrl_data::pDXDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &alphatest);
	mtrl_data::pDXDevice->GetRenderState(D3DRS_ZENABLE, &zenable);
	mtrl_data::pDXDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zwriteenable);
*/
	mtrl_data::pDXDevice->setDepthStencilState(m_pDSState);
	mtrl_data::pDXDevice->setBlendState(NULL);
	//mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//mtrl_data::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//mtrl_data::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	m_pSurface = m_pTexCubeRef->getMipmap((GXCUBEMAP_FACES)idFace);
	mtrl_data::pDXDevice->setColorTarget(m_pSurface);

	static ID s_idShader = SGCore_ShaderCreateKit(mtrl_data::shader_id::vs::idScreenOut, mtrl_data::shader_id::ps::idScreenOut);
	SGCore_ShaderBind(s_idShader);

	mtrl_data::pDXDevice->setTexture(m_pTexWork);
	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

/*	mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, alphablend);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, alphatest);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ZENABLE, zenable);
	mtrl_data::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, zwriteenable);
*/
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

	mtrl_data::pDXDevice->setColorTarget(m_pBackBuffer);
	mem_release_del(m_pBackBuffer);

	updateCountUpdate(pViewPos);
}

