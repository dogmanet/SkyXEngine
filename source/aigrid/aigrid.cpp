
#include <aigrid/aigrid.h>

void AIGrid::CorrectPosXZ(float3* pos)
{
	if(!pos)
		return;

	float count = pos->x / AIGRID_QUAD_SIZE;
	pos->x = AIGRID_QUAD_SIZE * (float)((int)floor(count + 0.5));

	/*count = pos->y / AIGRID_QUAD_SIZE;
	pos->y = AIGRID_QUAD_SIZE * (float)((int)floor(count + 0.5));*/

	count = pos->z / AIGRID_QUAD_SIZE;
	pos->z = AIGRID_QUAD_SIZE * (float)((int)floor(count + 0.5));
}

bool AIGrid::IsValidIdInArrQuads(ID id)
{
	return (id >= 0 && ArrQuads.size() > id && !(ArrQuads[id]->IsClose));
}

inline bool AIGrid::AIGridCompareHeight(ID id1,ID id2)
{
	return (
			abs(
			abs(ArrLongCoordQuads[id1].y) - abs(ArrLongCoordQuads[id2].y)
				) > LAIGRID_QUADS_CENTERS_MAXHEIGHT ? false : true
		
		);
}

AIGrid::AIGrid()
{
	AIQuadDecl = 0;
	TransVertBuf = 0;
	CountSplits = 0;
	BoundBox = 0;
	DXDevice = 0;
	Tex_BB = -1;
	IsMarkSplits = false;
	Tex_AIGrid = -1;

	IDVS = -1;
	IDPS = -1;

	UseGraphics = false;
}

AIGrid::~AIGrid()
{
	Clear();
	mem_release(AIQuadDecl);
	mem_release(BoundBox);
	mem_release(VertexQuad);
	mem_release(IndexQuad);
}

void AIGrid::GridSave(const char* path)
{
	if (ArrQuads.size() <= 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: AI grid not found\n");
		return;
	}

	if (CountSplits <= 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: can not find path, because AI grid is not validate splits\n");
		return;
	}

	FILE* file = 0;
	file = fopen(path, "wb");

	fwrite("aigrid\0", 7, 1, file);
	fwrite(&float3_t((ArrBound[0]->max.x + ArrBound[0]->min.x) * 0.5f), sizeof(float3_t), 1, file);
	fwrite(&float3_t(ArrBound[0]->max - ArrBound[0]->min), sizeof(float3_t), 1, file);

	int32_t CountBB = ArrBound.size() - 1;
	fwrite(&CountBB, sizeof(int32_t), 1, file);
	int32_t tmpCountObjectBBQuads = 0;
	int tmpcountids = 0;
	for (int32_t i = 1; i<ArrBound.size(); i++)
	{
		fwrite(&(ArrBound[i]->min), sizeof(float3_t), 1, file);
		fwrite(&(ArrBound[i]->max), sizeof(float3_t), 1, file);

		tmpCountObjectBBQuads = ArrBound[i]->ArrIdsQuads.size();
		fwrite(&tmpCountObjectBBQuads, sizeof(int32_t), 1, file);
		tmpcountids += tmpCountObjectBBQuads;
		if (tmpCountObjectBBQuads > 0)
		{
			fwrite(&(ArrBound[i]->ArrIdsQuads[0]), sizeof(ID), tmpCountObjectBBQuads, file);
		}
	}

	int32_t CountObject = ArrQuads.size();
	fwrite(&CountObject, sizeof(int32_t), 1, file);
	//fwrite((ArrQuads[0]), sizeof(AIQuad), CountObject, file);

	for (int32_t i = 0; i<CountObject; ++i)
	{
		AIQuad* tmpaq = ArrQuads[i];
		fwrite(tmpaq, sizeof(AIQuad), 1, file);
	}

	CountObject = ArrGraphPointsIDs.size();
	fwrite(&CountObject, sizeof(int32_t), 1, file);
	if (CountObject > 0)
		fwrite(&(ArrGraphPointsIDs[0]), sizeof(AIQuad), CountObject, file);

	fclose(file);
}

void AIGrid::GridLoad(const char* path)
{
	Clear();
	float3_t tmpcenter, tmpparam;

	FILE* file = 0;
	file = fopen(path, "rb");

	char HeaderFile[7];

	fread(HeaderFile, 7, 1, file);

	if (strcmp(HeaderFile, "aigrid") == 0)
	{
		fread(&tmpcenter, sizeof(float3_t), 1, file);
		fread(&tmpparam, sizeof(float3_t), 1, file);

		BBCreate(&float3(tmpcenter), &float3(tmpparam));
		Max = ArrBound[0]->max;
		Min = ArrBound[0]->min;
		int32_t CountObjectBB = 0;
		fread(&CountObjectBB, sizeof(int32_t), 1, file);

		float3_t tmpmin, tmpmax;
		int tmpcountids = 0;

		for (int32_t i = 0; i<CountObjectBB; ++i)
		{
			BoundAIQuad* tmpbb = AllocBoundAIQuad.Alloc();
			fread(&(tmpbb->min), sizeof(float3_t), 1, file);
			fread(&(tmpbb->max), sizeof(float3_t), 1, file);

			tmpbb->lmin.x = toint100(tmpbb->min.x);
			tmpbb->lmin.y = toint100(tmpbb->min.y);
			tmpbb->lmin.z = toint100(tmpbb->min.z);

			tmpbb->lmax.x = toint100(tmpbb->max.x);
			tmpbb->lmax.y = toint100(tmpbb->max.y);
			tmpbb->lmax.z = toint100(tmpbb->max.z);

			int32_t tmpCountObjectBBQuads = 0;
			fread(&tmpCountObjectBBQuads, sizeof(int32_t), 1, file);
			tmpcountids += tmpCountObjectBBQuads;
			if (tmpCountObjectBBQuads > 0)
			{
				tmpbb->ArrIdsQuads.resize(tmpCountObjectBBQuads);
				fread(&(tmpbb->ArrIdsQuads[0]), sizeof(ID), tmpCountObjectBBQuads, file);
			}

			ArrBound.push_back(tmpbb);
		}

		int32_t CountObject = 0;
		fread(&CountObject, sizeof(int32_t), 1, file);
		ArrQuads.reserve(CountObject);
		for (int32_t i = 0; i<CountObject; ++i)
		{
			AIQuad* tmpaq = AllocAIQuad.Alloc();
			fread(tmpaq, sizeof(AIQuad), 1, file);
			ArrQuads.push_back(tmpaq);
		}

		CountObject = 0;
		fread(&CountObject, sizeof(int32_t), 1, file);
		if (CountObject > 0)
		{
			ArrGraphPointsIDs.resize(CountObject);
			fread(&(ArrGraphPointsIDs[0]), sizeof(ID), CountObject, file);
		}
	}

	fclose(file);
	ReCreateBuffs();

	reportf(REPORT_MSG_LEVEL_NOTICE, "sxaigrid: AI grid is loaded, count quads %d, count graph points %d\n", ArrQuads.size(), ArrGraphPointsIDs.size());

	GridTestValidation();
}

UINT AIGrid::GridGetCountQuads()
{
	return ArrQuads.size();
}

void AIGrid::ReCreateBuffs()
{
	//GridClear();
	mem_release_del(TransVertBuf);
	if (ArrQuads.size() <= 0)
	{
		return;
	}
	
	if (UseGraphics)
	{
		DXDevice->CreateVertexBuffer(
			ArrQuads.size() * sizeof(AIQuadVertexDataInst),
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&TransVertBuf,
			0);
	}

	ArrColor.resize(ArrQuads.size());
	ArrIDsInOpen.resize(ArrQuads.size());
	ArrCostQuads.resize(ArrQuads.size());
	ArrOpenIDs.resize(ArrQuads.size());
	ArrParentIDs.resize(ArrQuads.size());
	ArrCloseIDs.resize(ArrQuads.size());
	ArrState.resize(ArrQuads.size());
	ArrPreCondFreeState.resize(ArrQuads.size());
	memset(&(ArrColor[0]), 0, ArrColor.size() * sizeof(uint32_t));
	memset(&(ArrIDsInOpen[0]), -1, ArrIDsInOpen.size() * sizeof(ID));
	memset(&(ArrCostQuads[0]), -1, ArrCostQuads.size() * sizeof(CostAIQuad));
	memset(&(ArrOpenIDs[0]), -1, ArrOpenIDs.size() * sizeof(ID));
	memset(&(ArrParentIDs[0]), -1, ArrParentIDs.size() * sizeof(ID));
	memset(&(ArrCloseIDs[0]), 0, ArrCloseIDs.size() * sizeof(bool));
	memset(&(ArrState[0]), (int)AIQUAD_STATE_FREE, ArrState.size() * sizeof(AIQUAD_STATE));
	memset(&(ArrPreCondFreeState[0]), false, ArrPreCondFreeState.size() * sizeof(bool));
}

void AIGrid::DefInitBuffs(ID id)
{
	mem_release_del(TransVertBuf);
	if (ArrQuads.size() <= 0)
	{
		return;
	}

	if (UseGraphics)
	{
		DXDevice->CreateVertexBuffer(
			ArrQuads.size() * sizeof(AIQuadVertexDataInst),
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&TransVertBuf,
			0);
	}

	ArrColor[id] = 0;
	ArrIDsInOpen[id] = -1;
	ArrCostQuads[id] = CostAIQuad();
	ArrOpenIDs[id] = -1;
	ArrParentIDs[id] = -1;
	ArrCloseIDs[id] = 0;
	ArrState[id] = AIQUAD_STATE_FREE;
	ArrPreCondFreeState[id] = true;
}

void AIGrid::BBCreate(const float3* center, const float3* param)
{
	if (ArrBound.size() > 1)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: bound box already splitting, unresolved create");
		return;
	}

	if (!center || !param)
		return;

	BoundAIQuad* tmpbb;
	
	if(ArrBound.size() > 0)
		tmpbb = ArrBound[0];
	else
		tmpbb = AllocBoundAIQuad.Alloc();

	tmpbb->max = float3_t((*center) + (*param)*0.5f);
	tmpbb->min = float3_t((*center) - (*param)*0.5f);
	mem_release(BoundBox);

	if (UseGraphics)
		SGCore_FCreateBoundingBoxMesh(&((float3)tmpbb->min), &((float3)tmpbb->max), &(BoundBox));
	ArrBound.push_back(tmpbb);
}

inline bool AIGrid::BBIsCreated() const
{
	return (ArrBound.size() > 0);
}

void AIGrid::BBSetDimensions(const float3* dim)
{
	if (ArrBound.size() <= 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: bound box is not created");
		return;
	}

	if (ArrBound.size() > 1)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: bound box already splitting, unresolved set dimensions");
		return;
	}

	if (!dim)
		return;

	float3 tmpdim = ArrBound[0]->max - ArrBound[0]->min;
	float3 tmpcx = (ArrBound[0]->max + ArrBound[0]->min) * 0.5f;
	ArrBound[0]->min = (float3_t)(-(*dim) * 0.5f);
	ArrBound[0]->max = (float3_t)((*dim) * 0.5f);
	ArrBound[0]->min = (float3_t)(ArrBound[0]->min + tmpcx);
	ArrBound[0]->max = (float3_t)(ArrBound[0]->max + tmpcx);

	float3 tmpdim2 = ArrBound[0]->max - ArrBound[0]->min;
	float3 tmpscale = tmpdim2 / tmpdim;

	if (UseGraphics)
	{
		char* tmppos;
		BoundBox->LockVertexBuffer(0, (void**)&tmppos);
		for (int i = 0; i < BoundBox->GetNumVertices(); i++)
		{
			float3_t* tpos = (float3_t*)((char*)(tmppos)+BoundBox->GetNumBytesPerVertex() * i);
			tpos->x *= tmpscale.x;
			tpos->y *= tmpscale.y;
			tpos->z *= tmpscale.z;
		}
		BoundBox->UnlockVertexBuffer();
	}
}

void AIGrid::BBGetDimensions(float3* dim) const
{
	if (ArrBound.size() <= 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: bound box is not created");
		return;
	}

	if (!dim)
		return;

	dim->x = ArrBound[0]->max.x - ArrBound[0]->min.x;
	dim->y = ArrBound[0]->max.y - ArrBound[0]->min.y;
	dim->z = ArrBound[0]->max.z - ArrBound[0]->min.z;
}

void AIGrid::BBSetPos(const float3* pos)
{
	if (ArrBound.size() <= 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: bound box is not created");
		return;
	}

	if(ArrBound.size() > 1)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: bound box already splitting, unresolved set position");
		return;
	}

	if (!pos)
		return;

	float3 tmpcenter = (float3(ArrBound[0]->max) + float3(ArrBound[0]->min)) * 0.5f;
	ArrBound[0]->min = (float3_t)(float3(ArrBound[0]->min) + (-tmpcenter + (*pos)));
	ArrBound[0]->max = (float3_t)(float3(ArrBound[0]->max) + (-tmpcenter + (*pos)));
	
	if (UseGraphics)
	{
		char* tmppos;
		BoundBox->LockVertexBuffer(0, (void**)&tmppos);
		for (int i = 0; i < BoundBox->GetNumVertices(); i++)
		{
			float3_t* tpos = (float3_t*)((char*)(tmppos)+BoundBox->GetNumBytesPerVertex() * i);
			tpos->x += -tmpcenter.x + (pos->x);
			tpos->y += -tmpcenter.y + (pos->y);
			tpos->z += -tmpcenter.z + (pos->z);
		}
		BoundBox->UnlockVertexBuffer();
	}
}

void AIGrid::BBGetPos(float3* pos) const
{
	if (ArrBound.size() <= 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: bound box is not created");
		return;
	}

	if (!pos)
		return;

	*pos = (float3(ArrBound[0]->max) + float3(ArrBound[0]->min)) * 0.5f;
}

