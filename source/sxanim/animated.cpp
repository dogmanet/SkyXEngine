
#include "animated.h"
#include <cstdlib>
#include <cstdio>
#include <common/sxtypes.h>
#include <gcore/sxgcore.h>

/**
*
* ModelFile implementation
*
*/
ModelFile::ModelFile(const char * name, AnimationManager * pMgr):
m_bLoaded(false),
m_bAssemblied(false),
m_pDeps(NULL),
pActivities(NULL),
m_pBones(NULL),
m_pControllers(NULL),
m_pSequences(NULL),
m_pLods(NULL),
m_iMaterials(NULL),
m_bInitPosInvSet(false),
m_pBonesBindPoseInv(false),
m_ppVertexBuffer(NULL),
m_ppIndexBuffer(NULL),
m_pMgr(pMgr),
m_bIsTemp(false),
m_pHitboxes(NULL),
m_pParts(NULL),
m_pBoundBox(NULL)
{
	memset(&m_hdr, 0, sizeof(ModelHeader));
	memset(&m_hdr2, 0, sizeof(ModelHeader2));
	if(name[0] && name[0] != '!')
	{
		Load(name);
	}
	else
	{
		strncpy(m_szFileName, name, MODEL_MAX_FILE);
	}
}

void ModelFile::Load(const char * name)
{
	strncpy(m_szFileName, name, MODEL_MAX_FILE);
	m_szFileName[MODEL_MAX_FILE - 1] = 0;
	FILE * fp = fopen(name, "rb");
	if(!fp)
	{
		Report(REPORT_MSG_LEVEL_ERROR, "Unable to open \"%s\"\n", name);
	}

	fread(&m_hdr, sizeof(ModelHeader), 1, fp);

	if(m_hdr.Magick != SX_MODEL_MAGICK)
	{
		Report(REPORT_MSG_LEVEL_ERROR, "Corrupt model \"%s\"\n", name);
		fclose(fp);
		return;
	}

	if(m_hdr.iVersion != SX_MODEL_VERSION)
	{
		Report(REPORT_MSG_LEVEL_ERROR, "Invalid version %d file \"%s\"\n", m_hdr.iVersion, name);
		fclose(fp);
		return;
	}

	if(m_hdr.iSecondHeaderOffset)
	{
		_fseeki64(fp, m_hdr.iSecondHeaderOffset, SEEK_SET);
		fread(&m_hdr2, sizeof(ModelHeader2), 1, fp);
	}

	if(m_hdr2.iDepsCount && m_hdr2.iDependensiesOffset)
	{
		_fseeki64(fp, m_hdr2.iDependensiesOffset, SEEK_SET);
		if(m_hdr.iFlags & MODEL_FLAG_NEW_STYLE_DEPS)
		{
			m_pParts = new ModelPart[m_hdr2.iDepsCount];
			fread(m_pParts, sizeof(ModelPart), m_hdr2.iDepsCount, fp);
			
		}
		else
		{
			ModelDependensy md;

			m_pDeps = new const ModelFile*[m_hdr2.iDepsCount];

			for(uint32_t i = 0; i < m_hdr2.iDepsCount; i++)
			{
				fread(&md, sizeof(ModelDependensy), 1, fp);
				m_pDeps[i] = m_pMgr->LoadModel(md.szName);
			}
		}
	}

	if(m_hdr2.iBoneTableOffset)
	{
		_fseeki64(fp, m_hdr2.iBoneTableOffset, SEEK_SET);
		m_pBones = new ModelBoneName[m_hdr2.iBoneTableCount];
		fread(m_pBones, sizeof(ModelBoneName), m_hdr2.iBoneTableCount, fp);
	}

	if(m_hdr2.iActivitiesTableCount && m_hdr2.iActivitiesTableOffset)
	{
		_fseeki64(fp, m_hdr2.iActivitiesTableOffset, SEEK_SET);
		pActivities = new ModelActivity[m_hdr2.iActivitiesTableCount];
		fread(pActivities, sizeof(ModelActivity), m_hdr2.iActivitiesTableCount, fp);
	}

	if(m_hdr2.iControllersCount && m_hdr2.iControllersOffset)
	{
		_fseeki64(fp, m_hdr2.iControllersOffset, SEEK_SET);
		m_pControllers = new ModelBoneController[m_hdr2.iControllersCount];
		fread(m_pControllers, sizeof(ModelBoneController), m_hdr2.iControllersCount, fp);
	}

	if(m_hdr.iMaterialsOffset)
	{
		_fseeki64(fp, m_hdr.iMaterialsOffset, SEEK_SET);

		m_iMaterials = new ModelMatrial*[m_hdr.iSkinCount];

		for(UINT j = 0; j < m_hdr.iSkinCount; j++)
		{
			m_iMaterials[j] = new ModelMatrial[m_hdr.iMaterialCount];
			for(uint32_t i = 0; i < m_hdr.iMaterialCount; i++)
			{
				fread(m_iMaterials[j][i].szName, 1, MODEL_MAX_NAME, fp);

				m_iMaterials[j][i].iMat = m_pMgr->GetMaterial(m_iMaterials[j][i].szName); //3

			}
		}
	}

	SGCore_LoadTexLoadTextures();

	if(m_hdr.iAnimationsOffset)
	{
		_fseeki64(fp, m_hdr.iAnimationsOffset, SEEK_SET);
		m_pSequences = new ModelSequence[m_hdr.iAnimationCount];

		for(UINT i = 0; i < m_hdr.iAnimationCount; i++)
		{
			fread(&m_pSequences[i], MODEL_SEQUENCE_STRUCT_SIZE, 1, fp);
			m_pSequences[i].m_vmAnimData = new ModelBone*[m_pSequences[i].iNumFrames];

			for(UINT j = 0; j < m_pSequences[i].iNumFrames; j++)
			{
				m_pSequences[i].m_vmAnimData[j] = new ModelBone[m_hdr.iBoneCount];
				fread(m_pSequences[i].m_vmAnimData[j], sizeof(ModelBone), m_hdr.iBoneCount, fp);
			}
			//m_mSeqNames[m_pSequences[i].name] = i;
		}
	}

	if(m_hdr.iLODcount && m_hdr.iLODoffset)
	{
		_fseeki64(fp, m_hdr.iLODoffset, SEEK_SET);

		m_pLods = new ModelLoD[m_hdr.iLODcount];

		for(uint32_t i = 0; i < m_hdr.iLODcount; i++)
		{
			fread(&m_pLods[i], MODEL_LOD_STRUCT_SIZE, 1, fp);
			int iVC = 0;
			m_pLods[i].pSubLODmeshes = new ModelLoDSubset[m_pLods[i].iSubMeshCount];
			for(uint32_t j = 0; j < m_pLods[i].iSubMeshCount; j++)
			{

				fread(&m_pLods[i].pSubLODmeshes[j].iMaterialID, sizeof(uint32_t), 1, fp);
				fread(&m_pLods[i].pSubLODmeshes[j].iVectexCount, sizeof(uint32_t), 1, fp);
				fread(&m_pLods[i].pSubLODmeshes[j].iIndexCount, sizeof(uint32_t), 1, fp);
				m_pLods[i].pSubLODmeshes[j].pVertices = new vertex_animated[m_pLods[i].pSubLODmeshes[j].iVectexCount];
				m_pLods[i].pSubLODmeshes[j].pIndices = new UINT[m_pLods[i].pSubLODmeshes[j].iIndexCount];
				fread(m_pLods[i].pSubLODmeshes[j].pVertices, sizeof(vertex_animated), m_pLods[i].pSubLODmeshes[j].iVectexCount, fp);
				fread(m_pLods[i].pSubLODmeshes[j].pIndices, sizeof(UINT), m_pLods[i].pSubLODmeshes[j].iIndexCount, fp);

				iVC += m_pLods[i].pSubLODmeshes[j].iVectexCount;
			}
		}
	}

	if(m_hdr2.iHitboxCount && m_hdr2.iHitboxesOffset)
	{
		_fseeki64(fp, m_hdr2.iHitboxesOffset, SEEK_SET);
		m_pHitboxes = new ModelHitbox[m_hdr2.iHitboxCount];
		fread(m_pHitboxes, sizeof(ModelHitbox), m_hdr2.iHitboxCount, fp);
	}

	fclose(fp);
	m_bLoaded = true;
}

void ModelFile::LoadParts()
{
	for(uint32_t i = 0; i < m_hdr2.iDepsCount; ++i)
	{
		m_pParts[i].pMdl = m_pMgr->LoadModel(m_pParts[i].file);
	}
}

void ModelFile::GetBoneName(UINT id, char * name, int len) const
{
	if(id >= m_hdr2.iBoneTableCount)
	{
		name[0] = 0;
		return;
	}

	strncpy(name, m_pBones[id].szName, len);
}

UINT ModelFile::GetBoneID(const char * name)
{
	for(uint32_t i = 0; i < m_hdr2.iBoneTableCount; i++)
	{
		if(_stricmp(m_pBones[i].szName, name) == 0)
		{
			return(m_pBones[i].bone.id);
		}
	}
	return(-1);
}

