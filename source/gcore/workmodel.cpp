

void CreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, IDirect3DDevice9 * pDevice,UINT iSideCount)
{
	UINT iVC = iSideCount * 2;
	UINT iIC = (iSideCount - 2) * 6 + iSideCount * 6;
	float3_t * pVertices = new float3_t[iVC];
	UINT * pIndices = new UINT[iIC];

	//top
	UINT iCurV = 0;
	for(UINT i = 0; i < iSideCount; i++)
	{
		float A = (float)i / (float)iSideCount * SM_2PI;
		pVertices[iCurV] = float3_t(fTopRadius * cosf(A),0.0f, -fTopRadius * sinf(A));
		iCurV++;
	}

	//bottom
	for(UINT i = 0; i < iSideCount; i++)
	{
		float A = (float)i / (float)iSideCount * SM_2PI;
		pVertices[iCurV] = float3_t(fBottomRadius * cosf(A), -fHeight, fBottomRadius * sinf(A));
		iCurV++;
	}

	//top
	UINT iCurI = 0;
	for(UINT i = 0; i < iSideCount - 2; i++)
	{
		pIndices[iCurI] = 0;
		iCurI++;
		pIndices[iCurI] = 1 + i;
		iCurI++;
		pIndices[iCurI] = 2 + i;
		iCurI++;
	}

	//bottom
	for(UINT i = 0; i < iSideCount - 2; i++)
	{
		pIndices[iCurI] = iSideCount;
		iCurI++;
		pIndices[iCurI] = iSideCount + 1 + i;
		iCurI++;
		pIndices[iCurI] = iSideCount + 2 + i;
		iCurI++;
	}

	//side
	for(UINT i = 0; i < iSideCount; i++)
	{
		pIndices[iCurI] = (i + 1) % iSideCount;
		iCurI++;
		pIndices[iCurI] = i;
		iCurI++;
		pIndices[iCurI] = (iSideCount - i) % iSideCount + iSideCount;
		iCurI++;

		pIndices[iCurI] = (iSideCount - i) % iSideCount + iSideCount;
		iCurI++;
		pIndices[iCurI] = (2 * iSideCount - i - 1) % iSideCount + iSideCount;
		iCurI++;
		pIndices[iCurI] = (i + 1) % iSideCount;
		iCurI++;
	}

	D3DXCreateMeshFVF(iIC / 3, iVC, D3DXMESH_32BIT | D3DXMESH_MANAGED, D3DFVF_XYZ, pDevice, ppMesh);
	VOID * pData;
	(*ppMesh)->LockVertexBuffer(D3DLOCK_DISCARD, &pData);
	memcpy(pData, pVertices, sizeof(float3_t) * iVC);
	(*ppMesh)->UnlockVertexBuffer();

	(*ppMesh)->LockIndexBuffer(D3DLOCK_DISCARD, &pData);
	memcpy(pData, pIndices, sizeof(UINT) * iIC);
	(*ppMesh)->UnlockIndexBuffer();
	
	mem_delete(pIndices);
	mem_delete(pVertices);
}

void ComputeBoundingBox(IDirect3DVertexBuffer9* vertex_buffer,ISXBound** bound,DWORD count_vert,DWORD bytepervert)
{
	float3_t *V = 0;
	HRESULT hr = 0;
	float3_t Max;
	float3_t Min;
	
		if(SUCCEEDED(vertex_buffer->Lock(0,0, (void **)&V,0)))
		{
			float3_t tmppos = *(float3_t*)((char*)(V) + bytepervert * 0);
			Max = tmppos;
			Min = tmppos;

				for(DWORD i=0;i<count_vert;i++)
				{
					float3_t pos = *(float3*)((char*)(V) + bytepervert * i);

						if(pos.x > Max.x)
							Max.x = pos.x;

						if(pos.y > Max.y)
							Max.y = pos.y;

						if(pos.z > Max.z)
							Max.z = pos.z;


						if(pos.x < Min.x)
							Min.x = pos.x;

						if(pos.y < Min.y)
							Min.y = pos.y;

						if(pos.z < Min.z)
							Min.z = pos.z;
				}
			vertex_buffer->Unlock();
		}

	(*bound)->SetMinMax(&float3(Min),&float3(Max));
}