void AIGrid::BBCreateFinish()
{
	if (ArrBound.size() == 1)
	{
		Max = float3_t(ArrBound[0]->max.x, ArrBound[0]->max.y, ArrBound[0]->max.z);
		Min = float3_t(ArrBound[0]->min.x, ArrBound[0]->min.y, ArrBound[0]->min.z);

		float tmpx = ArrBound[0]->max.x - ArrBound[0]->min.x;
		float tmpy = ArrBound[0]->max.y - ArrBound[0]->min.y;
		float tmpz = ArrBound[0]->max.z - ArrBound[0]->min.z;

		if (tmpx <= AIGRID_BBOXSIZE)
			tmpx = 1;
		else
			tmpx = tmpx / AIGRID_BBOXSIZE;

		if (tmpy <= AIGRID_BBOXSIZE)
			tmpy = 1;
		else
			tmpy = tmpy / AIGRID_BBOXSIZE;

		if (tmpz <= AIGRID_BBOXSIZE)
			tmpz = 1;
		else
			tmpz = tmpz / AIGRID_BBOXSIZE;

		SplitBB(tmpx, tmpy, tmpz);
	}
}

bool AIGrid::BBIsCreatedFinish() const
{
	return (ArrBound.size() > 1);
}

AIQUAD_STATE AIGrid::QuadGetState(ID id) const
{
	AIGRID_QUAD_PRECOND(id, AIQUAD_STATE::AIQUAD_STATE_FREE)
	return ArrState[id];
}

void AIGrid::QuadSetState(ID id, AIQUAD_STATE state)
{
	AIGRID_QUAD_PRECOND(id, _VOID);
	ArrState[id] = state;
}

void AIGrid::QuadSetPosY(ID id, float posy)
{
	AIGRID_QUAD_PRECOND(id, _VOID);
	ArrQuads[id]->pos.y = posy;
}

float AIGrid::QuadGetPosY(ID id) const
{
	AIGRID_QUAD_PRECOND(id, 0);
	return ArrQuads[id]->pos.y;
}

ID AIGrid::QuadAdd(const float3* pos)
{
	ID idquad = QuadGet(pos,false);

	AIQuad* aq = 0;
	if (idquad >= 0)
	{
		if (!(ArrQuads[idquad]->IsClose))
			return idquad;
		else
		{
			aq = ArrQuads[idquad];
			aq->IsClose = false;
		}
	}
	else
		aq = AllocAIQuad.Alloc();

	float3 tpos(*pos);
	CorrectPosXZ(&tpos);
	aq->pos.x = tpos.x;
	aq->pos.y = tpos.y;
	aq->pos.z = tpos.z;

	aq->pos.y += AIGRID_QUAD_ADDYPOS;
	ArrQuads.push_back(aq);
	idquad = ArrQuads.size() - 1;
	aq->Id = idquad;
	DefInitBuffs(idquad);

	ID idneighboor = QuadGet(&float3(float3(aq->pos) + float3(-AIGRID_QUAD_SIZE, 0, AIGRID_QUAD_SIZE)), false);
	if (IsValidIdInArrQuads(idneighboor))
	{
		aq->Arr[0] = idneighboor;
		ArrQuads[idneighboor]->Arr[7] = idquad;
	}

	idneighboor = QuadGet(&float3(float3(aq->pos) + float3(0, 0, AIGRID_QUAD_SIZE)), false);
	if (IsValidIdInArrQuads(idneighboor))
	{
		aq->Arr[1] = idneighboor;
		ArrQuads[idneighboor]->Arr[6] = idquad;

		if (IsValidIdInArrQuads(ArrQuads[idneighboor]->Arr[1]))
			ArrQuads[idneighboor]->axisy = AIGRID_QUAD_DIR_ALL;
		else
			ArrQuads[idneighboor]->axisy = AIGRID_QUAD_DIR_NEGATIVE;
	}

	idneighboor = QuadGet(&float3(float3(aq->pos) + float3(AIGRID_QUAD_SIZE, 0, AIGRID_QUAD_SIZE)), false);
	if (IsValidIdInArrQuads(idneighboor))
	{
		aq->Arr[2] = idneighboor;
		ArrQuads[idneighboor]->Arr[5] = idquad;
	}

	idneighboor = QuadGet(&float3(float3(aq->pos) + float3(-AIGRID_QUAD_SIZE, 0, 0)), false);
	if (IsValidIdInArrQuads(idneighboor))
	{
		aq->Arr[3] = idneighboor;
		ArrQuads[idneighboor]->Arr[4] = idquad;

		if (IsValidIdInArrQuads(ArrQuads[idneighboor]->Arr[3]))
			ArrQuads[idneighboor]->axisx = AIGRID_QUAD_DIR_ALL;
		else
			ArrQuads[idneighboor]->axisx = AIGRID_QUAD_DIR_POSITIVE;
	}

	idneighboor = QuadGet(&float3(float3(aq->pos) + float3(AIGRID_QUAD_SIZE, 0, 0)), false);
	if (IsValidIdInArrQuads(idneighboor))
	{
		aq->Arr[4] = idneighboor;
		ArrQuads[idneighboor]->Arr[3] = idquad;

		if (IsValidIdInArrQuads(ArrQuads[idneighboor]->Arr[4]))
			ArrQuads[idneighboor]->axisx = AIGRID_QUAD_DIR_ALL;
		else
			ArrQuads[idneighboor]->axisx = AIGRID_QUAD_DIR_NEGATIVE;
	}

	idneighboor = QuadGet(&float3(float3(aq->pos) + float3(-AIGRID_QUAD_SIZE, 0, -AIGRID_QUAD_SIZE)), false);
	if (IsValidIdInArrQuads(idneighboor))
	{
		aq->Arr[5] = idneighboor;
		ArrQuads[idneighboor]->Arr[2] = idquad;
	}

	idneighboor = QuadGet(&float3(float3(aq->pos) + float3(0, 0, -AIGRID_QUAD_SIZE)), false);
	if (IsValidIdInArrQuads(idneighboor))
	{
		aq->Arr[6] = idneighboor;
		ArrQuads[idneighboor]->Arr[1] = idquad;

		if (IsValidIdInArrQuads(ArrQuads[idneighboor]->Arr[6]))
			ArrQuads[idneighboor]->axisy = AIGRID_QUAD_DIR_ALL;
		else
			ArrQuads[idneighboor]->axisy = AIGRID_QUAD_DIR_POSITIVE;
	}

	idneighboor = QuadGet(&float3(float3(aq->pos) + float3(AIGRID_QUAD_SIZE, 0, -AIGRID_QUAD_SIZE)), false);
	if (IsValidIdInArrQuads(idneighboor))
	{
		aq->Arr[7] = idneighboor;
		ArrQuads[idneighboor]->Arr[0] = idquad;
	}

	//определяем возможные направления из квада по оси Y
	if (IsValidIdInArrQuads(aq->Arr[1]) && IsValidIdInArrQuads(aq->Arr[6]))
		aq->axisy = AIGRID_QUAD_DIR_ALL;
	else if (IsValidIdInArrQuads(aq->Arr[1]) && !IsValidIdInArrQuads(aq->Arr[6]))
		aq->axisy = AIGRID_QUAD_DIR_POSITIVE;
	else if (!IsValidIdInArrQuads(aq->Arr[1]) && IsValidIdInArrQuads(aq->Arr[6]))
		aq->axisy = AIGRID_QUAD_DIR_NEGATIVE;
	else
		aq->axisy = AIGRID_QUAD_DIR_NONE;

	//определяем возможные направления из квада по оси X
	if (IsValidIdInArrQuads(aq->Arr[3]) && IsValidIdInArrQuads(aq->Arr[4]))
		aq->axisx = AIGRID_QUAD_DIR_ALL;
	else if (IsValidIdInArrQuads(aq->Arr[3]) && !IsValidIdInArrQuads(aq->Arr[4]))
		aq->axisx = AIGRID_QUAD_DIR_NEGATIVE;
	else if (!IsValidIdInArrQuads(aq->Arr[3]) && IsValidIdInArrQuads(aq->Arr[4]))
		aq->axisx = AIGRID_QUAD_DIR_POSITIVE;
	else
		aq->axisx = AIGRID_QUAD_DIR_NONE;

	AddUniqQuadArrag(aq);
	CountSplits = 0;
	return idquad;
}

bool AIGrid::QuadDelete(ID id)
{
	AIGRID_QUAD_PRECOND(id, false);

	AIQuad* aq = ArrQuads[id];

	if (aq->Arr[0] != -1)
	{
		ArrQuads[aq->Arr[0]]->Arr[7] = -1;
	}

	if (aq->Arr[1] != -1)
	{
		
		ArrQuads[aq->Arr[1]]->Arr[6] = -1;
		if (IsValidIdInArrQuads(ArrQuads[aq->Arr[1]]->Arr[1]))
			ArrQuads[aq->Arr[1]]->axisy = AIGRID_QUAD_DIR_POSITIVE;
		else
			ArrQuads[aq->Arr[1]]->axisy = AIGRID_QUAD_DIR_NONE;
	}

	if (aq->Arr[2] != -1)
	{
		ArrQuads[aq->Arr[2]]->Arr[5] = -1;
	}

	if (aq->Arr[3] != -1)
	{
		ArrQuads[aq->Arr[3]]->Arr[4] = -1;
		if (IsValidIdInArrQuads(ArrQuads[aq->Arr[3]]->Arr[3]))
			ArrQuads[aq->Arr[3]]->axisx = AIGRID_QUAD_DIR_NEGATIVE;
		else
			ArrQuads[aq->Arr[3]]->axisx = AIGRID_QUAD_DIR_NONE;
	}

	if (aq->Arr[4] != -1)
	{
		ArrQuads[aq->Arr[4]]->Arr[3] = -1;
		if (IsValidIdInArrQuads(ArrQuads[aq->Arr[4]]->Arr[4]))
			ArrQuads[aq->Arr[4]]->axisx = AIGRID_QUAD_DIR_POSITIVE;
		else
			ArrQuads[aq->Arr[4]]->axisx = AIGRID_QUAD_DIR_NONE;
	}

	if (aq->Arr[5] != -1)
	{
		ArrQuads[aq->Arr[5]]->Arr[2] = -1;
	}

	if (aq->Arr[6] != -1)
	{
		ArrQuads[aq->Arr[6]]->Arr[1] = -1;
		if (IsValidIdInArrQuads(ArrQuads[aq->Arr[6]]->Arr[6]))
			ArrQuads[aq->Arr[6]]->axisy = AIGRID_QUAD_DIR_NEGATIVE;
		else
			ArrQuads[aq->Arr[6]]->axisy = AIGRID_QUAD_DIR_NONE;
	}

	if (aq->Arr[7] != -1)
	{
		ArrQuads[aq->Arr[7]]->Arr[0] = -1;
	}
	
	aq->IsClose = true;
	aq->axisx = aq->axisy = AIGRID_QUAD_DIR_NONE;
	for (int i = 0; i < 8; ++i)
	{
		aq->Arr[i] = -1;
	}
	CountSplits = 0;

	QuadDelete2(aq->Id);
	return true;
}

void AIGrid::QuadDeleteInvalidAll()
{
	AIQuad* aq = 0;
	ID iddel = -1;
	for (long i = 0, il = ArrQuads.size(); i < il; ++i)
	{
		aq = ArrQuads[i];		
		if (aq && (aq->IsClose || (aq->axisx == AIGRID_QUAD_DIR_NONE && aq->axisy == AIGRID_QUAD_DIR_NONE)))
		{
			QuadDelete(i);
			il = ArrQuads.size();
		}
	}
}

void AIGrid::QuadDelete2(ID id)
{
	AIGRID_QUAD_PRECOND(id, _VOID);

	AIQuad* aq = 0;
	for (long i = 0, il = ArrQuads.size(); i < il; ++i)
	{
		aq = ArrQuads[i];

		if (aq->Id > id)
			--(aq->Id);

		for (int k = 0; k < 8; ++k)
		{
			if (aq->Arr[k] > id)
				--(aq->Arr[k]);
			else if (aq->Arr[k] == id)
				aq->Arr[k] = -1;
		}
	}

	BoundAIQuad* bound = 0;
	long tmpx = toint100(ArrQuads[id]->pos.x);
	long tmpy = toint100(ArrQuads[id]->pos.y);
	long tmpz = toint100(ArrQuads[id]->pos.z);

	for (int i = 1, il = ArrBound.size(); i <il; ++i)
	{
		bound = ArrBound[i];
		for (int k = 0, kl = bound->ArrIdsQuads.size(); k < kl; ++k)
		{
			if (bound->ArrIdsQuads[k] > id)
				--(bound->ArrIdsQuads[k]);
			else if (bound->ArrIdsQuads[k] == id)
			{
				bound->ArrIdsQuads.erase(k);
				--k;
				kl = bound->ArrIdsQuads.size();
			}
		}
	}

	for (int i = 0, il = ArrGraphPointsIDs.size(); i < il; ++i)
	{
		if (ArrGraphPointsIDs[i] > id)
			--(ArrGraphPointsIDs[i]);
		else if (ArrGraphPointsIDs[i] == id)
			ArrGraphPointsIDs.erase(i);
	}

	ArrLongCoordQuads.erase(id);
	AllocAIQuad.Delete(ArrQuads[id]);
	ArrQuads.erase(id);

	/*mem_release(TransVertBuf);
	DXDevice->CreateVertexBuffer(
		ArrQuads.size() * sizeof(AIQuadVertexDataInst),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&TransVertBuf,
		0);*/

	ArrColor.erase(id);
	ArrIDsInOpen.erase(id);
	ArrCostQuads.erase(id);
	ArrOpenIDs.erase(id);
	ArrParentIDs.erase(id);
	ArrCloseIDs.erase(id);
	ArrState.erase(id);
	ArrPreCondFreeState.erase(id);
}

