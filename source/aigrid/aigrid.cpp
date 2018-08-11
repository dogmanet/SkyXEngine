
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "aigrid.h"

CAIGrid::CAIGrid()
{
	m_pDecl = 0;
	m_pTransVertBuf = 0;
	m_uiCountSplits = 0;
	m_pBoundBox = 0;
	m_pDXDevice = 0;
	m_idTexBB = -1;
	m_isMarkSplits = false;
	m_idTex = -1;

	m_idVS = -1;
	m_idPS = -1;

	m_useGraphics = false;

	m_iLastWait = 0;

	m_aQueueFind.resize(1024);

	m_pTreeBound = 0;
		
	//bbCreate(&float3(0, 0, 0), &float3(10, 10, 10));
}

CAIGrid::~CAIGrid()
{
	clear();
	mem_release(m_pDecl);
	mem_release(m_pBoundBox);
	mem_release(m_pVertexQuad);
	mem_release(m_pIndexQuad);
}

//##########################################################################

void CAIGrid::save(const char* path)
{
	if (m_aQuads.size() <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "AI grid not found\n");
		return;
	}

	if (m_uiCountSplits <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "can not find path, because AI grid is not validate splits\n");
		return;
	}

	FILE* file = 0;
	file = fopen(path, "wb");

	fwrite("aigrid\0", 7, 1, file);
	fwrite(&m_uiCountSplits, sizeof(uint16_t), 1, file);
	fwrite(&float3_t((m_aBound[0]->m_f3Max.x + m_aBound[0]->m_f3Min.x) * 0.5f), sizeof(float3_t), 1, file);
	fwrite(&float3_t(m_aBound[0]->m_f3Max - m_aBound[0]->m_f3Min), sizeof(float3_t), 1, file);

	int32_t CountBB = m_aBound.size() - 1;
	fwrite(&CountBB, sizeof(int32_t), 1, file);
	int32_t tmpCountObjectBBQuads = 0;
	int tmpcountids = 0;
	for (int32_t i = 1; i<m_aBound.size(); i++)
	{
		fwrite(&(m_aBound[i]->m_f3Min), sizeof(float3_t), 1, file);
		fwrite(&(m_aBound[i]->m_f3Max), sizeof(float3_t), 1, file);

		tmpCountObjectBBQuads = m_aBound[i]->m_aIdsQuads.size();
		fwrite(&tmpCountObjectBBQuads, sizeof(int32_t), 1, file);
		tmpcountids += tmpCountObjectBBQuads;
		if (tmpCountObjectBBQuads > 0)
		{
			fwrite(&(m_aBound[i]->m_aIdsQuads[0]), sizeof(ID), tmpCountObjectBBQuads, file);
		}
	}

	int32_t CountObject = m_aQuads.size();
	fwrite(&CountObject, sizeof(int32_t), 1, file);
	//fwrite((m_aQuads[0]), sizeof(CAIquad), CountObject, file);

	for (int32_t i = 0; i<CountObject; ++i)
	{
		CAIquad* tmpaq = m_aQuads[i];
		fwrite(tmpaq, sizeof(CAIquad), 1, file);
	}

	CountObject = m_aGraphPointsIDs.size();
	fwrite(&CountObject, sizeof(int32_t), 1, file);
	if (CountObject > 0)
		fwrite(&(m_aGraphPointsIDs[0]), sizeof(CAIquad), CountObject, file);

	fclose(file);
}

void CAIGrid::load(const char* path)
{
	clear();
	float3_t tmpcenter, tmpparam;

	FILE* file = 0;
	file = fopen(path, "rb");

	char HeaderFile[7];

	fread(HeaderFile, 7, 1, file);

	if (strcmp(HeaderFile, "aigrid") == 0)
	{
		fread(&m_uiCountSplits, sizeof(uint16_t), 1, file);
		fread(&tmpcenter, sizeof(float3_t), 1, file);
		fread(&tmpparam, sizeof(float3_t), 1, file);

		bbCreate(&float3(tmpcenter), &float3(tmpparam));
		m_vMax = m_aBound[0]->m_f3Max;
		m_vMin = m_aBound[0]->m_f3Min;
		int32_t CountObjectBB = 0;
		fread(&CountObjectBB, sizeof(int32_t), 1, file);

		float3_t tmpmin, tmpmax;
		int tmpcountids = 0;

		for (int32_t i = 0; i<CountObjectBB; ++i)
		{
			CBoundAIquad* tmpbb = m_oAllocBound.Alloc();
			fread(&(tmpbb->m_f3Min), sizeof(float3_t), 1, file);
			fread(&(tmpbb->m_f3Max), sizeof(float3_t), 1, file);

			tmpbb->m_i3Min.x = AIGRID_TOINT(tmpbb->m_f3Min.x);
			tmpbb->m_i3Min.y = AIGRID_TOINT(tmpbb->m_f3Min.y);
			tmpbb->m_i3Min.z = AIGRID_TOINT(tmpbb->m_f3Min.z);

			tmpbb->m_i3Max.x = AIGRID_TOINT(tmpbb->m_f3Max.x);
			tmpbb->m_i3Max.y = AIGRID_TOINT(tmpbb->m_f3Max.y);
			tmpbb->m_i3Max.z = AIGRID_TOINT(tmpbb->m_f3Max.z);

			int32_t tmpCountObjectBBQuads = 0;
			fread(&tmpCountObjectBBQuads, sizeof(int32_t), 1, file);
			tmpcountids += tmpCountObjectBBQuads;
			if (tmpCountObjectBBQuads > 0)
			{
				tmpbb->m_aIdsQuads.resize(tmpCountObjectBBQuads);
				fread(&(tmpbb->m_aIdsQuads[0]), sizeof(ID), tmpCountObjectBBQuads, file);
			}

			m_aBound.push_back(tmpbb);
		}

		int32_t CountObject = 0;
		fread(&CountObject, sizeof(int32_t), 1, file);
		m_aQuads.reserve(CountObject);
		for (int32_t i = 0; i<CountObject; ++i)
		{
			CAIquad* tmpaq = m_oAllocQuad.Alloc();
			fread(tmpaq, sizeof(CAIquad), 1, file);
			m_aQuads.push_back(tmpaq);
			m_aLongCoordQuads[tmpaq->m_id] = int3(AIGRID_TOINT(tmpaq->m_vPos.x), AIGRID_TOINT(tmpaq->m_vPos.y), AIGRID_TOINT(tmpaq->m_vPos.z));
		}

		CountObject = 0;
		fread(&CountObject, sizeof(int32_t), 1, file);
		if (CountObject > 0)
		{
			m_aGraphPointsIDs.resize(CountObject);
			fread(&(m_aGraphPointsIDs[0]), sizeof(ID), CountObject, file);
		}
	}

	fclose(file);
	reCreateBuffs();

	LibReport(REPORT_MSG_LEVEL_NOTICE, "AI grid is loaded, count quads %d, count graph points %d\n", m_aQuads.size(), m_aGraphPointsIDs.size());

	if (m_uiCountSplits <= 0)
		gridTestValidation();

	createTreeBound();
}

//##########################################################################

void CAIGrid::createTreeBound()
{
	mem_delete(m_pTreeBound);
	if (m_aBound.size() < 1)
		return;

	m_pTreeBound = new CTreeBoundAIquad();
	m_pTreeBound->m_pBoundBox = SGCore_CrBound();
	m_pTreeBound->m_pBoundBox->setMinMax(&float3(m_aBound[0]->m_f3Min), &float3(m_aBound[0]->m_f3Max));
	m_pTreeBound->m_i3Min = m_aBound[0]->m_i3Min;
	m_pTreeBound->m_i3Max = m_aBound[0]->m_i3Max;
	
	for (int i = 1, il = m_aBound.size(); i < il; ++i)
	{
		m_pTreeBound->m_pBounds.push_back(m_aBound[i]);
}

	Array<CTreeBoundAIquad*> queue;
	int iCountTree = 0;
	queue.push_back(m_pTreeBound);

	while (queue.size())
{
		CTreeBoundAIquad *pSplit = queue[0];

		float3 vRootMin, vRootMax;
		pSplit->m_pBoundBox->getMinMax(&vRootMin, &vRootMax);

		for (int i = 0; i<4; i++)
		{
			pSplit->m_aSplits[i] = new CTreeBoundAIquad();
		}

		ISXBound* aBounds[4];
		for (int i = 0; i < 4; ++i)
		{
			aBounds[i] = SGCore_CrBound();
		}

		float3 vMin, vMax, vNewMin, vNewMax;
		SGCore_0ComBoundBoxArr4(pSplit->m_pBoundBox, aBounds);

		bool isExists = false;
		
		for (int i = 0; i<4; i++)
		{
			pSplit->m_aSplits[i]->m_pBoundBox = aBounds[i];

			pSplit->m_aSplits[i]->m_pBoundBox->getMinMax(&vMin, &vMax);
			vNewMin = vMin;
			vNewMax = vMax;
			pSplit->m_aSplits[i]->m_i3Min = int3(AIGRID_TOINT(vMin.x), AIGRID_TOINT(vMin.y), AIGRID_TOINT(vMin.z));
			pSplit->m_aSplits[i]->m_i3Max = int3(AIGRID_TOINT(vMax.x), AIGRID_TOINT(vMax.y), AIGRID_TOINT(vMax.z));

			for (int k = 0, kl = pSplit->m_pBounds.size(); k < kl; k++)
			{
				CBoundAIquad *pBoundQuads = pSplit->m_pBounds[k];
				//проверка вхождения бокса в бокс
				if (
					(pBoundQuads->m_f3Min.x >= vMin.x && pBoundQuads->m_f3Min.z >= vMin.z && pBoundQuads->m_f3Min.x <= vMax.x && pBoundQuads->m_f3Min.z <= vMax.z) ||
					(pBoundQuads->m_f3Max.x <= vMax.x && pBoundQuads->m_f3Max.z <= vMax.z && pBoundQuads->m_f3Max.x >= vMin.x && pBoundQuads->m_f3Max.z >= vMin.z) ||
					(pBoundQuads->m_f3Min.x >= vMin.x && pBoundQuads->m_f3Max.z <= vMax.z && pBoundQuads->m_f3Min.x <= vMax.x && pBoundQuads->m_f3Max.z >= vMin.z) ||
					(pBoundQuads->m_f3Max.x <= vMax.x && pBoundQuads->m_f3Min.z >= vMin.z && pBoundQuads->m_f3Max.x >= vMin.x && pBoundQuads->m_f3Min.z <= vMax.z)
					)
				{
					pSplit->m_aSplits[i]->m_pBounds.push_back(pBoundQuads);

					if (vNewMin.x > pBoundQuads->m_f3Min.x)
						vNewMin.x = pBoundQuads->m_f3Min.x;

					if (vNewMin.z > pBoundQuads->m_f3Min.z)
						vNewMin.z = pBoundQuads->m_f3Min.z;

					if (vNewMax.x < pBoundQuads->m_f3Max.x)
						vNewMax.x = pBoundQuads->m_f3Max.x;

					if (vNewMax.z < pBoundQuads->m_f3Max.z)
						vNewMax.z = pBoundQuads->m_f3Max.z;
				}
			}

			pSplit->m_aSplits[i]->m_pBoundBox->setMinMax(&vNewMin, &vNewMax);
			pSplit->m_aSplits[i]->m_i3Min = int3(AIGRID_TOINT(vNewMin.x), AIGRID_TOINT(vNewMin.y), AIGRID_TOINT(vNewMin.z));
			pSplit->m_aSplits[i]->m_i3Max = int3(AIGRID_TOINT(vNewMax.x), AIGRID_TOINT(vNewMax.y), AIGRID_TOINT(vNewMax.z));

			if ((vNewMax.x - vNewMin.x) > AIGRID_SPLIT_MIN_SIZE && (vNewMax.z - vNewMin.z) > AIGRID_SPLIT_MIN_SIZE)
			{
				queue.push_back(pSplit->m_aSplits[i]);
				isExists = true;
			}
		}

		if (isExists)
			pSplit->m_pBounds.clear();

		queue.erase(0);
		++iCountTree;
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "tree created, count nodes %d\n", iCountTree);
}

UINT CAIGrid::gridGetCountQuads() const
{
	return m_aQuads.size();
}

void CAIGrid::correctPosXZ(float3* pos)
{
	if (!pos)
		return;

	float count = pos->x / AIGRID_QUAD_SIZE;
	pos->x = AIGRID_QUAD_SIZE * (float)((int)floor(count + 0.5));

	/*count = pos->y / AIGRID_QUAD_SIZE;
	pos->y = AIGRID_QUAD_SIZE * (float)((int)floor(count + 0.5));*/

	count = pos->z / AIGRID_QUAD_SIZE;
	pos->z = AIGRID_QUAD_SIZE * (float)((int)floor(count + 0.5));
}

bool CAIGrid::isValidIdInArrQuads(ID id)
{
	return (id >= 0 && m_aQuads.size() > id && !(m_aQuads[id]->m_isClose));
}

bool CAIGrid::gridCompareHeight(ID id1, ID id2)
{
	return (
		abs(
		abs(m_aLongCoordQuads[id1].y) - abs(m_aLongCoordQuads[id2].y)
		) > LAIGRID_QUADS_CENTERS_MAXHEIGHT ? false : true

		);
}

void CAIGrid::reCreateBuffs()
{
	//GridClear();
	mem_release_del(m_pTransVertBuf);
	if (m_aQuads.size() <= 0)
	{
		return;
	}
	
	if (m_useGraphics)
	{
		m_pDXDevice->CreateVertexBuffer(
			m_aQuads.size() * sizeof(CAIQuadVertexDataInst),
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&m_pTransVertBuf,
			0);
	}

	m_aQuadColors.resize(m_aQuads.size());
	m_aIDsInOpen.resize(m_aQuads.size());
	m_aCostQuads.resize(m_aQuads.size());
	m_aOpenIDs.resize(m_aQuads.size());
	m_aParentIDs.resize(m_aQuads.size());
	m_aTestSplitsIDs.resize(m_aQuads.size());
	m_aQuadState.resize(m_aQuads.size());
	m_aStateWho.resize(m_aQuads.size());
	m_aPreCondFreeState.resize(m_aQuads.size());
	memset(&(m_aQuadColors[0]), 0, m_aQuadColors.size() * sizeof(uint32_t));
	memset(&(m_aIDsInOpen[0]), -1, m_aIDsInOpen.size() * sizeof(ID));
	memset(&(m_aCostQuads[0]), -1, m_aCostQuads.size() * sizeof(CCostTransit));
	memset(&(m_aOpenIDs[0]), -1, m_aOpenIDs.size() * sizeof(ID));
	memset(&(m_aParentIDs[0]), -1, m_aParentIDs.size() * sizeof(ID));
	memset(&(m_aTestSplitsIDs[0]), 0, m_aTestSplitsIDs.size() * sizeof(bool));
	memset(&(m_aQuadState[0]), (int)AIQUAD_STATE_FREE, m_aQuadState.size() * sizeof(AIQUAD_STATE));
	memset(&(m_aStateWho[0]), -1, m_aStateWho.size() * sizeof(ID));
	memset(&(m_aPreCondFreeState[0]), false, m_aPreCondFreeState.size() * sizeof(bool));
}

void CAIGrid::defInitBuffs(ID id)
{
	mem_release_del(m_pTransVertBuf);
	if (m_aQuads.size() <= 0)
	{
		return;
	}

	if (m_useGraphics)
	{
		m_pDXDevice->CreateVertexBuffer(
			m_aQuads.size() * sizeof(CAIQuadVertexDataInst),
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&m_pTransVertBuf,
			0);
	}

	m_aQuadColors[id] = 0;
	m_aIDsInOpen[id] = -1;
	m_aCostQuads[id] = CCostTransit();
	m_aOpenIDs[id] = -1;
	m_aParentIDs[id] = -1;
	m_aTestSplitsIDs[id] = 0;
	m_aQuadState[id] = AIQUAD_STATE_FREE;
	m_aStateWho[id] = -1;
	m_aPreCondFreeState[id] = true;
}

void CAIGrid::bbCreate(const float3* center, const float3* param)
{
	if (m_aBound.size() > 1)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "bound box already splitting, unresolved create");
		return;
	}

	if (!center || !param)
		return;

	CBoundAIquad* tmpbb;
	
	if(m_aBound.size() > 0)
		tmpbb = m_aBound[0];
	else
		tmpbb = m_oAllocBound.Alloc();

	tmpbb->m_f3Max = float3_t((*center) + (*param)*0.5f);
	tmpbb->m_f3Min = float3_t((*center) - (*param)*0.5f);

	tmpbb->m_i3Min.x = AIGRID_TOINT(tmpbb->m_f3Min.x);
	tmpbb->m_i3Min.y = AIGRID_TOINT(tmpbb->m_f3Min.y);
	tmpbb->m_i3Min.z = AIGRID_TOINT(tmpbb->m_f3Min.z);

	tmpbb->m_i3Max.x = AIGRID_TOINT(tmpbb->m_f3Max.x);
	tmpbb->m_i3Max.y = AIGRID_TOINT(tmpbb->m_f3Max.y);
	tmpbb->m_i3Max.z = AIGRID_TOINT(tmpbb->m_f3Max.z);

	mem_release(m_pBoundBox);

	if (m_useGraphics)
		SGCore_FCreateBoundingBoxMesh(&((float3)tmpbb->m_f3Min), &((float3)tmpbb->m_f3Max), &(m_pBoundBox));

	m_aBound[0] = tmpbb;
}

