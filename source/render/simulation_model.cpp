
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "simulation_model.h"

CSimulationModel::CSimulationModel()
{
	m_iCurrRenderModel = 0;
	m_typeModel = MTLTYPE_MODEL_STATIC;

	D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		D3DDECL_END()
	};

	gdata::pDXDevice->CreateVertexDeclaration(layoutstatic, &m_pVertexDeclarationStatic);


	D3DVERTEXELEMENT9 InstanceGreen[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		D3DDECL_END()
	};

	gdata::pDXDevice->CreateVertexDeclaration(InstanceGreen, &m_pVertexDeclarationGreen);

	D3DVERTEXELEMENT9 layoutDynamic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 56, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 64, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		D3DDECL_END()
	};
	gdata::pDXDevice->CreateVertexDeclaration(layoutDynamic, &m_pVertexDeclarationSkin);

	gdata::pDXDevice->CreateVertexBuffer(
		1 * sizeof(CGreenDataVertex),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pTransVertBufGreen,
		0);

	m_oGreen.m_vPosition = float3_t(0, 0, 0);
	m_oGreen.m_vTexCoord.x = 1.0f;
	m_oGreen.m_vTexCoord.y = 0;
	m_oGreen.m_vTexCoord.z = 0;
	m_oGreen.m_vSinCosRot.x = sinf(m_oGreen.m_vTexCoord.y);
	m_oGreen.m_vSinCosRot.y = cosf(m_oGreen.m_vTexCoord.y);

	CGreenDataVertex* RTGPUArrVerteces = 0;
	m_pTransVertBufGreen->Lock(0, 0, (void**)&RTGPUArrVerteces, 0);
	memcpy(RTGPUArrVerteces, &m_oGreen, sizeof(CGreenDataVertex));
	m_pTransVertBufGreen->Unlock();
}

CSimulationModel::~CSimulationModel()
{
	mem_release(m_pVertexDeclarationStatic);
	mem_release(m_pVertexDeclarationGreen);
	mem_release(m_pVertexDeclarationSkin);

	mem_release(m_pTransVertBufGreen);

	for (int i = 0; i < m_aModels.size(); ++i)
	{
		mem_release(m_aModels[i]->m_pAnim);
		mem_release(m_aModels[i]->m_pModel);
		mem_delete(m_aModels[i]);
	}
}

void CSimulationModel::add(ISXDataStaticModel *pModel)
{
	char szFullPath[1024];
	sprintf(szFullPath, "%s.dds", pModel->m_ppTextures[0]);
	m_idMtrl = SGCore_MtlLoad(szFullPath, MTL_TYPE_GEOM);

	vertex_static_ex *pVertexStatic;
	pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertexStatic, 0);

	float3_t vPos = pVertexStatic[pModel->m_pStartVertex[0]].Pos;
	float3 vMax = vPos;
	float3 vMin = vPos;
	for (int k = 0; k<pModel->m_pVertexCount[0]; k++)
	{
		vPos = pVertexStatic[pModel->m_pStartVertex[0] + k].Pos;

		if (vPos.x > vMax.x)
			vMax.x = vPos.x;

		if (vPos.y > vMax.y)
			vMax.y = vPos.y;

		if (vPos.z > vMax.z)
			vMax.z = vPos.z;


		if (vPos.x < vMin.x)
			vMin.x = vPos.x;

		if (vPos.y < vMin.y)
			vMin.y = vPos.y;

		if (vPos.z < vMin.z)
			vMin.z = vPos.z;
	}

	UINT *pIndex;
	pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);

	D3DXPLANE oPlane;
	D3DXPlaneFromPoints(&oPlane,
		&D3DXVECTOR3(pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 0]].Pos.x, pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 0]].Pos.y, pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 0]].Pos.z),
		&D3DXVECTOR3(pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 1]].Pos.x, pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 1]].Pos.y, pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 1]].Pos.z),
		&D3DXVECTOR3(pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 2]].Pos.x, pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 2]].Pos.y, pVertexStatic[pIndex[pModel->m_pStartIndex[0] + 2]].Pos.z));
	pModel->m_pVertexBuffer->Unlock();
	pModel->m_pIndexBuffer->Unlock();

	float3_t vCenter = (float3_t)((vMax + vMin) * 0.5);

	m_aModels.push_back(new CModel(pModel, &vCenter, &float3_t(vMax), &float3_t(vMin), &oPlane));


	IDirect3DVertexBuffer9 *pVertexBufferAnim;
	gdata::pDXDevice->CreateVertexBuffer(
		pModel->m_uiAllVertexCount * sizeof(vertex_animated_ex),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&pVertexBufferAnim,
		0);

	vertex_animated_ex *pVertexAnim;
	pVertexBufferAnim->Lock(0, 0, (void**)&pVertexAnim, 0);
	pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertexStatic, 0);
	
	for (UINT i = 0; i < pModel->m_uiAllVertexCount; ++i)
	{
		pVertexAnim[i].Pos = pVertexStatic[i].Pos;
		pVertexAnim[i].Norm = pVertexStatic[i].Norm;
		pVertexAnim[i].Tex = pVertexStatic[i].Tex;
		pVertexAnim[i].Tangent = pVertexStatic[i].Tangent;
		pVertexAnim[i].Binorm = pVertexStatic[i].Binorm;
		pVertexAnim[i].BoneIndices[0] = pVertexAnim[i].BoneIndices[1] = pVertexAnim[i].BoneIndices[2] = pVertexAnim[i].BoneIndices[3] = 0;
		pVertexAnim[i].BoneWeights.x = 1;
		pVertexAnim[i].BoneWeights.y = pVertexAnim[i].BoneWeights.z = pVertexAnim[i].BoneWeights.w = 0;
	}

	pVertexBufferAnim->Unlock();
	pModel->m_pVertexBuffer->Unlock();

	m_aModels[m_aModels.size() - 1]->m_pAnim = pVertexBufferAnim;
}