const ModelHitbox * ModelFile::GetHitbox(const char * name) const
{
	for(uint32_t i = 0; i < m_hdr2.iHitboxCount; ++i)
	{
		if(!strcmp(name, m_pHitboxes[i].name))
		{
			return(&m_pHitboxes[i]);
		}
	}
	return(NULL);
}
const ModelHitbox * ModelFile::GetHitbox(uint32_t id) const
{
	if(id >= m_hdr2.iHitboxCount)
	{
		return(NULL);
	}
	return(&m_pHitboxes[id]);
}
uint32_t ModelFile::GetHitboxCount() const
{
	return(m_hdr2.iHitboxCount);
}
void ModelFile::AddHitbox(const ModelHitbox * hb)
{
	++m_hdr2.iHitboxCount;
	ModelHitbox * tmp = new ModelHitbox[m_hdr2.iHitboxCount];
	if(m_pHitboxes)
	{
		memcpy(tmp, m_pHitboxes, sizeof(ModelHitbox)* (m_hdr2.iHitboxCount - 1));
		mem_delete_a(m_pHitboxes);
	}
	tmp[m_hdr2.iHitboxCount - 1] = *hb;
	m_pHitboxes = tmp;
}

void ModelFile::DelHitbox(const char * name)
{
	ModelHitbox * tmp;
	for(uint32_t i = 0; i < m_hdr2.iHitboxCount; ++i)
	{
		if(!strcmp(name, m_pHitboxes[i].name))
		{
			--m_hdr2.iHitboxCount;
			if(m_hdr2.iHitboxCount)
			{
				tmp = new ModelHitbox[m_hdr2.iHitboxCount];
				memcpy(tmp, m_pHitboxes, sizeof(ModelHitbox) * i);
				memcpy(tmp + i, m_pHitboxes + i + 1, sizeof(ModelHitbox)* (m_hdr2.iHitboxCount - i));
			}
			else
			{
				tmp = NULL;
			}
			mem_delete_a(m_pHitboxes);
			m_pHitboxes = tmp;
			break;
		}
	}
}

void ModelFile::DelHitbox(uint32_t i)
{
	if(i >= m_hdr2.iHitboxCount)
	{
		return;
	}

	ModelHitbox * tmp;
	
	--m_hdr2.iHitboxCount;
	if(m_hdr2.iHitboxCount)
	{
		tmp = new ModelHitbox[m_hdr2.iHitboxCount];
		memcpy(tmp, m_pHitboxes, sizeof(ModelHitbox)* i);
		memcpy(tmp + i, m_pHitboxes + i + 1, sizeof(ModelHitbox)* (m_hdr2.iHitboxCount - i));
	}
	else
	{
		tmp = NULL;
	}
	mem_delete_a(m_pHitboxes);
	m_pHitboxes = tmp;
}

const ModelSequence * ModelFile::GetSequence(UINT id) const
{
	if(GetSequenceCount() <= id)
	{
		return(NULL);
	}
	return(&m_pSequences[id]);
}
const ModelSequence * ModelFile::GetSequence(const char * name) const
{
	for(int i = 0, l = GetSequenceCount(); i < l; ++i)
	{
		if(!strcmp(m_pSequences[i].name, name))
		{
			return(&m_pSequences[i]);
		}
	}
	return(NULL);
}
const ModelBoneController * ModelFile::GetController(UINT id) const
{
	if(GetControllersCount() <= id)
	{
		return(NULL);
	}
	return(&m_pControllers[id]);
}
UINT ModelFile::GetBoneCount() const
{
	return(m_hdr.iBoneCount);
}
UINT ModelFile::GetSequenceCount() const
{
	return(m_hdr.iAnimationCount);
}
UINT ModelFile::GetControllersCount() const
{
	return(m_hdr2.iControllersCount);
}
#ifndef _SERVER
void ModelFile::Render(SMMATRIX * mWorld, UINT nSkin, UINT nLod)
{
	if(nSkin >= m_hdr.iSkinCount)
	{
		nSkin = 0;
	}

	if(nLod >= m_hdr.iLODcount)
	{
		return;
	}

	static UINT iVertSize[] = {
		sizeof(vertex_static),
		sizeof(vertex_static_ex),
		sizeof(vertex_animated),
		sizeof(vertex_animated_ex)
	};

	m_pMgr->m_pd3dDevice->SetIndices(m_ppIndexBuffer[nLod]);
	m_pMgr->m_pd3dDevice->SetStreamSource(0, m_ppVertexBuffer[nLod], 0, iVertSize[MVT_DYNAMIC]);

	m_pMgr->SetVertexDeclaration(MVT_DYNAMIC);

	for(UINT i = 0; i < m_pLods[nLod].iSubMeshCount; i++)
	{
		//m_pMgr->SetMaterial(m_iMaterials[nSkin][m_pLods[nLod].pSubLODmeshes[i].iMaterialID].iMat);
		SGCore_MtlSet(m_iMaterials[nSkin][m_pLods[nLod].pSubLODmeshes[i].iMaterialID].iMat, mWorld);

		SGCore_DIP(
			D3DPT_TRIANGLELIST,
			m_pLods[nLod].pSubLODmeshes[i].iStartVertex,
			0,
			m_pLods[nLod].pSubLODmeshes[i].iVectexCount,
			m_pLods[nLod].pSubLODmeshes[i].iStartIndex,
			m_pLods[nLod].pSubLODmeshes[i].iIndexCount / 3);
	}
	
}
#endif

MBERR ModelFile::AppendBones(const ModelFile * mdl, char * root)
{
	bool bRootFound = false;
	bool bRootLeft = false;

	if(!m_hdr2.iBoneTableCount)
	{
		//simply copy bones
		if(root)
		{
			return(MBE_NO_ROOT);
		}
		mem_delete_a(m_pBones);
		m_pBones = new ModelBoneName[mdl->m_hdr2.iBoneTableCount];
		memcpy(m_pBones, mdl->m_pBones, sizeof(ModelBoneName)* mdl->m_hdr2.iBoneTableCount);
		m_hdr.iBoneCount = m_hdr2.iBoneTableCount = mdl->m_hdr2.iBoneTableCount;
		return(MBE_SUCCESS);
	}
	if(root)
	{
		for(uint32_t i = 0; i < m_hdr2.iBoneTableCount; ++i)
		{
			if(!strcmp(m_pBones[i].szName, root))
			{
				bRootFound = true;
				bRootLeft = true;
				break;
			}
		}
	}
	else
	{
		if(mdl->m_pBones)
		{
			for(uint32_t i = 0; i < m_hdr2.iBoneTableCount; ++i)
			{
				if(!strcmp(m_pBones[i].szName, mdl->m_pBones[0].szName))
				{
					bRootFound = true;
					bRootLeft = true;
					break;
				}
			}
		}
		if(!bRootFound)
		{
			for(uint32_t i = 0; i < mdl->m_hdr2.iBoneTableCount; ++i)
			{
				if(!strcmp(m_pBones[0].szName, mdl->m_pBones[i].szName))
				{
					bRootFound = true;
					bRootLeft = false;
					break;
				}
			}
		}
	}
	if(!bRootFound)
	{
		return(MBE_NO_ROOT);
	}
	Array<ModelBoneName> vNewBones, vTmpBones;
	if(bRootLeft)
	{
		///start m_pBones[i] <- mdl->m_pBones[0]

		for(uint32_t i = 0; i < m_hdr2.iBoneTableCount; ++i)
		{
			vNewBones.push_back(m_pBones[i]);
		}
		if(root)
		{
			ModelBoneName tmp;
			strcpy(tmp.szName, root);
			tmp.bone.pid = -1;
			tmp.bone.id = 0;
			vTmpBones.push_back(tmp);
		}
		for(uint32_t i = 0; i < mdl->m_hdr2.iBoneTableCount; ++i)
		{
			vTmpBones.push_back(mdl->m_pBones[i]);
		}
		if(root)
		{
			for(int i = 1, l = vTmpBones.size(); i < l; ++i)
			{
				++vTmpBones[i].bone.id;
				++vTmpBones[i].bone.pid;
			}
		}
	}
	else
	{
		///start m_pBones[i] -> mdl->m_pBones[0]

		for(uint32_t i = 0; i <= mdl->m_hdr2.iBoneTableCount; ++i)
		{
			vNewBones.push_back(mdl->m_pBones[i]);
		}
		for(uint32_t i = 0; i < m_hdr2.iBoneTableCount; ++i)
		{
			vTmpBones.push_back(m_pBones[i]);
		}
	}

	for(int i = 0, l = vTmpBones.size(); i < l; ++i)
	{
		bool found = false;
		for(int j = 0, l2 = vNewBones.size(); j < l2; ++j)
		{
			if(!strcmp(vTmpBones[i].szName, vNewBones[j].szName))
			{
				if(vTmpBones[i].bone.pid >= 0 && vNewBones[j].bone.pid >= 0
					&& strcmp(mdl->m_pBones[vTmpBones[i].bone.pid].szName, vNewBones[vNewBones[j].bone.pid].szName))
				{
					return(MBE_BAD_HIERARCY);
				}
				found = true;
				break;
			}
		}
		if(!found)
		{
			ModelBoneName tmp = vTmpBones[i];
			for(int j = 0, l2 = vNewBones.size(); j < l2; ++j)
			{
				if(!strcmp(vTmpBones[tmp.bone.pid].szName, vNewBones[j].szName))
				{
					tmp.bone.id = vNewBones.size();
					tmp.bone.pid = j;
					vNewBones.push_back(tmp);
					break;
				}
			}
		}
	}

	mem_delete_a(m_pBones);
	m_pBones = new ModelBoneName[vNewBones.size()];
	memcpy(m_pBones, &vNewBones[0], sizeof(ModelBoneName)* vNewBones.size());
	m_hdr.iBoneCount = m_hdr2.iBoneTableCount = vNewBones.size();
	return(MBE_SUCCESS);
}