bool CAIGrid::bbIsCreated() const
{
	return (m_aBound.size() > 0);
}

void CAIGrid::bbSetDimensions(const float3* dim)
{
	if (m_aBound.size() <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "bound box is not created");
		return;
	}

	if (m_aBound.size() > 1)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "bound box already splitting, unresolved set dimensions");
		return;
	}

	if (!dim)
		return;

	float3 tmpdim = m_aBound[0]->m_f3Max - m_aBound[0]->m_f3Min;
	float3 tmpcx = (m_aBound[0]->m_f3Max + m_aBound[0]->m_f3Min) * 0.5f;
	m_aBound[0]->m_f3Min = (float3_t)(-(*dim) * 0.5f);
	m_aBound[0]->m_f3Max = (float3_t)((*dim) * 0.5f);
	m_aBound[0]->m_f3Min = (float3_t)(m_aBound[0]->m_f3Min + tmpcx);
	m_aBound[0]->m_f3Max = (float3_t)(m_aBound[0]->m_f3Max + tmpcx);

	float3 tmpdim2 = m_aBound[0]->m_f3Max - m_aBound[0]->m_f3Min;
	float3 tmpscale = tmpdim2 / tmpdim;

	if (m_useGraphics)
	{
		char* tmppos;
		m_pBoundBox->LockVertexBuffer(0, (void**)&tmppos);
		for (int i = 0; i < m_pBoundBox->GetNumVertices(); i++)
		{
			float3_t* tpos = (float3_t*)((char*)(tmppos)+m_pBoundBox->GetNumBytesPerVertex() * i);
			tpos->x *= tmpscale.x;
			tpos->y *= tmpscale.y;
			tpos->z *= tmpscale.z;
		}
		m_pBoundBox->UnlockVertexBuffer();
	}
}

void CAIGrid::bbGetDimensions(float3* dim) const
{
	if (m_aBound.size() <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "bound box is not created");
		return;
	}

	if (!dim)
		return;

	dim->x = m_aBound[0]->m_f3Max.x - m_aBound[0]->m_f3Min.x;
	dim->y = m_aBound[0]->m_f3Max.y - m_aBound[0]->m_f3Min.y;
	dim->z = m_aBound[0]->m_f3Max.z - m_aBound[0]->m_f3Min.z;
}

void CAIGrid::bbSetPos(const float3* pos)
{
	if (m_aBound.size() <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "bound box is not created");
		return;
	}

	if(m_aBound.size() > 1)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "bound box already splitting, unresolved set position");
		return;
	}

	if (!pos)
		return;

	float3 tmpcenter = (float3(m_aBound[0]->m_f3Max) + float3(m_aBound[0]->m_f3Min)) * 0.5f;
	m_aBound[0]->m_f3Min = (float3_t)(float3(m_aBound[0]->m_f3Min) + (-tmpcenter + (*pos)));
	m_aBound[0]->m_f3Max = (float3_t)(float3(m_aBound[0]->m_f3Max) + (-tmpcenter + (*pos)));
	
	if (m_useGraphics)
	{
		char* tmppos;
		m_pBoundBox->LockVertexBuffer(0, (void**)&tmppos);
		for (int i = 0; i < m_pBoundBox->GetNumVertices(); i++)
		{
			float3_t* tpos = (float3_t*)((char*)(tmppos)+m_pBoundBox->GetNumBytesPerVertex() * i);
			tpos->x += -tmpcenter.x + (pos->x);
			tpos->y += -tmpcenter.y + (pos->y);
			tpos->z += -tmpcenter.z + (pos->z);
		}
		m_pBoundBox->UnlockVertexBuffer();
	}
}

void CAIGrid::bbGetPos(float3* pos) const
{
	if (m_aBound.size() <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "bound box is not created");
		return;
	}

	if (!pos)
		return;

	*pos = (float3(m_aBound[0]->m_f3Max) + float3(m_aBound[0]->m_f3Min)) * 0.5f;
}

void CAIGrid::bbCreateFinish()
{
	if (m_aBound.size() == 1)
	{
		m_vMax = float3_t(m_aBound[0]->m_f3Max.x, m_aBound[0]->m_f3Max.y, m_aBound[0]->m_f3Max.z);
		m_vMin = float3_t(m_aBound[0]->m_f3Min.x, m_aBound[0]->m_f3Min.y, m_aBound[0]->m_f3Min.z);

		float tmpx = m_aBound[0]->m_f3Max.x - m_aBound[0]->m_f3Min.x;
		float tmpy = m_aBound[0]->m_f3Max.y - m_aBound[0]->m_f3Min.y;
		float tmpz = m_aBound[0]->m_f3Max.z - m_aBound[0]->m_f3Min.z;

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

		splitBB(tmpx, tmpy, tmpz);
		createTreeBound();
	}
}

bool CAIGrid::bbIsCreatedFinish() const
{
	return (m_aBound.size() > 1);
}

AIQUAD_STATE CAIGrid::quadGetState(ID id) const
{
	AIGRID_QUAD_PRECOND(id, AIQUAD_STATE::AIQUAD_STATE_FREE)
	return m_aQuadState[id];
}

void CAIGrid::quadSetState(ID id, AIQUAD_STATE state)
{
	AIGRID_QUAD_PRECOND(id, _VOID);
	m_aQuadState[id] = state;
}

void CAIGrid::quadSetStateWho(ID id, ID who)
{
	AIGRID_QUAD_PRECOND(id, _VOID);
	m_aStateWho[id] = who;
}

ID CAIGrid::quadGetStateWho(ID id) const
{
	AIGRID_QUAD_PRECOND(id, -1);
	return m_aStateWho[id];
}

bool CAIGrid::quadIs2Neighbors(ID id, ID idn1, ID idn2) const
{
	AIGRID_QUAD_PRECOND(id, false);
	AIGRID_QUAD_PRECOND(idn1, false);
	AIGRID_QUAD_PRECOND(idn2, false);

	CAIquad* aq = m_aQuads[id];
	int concurrence = 0;
	for (int i = 0; i < 8; ++i)
	{
		if (aq->m_aNeighbors[i] == idn1 || aq->m_aNeighbors[i] == idn2)
			++concurrence;
	}

	return (concurrence == 2);
}

void CAIGrid::quadSetPosY(ID id, float posy)
{
	AIGRID_QUAD_PRECOND(id, _VOID);
	m_aQuads[id]->m_vPos.y = posy;
}

float CAIGrid::quadGetPosY(ID id) const
{
	AIGRID_QUAD_PRECOND(id, 0);
	return m_aQuads[id]->m_vPos.y;
}

ID CAIGrid::quadAdd(const float3* pos)
{
	ID idquad = quadGet(pos,false);

	CAIquad* aq = 0;
	if (idquad >= 0)
	{
		if (!(m_aQuads[idquad]->m_isClose))
			return idquad;
		else
		{
			aq = m_aQuads[idquad];
			aq->m_isClose = false;
		}
	}
	else
		aq = m_oAllocQuad.Alloc();

	float3 tpos(*pos);
	correctPosXZ(&tpos);
	aq->m_vPos.x = tpos.x;
	aq->m_vPos.y = tpos.y;
	aq->m_vPos.z = tpos.z;

	aq->m_vPos.y += AIGRID_QUAD_ADDYPOS;
	m_aQuads.push_back(aq);
	idquad = m_aQuads.size() - 1;
	aq->m_id = idquad;
	defInitBuffs(idquad);

	ID idneighboor = quadGet(&float3(float3(aq->m_vPos) + float3(-AIGRID_QUAD_SIZE, 0, AIGRID_QUAD_SIZE)), false);
	if (isValidIdInArrQuads(idneighboor))
	{
		aq->m_aNeighbors[0] = idneighboor;
		m_aQuads[idneighboor]->m_aNeighbors[7] = idquad;
	}

	idneighboor = quadGet(&float3(float3(aq->m_vPos) + float3(0, 0, AIGRID_QUAD_SIZE)), false);
	if (isValidIdInArrQuads(idneighboor))
	{
		aq->m_aNeighbors[1] = idneighboor;
		m_aQuads[idneighboor]->m_aNeighbors[6] = idquad;

		if (isValidIdInArrQuads(m_aQuads[idneighboor]->m_aNeighbors[1]))
			m_aQuads[idneighboor]->m_chAxisY = AIGRID_QUAD_DIR_ALL;
		else
			m_aQuads[idneighboor]->m_chAxisY = AIGRID_QUAD_DIR_NEGATIVE;
	}

	idneighboor = quadGet(&float3(float3(aq->m_vPos) + float3(AIGRID_QUAD_SIZE, 0, AIGRID_QUAD_SIZE)), false);
	if (isValidIdInArrQuads(idneighboor))
	{
		aq->m_aNeighbors[2] = idneighboor;
		m_aQuads[idneighboor]->m_aNeighbors[5] = idquad;
	}

	idneighboor = quadGet(&float3(float3(aq->m_vPos) + float3(-AIGRID_QUAD_SIZE, 0, 0)), false);
	if (isValidIdInArrQuads(idneighboor))
	{
		aq->m_aNeighbors[3] = idneighboor;
		m_aQuads[idneighboor]->m_aNeighbors[4] = idquad;

		if (isValidIdInArrQuads(m_aQuads[idneighboor]->m_aNeighbors[3]))
			m_aQuads[idneighboor]->m_chAxisX = AIGRID_QUAD_DIR_ALL;
		else
			m_aQuads[idneighboor]->m_chAxisX = AIGRID_QUAD_DIR_POSITIVE;
	}

	idneighboor = quadGet(&float3(float3(aq->m_vPos) + float3(AIGRID_QUAD_SIZE, 0, 0)), false);
	if (isValidIdInArrQuads(idneighboor))
	{
		aq->m_aNeighbors[4] = idneighboor;
		m_aQuads[idneighboor]->m_aNeighbors[3] = idquad;

		if (isValidIdInArrQuads(m_aQuads[idneighboor]->m_aNeighbors[4]))
			m_aQuads[idneighboor]->m_chAxisX = AIGRID_QUAD_DIR_ALL;
		else
			m_aQuads[idneighboor]->m_chAxisX = AIGRID_QUAD_DIR_NEGATIVE;
	}

	idneighboor = quadGet(&float3(float3(aq->m_vPos) + float3(-AIGRID_QUAD_SIZE, 0, -AIGRID_QUAD_SIZE)), false);
	if (isValidIdInArrQuads(idneighboor))
	{
		aq->m_aNeighbors[5] = idneighboor;
		m_aQuads[idneighboor]->m_aNeighbors[2] = idquad;
	}

	idneighboor = quadGet(&float3(float3(aq->m_vPos) + float3(0, 0, -AIGRID_QUAD_SIZE)), false);
	if (isValidIdInArrQuads(idneighboor))
	{
		aq->m_aNeighbors[6] = idneighboor;
		m_aQuads[idneighboor]->m_aNeighbors[1] = idquad;

		if (isValidIdInArrQuads(m_aQuads[idneighboor]->m_aNeighbors[6]))
			m_aQuads[idneighboor]->m_chAxisY = AIGRID_QUAD_DIR_ALL;
		else
			m_aQuads[idneighboor]->m_chAxisY = AIGRID_QUAD_DIR_POSITIVE;
	}

	idneighboor = quadGet(&float3(float3(aq->m_vPos) + float3(AIGRID_QUAD_SIZE, 0, -AIGRID_QUAD_SIZE)), false);
	if (isValidIdInArrQuads(idneighboor))
	{
		aq->m_aNeighbors[7] = idneighboor;
		m_aQuads[idneighboor]->m_aNeighbors[0] = idquad;
	}

	//определяем возможные направления из квада по оси Y
	if (isValidIdInArrQuads(aq->m_aNeighbors[1]) && isValidIdInArrQuads(aq->m_aNeighbors[6]))
		aq->m_chAxisY = AIGRID_QUAD_DIR_ALL;
	else if (isValidIdInArrQuads(aq->m_aNeighbors[1]) && !isValidIdInArrQuads(aq->m_aNeighbors[6]))
		aq->m_chAxisY = AIGRID_QUAD_DIR_POSITIVE;
	else if (!isValidIdInArrQuads(aq->m_aNeighbors[1]) && isValidIdInArrQuads(aq->m_aNeighbors[6]))
		aq->m_chAxisY = AIGRID_QUAD_DIR_NEGATIVE;
	else
		aq->m_chAxisY = AIGRID_QUAD_DIR_NONE;

	//определяем возможные направления из квада по оси X
	if (isValidIdInArrQuads(aq->m_aNeighbors[3]) && isValidIdInArrQuads(aq->m_aNeighbors[4]))
		aq->m_chAxisX = AIGRID_QUAD_DIR_ALL;
	else if (isValidIdInArrQuads(aq->m_aNeighbors[3]) && !isValidIdInArrQuads(aq->m_aNeighbors[4]))
		aq->m_chAxisX = AIGRID_QUAD_DIR_NEGATIVE;
	else if (!isValidIdInArrQuads(aq->m_aNeighbors[3]) && isValidIdInArrQuads(aq->m_aNeighbors[4]))
		aq->m_chAxisX = AIGRID_QUAD_DIR_POSITIVE;
	else
		aq->m_chAxisX = AIGRID_QUAD_DIR_NONE;

	addUniqQuadArrag(aq);
	m_uiCountSplits = 0;
	return idquad;
}

bool CAIGrid::quadDelete(ID id)
{
	AIGRID_QUAD_PRECOND(id, false);

	CAIquad* aq = m_aQuads[id];

	if (aq->m_aNeighbors[0] != -1)
	{
		m_aQuads[aq->m_aNeighbors[0]]->m_aNeighbors[7] = -1;
	}

	if (aq->m_aNeighbors[1] != -1)
	{
		
		m_aQuads[aq->m_aNeighbors[1]]->m_aNeighbors[6] = -1;
		if (isValidIdInArrQuads(m_aQuads[aq->m_aNeighbors[1]]->m_aNeighbors[1]))
			m_aQuads[aq->m_aNeighbors[1]]->m_chAxisY = AIGRID_QUAD_DIR_POSITIVE;
		else
			m_aQuads[aq->m_aNeighbors[1]]->m_chAxisY = AIGRID_QUAD_DIR_NONE;
	}

	if (aq->m_aNeighbors[2] != -1)
	{
		m_aQuads[aq->m_aNeighbors[2]]->m_aNeighbors[5] = -1;
	}

	if (aq->m_aNeighbors[3] != -1)
	{
		m_aQuads[aq->m_aNeighbors[3]]->m_aNeighbors[4] = -1;
		if (isValidIdInArrQuads(m_aQuads[aq->m_aNeighbors[3]]->m_aNeighbors[3]))
			m_aQuads[aq->m_aNeighbors[3]]->m_chAxisX = AIGRID_QUAD_DIR_NEGATIVE;
		else
			m_aQuads[aq->m_aNeighbors[3]]->m_chAxisX = AIGRID_QUAD_DIR_NONE;
	}

	if (aq->m_aNeighbors[4] != -1)
	{
		m_aQuads[aq->m_aNeighbors[4]]->m_aNeighbors[3] = -1;
		if (isValidIdInArrQuads(m_aQuads[aq->m_aNeighbors[4]]->m_aNeighbors[4]))
			m_aQuads[aq->m_aNeighbors[4]]->m_chAxisX = AIGRID_QUAD_DIR_POSITIVE;
		else
			m_aQuads[aq->m_aNeighbors[4]]->m_chAxisX = AIGRID_QUAD_DIR_NONE;
	}

	if (aq->m_aNeighbors[5] != -1)
	{
		m_aQuads[aq->m_aNeighbors[5]]->m_aNeighbors[2] = -1;
	}

	if (aq->m_aNeighbors[6] != -1)
	{
		m_aQuads[aq->m_aNeighbors[6]]->m_aNeighbors[1] = -1;
		if (isValidIdInArrQuads(m_aQuads[aq->m_aNeighbors[6]]->m_aNeighbors[6]))
			m_aQuads[aq->m_aNeighbors[6]]->m_chAxisY = AIGRID_QUAD_DIR_NEGATIVE;
		else
			m_aQuads[aq->m_aNeighbors[6]]->m_chAxisY = AIGRID_QUAD_DIR_NONE;
	}

	if (aq->m_aNeighbors[7] != -1)
	{
		m_aQuads[aq->m_aNeighbors[7]]->m_aNeighbors[0] = -1;
	}
	
	aq->m_isClose = true;
	aq->m_chAxisX = aq->m_chAxisY = AIGRID_QUAD_DIR_NONE;
	for (int i = 0; i < 8; ++i)
	{
		aq->m_aNeighbors[i] = -1;
	}
	m_uiCountSplits = 0;

	quadDelete2(aq->m_id);
	return true;
}

void CAIGrid::quadDeleteInvalidAll()
{
	CAIquad* aq = 0;
	ID iddel = -1;
	for (long i = 0, il = m_aQuads.size(); i < il; ++i)
	{
		aq = m_aQuads[i];		
		if (aq && (aq->m_isClose || (aq->m_chAxisX == AIGRID_QUAD_DIR_NONE && aq->m_chAxisY == AIGRID_QUAD_DIR_NONE)))
		{
			quadDelete(i);
			il = m_aQuads.size();
		}
	}
}