ID CSimulationModel::getIdMtl()
{
	return m_idMtrl;
}

void CSimulationModel::getPlane(D3DXPLANE* plane)
{
	if (m_typeModel == MTLTYPE_MODEL_STATIC || m_typeModel == MTLTYPE_MODEL_TREE || m_typeModel == MTLTYPE_MODEL_GRASS)
		D3DXPlaneTransform(plane, &m_aModels[m_iCurrRenderModel]->m_oPlane, &((D3DXMATRIX)m_mWorld));
}

void CSimulationModel::getCenter(float3_t* center)
{
	if (m_typeModel == MTLTYPE_MODEL_STATIC || m_typeModel == MTLTYPE_MODEL_TREE || m_typeModel == MTLTYPE_MODEL_GRASS)
		*center = SMVector3Transform(m_aModels[m_iCurrRenderModel]->m_vCenter, m_mWorld);
}

void CSimulationModel::render(DWORD timeDelta)
{
	if (!(m_iCurrRenderModel >= 0 && m_aModels.size() > (UINT)m_iCurrRenderModel && m_aModels[m_iCurrRenderModel]))
		return;

	m_mWorld = SMMatrixRotationX(m_vRotation.x) * SMMatrixRotationY(m_vRotation.y) * SMMatrixRotationZ(m_vRotation.z);
	if (m_typeModel == MTLTYPE_MODEL_STATIC)
		renderStatic(timeDelta);
	else if (m_typeModel == MTLTYPE_MODEL_TREE || m_typeModel == MTLTYPE_MODEL_GRASS)
	{
		m_oGreen.m_vTexCoord.y = m_vRotation.y;
		m_oGreen.m_vSinCosRot.x = sinf(m_oGreen.m_vTexCoord.y);
		m_oGreen.m_vSinCosRot.y = cosf(m_oGreen.m_vTexCoord.y);

		CGreenDataVertex *pVertex = 0;
		m_pTransVertBufGreen->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD);
		memcpy(pVertex, &m_oGreen, sizeof(CGreenDataVertex));
		m_pTransVertBufGreen->Unlock();

		renderGreen(timeDelta);
	}
	else if (m_typeModel == MTLTYPE_MODEL_SKIN)
		renderSkin(timeDelta);
}

void CSimulationModel::renderStatic(DWORD timeDelta)
{
	gdata::pDXDevice->SetStreamSource(0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static_ex));
	gdata::pDXDevice->SetIndices(m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexBuffer);
	gdata::pDXDevice->SetVertexDeclaration(m_pVertexDeclarationStatic);

	SGCore_MtlSet(m_idMtrl, &m_mWorld);
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexCount[0], 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
}

void CSimulationModel::renderGreen(DWORD timeDelta)
{
	gdata::pDXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | 1));

	gdata::pDXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
	gdata::pDXDevice->SetStreamSource(1, m_pTransVertBufGreen, 0, sizeof(CGreenDataVertex));

	gdata::pDXDevice->SetStreamSource(0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static_ex));
	gdata::pDXDevice->SetIndices(m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexBuffer);
	gdata::pDXDevice->SetVertexDeclaration(m_pVertexDeclarationGreen);


	int iCountIndex = 0;

	SGCore_MtlSet(m_idMtrl, &SMMatrixIdentity());
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexCount[0], iCountIndex, m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3) * 1));
	iCountIndex += m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0];

	gdata::pDXDevice->SetStreamSourceFreq(0, 1);
	gdata::pDXDevice->SetStreamSourceFreq(1, 1);
}

void CSimulationModel::renderSkin(DWORD timeDelta)
{
	ModelBoneShader oModelBoneShader;
	oModelBoneShader.position = float4(0, 0, 0, 1);
	oModelBoneShader.orient = SMQuaternion();

	gdata::pDXDevice->SetStreamSource(0, m_aModels[m_iCurrRenderModel]->m_pAnim, 0, sizeof(vertex_animated_ex));
	gdata::pDXDevice->SetIndices(m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexBuffer);
	gdata::pDXDevice->SetVertexDeclaration(m_pVertexDeclarationSkin);

	SGCore_MtlSet(m_idMtrl, &m_mWorld);
	gdata::pDXDevice->SetVertexShaderConstantF(16, (float*)&oModelBoneShader, 2);
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexCount[0], 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
}