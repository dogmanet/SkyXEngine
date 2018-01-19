
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __LOADER_STATIC_H
#define __LOADER_STATIC_H

#include <gdefines.h>
#include <stdio.h>
#include <d3d9.h>
#include <common/String.h>
#include <common/Array.h>
#include "sxgcore.h"

extern IDirect3DDevice9 *g_pDXDevice;
extern D3DPRESENT_PARAMETERS D3DAPP;
extern IDirect3DVertexDeclaration9 *g_pStaticVertexDecl;

struct DataStaticModel : public ISXDataStaticModel
{
	DataStaticModel()
	{
		m_pVertexBuffer = 0;
		m_pIndexBuffer = 0;
		//ArrVertBuf = 0;
		//ArrIndBuf = 0;

		m_uiSubsetCount = 0;
		m_ppTextures = 0;
		m_pStartIndex = 0;
		m_pIndexCount = 0;
		m_pStartVertex = 0;
		m_pVertexCount = 0;
		m_uiAllIndexCount = 0;
		m_uiAllVertexCount = 0;
	}

	void Release(){ mem_del(this); }

	ISXDataStaticModel* getCopy()
	{
		ISXDataStaticModel* nm = new DataStaticModel();
		nm->m_ppTextures = new char*[m_uiSubsetCount];
		for (DWORD i = 0; i < m_uiSubsetCount; i++)
		{
			nm->m_ppTextures[i] = new char[strlen(m_ppTextures[i]) + 1];
			sprintf(nm->m_ppTextures[i], "%s", m_ppTextures[i]);
		}
		nm->m_uiSubsetCount = m_uiSubsetCount;
		nm->m_pStartIndex = new UINT[m_uiSubsetCount];
		memcpy(nm->m_pStartIndex, m_pStartIndex, sizeof(UINT)*m_uiSubsetCount);
		nm->m_pIndexCount = new UINT[m_uiSubsetCount];
		memcpy(nm->m_pIndexCount, m_pIndexCount, sizeof(UINT)*m_uiSubsetCount);
		nm->m_pStartVertex = new UINT[m_uiSubsetCount];
		memcpy(nm->m_pStartVertex, m_pStartVertex, sizeof(UINT)*m_uiSubsetCount);
		nm->m_pVertexCount = new UINT[m_uiSubsetCount];
		memcpy(nm->m_pVertexCount, m_pVertexCount, sizeof(UINT)*m_uiSubsetCount);

		DWORD tmpvert = 0;
		DWORD tmpind = 0;
		for (DWORD i = 0; i < m_uiSubsetCount; i++)
		{
			tmpvert += nm->m_pVertexCount[i];
			tmpind += nm->m_pIndexCount[i];
		}

		g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* tmpvert, NULL, NULL, D3DPOOL_MANAGED, &nm->m_pVertexBuffer, 0);
		//nm->ArrVertBuf = new vertex_static[tmpvert];
		vertex_static * dstData, *srcData;
		nm->m_pVertexBuffer->Lock(0, 0, (void**)&dstData, 0);
		m_pVertexBuffer->Lock(0, 0, (void**)&srcData, 0);

		memcpy(dstData, srcData, sizeof(vertex_static)* tmpvert);
		//memcpy(nm->ArrVertBuf, srcData, sizeof(vertex_static)* tmpvert);

		nm->m_pVertexBuffer->Unlock();
		m_pVertexBuffer->Unlock();


		g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* tmpind, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &nm->m_pIndexBuffer, 0);
		//nm->ArrIndBuf = new UINT[tmpind];
		nm->m_pIndexBuffer->Lock(0, 0, (void**)&dstData, 0);
		m_pIndexBuffer->Lock(0, 0, (void**)&srcData, 0);

		memcpy(dstData, srcData, sizeof(UINT)* tmpind);
		//memcpy(nm->ArrIndBuf, srcData, sizeof(UINT)* tmpind);

		nm->m_pIndexBuffer->Unlock();
		m_pIndexBuffer->Unlock();

		return nm;
	}

	DataStaticModel::~DataStaticModel()
	{
		mem_release_del(m_pVertexBuffer);
		mem_release_del(m_pIndexBuffer);

		for (DWORD i = 0; i < m_uiSubsetCount; ++i)
		{
			mem_delete_a(m_ppTextures[i]);
		}

		//mem_delete_a(ArrVertBuf);
		//mem_delete_a(ArrIndBuf);

		mem_delete_a(m_ppTextures);
		mem_delete_a(m_pStartIndex);
		mem_delete_a(m_pIndexCount);
		mem_delete_a(m_pStartVertex);
		mem_delete_a(m_pVertexCount);
	}
};

#endif