void CAIGrid::quadDelete2(ID id)
{
	AIGRID_QUAD_PRECOND(id, _VOID);

	CAIquad* aq = 0;
	for (long i = 0, il = m_aQuads.size(); i < il; ++i)
	{
		aq = m_aQuads[i];

		if (aq->m_id > id)
			--(aq->m_id);

		for (int k = 0; k < 8; ++k)
		{
			if (aq->m_aNeighbors[k] > id)
				--(aq->m_aNeighbors[k]);
			else if (aq->m_aNeighbors[k] == id)
				aq->m_aNeighbors[k] = -1;
		}
	}

	CBoundAIquad* bound = 0;
	long tmpx = AIGRID_TOINT(m_aQuads[id]->m_vPos.x);
	long tmpy = AIGRID_TOINT(m_aQuads[id]->m_vPos.y);
	long tmpz = AIGRID_TOINT(m_aQuads[id]->m_vPos.z);

	for (int i = 1, il = m_aBound.size(); i <il; ++i)
	{
		bound = m_aBound[i];
		for (int k = 0, kl = bound->m_aIdsQuads.size(); k < kl; ++k)
		{
			if (bound->m_aIdsQuads[k] > id)
				--(bound->m_aIdsQuads[k]);
			else if (bound->m_aIdsQuads[k] == id)
			{
				bound->m_aIdsQuads.erase(k);
				--k;
				kl = bound->m_aIdsQuads.size();
			}
		}
	}

	for (int i = 0, il = m_aGraphPointsIDs.size(); i < il; ++i)
	{
		if (m_aGraphPointsIDs[i] > id)
			--(m_aGraphPointsIDs[i]);
		else if (m_aGraphPointsIDs[i] == id)
			m_aGraphPointsIDs.erase(i);
	}

	m_aLongCoordQuads.erase(id);
	m_oAllocQuad.Delete(m_aQuads[id]);
	m_aQuads.erase(id);

	/*mem_release(TransVertBuf);
	m_pDXDevice->CreateVertexBuffer(
		m_aQuads.size() * sizeof(CAIQuadVertexDataInst),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&TransVertBuf,
		0);*/

	m_aQuadColors.erase(id);
	m_aIDsInOpen.erase(id);
	m_aCostQuads.erase(id);
	m_aOpenIDs.erase(id);
	m_aParentIDs.erase(id);
	m_aTestSplitsIDs.erase(id);
	m_aQuadState.erase(id);
	m_aStateWho.erase(id);
	m_aPreCondFreeState.erase(id);
}

void CAIGrid::quadSelect(ID id, bool consider_prev)
{
	if (id < 0)
	{
		m_aSelectedQuads.clear();
		return;
	}

	AIGRID_QUAD_PRECOND(id, _VOID);

	if (!consider_prev)
		m_aSelectedQuads.clear();

	if (quadSelectedExists(id))
		return;

	m_aSelectedQuads.push_back(id);
}

bool CAIGrid::quadSelectedExists(ID id)
{
	for (int i = 0, il = m_aSelectedQuads.size(); i < il; ++i)
	{
		if (m_aSelectedQuads[i] == id)
			return true;
	}

	return false;
}

bool CAIGrid::quadSelectedExistsNeightborDirect(ID id)
{
	AIGRID_QUAD_PRECOND(id, false);

	if (m_aSelectedQuads.size() <= 0)
		return false;

	if (quadSelectedExists(id))
		return false;

	CAIquad* aq = m_aQuads[id];

	if (quadSelectedExists(aq->m_aNeighbors[1]))
		return true;

	if (quadSelectedExists(aq->m_aNeighbors[3]))
		return true;

	if (quadSelectedExists(aq->m_aNeighbors[4]))
		return true;

	if (quadSelectedExists(aq->m_aNeighbors[6]))
		return true;

	return false;
}

void CAIGrid::quadSelectedAddPosY(float posy)
{
	for (int i = 0, il = m_aSelectedQuads.size(); i < il; ++i)
	{
		if (m_aSelectedQuads[i] >= 0 && m_aQuads.size() > m_aSelectedQuads[i])
			m_aQuads[m_aSelectedQuads[i]]->m_vPos.y += posy;
	}
}

void CAIGrid::quadSelectedDelete()
{
	for (int i = 0, il = m_aSelectedQuads.size(); i < il; ++i)
	{
		quadDelete(m_aSelectedQuads[i]);
	}

	m_aSelectedQuads.clear();
}

void CAIGrid::phyNavigate(CAIquad *pQuad)
{
	pQuad->m_isClose = AIQuadPhyNavigate(&(pQuad->m_vPos));
				}

void CAIGrid::initGraphics()
{
	m_useGraphics = true;
	m_pDXDevice = SGCore_GetDXDevice();

	if (!m_pDXDevice)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - dx device is not init", GEN_MSG_LOCATION);
	}

	D3DVERTEXELEMENT9 InstanceCAIquad[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 28, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

		D3DDECL_END()
	};

	m_pDXDevice->CreateVertexDeclaration(InstanceCAIquad, &m_pDecl);

	m_idTex = SGCore_LoadTexAddName("decal_aigrid1.dds", LOAD_TEXTURE_TYPE_CONST);

	m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "aigrid_quad.vs", "aigrid_quad.vs", SHADER_CHECKDOUBLE_PATH);
	m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "aigrid_quad.ps", "aigrid_quad.ps", SHADER_CHECKDOUBLE_PATH);


	IDirect3DTexture9* TexBB;
	m_pDXDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &TexBB, NULL);
	D3DLOCKED_RECT LockedRect;
	
	TexBB->LockRect(0, &LockedRect, 0, 0);

	uint32_t* tmpOldColor = (uint32_t*)LockedRect.pBits + 0 * LockedRect.Pitch + 0 * sizeof(uint32_t);
	*tmpOldColor = AIGRID_COLOR_GBBOX;

	TexBB->UnlockRect(0);

	//SGCore_LoadTexLoadTextures();
	m_idTexBB = SGCore_LoadTexCreate("aigrid_tex_bb__", TexBB);

	m_pDXDevice->CreateVertexBuffer(
                   4 * sizeof(CAIQuadVertexData),
                   D3DUSAGE_WRITEONLY,
                   0,
                   D3DPOOL_MANAGED,
				   &m_pVertexQuad,
                   0);

	m_pDXDevice->CreateIndexBuffer(
                   6 * sizeof(WORD),
                   D3DUSAGE_WRITEONLY,
                   D3DFMT_INDEX16,
                   D3DPOOL_MANAGED,
				   &m_pIndexQuad,
                   0);

	CAIQuadVertexData* vertices;
	m_pVertexQuad->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = CAIQuadVertexData(-AIGRID_QUAD_SIZEDIV2, 0.0f, -AIGRID_QUAD_SIZEDIV2, 0.0, 1.0f);
	vertices[1] = CAIQuadVertexData(-AIGRID_QUAD_SIZEDIV2, 0.0f,  AIGRID_QUAD_SIZEDIV2, 0.0, 0.0f);
	vertices[2] = CAIQuadVertexData( AIGRID_QUAD_SIZEDIV2, 0.0f,  AIGRID_QUAD_SIZEDIV2, 1.0, 0.0f);
	vertices[3] = CAIQuadVertexData( AIGRID_QUAD_SIZEDIV2, 0.0f, -AIGRID_QUAD_SIZEDIV2, 1.0, 1.0f);

	m_pVertexQuad->Unlock();

	WORD* indices = 0;
	m_pIndexQuad->Lock(0, 0, (void**)&indices, 0);

	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;

	m_pIndexQuad->Unlock();
}

void CAIGrid::clear()
{
	mem_delete(m_pTreeBound);
	m_oAllocQuad.clear();
	m_aQuads.clear();

	m_oAllocBound.clear();
	m_aBound.clear();

	mem_release_del(m_pTransVertBuf);
	m_aQuadColors.clear();
	m_aIDsInOpen.clear();
	m_aCostQuads.clear();
	m_aOpenIDs.clear();
	m_aParentIDs.clear();
	m_aTestSplitsIDs.clear();
	m_aQuadState.clear();
	m_aStateWho.clear();
	m_aPreCondFreeState.clear();
	m_aGraphPointsIDs.clear();
	m_aCostGraphPointsIDs.clear();
	mem_release_del(m_pBoundBox);
}

bool CAIGrid::existsQuads() const
{
	return (m_aQuads.size() > 0);
}

void CAIGrid::gridClear()
{
	m_oAllocQuad.clear();
	m_aQuads.clear();

	for (int i = 0, il = m_aBound.size(); i < il; ++i)
	{
		m_aBound[i]->m_aIdsQuads.clear();
	}

	mem_release_del(m_pTransVertBuf);
	m_aQuadColors.clear();
	m_aIDsInOpen.clear();
	m_aCostQuads.clear();
	m_aOpenIDs.clear();
	m_aParentIDs.clear();
	m_aTestSplitsIDs.clear();
	m_aQuadState.clear();
	m_aStateWho.clear();
	m_aPreCondFreeState.clear();
	m_aGraphPointsIDs.clear();
	m_aCostGraphPointsIDs.clear();
}

void CAIGrid::splitBB(int xv,int yv,int zv)
{
	float xval = (m_aBound[0]->m_f3Max.x - m_aBound[0]->m_f3Min.x);
	float yval = (m_aBound[0]->m_f3Max.y - m_aBound[0]->m_f3Min.y);
	float zval = (m_aBound[0]->m_f3Max.z - m_aBound[0]->m_f3Min.z);

	float countxval = xval / float(xv);
	float countyval = yval / float(yv);
	float countzval = zval / float(zv);

	float cxval = (m_aBound[0]->m_f3Max.x + m_aBound[0]->m_f3Min.x)*0.5f;
	float cyval = (m_aBound[0]->m_f3Max.y + m_aBound[0]->m_f3Min.y)*0.5f;
	float czval = (m_aBound[0]->m_f3Max.z + m_aBound[0]->m_f3Min.z)*0.5f;

	m_aBound.reserve(xv*yv*zv);

		for(int x=0;x<xv;x++)
		{
				for(int y=0;y<yv;y++)
				{
						for(int z=0;z<zv;z++)
						{
							CBoundAIquad* tmpbb = m_oAllocBound.Alloc();
							float3 center = float3(
													lerpf(m_aBound[0]->m_f3Min.x,m_aBound[0]->m_f3Max.x,float(x)/float(xv)) + countxval*0.5,
													lerpf(m_aBound[0]->m_f3Min.y,m_aBound[0]->m_f3Max.y,float(y)/float(yv)) + countyval*0.5,
													lerpf(m_aBound[0]->m_f3Min.z,m_aBound[0]->m_f3Max.z,float(z)/float(zv)) + countzval*0.5);

							tmpbb->m_f3Max.x = countxval*0.5 + center.x;
							tmpbb->m_f3Max.y = countyval*0.5 + center.y;
							tmpbb->m_f3Max.z = countzval*0.5 + center.z;

							tmpbb->m_f3Min.x = -(countxval*0.5) + center.x;
							tmpbb->m_f3Min.y = -(countyval*0.5) + center.y;
							tmpbb->m_f3Min.z = -(countzval*0.5) + center.z;

							tmpbb->m_i3Max.x = AIGRID_TOINT(tmpbb->m_f3Max.x);
							tmpbb->m_i3Max.y = AIGRID_TOINT(tmpbb->m_f3Max.y);
							tmpbb->m_i3Max.z = AIGRID_TOINT(tmpbb->m_f3Max.z);

							tmpbb->m_i3Min.x = AIGRID_TOINT(tmpbb->m_f3Min.x);
							tmpbb->m_i3Min.y = AIGRID_TOINT(tmpbb->m_f3Min.y);
							tmpbb->m_i3Min.z = AIGRID_TOINT(tmpbb->m_f3Min.z);

							m_aBound.push_back(tmpbb);
						}
				}
		}
}

void CAIGrid::gridGenerate()
{
	if (m_aQuads.size() <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "it is necessary one quad in AI grid");
		return;
	}

	DWORD ttime = GetTickCount();
	int oldsizegrid = m_aQuads.size();

	while (true)
	{
		ID idquadunchecked = -1;

		//поиск первого не входящего ни в какой сплит
		for (int i = 0, il = m_aQuads.size(); i < il; ++i)
		{
			if (isValidIdInArrQuads(i) && m_aQuads[i]->m_chAxisX == AIGRID_QUAD_DIR_NONE && m_aQuads[i]->m_chAxisY == AIGRID_QUAD_DIR_NONE)
			{
				idquadunchecked = i;
				break;
			}
		}

		//не входящие в какой-либо сплит не найдены
		if (idquadunchecked < 0)
			break;

		m_aQuadsCheck.push_back(m_aQuads[idquadunchecked]);

		DWORD tmptmp = 0;
		DWORD tmpcountkey = 0;
		while (m_aQuadsCheck.size() > 0 && m_aQuadsCheck.size() != tmptmp)
		{
			tmpcountkey = tmptmp;
			if (m_aQuadsCheck[tmpcountkey])
			{
				genQuad(m_aQuadsCheck[tmpcountkey]);
			}
			++tmptmp;
		}
		m_aQuadsCheck.clear();
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "AI grid generated, all count = %d, gen count = %d, count time = %d mlsec\n", m_aQuads.size(), m_aQuads.size() - oldsizegrid, GetTickCount() - ttime);
	
	m_uiCountSplits = 0;
	reCreateBuffs();
}

void CAIGrid::graphPointGenerate()
{
	if (m_aQuads.size() <= 1)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "AI grid not found\n");
		return;
	}

	m_aGraphPointsIDs.clear();
	m_aCostGraphPointsIDs.clear();
	for (int k = 1, kl = m_aBound.size(); k<kl; ++k)
	{
		DWORD tmpgp = -1;
		float tmplastdist = 0;
		float tmplastdist2;
		float3 tmpcenterbb = (float3(m_aBound[k]->m_f3Max) + float3(m_aBound[k]->m_f3Min)) * 0.5f;

		for (DWORD j = 0; j<m_aBound[k]->m_aIdsQuads.size(); j++)
		{
			if ((tmplastdist2 = SMVector3Length2(tmpcenterbb - m_aQuads[m_aBound[k]->m_aIdsQuads[j]]->m_vPos)) < tmplastdist || tmpgp == -1)
			{
				tmpgp = j;
				tmplastdist = tmplastdist2;
			}
		}

		if (tmpgp != -1)
		{
			m_aGraphPointsIDs.push_back(m_aBound[k]->m_aIdsQuads[tmpgp]);
		}
	}

	m_aCostGraphPointsIDs.resize(m_aGraphPointsIDs.size());
	if (m_aGraphPointsIDs.size() > 0)
	{
		memset(&(m_aCostGraphPointsIDs[0]), -1, m_aCostGraphPointsIDs.size() * sizeof(int32_t));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "graph points is generated, count %d\n", m_aGraphPointsIDs.size());
	}
}

UINT CAIGrid::graphPointGetCount() const
{
	return m_aGraphPointsIDs.size();
}

void CAIGrid::graphPointClear()
{
	m_aGraphPointsIDs.clear();
}

void CAIGrid::graphPointAdd(ID id)
{
	AIGRID_QUAD_PRECOND(id, _VOID);

	if (graphPointGetKey(id) >= 0)
		return;

	m_aGraphPointsIDs.push_back(id);
}

void CAIGrid::graphPointDelete(ID id)
{
	AIGRID_QUAD_PRECOND(id, _VOID);

	ID gpkey = -1;
	if ((gpkey = graphPointGetKey(id)) < 0)
		return;

	m_aGraphPointsIDs.erase(gpkey);
}

ID CAIGrid::graphPointGetKey(ID id)
{
	AIGRID_QUAD_PRECOND(id, -1);

	for (int i = 0, il = m_aGraphPointsIDs.size(); i < il; ++i)
	{
		if (m_aGraphPointsIDs[i] == id)
			return i;
	}

	return -1;
}

void CAIGrid::addUniqQuadArrag(const CAIquad* aq)
{
	long tmpx = AIGRID_TOINT(aq->m_vPos.x);
	long tmpy = AIGRID_TOINT(aq->m_vPos.y);
	long tmpz = AIGRID_TOINT(aq->m_vPos.z);
	CBoundAIquad* bound = 0;
	for (int i = 1, il = m_aBound.size(); i < il; ++i)
	{
		bound = m_aBound[i];
		//если центр квада входит в какой-либо бокс
		if (
			bound->m_i3Min.x <= tmpx && bound->m_i3Min.y <= tmpy && bound->m_i3Min.z <= tmpz &&
			bound->m_i3Max.x >= tmpx && bound->m_i3Max.y >= tmpy && bound->m_i3Max.z >= tmpz
			)
		{
			bound->m_aIdsQuads.push_back(aq->m_id);
			break;
		}
	}

	m_aLongCoordQuads[aq->m_id] = int3(tmpx, tmpy, tmpz);
}

void CAIGrid::addUniqQuadArrag2(const CAIquad* aq, CBoundAIquad *pBB)
{
	if (!pBB)
	{
		return;
	}

	pBB->m_aIdsQuads.push_back(aq->m_id);
	m_aLongCoordQuads[aq->m_id] = int3(AIGRID_TOINT(aq->m_vPos.x), AIGRID_TOINT(aq->m_vPos.y), AIGRID_TOINT(aq->m_vPos.z));
}