void ModelFile::BuildMeshBuffers()
{
	UINT m_iLodCount = m_hdr.iLODcount;
	if(m_ppIndexBuffer && m_ppVertexBuffer)
	{
		for(UINT j = 0; j < m_iLodCount; j++)
		{
			mem_release(m_ppIndexBuffer[j]);
			mem_release(m_ppVertexBuffer[j]);
		}
	}
	mem_delete_a(m_ppIndexBuffer);
	mem_delete_a(m_ppVertexBuffer);

	mem_release(m_pBoundBox);

	m_ppIndexBuffer = new IDirect3DIndexBuffer9 *[m_iLodCount];
	m_ppVertexBuffer = new IDirect3DVertexBuffer9 *[m_iLodCount];

	m_pBoundBox = SGCore_CrBound();
	DWORD tmpCountVert = 0;
	for(UINT j = 0; j < m_iLodCount; ++j)
	{
		UINT iStartIndex = 0;
		UINT iStartVertex = 0;
		for(uint32_t i = 0; i < m_pLods[j].iSubMeshCount; ++i)
		{
			m_pLods[j].pSubLODmeshes[i].iStartIndex = iStartIndex;
			m_pLods[j].pSubLODmeshes[i].iStartVertex = iStartVertex;

			iStartIndex += m_pLods[j].pSubLODmeshes[i].iIndexCount;
			iStartVertex += m_pLods[j].pSubLODmeshes[i].iVectexCount;
		}

		tmpCountVert = iStartVertex;

		UINT * pIndices = new UINT[iStartIndex];
		vertex_animated * pVertices = new vertex_animated[iStartVertex];
		ModelLoDSubset * pSM;
		for(uint32_t i = 0; i < m_pLods[j].iSubMeshCount; ++i)
		{
			pSM = &m_pLods[j].pSubLODmeshes[i];
			memcpy(pIndices + m_pLods[j].pSubLODmeshes[i].iStartIndex, m_pLods[j].pSubLODmeshes[i].pIndices, sizeof(UINT) * m_pLods[j].pSubLODmeshes[i].iIndexCount);
			memcpy(pVertices + m_pLods[j].pSubLODmeshes[i].iStartVertex, m_pLods[j].pSubLODmeshes[i].pVertices, sizeof(vertex_animated) * m_pLods[j].pSubLODmeshes[i].iVectexCount);
		}

		m_pMgr->m_pd3dDevice->CreateVertexBuffer(sizeof(vertex_animated) * iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &m_ppVertexBuffer[j], 0);
		VOID * pData;
		if(!FAILED(m_ppVertexBuffer[j]->Lock(0, sizeof(vertex_animated) * iStartVertex, (void**)&pData, 0)))
		{
			memcpy(pData, pVertices, sizeof(vertex_animated) * iStartVertex);
			m_ppVertexBuffer[j]->Unlock();
		}

		m_pMgr->m_pd3dDevice->CreateIndexBuffer(sizeof(UINT) * iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_ppIndexBuffer[j], 0);
		if(!FAILED(m_ppIndexBuffer[j]->Lock(0, sizeof(UINT) * iStartIndex, (void**)&pData, 0)))
		{
			memcpy(pData, pIndices, sizeof(UINT) * iStartIndex);
			m_ppIndexBuffer[j]->Unlock();
		}
		mem_delete_a(pVertices);
		mem_delete_a(pIndices);

		if(j == 0)
		{
			m_pBoundBox->CalcBound(m_ppVertexBuffer[0], iStartVertex, sizeof(vertex_animated));
		}
	}

}

const ISXBound * ModelFile::GetBound() const
{
	return(m_pBoundBox);
}

ModelFile::~ModelFile()
{
#ifndef _SERVER
	if(m_ppIndexBuffer && m_ppVertexBuffer)
	{
		for(uint32_t j = 0; j < m_hdr.iLODcount; ++j)
		{
			mem_release(m_ppIndexBuffer[j]); 		
			mem_release(m_ppVertexBuffer[j]);
		}
	} 		
	mem_delete_a(m_ppIndexBuffer);
	mem_delete_a(m_ppVertexBuffer);
#endif

	mem_delete_a(m_pBones);
	mem_delete_a(m_pBonesBindPoseInv);
	mem_delete_a(pActivities);
	mem_delete_a(m_pControllers);

	mem_delete_a(m_pDeps);

	if(m_iMaterials)
	{
		for(uint32_t j = 0; j < m_hdr.iSkinCount; ++j)
		{
			mem_delete_a(m_iMaterials[j]);
		}
		mem_delete_a(m_iMaterials);
	}

	for(uint32_t i = 0; i < m_hdr.iAnimationCount; ++i)
	{
		for(uint32_t j = 0; j < m_pSequences[i].iNumFrames; ++j)
		{
			mem_delete_a(m_pSequences[i].m_vmAnimData[j]);
		}
		mem_delete_a(m_pSequences[i].m_vmAnimData);
	}
	mem_delete_a(m_pSequences);


	for(uint32_t i = 0; i < m_hdr.iLODcount; ++i)
	{
		for(uint32_t j = 0; j < m_pLods[i].iSubMeshCount; ++j)
		{
			mem_delete_a(m_pLods[i].pSubLODmeshes[j].pVertices);
			mem_delete_a(m_pLods[i].pSubLODmeshes[j].pIndices);
		}
		mem_delete_a(m_pLods[i].pSubLODmeshes);
	}
	mem_delete_a(m_pLods);
	mem_delete_a(m_pHitboxes);
	if(m_hdr.iFlags & MODEL_FLAG_NEW_STYLE_DEPS)
	{
		for(uint32_t i = 0; i < m_hdr2.iDepsCount; ++i)
		{
			m_pMgr->UnloadModel(m_pParts[i].pMdl);
		}
	}
	mem_delete_a(m_pParts);
}

bool ModelFile::Save(const char * file)
{
	FILE * fp = fopen(file, "wb");
	if(!fp)
	{
		return(false);
	}

	m_hdr.Magick = SX_MODEL_MAGICK;
	m_hdr.iVersion = SX_MODEL_VERSION;

	fwrite(&m_hdr, sizeof(ModelHeader), 1, fp);
	m_hdr.iSecondHeaderOffset = ftell(fp);
	fwrite(&m_hdr2, sizeof(ModelHeader2), 1, fp);

	//write bones
	m_hdr2.iBoneTableOffset = ftell(fp);
	fwrite(m_pBones, sizeof(ModelBoneName), m_hdr2.iBoneTableCount, fp);

	//write activities
	if(pActivities)
	{
		m_hdr2.iActivitiesTableOffset = ftell(fp);
		fwrite(pActivities, sizeof(ModelActivity), m_hdr2.iActivitiesTableCount, fp);
	}

	//write controllers
	if(m_pControllers)
	{
		m_hdr2.iControllersOffset = ftell(fp);
		fwrite(m_pControllers, sizeof(ModelBoneController), m_hdr2.iControllersCount, fp);
	}

	//write materials
	if(m_iMaterials)
	{
		m_hdr.iMaterialsOffset = ftell(fp);
		for(UINT j = 0; j < m_hdr.iSkinCount; j++)
		{
			for(uint32_t i = 0; i < m_hdr.iMaterialCount; i++)
			{
				fwrite(m_iMaterials[j][i].szName, 1, MODEL_MAX_NAME, fp);
			}
		}
	}

	//write animations
	if(m_pSequences)
	{
		m_hdr.iAnimationsOffset = ftell(fp);
		for(UINT i = 0; i < m_hdr.iAnimationCount; i++)
		{
			//@TODO: check activity index
			fwrite(&m_pSequences[i], MODEL_SEQUENCE_STRUCT_SIZE, 1, fp);
			for(UINT j = 0; j < m_pSequences[i].iNumFrames; j++)
			{
				fwrite(m_pSequences[i].m_vmAnimData[j], sizeof(ModelBone), m_hdr.iBoneCount, fp);
			}
		}
	}

	//write lods
	if(m_pLods)
	{
		m_hdr.iLODoffset = ftell(fp);
		for(uint32_t i = 0; i < m_hdr.iLODcount; i++)
		{
			fwrite(&m_pLods[i], MODEL_LOD_STRUCT_SIZE, 1, fp);
			for(uint32_t j = 0; j < m_pLods[i].iSubMeshCount; j++)
			{
				fwrite(&m_pLods[i].pSubLODmeshes[j].iMaterialID, sizeof(uint32_t), 1, fp);
				fwrite(&m_pLods[i].pSubLODmeshes[j].iVectexCount, sizeof(uint32_t), 1, fp);
				fwrite(&m_pLods[i].pSubLODmeshes[j].iIndexCount, sizeof(uint32_t), 1, fp);
				
				fwrite(m_pLods[i].pSubLODmeshes[j].pVertices, sizeof(vertex_animated), m_pLods[i].pSubLODmeshes[j].iVectexCount, fp);
				fwrite(m_pLods[i].pSubLODmeshes[j].pIndices, sizeof(UINT), m_pLods[i].pSubLODmeshes[j].iIndexCount, fp);
			}
		}
	}

	//write hitboxes
	if(m_pHitboxes)
	{
		m_hdr2.iHitboxesOffset = ftell(fp);
		fwrite(m_pHitboxes, sizeof(ModelHitbox), m_hdr2.iHitboxCount, fp);
	}

	//write parts
	if(m_pParts)
	{
		m_hdr2.iDependensiesOffset = ftell(fp);
		fwrite(m_pParts, sizeof(ModelPart), m_hdr2.iDepsCount, fp);
	}

	m_hdr.iFlags |= MODEL_FLAG_COMPILED | MODEL_FLAG_NEW_STYLE_DEPS;

	fseek(fp, 0, SEEK_SET);
	fwrite(&m_hdr, sizeof(ModelHeader), 1, fp);
	fwrite(&m_hdr2, sizeof(ModelHeader2), 1, fp);

	fclose(fp);
	return(true);
}