void AIGrid::QuadSelect(ID id, bool consider_prev)
{
	if (id < 0)
	{
		ArrSelected.clear();
		return;
	}

	AIGRID_QUAD_PRECOND(id, _VOID);

	if (!consider_prev)
		ArrSelected.clear();

	if (QuadSelectedExists(id))
		return;

	ArrSelected.push_back(id);
}

bool AIGrid::QuadSelectedExists(ID id)
{
	for (int i = 0, il = ArrSelected.size(); i < il; ++i)
	{
		if (ArrSelected[i] == id)
			return true;
	}

	return false;
}

bool AIGrid::QuadSelectedExistsNeightborDirect(ID id)
{
	AIGRID_QUAD_PRECOND(id, false);

	if (ArrSelected.size() <= 0)
		return false;

	if (QuadSelectedExists(id))
		return false;

	AIQuad* aq = ArrQuads[id];

	if (QuadSelectedExists(aq->Arr[1]))
		return true;

	if (QuadSelectedExists(aq->Arr[3]))
		return true;

	if (QuadSelectedExists(aq->Arr[4]))
		return true;

	if (QuadSelectedExists(aq->Arr[6]))
		return true;

	return false;
}

void AIGrid::QuadSelectedAddPosY(float posy)
{
	for (int i = 0, il = ArrSelected.size(); i < il; ++i)
	{
		if (ArrSelected[i] >= 0 && ArrQuads.size() > ArrSelected[i])
			ArrQuads[ArrSelected[i]]->pos.y += posy;
	}
}

void AIGrid::QuadSelectedDelete()
{
	for (int i = 0, il = ArrSelected.size(); i < il; ++i)
	{
		QuadDelete(ArrSelected[i]);
	}

	ArrSelected.clear();
}

void AIGrid::PhyNavigate(AIQuad* quad)
{
	quad->IsClose = AIQuadPhyNavigate(&(quad->pos));
	/*static btBoxShape boxfull(btVector3(AIGRID_QUAD_SIZEDIV2, AIGRID_ENTITY_MAX_HEIGHTDIV2, AIGRID_QUAD_SIZEDIV2));
	float3 start = quad->pos;
	start.y = quad->pos.y + AIGRID_ENTITY_MAX_HEIGHT;
	float3 end = quad->pos;
	//end.y = min->y - AIGRID_ENTITY_MAX_HEIGHT;
	static btDiscreteDynamicsWorld::ClosestConvexResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
	cb = btDiscreteDynamicsWorld::ClosestConvexResultCallback(F3_BTVEC(start), F3_BTVEC(end));
	
	static btTransform xForm;
	xForm.setOrigin(F3_BTVEC(start));
	xForm.getBasis().setIdentity();
	static btTransform xForm2;
	xForm2.setOrigin(F3_BTVEC(end));
	xForm2.getBasis().setIdentity();
	SXPhysics_GetDynWorld()->convexSweepTest(&boxfull, xForm, xForm2, cb);

	if (cb.hasHit())
	{
		quad->pos.y = cb.m_hitPointWorld[1];
		quad->IsClose = false;

		static btBoxShape boxoff(btVector3(AIGRID_QUAD_SIZEDIV2, (AIGRID_ENTITY_MAX_HEIGHT - AIGRID_QUADS_CENTERS_MAXHEIGHT) * 0.5, AIGRID_QUAD_SIZEDIV2));

		start = quad->pos;
		start.y = quad->pos.y + AIGRID_ENTITY_MAX_HEIGHTDIV2 + AIGRID_QUADS_CENTERS_MAXHEIGHT;
		static btVector3 vec;
		vec = btVector3(F3_BTVEC(start));
		cb = btDiscreteDynamicsWorld::ClosestConvexResultCallback(vec, vec);
		static btVector3 offs;
		for (int x = -1; x <= 1; ++x)
		{
			for (int z = -1; z <= 1; ++z)
			{
				offs[0] = 0.5f*float(x) * 0.01f;
				offs[1] = 1.f * 0.01f;
				offs[2] = 0.5f*float(z) * 0.01f;
				xForm.setOrigin(vec - offs);
				xForm.getBasis().setIdentity();
				xForm2.setOrigin(vec + offs);
				xForm2.getBasis().setIdentity();
				SXPhysics_GetDynWorld()->convexSweepTest(&boxoff, xForm, xForm2, cb);

				if (cb.hasHit())
				{
					quad->IsClose = true;
					return;
				}
			}
		}
	}
	else
		quad->IsClose = true;*/
}

void AIGrid::GraphicsInit()
{
	UseGraphics = true;
	DXDevice = SGCore_GetDXDevice();

	if (!DXDevice)
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "sxaigrid: %s - dx device is not init", gen_msg_location);
	}

	D3DVERTEXELEMENT9 InstanceAIQuad[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 28, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

		D3DDECL_END()
	};

	DXDevice->CreateVertexDeclaration(InstanceAIQuad, &AIQuadDecl);

	Tex_AIGrid = SGCore_LoadTexAddName("decal_aigrid1.dds", LoadTexType::ltt_const);

	IDVS = SGCore_ShaderLoad(ShaderType::st_vertex, "aigrid_quad.vs", "aigrid_quad", ShaderCheckDouble::scd_path);
	IDPS = SGCore_ShaderLoad(ShaderType::st_pixel, "aigrid_quad.ps", "aigrid_quad", ShaderCheckDouble::scd_path);


	IDirect3DTexture9* TexBB;
	DXDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &TexBB, NULL);
	D3DLOCKED_RECT LockedRect;
	
	TexBB->LockRect(0, &LockedRect, 0, 0);

	uint32_t* tmpOldColor = (uint32_t*)LockedRect.pBits + 0 * LockedRect.Pitch + 0 * sizeof(uint32_t);
	*tmpOldColor = AIGRID_COLOR_GBBOX;

	TexBB->UnlockRect(0);

	SGCore_LoadTexLoadTextures();
	Tex_BB = SGCore_LoadTexCreate("aigrid_tex_bb__", TexBB);

	DXDevice->CreateVertexBuffer(
                   4 * sizeof(AIQuadVertexData),
                   D3DUSAGE_WRITEONLY,
                   0,
                   D3DPOOL_MANAGED,
                   &VertexQuad,
                   0);

	DXDevice->CreateIndexBuffer(
                   6 * sizeof(WORD),
                   D3DUSAGE_WRITEONLY,
                   D3DFMT_INDEX16,
                   D3DPOOL_MANAGED,
                   &IndexQuad,
                   0);

	AIQuadVertexData* vertices;
	VertexQuad->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = AIQuadVertexData(-AIGRID_QUAD_SIZEDIV2, 0.0f, -AIGRID_QUAD_SIZEDIV2, 0.0, 1.0f);
	vertices[1] = AIQuadVertexData(-AIGRID_QUAD_SIZEDIV2, 0.0f,  AIGRID_QUAD_SIZEDIV2, 0.0, 0.0f);
	vertices[2] = AIQuadVertexData( AIGRID_QUAD_SIZEDIV2, 0.0f,  AIGRID_QUAD_SIZEDIV2, 1.0, 0.0f);
	vertices[3] = AIQuadVertexData( AIGRID_QUAD_SIZEDIV2, 0.0f, -AIGRID_QUAD_SIZEDIV2, 1.0, 1.0f);

	VertexQuad->Unlock();

	WORD* indices = 0;
	IndexQuad->Lock(0, 0, (void**)&indices, 0);

	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;

	IndexQuad->Unlock();
}

void AIGrid::Clear()
{
	AllocAIQuad.clear();
	ArrQuads.clear();

	AllocBoundAIQuad.clear();
	ArrBound.clear();

	mem_release_del(TransVertBuf);
	ArrColor.clear();
	ArrIDsInOpen.clear();
	ArrCostQuads.clear();
	ArrOpenIDs.clear();
	ArrParentIDs.clear();
	ArrCloseIDs.clear();
	ArrState.clear();
	ArrPreCondFreeState.clear();
	ArrGraphPointsIDs.clear();
	ArrCostGPIDs.clear();
	mem_release_del(BoundBox);
}

void AIGrid::GridClear()
{
	AllocAIQuad.clear();
	ArrQuads.clear();

	for (int i = 0, il = ArrBound.size(); i < il; ++i)
	{
		ArrBound[i]->ArrIdsQuads.clear();
	}

	mem_release_del(TransVertBuf);
	ArrColor.clear();
	ArrIDsInOpen.clear();
	ArrCostQuads.clear();
	ArrOpenIDs.clear();
	ArrParentIDs.clear();
	ArrCloseIDs.clear();
	ArrState.clear();
	ArrPreCondFreeState.clear();
	ArrGraphPointsIDs.clear();
	ArrCostGPIDs.clear();
}

void AIGrid::SplitBB(int xv,int yv,int zv)
{
	float xval = (ArrBound[0]->max.x - ArrBound[0]->min.x);
	float yval = (ArrBound[0]->max.y - ArrBound[0]->min.y);
	float zval = (ArrBound[0]->max.z - ArrBound[0]->min.z);

	float countxval = xval / float(xv);
	float countyval = yval / float(yv);
	float countzval = zval / float(zv);

	float cxval = (ArrBound[0]->max.x + ArrBound[0]->min.x)*0.5f;
	float cyval = (ArrBound[0]->max.y + ArrBound[0]->min.y)*0.5f;
	float czval = (ArrBound[0]->max.z + ArrBound[0]->min.z)*0.5f;

	ArrBound.reserve(xv*yv*zv);

		for(int x=0;x<xv;x++)
		{
				for(int y=0;y<yv;y++)
				{
						for(int z=0;z<zv;z++)
						{
							BoundAIQuad* tmpbb = AllocBoundAIQuad.Alloc();
							float3 center = float3(
													lerpf(ArrBound[0]->min.x,ArrBound[0]->max.x,float(x)/float(xv)) + countxval*0.5,
													lerpf(ArrBound[0]->min.y,ArrBound[0]->max.y,float(y)/float(yv)) + countyval*0.5,
													lerpf(ArrBound[0]->min.z,ArrBound[0]->max.z,float(z)/float(zv)) + countzval*0.5);

							tmpbb->max.x = countxval*0.5 + center.x;
							tmpbb->max.y = countyval*0.5 + center.y;
							tmpbb->max.z = countzval*0.5 + center.z;

							tmpbb->min.x = -(countxval*0.5) + center.x;
							tmpbb->min.y = -(countyval*0.5) + center.y;
							tmpbb->min.z = -(countzval*0.5) + center.z;

							tmpbb->lmax.x = toint100(tmpbb->max.x);
							tmpbb->lmax.y = toint100(tmpbb->max.y);
							tmpbb->lmax.z = toint100(tmpbb->max.z);

							tmpbb->lmin.x = toint100(tmpbb->min.x);
							tmpbb->lmin.y = toint100(tmpbb->min.y);
							tmpbb->lmin.z = toint100(tmpbb->min.z);

							//SkyXEngine::Core::WorkModel::CreateBoundingBoxMesh(&float3(tmpbb->min),&float3(tmpbb->max),&(tmpbb->BoundBox));
							ArrBound.push_back(tmpbb);
						}
				}
		}
}

void AIGrid::GridGenerate()
{
	if (ArrQuads.size() <= 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: it is necessary one quad in AI grid");
		return;
	}

	DWORD ttime = GetTickCount();
	int oldsizegrid = ArrQuads.size();

	while (true)
	{
		ID idquadunchecked = -1;

		//поиск первого не входящего ни в какой сплит
		for (int i = 0, il = ArrQuads.size(); i < il; ++i)
		{
			if (IsValidIdInArrQuads(i) && ArrQuads[i]->axisx == AIGRID_QUAD_DIR_NONE && ArrQuads[i]->axisy == AIGRID_QUAD_DIR_NONE)
			{
				idquadunchecked = i;
				break;
			}
		}

		//не входящие в какой-либо сплит не найдены
		if (idquadunchecked < 0)
			break;

		ArrQuadsCheck.push_back(ArrQuads[idquadunchecked]);

		DWORD tmptmp = 0;
		DWORD tmpcountkey = 0;
		while (ArrQuadsCheck.size() > 0 && ArrQuadsCheck.size() != tmptmp)
		{
			tmpcountkey = tmptmp;
			if (ArrQuadsCheck[tmpcountkey])
			{
				GenQuad(ArrQuadsCheck[tmpcountkey]);
			}
			++tmptmp;
		}
		ArrQuadsCheck.clear();
	}

	reportf(REPORT_MSG_LEVEL_NOTICE, "sxaigrid: AI grid generated, all count = %d, gen count = %d, count time = %d mlsec\n", ArrQuads.size(), ArrQuads.size() - oldsizegrid, GetTickCount() - ttime);
	
	CountSplits = 0;
	ReCreateBuffs();
}