bool CAIGrid::isUniqQuadArrag(CAIquad* aq, ID * idquad, CBoundAIquad ** idbb)
{
	if (!m_pTreeBound)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, " %s - tree bounds is not build", GEN_MSG_LOCATION);
		return false;
	}

	int3 l3;
	CBoundAIquad *tmpidbb = 0;
	ID tmpsizearr = m_aQuads.size();
	long tmpx = AIGRID_TOINT(aq->m_vPos.x);
	long tmpy = AIGRID_TOINT(aq->m_vPos.y);
	long tmpz = AIGRID_TOINT(aq->m_vPos.z);
	CBoundAIquad* bound = 0;

	/*for (UINT i = 1, il = m_aBound.size(); i<il; ++i)
	{
		bound = m_aBound[i];
		//если центр квада входит в какой-либо бокс
		if (
			bound->m_i3Min.x <= tmpx && bound->m_i3Min.y <= tmpy && bound->m_i3Min.z <= tmpz &&
			bound->m_i3Max.x >= tmpx && bound->m_i3Max.y >= tmpy && bound->m_i3Max.z >= tmpz
			)
		{
			tmpidbb = i;
			//анализируем все входящие в данный бокс квады
			for (UINT k = 0, kl = bound->m_aIdsQuads.size(); k < kl; ++k)
			{
				l3 = m_aLongCoordQuads[bound->m_aIdsQuads[k]];
				if (
					l3.x == tmpx &&
					l3.z == tmpz &&
					abs(l3.y - tmpy) < LAIGRID_QUADS_CENTERS_MAXHEIGHT
					)
				{
					*idquad = bound->m_aIdsQuads[k];
					
					if (idbb)
						*idbb = tmpidbb;
					return false;
				}
			}
		}
	}*/

	Array<CTreeBoundAIquad*, 64> queue;
	int iCurrKey = 0;
	queue.push_back(m_pTreeBound);
	CTreeBoundAIquad *pFoundSplit = 0;

	while (queue.size() > iCurrKey)
	{
		CTreeBoundAIquad *pSplit = queue[iCurrKey];

		for (int i = 0; i<4; i++)
		{
			if (
				/*pSplit->m_aSplits[i]->m_i3Min.x - LAIGRID_QUAD_SIZE <= tmpx && pSplit->m_aSplits[i]->m_i3Min.z - LAIGRID_QUAD_SIZE <= tmpz &&
				pSplit->m_aSplits[i]->m_i3Max.x + LAIGRID_QUAD_SIZE >= tmpx && pSplit->m_aSplits[i]->m_i3Max.z + LAIGRID_QUAD_SIZE >= tmpz*/
				pSplit->m_aSplits[i]->m_i3Min.x <= tmpx && pSplit->m_aSplits[i]->m_i3Min.y <= tmpy && pSplit->m_aSplits[i]->m_i3Min.z <= tmpz &&
				pSplit->m_aSplits[i]->m_i3Max.x >= tmpx && pSplit->m_aSplits[i]->m_i3Max.y >= tmpy && pSplit->m_aSplits[i]->m_i3Max.z >= tmpz
				)
			{
				if (pSplit->m_aSplits[i]->m_aSplits[0])
					queue.push_back(pSplit->m_aSplits[i]);
				else
				{
					pFoundSplit = pSplit->m_aSplits[i];
					queue.clear();
					break;
	}
			}
		}

		//queue.erase(0);
		++iCurrKey;
	}

	if (!pFoundSplit)
		return -1;

	for (int i = 0, il = pFoundSplit->m_pBounds.size(); i < il; ++i)
	{
		bound = pFoundSplit->m_pBounds[i];
		if (
			bound->m_i3Min.x <= tmpx && bound->m_i3Min.y <= tmpy && bound->m_i3Min.z <= tmpz &&
			bound->m_i3Max.x >= tmpx && bound->m_i3Max.y >= tmpy && bound->m_i3Max.z >= tmpz
			)
		{
			tmpidbb = bound;
			for (int k = 0, kl = bound->m_aIdsQuads.size(); k < kl; ++k)
			{
				l3 = m_aLongCoordQuads[bound->m_aIdsQuads[k]];
				/*if (
					abs((l3.x) - (tmpx)) <= LAIGRID_QUAD_SIZEDIV2 &&
					abs((l3.z) - (tmpz)) <= LAIGRID_QUAD_SIZEDIV2
					)*/
				{

					if (
						l3.x == tmpx &&
						l3.z == tmpz &&
						abs(l3.y - tmpy) < LAIGRID_QUADS_CENTERS_MAXHEIGHT
						)
					{
						*idquad = bound->m_aIdsQuads[k];

	if (idbb)
							*idbb = bound;
						return false;
					}
				}
			}
		}
	}

	if (idbb)
		*idbb = tmpidbb;

	return true;
}

void CAIGrid::genQuad(CAIquad* aq)
{
	bool tmpiscom = true;
	bool tmpiscom2 = true;
	ID idquad = -1;
	CBoundAIquad *pBB = 0;
	static float3_t tmppos;

	CAIquad *aq12, *aq21, *aq23, *aq32, *aqold;
	aq12 = aq21 = aq23 = aq32 = aqold = 0;
	bool isunic = false;

	// ^
	if (!isValidIdInArrQuads(aq->m_aNeighbors[1]))
	{
		tmppos = aq->m_vPos;
		tmppos.z += AIGRID_QUAD_SIZE;

		if (
			m_vMax.x >= tmppos.x + AIGRID_QUAD_SIZEDIV2 && m_vMax.z >= tmppos.z + AIGRID_QUAD_SIZEDIV2 &&
			m_vMax.y >= tmppos.y && m_vMin.y <= tmppos.y &&
			m_vMin.x <= tmppos.x - AIGRID_QUAD_SIZEDIV2 && m_vMin.z <= tmppos.z - AIGRID_QUAD_SIZEDIV2
			)
		{
			aq12 = m_oAllocQuad.Alloc();

			aq12->m_vPos = tmppos;

			idquad = -1;
			pBB = 0;
			isunic = isUniqQuadArrag(aq12, &idquad, &pBB);

			if (isunic)
			{
				phyNavigate(aq12);

				if (abs(aq->m_vPos.y - aq12->m_vPos.y) < AIGRID_QUADS_CENTERS_MAXHEIGHT && !(aq12->m_isClose))
				{
					aq12->m_vPos.y += AIGRID_QUAD_ADDYPOS;

					m_aQuadsCheck.push_back(aq12);
					aq12->m_id = m_aQuads.size();
					m_aQuads.push_back(aq12);
					aq->m_aNeighbors[1] = aq12->m_id;
					addUniqQuadArrag2(aq12, pBB);
				}
				else
				{
					m_oAllocQuad.Delete(aq12);
					aq12 = 0;
				}
			}
			else
			{
				aqold = aq12;
				aq->m_aNeighbors[1] = idquad;
				aq12 = m_aQuads[idquad];
				if (aq12->m_isClose)
				{
					aq12->m_vPos = aqold->m_vPos;
					aq12->m_vPos.y += AIGRID_QUAD_ADDYPOS;
					aq12->m_isClose = false;
					m_aQuadsCheck.push_back(aq12);
				}

				m_oAllocQuad.Delete(aqold);
				aqold = 0;
			}
		}
	}

	// \/
	if (!isValidIdInArrQuads(aq->m_aNeighbors[6]))
	{
		tmppos = aq->m_vPos;
		tmppos.z -= AIGRID_QUAD_SIZE;

		if (
			m_vMax.x >= tmppos.x + AIGRID_QUAD_SIZEDIV2 && m_vMax.z >= tmppos.z + AIGRID_QUAD_SIZEDIV2 &&
			m_vMax.y >= tmppos.y && m_vMin.y <= tmppos.y &&
			m_vMin.x <= tmppos.x - AIGRID_QUAD_SIZEDIV2 && m_vMin.z <= tmppos.z - AIGRID_QUAD_SIZEDIV2
			)
		{
			aq32 = m_oAllocQuad.Alloc();

			aq32->m_vPos = tmppos;

			idquad = -1;
			pBB = 0;
			isunic = isUniqQuadArrag(aq32, &idquad, &pBB);

			if (isunic)
			{
				phyNavigate(aq32);

				if (abs(aq->m_vPos.y - aq32->m_vPos.y) < AIGRID_QUADS_CENTERS_MAXHEIGHT && !(aq32->m_isClose))
				{
					aq32->m_vPos.y += AIGRID_QUAD_ADDYPOS;

					m_aQuadsCheck.push_back(aq32);
					aq32->m_id = m_aQuads.size();
					m_aQuads.push_back(aq32);
					aq->m_aNeighbors[6] = aq32->m_id;
					addUniqQuadArrag2(aq32, pBB);
				}
				else
				{
					m_oAllocQuad.Delete(aq32);
					aq32 = 0;
				}
			}
			else
			{
				aqold = aq32;
				aq->m_aNeighbors[6] = idquad;
				aq32 = m_aQuads[idquad];
				if (aq32->m_isClose)
				{
					aq32->m_vPos = aqold->m_vPos;
					aq32->m_vPos.y += AIGRID_QUAD_ADDYPOS;
					aq32->m_isClose = false;
					m_aQuadsCheck.push_back(aq32);
				}

				m_oAllocQuad.Delete(aqold);
				aqold = 0;
			}
		}
	}

	// ->
	if (!isValidIdInArrQuads(aq->m_aNeighbors[4]))
	{
		tmppos = aq->m_vPos;
		tmppos.x += AIGRID_QUAD_SIZE;

		if (
			m_vMax.x >= tmppos.x + AIGRID_QUAD_SIZEDIV2 && m_vMax.z >= tmppos.z + AIGRID_QUAD_SIZEDIV2 &&
			m_vMax.y >= tmppos.y && m_vMin.y <= tmppos.y &&
			m_vMin.x <= tmppos.x - AIGRID_QUAD_SIZEDIV2 && m_vMin.z <= tmppos.z - AIGRID_QUAD_SIZEDIV2
			)
		{
			aq23 = m_oAllocQuad.Alloc();

			aq23->m_vPos = tmppos;

			idquad = -1;
			pBB = 0;
			isunic = isUniqQuadArrag(aq23, &idquad, &pBB);

			if (isunic)
			{
				phyNavigate(aq23);

				if (abs(aq->m_vPos.y - aq23->m_vPos.y) < AIGRID_QUADS_CENTERS_MAXHEIGHT && !(aq23->m_isClose))
				{
					aq23->m_vPos.y += AIGRID_QUAD_ADDYPOS;

					m_aQuadsCheck.push_back(aq23);
					aq23->m_id = m_aQuads.size();
					m_aQuads.push_back(aq23);
					aq->m_aNeighbors[4] = aq23->m_id;
					addUniqQuadArrag2(aq23, pBB);
				}
				else
				{
					m_oAllocQuad.Delete(aq23);
					aq23 = 0;
				}
			}
			else
			{
				aqold = aq23;
				aq->m_aNeighbors[4] = idquad;
				aq23 = m_aQuads[idquad];
				if (aq23->m_isClose)
				{
					aq23->m_vPos = aqold->m_vPos;
					aq23->m_vPos.y += AIGRID_QUAD_ADDYPOS;
					aq23->m_isClose = false;
					m_aQuadsCheck.push_back(aq23);
				}

				m_oAllocQuad.Delete(aqold);
				aqold = 0;
			}
		}
	}

	// <-
	if (!isValidIdInArrQuads(aq->m_aNeighbors[3]))
	{
		tmppos = aq->m_vPos;
		tmppos.x -= AIGRID_QUAD_SIZE;

		if (
			m_vMax.x >= tmppos.x + AIGRID_QUAD_SIZEDIV2 && m_vMax.z >= tmppos.z + AIGRID_QUAD_SIZEDIV2 &&
			m_vMax.y >= tmppos.y && m_vMin.y <= tmppos.y &&
			m_vMin.x <= tmppos.x - AIGRID_QUAD_SIZEDIV2 && m_vMin.z <= tmppos.z - AIGRID_QUAD_SIZEDIV2
			)
		{
			aq21 = m_oAllocQuad.Alloc();

			aq21->m_vPos = tmppos;

			idquad = -1;
			pBB = 0;
			isunic = isUniqQuadArrag(aq21, &idquad, &pBB);

			if (isunic)
			{
				phyNavigate(aq21);

				if (abs(aq->m_vPos.y - aq21->m_vPos.y) < AIGRID_QUADS_CENTERS_MAXHEIGHT && !(aq21->m_isClose))
				{
					aq21->m_vPos.y += AIGRID_QUAD_ADDYPOS;

					m_aQuadsCheck.push_back(aq21);
					aq21->m_id = m_aQuads.size();
					m_aQuads.push_back(aq21);
					aq->m_aNeighbors[3] = aq21->m_id;
					addUniqQuadArrag2(aq21, pBB);
				}
				else
				{
					m_oAllocQuad.Delete(aq21);
					aq21 = 0;
				}
			}
			else
			{
				aqold = aq21;
				aq->m_aNeighbors[3] = idquad;
				aq21 = m_aQuads[idquad];
				if (aq21->m_isClose)
				{
					aq21->m_vPos = aqold->m_vPos;
					aq21->m_vPos.y += AIGRID_QUAD_ADDYPOS;
					aq21->m_isClose = false;
					m_aQuadsCheck.push_back(aq21);
				}

				m_oAllocQuad.Delete(aqold);
				aqold = 0;
			}
		}
	}

	//определяем возможные направления из квада по оси Y
	if (isValidIdInArrQuads(aq->m_aNeighbors[1]) && isValidIdInArrQuads(aq->m_aNeighbors[6]))
		aq->m_chAxisY = AIGRID_QUAD_DIR_ALL;
	else if (isValidIdInArrQuads(aq->m_aNeighbors[1]) && !isValidIdInArrQuads(aq->m_aNeighbors[6]))
		aq->m_chAxisY = AIGRID_QUAD_DIR_POSITIVE;
	else if (!isValidIdInArrQuads(aq->m_aNeighbors[1]) && isValidIdInArrQuads(aq->m_aNeighbors[6]))
		aq->m_chAxisY = AIGRID_QUAD_DIR_NEGATIVE;
	else
		aq->m_chAxisY = AIGRID_QUAD_DIR_NONE;

	//определяем возможные направления из квада по оси X
	if (isValidIdInArrQuads(aq->m_aNeighbors[3]) && isValidIdInArrQuads(aq->m_aNeighbors[4]))
		aq->m_chAxisX = AIGRID_QUAD_DIR_ALL;
	else if (isValidIdInArrQuads(aq->m_aNeighbors[3]) && !isValidIdInArrQuads(aq->m_aNeighbors[4]))
		aq->m_chAxisX = AIGRID_QUAD_DIR_NEGATIVE;
	else if (!isValidIdInArrQuads(aq->m_aNeighbors[3]) && isValidIdInArrQuads(aq->m_aNeighbors[4]))
		aq->m_chAxisX = AIGRID_QUAD_DIR_POSITIVE;
	else
		aq->m_chAxisX = AIGRID_QUAD_DIR_NONE;

	if (aq->m_chAxisY == AIGRID_QUAD_DIR_NONE && aq->m_chAxisX == AIGRID_QUAD_DIR_NONE)
	{
		aq->m_isClose = true;
		quadDelete(aq->m_id);
	}
	else
	{
		computeNeighbor(aq->m_id);
	}
}