/**
*
* Animation implementation
*
*/

#define RUN_CB(slot, state) if(m_pfnCallBack) m_pfnCallBack(slot, state, this)
#define RUN_P_CB(slot, state) if(m_pfnProgressCB) m_pfnProgressCB(slot, state, this)

Animation::Animation(AnimationManager * pMgr):
//m_bUpdating(false),
m_pBoneControllers(NULL),
//m_fTestBoneCtl(NULL),
m_bBoneMatrixReFilled(false),
m_pMdl(NULL),
//m_bRendering(false),
//m_mfBoneControllers(NULL),
m_vPosition(float3_t(0.0f, 0.0f, 0.0f)),
m_vOrientation(SMQuaternion(0.0f, 0.0f, 0.0f, 1.0f)),
m_iCurTime(0),
//m_bRenderReady(false),
m_pBoneMatrix(NULL),
m_FinalBones(NULL),
m_pBoneMatrixRender(NULL),
m_mfBoneControllerValues(NULL),
m_iCurrentSkin(0),
m_pMgr(pMgr),
m_pfnCallBack(NULL),
m_pfnProgressCB(NULL)
{
	for(int i = 0; i < BLEND_MAX; i++)
	{
		m_bIsAnimationPlaying[i] = false;
		m_iFadeTime[i] = 0;
		m_iFadeCurTime[i] = 0;
		m_bInFade[i] = false;
		m_bNewAnimPlayed[i] = false;
		m_LastFrameBones[i] = NULL;
		m_CurrentBones[i] = NULL;
		m_bDoAdvance[i] = true;
		t[i] = 0;
	}

	myId = pMgr->Register(this);
}

Animation::~Animation()
{
	mem_delete_a(m_pBoneControllers);
	mem_delete_a(m_mfBoneControllerValues);
	mem_delete_a(m_pBoneMatrix);
	mem_delete_a(m_FinalBones);
	mem_delete_a(m_pBoneMatrixRender);
	m_pMgr->UnRegister(myId);
}

SMMATRIX Animation::GetBoneTransform(UINT id)
{
	//id *= 2;
	float3 pos = m_pBoneMatrix[id].position;
	SMQuaternion q = m_pBoneMatrix[id].orient;
	return(q.GetMatrix() * SMMatrixTranslation(pos));
}
/*
bool Animation::IsVisibleFrustum(Core::ControllMoving::Frustum* frustum)
{
	m_pMdl->BoundVol.GetSphere(&jcenter2, &jradius);
	jcenter = SMVector3Transform(jcenter2, GetWorldTM());

	if((frustum->SphereInFrustum(&jcenter, jradius)))
		return true;
	else
		return false;
}*/

void Animation::SyncAnims()
{
	int seqCount = m_pMdl->GetSequenceCount();
	m_mSeqIds.clear();
	for(int i = 0; i < seqCount; ++i)
	{
		const ModelSequence * pSeq = m_pMdl->GetSequence(i);
		m_mSeqIds[pSeq->name] = i;
	}
}

void Animation::SetModel(const char * file)
{
	m_pMdl = const_cast<ModelFile*>(m_pMgr->LoadModel(file));
	if(!m_pMdl)
	{
		return;
	}
	DownloadData();
}

void Animation::DownloadData()
{
	mem_delete_a(m_pBoneMatrix);
	mem_delete_a(m_FinalBones);
	mem_delete_a(m_pBoneMatrixRender);

	m_iBoneCount = m_pMdl->GetBoneCount();

	SyncAnims();

	ctlCount = m_pMdl->GetControllersCount();
	for(int i = 0; i < ctlCount; i++)
	{
		const ModelBoneController * pSeq = m_pMdl->GetController(i);
		m_mfBoneControllers[pSeq->szName] = i;
	}
	m_mfBoneControllerValues = new ModelBoneCrontrollerValue[ctlCount];
	memset(m_mfBoneControllerValues, 0, sizeof(ModelBoneCrontrollerValue) * ctlCount);

	m_pBoneControllers = new ModelBoneCrontrollerValue[m_iBoneCount];
	memset(m_pBoneControllers, 0, sizeof(ModelBoneCrontrollerValue) * m_iBoneCount);

	UpdateControllers();

	m_iCurTime = GetTickCount();

	for(UINT slot = 0; slot < BLEND_MAX; slot++)
	{
		m_CurrentBones[slot] = new ModelBone[m_iBoneCount];
	}

	const ModelBoneName * pBindData = m_pMdl->m_pBones;
	for(UINT i = 0; i < m_iBoneCount; i++)
	{
		for(UINT slot = 0; slot < BLEND_MAX; slot++)
		{
			if(!m_CurrentBones[slot])
			{
				m_CurrentBones[slot] = new ModelBone[m_iBoneCount];
			}
			m_CurrentBones[slot][i] = pBindData[i].bone;
		}
	}

	FillBoneMatrix();
}

ModelSequence const * Animation::GetCurAnim(int slot)
{
	return(m_pMdl->GetSequence(m_iPlayingAnim[slot]));
}

