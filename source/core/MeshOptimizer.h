#ifndef __MESHOPTIMIZER_H
#define __MESHOPTIMIZER_H

#include <gdefines.h>
#include <memory>

#define MESH_OPTIMIZE_DEFAULT_CACHE_SIZE 12
#define MESH_OPTIMIZE_DEFAULT_RESTART 7

#define UNUSED32 (0xffffffff)

namespace DirectX
{
	HRESULT __cdecl OptimizeFaces(
		_In_reads_(nFaces * 3) const uint16_t* indices, _In_ size_t nFaces,
		_In_reads_(nFaces * 3) const uint32_t* adjacency,
		_Out_writes_(nFaces) uint32_t* faceRemap,
		_In_ uint32_t vertexCache = MESH_OPTIMIZE_DEFAULT_CACHE_SIZE,
		_In_ uint32_t restart = MESH_OPTIMIZE_DEFAULT_RESTART);
	HRESULT __cdecl OptimizeFaces(
		_In_reads_(nFaces * 3) const uint32_t* indices, _In_ size_t nFaces,
		_In_reads_(nFaces * 3) const uint32_t* adjacency,
		_Out_writes_(nFaces) uint32_t* faceRemap,
		_In_ uint32_t vertexCache = MESH_OPTIMIZE_DEFAULT_CACHE_SIZE,
		_In_ uint32_t restart = MESH_OPTIMIZE_DEFAULT_RESTART);

	HRESULT __cdecl GenerateAdjacencyAndPointReps(
		_In_reads_(nFaces * 3) const uint16_t* indices, _In_ size_t nFaces,
		_In_reads_(nVerts) const float3* positions, _In_ size_t nVerts,
		_In_ float epsilon,
		_Out_writes_opt_(nVerts) uint32_t* pointRep,
		_Out_writes_opt_(nFaces * 3) uint32_t* adjacency);
	HRESULT __cdecl GenerateAdjacencyAndPointReps(
		_In_reads_(nFaces * 3) const uint32_t* indices, _In_ size_t nFaces,
		_In_reads_(nVerts) const float3* positions, _In_ size_t nVerts,
		_In_ float epsilon,
		_Out_writes_opt_(nVerts) uint32_t* pointRep,
		_Out_writes_opt_(nFaces * 3) uint32_t* adjacency);
	// If pointRep is null, it still generates them internally as they are needed for the final adjacency computation
};

template <typename VertexType, typename IndexType>
class CMeshOptimizer
{
public:
	static void Optimize(VertexType *pVertices, IndexType *pIndices, UINT uVertexCount, UINT uIndexCount, UINT uVertexCacheSize = MESH_OPTIMIZE_DEFAULT_CACHE_SIZE, UINT uRestart = MESH_OPTIMIZE_DEFAULT_RESTART)
	{
		assert(pVertices);
		assert(pIndices);
		assert(uVertexCount);
		assert(uIndexCount);

		//UINT *pFaceRemap = new UINT[uVertexCount];

		size_t nFaces = uIndexCount / 3;
		size_t nVerts = uVertexCount;

		std::unique_ptr<float3[]> pos(new float3[nVerts]);
		for(size_t j = 0; j < nVerts; ++j)
			pos[j] = pVertices[j].vPos;

		std::unique_ptr<uint32_t[]> adj(new uint32_t[uIndexCount]);
		if(FAILED(DirectX::GenerateAdjacencyAndPointReps(pIndices, nFaces,
			pos.get(), nVerts, 0.f, nullptr, adj.get())))
		{
			assert(!"Error");
			return;
		}

		std::unique_ptr<uint32_t[]> faceRemap(new uint32_t[nFaces]);
		if(FAILED(DirectX::OptimizeFaces(pIndices, nFaces, adj.get(), faceRemap.get())))
		{
			assert(!"Error");
			return;
		}

		std::unique_ptr<IndexType[]> pNewIndices(new IndexType[uIndexCount]);

		IndexType *pOutIndices = pNewIndices.get();
		for(size_t j = 0; j < nFaces; ++j)
		{
			uint32_t src = faceRemap[j];

			if(src == UNUSED32)
				continue;

			assert(src < nFaces);
			
			pOutIndices[j * 3] = pIndices[src * 3];
			pOutIndices[j * 3 + 1] = pIndices[src * 3 + 1];
			pOutIndices[j * 3 + 2] = pIndices[src * 3 + 2];
		}

		memcpy(pIndices, pOutIndices, sizeof(IndexType) * uIndexCount);


	}
};

#endif