void CAIGrid::computeNeighbor(ID id)
{
	CAIquad* aq = m_aQuads[id];
	CAIquad *aq12, *aq21, *aq23, *aq32;
	aq12 = aq21 = aq23 = aq32 = 0;

	CAIquad *aq11, *aq13, *aq31, *aq33;
	aq11 = aq13 = aq31 = aq33 = 0;

	if (aq)
	{
		if (aq->m_aNeighbors[0] != -1)
			aq11 = m_aQuads[aq->m_aNeighbors[0]];

		if (aq->m_aNeighbors[1] != -1)
			aq12 = m_aQuads[aq->m_aNeighbors[1]];

		if (aq->m_aNeighbors[2] != -1)
			aq13 = m_aQuads[aq->m_aNeighbors[2]];

		if (aq->m_aNeighbors[3] != -1)
			aq21 = m_aQuads[aq->m_aNeighbors[3]];

		if (aq->m_aNeighbors[4] != -1)
			aq23 = m_aQuads[aq->m_aNeighbors[4]];

		if (aq->m_aNeighbors[5] != -1)
			aq31 = m_aQuads[aq->m_aNeighbors[5]];

		if (aq->m_aNeighbors[6] != -1)
			aq32 = m_aQuads[aq->m_aNeighbors[6]];

		if (aq->m_aNeighbors[7] != -1)
			aq33 = m_aQuads[aq->m_aNeighbors[7]];

		if (aq->m_aNeighbors[1] != -1)
		{
			//указываем самый нижний соседом
			aq12->setNeighbor(6, aq->m_id);
			//если левый нижний создан то он тоже сосед
			if (aq->m_aNeighbors[3] != -1)
				aq12->setNeighbor(5, aq->m_aNeighbors[3]);

			//если правый нижний создан то он тоже сосед
			if (aq->m_aNeighbors[4] != -1)
				aq12->setNeighbor(7, aq->m_aNeighbors[4]);

			//если у обрабатываемого нода есть верхний левый
			if (aq->m_aNeighbors[0] != -1)
			{
				if (gridCompareHeight(aq->m_aNeighbors[1], aq->m_aNeighbors[0]))
					aq12->setNeighbor(3, aq->m_aNeighbors[0]);

				if (gridCompareHeight(aq->m_aNeighbors[0], aq->m_aNeighbors[1]))
					aq11->setNeighbor(4, aq->m_aNeighbors[1]);

				if (aq11->m_aNeighbors[1] != -1)
				{
					if (gridCompareHeight(aq11->m_aNeighbors[1], aq->m_aNeighbors[1]))
						m_aQuads[aq11->m_aNeighbors[1]]->setNeighbor(7, aq->m_aNeighbors[1]);

					if (gridCompareHeight(aq->m_aNeighbors[1], aq11->m_aNeighbors[1]))
						aq12->setNeighbor(0, aq11->m_aNeighbors[1]);
				}

				if (aq11->m_aNeighbors[1] != -1 && m_aQuads[aq11->m_aNeighbors[1]]->m_aNeighbors[4] != -1)
				{
					if (gridCompareHeight(m_aQuads[aq11->m_aNeighbors[1]]->m_aNeighbors[4], aq->m_aNeighbors[1]))
						m_aQuads[m_aQuads[aq11->m_aNeighbors[1]]->m_aNeighbors[4]]->setNeighbor(6, aq->m_aNeighbors[1]);

					if (gridCompareHeight(aq->m_aNeighbors[1], m_aQuads[aq11->m_aNeighbors[1]]->m_aNeighbors[4]))
						aq12->setNeighbor(1, m_aQuads[aq11->m_aNeighbors[1]]->m_aNeighbors[4]);
				}
				else if (aq11->m_aNeighbors[2] != -1)
				{
					if (gridCompareHeight(aq11->m_aNeighbors[2], aq->m_aNeighbors[1]))
						m_aQuads[aq11->m_aNeighbors[2]]->setNeighbor(6, aq->m_aNeighbors[1]);

					if (gridCompareHeight(aq->m_aNeighbors[1], aq11->m_aNeighbors[2]))
						aq12->setNeighbor(1, aq11->m_aNeighbors[2]);
				}
			}

			//если у обрабатываемого нода есть верхний правый
			if (aq->m_aNeighbors[2] != -1)
			{
				if (gridCompareHeight(aq->m_aNeighbors[1], aq->m_aNeighbors[2]))
					aq12->setNeighbor(4, aq->m_aNeighbors[2]);

				if (gridCompareHeight(aq->m_aNeighbors[1], aq->m_aNeighbors[1]))
					aq13->setNeighbor(3, aq->m_aNeighbors[1]);

				if (aq13->m_aNeighbors[1] != -1)
				{
					if (gridCompareHeight(aq13->m_aNeighbors[1], aq->m_aNeighbors[1]))
						m_aQuads[aq13->m_aNeighbors[1]]->setNeighbor(5, aq->m_aNeighbors[1]);

					if (gridCompareHeight(aq->m_aNeighbors[1], aq13->m_aNeighbors[1]))
						aq12->setNeighbor(2, aq13->m_aNeighbors[1]);
				}

				if (aq12->m_aNeighbors[1] == -1)
				{
					if (aq13->m_aNeighbors[1] != -1 && m_aQuads[aq13->m_aNeighbors[1]]->m_aNeighbors[3] != -1)
					{
						if (gridCompareHeight(m_aQuads[aq13->m_aNeighbors[1]]->m_aNeighbors[3], aq->m_aNeighbors[1]))
							m_aQuads[m_aQuads[aq13->m_aNeighbors[1]]->m_aNeighbors[3]]->setNeighbor(6, aq->m_aNeighbors[1]);

						if (gridCompareHeight(aq->m_aNeighbors[1], m_aQuads[aq13->m_aNeighbors[1]]->m_aNeighbors[3]))
							aq12->setNeighbor(1, m_aQuads[aq13->m_aNeighbors[1]]->m_aNeighbors[3]);
					}
					else if (aq13->m_aNeighbors[0] != -1)
					{
						if (gridCompareHeight(aq13->m_aNeighbors[0], aq->m_aNeighbors[1]))
							m_aQuads[aq13->m_aNeighbors[0]]->setNeighbor(6, aq->m_aNeighbors[1]);

						if (gridCompareHeight(aq->m_aNeighbors[1], aq13->m_aNeighbors[0]))
							aq12->setNeighbor(1, aq13->m_aNeighbors[0]);
					}
				}
			}


		}


		//если создали нижний нод
		if (aq->m_aNeighbors[6] != -1)
		{
			//указываем самый верхний соседом
			//aq32->m_aNeighbors[1] = aq->m_id;
			aq32->setNeighbor(1, aq->m_id);
			//если левый верхний создан то он тоже сосед
			if (aq->m_aNeighbors[3] != -1)
				aq32->setNeighbor(0, aq->m_aNeighbors[3]);

			//если правый верхний создан то он тоже сосед
			if (aq->m_aNeighbors[4] != -1)
				aq32->setNeighbor(2, aq->m_aNeighbors[4]);

			//если у обрабатываемого нода есть нижний левый
			if (aq->m_aNeighbors[5] != -1)
			{
				if (gridCompareHeight(aq->m_aNeighbors[6], aq->m_aNeighbors[5]))
					aq32->setNeighbor(3, aq->m_aNeighbors[5]);

				if (gridCompareHeight(aq->m_aNeighbors[5], aq->m_aNeighbors[6]))
					aq31->setNeighbor(4, aq->m_aNeighbors[6]);

				if (aq31->m_aNeighbors[6] != -1)
				{
					if (gridCompareHeight(aq->m_aNeighbors[6], aq31->m_aNeighbors[6]))
						aq32->setNeighbor(5, aq31->m_aNeighbors[6]);

					if (gridCompareHeight(aq31->m_aNeighbors[6], aq->m_aNeighbors[6]))
						m_aQuads[aq31->m_aNeighbors[6]]->setNeighbor(2, aq->m_aNeighbors[6]);
				}

				if (aq31->m_aNeighbors[6] != -1 && m_aQuads[aq31->m_aNeighbors[6]]->m_aNeighbors[4] != -1)
				{
					if (gridCompareHeight(m_aQuads[aq31->m_aNeighbors[6]]->m_aNeighbors[4], aq->m_aNeighbors[6]))
						m_aQuads[m_aQuads[aq31->m_aNeighbors[6]]->m_aNeighbors[4]]->setNeighbor(1, aq->m_aNeighbors[6]);

					if (gridCompareHeight(aq->m_aNeighbors[6], m_aQuads[aq31->m_aNeighbors[6]]->m_aNeighbors[4]))
						aq32->setNeighbor(6, m_aQuads[aq31->m_aNeighbors[6]]->m_aNeighbors[4]);
				}
				else if (aq31->m_aNeighbors[7] != -1)
				{
					if (gridCompareHeight(aq31->m_aNeighbors[7], aq->m_aNeighbors[6]))
						m_aQuads[aq31->m_aNeighbors[7]]->setNeighbor(1, aq->m_aNeighbors[6]);

					if (gridCompareHeight(aq->m_aNeighbors[6], aq31->m_aNeighbors[7]))
						aq32->setNeighbor(6, aq31->m_aNeighbors[7]);
				}
			}

			//если у обрабатываемого нода есть нижний правый
			if (aq->m_aNeighbors[7] != -1)
			{
				if (gridCompareHeight(aq->m_aNeighbors[6], aq->m_aNeighbors[7]))
					aq32->setNeighbor(4, aq->m_aNeighbors[7]);

				if (gridCompareHeight(aq->m_aNeighbors[7], aq->m_aNeighbors[6]))
					aq33->setNeighbor(3, aq->m_aNeighbors[6]);

				if (aq33->m_aNeighbors[6] != -1)
				{
					if (gridCompareHeight(aq->m_aNeighbors[7], aq->m_aNeighbors[6]))
						m_aQuads[aq33->m_aNeighbors[6]]->setNeighbor(0, aq->m_aNeighbors[6]);

					if (gridCompareHeight(aq->m_aNeighbors[6], aq33->m_aNeighbors[6]))
						aq32->setNeighbor(7, aq33->m_aNeighbors[6]);
				}

				if (aq32->m_aNeighbors[6] == -1)
				{
					if (aq33->m_aNeighbors[6] != -1 && m_aQuads[aq33->m_aNeighbors[6]]->m_aNeighbors[3] != -1)
					{
						if (gridCompareHeight(m_aQuads[aq33->m_aNeighbors[6]]->m_aNeighbors[3], aq->m_aNeighbors[6]))
							m_aQuads[m_aQuads[aq33->m_aNeighbors[6]]->m_aNeighbors[3]]->setNeighbor(1, aq->m_aNeighbors[6]);

						if (gridCompareHeight(aq->m_aNeighbors[6], m_aQuads[aq33->m_aNeighbors[6]]->m_aNeighbors[3]))
							aq32->setNeighbor(6, m_aQuads[aq33->m_aNeighbors[6]]->m_aNeighbors[3]);
					}
					else if (aq33->m_aNeighbors[5] != -1)
					{
						if (gridCompareHeight(aq33->m_aNeighbors[5], aq->m_aNeighbors[6]))
							m_aQuads[aq33->m_aNeighbors[5]]->setNeighbor(1, aq->m_aNeighbors[6]);

						if (gridCompareHeight(aq->m_aNeighbors[6], aq33->m_aNeighbors[5]))
							aq32->setNeighbor(6, aq33->m_aNeighbors[5]);
					}
				}
			}
		}


		//если создали левый нод
		if (aq->m_aNeighbors[3] != -1)
		{
			//указываем самый правый соседом
			//aq21->m_aNeighbors[4] = aq->m_id;
			aq21->setNeighbor(4, aq->m_id);
			//если верхний создан то он тоже сосед
			if (aq->m_aNeighbors[1] != -1)
				aq21->setNeighbor(2, aq->m_aNeighbors[1]);

			//если верхний создан то он тоже сосед
			if (aq->m_aNeighbors[6] != -1)
				aq21->setNeighbor(7, aq->m_aNeighbors[6]);

			if (aq->m_aNeighbors[0] != -1)
			{
				if (gridCompareHeight(aq->m_aNeighbors[0], aq->m_aNeighbors[3]))
					aq11->setNeighbor(6, aq->m_aNeighbors[3]);

				if (gridCompareHeight(aq->m_aNeighbors[3], aq->m_aNeighbors[0]))
					aq21->setNeighbor(1, aq->m_aNeighbors[0]);

				if (aq11->m_aNeighbors[3] != -1)
				{
					if (gridCompareHeight(aq11->m_aNeighbors[3], aq->m_aNeighbors[3]))
						m_aQuads[aq11->m_aNeighbors[3]]->setNeighbor(7, aq->m_aNeighbors[3]);

					if (gridCompareHeight(aq->m_aNeighbors[3], aq11->m_aNeighbors[3]))
						aq21->setNeighbor(0, aq11->m_aNeighbors[3]);
				}

				if (aq11->m_aNeighbors[3] != -1 && m_aQuads[aq11->m_aNeighbors[3]]->m_aNeighbors[6] != -1)
				{
					if (gridCompareHeight(aq->m_aNeighbors[3], m_aQuads[aq11->m_aNeighbors[3]]->m_aNeighbors[6]))
						aq21->setNeighbor(3, m_aQuads[aq11->m_aNeighbors[3]]->m_aNeighbors[6]);

					if (gridCompareHeight(m_aQuads[aq11->m_aNeighbors[3]]->m_aNeighbors[6], aq->m_aNeighbors[3]))
						m_aQuads[m_aQuads[aq11->m_aNeighbors[3]]->m_aNeighbors[6]]->setNeighbor(4, aq->m_aNeighbors[3]);
				}
				else if (aq11->m_aNeighbors[5] != -1)
				{
					if (gridCompareHeight(aq->m_aNeighbors[3], aq11->m_aNeighbors[5]))
						aq21->setNeighbor(3, aq11->m_aNeighbors[5]);

					if (gridCompareHeight(aq11->m_aNeighbors[5], aq->m_aNeighbors[3]))
						m_aQuads[aq11->m_aNeighbors[5]]->setNeighbor(4, aq->m_aNeighbors[3]);
				}
			}

			if (aq->m_aNeighbors[5] != -1)
			{
				if (gridCompareHeight(aq->m_aNeighbors[5], aq->m_aNeighbors[3]))
					aq31->setNeighbor(1, aq->m_aNeighbors[3]);

				if (gridCompareHeight(aq->m_aNeighbors[3], aq->m_aNeighbors[5]))
					aq21->setNeighbor(6, aq->m_aNeighbors[5]);

				if (aq31->m_aNeighbors[3] != -1)
				{
					if (gridCompareHeight(aq31->m_aNeighbors[3], aq->m_aNeighbors[3]))
						m_aQuads[aq31->m_aNeighbors[3]]->setNeighbor(2, aq->m_aNeighbors[3]);

					if (gridCompareHeight(aq->m_aNeighbors[3], aq31->m_aNeighbors[3]))
						aq21->setNeighbor(5, aq31->m_aNeighbors[3]);
				}

				if (aq21->m_aNeighbors[3] == -1)
				{
					if (aq31->m_aNeighbors[3] != -1 && m_aQuads[aq31->m_aNeighbors[3]]->m_aNeighbors[1] != -1)
					{
						if (gridCompareHeight(m_aQuads[aq31->m_aNeighbors[3]]->m_aNeighbors[1], aq->m_aNeighbors[3]))
							m_aQuads[m_aQuads[aq31->m_aNeighbors[3]]->m_aNeighbors[1]]->setNeighbor(4, aq->m_aNeighbors[3]);

						if (gridCompareHeight(aq->m_aNeighbors[3], m_aQuads[aq31->m_aNeighbors[3]]->m_aNeighbors[1]))
							aq21->setNeighbor(3, m_aQuads[aq31->m_aNeighbors[3]]->m_aNeighbors[1]);
					}
					else if (aq31->m_aNeighbors[0] != -1)
					{
						if (gridCompareHeight(aq31->m_aNeighbors[0], aq->m_aNeighbors[3]))
							m_aQuads[aq31->m_aNeighbors[0]]->setNeighbor(4, aq->m_aNeighbors[3]);

						if (gridCompareHeight(aq->m_aNeighbors[3], aq31->m_aNeighbors[0]))
							aq21->setNeighbor(3, aq31->m_aNeighbors[0]);
					}
				}
			}
		}

		//если создали правый нод
		if (aq->m_aNeighbors[4] != -1)
		{
			//указываем самый левый соседом
			aq23->setNeighbor(3, aq->m_id);
			//если верхний создан то он тоже сосед
			if (aq->m_aNeighbors[1] != -1)
				aq23->setNeighbor(0, aq->m_aNeighbors[1]);

			//если нижний создан то он тоже сосед
			if (aq->m_aNeighbors[6] != -1)
				aq23->setNeighbor(5, aq->m_aNeighbors[6]);


			if (aq->m_aNeighbors[2] != -1)
			{
				if (gridCompareHeight(aq->m_aNeighbors[2], aq->m_aNeighbors[4]))
					aq13->setNeighbor(6, aq->m_aNeighbors[4]);

				if (gridCompareHeight(aq->m_aNeighbors[4], aq->m_aNeighbors[2]))
					aq23->setNeighbor(1, aq->m_aNeighbors[2]);

				if (aq13->m_aNeighbors[4] != -1)
				{
					if (gridCompareHeight(aq->m_aNeighbors[4], aq13->m_aNeighbors[4]))
						aq23->setNeighbor(2, aq13->m_aNeighbors[4]);

					if (gridCompareHeight(aq13->m_aNeighbors[4], aq->m_aNeighbors[4]))
						m_aQuads[aq13->m_aNeighbors[4]]->setNeighbor(5, aq->m_aNeighbors[4]);
				}


				if (aq13->m_aNeighbors[4] != -1 && m_aQuads[aq13->m_aNeighbors[4]]->m_aNeighbors[6] != -1)
				{
					if (gridCompareHeight(aq->m_aNeighbors[4], m_aQuads[aq13->m_aNeighbors[4]]->m_aNeighbors[6]))
						aq23->setNeighbor(4, m_aQuads[aq13->m_aNeighbors[4]]->m_aNeighbors[6]);

					if (gridCompareHeight(m_aQuads[aq13->m_aNeighbors[4]]->m_aNeighbors[6], aq->m_aNeighbors[4]))
						m_aQuads[m_aQuads[aq13->m_aNeighbors[4]]->m_aNeighbors[6]]->setNeighbor(3, aq->m_aNeighbors[4]);
				}
				else if (aq13->m_aNeighbors[7] != -1)
				{
					if (gridCompareHeight(aq->m_aNeighbors[4], aq13->m_aNeighbors[7]))
						aq23->setNeighbor(4, aq13->m_aNeighbors[7]);

					if (gridCompareHeight(aq13->m_aNeighbors[7], aq->m_aNeighbors[4]))
						m_aQuads[aq13->m_aNeighbors[7]]->setNeighbor(3, aq->m_aNeighbors[4]);
				}
			}

			if (aq->m_aNeighbors[7] != -1)
			{
				if (gridCompareHeight(aq->m_aNeighbors[7], aq->m_aNeighbors[4]))
					aq33->setNeighbor(1, aq->m_aNeighbors[4]);

				if (gridCompareHeight(aq->m_aNeighbors[4], aq->m_aNeighbors[7]))
					aq23->setNeighbor(6, aq->m_aNeighbors[7]);

				if (aq33->m_aNeighbors[4] != -1)
				{
					if (gridCompareHeight(aq->m_aNeighbors[4], aq33->m_aNeighbors[4]))
						aq23->setNeighbor(7, aq33->m_aNeighbors[4]);

					if (gridCompareHeight(aq33->m_aNeighbors[4], aq->m_aNeighbors[4]))
						m_aQuads[aq33->m_aNeighbors[4]]->setNeighbor(0, aq->m_aNeighbors[4]);
				}

				if (aq23->m_aNeighbors[4] == -1)
				{
					if (aq33->m_aNeighbors[4] != -1 && m_aQuads[aq33->m_aNeighbors[4]]->m_aNeighbors[1] != -1)
					{
						if (gridCompareHeight(aq->m_aNeighbors[4], m_aQuads[aq33->m_aNeighbors[4]]->m_aNeighbors[1]))
							aq23->setNeighbor(4, m_aQuads[aq33->m_aNeighbors[4]]->m_aNeighbors[1]);

						if (gridCompareHeight(m_aQuads[aq33->m_aNeighbors[4]]->m_aNeighbors[1], aq->m_aNeighbors[4]))
							m_aQuads[m_aQuads[aq33->m_aNeighbors[4]]->m_aNeighbors[1]]->setNeighbor(3, aq->m_aNeighbors[4]);
					}
					else if (aq33->m_aNeighbors[2] != -1)
					{
						if (gridCompareHeight(aq->m_aNeighbors[4], aq33->m_aNeighbors[2]))
							aq23->setNeighbor(4, aq33->m_aNeighbors[2]);

						if (gridCompareHeight(aq33->m_aNeighbors[2], aq->m_aNeighbors[4]))
							m_aQuads[aq33->m_aNeighbors[2]]->setNeighbor(3, aq->m_aNeighbors[4]);
					}
				}
			}
		}
	}
}