void Animation::Advance(unsigned long int dt)
{
	int deltat = dt - m_iCurTime;
	m_iCurTime = dt;
	UINT deltat_orig = deltat;

	for(int slot = 0; slot < BLEND_MAX; slot++)
	{
		deltat = deltat_orig;
		if(m_bInFade[slot])
		{
			if(!m_LastFrameBones[slot])
			{
				m_LastFrameBones[slot] = new ModelBone[m_iBoneCount];
			}
			if(m_iFadeCurTime[slot] == 0)
			{
				for(UINT i = 0; i < m_iBoneCount; i++)
				{
					m_LastFrameBones[slot][i] = m_CurrentBones[slot][i];
				}
			}

			m_iFadeCurTime[slot] += deltat;

			const ModelSequence * pCurAnim = m_pMdl->GetSequence(m_iPlayingAnim[slot]);

			bool rev = pCurAnim->framerate < 0;
			int frame = 0;
			if(rev)
			{
				frame = m_iAnimFrameCount[slot] - 2;
			}

			float delta = (float)m_iFadeCurTime[slot] / (float)m_iFadeTime[slot];
			for(UINT i = 0; i < m_iBoneCount; i++)
			{
				m_CurrentBones[slot][i] = m_LastFrameBones[slot][i];
				m_CurrentBones[slot][i].orient = SMquaternionSlerp(m_LastFrameBones[slot][i].orient, pCurAnim->m_vmAnimData[frame][i].orient * m_pBoneControllers[i].rot, delta).Normalize();
				m_CurrentBones[slot][i].position = (float3_t)(m_LastFrameBones[slot][i].position + (pCurAnim->m_vmAnimData[frame][i].position + m_pBoneControllers[i].tr - m_LastFrameBones[slot][i].position) * delta);
			}
			FillBoneMatrix();

			if(m_iFadeCurTime[slot] >= m_iFadeTime[slot])
			{
				m_bInFade[slot] = false;
				deltat = m_iFadeCurTime[slot] - m_iFadeTime[slot];
			}
			else
			{
				continue;
			}
		}

		if(m_bIsAnimationPlaying[slot])
		{
			if(!m_iAnimFrameCount[slot])
			{
				m_bIsAnimationPlaying[slot] = false;
				RUN_P_CB(slot, 0);
				RUN_CB(slot, AS_STOP);
				continue;
			}

			const ModelSequence * pCurAnim = m_pMdl->GetSequence(m_iPlayingAnim[slot]);
			//static UINT t[BLEND_MAX] = {0};
			int fr = 1;
			bool rev = pCurAnim->framerate < 0;
			if(pCurAnim->framerate)
			{
				fr = 1000 / pCurAnim->framerate;
			}
			if(rev)
			{
				fr = -fr;
			}
			if(m_bNewAnimPlayed[slot])
			{
				t[slot] = rev ? fr : 0;
				m_bNewAnimPlayed[slot] = false;
				if(rev)
				{
					m_iCurrentFrame[slot] = -2;
				}
			}
			if(pCurAnim->framerate)
			{
				if(m_bDoAdvance[slot])
				{
					t[slot] += rev ? -deltat : deltat;
				}
				bool cont = false;
				while((t[slot] >= fr && !rev) || (t[slot] <= 0 && rev))
				{
					if(rev)
					{
						--m_iCurrentFrame[slot];
					}
					else
					{
						++m_iCurrentFrame[slot];
					}
					if((!rev && m_iCurrentFrame[slot] >= (int)m_iAnimFrameCount[slot] - 1) || (rev && m_iCurrentFrame[slot] <= 0))
					{
						if(pCurAnim->bLooped)
						{
							RUN_CB(slot, AS_LOOP);
						}
						else
						{
							m_bIsAnimationPlaying[slot] = false;
							//StopAnimations();
							RUN_CB(slot, AS_STOP);
							cont = true;
							break;
						}
					}
					if(rev)
					{
						t[slot] += fr;
					}
					else
					{
						t[slot] -= fr;
					}
				}
				if(cont)
				{
					continue;
				}
				if(m_iCurrentFrame[slot] < 0)
				{
					m_iCurrentFrame[slot] += m_iAnimFrameCount[slot];
				}
				m_iCurrentFrame[slot] %= m_iAnimFrameCount[slot];
				
			}
			//char out[256];
			//sprintf(out, "=%d\n", m_iCurrentFrame[slot]);
			//OutputDebugStringA(out);
			int nextFrame = m_iCurrentFrame[slot] + (rev ? -1 : 1);
			if(nextFrame < 0)
			{
				nextFrame += m_iAnimFrameCount[slot];
			}
			nextFrame %= m_iAnimFrameCount[slot];

			float delta = (float)t[slot] / (float)fr;
			if(rev)
			{
				delta = 1.0f - delta;
			}

			//char out[256];
			//sprintf(out, "=%d %f\n", m_iCurrentFrame[slot], delta);
			//OutputDebugStringA(out);

			RUN_P_CB(slot, (((float)m_iCurrentFrame[slot] + (rev ? -delta : delta)) / (float)m_iAnimFrameCount[slot]));
			//SXString bname;
			//delta = 0;
			for(UINT i = 0; i < m_iBoneCount; i++)
			{
				m_CurrentBones[slot][i] = pCurAnim->m_vmAnimData[m_iCurrentFrame[slot]][i];
				m_CurrentBones[slot][i].orient = (SMquaternionSlerp(pCurAnim->m_vmAnimData[m_iCurrentFrame[slot]][i].orient, pCurAnim->m_vmAnimData[nextFrame][i].orient, delta) * m_pBoneControllers[i].rot).Normalize();
				m_CurrentBones[slot][i].position = (float3_t)(pCurAnim->m_vmAnimData[m_iCurrentFrame[slot]][i].position + (pCurAnim->m_vmAnimData[nextFrame][i].position - pCurAnim->m_vmAnimData[m_iCurrentFrame[slot]][i].position) * delta + m_pBoneControllers[i].tr);
			}
		}
		FillBoneMatrix();
		//pCurAnim->
	}
}

inline bool Animation::PlayingAnimations()
{
	for(int i = 0; i<BLEND_MAX; i++)
	{
		if(m_bIsAnimationPlaying[i])
			return true;
	}
	return false;
}

void Animation::StopAll()
{
	for(int i = 0; i < BLEND_MAX; ++i)
	{
		if(m_bIsAnimationPlaying[i])
		{
			m_bIsAnimationPlaying[i] = false;
			m_bInFade[i] = false;
			RUN_CB(i, AS_STOP);
		}
	}
}

void Animation::SetProgress(float progress, UINT slot)
{
	const ModelSequence * pCurAnim = m_pMdl->GetSequence(m_iPlayingAnim[slot]);
	if(!pCurAnim)
	{
		return;
	}
	UINT fr = 1;
	if(pCurAnim->framerate)
	{
		fr = 1000 / pCurAnim->framerate;
	}
	m_iCurrentFrame[slot] = (int)(m_iAnimFrameCount[slot] * progress);
	t[slot] = (int)((progress - (float)m_iCurrentFrame[slot] / (float)m_iAnimFrameCount[slot]) * fr * m_iAnimFrameCount[slot]);


}

void Animation::SetAdvance(bool set, UINT slot)
{
	m_bDoAdvance[slot] = set;
}

void Animation::Stop(UINT slot)
{
	if(m_bIsAnimationPlaying[slot])
	{
		m_bIsAnimationPlaying[slot] = false;
		RUN_CB(slot, AS_STOP);
	}
}

void Animation::Resume(UINT slot)
{
	if(!m_bIsAnimationPlaying[slot])
	{
		m_bIsAnimationPlaying[slot] = true;
		RUN_CB(slot, AS_PLAY);
	}
}

void Animation::FillBoneMatrix()
{
	//AssotiativeArray<int, SXmodel::boneID> * mBonesOrig = m_pMdl->GetBindPoseOrig();
	//const AssotiativeArray<int, SXmodel::boneID> * mBonesBind = m_pMdl->GetBindPose();
	if(!m_pMdl)
	{
		return;
	}
	ModelBone * mBonesOrig = m_pMdl->m_pBonesBindPoseInv;
	const ModelBoneName * mBonesBind = m_pMdl->m_pBones;

	if(!mBonesOrig)
	{
		m_pMdl->m_pBonesBindPoseInv = mBonesOrig = new ModelBone[m_iBoneCount];
	}

	if(!m_pBoneMatrix)
	{
		m_pBoneMatrix = new ModelBoneShader[m_iBoneCount];
	}
	if(!m_FinalBones)
	{
		m_FinalBones = new ModelBone[m_iBoneCount];
	}
	if(!m_pBoneMatrixRender)
	{
		m_pBoneMatrixRender = new ModelBoneShader[m_iBoneCount];
	}

	for(UINT i = 0; i < m_iBoneCount; i++)
	{
		m_FinalBones[i].orient = SMQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
		m_FinalBones[i].position = float3_t(0.0f, 0.0f, 0.0f);
	}

	//рассчитываем суммарную анимацию
	for(UINT slot = 0; slot < BLEND_MAX; slot++)
	{
		for(UINT i = 0; i < m_iBoneCount; i++)
		{
			if(slot == 0)
			{
				m_FinalBones[i].id = m_CurrentBones[slot][i].id;
				m_FinalBones[i].pid = m_CurrentBones[slot][i].pid;
			}

			/*	if(m_FinalBones[i].pid < 0)
			{
			SMQuaternion np = mBonesBind[m_FinalBones[i].id].bone.orient;
			}
			else
			{
			SMQuaternion np = mBonesBind[m_FinalBones[i].id].bone.orient * mBonesBind[m_FinalBones[i].pid].bone.orient.Conjugate();
			}*/


			m_FinalBones[i].position = (float3)(m_FinalBones[i].position + (m_CurrentBones[slot][i].position - mBonesBind[m_FinalBones[i].id].bone.position));
			m_FinalBones[i].orient = m_FinalBones[i].orient * (m_CurrentBones[slot][i].orient * mBonesBind[m_FinalBones[i].id].bone.orient.Conjugate());
		}
	}
	for(UINT i = 0; i < m_iBoneCount; i++)
	{
		m_FinalBones[i].position = (float3)(m_FinalBones[i].position + (mBonesBind[m_FinalBones[i].id].bone.position));
		m_FinalBones[i].orient = m_FinalBones[i].orient * mBonesBind[m_FinalBones[i].id].bone.orient;
	}


	//m_bUpdating = true;

	//Переводим рассчитанный скелет в Мировую СК
		{
			for(UINT i = 0; i < m_iBoneCount; i++)
			{
				m_pBoneMatrix[i].orient = m_FinalBones[i].orient;
				m_pBoneMatrix[i].position = (float4)m_FinalBones[i].position;
			}

			for(UINT i = 0; i < m_iBoneCount; i++)
			{
				if(m_FinalBones[i].pid != -1 && m_FinalBones[i].id > 0)
				{
					m_pBoneMatrix[i].orient = (m_pBoneMatrix[i].orient * m_pBoneMatrix[m_FinalBones[i].pid].orient).Normalize();

					float4 p = float4(m_pBoneMatrix[m_FinalBones[i].pid].orient * (float3)m_pBoneMatrix[i].position, 1.0f) + m_pBoneMatrix[m_FinalBones[i].pid].position;
					m_pBoneMatrix[i].position = p;
				}
			}
		}


		//Рассчитываем инвертированные трансформации позы скиннинга (если еще не сделали этого)
		if(!m_pMdl->m_bInitPosInvSet)
		{
			for(UINT i = 0; i < m_iBoneCount; i++)
			{
				mBonesOrig[i].orient = mBonesBind[i].bone.orient;
				mBonesOrig[i].position = mBonesBind[i].bone.position;
			}
			for(UINT i = 0; i < m_iBoneCount; i++)
			{
				if(m_FinalBones[i].pid != -1 && m_FinalBones[i].id > 0)
				{
					mBonesOrig[i].orient = (mBonesOrig[i].orient * mBonesOrig[m_FinalBones[i].pid].orient).Normalize();
					mBonesOrig[i].position = (float3)(float4(mBonesOrig[m_FinalBones[i].pid].orient * mBonesOrig[i].position, 1.0f) + mBonesOrig[m_FinalBones[i].pid].position);
				}
				//	wprintf(L"bID: %2d;\tParent: %2d;\tName: %s;\tPos: %f %f %f;\tQuat: %f %f %f %f\n", (*mBonesOrig)[m_FinalBones[i].id].id, (*mBonesOrig)[m_FinalBones[i].id].pid, (*mBonesOrig)[m_FinalBones[i].id].name.c_str(), (*mBonesOrig)[m_FinalBones[i].id].position.x, (*mBonesOrig)[m_FinalBones[i].id].position.y, (*mBonesOrig)[m_FinalBones[i].id].position.z, (*mBonesOrig)[m_FinalBones[i].id].orient.x, (*mBonesOrig)[m_FinalBones[i].id].orient.y, (*mBonesOrig)[m_FinalBones[i].id].orient.z, (*mBonesOrig)[m_FinalBones[i].id].orient.w);
			}
			for(UINT i = 0; i < m_iBoneCount; i++)
			{
				mBonesOrig[i].orient = mBonesOrig[i].orient.Conjugate();
				mBonesOrig[i].position = (float3)-mBonesOrig[i].position;
			}
			m_pMdl->m_bInitPosInvSet = true;
		}

		//Домножаем на инвертированные трансформации позы скиннинга
		for(UINT i = 0; i < m_iBoneCount; i++)
		{
			SMQuaternion q = mBonesOrig[i].orient * m_pBoneMatrix[i].orient;
			m_pBoneMatrix[i].orient = q;

			m_pBoneMatrix[i].position = (float4)(m_pBoneMatrix[i].position + float4(q * (float3)mBonesOrig[i].position, 1.0));

			//m_pBoneMatrix[i].orient = SMQuaternion(0, 0, 0, 1);
			//m_pBoneMatrix[i].position = float4_t(0, 0, 0, 0);
		}

		//m_bUpdating = false;

		m_bBoneMatrixReFilled = true;
}

