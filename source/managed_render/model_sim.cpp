
#include <managed_render/model_sim.h>

ModelSim::ModelSim()
{
	CurrRenderModel = 0;
	TypeModel = MtlTypeModel::tms_static;

	D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	GData::DXDevice->CreateVertexDeclaration(layoutstatic, &VertexDeclarationStatic);


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

	GData::DXDevice->CreateVertexDeclaration(InstanceGreen, &VertexDeclarationGreen);

	D3DVERTEXELEMENT9 layoutDynamic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		D3DDECL_END()
	};
	GData::DXDevice->CreateVertexDeclaration(layoutDynamic, &VertexDeclarationSkin);

	GData::DXDevice->CreateVertexBuffer(
		1 * sizeof(DataVertex),
		/*D3DUSAGE_DYNAMIC | */D3DUSAGE_WRITEONLY,
		0,
		/*D3DPOOL_DEFAULT*/D3DPOOL_MANAGED,
		&TransVertBufGreen,
		0);

	DVGreen.Position = float3_t(0,0,0);
	DVGreen.TexCoord.x = 1.0f;
	DVGreen.TexCoord.y = 0;
	DVGreen.TexCoord.z = 0;
	DVGreen.SinCosRot.x = sinf(DVGreen.TexCoord.y);
	DVGreen.SinCosRot.y = cosf(DVGreen.TexCoord.y);

	DataVertex* RTGPUArrVerteces = 0;
	TransVertBufGreen->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
	memcpy(RTGPUArrVerteces, &DVGreen, sizeof(DataVertex));
	TransVertBufGreen->Unlock();
}

ModelSim::~ModelSim()
{
	mem_release(VertexDeclarationStatic);
	mem_release(VertexDeclarationGreen);
	mem_release(VertexDeclarationSkin);

	mem_release(TransVertBufGreen);

	for (int i = 0; i < ArrStaticModel.size(); ++i)
	{
		mem_release(ArrStaticModel[i]->Anim);
		mem_release(ArrStaticModel[i]->Model);
		mem_delete(ArrStaticModel[i]);
	}
}

void ModelSim::Add(const char* path)
{
	char tmppath[1024];
	ISXDataStaticModel* StaticModel = 0;
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path);
	SGCore_StaticModelLoad(tmppath, &StaticModel);

	if (StaticModel->SubsetCount > 1)
	{
		//error
	}

	sprintf(tmppath, "%s.dds", StaticModel->ArrTextures[0]);
	IDsMat = SGCore_MtlLoad(tmppath, MTL_TYPE_GEOM);


	vertex_static* pData;
	StaticModel->VertexBuffer->Lock(0, 0, (void**)&pData, 0);

	float3_t tmppos = pData[StaticModel->StartVertex[0]].Pos;
	float3 tmpMax = tmppos;
	float3 tmpMin = tmppos;
	for (DWORD k = 0; k<StaticModel->VertexCount[0]; k++)
	{
		tmppos = pData[StaticModel->StartVertex[0] + k].Pos;

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
	StaticModel->IndexBuffer->Lock(0, 0, (void **)&indeces, 0);

	float3 tmpMM = SMVectorLerp(tmpMax, tmpMin, 0.5f);
	D3DXPLANE Plane;
	D3DXPlaneFromPoints(&Plane,
		&D3DXVECTOR3(pData[indeces[StaticModel->StartIndex[0] + 0]].Pos.x, pData[indeces[StaticModel->StartIndex[0] + 0]].Pos.y, pData[indeces[StaticModel->StartIndex[0] + 0]].Pos.z),
		&D3DXVECTOR3(pData[indeces[StaticModel->StartIndex[0] + 1]].Pos.x, pData[indeces[StaticModel->StartIndex[0] + 1]].Pos.y, pData[indeces[StaticModel->StartIndex[0] + 1]].Pos.z),
		&D3DXVECTOR3(pData[indeces[StaticModel->StartIndex[0] + 2]].Pos.x, pData[indeces[StaticModel->StartIndex[0] + 2]].Pos.y, pData[indeces[StaticModel->StartIndex[0] + 2]].Pos.z));
	StaticModel->VertexBuffer->Unlock();
	StaticModel->IndexBuffer->Unlock();

	float3_t Center = (float3_t)((tmpMax + tmpMin) * 0.5);
	float3_t Min = tmpMin;
	float3_t Max = tmpMax;

	ArrStaticModel.push_back(new DataModel(StaticModel, &Center,&Max,&Min,&Plane));

	IDirect3DVertexBuffer9* Anim;
	GData::DXDevice->CreateVertexBuffer(
		StaticModel->AllVertexCount * sizeof(vertex_animated),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&Anim,
		0);

	vertex_animated* pDataAnim;
	Anim->Lock(0, 0, (void**)&pDataAnim, 0);
	StaticModel->VertexBuffer->Lock(0, 0, (void**)&pData, 0);
	
	for (int i = 0; i < StaticModel->AllVertexCount; ++i)
	{
		pDataAnim[i].Pos = pData[i].Pos;
		pDataAnim[i].Norm = pData[i].Norm;
		pDataAnim[i].Tex = pData[i].Tex;
		pDataAnim[i].BoneIndices[0] = pDataAnim[i].BoneIndices[1] = pDataAnim[i].BoneIndices[2] = pDataAnim[i].BoneIndices[3] = 0;
		pDataAnim[i].BoneWeights.x = 1;
		pDataAnim[i].BoneWeights.y = pDataAnim[i].BoneWeights.z = pDataAnim[i].BoneWeights.w = 0;
	}

	Anim->Unlock();
	StaticModel->VertexBuffer->Unlock();

	ArrStaticModel[ArrStaticModel.size() - 1]->Anim = Anim;
}