ID CAIGrid::quadGet(const float3* pos, bool isnear_or_permissible) const
{
	if (!m_pTreeBound)
		return -1;

	long heightmin = -1;
	ID howf = -1;
	long tmpx = AIGRID_TOINT(pos->x);
	long tmpy = AIGRID_TOINT(pos->y);
	long tmpz = AIGRID_TOINT(pos->z);
	CBoundAIquad *bound = 0;

	int3 tiv;

	/*for (int i = 1, il = m_aBound.size(); i<il; ++i)
	{
		bound = m_aBound[i];
		if (
			m_aBound[i]->m_i3Min.x - LAIGRID_QUAD_SIZE <= tmpx && m_aBound[i]->m_i3Min.z - LAIGRID_QUAD_SIZE <= tmpz &&
			m_aBound[i]->m_i3Max.x + LAIGRID_QUAD_SIZE >= tmpx && m_aBound[i]->m_i3Max.z + LAIGRID_QUAD_SIZE >= tmpz
			)
		{
			for (int k = 0, kl = m_aBound[i]->m_aIdsQuads.size(); k<kl; ++k)
			{
				tiv = m_aLongCoordQuads[m_aBound[i]->m_aIdsQuads[k]];
				if (
					abs((tiv.x) - (tmpx)) <= LAIGRID_QUAD_SIZE &&
					abs((tiv.z) - (tmpz)) <= LAIGRID_QUAD_SIZE
					)
	{

					CAIquad *pQuad = m_aQuads[bound->m_aIdsQuads[k]];
		if (
						(
						abs((pQuad->m_vPos.x) - (pos->x)) <= AIGRID_QUAD_SIZEDIV2 &&
						abs((pQuad->m_vPos.z) - (pos->z)) <= AIGRID_QUAD_SIZEDIV2
						)
						&&
						(isnear_or_permissible && ((heightmin > -1 && heightmin > abs((tiv.y) - (tmpy))) || heightmin == -1)) ||
						(!isnear_or_permissible && (abs((tiv.y) - (tmpy)) <= LAIGRID_QUADS_CENTERS_MAXHEIGHT))
			)
		{
						//int3 l3 = m_aLongCoordQuads[m_aBound[i]->m_aIdsQuads[k]];
						//CAIquad* aq22 = m_aQuads[m_aBound[i]->m_aIdsQuads[k]];
						heightmin = abs(abs(m_aLongCoordQuads[m_aBound[i]->m_aIdsQuads[k]].y) - abs(tmpy));
						howf = m_aBound[i]->m_aIdsQuads[k];
					}
				}
			}
		}
	}*/

	//LibReport(REPORT_MSG_LEVEL_NOTICE, "quadGet 0\n");

	Array<CTreeBoundAIquad*, 64> queue;
	int iCurrKey = 0;
	queue.push_back(m_pTreeBound);
	CTreeBoundAIquad *pFoundSplit = 0;

	while (queue.size() > iCurrKey)
	{
		CTreeBoundAIquad *pSplit = queue[iCurrKey];

		for (int i = 0; i<4; i++)
			{
				if (
				/*pSplit->m_aSplits[i]->m_i3Min.x - LAIGRID_QUAD_SIZE <= tmpx && pSplit->m_aSplits[i]->m_i3Min.z - LAIGRID_QUAD_SIZE <= tmpz &&
				pSplit->m_aSplits[i]->m_i3Max.x + LAIGRID_QUAD_SIZE >= tmpx && pSplit->m_aSplits[i]->m_i3Max.z + LAIGRID_QUAD_SIZE >= tmpz*/
				pSplit->m_aSplits[i]->m_i3Min.x <= tmpx && pSplit->m_aSplits[i]->m_i3Min.y <= tmpy && pSplit->m_aSplits[i]->m_i3Min.z <= tmpz &&
				pSplit->m_aSplits[i]->m_i3Max.x >= tmpx && pSplit->m_aSplits[i]->m_i3Max.y >= tmpy && pSplit->m_aSplits[i]->m_i3Max.z >= tmpz
					)
				{
				if (pSplit->m_aSplits[i]->m_aSplits[0])
					queue.push_back(pSplit->m_aSplits[i]);
				else
				{
					pFoundSplit = pSplit->m_aSplits[i];
					/*queue.clear();
					break;*/

					for (int j = 0, jl = pFoundSplit->m_pBounds.size(); j < jl; ++j)
					{
						bound = pFoundSplit->m_pBounds[j];
						if (
							bound->m_aIdsQuads.size() > 0 &&
							/*bound->m_i3Min.x <= tmpx && bound->m_i3Min.z <= tmpz &&
							bound->m_i3Max.x >= tmpx && bound->m_i3Max.z >= tmpz*/
							bound->m_i3Min.x - LAIGRID_QUAD_SIZE <= tmpx && bound->m_i3Min.z - LAIGRID_QUAD_SIZE <= tmpz &&
							bound->m_i3Max.x + LAIGRID_QUAD_SIZE >= tmpx && bound->m_i3Max.z + LAIGRID_QUAD_SIZE >= tmpz
							)
						{
							for (int k = 0, kl = bound->m_aIdsQuads.size(); k<kl; ++k)
							{
								tiv = m_aLongCoordQuads[bound->m_aIdsQuads[k]];
								if (
									abs((tiv.x) - (tmpx)) <= LAIGRID_QUAD_SIZE &&
									abs((tiv.z) - (tmpz)) <= LAIGRID_QUAD_SIZE
									)
								{
									CAIquad *pQuad = m_aQuads[bound->m_aIdsQuads[k]];
					if (
										(
										abs((pQuad->m_vPos.x) - (pos->x)) <= AIGRID_QUAD_SIZEDIV2 &&
										abs((pQuad->m_vPos.z) - (pos->z)) <= AIGRID_QUAD_SIZEDIV2
										)
										&&
										(isnear_or_permissible && ((heightmin > -1 && heightmin > abs((tiv.y) - (tmpy))) || heightmin == -1)) ||
										(!isnear_or_permissible && (abs((tiv.y) - (tmpy)) <= LAIGRID_QUADS_CENTERS_MAXHEIGHT))
						)
					{
										//int3 l3 = m_aLongCoordQuads[m_aBound[i]->m_aIdsQuads[k]];
										//CAIquad* aq22 = m_aQuads[m_aBound[i]->m_aIdsQuads[k]];
										heightmin = abs(abs(tiv.y) - abs(tmpy));
										howf = bound->m_aIdsQuads[k];
									}
								}
							}
						}
					}
				}
			}
		}

		//queue.erase(0);
		++iCurrKey;
					}

	if (!pFoundSplit)
		return -1;

	//LibReport(REPORT_MSG_LEVEL_NOTICE, "quadGet 1\n");

	/*for (int j = 0, jl = pFoundSplit->m_pBounds.size(); j < jl; ++j)
	{
		bound = pFoundSplit->m_pBounds[j];
		if (
			bound->m_i3Min.x <= tmpx && bound->m_i3Min.y <= tmpy && bound->m_i3Min.z <= tmpz &&
			bound->m_i3Max.x >= tmpx && bound->m_i3Max.y >= tmpy && bound->m_i3Max.z >= tmpz
			)
		{
			for (int k = 0, kl = bound->m_aIdsQuads.size(); k<kl; ++k)
			{
				tiv = m_aLongCoordQuads[bound->m_aIdsQuads[k]];
				if (
					abs((tiv.x) - (tmpx)) <= LAIGRID_QUAD_SIZE &&
					abs((tiv.z) - (tmpz)) <= LAIGRID_QUAD_SIZE
					)
				{
					CAIquad *pQuad = m_aQuads[bound->m_aIdsQuads[k]];
					if (
						(
						abs((pQuad->m_vPos.x) - (pos->x)) <= AIGRID_QUAD_SIZEDIV2 &&
						abs((pQuad->m_vPos.z) - (pos->z)) <= AIGRID_QUAD_SIZEDIV2
						)
						&&
						(isnear_or_permissible && ((heightmin > -1 && heightmin > abs((tiv.y) - (tmpy))) || heightmin == -1)) ||
						(!isnear_or_permissible && (abs((tiv.y) - (tmpy)) <= LAIGRID_QUADS_CENTERS_MAXHEIGHT))
						)
					{
						//int3 l3 = m_aLongCoordQuads[m_aBound[i]->m_aIdsQuads[k]];
						//CAIquad* aq22 = m_aQuads[m_aBound[i]->m_aIdsQuads[k]];
						heightmin = abs(abs(tiv.y) - abs(tmpy));
						howf = bound->m_aIdsQuads[k];
				}
			}
		}
	}
	}*/

	return howf;
}

bool CAIGrid::quadGetPos(ID id, float3* pos) const
{
	if (pos && id >= 0 && id < m_aQuads.size())
	{
		*pos = m_aQuads[id]->m_vPos;
		return false;
	}

	return false;
}

ID CAIGrid::gridTraceBeam(const float3* start, const float3* dir) const
{
	ID idf = -1;
	CTriangle tmptri;
	float3 ip;
	float3 res;
	float3 ildir;
	res = (*start) + float3(10000.0f, 10000.0f, 10000.0f);
	ildir = (*dir) * 10000.0f;

	float3 center;
	float radius;
	CBoundAIquad* bound = 0;
	float3 posquad, posquad2;

	for (int i = 0, il = m_aBound.size(); i < il; ++i)
	{
		bound = m_aBound[i];
		center = (bound->m_f3Min + bound->m_f3Max) * 0.5f;
		radius = SMVector3Length(center - bound->m_f3Max);
		float distsqr = SGCore_0DistancePointBeam2(center, *start, *dir);
		if (distsqr <= radius*radius)
		{
			for (int k = 0, kl = bound->m_aIdsQuads.size(); k < kl; ++k)
			{
				posquad = (float3)m_aQuads[bound->m_aIdsQuads[k]]->m_vPos;
				tmptri.m_vA = (float3_t)(posquad + float3(-AIGRID_QUAD_SIZEDIV2, 0, -AIGRID_QUAD_SIZEDIV2));
				tmptri.m_vB = (float3_t)(posquad + float3(AIGRID_QUAD_SIZEDIV2, 0, AIGRID_QUAD_SIZEDIV2));
				tmptri.m_vC = (float3_t)(posquad + float3(AIGRID_QUAD_SIZEDIV2, 0, -AIGRID_QUAD_SIZEDIV2));
				if (tmptri.IntersectLine((*start), ildir, &ip))
				{
					if (SMVector3Length2((*start) - res) > SMVector3Length2((*start) - ip))
					{
						res = ip;
						idf = bound->m_aIdsQuads[k];
					}
				}

				tmptri.m_vA = (float3_t)(posquad + float3(-AIGRID_QUAD_SIZEDIV2, 0, -AIGRID_QUAD_SIZEDIV2));
				tmptri.m_vB = (float3_t)(posquad + float3(-AIGRID_QUAD_SIZEDIV2, 0, AIGRID_QUAD_SIZEDIV2));
				tmptri.m_vC = (float3_t)(posquad + float3(AIGRID_QUAD_SIZEDIV2, 0, AIGRID_QUAD_SIZEDIV2));
				if (tmptri.IntersectLine((*start), ildir, &ip))
				{
					if (SMVector3Length2((*start) - res) > SMVector3Length2((*start) - ip))
					{
						res = ip;
						idf = bound->m_aIdsQuads[k];
					}
				}
			}
		}
	}

	return idf;
}


//!!! (возможно неактуально) почему то возникла ситуация когда после прохода валидации, у одного (наверное у одного) из квадов айди сплита был == 0, чего в общемто не должно быть
//возможно надо просто проходится циклом по всему массиву квадов и смотреть где у какого квада айди сплита еще равен нулю, а перед этим проходом обнулить все эти айди

void CAIGrid::gridTestValidation()
{
	if (m_aQuads.size() <= 1 || m_aBound.size() <= 1)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "AI grid is not created, unresolved validation\n");
		return;
	}

	int oldcountquads = m_aQuads.size();

	//удаление лишних квадов
	quadDeleteInvalidAll();

	//подсчет и пометка сплитов
	//{

	for (int i = 0, il = m_aQuads.size(); i < il; ++i)
	{
		m_aQuads[i]->m_iSplit = 0;
	}

	m_aTestSplitsIDs.clear();
	m_uiCountSplits = 0;
	
	while (true)
	{
		ID idquadunchecked = -1;
		CAIquad *quad = 0;
		//поиск первого не входящего ни в какой сплит
		for (int i = 0, il = m_aQuads.size(); i < il; ++i)
		{
			if (m_aQuads[i]->m_iSplit == 0 && !m_aQuads[i]->m_isClose)
			{
				idquadunchecked = i;
				quad = m_aQuads[i];
				break;
			}
		}

		//не входящие в какой-либо сплит не найдены
		if (idquadunchecked < 0)
			break;

		ID currkey = 0;
		ID tmpid = -1;
		m_aTestSplitsIDs.clear();
		m_aTestSplitsIDs[0] = idquadunchecked;
		++m_uiCountSplits;

		Array<bool> aPushed;
		aPushed.resize(m_aQuads.size());
		memset(&(aPushed[0]), 0, sizeof(bool)* aPushed.size());

		//начиная с первого невходящего квада проходимся по всем узлам к которым можно дойти, относя их к текущему идентификатору сплита
		while (m_aTestSplitsIDs.size() > currkey)
			{
			tmpid = m_aTestSplitsIDs[currkey];
			
			m_aQuads[tmpid]->m_iSplit = m_uiCountSplits;

				//заносим всех соседей в непомеченный список
				for (int i = 0; i < 8; ++i)
				{
				if (isValidIdInArrQuads(m_aQuads[tmpid]->m_aNeighbors[i]) && m_aQuads[m_aQuads[tmpid]->m_aNeighbors[i]]->m_iSplit == 0 && aPushed[m_aQuads[tmpid]->m_aNeighbors[i]] == false)
				{
					m_aTestSplitsIDs.push_back(m_aQuads[tmpid]->m_aNeighbors[i]);
					aPushed[m_aQuads[tmpid]->m_aNeighbors[i]] = true;
				}
			}
			
			++currkey;
		}

		int qwerty = 0;
	}

	for (int i = 0; i < m_uiCountSplits; ++i)
	{
		m_aSplitsColor[i] = D3DCOLOR_ARGB(200, rand() % 255, rand() % 255, rand() % 255);
	}
	//}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "AI grid is validation, deleted quads %d, count splits %d\n", oldcountquads - m_aQuads.size(), m_uiCountSplits);
}

UINT CAIGrid::gridGetCountSplits() const
{
	return m_uiCountSplits;
}

void CAIGrid::gridSetMarkSplits(bool mark)
{
	m_isMarkSplits = mark;
}

bool CAIGrid::gridGetMarkSplits() const
{
	return m_isMarkSplits;
}

void CAIGrid::renderBB()
{
	if (!m_useGraphics)
		return;

	m_pDXDevice->SetTexture(0, SGCore_LoadTexGetTex(m_idTexBB));
	if (m_pBoundBox)
		m_pBoundBox->DrawSubset(0);
}