inline bool Animation::PlayingAnimations(const char* name)
{
	for(int i = 0; i<BLEND_MAX; i++)
	{
		if(m_bIsAnimationPlaying[i] && m_mSeqIds[name] == m_iPlayingAnim[i])
			return true;
	}
	return false;
}

void Animation::Play(const char * name, UINT iFadeTime, UINT slot) // name: Animation name; changeTime: time to fade to this animation from previous
{
	if(!m_pMdl)
	{
		return;
	}
	if(slot >= BLEND_MAX)
	{
		Report(REPORT_MSG_LEVEL_WARRNING, "Unable to play animation \"%s\" Invalid slot %d, max valid slot is %d\n", name, slot, BLEND_MAX - 1);
		return;
	}
	if(!m_mSeqIds.KeyExists(name))
	{
		Report(REPORT_MSG_LEVEL_WARRNING, "Unable to play animation \"%s\"\n", name);
		return;
	}
	UINT sid = m_mSeqIds[name];
	m_iPlayingAnim[slot] = sid;

	m_bIsAnimationPlaying[slot] = true;
	m_iAnimFrameCount[slot] = m_pMdl->GetSequence(sid)->iNumFrames;
	m_iCurrentFrame[slot] = 0;
	m_bNewAnimPlayed[slot] = true;

	RUN_CB(slot, AS_PLAY);

	if(iFadeTime)
	{
		m_iFadeTime[slot] = iFadeTime;
		m_iFadeCurTime[slot] = 0;
		m_bInFade[slot] = true;
	}
}

void Animation::SetBoneController(const String & name, float value, MODEL_BONE_CTL what)
{
	if(m_mfBoneControllers.KeyExists(name))
	{
		UINT cid = m_mfBoneControllers[name];
		switch(what)
		{
		case MBCTL_ROT_X:
			m_mfBoneControllerValues[cid].rot.x = value;
			break;
		case MBCTL_ROT_Y:
			m_mfBoneControllerValues[cid].rot.y = value;
			break;
		case MBCTL_ROT_Z:
			m_mfBoneControllerValues[cid].rot.z = value;
			break;
		case MBCTL_TRANS_X:
			m_mfBoneControllerValues[cid].tr.x = value;
			break;
		case MBCTL_TRANS_Y:
			m_mfBoneControllerValues[cid].tr.y = value;
			break;
		case MBCTL_TRANS_Z:
			m_mfBoneControllerValues[cid].tr.z = value;
			break;
		}
	}
}

void Animation::UpdateControllers()
{
	for(UINT i = 0; i < m_iBoneCount; i++)
	{
		m_pBoneControllers[i].rot = SMQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
		m_pBoneControllers[i].tr = float3_t(0.0f, 0.0f, 0.0f);
	}
	for(int i = 0; i < ctlCount; i++)
	{
		const ModelBoneController * ctl = m_pMdl->GetController(i);
		for(UINT j = 0; j < ctl->iBoneCount; j++)
		{
			float3_t d = ctl->fMaxTrans - ctl->fMinTrans;
			d.x *= m_mfBoneControllerValues[i].tr.x;
			d.y *= m_mfBoneControllerValues[i].tr.y;
			d.z *= m_mfBoneControllerValues[i].tr.z;

			m_pBoneControllers[ctl->bones[j]].tr = (float3)(m_pBoneControllers[ctl->bones[j]].tr + d + ctl->fMinTrans);

			m_pBoneControllers[ctl->bones[j]].rot = m_pBoneControllers[ctl->bones[j]].rot * SMquaternionSlerp(SMQuaternion(ctl->fMinRot.x / 180.0f * 3.1415f, 'x'), SMQuaternion(ctl->fMaxRot.x / 180.0f * 3.1415f, 'x'), m_mfBoneControllerValues[i].rot.x)
				* SMquaternionSlerp(SMQuaternion(ctl->fMinRot.y / 180.0f * 3.1415f, 'y'), SMQuaternion(ctl->fMaxRot.y / 180.0f * 3.1415f, 'y'), m_mfBoneControllerValues[i].rot.y)
				* SMquaternionSlerp(SMQuaternion(ctl->fMinRot.z / 180.0f * 3.1415f, 'z'), SMQuaternion(ctl->fMaxRot.z / 180.0f * 3.1415f, 'z'), m_mfBoneControllerValues[i].rot.z);
		}
	}
}

void Animation::StartActivity(const String & name, UINT iFadeTime, UINT slot)
{
	//SX_ASSERT_MSG(false, "Implement me first");
}

int Animation::GetActiveSkin()
{
	return(m_iCurrentSkin);
}

void Animation::SetSkin(int n)
{
	m_iCurrentSkin = n;
}

void Animation::SetPos(const float3 & pos)
{
	m_vPosition = pos;
}

float3 Animation::GetPos()
{
	return(m_vPosition);
}

SMMATRIX Animation::GetWorldTM()
{
	return(SMMatrixScaling(0.01f, 0.01f, 0.01f) * m_vOrientation.GetMatrix() * SMMatrixTranslation(m_vPosition));
}

void Animation::SetOrient(const SMQuaternion & pos)
{
	m_vOrientation = pos;
}

SMQuaternion Animation::GetOrient()
{
	return(m_vOrientation);
}

void Animation::Render()
{
	if(m_bBoneMatrixReFilled)
	{
		m_bBoneMatrixReFilled = false;
	}
	if(!m_pMdl)
	{
		return;
	}
	m_pMgr->m_pd3dDevice->SetVertexShaderConstantF(16, (float*)m_pBoneMatrixRender, sizeof(ModelBoneShader)* m_iBoneCount / sizeof(float) / 4);

	m_pMdl->Render(&(GetWorldTM()), m_iCurrentSkin);
}

UINT Animation::GetBoneCount() const
{
	return(m_pMdl->GetBoneCount());
}
void Animation::GetBoneName(UINT id, char * name, int len) const
{
	m_pMdl->GetBoneName(id, name, len);
}
UINT Animation::GetBone(const char * str)
{
	return(m_pMdl->GetBoneID(str));
}

AnimStateCB Animation::SetCallback(AnimStateCB cb)
{
	AnimStateCB old = m_pfnCallBack;
	m_pfnCallBack = cb;
	return(old);
}

AnimProgressCB Animation::SetProgressCB(AnimProgressCB cb)
{
	AnimProgressCB old = m_pfnProgressCB;
	m_pfnProgressCB = cb;
	return(old);
}