void AIGrid::GraphPointGenerate()
{
	if (ArrQuads.size() <= 1)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: AI grid not found\n");
		return;
	}

	ArrGraphPointsIDs.clear();
	ArrCostGPIDs.clear();
	for (int k = 1, kl = ArrBound.size(); k<kl; ++k)
	{
		DWORD tmpgp = -1;
		float tmplastdist = 0;
		float tmplastdist2;
		float3 tmpcenterbb = (float3(ArrBound[k]->max) + float3(ArrBound[k]->min)) * 0.5f;

		for (DWORD j = 0; j<ArrBound[k]->ArrIdsQuads.size(); j++)
		{
			if ((tmplastdist2 = SMVector3Length2(tmpcenterbb - ArrQuads[ArrBound[k]->ArrIdsQuads[j]]->pos)) < tmplastdist || tmpgp == -1)
			{
				tmpgp = j;
				tmplastdist = tmplastdist2;
			}
		}

		if (tmpgp != -1)
		{
			ArrGraphPointsIDs.push_back(ArrBound[k]->ArrIdsQuads[tmpgp]);
		}
	}

	ArrCostGPIDs.resize(ArrGraphPointsIDs.size());
	if (ArrGraphPointsIDs.size() > 0)
	{
		memset(&(ArrCostGPIDs[0]), -1, ArrCostGPIDs.size() * sizeof(int32_t));
		reportf(REPORT_MSG_LEVEL_NOTICE, "sxaigrid: graph points is generated, count %d\n", ArrGraphPointsIDs.size());
	}
}

UINT AIGrid::GraphPointGetCount()
{
	return ArrGraphPointsIDs.size();
}

void AIGrid::GraphPointClear()
{
	ArrGraphPointsIDs.clear();
}

void AIGrid::GraphPointAdd(ID id)
{
	AIGRID_QUAD_PRECOND(id, _VOID);

	if (GraphPointGetKey(id) >= 0)
		return;

	ArrGraphPointsIDs.push_back(id);
}

void AIGrid::GraphPointDelete(ID id)
{
	AIGRID_QUAD_PRECOND(id, _VOID);

	ID gpkey = -1;
	if ((gpkey = GraphPointGetKey(id)) < 0)
		return;

	ArrGraphPointsIDs.erase(gpkey);
}

ID AIGrid::GraphPointGetKey(ID id)
{
	AIGRID_QUAD_PRECOND(id, -1);

	for (int i = 0, il = ArrGraphPointsIDs.size(); i < il; ++i)
	{
		if (ArrGraphPointsIDs[i] == id)
			return i;
	}

	return -1;
}

void AIGrid::AddUniqQuadArrag(const AIQuad* aq)
{
	long tmpx = toint100(aq->pos.x);
	long tmpy = toint100(aq->pos.y);
	long tmpz = toint100(aq->pos.z);
	BoundAIQuad* bound = 0;
	for (int i = 1, il = ArrBound.size(); i < il; ++i)
	{
		bound = ArrBound[i];
		//если центр квада входит в какой-либо бокс
		if (
			bound->lmin.x <= tmpx && bound->lmin.y <= tmpy && bound->lmin.z <= tmpz &&
			bound->lmax.x >= tmpx && bound->lmax.y >= tmpy && bound->lmax.z >= tmpz
			)
		{
			bound->ArrIdsQuads.push_back(aq->Id);
			break;
		}
	}

	ArrLongCoordQuads[aq->Id] = int3(tmpx, tmpy, tmpz);
}

void AIGrid::AddUniqQuadArrag2(const AIQuad* aq, ID idbb)
{
	if (!(idbb > 0 && ArrBound.size() > idbb))
	{
		return;
	}

	ArrBound[idbb]->ArrIdsQuads.push_back(aq->Id);
	ArrLongCoordQuads[aq->Id] = int3(toint100(aq->pos.x), toint100(aq->pos.y), toint100(aq->pos.z));
}

bool AIGrid::IsUniqQuadArrag(AIQuad* aq, ID * idquad, ID * idbb)
{
	int3 l3;
	ID tmpidbb = -1;
	ID tmpsizearr = ArrQuads.size();
	long tmpx = toint100(aq->pos.x);
	long tmpy = toint100(aq->pos.y);
	long tmpz = toint100(aq->pos.z);
	BoundAIQuad* bound = 0;

	for (UINT i = 1, il = ArrBound.size(); i<il; ++i)
	{
		bound = ArrBound[i];
		//если центр квада входит в какой-либо бокс
		if (
			bound->lmin.x <= tmpx && bound->lmin.y <= tmpy && bound->lmin.z <= tmpz &&
			bound->lmax.x >= tmpx && bound->lmax.y >= tmpy && bound->lmax.z >= tmpz
			)
		{
			tmpidbb = i;
			//анализируем все входящие в данный бокс квады
			for (UINT k = 0, kl = bound->ArrIdsQuads.size(); k < kl; ++k)
			{
				l3 = ArrLongCoordQuads[bound->ArrIdsQuads[k]];
				if (
					l3.x == tmpx &&
					l3.z == tmpz &&
					abs(l3.y - tmpy) < LAIGRID_QUADS_CENTERS_MAXHEIGHT
					)
				{
					*idquad = bound->ArrIdsQuads[k];
					
					if (idbb)
						*idbb = tmpidbb;
					return false;
				}
			}
		}
	}

	if (idbb)
		*idbb = tmpidbb;

	return true;
}

void AIGrid::GenQuad(AIQuad* aq)
{
	bool tmpiscom = true;
	bool tmpiscom2 = true;
	ID idquad = -1;
	ID idbb = -1;
	static float3_t tmppos;

	AIQuad *aq12, *aq21, *aq23, *aq32, *aqold;
	aq12 = aq21 = aq23 = aq32 = aqold = 0;
	bool isunic = false;

	// ^
	if (!IsValidIdInArrQuads(aq->Arr[1]))
	{
		tmppos = aq->pos;
		tmppos.z += AIGRID_QUAD_SIZE;

		if (
			Max.x >= tmppos.x + AIGRID_QUAD_SIZEDIV2 && Max.z >= tmppos.z + AIGRID_QUAD_SIZEDIV2 && 
			Max.y >= tmppos.y && Min.y <= tmppos.y &&
			Min.x <= tmppos.x - AIGRID_QUAD_SIZEDIV2 && Min.z <= tmppos.z - AIGRID_QUAD_SIZEDIV2
			)
		{
			aq12 = AllocAIQuad.Alloc();

			aq12->pos = tmppos;

			idquad = idbb = -1;
			isunic = IsUniqQuadArrag(aq12, &idquad, &idbb);

			if (isunic)
			{
				PhyNavigate(aq12);

				if (abs(aq->pos.y - aq12->pos.y) < AIGRID_QUADS_CENTERS_MAXHEIGHT && !(aq12->IsClose))
				{
					aq12->pos.y += AIGRID_QUAD_ADDYPOS;

					ArrQuadsCheck.push_back(aq12);
					aq12->Id = ArrQuads.size();
					ArrQuads.push_back(aq12);
					aq->Arr[1] = aq12->Id;
					AddUniqQuadArrag2(aq12, idbb);
				}
				else
				{
					AllocAIQuad.Delete(aq12);
					aq12 = 0;
				}
			}
			else
			{
				aqold = aq12;
				aq->Arr[1] = idquad;
				aq12 = ArrQuads[idquad];
				if (aq12->IsClose)
				{
					aq12->pos = aqold->pos;
					aq12->pos.y += AIGRID_QUAD_ADDYPOS;
					aq12->IsClose = false;
					ArrQuadsCheck.push_back(aq12);
				}

				AllocAIQuad.Delete(aqold);
				aqold = 0;
			}
		}
	}

	// \/
	if (!IsValidIdInArrQuads(aq->Arr[6]))
	{
		tmppos = aq->pos;
		tmppos.z -= AIGRID_QUAD_SIZE;

		if (
			Max.x >= tmppos.x + AIGRID_QUAD_SIZEDIV2 && Max.z >= tmppos.z + AIGRID_QUAD_SIZEDIV2 &&
			Max.y >= tmppos.y && Min.y <= tmppos.y &&
			Min.x <= tmppos.x - AIGRID_QUAD_SIZEDIV2 && Min.z <= tmppos.z - AIGRID_QUAD_SIZEDIV2
			)
		{
			aq32 = AllocAIQuad.Alloc();

			aq32->pos = tmppos;

			idquad = idbb = -1;
			isunic = IsUniqQuadArrag(aq32, &idquad, &idbb);

			if (isunic)
			{
				PhyNavigate(aq32);

				if (abs(aq->pos.y - aq32->pos.y) < AIGRID_QUADS_CENTERS_MAXHEIGHT && !(aq32->IsClose))
				{
					aq32->pos.y += AIGRID_QUAD_ADDYPOS;

					ArrQuadsCheck.push_back(aq32);
					aq32->Id = ArrQuads.size();
					ArrQuads.push_back(aq32);
					aq->Arr[6] = aq32->Id;
					AddUniqQuadArrag2(aq32, idbb);
				}
				else
				{
					AllocAIQuad.Delete(aq32);
					aq32 = 0;
				}
			}
			else
			{
				aqold = aq32;
				aq->Arr[6] = idquad;
				aq32 = ArrQuads[idquad];
				if (aq32->IsClose)
				{
					aq32->pos = aqold->pos;
					aq32->pos.y += AIGRID_QUAD_ADDYPOS;
					aq32->IsClose = false;
					ArrQuadsCheck.push_back(aq32);
				}

				AllocAIQuad.Delete(aqold);
				aqold = 0;
			}
		}
	}

	// ->
	if (!IsValidIdInArrQuads(aq->Arr[4]))
	{
		tmppos = aq->pos;
		tmppos.x += AIGRID_QUAD_SIZE;

		if (
			Max.x >= tmppos.x + AIGRID_QUAD_SIZEDIV2 && Max.z >= tmppos.z + AIGRID_QUAD_SIZEDIV2 &&
			Max.y >= tmppos.y && Min.y <= tmppos.y &&
			Min.x <= tmppos.x - AIGRID_QUAD_SIZEDIV2 && Min.z <= tmppos.z - AIGRID_QUAD_SIZEDIV2
			)
		{
			aq23 = AllocAIQuad.Alloc();

			aq23->pos = tmppos;

			idquad = idbb = -1;
			isunic = IsUniqQuadArrag(aq23, &idquad, &idbb);

			if (isunic)
			{
				PhyNavigate(aq23);

				if (abs(aq->pos.y - aq23->pos.y) < AIGRID_QUADS_CENTERS_MAXHEIGHT && !(aq23->IsClose))
				{
					aq23->pos.y += AIGRID_QUAD_ADDYPOS;

					ArrQuadsCheck.push_back(aq23);
					aq23->Id = ArrQuads.size();
					ArrQuads.push_back(aq23);
					aq->Arr[4] = aq23->Id;
					AddUniqQuadArrag2(aq23, idbb);
				}
				else
				{
					AllocAIQuad.Delete(aq23);
					aq23 = 0;
				}
			}
			else
			{
				aqold = aq23;
				aq->Arr[4] = idquad;
				aq23 = ArrQuads[idquad];
				if (aq23->IsClose)
				{
					aq23->pos = aqold->pos;
					aq23->pos.y += AIGRID_QUAD_ADDYPOS;
					aq23->IsClose = false;
					ArrQuadsCheck.push_back(aq23);
				}

				AllocAIQuad.Delete(aqold);
				aqold = 0;
			}
		}
	}

	// <-
	if (!IsValidIdInArrQuads(aq->Arr[3]))
	{
		tmppos = aq->pos;
		tmppos.x -= AIGRID_QUAD_SIZE;

		if (
			Max.x >= tmppos.x + AIGRID_QUAD_SIZEDIV2 && Max.z >= tmppos.z + AIGRID_QUAD_SIZEDIV2 &&
			Max.y >= tmppos.y && Min.y <= tmppos.y &&
			Min.x <= tmppos.x - AIGRID_QUAD_SIZEDIV2 && Min.z <= tmppos.z - AIGRID_QUAD_SIZEDIV2
			)
		{
			aq21 = AllocAIQuad.Alloc();

			aq21->pos = tmppos;

			idquad = idbb = -1;
			isunic = IsUniqQuadArrag(aq21, &idquad, &idbb);

			if (isunic)
			{
				PhyNavigate(aq21);

				if (abs(aq->pos.y - aq21->pos.y) < AIGRID_QUADS_CENTERS_MAXHEIGHT && !(aq21->IsClose))
				{
					aq21->pos.y += AIGRID_QUAD_ADDYPOS;

					ArrQuadsCheck.push_back(aq21);
					aq21->Id = ArrQuads.size();
					ArrQuads.push_back(aq21);
					aq->Arr[3] = aq21->Id;
					AddUniqQuadArrag2(aq21, idbb);
				}
				else
				{
					AllocAIQuad.Delete(aq21);
					aq21 = 0;
				}
			}
			else
			{
				aqold = aq21;
				aq->Arr[3] = idquad;
				aq21 = ArrQuads[idquad];
				if (aq21->IsClose)
				{
					aq21->pos = aqold->pos;
					aq21->pos.y += AIGRID_QUAD_ADDYPOS;
					aq21->IsClose = false;
					ArrQuadsCheck.push_back(aq21);
				}

				AllocAIQuad.Delete(aqold);
				aqold = 0;
			}
		}
	}

	//определяем возможные направления из квада по оси Y
	if (IsValidIdInArrQuads(aq->Arr[1]) && IsValidIdInArrQuads(aq->Arr[6]))
		aq->axisy = AIGRID_QUAD_DIR_ALL;
	else if (IsValidIdInArrQuads(aq->Arr[1]) && !IsValidIdInArrQuads(aq->Arr[6]))
		aq->axisy = AIGRID_QUAD_DIR_POSITIVE;
	else if (!IsValidIdInArrQuads(aq->Arr[1]) && IsValidIdInArrQuads(aq->Arr[6]))
		aq->axisy = AIGRID_QUAD_DIR_NEGATIVE;
	else
		aq->axisy = AIGRID_QUAD_DIR_NONE;

	//определяем возможные направления из квада по оси X
	if (IsValidIdInArrQuads(aq->Arr[3]) && IsValidIdInArrQuads(aq->Arr[4]))
		aq->axisx = AIGRID_QUAD_DIR_ALL;
	else if (IsValidIdInArrQuads(aq->Arr[3]) && !IsValidIdInArrQuads(aq->Arr[4]))
		aq->axisx = AIGRID_QUAD_DIR_NEGATIVE;
	else if (!IsValidIdInArrQuads(aq->Arr[3]) && IsValidIdInArrQuads(aq->Arr[4]))
		aq->axisx = AIGRID_QUAD_DIR_POSITIVE;
	else
		aq->axisx = AIGRID_QUAD_DIR_NONE;

	if (aq->axisy == AIGRID_QUAD_DIR_NONE && aq->axisx == AIGRID_QUAD_DIR_NONE)
	{
		aq->IsClose = true;
		QuadDelete(aq->Id);
	}
	else
		ComputeNeighbor(aq->Id);
}