void ComputeBoundingBox2(IDirect3DVertexBuffer9* vertex_buffer,ISXBound* bound,DWORD count_vert,DWORD bytepervert)
{
	float3_t *V = 0;
	HRESULT hr = 0;
	float3_t Max;
	float3_t Min;
	
		if(SUCCEEDED(vertex_buffer->Lock(0,0, (void **)&V,0)))
		{
			float3_t tmppos = *(float3*)((char*)(V) + bytepervert * 0);
			Max = tmppos;
			Min = tmppos;

				for(DWORD i=0;i<count_vert;i++)
				{
					float3_t pos = *(float3*)((char*)(V) + bytepervert * i);

						if(pos.x > Max.x)
							Max.x = pos.x;

						if(pos.y > Max.y)
							Max.y = pos.y;

						if(pos.z > Max.z)
							Max.z = pos.z;


						if(pos.x < Min.x)
							Min.x = pos.x;

						if(pos.y < Min.y)
							Min.y = pos.y;

						if(pos.z < Min.z)
							Min.z = pos.z;
				}
			vertex_buffer->Unlock();
		}

	Min.x /= 100.f;
	Min.y /= 100.f;
	Min.z /= 100.f;

	Max.x /= 100.f;
	Max.y /= 100.f;
	Max.z /= 100.f;

	bound->SetMinMax(&float3(Min),&float3(Max));
}


//////////////////////////////////

inline bool InPosition2D(float3* min,float3* max,float3* pos)
{
		if((max->x >= pos->x && min->x <= pos->x) && (max->z >= pos->z && min->z <= pos->z))
			return true;
		else if(
				(long(max->x * 1000) >= long(pos->x * 1000) && long(min->x * 1000) <= long(pos->x * 1000))
				&& 
				(long(max->z * 1000) >= long(pos->z * 1000) && long(min->z * 1000) <= long(pos->z * 1000))
				)
			return true;
		else 
			return false;
}

inline bool InPositionAbs2D(float3* min,float3* max,float3* pos)
{
		if((max->x > pos->x && min->x < pos->x) && (max->z > pos->z && min->z < pos->z))
			return true;
		else
			return false;
}


inline int CountPositionPoints2D(float3* min,float3* max,float3* p1,float3* p2,float3* p3)
{
	int Count = 0;

		if(InPosition2D(min,max,p1))
			Count++;
		if(InPosition2D(min,max,p2))
			Count++;
		if(InPosition2D(min,max,p3))
			Count++;

	return Count;
}

inline int CountPositionPointsAbs2D(float3* min,float3* max,float3* p1,float3* p2,float3* p3)
{
	int Count = 0;

		if(InPositionAbs2D(min,max,p1))
			Count++;
		if(InPositionAbs2D(min,max,p2))
			Count++;
		if(InPositionAbs2D(min,max,p3))
			Count++;

	return Count;
}


inline bool InPositionPoints2D(float3* min,float3* max,float3* p1,float3* p2,float3* p3)
{
		if(CountPositionPointsAbs2D(min,max,p1,p2,p3) >= 1)	
			return true;
		else if(CountPositionPoints2D(min,max,p1,p2,p3) >= 2)	
			return true;
		else
			return false;
}

///////////////////////////////////

inline bool InPosition3D(float3* min,float3* max,float3* pos)
{
		if((max->x >= pos->x && min->x <= pos->x) && (max->y >= pos->y && min->y <= pos->y) && (max->z >= pos->z && min->z <= pos->z))
			return true;
		else if(
				(long(max->x * 1000) >= long(pos->x * 1000) && long(min->x * 1000) <= long(pos->x * 1000))
				&& 
				(long(max->y * 1000) >= long(pos->y * 1000) && long(min->y * 1000) <= long(pos->y * 1000))
				&&
				(long(max->z * 1000) >= long(pos->z * 1000) && long(min->z * 1000) <= long(pos->z * 1000))
				)
			return true;
		else 
			return false;
}