const ModelFile * Animation::AddModel(const char * mdl, UINT flags, char * name)
{
	const ModelFile * pMdl = m_pMgr->LoadModel(mdl);
	if(pMdl)
	{
		AddModel(pMdl, flags, name);
	}
	return(pMdl);
}
void Animation::AddModel(const ModelFile * mdl, UINT flags, char * name)
{
	ModelPart pt;
	pt.pMdl = mdl;
	pt.uImportFlags = flags;
	pt.uFlags = 0;
	strcpy(pt.name, name);
	if(mdl)
	{
		strcpy(pt.file, mdl->m_szFileName);
	}
	else
	{
		pt.file[0] = 0;
	}
	pt.attachDesc.type = MA_SKIN;
	AddModel(&pt);
}
int Animation::AddModel(ModelPart * mp)
{
	if(!mp)
	{
		return(-1);
	}

	m_mMdls.push_back(m_aMdls.Alloc(*mp));
	return(m_mMdls.size() - 1);
}
ModelPart * Animation::GetPart(UINT idx)
{
	if(idx >= m_mMdls.size())
	{
		return(NULL);
	}
	return(m_mMdls[idx]);
}

void Animation::DelModel(ModelPart * mp)
{
	for(int i = 0, l = m_mMdls.size(); i < l; ++i)
	{
		if(m_mMdls[i] == mp)
		{
			m_mMdls.erase(i);
			m_aMdls.Delete(mp);
			break;
		}
	}
}

UINT Animation::GetPartCount()
{
	return(m_mMdls.size());
}

const ISXBound * Animation::GetBound() const
{
	return(m_pMdl->GetBound());
}

void Animation::Assembly()
{
	mem_delete(m_pMdl);
	///@TODO: clear previous data
	m_pMdl = new ModelFile("", m_pMgr);
	//AssemblyMdl(m_pMdl
	//vvBoneRelinkList[modelPartIdx][OrigBoneIdx] = newBoneIdx;
	Array<Array<int> > vvBoneRelinkList;

	//merge bones
	int sPrev = -1;
	int sCur = 0;
	Array<ModelPart*> mMdls = m_mMdls;
	while(sPrev != sCur)
	{
		sPrev = sCur;
		for(UINT i = 0; i < mMdls.size(); ++i)
		{
			ModelPart * mp = mMdls[i];
			MBERR res;
			if(!mp->pMdl)
			{
				mMdls.erase(i);
				++sCur;
				--i;
				continue;
			}
			switch(mp->attachDesc.type)
			{
			case MA_BONE: // target bone
				res = m_pMdl->AppendBones(mp->pMdl, mp->attachDesc.szBone);
				break;

			case MA_SKIN: // merge
				res = m_pMdl->AppendBones(mp->pMdl);
				break;
			}

			switch(res)
			{
			case MBE_SUCCESS:
				mMdls.erase(i);
				++sCur;
				--i;
				break;
			case MBE_NO_ROOT:
				//try again
				break;
			case MBE_BAD_HIERARCY:
				mMdls.erase(i);
				++sCur;
				--i;
				//report error
				Report(REPORT_MSG_LEVEL_WARRNING, NULL, "Skeleton hierarchy incompatible");
				break;
			}
		}
	}
	m_iBoneCount = m_pMdl->GetBoneCount();

	//build relink lists
	for(int i = 0, l = m_mMdls.size(); i < l; ++i)
	{//vvBoneRelinkList[modelPartIdx][OrigBoneIdx] = newBoneIdx;
		ModelPart * mp = m_mMdls[i];
		if(!mp->pMdl)
		{
			continue;
		}
		for(int j = 0, ll = mp->pMdl->GetBoneCount(); j < ll; ++j)
		{
			vvBoneRelinkList[i][j] = 0;
			for(int k = 0, lll = m_pMdl->GetBoneCount(); k < lll; ++k)
			{
				if(!strcmp(mp->pMdl->m_pBones[j].szName, m_pMdl->m_pBones[k].szName))
				{
					vvBoneRelinkList[i][j] = k;
					break;
				}
			}
		}
	}

	Array<Array<ModelLoDSubset> > vvLods;
	Array<char*> vszMats;
	Array<Array<int> > vvMatsRelinkList;

	Array<ModelSequence> vAnims;
	Array<ModelHitbox> vHitboxes;

	for(int i = 0, l = m_mMdls.size(); i < l; ++i)
	{
		ModelPart * mp = m_mMdls[i];
		const ModelFile * mdl = mp->pMdl;
		if(!mdl)
		{
			continue;
		}
		if((mp->uImportFlags & MI_MESH) && !(mp->uFlags & MP_HIDDEN))
		{
			for(int ii = 0, lll = mdl->m_hdr.iMaterialCount; ii < lll; ++ii)
			{
				bool found = false;
				for(int j = 0, ll = vszMats.size(); j < ll; ++j)
				{
					if(!strcmp(vszMats[j], mdl->m_iMaterials[0][ii].szName))
					{
						found = true;
						vvMatsRelinkList[i][ii] = j;
						break;
					}
				}
				if(!found)
				{
					vvMatsRelinkList[i][ii] = vszMats.size();
					vszMats.push_back(mdl->m_iMaterials[0][ii].szName);
				}
			}

			for(int ii = 0, ll = mdl->m_hdr.iLODcount; ii < ll; ++ii)
			{
				for(uint32_t j = 0; j < mdl->m_pLods[ii].iSubMeshCount; ++j)
				{
					int idx = vvMatsRelinkList[i][mdl->m_pLods[ii].pSubLODmeshes[j].iMaterialID];
					if((int)vvLods[ii].size() <= idx)
					{
						for(int u = vvLods[ii].size(), us = idx; u <= us; ++u)
						{
							memset(&vvLods[ii][u], 0, sizeof(vvLods[ii][u]));
						}
					}
					AppendMesh(&vvLods[ii][idx], &mdl->m_pLods[ii].pSubLODmeshes[j], vvBoneRelinkList[i]);
					vvLods[ii][idx].iMaterialID = idx;
				}
			}
		}

		if(mp->uImportFlags & MI_ANIMATIONS)
		{
			for(int j = 0, ll = mdl->m_hdr.iAnimationCount; j < ll; ++j)
			{
				ModelSequence tmp = mdl->m_pSequences[j];
				tmp.m_vmAnimData = new ModelBone*[tmp.iNumFrames];
				for(UINT k = 0; k < tmp.iNumFrames; k++)
				{
					tmp.m_vmAnimData[k] = new ModelBone[m_pMdl->m_hdr.iBoneCount];
					for(uint32_t o = 0; o < m_pMdl->m_hdr.iBoneCount; ++o)
					{
						tmp.m_vmAnimData[k][o] = m_pMdl->m_pBones[o].bone;
					}
					for(uint32_t o = 0; o < mdl->m_hdr.iBoneCount; ++o)
					{
						int idx = vvBoneRelinkList[i][o];
						tmp.m_vmAnimData[k][idx].orient = mdl->m_pSequences[j].m_vmAnimData[k][o].orient;
						tmp.m_vmAnimData[k][idx].position = mdl->m_pSequences[j].m_vmAnimData[k][o].position;
					}
				}
				vAnims.push_back(tmp);
			}
		}

		if(mp->uImportFlags & MI_SKINS)
		{

		}

		if(mp->uImportFlags & MI_CONTROLLERS)
		{

		}

		if(mp->uImportFlags & MI_HITBOXES)
		{
			ModelHitbox hb;
			for(int j = 0, ll = mdl->m_hdr2.iHitboxCount; j < ll; ++j)
			{
				hb = mdl->m_pHitboxes[j];
				hb.bone_id = -1;
				for(int k = 0, lll = m_pMdl->GetBoneCount(); k < lll; ++k)
				{
					if(!strcmp(hb.bone, m_pMdl->m_pBones[k].szName))
					{
						hb.bone_id = k;
						break;
					}
				}
				vHitboxes.push_back(hb);
			}
		}

		/*if(mp->file[0] != '@')
		{
			for(int j = 0; j < mdl->m_hdr2.iDepsCount; ++j)
			{
				m_mMdls.push_back(&(mdl->m_pParts[j]));
			}
		}*/
	}

	m_pMdl->m_hdr.iSkinCount = 1;
	m_pMdl->m_iMaterials = new ModelMatrial*[m_pMdl->m_hdr.iSkinCount];
	m_pMdl->m_hdr.iMaterialCount = vszMats.size();
	m_pMdl->m_iMaterials[0] = new ModelMatrial[m_pMdl->m_hdr.iMaterialCount];
	for(uint32_t i = 0; i < m_pMdl->m_hdr.iMaterialCount; ++i)
	{
		strcpy(m_pMdl->m_iMaterials[0][i].szName, vszMats[i]);
		m_pMdl->m_iMaterials[0][i].iMat = m_pMgr->GetMaterial(m_pMdl->m_iMaterials[0][i].szName);
	}

	SGCore_LoadTexLoadTextures();

	m_pMdl->m_hdr.iLODcount = vvLods.size();
	m_pMdl->m_pLods = new ModelLoD[m_pMdl->m_hdr.iLODcount];
	for(uint32_t i = 0; i < m_pMdl->m_hdr.iLODcount; i++)
	{
		m_pMdl->m_pLods[i].iSubMeshCount = vvLods[i].size();
		m_pMdl->m_pLods[i].pSubLODmeshes = new ModelLoDSubset[m_pMdl->m_pLods[i].iSubMeshCount];
		memcpy(m_pMdl->m_pLods[i].pSubLODmeshes, &vvLods[i][0], sizeof(vvLods[i][0]) * m_pMdl->m_pLods[i].iSubMeshCount);
	}

	m_pMdl->m_hdr.iAnimationCount = vAnims.size();
	m_pMdl->m_pSequences = new ModelSequence[m_pMdl->m_hdr.iAnimationCount];
	memcpy(m_pMdl->m_pSequences, &vAnims[0], sizeof(vAnims[0]) * m_pMdl->m_hdr.iAnimationCount);

	m_pMdl->m_hdr2.iHitboxCount = vHitboxes.size();
	m_pMdl->m_pHitboxes = new ModelHitbox[m_pMdl->m_hdr2.iHitboxCount];
	memcpy(m_pMdl->m_pHitboxes, &vHitboxes[0], sizeof(vHitboxes[0]) * m_pMdl->m_hdr2.iHitboxCount);


	DownloadData();
	m_pMdl->BuildMeshBuffers();
}