void AIGrid::ComputeNeighbor(ID id)
{
	AIQuad* aq = ArrQuads[id];
	AIQuad *aq12, *aq21, *aq23, *aq32;
	aq12 = aq21 = aq23 = aq32 = 0;

	AIQuad *aq11, *aq13, *aq31, *aq33;
	aq11 = aq13 = aq31 = aq33 = 0;

	if (aq)
	{
		if (aq->Arr[0] != -1)
			aq11 = ArrQuads[aq->Arr[0]];

		if (aq->Arr[1] != -1)
			aq12 = ArrQuads[aq->Arr[1]];

		if (aq->Arr[2] != -1)
			aq13 = ArrQuads[aq->Arr[2]];

		if (aq->Arr[3] != -1)
			aq21 = ArrQuads[aq->Arr[3]];

		if (aq->Arr[4] != -1)
			aq23 = ArrQuads[aq->Arr[4]];

		if (aq->Arr[5] != -1)
			aq31 = ArrQuads[aq->Arr[5]];

		if (aq->Arr[6] != -1)
			aq32 = ArrQuads[aq->Arr[6]];

		if (aq->Arr[7] != -1)
			aq33 = ArrQuads[aq->Arr[7]];

		if (aq->Arr[1] != -1)
		{
			//указываем самый нижний соседом
			aq12->SetNeighbor(6, aq->Id);
			//если левый нижний создан то он тоже сосед
			if (aq->Arr[3] != -1)
				aq12->SetNeighbor(5, aq->Arr[3]);

			//если правый нижний создан то он тоже сосед
			if (aq->Arr[4] != -1)
				aq12->SetNeighbor(7, aq->Arr[4]);

			//если у обрабатываемого нода есть верхний левый
			if (aq->Arr[0] != -1)
			{
				if (AIGridCompareHeight(aq->Arr[1], aq->Arr[0]))
					aq12->SetNeighbor(3, aq->Arr[0]);

				if (AIGridCompareHeight(aq->Arr[0], aq->Arr[1]))
					aq11->SetNeighbor(4, aq->Arr[1]);

				if (aq11->Arr[1] != -1)
				{
					if (AIGridCompareHeight(aq11->Arr[1], aq->Arr[1]))
						ArrQuads[aq11->Arr[1]]->SetNeighbor(7, aq->Arr[1]);

					if (AIGridCompareHeight(aq->Arr[1], aq11->Arr[1]))
						aq12->SetNeighbor(0, aq11->Arr[1]);
				}

				if (aq11->Arr[1] != -1 && ArrQuads[aq11->Arr[1]]->Arr[4] != -1)
				{
					if (AIGridCompareHeight(ArrQuads[aq11->Arr[1]]->Arr[4], aq->Arr[1]))
						ArrQuads[ArrQuads[aq11->Arr[1]]->Arr[4]]->SetNeighbor(6, aq->Arr[1]);

					if (AIGridCompareHeight(aq->Arr[1], ArrQuads[aq11->Arr[1]]->Arr[4]))
						aq12->SetNeighbor(1, ArrQuads[aq11->Arr[1]]->Arr[4]);
				}
				else if (aq11->Arr[2] != -1)
				{
					if (AIGridCompareHeight(aq11->Arr[2], aq->Arr[1]))
						ArrQuads[aq11->Arr[2]]->SetNeighbor(6, aq->Arr[1]);

					if (AIGridCompareHeight(aq->Arr[1], aq11->Arr[2]))
						aq12->SetNeighbor(1, aq11->Arr[2]);
				}
			}

			//если у обрабатываемого нода есть верхний правый
			if (aq->Arr[2] != -1)
			{
				if (AIGridCompareHeight(aq->Arr[1], aq->Arr[2]))
					aq12->SetNeighbor(4, aq->Arr[2]);

				if (AIGridCompareHeight(aq->Arr[1], aq->Arr[1]))
					aq13->SetNeighbor(3, aq->Arr[1]);

				if (aq13->Arr[1] != -1)
				{
					if (AIGridCompareHeight(aq13->Arr[1], aq->Arr[1]))
						ArrQuads[aq13->Arr[1]]->SetNeighbor(5, aq->Arr[1]);

					if (AIGridCompareHeight(aq->Arr[1], aq13->Arr[1]))
						aq12->SetNeighbor(2, aq13->Arr[1]);
				}

				if (aq12->Arr[1] == -1)
				{
					if (aq13->Arr[1] != -1 && ArrQuads[aq13->Arr[1]]->Arr[3] != -1)
					{
						if (AIGridCompareHeight(ArrQuads[aq13->Arr[1]]->Arr[3], aq->Arr[1]))
							ArrQuads[ArrQuads[aq13->Arr[1]]->Arr[3]]->SetNeighbor(6, aq->Arr[1]);

						if (AIGridCompareHeight(aq->Arr[1], ArrQuads[aq13->Arr[1]]->Arr[3]))
							aq12->SetNeighbor(1, ArrQuads[aq13->Arr[1]]->Arr[3]);
					}
					else if (aq13->Arr[0] != -1)
					{
						if (AIGridCompareHeight(aq13->Arr[0], aq->Arr[1]))
							ArrQuads[aq13->Arr[0]]->SetNeighbor(6, aq->Arr[1]);

						if (AIGridCompareHeight(aq->Arr[1], aq13->Arr[0]))
							aq12->SetNeighbor(1, aq13->Arr[0]);
					}
				}
			}


		}


		//если создали нижний нод
		if (aq->Arr[6] != -1)
		{
			//указываем самый верхний соседом
			//aq32->Arr[1] = aq->Id;
			aq32->SetNeighbor(1, aq->Id);
			//если левый верхний создан то он тоже сосед
			if (aq->Arr[3] != -1)
				aq32->SetNeighbor(0, aq->Arr[3]);

			//если правый верхний создан то он тоже сосед
			if (aq->Arr[4] != -1)
				aq32->SetNeighbor(2, aq->Arr[4]);

			//если у обрабатываемого нода есть нижний левый
			if (aq->Arr[5] != -1)
			{
				if (AIGridCompareHeight(aq->Arr[6], aq->Arr[5]))
					aq32->SetNeighbor(3, aq->Arr[5]);

				if (AIGridCompareHeight(aq->Arr[5], aq->Arr[6]))
					aq31->SetNeighbor(4, aq->Arr[6]);

				if (aq31->Arr[6] != -1)
				{
					if (AIGridCompareHeight(aq->Arr[6], aq31->Arr[6]))
						aq32->SetNeighbor(5, aq31->Arr[6]);

					if (AIGridCompareHeight(aq31->Arr[6], aq->Arr[6]))
						ArrQuads[aq31->Arr[6]]->SetNeighbor(2, aq->Arr[6]);
				}

				if (aq31->Arr[6] != -1 && ArrQuads[aq31->Arr[6]]->Arr[4] != -1)
				{
					if (AIGridCompareHeight(ArrQuads[aq31->Arr[6]]->Arr[4], aq->Arr[6]))
						ArrQuads[ArrQuads[aq31->Arr[6]]->Arr[4]]->SetNeighbor(1, aq->Arr[6]);

					if (AIGridCompareHeight(aq->Arr[6], ArrQuads[aq31->Arr[6]]->Arr[4]))
						aq32->SetNeighbor(6, ArrQuads[aq31->Arr[6]]->Arr[4]);
				}
				else if (aq31->Arr[7] != -1)
				{
					if (AIGridCompareHeight(aq31->Arr[7], aq->Arr[6]))
						ArrQuads[aq31->Arr[7]]->SetNeighbor(1, aq->Arr[6]);

					if (AIGridCompareHeight(aq->Arr[6], aq31->Arr[7]))
						aq32->SetNeighbor(6, aq31->Arr[7]);
				}
			}

			//если у обрабатываемого нода есть нижний правый
			if (aq->Arr[7] != -1)
			{
				if (AIGridCompareHeight(aq->Arr[6], aq->Arr[7]))
					aq32->SetNeighbor(4, aq->Arr[7]);

				if (AIGridCompareHeight(aq->Arr[7], aq->Arr[6]))
					aq33->SetNeighbor(3, aq->Arr[6]);

				if (aq33->Arr[6] != -1)
				{
					if (AIGridCompareHeight(aq->Arr[7], aq->Arr[6]))
						ArrQuads[aq33->Arr[6]]->SetNeighbor(0, aq->Arr[6]);

					if (AIGridCompareHeight(aq->Arr[6], aq33->Arr[6]))
						aq32->SetNeighbor(7, aq33->Arr[6]);
				}

				if (aq32->Arr[6] == -1)
				{
					if (aq33->Arr[6] != -1 && ArrQuads[aq33->Arr[6]]->Arr[3] != -1)
					{
						if (AIGridCompareHeight(ArrQuads[aq33->Arr[6]]->Arr[3], aq->Arr[6]))
							ArrQuads[ArrQuads[aq33->Arr[6]]->Arr[3]]->SetNeighbor(1, aq->Arr[6]);

						if (AIGridCompareHeight(aq->Arr[6], ArrQuads[aq33->Arr[6]]->Arr[3]))
							aq32->SetNeighbor(6, ArrQuads[aq33->Arr[6]]->Arr[3]);
					}
					else if (aq33->Arr[5] != -1)
					{
						if (AIGridCompareHeight(aq33->Arr[5], aq->Arr[6]))
							ArrQuads[aq33->Arr[5]]->SetNeighbor(1, aq->Arr[6]);

						if (AIGridCompareHeight(aq->Arr[6], aq33->Arr[5]))
							aq32->SetNeighbor(6, aq33->Arr[5]);
					}
				}
			}
		}


		//если создали левый нод
		if (aq->Arr[3] != -1)
		{
			//указываем самый правый соседом
			//aq21->Arr[4] = aq->Id;
			aq21->SetNeighbor(4, aq->Id);
			//если верхний создан то он тоже сосед
			if (aq->Arr[1] != -1)
				aq21->SetNeighbor(2, aq->Arr[1]);

			//если верхний создан то он тоже сосед
			if (aq->Arr[6] != -1)
				aq21->SetNeighbor(7, aq->Arr[6]);

			if (aq->Arr[0] != -1)
			{
				if (AIGridCompareHeight(aq->Arr[0], aq->Arr[3]))
					aq11->SetNeighbor(6, aq->Arr[3]);

				if (AIGridCompareHeight(aq->Arr[3], aq->Arr[0]))
					aq21->SetNeighbor(1, aq->Arr[0]);

				if (aq11->Arr[3] != -1)
				{
					if (AIGridCompareHeight(aq11->Arr[3], aq->Arr[3]))
						ArrQuads[aq11->Arr[3]]->SetNeighbor(7, aq->Arr[3]);

					if (AIGridCompareHeight(aq->Arr[3], aq11->Arr[3]))
						aq21->SetNeighbor(0, aq11->Arr[3]);
				}

				if (aq11->Arr[3] != -1 && ArrQuads[aq11->Arr[3]]->Arr[6] != -1)
				{
					if (AIGridCompareHeight(aq->Arr[3], ArrQuads[aq11->Arr[3]]->Arr[6]))
						aq21->SetNeighbor(3, ArrQuads[aq11->Arr[3]]->Arr[6]);

					if (AIGridCompareHeight(ArrQuads[aq11->Arr[3]]->Arr[6], aq->Arr[3]))
						ArrQuads[ArrQuads[aq11->Arr[3]]->Arr[6]]->SetNeighbor(4, aq->Arr[3]);
				}
				else if (aq11->Arr[5] != -1)
				{
					if (AIGridCompareHeight(aq->Arr[3], aq11->Arr[5]))
						aq21->SetNeighbor(3, aq11->Arr[5]);

					if (AIGridCompareHeight(aq11->Arr[5], aq->Arr[3]))
						ArrQuads[aq11->Arr[5]]->SetNeighbor(4, aq->Arr[3]);
				}
			}

			if (aq->Arr[5] != -1)
			{
				if (AIGridCompareHeight(aq->Arr[5], aq->Arr[3]))
					aq31->SetNeighbor(1, aq->Arr[3]);

				if (AIGridCompareHeight(aq->Arr[3], aq->Arr[5]))
					aq21->SetNeighbor(6, aq->Arr[5]);

				if (aq31->Arr[3] != -1)
				{
					if (AIGridCompareHeight(aq31->Arr[3], aq->Arr[3]))
						ArrQuads[aq31->Arr[3]]->SetNeighbor(2, aq->Arr[3]);

					if (AIGridCompareHeight(aq->Arr[3], aq31->Arr[3]))
						aq21->SetNeighbor(5, aq31->Arr[3]);
				}

				if (aq21->Arr[3] == -1)
				{
					if (aq31->Arr[3] != -1 && ArrQuads[aq31->Arr[3]]->Arr[1] != -1)
					{
						if (AIGridCompareHeight(ArrQuads[aq31->Arr[3]]->Arr[1], aq->Arr[3]))
							ArrQuads[ArrQuads[aq31->Arr[3]]->Arr[1]]->SetNeighbor(4, aq->Arr[3]);

						if (AIGridCompareHeight(aq->Arr[3], ArrQuads[aq31->Arr[3]]->Arr[1]))
							aq21->SetNeighbor(3, ArrQuads[aq31->Arr[3]]->Arr[1]);
					}
					else if (aq31->Arr[0] != -1)
					{
						if (AIGridCompareHeight(aq31->Arr[0], aq->Arr[3]))
							ArrQuads[aq31->Arr[0]]->SetNeighbor(4, aq->Arr[3]);

						if (AIGridCompareHeight(aq->Arr[3], aq31->Arr[0]))
							aq21->SetNeighbor(3, aq31->Arr[0]);
					}
				}
			}
		}

		//если создали правый нод
		if (aq->Arr[4] != -1)
		{
			//указываем самый левый соседом
			aq23->SetNeighbor(3, aq->Id);
			//если верхний создан то он тоже сосед
			if (aq->Arr[1] != -1)
				aq23->SetNeighbor(0, aq->Arr[1]);

			//если нижний создан то он тоже сосед
			if (aq->Arr[6] != -1)
				aq23->SetNeighbor(5, aq->Arr[6]);


			if (aq->Arr[2] != -1)
			{
				if (AIGridCompareHeight(aq->Arr[2], aq->Arr[4]))
					aq13->SetNeighbor(6, aq->Arr[4]);

				if (AIGridCompareHeight(aq->Arr[4], aq->Arr[2]))
					aq23->SetNeighbor(1, aq->Arr[2]);

				if (aq13->Arr[4] != -1)
				{
					if (AIGridCompareHeight(aq->Arr[4], aq13->Arr[4]))
						aq23->SetNeighbor(2, aq13->Arr[4]);

					if (AIGridCompareHeight(aq13->Arr[4], aq->Arr[4]))
						ArrQuads[aq13->Arr[4]]->SetNeighbor(5, aq->Arr[4]);
				}


				if (aq13->Arr[4] != -1 && ArrQuads[aq13->Arr[4]]->Arr[6] != -1)
				{
					if (AIGridCompareHeight(aq->Arr[4], ArrQuads[aq13->Arr[4]]->Arr[6]))
						aq23->SetNeighbor(4, ArrQuads[aq13->Arr[4]]->Arr[6]);

					if (AIGridCompareHeight(ArrQuads[aq13->Arr[4]]->Arr[6], aq->Arr[4]))
						ArrQuads[ArrQuads[aq13->Arr[4]]->Arr[6]]->SetNeighbor(3, aq->Arr[4]);
				}
				else if (aq13->Arr[7] != -1)
				{
					if (AIGridCompareHeight(aq->Arr[4], aq13->Arr[7]))
						aq23->SetNeighbor(4, aq13->Arr[7]);

					if (AIGridCompareHeight(aq13->Arr[7], aq->Arr[4]))
						ArrQuads[aq13->Arr[7]]->SetNeighbor(3, aq->Arr[4]);
				}
			}

			if (aq->Arr[7] != -1)
			{
				if (AIGridCompareHeight(aq->Arr[7], aq->Arr[4]))
					aq33->SetNeighbor(1, aq->Arr[4]);

				if (AIGridCompareHeight(aq->Arr[4], aq->Arr[7]))
					aq23->SetNeighbor(6, aq->Arr[7]);

				if (aq33->Arr[4] != -1)
				{
					if (AIGridCompareHeight(aq->Arr[4], aq33->Arr[4]))
						aq23->SetNeighbor(7, aq33->Arr[4]);

					if (AIGridCompareHeight(aq33->Arr[4], aq->Arr[4]))
						ArrQuads[aq33->Arr[4]]->SetNeighbor(0, aq->Arr[4]);
				}

				if (aq23->Arr[4] == -1)
				{
					if (aq33->Arr[4] != -1 && ArrQuads[aq33->Arr[4]]->Arr[1] != -1)
					{
						if (AIGridCompareHeight(aq->Arr[4], ArrQuads[aq33->Arr[4]]->Arr[1]))
							aq23->SetNeighbor(4, ArrQuads[aq33->Arr[4]]->Arr[1]);

						if (AIGridCompareHeight(ArrQuads[aq33->Arr[4]]->Arr[1], aq->Arr[4]))
							ArrQuads[ArrQuads[aq33->Arr[4]]->Arr[1]]->SetNeighbor(3, aq->Arr[4]);
					}
					else if (aq33->Arr[2] != -1)
					{
						if (AIGridCompareHeight(aq->Arr[4], aq33->Arr[2]))
							aq23->SetNeighbor(4, aq33->Arr[2]);

						if (AIGridCompareHeight(aq33->Arr[2], aq->Arr[4]))
							ArrQuads[aq33->Arr[2]]->SetNeighbor(3, aq->Arr[4]);
					}
				}
			}
		}
	}
}