inline ID ModelSim::GetIDMtl()
{
	return IDsMat;
}

inline void ModelSim::GetPlane(D3DXPLANE* plane)
{
	if (TypeModel == MtlTypeModel::tms_static || TypeModel == MtlTypeModel::tms_tree || TypeModel == MtlTypeModel::tms_grass)
	{
		D3DXPlaneTransform(plane, &ArrStaticModel[CurrRenderModel]->Plane, &((D3DXMATRIX)WorldMat));
	}
}

inline void ModelSim::GetCenter(float3_t* center)
{
	if (TypeModel == MtlTypeModel::tms_static || TypeModel == MtlTypeModel::tms_tree || TypeModel == MtlTypeModel::tms_grass)
	{
		*center = SMVector3Transform(ArrStaticModel[CurrRenderModel]->Center, WorldMat);
	}
}

void ModelSim::Render(DWORD timeDelta)
{
	if (!(CurrRenderModel >= 0 && ArrStaticModel.size() > CurrRenderModel && ArrStaticModel[CurrRenderModel]))
		return;

	WorldMat = SMMatrixRotationX(Rotation.x) * SMMatrixRotationY(Rotation.y) * SMMatrixRotationZ(Rotation.z);
	if (TypeModel == MtlTypeModel::tms_static)
		RenderStatic(timeDelta);
	else if (TypeModel == MtlTypeModel::tms_tree || TypeModel == MtlTypeModel::tms_grass)
	{
		DVGreen.TexCoord.y = Rotation.y;
		DVGreen.SinCosRot.x = sinf(DVGreen.TexCoord.y);
		DVGreen.SinCosRot.y = cosf(DVGreen.TexCoord.y);

		DataVertex* RTGPUArrVerteces = 0;
		TransVertBufGreen->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		memcpy(RTGPUArrVerteces, &DVGreen, sizeof(DataVertex));
		TransVertBufGreen->Unlock();

		RenderGreen(timeDelta);
	}
	else if (TypeModel == MtlTypeModel::tms_skin)
		RenderSkin(timeDelta);
}

void ModelSim::RenderStatic(DWORD timeDelta)
{
	uint32_t* RTGPUArrIndicesPtrs2;

	GData::DXDevice->SetStreamSource(0, ArrStaticModel[CurrRenderModel]->Model->VertexBuffer, 0, sizeof(vertex_static));
	GData::DXDevice->SetIndices(ArrStaticModel[CurrRenderModel]->Model->IndexBuffer);
	GData::DXDevice->SetVertexDeclaration(VertexDeclarationStatic);
	long jCountIndex = 0;


	SGCore_MtlSet(IDsMat, &WorldMat);
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, ArrStaticModel[CurrRenderModel]->Model->VertexCount[0], jCountIndex, ArrStaticModel[CurrRenderModel]->Model->IndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ArrStaticModel[CurrRenderModel]->Model->IndexCount[0] / 3);
	jCountIndex += ArrStaticModel[CurrRenderModel]->Model->IndexCount[0];
}

void ModelSim::RenderGreen(DWORD timeDelta)
{
	GData::DXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | 1));

	GData::DXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
	GData::DXDevice->SetStreamSource(1, TransVertBufGreen, 0, sizeof(DataVertex));

	GData::DXDevice->SetStreamSource(0, ArrStaticModel[CurrRenderModel]->Model->VertexBuffer, 0, sizeof(vertex_static));
	GData::DXDevice->SetIndices(ArrStaticModel[CurrRenderModel]->Model->IndexBuffer);
	GData::DXDevice->SetVertexDeclaration(VertexDeclarationGreen);


	long jCountIndex = 0;

	SGCore_MtlSet(IDsMat, &SMMatrixIdentity());
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, ArrStaticModel[CurrRenderModel]->Model->VertexCount[0], jCountIndex, ArrStaticModel[CurrRenderModel]->Model->IndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((ArrStaticModel[CurrRenderModel]->Model->IndexCount[0] / 3) * 1));
	jCountIndex += ArrStaticModel[CurrRenderModel]->Model->IndexCount[0];

	GData::DXDevice->SetStreamSourceFreq(0, 1);
	GData::DXDevice->SetStreamSourceFreq(1, 1);
}

void ModelSim::RenderSkin(DWORD timeDelta)
{
	ModelBoneShader mbs;
	mbs.position = float4(0, 0, 0, 1);
	mbs.orient = SMQuaternion();

	GData::DXDevice->SetStreamSource(0, ArrStaticModel[CurrRenderModel]->Anim, 0, sizeof(vertex_animated));
	GData::DXDevice->SetIndices(ArrStaticModel[CurrRenderModel]->Model->IndexBuffer);
	GData::DXDevice->SetVertexDeclaration(VertexDeclarationSkin);

	SGCore_MtlSet(IDsMat, &WorldMat);
	GData::DXDevice->SetVertexShaderConstantF(16, (float*)&mbs, 2);
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, ArrStaticModel[CurrRenderModel]->Model->VertexCount[0], 0, ArrStaticModel[CurrRenderModel]->Model->IndexCount[0] / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ArrStaticModel[CurrRenderModel]->Model->IndexCount[0] / 3);
}