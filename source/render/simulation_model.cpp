
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "simulation_model.h"

CSimulationModel::CSimulationModel()
{
	m_iCurrRenderModel = 0;
	m_type_model = MTLTYPE_MODEL_STATIC;

	D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	GData::DXDevice->CreateVertexDeclaration(layoutstatic, &m_pVertexDeclarationStatic);


	D3DVERTEXELEMENT9 InstanceGreen[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		D3DDECL_END()
	};

	GData::DXDevice->CreateVertexDeclaration(InstanceGreen, &m_pVertexDeclarationGreen);

	D3DVERTEXELEMENT9 layoutDynamic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		D3DDECL_END()
	};
	GData::DXDevice->CreateVertexDeclaration(layoutDynamic, &m_pVertexDeclarationSkin);

	GData::DXDevice->CreateVertexBuffer(
		1 * sizeof(CGreenDataVertex),
		/*D3DUSAGE_DYNAMIC | */D3DUSAGE_WRITEONLY,
		0,
		/*D3DPOOL_DEFAULT*/D3DPOOL_MANAGED,
		&m_pTransVertBufGreen,
		0);

	m_oGreen.m_vPosition = float3_t(0, 0, 0);
	m_oGreen.m_vTexCoord.x = 1.0f;
	m_oGreen.m_vTexCoord.y = 0;
	m_oGreen.m_vTexCoord.z = 0;
	m_oGreen.m_vSinCosRot.x = sinf(m_oGreen.m_vTexCoord.y);
	m_oGreen.m_vSinCosRot.y = cosf(m_oGreen.m_vTexCoord.y);

	CGreenDataVertex* RTGPUArrVerteces = 0;
	m_pTransVertBufGreen->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
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

void CSimulationModel::add(const char *szPath)
{
	char szFullPath[1024];
	ISXDataStaticModel *pStaticModel = 0;
	sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), szPath);
	SGCore_StaticModelLoad(szFullPath, &pStaticModel);

	if (pStaticModel->m_uiSubsetCount > 1)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "unresolved more than one group for simulation model\n");
	}

	sprintf(szFullPath, "%s.dds", pStaticModel->m_ppTextures[0]);
	m_idMtrl = SGCore_MtlLoad(szFullPath, MTL_TYPE_GEOM);

	vertex_static* pData;
	pStaticModel->m_pVertexBuffer->Lock(0, 0, (void**)&pData, 0);

	float3_t tmppos = pData[pStaticModel->m_pStartVertex[0]].Pos;
	float3 tmpMax = tmppos;
	float3 tmpMin = tmppos;
	for (DWORD k = 0; k<pStaticModel->m_pVertexCount[0]; k++)
	{
		tmppos = pData[pStaticModel->m_pStartVertex[0] + k].Pos;

		if (tmppos.x > tmpMax.x)
			tmpMax.x = tmppos.x;

		if (tmppos.y > tmpMax.y)
			tmpMax.y = tmppos.y;

		if (tmppos.z > tmpMax.z)
			tmpMax.z = tmppos.z;


		if (tmppos.x < tmpMin.x)
			tmpMin.x = tmppos.x;

		if (tmppos.y < tmpMin.y)
			tmpMin.y = tmppos.y;

		if (tmppos.z < tmpMin.z)
			tmpMin.z = tmppos.z;
	}

	DWORD* indeces;
	pStaticModel->m_pIndexBuffer->Lock(0, 0, (void **)&indeces, 0);

	float3 tmpMM = SMVectorLerp(tmpMax, tmpMin, 0.5f);
	D3DXPLANE Plane;
	D3DXPlaneFromPoints(&Plane,
		&D3DXVECTOR3(pData[indeces[pStaticModel->m_pStartIndex[0] + 0]].Pos.x, pData[indeces[pStaticModel->m_pStartIndex[0] + 0]].Pos.y, pData[indeces[pStaticModel->m_pStartIndex[0] + 0]].Pos.z),
		&D3DXVECTOR3(pData[indeces[pStaticModel->m_pStartIndex[0] + 1]].Pos.x, pData[indeces[pStaticModel->m_pStartIndex[0] + 1]].Pos.y, pData[indeces[pStaticModel->m_pStartIndex[0] + 1]].Pos.z),
		&D3DXVECTOR3(pData[indeces[pStaticModel->m_pStartIndex[0] + 2]].Pos.x, pData[indeces[pStaticModel->m_pStartIndex[0] + 2]].Pos.y, pData[indeces[pStaticModel->m_pStartIndex[0] + 2]].Pos.z));
	pStaticModel->m_pVertexBuffer->Unlock();
	pStaticModel->m_pIndexBuffer->Unlock();

	float3_t Center = (float3_t)((tmpMax + tmpMin) * 0.5);
	float3_t Min = tmpMin;
	float3_t Max = tmpMax;

	m_aModels.push_back(new CModel(pStaticModel, &Center, &Max, &Min, &Plane));

	IDirect3DVertexBuffer9* Anim;
	GData::DXDevice->CreateVertexBuffer(
		pStaticModel->m_uiAllVertexCount * sizeof(vertex_animated),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&Anim,
		0);

	vertex_animated* pDataAnim;
	Anim->Lock(0, 0, (void**)&pDataAnim, 0);
	pStaticModel->m_pVertexBuffer->Lock(0, 0, (void**)&pData, 0);
	
	for (UINT i = 0; i < pStaticModel->m_uiAllVertexCount; ++i)
	{
		pDataAnim[i].Pos = pData[i].Pos;
		pDataAnim[i].Norm = pData[i].Norm;
		pDataAnim[i].Tex = pData[i].Tex;
		pDataAnim[i].BoneIndices[0] = pDataAnim[i].BoneIndices[1] = pDataAnim[i].BoneIndices[2] = pDataAnim[i].BoneIndices[3] = 0;
		pDataAnim[i].BoneWeights.x = 1;
		pDataAnim[i].BoneWeights.y = pDataAnim[i].BoneWeights.z = pDataAnim[i].BoneWeights.w = 0;
	}

	Anim->Unlock();
	pStaticModel->m_pVertexBuffer->Unlock();

	m_aModels[m_aModels.size() - 1]->m_pAnim = Anim;
}