ID AIGrid::QuadGet(const float3* pos, bool isnear_or_permissible) const
{
	long heightmin = -1;
	ID howf = -1;
	long tmpx = toint100(pos->x);
	long tmpy = toint100(pos->y);
	long tmpz = toint100(pos->z);

	for (long i = 1, il = ArrBound.size(); i<il; ++i)
	{
		if (
			ArrBound[i]->lmin.x - LAIGRID_QUAD_SIZE <= tmpx && ArrBound[i]->lmin.z - LAIGRID_QUAD_SIZE <= tmpz &&
			ArrBound[i]->lmax.x + LAIGRID_QUAD_SIZE >= tmpx && ArrBound[i]->lmax.z + LAIGRID_QUAD_SIZE >= tmpz
			)
		{
			for (DWORD k = 0, kl = ArrBound[i]->ArrIdsQuads.size(); k<kl; ++k)
			{
				if (
					abs((ArrLongCoordQuads[ArrBound[i]->ArrIdsQuads[k]].x) - (tmpx)) <= LAIGRID_QUAD_SIZEDIV2 &&
					abs((ArrLongCoordQuads[ArrBound[i]->ArrIdsQuads[k]].z) - (tmpz)) <= LAIGRID_QUAD_SIZEDIV2
					)
				{

					if (
						(isnear_or_permissible && ((heightmin > -1 && heightmin > abs(abs(ArrLongCoordQuads[ArrBound[i]->ArrIdsQuads[k]].y) - abs(tmpy))) || heightmin == -1)) ||
						(!isnear_or_permissible && (abs(abs(ArrLongCoordQuads[ArrBound[i]->ArrIdsQuads[k]].y) - abs(tmpy))  < LAIGRID_QUADS_CENTERS_MAXHEIGHT))
						)
					{
						//int3 l3 = ArrLongCoordQuads[ArrBound[i]->ArrIdsQuads[k]];
						//AIQuad* aq22 = ArrQuads[ArrBound[i]->ArrIdsQuads[k]];
						heightmin = abs(abs(ArrLongCoordQuads[ArrBound[i]->ArrIdsQuads[k]].y) - abs(tmpy));
						howf = ArrBound[i]->ArrIdsQuads[k];
					}
				}
			}
		}
	}
	return howf;
}

ID AIGrid::GridTraceBeam(const float3* start, const float3* dir) const
{
	ID idf = -1;
	SXTriangle tmptri;
	float3 ip;
	float3 res;
	float3 ildir;
	res = (*start) + float3(10000.0f, 10000.0f, 10000.0f);
	ildir = (*dir) * 10000.0f;

	float3 center;
	float radius;
	BoundAIQuad* bound = 0;
	float3 posquad, posquad2;

	for (int i = 0, il = ArrBound.size(); i < il; ++i)
	{
		bound = ArrBound[i];
		center = (bound->min + bound->max) * 0.5f;
		radius = SMVector3Length(center - bound->max);
		float distsqr = SGCore_0DistancePointBeam2(center, *start, *dir);
		if (distsqr <= radius*radius)
		{
			for (int k = 0, kl = bound->ArrIdsQuads.size(); k < kl; ++k)
			{
				posquad = (float3)ArrQuads[bound->ArrIdsQuads[k]]->pos;
				tmptri.a = (float3_t)(posquad + float3(-AIGRID_QUAD_SIZEDIV2, 0, -AIGRID_QUAD_SIZEDIV2));
				tmptri.b = (float3_t)(posquad + float3(AIGRID_QUAD_SIZEDIV2, 0, AIGRID_QUAD_SIZEDIV2));
				tmptri.c = (float3_t)(posquad + float3(AIGRID_QUAD_SIZEDIV2, 0, -AIGRID_QUAD_SIZEDIV2));
				if (tmptri.IntersectLine((*start), ildir, &ip))
				{
					if (SMVector3Length2((*start) - res) > SMVector3Length2((*start) - ip))
					{
						res = ip;
						idf = bound->ArrIdsQuads[k];
					}
				}

				tmptri.a = (float3_t)(posquad + float3(-AIGRID_QUAD_SIZEDIV2, 0, -AIGRID_QUAD_SIZEDIV2));
				tmptri.b = (float3_t)(posquad + float3(-AIGRID_QUAD_SIZEDIV2, 0, AIGRID_QUAD_SIZEDIV2));
				tmptri.c = (float3_t)(posquad + float3(AIGRID_QUAD_SIZEDIV2, 0, AIGRID_QUAD_SIZEDIV2));
				if (tmptri.IntersectLine((*start), ildir, &ip))
				{
					if (SMVector3Length2((*start) - res) > SMVector3Length2((*start) - ip))
					{
						res = ip;
						idf = bound->ArrIdsQuads[k];
					}
				}
			}
		}
	}

	return idf;
}


//!!! почему то возникла ситуация когда после прохода валидации, у одного (наверное у одного) из квадов айди сплита был == 0, чего в общемто не должно быть
//возможно надо просто проходится циклом по всему массиву квадов и смотреть где у какого квада айди сплита еще равен нулю, а перед этим проходом обнулить все эти айди

void AIGrid::GridTestValidation()
{
	if (ArrQuads.size() <= 1 || ArrBound.size() <= 1)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: AI grid is not created, unresolved validation\n");
		return;
	}

	int oldcountquads = ArrQuads.size();

	//удаление лишних квадов
	QuadDeleteInvalidAll();

	//подсчет и пометка сплитов
	//{

	for (int i = 0, il = ArrQuads.size(); i < il; ++i)
	{
		ArrQuads[i]->IdSplit = 0;
	}

	ArrTestSplitsIDs.clear();
	CountSplits = 0;
	
	while (true)
	{
		ID idquadunchecked = -1;
		
		//поиск первого не входящего ни в какой сплит
		for (int i = 0, il = ArrQuads.size(); i < il; ++i)
		{
			if (ArrQuads[i]->IdSplit == 0 && !ArrQuads[i]->IsClose)
			{
				idquadunchecked = i;
				break;
			}
		}

		//не входящие в какой-либо сплит не найдены
		if (idquadunchecked < 0)
			break;

		ID currkey = 0;
		ID tmpid = -1;
		ArrTestSplitsIDs.clear();
		ArrTestSplitsIDs[0] = idquadunchecked;
		++CountSplits;

		//начиная с первого невходящего квада проходимся по всем узлам к которым можно дойти, относя их к текущему идентификатору сплита
		while (ArrTestSplitsIDs.size() > currkey)
		{
			tmpid = ArrTestSplitsIDs[currkey];
			if (ArrQuads[tmpid]->IdSplit == 0)
			{
				ArrQuads[tmpid]->IdSplit = CountSplits;

				//заносим всех соседей в непомеченный список
				for (int i = 0; i < 8; ++i)
				{
					if (IsValidIdInArrQuads(ArrQuads[tmpid]->Arr[i]))
						ArrTestSplitsIDs.push_back(ArrQuads[tmpid]->Arr[i]);
				}
			}
			++currkey;
		}
	}

	for (int i = 0; i < CountSplits; ++i)
	{
		ArrSplitsColor[i] = D3DCOLOR_ARGB(200, rand() % 255, rand() % 255, rand() % 255);
	}
	//}

	reportf(REPORT_MSG_LEVEL_NOTICE, "sxaigrid: AI grid is validation, deleted quads %d, count splits %d\n", oldcountquads - ArrQuads.size(), CountSplits);
}

UINT AIGrid::GridGetCountSplits()
{
	return CountSplits;
}

void AIGrid::GridSetMarkSplits(bool mark)
{
	IsMarkSplits = mark;
}

bool AIGrid::GridGetMarkSplits()
{
	return IsMarkSplits;
}

void AIGrid::RenderBB()
{
	if (!UseGraphics)
		return;

	DXDevice->SetTexture(0, SGCore_LoadTexGetTex(Tex_BB));
	if (BoundBox)
		BoundBox->DrawSubset(0);
}