inline bool InPositionAbs3D(float3* min,float3* max,float3* pos)
{
		if((max->x > pos->x && min->x < pos->x) && (max->y > pos->y && min->y < pos->y) && (max->z > pos->z && min->z < pos->z))
			return true;
		else
			return false;
}


inline int CountPositionPoints3D(float3* min,float3* max,float3* p1,float3* p2,float3* p3)
{
	int Count = 0;

		if(InPosition3D(min,max,p1))
			Count++;
		if(InPosition3D(min,max,p2))
			Count++;
		if(InPosition3D(min,max,p3))
			Count++;

	return Count;
}

inline int CountPositionPointsAbs3D(float3* min,float3* max,float3* p1,float3* p2,float3* p3)
{
	int Count = 0;

		if(InPositionAbs3D(min,max,p1))
			Count++;
		if(InPositionAbs3D(min,max,p2))
			Count++;
		if(InPositionAbs3D(min,max,p3))
			Count++;

	return Count;
}


inline bool InPositionPoints3D(float3* min,float3* max,float3* p1,float3* p2,float3* p3)
{
		if(CountPositionPointsAbs3D(min,max,p1,p2,p3) >= 1)	
			return true;
		else if(CountPositionPoints3D(min,max,p1,p2,p3) >= 2)	
			return true;
		else
			return false;
}

////////////////////

