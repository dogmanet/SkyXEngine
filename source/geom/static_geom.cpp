
#include <geom\\static_geom.h>

StaticGeom::StaticGeom()
{
	D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	DXDevice->CreateVertexDeclaration(layoutstatic, &VertexDeclarationStatic);
}

long StaticGeom::AddModel(const char* path)
{
	DataStaticModel model;
	SGCore_LoadStaticModel(path, &model);

	bool isunictex = true;
	for (int i = 0; i < model.SubsetCount; ++i)
	{
		isunictex = true;
		for (int k = 0; k < AllGroups.size(); ++k)
		{
			if (stricmp(AllGroups[i].name.c_str(), model.ArrTextures[k]) == 0)
			{
				isunictex = false;

			}
		}

		if (isunictex)
		{
			Group ngroup;
			//ngroup.VertexBuffer.push_back(model.VertexBuffer);
			//ngroup.IndexBuffer.push_back(model.IndexBuffer);

			IDirect3DVertexBuffer9* vb;
			DXDevice->CreateVertexBuffer(sizeof(vertex_static)* model.VertexCount[i], NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
			ngroup.CountVertex = model.VertexCount[i];
			vertex_static * pData;
			if (!FAILED(vb->Lock(0, 0, (void**)&pData, 0)))
			{
				memcpy(pData, model.ArrVertBuf + model.StartVertex[i], sizeof(vertex_static)* model.VertexCount[i]);
				vb->Unlock();
			}

			IDirect3DIndexBuffer9* ib;
			DXDevice->CreateIndexBuffer(sizeof(UINT)* model.IndexCount[i], NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &ib, 0);
			ngroup.CountIndex = model.IndexCount[i];
			if (!FAILED(ib->Lock(0, 0, (void**)&pData, 0)))
			{
				memcpy(pData, model.ArrIndBuf + model.StartIndex[i], sizeof(UINT)* model.IndexCount[i]);
				ib->Unlock();
			}

			ngroup.VertexBuffer.push_back(vb);
			ngroup.IndexBuffer.push_back(ib);

			char tmptex[1024];
			sprintf(tmptex, "%s.dds", model.ArrTextures[i]);

			ngroup.idtex = SGCore_LoadTexAddName(tmptex);
			ngroup.name = model.ArrTextures[i];
			AllGroups.push_back(ngroup);
		}
	}

	//mem_release_del(model.VertexBuffer);
	//mem_release_del(model.IndexBuffer);

	for (DWORD i = 0; i < model.SubsetCount; ++i)
	{
		mem_delete_a(model.ArrTextures[i]);
	}

	mem_delete_a(model.ArrTextures);
	mem_delete_a(model.StartIndex);
	mem_delete_a(model.IndexCount);
	mem_delete_a(model.StartVertex);
	mem_delete_a(model.VertexCount);

	return 0;
}

void StaticGeom::Render()
{
	for (int i = 0; i < AllGroups.size(); ++i)
	{
		DXDevice->SetStreamSource(0, AllGroups[i].VertexBuffer[0], 0, sizeof(vertex_static));
		DXDevice->SetIndices(AllGroups[i].IndexBuffer[0]);
		DXDevice->SetVertexDeclaration(VertexDeclarationStatic);
		DXDevice->SetTexture(0, SGCore_GetTex(AllGroups[i].idtex));
		long tmpvert = AllGroups[i].CountVertex;
		long tmpcountpoly = AllGroups[i].CountIndex / 3;
		DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, AllGroups[i].CountVertex, 0, AllGroups[i].CountIndex / 3);
	}
}