void AIGrid::RenderQuads(const ISXFrustum * frustum, const float3 * viewpos, float dist)
{
	if (!UseGraphics)
		return;

	if (!frustum)
		return;

	if (TransVertBuf)
	{
		AIQuadVertexDataInst* RTGPUArrVerteces;
		TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		UINT tmpkey = 0;
		BoundAIQuad * bound = 0;
		AIQuad * aq = 0;
		float3 center;
		float radius;

		for (int i = 0, il = ArrBound.size(); i < il; ++i)
		{
			bound = ArrBound[i];
			center = (bound->min + bound->max) * 0.5f;
			radius = SMVector3Length(center - bound->max);
			if (frustum->SphereInFrustum(&center, radius))
			{
				for (int k = 0, kl = bound->ArrIdsQuads.size(); k < kl; ++k)
				{
					aq = ArrQuads[bound->ArrIdsQuads[k]];
					if (SMVector3Distance(aq->pos, (*viewpos)) > dist)
						continue;

					RTGPUArrVerteces[tmpkey].pos = aq->pos;

					if (QuadSelectedExists(aq->Id))
						RTGPUArrVerteces[tmpkey].color = AIGRID_COLOR_SEL_MAIN;
					else if (QuadSelectedExistsNeightborDirect(aq->Id))
						RTGPUArrVerteces[tmpkey].color = AIGRID_COLOR_SEL_ADD;
					else if (CountSplits > 0 && IsMarkSplits)
						RTGPUArrVerteces[tmpkey].color = (ArrSplitsColor[aq->IdSplit - 1]);
					else
						RTGPUArrVerteces[tmpkey].color = ArrColor[aq->Id];

					if (aq->axisx == AIGRID_QUAD_DIR_POSITIVE && aq->axisy == AIGRID_QUAD_DIR_NEGATIVE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0, 0);
					else if (aq->axisx == AIGRID_QUAD_DIR_ALL && aq->axisy == AIGRID_QUAD_DIR_NEGATIVE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.25f, 0);
					else if (aq->axisx == AIGRID_QUAD_DIR_NEGATIVE && aq->axisy == AIGRID_QUAD_DIR_NEGATIVE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.5f, 0);

					else if (aq->axisx == AIGRID_QUAD_DIR_POSITIVE && aq->axisy == AIGRID_QUAD_DIR_ALL)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0, 0.25f);
					else if (aq->axisx == AIGRID_QUAD_DIR_ALL && aq->axisy == AIGRID_QUAD_DIR_ALL)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.25f, 0.25f);
					else if (aq->axisx == AIGRID_QUAD_DIR_NEGATIVE && aq->axisy == AIGRID_QUAD_DIR_ALL)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.5f, 0.25f);

					else if (aq->axisx == AIGRID_QUAD_DIR_POSITIVE && aq->axisy == AIGRID_QUAD_DIR_POSITIVE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0, 0.5f);
					else if (aq->axisx == AIGRID_QUAD_DIR_ALL && aq->axisy == AIGRID_QUAD_DIR_POSITIVE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.25f, 0.5f);
					else if (aq->axisx == AIGRID_QUAD_DIR_NEGATIVE && aq->axisy == AIGRID_QUAD_DIR_POSITIVE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.5f, 0.5f);

					else if (aq->axisx == AIGRID_QUAD_DIR_NEGATIVE && aq->axisy == AIGRID_QUAD_DIR_NONE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.25f, 0.75f);
					else if (aq->axisx == AIGRID_QUAD_DIR_POSITIVE && aq->axisy == AIGRID_QUAD_DIR_NONE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.5f, 0.75f);

					else if (aq->axisx == AIGRID_QUAD_DIR_NONE && aq->axisy == AIGRID_QUAD_DIR_NEGATIVE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.75f, 0.75f);
					else if (aq->axisx == AIGRID_QUAD_DIR_NONE && aq->axisy == AIGRID_QUAD_DIR_POSITIVE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.75f, 0.5f);

					else if (aq->axisx == AIGRID_QUAD_DIR_ALL && aq->axisy == AIGRID_QUAD_DIR_NONE)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.75f, 0.25f);
					else if (aq->axisx == AIGRID_QUAD_DIR_NONE && aq->axisy == AIGRID_QUAD_DIR_ALL)
						RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.75f, 0);

					else
					{
						RTGPUArrVerteces[tmpkey].tex = float4_t(1, 1, 0, 0);
						RTGPUArrVerteces[tmpkey].color = D3DCOLOR_ARGB(255, 255, 0, 0);
					}

					++tmpkey;
				}
			}
		}

		TransVertBuf->Unlock();

		DXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | tmpkey));

		DXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		DXDevice->SetStreamSource(1, TransVertBuf, 0, sizeof(AIQuadVertexDataInst));

		DXDevice->SetStreamSource(0, VertexQuad, 0, sizeof(AIQuadVertexData));
		DXDevice->SetIndices(IndexQuad);
		DXDevice->SetVertexDeclaration(AIQuadDecl);

		DXDevice->SetTexture(0, SGCore_LoadTexGetTex(Tex_AIGrid));

		SGCore_ShaderBind(ShaderType::st_vertex, IDVS);
		SGCore_ShaderBind(ShaderType::st_pixel, IDPS);

		D3DXMATRIX tmpview, tmpproj;
		DXDevice->GetTransform(D3DTS_VIEW, &tmpview);
		DXDevice->GetTransform(D3DTS_PROJECTION, &tmpproj);

		float4x4 wvp = SMMatrixIdentity() * float4x4(tmpview) * float4x4(tmpproj);
		wvp = SMMatrixTranspose(wvp);

		SGCore_ShaderSetVRF(ShaderType::st_vertex, IDVS, "WorldViewProjection", &wvp);

		DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		SGCore_ShaderUnBind();

		DXDevice->SetStreamSourceFreq(0, 1);
		DXDevice->SetStreamSourceFreq(1, 1);
	}
}

void AIGrid::RenderGraphPoints(const float3 * viewpos, float dist)
{
	if (!UseGraphics)
		return;

	if (TransVertBuf)
	{
		AIQuadVertexDataInst* RTGPUArrVerteces;
		TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		UINT tmpkey = 0;
		AIQuad* aq = 0;
		for (UINT i = 0; i<ArrGraphPointsIDs.size(); ++i)
		{
			aq = ArrQuads[ArrGraphPointsIDs[i]];
			if (!(aq->IsClose) && SMVector3Distance(aq->pos, (*viewpos)) <= dist)
			{
				RTGPUArrVerteces[tmpkey].pos = aq->pos;
				//RTGPUArrVerteces[tmpkey].pos.y += 0.1f;

				if (aq->axisx == 1 && aq->axisy == -1)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0, 0);
				else if (aq->axisx == 0 && aq->axisy == -1)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.25f, 0);
				else if (aq->axisx == -1 && aq->axisy == -1)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.5f, 0);

				else if (aq->axisx == 1 && aq->axisy == 0)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0, 0.25f);
				else if (aq->axisx == 0 && aq->axisy == 0)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.25f, 0.25f);
				else if (aq->axisx == -1 && aq->axisy == 0)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.5f, 0.25f);

				else if (aq->axisx == 1 && aq->axisy == 1)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0, 0.5f);
				else if (aq->axisx == 0 && aq->axisy == 1)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.25f, 0.5f);
				else if (aq->axisx == -1 && aq->axisy == 1)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.5f, 0.5f);

				else if (aq->axisx == -1 && aq->axisy == 2)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.25f, 0.75f);
				else if (aq->axisx == 1 && aq->axisy == 2)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.5f, 0.75f);

				else if (aq->axisx == 2 && aq->axisy == -1)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.75f, 0.75f);
				else if (aq->axisx == 2 && aq->axisy == 1)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.75f, 0.5f);

				else if (aq->axisx == 0 && aq->axisy == 2)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.75f, 0.25f);
				else if (aq->axisx == 2 && aq->axisy == 0)
					RTGPUArrVerteces[tmpkey].tex = float4_t(0.25f, 0.25f, 0.75f, 0);

				else
					RTGPUArrVerteces[tmpkey].tex = float4_t(1, 1, 0, 0);

				RTGPUArrVerteces[tmpkey].color = D3DCOLOR_ARGB(200, 0, 0, 255);
				++tmpkey;
			}
		}

		TransVertBuf->Unlock();

		DXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | tmpkey));

		DXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		DXDevice->SetStreamSource(1, TransVertBuf, 0, sizeof(AIQuadVertexDataInst));

		DXDevice->SetStreamSource(0, VertexQuad, 0, sizeof(AIQuadVertexData));
		DXDevice->SetIndices(IndexQuad);
		DXDevice->SetVertexDeclaration(AIQuadDecl);

		DXDevice->SetTexture(0, SGCore_LoadTexGetTex(Tex_AIGrid));

		SGCore_ShaderBind(ShaderType::st_vertex, IDVS);
		SGCore_ShaderBind(ShaderType::st_pixel, IDPS);

		D3DXMATRIX tmpview, tmpproj;
		DXDevice->GetTransform(D3DTS_VIEW, &tmpview);
		DXDevice->GetTransform(D3DTS_PROJECTION, &tmpproj);

		float4x4 wvp = SMMatrixIdentity() * float4x4(tmpview) * float4x4(tmpproj);
		wvp = SMMatrixTranspose(wvp);

		SGCore_ShaderSetVRF(ShaderType::st_vertex, IDVS, "WorldViewProjection", &wvp);

		DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		SGCore_ShaderUnBind();

		DXDevice->SetStreamSourceFreq(0, 1);
		DXDevice->SetStreamSourceFreq(1, 1);
	}
}

//#############################################################################

inline bool AIGrid::IsOpen(ID id)
{
	for (int i = 0; i<ArrOpenIDs.size(); ++i)
	{
		if (ArrOpenIDs[i] == id)
			return true;
	}
	return false;
}

inline int AIGrid::CalcHCost(ID id, ID endid)
{
	return (
		(
		abs(
		(ArrLongCoordQuads[id].x) - (ArrLongCoordQuads[endid].x)
		) / LAIGRID_QUAD_SIZE
		)

		) +

		(
		(
		abs(
		(ArrLongCoordQuads[id].z) - (ArrLongCoordQuads[endid].z)
		) / LAIGRID_QUAD_SIZE
		)
		*10
		);
}

inline int AIGrid::CalcH2Cost(ID id1, ID id2)
{
	/*long tmpx = abs((ArrIntCoord[id].x) - (ArrIntCoord[endid].x));
	long tmpz = abs((ArrIntCoord[id].z) - (ArrIntCoord[endid].z));
	long tmpxz = tmpx + tmpz;
	long tmpxzm = float(abs(tmpx - tmpz)) * 0.4f;
	return tmpxz - tmpxzm;*/
	return SMVector2Length(float2(ArrQuads[id1]->pos.x, ArrQuads[id1]->pos.z) - float2(ArrQuads[id2]->pos.x, ArrQuads[id2]->pos.z)) * 100.f;
}

inline ID AIGrid::AddInOpenList(ID id)
{
	for (int i = NumLastKeyOpenList; i<ArrOpenIDs.size(); i++)
	{
		if (ArrOpenIDs[i] == -1)
		{
			ArrOpenIDs[i] = id;
			NumLastKeyOpenList = i;	//говорим первый -1 ключ
			if (CountInitOpen2 < i + 1)
			{
				CountInitOpen2 = i + 1;
			}
			return i;
		}
	}
	return -1;
}

bool AIGrid::QuadIsFree(ID id, int radius)
{
	AIGRID_QUAD_PRECOND(id, false);

	if (ArrState[id] == AIQUAD_STATE::AIQUAD_STATE_FREE)
	{
		if (radius == 1)
			return true;
		else if (radius > 1)
		{
			memset(&(ArrPreCondFreeState[0]), 0, ArrPreCondFreeState.size() * sizeof(bool));
			ArrPreCondFreeState[id] = true;
			return IsFreeQuad2(id, radius - 1);
		}
	}
	return false;
}

bool AIGrid::IsFreeQuad2(ID id, int radius)
{
	AIGRID_QUAD_PRECOND(id, false);
	bool tmpisfree = true;
	
	AIQuad* aq = ArrQuads[id];
	
	if (radius > 0)
	{
		for (int i = 0; i<8; ++i)
		{
			if (aq->Arr[i] >= 0 && ArrState[aq->Arr[i]] == AIQUAD_STATE::AIQUAD_STATE_FREE)
			{
				if (!ArrPreCondFreeState[aq->Arr[i]])
				{
					ArrPreCondFreeState[aq->Arr[i]] = true;
					if (!IsFreeQuad2(aq->Arr[i], radius - 1))
					{
						tmpisfree = false;
						break;
					}
				}
			}
			else
			{
				tmpisfree = false;
				break;
			}
		}
	}
	return tmpisfree;
}

ID AIGrid::QuadGetNear(const float3* pos, bool isfree, int raius)
{
	ID howf = -1;

	ID tmpsmall = -1;
	float tmpsmalldist = 0;
	float tmpsmalldist2 = 0;
	float tmpsmallqdist = 0;
	float tmpsmallqdist2 = 0;

	ID tmparrbb[8];
	float tmparrdist2[8];

	for (int i = 0; i<8; ++i)
	{
		tmparrbb[i] = -1;
		tmparrdist2[i] = -1;
	}

	bool isexistsfree = false;
	BoundAIQuad* bound = 0;
	for (int i = 1, il = ArrBound.size(); i<il; ++i)
	{
		if (ArrBound[i]->ArrIdsQuads.size() > 0)
		{
			bound = ArrBound[i];

			isexistsfree = false;
			if (!isfree)
				isexistsfree = true;
			else
			{
				for (int k = 0, kl = bound->ArrIdsQuads.size(); k<kl; ++k)
				{
					if (QuadIsFree(bound->ArrIdsQuads[k], raius))
					{
						isexistsfree = true;
						break;
					}
				}
			}

			if (isexistsfree)
			{
				tmpsmalldist2 = SMVector3Length2(((bound->max + bound->min) * 0.5f) - (*pos));
				tmpsmall = -1;
				for (int k = 0; k<8; ++k)
				{
					if (tmparrbb[k] == -1)
					{
						tmparrbb[k] = i;
						tmparrdist2[k] = tmpsmalldist2;
						break;
					}

					if (tmpsmall == -1 || (tmpsmalldist2 < tmparrdist2[k] && tmpsmalldist > tmparrdist2[k] - tmpsmalldist2))
					{
						tmpsmalldist = tmparrdist2[k] - tmpsmalldist2;
						tmpsmall = k;
					}
				}

				if (tmpsmall != -1)
				{
					tmparrbb[tmpsmall] = i;
					tmparrdist2[tmpsmall] = tmpsmalldist2;
				}
			}
		}
	}

	for (int i = 0; i<8; ++i)
	{
		if (tmparrbb[i] != -1)
		{
			bound = ArrBound[tmparrbb[i]];
			for (int k = 0, kl = bound->ArrIdsQuads.size(); k<kl; ++k)
			{
				isexistsfree = true;
				tmpsmallqdist2 = SMVector3Length2(ArrQuads[bound->ArrIdsQuads[k]]->pos - (*pos));
				if ((howf == -1 || tmpsmallqdist2 < tmpsmallqdist) && (ArrState[bound->ArrIdsQuads[k]] == 0 || !isfree))
				{
					if (!isfree)
					{
						tmpsmallqdist = tmpsmallqdist2;
						howf = bound->ArrIdsQuads[k];
					}
					else if (QuadIsFree(bound->ArrIdsQuads[k], raius))
					{
						tmpsmallqdist = tmpsmallqdist2;
						howf = bound->ArrIdsQuads[k];
					}
				}
			}
		}
	}

	return howf;
}