void CAIGrid::renderQuads(const IFrustum * frustum, const float3 * viewpos, float dist)
{
	if (!m_useGraphics)
		return;

	if (!frustum)
		return;

	if (m_pTransVertBuf)
	{
		CAIQuadVertexDataInst* RTGPUArrVerteces;
		m_pTransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		UINT tmpkey = 0;
		CBoundAIquad * bound = 0;
		CAIquad * aq = 0;
		float3 center;
		float radius;

		for (int i = 0, il = m_aBound.size(); i < il; ++i)
		{
			bound = m_aBound[i];
			center = (bound->m_f3Min + bound->m_f3Max) * 0.5f;
			radius = SMVector3Length(center - bound->m_f3Max);
			if (frustum->sphereInFrustum(&center, radius))
			{
				for (int k = 0, kl = bound->m_aIdsQuads.size(); k < kl; ++k)
				{
					aq = m_aQuads[bound->m_aIdsQuads[k]];
					if (SMVector3Distance(aq->m_vPos, (*viewpos)) > dist)
						continue;

					RTGPUArrVerteces[tmpkey].m_vPos = aq->m_vPos;

					if (quadSelectedExists(aq->m_id))
						RTGPUArrVerteces[tmpkey].m_uiColor = AIGRID_COLOR_SEL_MAIN;
					else if (quadSelectedExistsNeightborDirect(aq->m_id))
						RTGPUArrVerteces[tmpkey].m_uiColor = AIGRID_COLOR_SEL_ADD;
					else if (m_uiCountSplits > 0 && m_isMarkSplits)
						RTGPUArrVerteces[tmpkey].m_uiColor = (m_aSplitsColor[aq->m_iSplit - 1]);
					else
						RTGPUArrVerteces[tmpkey].m_uiColor = m_aQuadColors[aq->m_id];

					/*if (aq->m_iSplit > 1)
					{
						int qwerty = 0;
						RTGPUArrVerteces[tmpkey].pos.y += 0.1;
						RTGPUArrVerteces[tmpkey].m_uiColor = D3DCOLOR_ARGB(128, 255, 0, 0);
					}*/

					if (aq->m_chAxisX == AIGRID_QUAD_DIR_POSITIVE && aq->m_chAxisY == AIGRID_QUAD_DIR_NEGATIVE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0, 0);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_ALL && aq->m_chAxisY == AIGRID_QUAD_DIR_NEGATIVE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.25f, 0);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_NEGATIVE && aq->m_chAxisY == AIGRID_QUAD_DIR_NEGATIVE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.5f, 0);

					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_POSITIVE && aq->m_chAxisY == AIGRID_QUAD_DIR_ALL)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0, 0.25f);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_ALL && aq->m_chAxisY == AIGRID_QUAD_DIR_ALL)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.25f, 0.25f);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_NEGATIVE && aq->m_chAxisY == AIGRID_QUAD_DIR_ALL)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.5f, 0.25f);

					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_POSITIVE && aq->m_chAxisY == AIGRID_QUAD_DIR_POSITIVE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0, 0.5f);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_ALL && aq->m_chAxisY == AIGRID_QUAD_DIR_POSITIVE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.25f, 0.5f);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_NEGATIVE && aq->m_chAxisY == AIGRID_QUAD_DIR_POSITIVE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.5f, 0.5f);

					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_NEGATIVE && aq->m_chAxisY == AIGRID_QUAD_DIR_NONE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.25f, 0.75f);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_POSITIVE && aq->m_chAxisY == AIGRID_QUAD_DIR_NONE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.5f, 0.75f);

					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_NONE && aq->m_chAxisY == AIGRID_QUAD_DIR_NEGATIVE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.75f, 0.75f);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_NONE && aq->m_chAxisY == AIGRID_QUAD_DIR_POSITIVE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.75f, 0.5f);

					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_ALL && aq->m_chAxisY == AIGRID_QUAD_DIR_NONE)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.75f, 0.25f);
					else if (aq->m_chAxisX == AIGRID_QUAD_DIR_NONE && aq->m_chAxisY == AIGRID_QUAD_DIR_ALL)
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.75f, 0);

					else
					{
						RTGPUArrVerteces[tmpkey].m_vTC = float4_t(1, 1, 0, 0);
						RTGPUArrVerteces[tmpkey].m_uiColor = D3DCOLOR_ARGB(255, 255, 0, 0);
					}

					++tmpkey;
				}
			}
		}

		m_pTransVertBuf->Unlock();

		m_pDXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | tmpkey));

		m_pDXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		m_pDXDevice->SetStreamSource(1, m_pTransVertBuf, 0, sizeof(CAIQuadVertexDataInst));

		m_pDXDevice->SetStreamSource(0, m_pVertexQuad, 0, sizeof(CAIQuadVertexData));
		m_pDXDevice->SetIndices(m_pIndexQuad);
		m_pDXDevice->SetVertexDeclaration(m_pDecl);

		m_pDXDevice->SetTexture(0, SGCore_LoadTexGetTex(m_idTex));

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, m_idVS);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, m_idPS);

		D3DXMATRIX tmpview, tmpproj;
		m_pDXDevice->GetTransform(D3DTS_VIEW, &tmpview);
		m_pDXDevice->GetTransform(D3DTS_PROJECTION, &tmpproj);

		float4x4 wvp = SMMatrixIdentity() * float4x4(tmpview) * float4x4(tmpproj);
		wvp = SMMatrixTranspose(wvp);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "g_mWVP", &wvp);

		m_pDXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		SGCore_ShaderUnBind();

		m_pDXDevice->SetStreamSourceFreq(0, 1);
		m_pDXDevice->SetStreamSourceFreq(1, 1);
	}
}

void CAIGrid::renderGraphPoints(const float3 * viewpos, float dist)
{
	if (!m_useGraphics)
		return;

	if (m_pTransVertBuf)
	{
		CAIQuadVertexDataInst* RTGPUArrVerteces;
		m_pTransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		UINT tmpkey = 0;
		CAIquad* aq = 0;
		for (UINT i = 0; i<m_aGraphPointsIDs.size(); ++i)
		{
			aq = m_aQuads[m_aGraphPointsIDs[i]];
			if (!(aq->m_isClose) && SMVector3Distance(aq->m_vPos, (*viewpos)) <= dist)
			{
				RTGPUArrVerteces[tmpkey].m_vPos = aq->m_vPos;
				//RTGPUArrVerteces[tmpkey].pos.y += 0.1f;

				if (aq->m_chAxisX == 1 && aq->m_chAxisY == -1)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0, 0);
				else if (aq->m_chAxisX == 0 && aq->m_chAxisY == -1)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.25f, 0);
				else if (aq->m_chAxisX == -1 && aq->m_chAxisY == -1)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.5f, 0);

				else if (aq->m_chAxisX == 1 && aq->m_chAxisY == 0)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0, 0.25f);
				else if (aq->m_chAxisX == 0 && aq->m_chAxisY == 0)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.25f, 0.25f);
				else if (aq->m_chAxisX == -1 && aq->m_chAxisY == 0)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.5f, 0.25f);

				else if (aq->m_chAxisX == 1 && aq->m_chAxisY == 1)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0, 0.5f);
				else if (aq->m_chAxisX == 0 && aq->m_chAxisY == 1)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.25f, 0.5f);
				else if (aq->m_chAxisX == -1 && aq->m_chAxisY == 1)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.5f, 0.5f);

				else if (aq->m_chAxisX == -1 && aq->m_chAxisY == 2)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.25f, 0.75f);
				else if (aq->m_chAxisX == 1 && aq->m_chAxisY == 2)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.5f, 0.75f);

				else if (aq->m_chAxisX == 2 && aq->m_chAxisY == -1)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.75f, 0.75f);
				else if (aq->m_chAxisX == 2 && aq->m_chAxisY == 1)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.75f, 0.5f);

				else if (aq->m_chAxisX == 0 && aq->m_chAxisY == 2)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.75f, 0.25f);
				else if (aq->m_chAxisX == 2 && aq->m_chAxisY == 0)
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(0.25f, 0.25f, 0.75f, 0);

				else
					RTGPUArrVerteces[tmpkey].m_vTC = float4_t(1, 1, 0, 0);

				RTGPUArrVerteces[tmpkey].m_uiColor = D3DCOLOR_ARGB(200, 0, 0, 255);
				++tmpkey;
			}
		}

		m_pTransVertBuf->Unlock();

		m_pDXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | tmpkey));

		m_pDXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		m_pDXDevice->SetStreamSource(1, m_pTransVertBuf, 0, sizeof(CAIQuadVertexDataInst));

		m_pDXDevice->SetStreamSource(0, m_pVertexQuad, 0, sizeof(CAIQuadVertexData));
		m_pDXDevice->SetIndices(m_pIndexQuad);
		m_pDXDevice->SetVertexDeclaration(m_pDecl);

		m_pDXDevice->SetTexture(0, SGCore_LoadTexGetTex(m_idTex));

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, m_idVS);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, m_idPS);

		D3DXMATRIX tmpview, tmpproj;
		m_pDXDevice->GetTransform(D3DTS_VIEW, &tmpview);
		m_pDXDevice->GetTransform(D3DTS_PROJECTION, &tmpproj);

		float4x4 wvp = SMMatrixIdentity() * float4x4(tmpview) * float4x4(tmpproj);
		wvp = SMMatrixTranspose(wvp);

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, m_idVS, "WorldViewProjection", &wvp);

		m_pDXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		SGCore_ShaderUnBind();

		m_pDXDevice->SetStreamSourceFreq(0, 1);
		m_pDXDevice->SetStreamSourceFreq(1, 1);
	}
}

//#############################################################################

bool CAIGrid::IsOpen(ID id)
{
	for (int i = 0; i<m_aOpenIDs.size(); ++i)
	{
		if (m_aOpenIDs[i] == id)
			return true;
	}
	return false;
}

int CAIGrid::CalcHCost(ID id, ID endid)
{
	return (
		(
		abs(
		(m_aLongCoordQuads[id].x) - (m_aLongCoordQuads[endid].x)
		) / LAIGRID_QUAD_SIZE
		)

		) +

		(
		(
		abs(
		(m_aLongCoordQuads[id].z) - (m_aLongCoordQuads[endid].z)
		) / LAIGRID_QUAD_SIZE
		)
		*10
		);
}

int CAIGrid::CalcH2Cost(ID id1, ID id2)
{
	return SMVector2Length(float2(m_aQuads[id1]->m_vPos.x, m_aQuads[id1]->m_vPos.z) - float2(m_aQuads[id2]->m_vPos.x, m_aQuads[id2]->m_vPos.z)) * 100.f;
}

ID CAIGrid::AddInOpenList(ID id)
{
	for (int i = NumLastKeyOpenList; i<m_aOpenIDs.size(); i++)
	{
		if (m_aOpenIDs[i] == -1)
		{
			m_aOpenIDs[i] = id;
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

bool CAIGrid::quadIsFree(ID id, int radius)
{
	AIGRID_QUAD_PRECOND(id, false);

	if (m_aQuadState[id] == AIQUAD_STATE::AIQUAD_STATE_FREE)
	{
		if (radius == 1)
			return true;
		else if (radius > 1)
		{
			memset(&(m_aPreCondFreeState[0]), 0, m_aPreCondFreeState.size() * sizeof(bool));
			m_aPreCondFreeState[id] = true;
			return isFreeQuad2(id, radius - 1);
		}
	}
	return false;
}

bool CAIGrid::isFreeQuad2(ID id, int radius)
{
	AIGRID_QUAD_PRECOND(id, false);
	bool tmpisfree = true;
	
	CAIquad* aq = m_aQuads[id];
	
	if (radius > 0)
	{
		for (int i = 0; i<8; ++i)
		{
			if (aq->m_aNeighbors[i] >= 0 && m_aQuadState[aq->m_aNeighbors[i]] == AIQUAD_STATE::AIQUAD_STATE_FREE)
			{
				if (!m_aPreCondFreeState[aq->m_aNeighbors[i]])
				{
					m_aPreCondFreeState[aq->m_aNeighbors[i]] = true;
					if (!isFreeQuad2(aq->m_aNeighbors[i], radius - 1))
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

ID CAIGrid::quadGetNear(const float3 *pPos, float fRadius, bool isFree, int iQuadRadius)
{
	if (!m_pTreeBound)
		return -1;

	ID howf = -1;

	ID tmpsmall = -1;
	float tmpsmalldist = 0;
	float tmpsmalldist2 = 0;
	float tmpsmallqdist = 0;
	float tmpsmallqdist2 = 0;

	CBoundAIquad *tmparrbb[8];
	float tmparrdist2[8];

	for (int i = 0; i<8; ++i)
	{
		tmparrbb[i] = 0;
		tmparrdist2[i] = -1;
	}

	bool isexistsfree = false;
	CBoundAIquad* bound = 0;


	Array<CTreeBoundAIquad*, 64> queue;
	int iCurrKey = 0;
	queue.push_back(m_pTreeBound);
	
	Array<CTreeBoundAIquad*> aBounds;
	float3 vSphereCenter;
	float fSphereRadius;

	//LibReport(REPORT_MSG_LEVEL_NOTICE, "quadGetNear 0\n");

	while (queue.size() > iCurrKey)
	{
		CTreeBoundAIquad *pSplit = queue[iCurrKey];

		for (int i = 0; i<4; i++)
		{
			pSplit->m_aSplits[i]->m_pBoundBox->getSphere(&vSphereCenter, &fSphereRadius);
			vSphereCenter.y = 0;
			if (
				SMVector3Length(float3(pPos->x, 0, pPos->z) - vSphereCenter) <= (fRadius + fSphereRadius)
				)
	{
				if (pSplit->m_aSplits[i] && pSplit->m_aSplits[i]->m_aSplits[0])
					queue.push_back(pSplit->m_aSplits[i]);
				else
		{
					aBounds.push_back(pSplit->m_aSplits[i]);
				}
			}
		}

		//queue.erase(0);
		++iCurrKey;
		//LibReport(REPORT_MSG_LEVEL_NOTICE, "iCurrKey %d\n", iCurrKey);
	}

	for (int i = 0, il = aBounds.size(); i<il; ++i)
	{
		for (int k = 0, kl = aBounds[i]->m_pBounds.size(); k<kl; ++k)
		{
			bound = aBounds[i]->m_pBounds[k];
			if (bound->m_aIdsQuads.size() > 0 /*&& SMVector3Length2((*pPos) - ((bound->m_f3Max + bound->m_f3Min) * 0.5f)) <= fRadius*fRadius*/)
			{
			isexistsfree = false;
				if (!isFree)
				isexistsfree = true;
			else
			{
					for (int j = 0, jl = bound->m_aIdsQuads.size(); j < jl; ++j)
				{
						if (quadIsFree(bound->m_aIdsQuads[j], iQuadRadius))
					{
						isexistsfree = true;
						break;
					}
				}
			}

			if (isexistsfree)
			{
					tmpsmalldist2 = SMVector3Length2(((bound->m_f3Max + bound->m_f3Min) * 0.5f) - (*pPos));
				tmpsmall = -1;
					for (int j = 0; j < 8; ++j)
				{
						if (tmparrbb[j] == 0)
					{
							tmparrbb[j] = bound;
							tmparrdist2[j] = tmpsmalldist2;
						break;
					}

						if (tmpsmall == -1 || (tmpsmalldist2 < tmparrdist2[j] && tmpsmalldist > tmparrdist2[j] - tmpsmalldist2))
					{
							tmpsmalldist = tmparrdist2[j] - tmpsmalldist2;
							tmpsmall = j;
					}
				}

				if (tmpsmall != -1)
				{
						tmparrbb[tmpsmall] = bound;
					tmparrdist2[tmpsmall] = tmpsmalldist2;
				}
			}
		}
	}
	}

	//LibReport(REPORT_MSG_LEVEL_NOTICE, "quadGetNear 2\n");

	for (int i = 0; i<8; ++i)
	{
		if (tmparrbb[i] != 0)
		{
			bound = tmparrbb[i];
			for (int k = 0, kl = bound->m_aIdsQuads.size(); k<kl; ++k)
			{
				isexistsfree = true;
				tmpsmallqdist2 = SMVector3Length2(m_aQuads[bound->m_aIdsQuads[k]]->m_vPos - (*pPos));
				if ((howf == -1 || tmpsmallqdist2 < tmpsmallqdist) && (m_aQuadState[bound->m_aIdsQuads[k]] == 0 || !isFree))
				{
					if (!isFree)
					{
						tmpsmallqdist = tmpsmallqdist2;
						howf = bound->m_aIdsQuads[k];
					}
					else if (quadIsFree(bound->m_aIdsQuads[k], iQuadRadius))
					{
						tmpsmallqdist = tmpsmallqdist2;
						howf = bound->m_aIdsQuads[k];
					}
				}
			}
		}
	}

	return howf;
}

ID CAIGrid::graphPointGetNear(ID beginq, ID endq)
{
	if (m_aQuads.size() <= 1)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "AI grid not found\n");
		return -1;
	}

	AIGRID_QUAD_PRECOND(beginq, false);
	AIGRID_QUAD_PRECOND(endq, false);

	if (m_uiCountSplits == 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "can not find path, because AI grid is not validate splits\n");
		return -1;
	}

	if (m_aQuads[beginq]->m_iSplit != m_aQuads[endq]->m_iSplit)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "quads in different splits, path not found\n");
		return -1;
	}

	if (beginq == endq)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "beginq == endq, path not found\n");
		return -1;
	}

	if (m_aGraphPointsIDs.size() == 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "graph points not found\n");
		return -1;
	}
	
	if (CalcH2Cost(beginq, endq) > AIGRID_COST_GPATH)
	{
		if (m_aCostGraphPointsIDs.size() != m_aGraphPointsIDs.size())
			m_aCostGraphPointsIDs.resize(m_aGraphPointsIDs.size());

		memset(&(m_aCostGraphPointsIDs[0]), -1, m_aCostGraphPointsIDs.size() * sizeof(int32_t));

		long tmpcountstepbegin = 0;	//на сколько мы продвинулись от начальной точки до текущей точки графа
		long tmpcountdel = 0;		//на сколько короче стал путь
		DWORD tmpbigeff = -1;		//самая эффективная ближайшая точка
		long tmpcostbe = -1;
		long tmpcostagpe = -1;
		long tmpdist = AIGRID_COST_GPATH;

		while (tmpbigeff == -1)
		{
			for (int i = 0, il = m_aGraphPointsIDs.size(); i<il; ++i)
			{
				if (m_aGraphPointsIDs[i] != beginq && m_aGraphPointsIDs[i] != endq)
				{
					tmpcostbe = CalcH2Cost(beginq, endq);
					tmpcostagpe = CalcH2Cost(m_aGraphPointsIDs[i], endq);
					tmpcountdel = tmpcostbe - tmpcostagpe;
					if (tmpcountdel > 0 && tmpcountdel < tmpdist)
					{
						tmpcountstepbegin = CalcH2Cost(beginq, m_aGraphPointsIDs[i]);
						m_aCostGraphPointsIDs[i] = (tmpcountstepbegin - tmpcountdel);
						if (tmpbigeff == -1 || (m_aCostGraphPointsIDs[tmpbigeff] > m_aCostGraphPointsIDs[i]))
							tmpbigeff = i;
					}
				}
			}
			tmpdist *= 2;
		}

		if (tmpbigeff != -1 && CalcH2Cost(beginq, m_aGraphPointsIDs[tmpbigeff]) < CalcH2Cost(beginq, endq))
		{
			//m_aQuadColors[m_aGraphPointsIDs[tmpbigeff]] = 255;
			return m_aGraphPointsIDs[tmpbigeff];
		}
		return -1;
	}
	return -1;
}