void Animation::AppendMesh(ModelLoDSubset * to, ModelLoDSubset * from, Array<int> & bone_relink)
{
	int iNewIdxC = to->iIndexCount + from->iIndexCount;
	int iNewVtxC = to->iVectexCount + from->iVectexCount;

	UINT * idx = new UINT[iNewIdxC];
	vertex_animated * vtx = new vertex_animated[iNewVtxC];

	memcpy(idx, to->pIndices, sizeof(UINT) * to->iIndexCount);
	memcpy(vtx, to->pVertices, sizeof(vertex_animated)* to->iVectexCount);

	memcpy(idx + to->iIndexCount, from->pIndices, sizeof(UINT)* from->iIndexCount);
	memcpy(vtx + to->iVectexCount, from->pVertices, sizeof(vertex_animated)* from->iVectexCount);

	for(uint32_t i = 0; i < from->iIndexCount; ++i)
	{
		idx[to->iIndexCount + i] += to->iVectexCount;
	}

	for(uint32_t i = 0; i < from->iVectexCount; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			vtx[to->iVectexCount + i].BoneIndices[j] = bone_relink[vtx[to->iVectexCount + i].BoneIndices[j]];
		}
	}

	mem_delete_a(to->pIndices);
	mem_delete_a(to->pVertices);
	to->pIndices = idx;
	to->pVertices = vtx;
	to->iIndexCount = iNewIdxC;
	to->iVectexCount = iNewVtxC;
}

void Animation::SwapBoneBuffs()
{
	ModelBoneShader * tmp = m_pBoneMatrixRender;
	m_pBoneMatrixRender = m_pBoneMatrix;
	m_pBoneMatrix = tmp;
}



/**
*
* AnimationManager implementation
*
*/

const ModelFile * AnimationManager::LoadModel(const char * name, bool newInst)
{
	if(!newInst && m_pModels.KeyExists(name))
	{
		return(m_pModels[name]);
	}
	else
	{
		bool build = true;
		if(name[0] == '@')
		{
			build = false;
			++name;
		}
		ModelFile * mdl = new ModelFile(name, this);
		if((mdl->m_hdr.iFlags & MODEL_FLAG_NEW_STYLE_DEPS) && mdl->m_hdr2.iDepsCount && build)
		{
			mdl->LoadParts();
			Animation tmp(this);
			tmp.AddModel(mdl);
			for(uint32_t i = 0; i < mdl->m_hdr2.iDepsCount; ++i)
			{
				tmp.AddModel(&mdl->m_pParts[i]);
			}
			tmp.Assembly();
			strcpy(tmp.m_pMdl->m_szFileName, mdl->m_szFileName);
			ModelFile * tmpmdl = mdl;
			mdl = tmp.m_pMdl;
			tmp.m_pMdl = tmpmdl;
		}
		else
		{
			mdl->BuildMeshBuffers();
		}

		m_pModels[name] = mdl;
		return(mdl);
	}
}

void AnimationManager::UnloadModel(const ModelFile * mdl)
{
	for(AssotiativeArray<String, ModelFile*>::Iterator i = m_pModels.begin(); i; i++)
	{
		if(*(i.second) == mdl)
		{
			m_pModels[*i.first] = NULL;
			delete mdl;
			break;
		}
	}
}

AnimationManager::AnimationManager(IDirect3DDevice9*dev):
m_pd3dDevice(dev),
m_iVisID(0),
m_iThreadNum(1)
{
	InitVertexDeclarations();
}

AnimationManager::~AnimationManager()
{
	for(int i = 0; i < MVT_SIZE; ++i)
	{
		mem_release(pVertexDeclaration[i]);
	}
}

void AnimationManager::InitVertexDeclarations()
{
	D3DVERTEXELEMENT9 layoutStatic[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		D3DDECL_END()
	};
	m_pd3dDevice->CreateVertexDeclaration(layoutStatic, &pVertexDeclaration[MVT_STATIC]);

	D3DVERTEXELEMENT9 layoutStaticEx[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
		{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
		D3DDECL_END()
	};
	m_pd3dDevice->CreateVertexDeclaration(layoutStaticEx, &pVertexDeclaration[MVT_STATIC_EX]);

	D3DVERTEXELEMENT9 layoutDynamic[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 32, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		D3DDECL_END()
	};
	m_pd3dDevice->CreateVertexDeclaration(layoutDynamic, &pVertexDeclaration[MVT_DYNAMIC]);

	D3DVERTEXELEMENT9 layoutDynamicEx[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
		{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
		{0, 56, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 60, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		D3DDECL_END()
	};
	m_pd3dDevice->CreateVertexDeclaration(layoutDynamicEx, &pVertexDeclaration[MVT_DYNAMIC_EX]);
}

void AnimationManager::SetVertexDeclaration(MODEL_VERTEX_TYPE nDecl)
{
	if(nDecl >= MVT_SIZE)
	{
		Report(REPORT_MSG_LEVEL_ERROR, "Unknown vertex declaration %d in AnimationManager::SetVertexDeclaration()\n", nDecl);
		return;
	}
	m_pd3dDevice->SetVertexDeclaration(pVertexDeclaration[nDecl]);
}

UINT AnimationManager::Register(Animation * pAnim)
{
	m_pAnimatedList.push_back(pAnim);
	return(m_pAnimatedList.size() - 1);
}
void AnimationManager::UnRegister(UINT id)
{
	m_pAnimatedList.erase(id);
}
void AnimationManager::Render(ID for_id)
{
#ifndef _SERVER
	Animation * pAnim;
	for(uint32_t i = 0, l = m_pAnimatedList.size(); i < l; ++i)
	{
		pAnim = m_pAnimatedList[i];
		if(pAnim->m_vIsVisibleFor[for_id])
		{
			pAnim->Render();
		}
	}
#endif
}
void AnimationManager::Update(int thread)
{
	if(thread >= m_iThreadNum)
	{
		Report(REPORT_MSG_LEVEL_WARRNING, "Requested thread %d but only %d threads allowed\n", thread, m_iThreadNum);
		return;
	}
	for(uint32_t i = thread, l = m_pAnimatedList.size(); i < l; i += m_iThreadNum)
	{
		m_pAnimatedList[i]->Advance(GetTickCount());
	}
}

void AnimationManager::Sync()
{
	for(uint32_t i = 0, l = m_pAnimatedList.size(); i < l; ++i)
	{
		m_pAnimatedList[i]->SwapBoneBuffs();
	}
}

UINT AnimationManager::GetMaterial(const char * mat)
{
	char * tmp = (char*)alloca(sizeof(char)* (strlen(mat) + 5));
	sprintf(tmp, "%s.dds", mat);
	return(SGCore_MtlLoad(tmp, MTL_TYPE_SKIN));
}

void AnimationManager::ComputeVis(const ISXFrustum * frustum, const float3 * viewpos, ID id_arr)
{
	float3 jcenter;
	float3 jradius;
	Animation * pAnim;

	SMMATRIX m;

	for(uint32_t i = 0, l = m_pAnimatedList.size(); i < l; ++i)
	{
		pAnim = m_pAnimatedList[i];
		pAnim->GetBound()->GetSphere(&jcenter, &jradius.x);

		m = pAnim->GetWorldTM();
		m._11 = SMVector3Length(float3(m._11, m._21, m._31));
		m._22 = SMVector3Length(float3(m._12, m._22, m._32));
		m._33 = SMVector3Length(float3(m._13, m._23, m._33));
		m._12 = m._13 = m._21 = m._23 = m._31 = m._32 = 0.0f;
		jcenter = SMVector3Transform(jcenter, m);
		m._41 = m._42 = m._43 = 0.0f;
		jradius = SMVector3Transform(jradius, m);
		
		pAnim->m_vIsVisibleFor[id_arr] = frustum->SphereInFrustum(&jcenter, jradius.x);
	}
}

ID AnimationManager::GetNextVisId()
{
	return(++m_iVisID);
}