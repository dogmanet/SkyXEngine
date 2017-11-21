
#ifndef __LOADER_STATIC_H
#define __LOADER_STATIC_H

#include <gdefines.h>
#include <stdio.h>
#include <d3d9.h>
#include <common/String.h>
#include <common/Array.h>
#include "sxgcore.h"

extern report_func g_fnReportf;
extern IDirect3DDevice9* DXDevice;
extern D3DPRESENT_PARAMETERS D3DAPP;
extern IDirect3DVertexDeclaration9* StaticVertexDecl;

struct DataStaticModel : public ISXDataStaticModel
{
	DataStaticModel()
	{
		VertexBuffer = 0;
		IndexBuffer = 0;
		//ArrVertBuf = 0;
		//ArrIndBuf = 0;

		SubsetCount = 0;
		ArrTextures = 0;
		StartIndex = 0;
		IndexCount = 0;
		StartVertex = 0;
		VertexCount = 0;
		AllIndexCount = 0;
		AllVertexCount = 0;
	}

	void Release(){ mem_del(this); }

	ISXDataStaticModel* GetCopy()
	{
		ISXDataStaticModel* nm = new DataStaticModel();
		nm->ArrTextures = new char*[SubsetCount];
		for (DWORD i = 0; i < SubsetCount; i++)
		{
			nm->ArrTextures[i] = new char[strlen(ArrTextures[i]) + 1];
			sprintf(nm->ArrTextures[i], "%s", ArrTextures[i]);
		}
		nm->SubsetCount = SubsetCount;
		nm->StartIndex = new UINT[SubsetCount];
		memcpy(nm->StartIndex, StartIndex, sizeof(UINT)*SubsetCount);
		nm->IndexCount = new UINT[SubsetCount];
		memcpy(nm->IndexCount, IndexCount, sizeof(UINT)*SubsetCount);
		nm->StartVertex = new UINT[SubsetCount];
		memcpy(nm->StartVertex, StartVertex, sizeof(UINT)*SubsetCount);
		nm->VertexCount = new UINT[SubsetCount];
		memcpy(nm->VertexCount, VertexCount, sizeof(UINT)*SubsetCount);

		DWORD tmpvert = 0;
		DWORD tmpind = 0;
		for (DWORD i = 0; i < SubsetCount; i++)
		{
			tmpvert += nm->VertexCount[i];
			tmpind += nm->IndexCount[i];
		}

		DXDevice->CreateVertexBuffer(sizeof(vertex_static)* tmpvert, NULL, NULL, D3DPOOL_MANAGED, &nm->VertexBuffer, 0);
		//nm->ArrVertBuf = new vertex_static[tmpvert];
		vertex_static * dstData, *srcData;
		nm->VertexBuffer->Lock(0, 0, (void**)&dstData, 0);
		VertexBuffer->Lock(0, 0, (void**)&srcData, 0);

		memcpy(dstData, srcData, sizeof(vertex_static)* tmpvert);
		//memcpy(nm->ArrVertBuf, srcData, sizeof(vertex_static)* tmpvert);

		nm->VertexBuffer->Unlock();
		VertexBuffer->Unlock();


		DXDevice->CreateIndexBuffer(sizeof(UINT)* tmpind, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &nm->IndexBuffer, 0);
		//nm->ArrIndBuf = new UINT[tmpind];
		nm->IndexBuffer->Lock(0, 0, (void**)&dstData, 0);
		IndexBuffer->Lock(0, 0, (void**)&srcData, 0);

		memcpy(dstData, srcData, sizeof(UINT)* tmpind);
		//memcpy(nm->ArrIndBuf, srcData, sizeof(UINT)* tmpind);

		nm->IndexBuffer->Unlock();
		IndexBuffer->Unlock();

		return nm;
	}

	DataStaticModel::~DataStaticModel()
	{
		mem_release_del(VertexBuffer);
		mem_release_del(IndexBuffer);

		for (DWORD i = 0; i < SubsetCount; ++i)
		{
			mem_delete_a(ArrTextures[i]);
		}

		//mem_delete_a(ArrVertBuf);
		//mem_delete_a(ArrIndBuf);

		mem_delete_a(ArrTextures);
		mem_delete_a(StartIndex);
		mem_delete_a(IndexCount);
		mem_delete_a(StartVertex);
		mem_delete_a(VertexCount);
	}
};


///////////



#endif