ID CSimulationModel::getIdMtl()
{
	return m_idMtrl;
}

void CSimulationModel::getPlane(D3DXPLANE* plane)
{
	if (m_type_model == MTLTYPE_MODEL_STATIC || m_type_model == MTLTYPE_MODEL_TREE || m_type_model == MTLTYPE_MODEL_GRASS)
	{
		D3DXPlaneTransform(plane, &m_aModels[m_iCurrRenderModel]->m_oPlane, &((D3DXMATRIX)m_mWorld));
	}
}

void CSimulationModel::getCenter(float3_t* center)
{
	if (m_type_model == MTLTYPE_MODEL_STATIC || m_type_model == MTLTYPE_MODEL_TREE || m_type_model == MTLTYPE_MODEL_GRASS)
	{
		*center = SMVector3Transform(m_aModels[m_iCurrRenderModel]->m_vCenter, m_mWorld);
	}
}

void CSimulationModel::render(DWORD timeDelta)
{
	if (!(m_iCurrRenderModel >= 0 && m_aModels.size() > (UINT)m_iCurrRenderModel && m_aModels[m_iCurrRenderModel]))
		return;

	m_mWorld = SMMatrixRotationX(m_vRotation.x) * SMMatrixRotationY(m_vRotation.y) * SMMatrixRotationZ(m_vRotation.z);
	if (m_type_model == MTLTYPE_MODEL_STATIC)
		renderStatic(timeDelta);
	else if (m_type_model == MTLTYPE_MODEL_TREE || m_type_model == MTLTYPE_MODEL_GRASS)
	{
		m_oGreen.m_vTexCoord.y = m_vRotation.y;
		m_oGreen.m_vSinCosRot.x = sinf(m_oGreen.m_vTexCoord.y);
		m_oGreen.m_vSinCosRot.y = cosf(m_oGreen.m_vTexCoord.y);

		CGreenDataVertex* RTGPUArrVerteces = 0;
		m_pTransVertBufGreen->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		memcpy(RTGPUArrVerteces, &m_oGreen, sizeof(CGreenDataVertex));
		m_pTransVertBufGreen->Unlock();

		renderGreen(timeDelta);
	}
	else if (m_type_model == MTLTYPE_MODEL_SKIN)
		renderSkin(timeDelta);
}

void CSimulationModel::renderStatic(DWORD timeDelta)
{
	GData::DXDevice->SetStreamSource(0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static));
	GData::DXDevice->SetIndices(m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexBuffer);
	GData::DXDevice->SetVertexDeclaration(m_pVertexDeclarationStatic);

	SGCore_MtlSet(m_idMtrl, &m_mWorld);
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexCount[0], 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
}

void CSimulationModel::renderGreen(DWORD timeDelta)
{
	GData::DXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | 1));

	GData::DXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
	GData::DXDevice->SetStreamSource(1, m_pTransVertBufGreen, 0, sizeof(CGreenDataVertex));

	GData::DXDevice->SetStreamSource(0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static));
	GData::DXDevice->SetIndices(m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexBuffer);
	GData::DXDevice->SetVertexDeclaration(m_pVertexDeclarationGreen);


	long jCountIndex = 0;

	SGCore_MtlSet(m_idMtrl, &SMMatrixIdentity());
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexCount[0], jCountIndex, m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3) * 1));
	jCountIndex += m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0];

	GData::DXDevice->SetStreamSourceFreq(0, 1);
	GData::DXDevice->SetStreamSourceFreq(1, 1);
}

void CSimulationModel::renderSkin(DWORD timeDelta)
{
	ModelBoneShader mbs;
	mbs.position = float4(0, 0, 0, 1);
	mbs.orient = SMQuaternion();

	GData::DXDevice->SetStreamSource(0, m_aModels[m_iCurrRenderModel]->m_pAnim, 0, sizeof(vertex_animated));
	GData::DXDevice->SetIndices(m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexBuffer);
	GData::DXDevice->SetVertexDeclaration(m_pVertexDeclarationSkin);

	SGCore_MtlSet(m_idMtrl, &m_mWorld);
	GData::DXDevice->SetVertexShaderConstantF(16, (float*)&mbs, 2);
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pVertexCount[0], 0, m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_aModels[m_iCurrRenderModel]->m_pModel->m_pIndexCount[0] / 3);
}