void ComputeBoundingBoxArr8(ISXBound* bound,ISXBound** bound_arr)
{
	float3 tmpMin2,tmpMax2;
	float3 tmpMin,tmpMax;
	bound->GetMinMax(&tmpMin2,&tmpMax2);
	bound->GetMinMax(&tmpMin,&tmpMax);

	float x = (tmpMax.x + tmpMin.x) * 0.5;
	float y = (tmpMax.y + tmpMin.y) * 0.5;
	float z = (tmpMax.z + tmpMin.z) * 0.5;

	float dist_x = (tmpMax.x - tmpMin.x) * 0.25;
	float dist_y = (tmpMax.y - tmpMin.y) * 0.25;
	float dist_z = (tmpMax.z - tmpMin.z) * 0.25;


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(dist_x,dist_y,dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(dist_x,dist_y,dist_z);

	(bound_arr[0])->SetMinMax(&tmpMin2,&tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(-dist_x,dist_y,dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(-dist_x,dist_y,dist_z);

	(bound_arr[1])->SetMinMax(&tmpMin2, &tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;
	//bound->GetMinMax(&tmpMin2,&tmpMax2);

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(dist_x,dist_y,-dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(dist_x,dist_y,-dist_z);

	(bound_arr[2])->SetMinMax(&tmpMin2, &tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;
	//bound->GetMinMax(&tmpMin2,&tmpMax2);

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(-dist_x,dist_y,-dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(-dist_x,dist_y,-dist_z);

	(bound_arr[3])->SetMinMax(&tmpMin2, &tmpMax2);


	
	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(dist_x,-dist_y,dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(dist_x,-dist_y,dist_z);

	(bound_arr[4])->SetMinMax(&tmpMin2, &tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(-dist_x,-dist_y,dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(-dist_x,-dist_y,dist_z);

	(bound_arr[5])->SetMinMax(&tmpMin2, &tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;
	//bound->GetMinMax(&tmpMin2,&tmpMax2);

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(dist_x,-dist_y,-dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(dist_x,-dist_y,-dist_z);

	(bound_arr[6])->SetMinMax(&tmpMin2, &tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;
	//bound->GetMinMax(&tmpMin2,&tmpMax2);

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(-dist_x,-dist_y,-dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(-dist_x,-dist_y,-dist_z);

	(bound_arr[7])->SetMinMax(&tmpMin2, &tmpMax2);
}

void ComputeBoundingBoxArr4(ISXBound* bound,ISXBound** bound_arr)
{
	float3 tmpMin2,tmpMax2;
	float3 tmpMin,tmpMax;
	bound->GetMinMax(&tmpMin2,&tmpMax2);
	bound->GetMinMax(&tmpMin,&tmpMax);

	float x = (tmpMax.x + tmpMin.x) * 0.5;
	float y = (tmpMax.y + tmpMin.y) * 0.5;
	float z = (tmpMax.z + tmpMin.z) * 0.5;

	float dist_x = (tmpMax.x - tmpMin.x) * 0.25;
	float dist_y = (tmpMax.y - tmpMin.y) * 0.5;
	float dist_z = (tmpMax.z - tmpMin.z) * 0.25;


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(dist_x,0,dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(dist_x,0,dist_z);

	(bound_arr[0])->SetMinMax(&tmpMin2,&tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(-dist_x,0,dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(-dist_x,0,dist_z);

	(bound_arr[1])->SetMinMax(&tmpMin2, &tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;
	//bound->GetMinMax(&tmpMin2,&tmpMax2);

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(dist_x,0,-dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(dist_x,0,-dist_z);

	(bound_arr[2])->SetMinMax(&tmpMin2, &tmpMax2);


	tmpMin2.x = -dist_x;
	tmpMin2.y = -dist_y;
	tmpMin2.z = -dist_z;

	tmpMax2.x = dist_x;
	tmpMax2.y = dist_y;
	tmpMax2.z = dist_z;
	//bound->GetMinMax(&tmpMin2,&tmpMax2);

	tmpMin2 += float3(x,y,z);
	tmpMin2 += float3(-dist_x,0,-dist_z);

	tmpMax2 += float3(x,y,z);
	tmpMax2 += float3(-dist_x,0,-dist_z);

	(bound_arr[3])->SetMinMax(&tmpMin2, &tmpMax2);
}

////////////////////


void CreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh, IDirect3DDevice9* device)
{
	float dist_x = abs(max->x - min->x);
	float dist_y = abs(max->y - min->y);
	float dist_z = abs(max->z - min->z);

	//ID3DXMesh* BoundMesh = 0;
	HRESULT hr = 0;

	hr = D3DXCreateBox(
						device,
						dist_x,
						dist_y,
						dist_z,
						bbmesh,
						0);

	void* Vetx = 0;
	int VertexBytes =  (*bbmesh)->GetNumBytesPerVertex();
	D3DXVECTOR3 dv = D3DXVECTOR3((max->x + min->x)*0.5f,(max->y + min->y)*0.5f,(max->z + min->z)*0.5f);

	(*bbmesh)->LockVertexBuffer(0,&Vetx);
		for(int i=0; i<(*bbmesh)->GetNumVertices(); i++)
		{
			D3DXVECTOR3* v = (D3DXVECTOR3*)((char*)(Vetx) + VertexBytes * i);
			*v += dv;
		}
	(*bbmesh)->UnlockVertexBuffer();
}

void OptimizeIndecesInSubsetUint16(uint16_t* ib, uint16_t numFaces, uint16_t numVerts)
{
	uint16_t* pdwRemap = new uint16_t[numFaces];
	D3DXOptimizeFaces(ib, numFaces, numVerts, FALSE, (DWORD*)pdwRemap);

	uint16_t* pCopyIB = new uint16_t[numFaces * 3];
	memcpy(pCopyIB, ib, numFaces * 3 * sizeof(uint16_t));

	for (int i = 0; i<numFaces; ++i)
	{
		int newFace = (int)pdwRemap[i];
		for (int j = 0; j<3; ++j)
		{
			ib[i * 3 + j] = pCopyIB[newFace * 3 + j];
		}
	}

	mem_delete_a(pCopyIB);
	mem_delete_a(pdwRemap);
}

void OptimizeIndecesInSubsetUint32(uint32_t* ib, uint32_t numFaces, uint32_t numVerts)
{
	uint32_t* pdwRemap = new uint32_t[numFaces];
	D3DXOptimizeFaces(ib, numFaces, numVerts, TRUE, (DWORD*)pdwRemap);

	uint32_t* pCopyIB = new uint32_t[numFaces * 3];
	memcpy(pCopyIB, ib, numFaces * 3 * sizeof(uint32_t));

	for (int i = 0; i<numFaces; ++i)
	{
		int newFace = (int)pdwRemap[i];
		for (int j = 0; j<3; ++j)
		{
			ib[i * 3 + j] = pCopyIB[newFace * 3 + j];
		}
	}

	mem_delete_a(pCopyIB);
	mem_delete_a(pdwRemap);
}