bool CAIGrid::gridFindPath(ID idQueueObject)
{
	if (!existsQuads())
		return false;

	ID idStart = m_aQueueFind[idQueueObject].m_idStart;
	ID idFinish = m_aQueueFind[idQueueObject].m_idFinish;
	Array<ID> &aQuads = m_aQueueFind[idQueueObject].m_aQuads;
	aQuads.clearFast();

	int tmpcost = AIGRID_COST_DIAGONAL;
	int cycnum = 0;
	long tmpaddcost = 0;
	long tmphcost = 0;
	//обнуляем все данные

	CountInitOpen2 = 0;
	NumLastKeyOpenList = 0;
	//memset(&(m_aQuadColors[0]), 0, m_aQuadColors.size() * sizeof(uint32_t));
	memset(&(m_aIDsInOpen[0]), -1, m_aIDsInOpen.size() * sizeof(ID));
	memset(&(m_aOpenIDs[0]), -1, m_aOpenIDs.size() * sizeof(ID));
	memset(&(m_aParentIDs[0]), -1, m_aParentIDs.size() * sizeof(ID));
	memset(&(m_aCostQuads[0]), 0, m_aCostQuads.size() * sizeof(CCostTransit));
	memset(&(m_aTestSplitsIDs[0]), 0, m_aTestSplitsIDs.size() * sizeof(bool));
	//ArrPathIDs.clear();

	bool IsFindPath = false;

	//анализируем стоимости для всех соседних квадов и помещаем в открытый список
	//и назначаем им родителей стартовую точку

	for (cycnum = 0; cycnum<8; cycnum++)
	{
		if (
			m_aQuads[idStart]->m_aNeighbors[cycnum] != -1)
		{
			if (m_aQuads[idStart]->m_aNeighbors[cycnum] == idFinish)
			{
				aQuads.push_back(idFinish);
				aQuads.push_back(m_aQuads[idStart]->m_aNeighbors[cycnum]);
				return true;
			}
			tmpcost = AIGRID_COST_DIAGONAL;

			if (cycnum == 1 || cycnum == 3 || cycnum == 4 || cycnum == 6)
				tmpcost = AIGRID_COST_DIRECT;

			if (quadGetState(m_aQuads[idStart]->m_aNeighbors[cycnum]) != AIQUAD_STATE::AIQUAD_STATE_FREE)
			{
				if (quadGetState(m_aQuads[idStart]->m_aNeighbors[cycnum]) == AIQUAD_STATE::AIQUAD_STATE_TEMPBUSY)
					tmpcost += AIGRID_COSTADD_TEMPBUSY;
				else if (quadGetState(m_aQuads[idStart]->m_aNeighbors[cycnum]) == AIQUAD_STATE::AIQUAD_STATE_BUSY)
					tmpcost += AIGRID_COSTADD_BUSY;
			}
			else
				tmpaddcost = 0;

			m_aParentIDs[m_aQuads[idStart]->m_aNeighbors[cycnum]] = idStart;
			m_aCostQuads[m_aQuads[idStart]->m_aNeighbors[cycnum]].G = tmpcost;
			m_aCostQuads[m_aQuads[idStart]->m_aNeighbors[cycnum]].H = CalcHCost(m_aQuads[idStart]->m_aNeighbors[cycnum], idFinish);

			m_aCostQuads[m_aQuads[idStart]->m_aNeighbors[cycnum]].F = m_aCostQuads[m_aQuads[idStart]->m_aNeighbors[cycnum]].G + m_aCostQuads[m_aQuads[idStart]->m_aNeighbors[cycnum]].H + tmpaddcost;

			m_aIDsInOpen[m_aQuads[idStart]->m_aNeighbors[cycnum]] = AddInOpenList(m_aQuads[idStart]->m_aNeighbors[cycnum]);
		}
	}

	//стартовую точку помещаем в закрытый
	m_aTestSplitsIDs[idStart] = true;

	bool isfind = false;
	ID tmpsmallF = -1;
	bool issmall = true;

	DWORD ttime = GetTickCount();
	ID tmpcurrsmall = -1;
	ID tmpcurridquad = -1;

	CAIquad* tmpquad = 0;

	while (!IsFindPath)
	{
		isfind = false;
		tmpsmallF = -1;

		//находим квад с наименьшей стоимостью F

		tmpcurrsmall = -1;
		for (cycnum = 0; cycnum<CountInitOpen2; ++cycnum)
		{
			if (m_aOpenIDs[cycnum] != -1)
			{
				if (tmpcurrsmall != -1)
				{
					if (m_aCostQuads[m_aOpenIDs[tmpcurrsmall]].F > m_aCostQuads[m_aOpenIDs[cycnum]].F)
						tmpcurrsmall = cycnum;
				}
				else
					tmpcurrsmall = cycnum;
			}
		}

		if (tmpcurrsmall != -1)
			tmpsmallF = m_aOpenIDs[tmpcurrsmall];

		//анализируем соседей квада на наличие их в открытом списке
		if (tmpsmallF != -1)
		{
			tmpquad = m_aQuads[tmpsmallF];
			
			for (cycnum = 0; cycnum<8; ++cycnum)
			{
				tmpcurridquad = tmpquad->m_aNeighbors[cycnum];
				//если квад существует
				if (tmpcurridquad != -1)
				{
					if (!m_aTestSplitsIDs[tmpcurridquad])
					{
						//если сосденяя точка конечная точка то мы нашли путь!!!
						if (tmpcurridquad == idFinish)
						{
							isfind = true;
							break;
						}
					
						tmpcost = AIGRID_COST_DIAGONAL;
						if (cycnum == 1 || cycnum == 3 || cycnum == 4 || cycnum == 6)
							tmpcost = AIGRID_COST_DIRECT;

						tmphcost = CalcHCost(tmpcurridquad, idFinish);

						if (quadGetState(tmpcurridquad) != AIQUAD_STATE::AIQUAD_STATE_FREE && tmphcost / 10 < AIGRID_QUADSCOUNT_BUSY)
						{
							if (quadGetState(tmpcurridquad) == AIQUAD_STATE::AIQUAD_STATE_TEMPBUSY)
								tmpcost += AIGRID_COSTADD_TEMPBUSY;
							else if (quadGetState(tmpcurridquad) == AIQUAD_STATE::AIQUAD_STATE_BUSY)
								tmpcost += AIGRID_COSTADD_BUSY;
						}
						else
							tmpaddcost = 0;

						//если есть родитель
						if (m_aParentIDs[tmpcurridquad] != -1)
						{
							//считаем а не дешевле ли перейти на соседний квад через этот квад
							if (m_aCostQuads[tmpcurridquad].G > m_aCostQuads[tmpsmallF].G + tmpcost)
							{
								m_aParentIDs[tmpcurridquad] = tmpsmallF;
								m_aCostQuads[tmpcurridquad].G = m_aCostQuads[tmpsmallF].G + tmpcost;
								m_aCostQuads[tmpcurridquad].F = m_aCostQuads[tmpcurridquad].G + m_aCostQuads[tmpcurridquad].H;
							}
						}
						//если нет родителя то назначаем текущий квад родителем и считаем стоимости
						//и если не в закрытом списке
						else
						{
							m_aParentIDs[tmpcurridquad] = tmpsmallF;
							m_aCostQuads[tmpcurridquad].G = m_aCostQuads[tmpsmallF].G + tmpcost;
							m_aCostQuads[tmpcurridquad].H = tmphcost;// CalcHCost(tmpcurridquad, idFinish);
							m_aCostQuads[tmpcurridquad].F = m_aCostQuads[tmpcurridquad].G + m_aCostQuads[tmpcurridquad].H;
							m_aIDsInOpen[tmpcurridquad] = AddInOpenList(tmpcurridquad);
						}
					}
				}
			}

			m_aOpenIDs[m_aIDsInOpen[tmpsmallF]] = -1;
			if (NumLastKeyOpenList > m_aIDsInOpen[tmpsmallF])
				NumLastKeyOpenList = m_aIDsInOpen[tmpsmallF];
			m_aIDsInOpen[tmpsmallF] = -1;

			m_aTestSplitsIDs[tmpsmallF] = true;

			if (isfind)
			{
				IsFindPath = true;
				aQuads.push_back(idFinish);
				aQuads.push_back(tmpsmallF);
				ID tmpquad = tmpsmallF;
				ID tmpparent = -1;
				/*m_aQuadColors[tmpsmallF] = D3DCOLOR_ARGB(200,0,255,0);
				m_aQuadColors[idStart] = D3DCOLOR_ARGB(255, 0, 255, 0);
				m_aQuadColors[idFinish] = D3DCOLOR_ARGB(255, 0, 255, 0);*/
				while ((tmpparent = m_aParentIDs[tmpquad]) != -1 && idStart != tmpparent)
				{
					aQuads.push_back(tmpparent);
					//m_aQuadColors[tmpparent] = D3DCOLOR_ARGB(128, 0, 255, 0);
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

int CAIGrid::gridGetSizePath(ID idQueueObject) const
{
	if (!existsQuads())
		return -1;

	if (idQueueObject >= 0 && idQueueObject < m_aQueueFind.size() && m_aQueueFind[idQueueObject].m_state == QUEUE_OBJ_STATE_COMPLITE)
		return m_aQueueFind[idQueueObject].m_aQuads.size();

	return -1;
}

bool CAIGrid::gridGetPath(ID idQueueObject, ID *pMemory, UINT uiCount, bool canReverse)
{
	if (!existsQuads())
		return false;

	if (idQueueObject >= 0 && idQueueObject < m_aQueueFind.size() && m_aQueueFind[idQueueObject].m_state == QUEUE_OBJ_STATE_COMPLITE)
	{
		if (pMemory)
		{
			if (!canReverse)
				memcpy(pMemory, &(m_aQueueFind[idQueueObject].m_aQuads[0]), uiCount * sizeof(ID));
			else
			{
				for (int i = 0, il = m_aQueueFind[idQueueObject].m_aQuads.size(); i < il; ++i)
				{
					pMemory[i] = m_aQueueFind[idQueueObject].m_aQuads[(il - 1) - i];
				}
			}
			m_aQueueFind[idQueueObject].m_aQuads.clearFast();
			m_aQueueFind[idQueueObject].m_state = QUEUE_OBJ_STATE_IDLE;
			m_aQueueFind[idQueueObject].m_idStart = m_aQueueFind[idQueueObject].m_idFinish = -1;
			return true;
		}
	}

	return false;
}

void CAIGrid::gridSetColorArr(const ID * pmem, DWORD color, UINT count)
{
	if (m_aQuadColors.size() == 0 || !pmem)
		return;

	for (int i = 0; i < count; ++i)
	{
		if (pmem[i] >= 0 && pmem[i] < m_aQuadColors.size())
			m_aQuadColors[pmem[i]] = color;
	}
}

void CAIGrid::gridSetNullColor()
{
	if (m_aQuadColors.size() > 0)
		memset(&(m_aQuadColors[0]), 0, sizeof(uint32_t)* m_aQuadColors.size());
}

//##########################################################################

ID CAIGrid::getQueueWaiting()
{
	for (int i = m_iLastWait, il = m_aQueueFind.size(); i < il; ++i)
	{
		if (m_aQueueFind[i].m_state == QUEUE_OBJ_STATE_WAIT)
		{
			m_iLastWait = i;
			return i;
		}
	}

	m_iLastWait = 0;

	for (int i = m_iLastWait, il = m_aQueueFind.size(); i < il; ++i)
	{
		if (m_aQueueFind[i].m_state == QUEUE_OBJ_STATE_WAIT)
		{
			m_iLastWait = i;
			return i;
		}
	}

	if (m_iLastWait >= m_aQueueFind.size() - 1)
		m_iLastWait = 0;

	return -1;
}

ID CAIGrid::getQueueIdle()
{
	for (int i = m_iLastWait, il = m_aQueueFind.size(); i < il; ++i)
	{
		if (m_aQueueFind[i].m_state == QUEUE_OBJ_STATE_IDLE)
			return i;
	}

	for (int i = 0, il = m_iLastWait; i < il; ++i)
	{
		if (m_aQueueFind[i].m_state == QUEUE_OBJ_STATE_IDLE)
			return i;
	}

	m_aQueueFind.push_back(CQueueObject());

	return m_aQueueFind.size() - 1;
}

ID CAIGrid::gridQueryFindPath(ID idStart, ID idFinish)
{
	if (!existsQuads())
		return -1;

	if (m_aQuads.size() <= 1)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "AI grid not found\n");
		return -1;
	}

	if (idStart < 0 || m_aQuads.size() <= idStart)
	{ 
		LibReport(REPORT_MSG_LEVEL_WARNING, " %s - unresolved index '%d' of quad, sxaigrid\n", GEN_MSG_LOCATION, idStart);
		return -1; 
	}

	if (idFinish < 0 || m_aQuads.size() <= idFinish)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, " %s - unresolved index '%d' of quad, sxaigrid\n", GEN_MSG_LOCATION, idFinish);
		return -1;
	}

	if (m_uiCountSplits == 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "can not find path, because AI grid is not validate splits\n");
		return -1;
	}

	if (m_aQuads[idStart]->m_iSplit != m_aQuads[idFinish]->m_iSplit)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "quads in different splits, path not found\n");
		return -1;
	}

	if (idStart == idFinish)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "idStart == idFinish, path not found\n");
		return -1;
	}

	ID idQueueObject = getQueueIdle();

	m_aQueueFind[idQueueObject].m_aQuads.clearFast();
	m_aQueueFind[idQueueObject].m_state = QUEUE_OBJ_STATE_WAIT;
	m_aQueueFind[idQueueObject].m_idStart = idStart;
	m_aQueueFind[idQueueObject].m_idFinish = idFinish;

	return idQueueObject;
}

bool CAIGrid::gridCancelQueryFindPath(ID idQuery)
{
	if (!existsQuads())
		return false;

	if (idQuery >= 0 && idQuery < m_aQueueFind.size())
	{
		m_aQueueFind[idQuery].m_state = QUEUE_OBJ_STATE_IDLE;
		m_aQueueFind[idQuery].m_idStart = m_aQueueFind[idQuery].m_idFinish = -1;
		return true;
	}

	return false;
}

void CAIGrid::gridQueryFindPathUpdate(UINT uiLimitMls)
{
	if (!existsQuads())
		return;

	UINT uiStartTime = GetTickCount();

	while ((GetTickCount() - uiStartTime < uiLimitMls) || uiLimitMls == 0)
	{
		ID idQueueObject = getQueueWaiting();

		if (idQueueObject >= 0)
		{
			if (gridFindPath(idQueueObject))
				m_aQueueFind[idQueueObject].m_state = QUEUE_OBJ_STATE_COMPLITE;
			else
				m_aQueueFind[idQueueObject].m_state = QUEUE_OBJ_STATE_ERROR;
		}
		else
			break;
	}
}