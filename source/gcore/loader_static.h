
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
		ISXDataStaticModel *pModelCopy = new DataStaticModel();

		//не все данные копируюцо!!!!!!!!!!!!!
		pModelCopy->m_uiAllVertexCount = m_uiAllVertexCount;
		pModelCopy->m_uiAllIndexCount = m_uiAllIndexCount;

		pModelCopy->m_vBBMax = m_vBBMax;
		pModelCopy->m_vBBMin = m_vBBMin;
		pModelCopy->m_vBSphere = m_vBSphere;

		pModelCopy->m_ppTextures = new char*[m_uiSubsetCount];
		for (int i = 0; i < m_uiSubsetCount; ++i)
		{
			pModelCopy->m_ppTextures[i] = new char[strlen(m_ppTextures[i]) + 1];
			sprintf(pModelCopy->m_ppTextures[i], "%s", m_ppTextures[i]);
		}

		pModelCopy->m_uiSubsetCount = m_uiSubsetCount;
		pModelCopy->m_pStartIndex = new UINT[m_uiSubsetCount];
		memcpy(pModelCopy->m_pStartIndex, m_pStartIndex, sizeof(UINT)*m_uiSubsetCount);
		pModelCopy->m_pIndexCount = new UINT[m_uiSubsetCount];
		memcpy(pModelCopy->m_pIndexCount, m_pIndexCount, sizeof(UINT)*m_uiSubsetCount);
		pModelCopy->m_pStartVertex = new UINT[m_uiSubsetCount];
		memcpy(pModelCopy->m_pStartVertex, m_pStartVertex, sizeof(UINT)*m_uiSubsetCount);
		pModelCopy->m_pVertexCount = new UINT[m_uiSubsetCount];
		memcpy(pModelCopy->m_pVertexCount, m_pVertexCount, sizeof(UINT)*m_uiSubsetCount);

		g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* m_uiAllVertexCount, NULL, NULL, D3DPOOL_MANAGED, &pModelCopy->m_pVertexBuffer, 0);
		BYTE *pDestData, *pSrcData;
		pModelCopy->m_pVertexBuffer->Lock(0, 0, (void**)&pDestData, 0);
		m_pVertexBuffer->Lock(0, 0, (void**)&pSrcData, 0);

		memcpy(pDestData, pSrcData, sizeof(vertex_static)* m_uiAllVertexCount);

		pModelCopy->m_pVertexBuffer->Unlock();
		m_pVertexBuffer->Unlock();


		g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* m_uiAllIndexCount, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &pModelCopy->m_pIndexBuffer, 0);
		pModelCopy->m_pIndexBuffer->Lock(0, 0, (void**)&pDestData, 0);
		m_pIndexBuffer->Lock(0, 0, (void**)&pSrcData, 0);

		memcpy(pDestData, pSrcData, sizeof(UINT)* m_uiAllIndexCount);

		pModelCopy->m_pIndexBuffer->Unlock();
		m_pIndexBuffer->Unlock();

		return pModelCopy;
	}

	DataStaticModel::~DataStaticModel()
	{
		mem_release_del(m_pVertexBuffer);
		mem_release_del(m_pIndexBuffer);

		for (int i = 0; i < m_uiSubsetCount; ++i)
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