ID AIGrid::GraphPointGetNear(ID beginq, ID endq)
{
	if (ArrQuads.size() <= 1)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: AI grid not found\n");
		return -1;
	}

	AIGRID_QUAD_PRECOND(beginq, false);
	AIGRID_QUAD_PRECOND(endq, false);

	if (CountSplits == 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: can not find path, because AI grid is not validate splits\n");
		return -1;
	}

	if (ArrQuads[beginq]->IdSplit != ArrQuads[endq]->IdSplit)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: quads in different splits, path not found\n");
		return -1;
	}

	if (beginq == endq)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: beginq == endq, path not found\n");
		return -1;
	}

	if (ArrGraphPointsIDs.size() == 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: graph points not found\n");
		return -1;
	}
	
	if (CalcH2Cost(beginq, endq) > AIGRID_COST_GPATH)
	{
		if (ArrCostGPIDs.size() != ArrGraphPointsIDs.size())
			ArrCostGPIDs.resize(ArrGraphPointsIDs.size());

		memset(&(ArrCostGPIDs[0]), -1, ArrCostGPIDs.size() * sizeof(int32_t));

		long tmpcountstepbegin = 0;	//на сколько мы продвинулись от начальной точки до текущей точки графа
		long tmpcountdel = 0;		//на сколько короче стал путь
		DWORD tmpbigeff = -1;		//самая эффективная ближайшая точка
		long tmpcostbe = -1;
		long tmpcostagpe = -1;
		long tmpdist = AIGRID_COST_GPATH;

		while (tmpbigeff == -1)
		{
			for (int i = 0, il = ArrGraphPointsIDs.size(); i<il; ++i)
			{
				if (ArrGraphPointsIDs[i] != beginq && ArrGraphPointsIDs[i] != endq)
				{
					tmpcostbe = CalcH2Cost(beginq, endq);
					tmpcostagpe = CalcH2Cost(ArrGraphPointsIDs[i], endq);
					tmpcountdel = tmpcostbe - tmpcostagpe;
					if (tmpcountdel > 0 && tmpcountdel < tmpdist)
					{
						tmpcountstepbegin = CalcH2Cost(beginq, ArrGraphPointsIDs[i]);
						ArrCostGPIDs[i] = (tmpcountstepbegin - tmpcountdel);
						if (tmpbigeff == -1 || (ArrCostGPIDs[tmpbigeff] > ArrCostGPIDs[i]))
							tmpbigeff = i;
					}
				}
			}
			tmpdist *= 2;
		}

		if (tmpbigeff != -1 && CalcH2Cost(beginq, ArrGraphPointsIDs[tmpbigeff]) < CalcH2Cost(beginq, endq))
		{
			//ArrColor[ArrGraphPointsIDs[tmpbigeff]] = 255;
			return ArrGraphPointsIDs[tmpbigeff];
		}
		return -1;
	}
	return -1;
}

bool AIGrid::GridFindPath(ID beginq, ID endq)
{
	if (ArrQuads.size() <= 1)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: AI grid not found\n");
		return false;
	}

	AIGRID_QUAD_PRECOND(beginq, false);
	AIGRID_QUAD_PRECOND(endq, false);

	if (CountSplits == 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: can not find path, because AI grid is not validate splits\n");
		return false;
	}

	if (ArrQuads[beginq]->IdSplit != ArrQuads[endq]->IdSplit)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: quads in different splits, path not found\n");
		return false;
	}

	if (beginq == endq)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "sxaigrid: beginq == endq, path not found\n");
		return false;
	}

	int tmpcost = AIGRID_COST_DIAGONAL;
	int cycnum = 0;
	long tmpaddcost = 0;
	long tmphcost = 0;
	//обнуляем все данные

	CountInitOpen2 = 0;
	NumLastKeyOpenList = 0;
	memset(&(ArrColor[0]), 0, ArrColor.size() * sizeof(uint32_t));
	memset(&(ArrIDsInOpen[0]), -1, ArrIDsInOpen.size() * sizeof(ID));
	memset(&(ArrOpenIDs[0]), -1, ArrOpenIDs.size() * sizeof(ID));
	memset(&(ArrParentIDs[0]), -1, ArrParentIDs.size() * sizeof(ID));
	memset(&(ArrCostQuads[0]), 0, ArrCostQuads.size() * sizeof(CostAIQuad));
	memset(&(ArrCloseIDs[0]), 0, ArrCloseIDs.size() * sizeof(bool));
	ArrPathIDs.clear();

	bool IsFindPath = false;

	//анализируем стоимости для всех соседних квадов и помещаем в открытый список
	//и назначаем им родителей стартовую точку

	for (cycnum = 0; cycnum<8; cycnum++)
	{
		if (
			ArrQuads[beginq]->Arr[cycnum] != -1)
		{
			if (ArrQuads[beginq]->Arr[cycnum] == endq)
			{
				ArrPathIDs.push_back(endq);
				ArrPathIDs.push_back(ArrQuads[beginq]->Arr[cycnum]);
				return true;
			}
			tmpcost = AIGRID_COST_DIAGONAL;

			if (cycnum == 1 || cycnum == 3 || cycnum == 4 || cycnum == 6)
				tmpcost = AIGRID_COST_DIRECT;

			if (QuadGetState(ArrQuads[beginq]->Arr[cycnum]) != AIQUAD_STATE::AIQUAD_STATE_FREE)
			{
				if (QuadGetState(ArrQuads[beginq]->Arr[cycnum]) == AIQUAD_STATE::AIQUAD_STATE_TEMPBUSY)
					tmpcost += AIGRID_COSTADD_TEMPBUSY;
				else if (QuadGetState(ArrQuads[beginq]->Arr[cycnum]) == AIQUAD_STATE::AIQUAD_STATE_BUSY)
					tmpcost += AIGRID_COSTADD_BUSY;
			}
			else
				tmpaddcost = 0;

			ArrParentIDs[ArrQuads[beginq]->Arr[cycnum]] = beginq;
			ArrCostQuads[ArrQuads[beginq]->Arr[cycnum]].G = tmpcost;
			ArrCostQuads[ArrQuads[beginq]->Arr[cycnum]].H = CalcHCost(ArrQuads[beginq]->Arr[cycnum], endq);

			ArrCostQuads[ArrQuads[beginq]->Arr[cycnum]].F = ArrCostQuads[ArrQuads[beginq]->Arr[cycnum]].G + ArrCostQuads[ArrQuads[beginq]->Arr[cycnum]].H + tmpaddcost;

			ArrIDsInOpen[ArrQuads[beginq]->Arr[cycnum]] = AddInOpenList(ArrQuads[beginq]->Arr[cycnum]);
		}
	}

	//стартовую точку помещаем в закрытый
	ArrCloseIDs[beginq] = true;

	bool isfind = false;
	ID tmpsmallF = -1;
	bool issmall = true;

	DWORD ttime = GetTickCount();
	ID tmpcurrsmall = -1;
	ID tmpcurridquad = -1;

	AIQuad* tmpquad = 0;

	while (!IsFindPath)
	{
		isfind = false;
		tmpsmallF = -1;

		//находим квад с наименьшей стоимостью F

		tmpcurrsmall = -1;
		for (cycnum = 0; cycnum<CountInitOpen2; ++cycnum)
		{
			if (ArrOpenIDs[cycnum] != -1)
			{
				if (tmpcurrsmall != -1)
				{
					if (ArrCostQuads[ArrOpenIDs[tmpcurrsmall]].F > ArrCostQuads[ArrOpenIDs[cycnum]].F)
						tmpcurrsmall = cycnum;
				}
				else
					tmpcurrsmall = cycnum;
			}
		}

		if (tmpcurrsmall != -1)
			tmpsmallF = ArrOpenIDs[tmpcurrsmall];

		//анализируем соседей квада на наличие их в открытом списке
		if (tmpsmallF != -1)
		{
			tmpquad = ArrQuads[tmpsmallF];
			
			for (cycnum = 0; cycnum<8; ++cycnum)
			{
				tmpcurridquad = tmpquad->Arr[cycnum];
				//если квад существует
				if (tmpcurridquad != -1)
				{
					if (!ArrCloseIDs[tmpcurridquad])
					{
						//если сосденяя точка конечная точка то мы нашли путь!!!
						if (tmpcurridquad == endq)
						{
							isfind = true;
							break;
						}
					
						tmpcost = AIGRID_COST_DIAGONAL;
						if (cycnum == 1 || cycnum == 3 || cycnum == 4 || cycnum == 6)
							tmpcost = AIGRID_COST_DIRECT;

						tmphcost = CalcHCost(tmpcurridquad, endq);

						if (QuadGetState(tmpcurridquad) != AIQUAD_STATE::AIQUAD_STATE_FREE && tmphcost / 10 < AIGRID_QUADSCOUNT_BUSY)
						{
							if (QuadGetState(tmpcurridquad) == AIQUAD_STATE::AIQUAD_STATE_TEMPBUSY)
								tmpcost += AIGRID_COSTADD_TEMPBUSY;
							else if (QuadGetState(tmpcurridquad) == AIQUAD_STATE::AIQUAD_STATE_BUSY)
								tmpcost += AIGRID_COSTADD_BUSY;
						}
						else
							tmpaddcost = 0;

						//если есть родитель
						if (ArrParentIDs[tmpcurridquad] != -1)
						{
							//считаем а не дешевле ли перейти на соседний квад через этот квад
							if (ArrCostQuads[tmpcurridquad].G > ArrCostQuads[tmpsmallF].G + tmpcost)
							{
								ArrParentIDs[tmpcurridquad] = tmpsmallF;
								ArrCostQuads[tmpcurridquad].G = ArrCostQuads[tmpsmallF].G + tmpcost;
								ArrCostQuads[tmpcurridquad].F = ArrCostQuads[tmpcurridquad].G + ArrCostQuads[tmpcurridquad].H;
							}
						}
						//если нет родителя то назначаем текущий квад родителем и считаем стоимости
						//и если не в закрытом списке
						else
						{
							ArrParentIDs[tmpcurridquad] = tmpsmallF;
							ArrCostQuads[tmpcurridquad].G = ArrCostQuads[tmpsmallF].G + tmpcost;
							ArrCostQuads[tmpcurridquad].H = tmphcost;// CalcHCost(tmpcurridquad, endq);
							ArrCostQuads[tmpcurridquad].F = ArrCostQuads[tmpcurridquad].G + ArrCostQuads[tmpcurridquad].H;
							ArrIDsInOpen[tmpcurridquad] = AddInOpenList(tmpcurridquad);
						}
					}
				}
			}

			ArrOpenIDs[ArrIDsInOpen[tmpsmallF]] = -1;
			if (NumLastKeyOpenList > ArrIDsInOpen[tmpsmallF])
				NumLastKeyOpenList = ArrIDsInOpen[tmpsmallF];
			ArrIDsInOpen[tmpsmallF] = -1;

			ArrCloseIDs[tmpsmallF] = true;

			if (isfind)
			{
				IsFindPath = true;
				ArrPathIDs.push_back(endq);
				ArrPathIDs.push_back(tmpsmallF);
				ID tmpquad = tmpsmallF;
				ID tmpparent = -1;
				/*ArrColor[tmpsmallF] = D3DCOLOR_ARGB(200,0,255,0);
				ArrColor[beginq] = D3DCOLOR_ARGB(255, 0, 255, 0);
				ArrColor[endq] = D3DCOLOR_ARGB(255, 0, 255, 0);*/
				while ((tmpparent = ArrParentIDs[tmpquad]) != -1 && beginq != tmpparent)
				{
					ArrPathIDs.push_back(tmpparent);
					//ArrColor[tmpparent] = D3DCOLOR_ARGB(128, 0, 255, 0);
					tmpquad = tmpparent;
				}

			}
		}
		else
		{
			IsFindPath = false;
			break;
		}
	}
	ttime = GetTickCount() - ttime;
	return IsFindPath;
}

UINT AIGrid::GridGetSizePath()
{
	return ArrPathIDs.size();
}

bool AIGrid::GridGetPath(ID * pmem, UINT count)
{
	if (pmem)
	{
		memcpy(pmem, &(ArrPathIDs[0]), count * sizeof(ID));
		return true;
	}

	return false;
}

void AIGrid::GridSetColorArr(const ID * pmem, DWORD color, UINT count)
{
	if (!pmem)
		return;

	for (int i = 0; i < count; ++i)
	{
		if (pmem[i] >= 0 && pmem[i] < ArrColor.size())
			ArrColor[pmem[i]] = color;
	}
}

void AIGrid::GridSetNullColor()
{
	memset(&(ArrColor[0]), 0, sizeof(uint32_t)* ArrColor.size());
}