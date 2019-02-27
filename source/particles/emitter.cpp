
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "Emitter.h"

void CEmitter::initNulling()
{
	m_dwOldTime = 0;
	m_dwTimeNextSpawnParticle = 0;
	m_pVertexBuff = 0;
	m_pIndexBuff = 0;
	m_pVertexBuffQuad = 0;
	m_pIndexBuffQuad = 0;
	m_isTexInit = false;
	m_idTex = -1;
	m_pArr = 0;
	m_pTransVertBuf = 0;
	m_szName[0] = 0;
	m_iCount = 0;
	m_isEnable = false;
	m_isAlife = false;
	GTransparency = 1;
	m_dwTimerDeath = 0;
	m_fSizeAdd = 0;

	m_idTexTrack = -1;
	m_vOldSize.x = 0;
	m_vOldSize.y = 0;
}

CEmitter::CEmitter()
{
	initNulling();
}

CEmitter::CEmitter(CEmitter &oPart)
{
	initNulling();
	m_idTex = oPart.m_idTex;
	m_idTexTrack = oPart.m_idTexTrack;
	m_oData = oPart.m_oData;
	setCount(oPart.m_iCount);
}

CEmitter::~CEmitter()
{
	mem_delete_a(m_pArr);
	mem_release_del(m_pTransVertBuf);
	mem_release_del(m_pVertexBuff);
	mem_release_del(m_pIndexBuff);
	mem_release_del(m_pVertexBuffQuad);
	mem_release_del(m_pIndexBuffQuad);
	mem_release_del(m_pRenderBuff);
	mem_release_del(m_pRenderBuffQuad);
}

void CEmitter::onLostDevice()
{
//	mem_release_del(m_pTransVertBuf);
}

void CEmitter::onResetDevice()
{
	//pe_data::pDXDevice->CreateVertexBuffer(
	//	m_iCount * sizeof(CommonParticleDecl2),
	//	D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
	//	0,
	//	D3DPOOL_DEFAULT,
	//	&m_pTransVertBuf,
	//	0);
}

void CEmitter::init(const CParticlesData *pData)
{
	if (pData)
		memcpy(&m_oData, pData, sizeof(CParticlesData));

	if (m_idTex >= 0)
		initAnimTexData();

	createGeomData();

	if (m_isEnable)
	{
		setEnable(false);
		setEnable(true);
	}
}

CParticlesData* CEmitter::getData()
{
	return &m_oData;
}

void CEmitter::setName(const char *szName)
{
	if (szName)
		strcpy(m_szName, szName);
}

void CEmitter::getName(char *szName)
{
	if (szName)
		strcpy(szName, m_szName);
}

void CEmitter::setTextureID(ID idTex)
{
	m_idTex = idTex;

	m_isTexInit = false;
	if (SGCore_LoadTexGetTex(m_idTex))
		m_isTexInit = true;

	initAnimTexData();
}

void CEmitter::setTexture(const char *szTex)
{
	m_idTex = SGCore_LoadTexAddName(szTex, LOAD_TEXTURE_TYPE_LOAD);
	//SGCore_LoadTexLoadTextures();
	m_isTexInit = false;
	if (SGCore_LoadTexGetTex(m_idTex))
		m_isTexInit = true;

	initAnimTexData();
}

ID CEmitter::getTextureID()
{
	return m_idTex;
}

void CEmitter::getTexture(char *szTex)
{
	if (m_idTex >= 0)
	{
		SGCore_LoadTexGetName(m_idTex, szTex);
	}
}


void CEmitter::setTextureTrackID(ID idTex)
{
	m_idTexTrack = idTex;
}

void CEmitter::setTextureTrack(const char *szTex)
{
	m_idTexTrack = SGCore_LoadTexAddName(szTex, LOAD_TEXTURE_TYPE_LOAD);
	//SGCore_LoadTexLoadTextures();
}

ID CEmitter::getTextureTrackID()
{
	return m_idTexTrack;
}

void CEmitter::getTextureTrack(char *szTex)
{
	if (m_idTexTrack >= 0)
	{
		SGCore_LoadTexGetName(m_idTexTrack, szTex);
	}
}


void CEmitter::initAnimTexData()
{
	if (m_isTexInit && m_oData.m_iAnimTexCountCadrsX != 0 && m_oData.m_iAnimTexCountCadrsY != 0)
	{
		IGXTexture2D *pTex = SGCore_LoadTexGetTex(m_idTex);

		m_vAnimTexSize.x = pTex->getWidth();
		m_vAnimTexSize.y = pTex->getHeight();

		m_vAnimSizeCadr.x = (m_vAnimTexSize.x / float(m_oData.m_iAnimTexCountCadrsX));
		m_vAnimSizeCadr.y = (m_vAnimTexSize.y / float(m_oData.m_iAnimTexCountCadrsY));
		m_vAnimSizeCadr.z = m_vAnimSizeCadr.x / m_vAnimTexSize.x;
		m_vAnimSizeCadr.w = m_vAnimSizeCadr.y / m_vAnimTexSize.y;
	}
}

void CEmitter::setAlife(bool isAlife)
{
	if (m_isAlife != isAlife)
	{
		GTransparency = 1.f;
		m_dwTimerDeath = 0;
	}

	m_isAlife = isAlife;

	if (!m_isEnable && m_isAlife)
		m_isEnable = m_isAlife;
}

bool CEmitter::getAlife()
{
	return m_isAlife;
}

//##########################################################################

void CEmitter::computeLighting()
{
	if (!m_isEnable)
		return;

	if (m_iCountLifeParticle > 0 && m_oData.m_isLighting)
	{
		float3 tmpPosition;
		float3 tmpColor;
		for (int i = 0; i < m_iCount; ++i)
		{
			if (m_pArr[i].IsAlife)
			{
				m_pArr[i].Ambient.x = m_pArr[i].Ambient.y = m_pArr[i].Ambient.z = m_pArr[i].Ambient.w = 0;

				for (int k = 0; k<SLight_GetCount(); ++k)
				{
					//если свет виден фрустуму камеры (это надо было заранее просчитать) и если свет включен
					if (SLight_GetVisibleForFrustum(k) && SLight_GetEnable(k))
					{
						SLight_GetColor(k, &tmpColor);

						float intens = 1;

						if (SLight_GetType(k) != LTYPE_LIGHT_GLOBAL)
						{
							SLight_GetPos(k, &tmpPosition, true);

							float dist = SMVector3Dot(m_pArr[i].Pos - float3(tmpPosition.x, tmpPosition.y, tmpPosition.z));
							float invdist = 1.f - (dist / (SLight_GetDist(k)));
							if (invdist > 1.f)
								invdist = 1.f;
							else if (invdist < 0.f)
								invdist = 0.f;

							intens = invdist;
						}

						m_pArr[i].LightingIntens += intens;
					}
				}
			}
		}
	}
}

void CEmitter::setCount(int iCount)
{
	m_iCount = iCount;

	if (m_iCount <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - buffer null size", GEN_MSG_LOCATION);
		return;
	}

	mem_delete_a(m_pArr);
	mem_release(m_pTransVertBuf);
	mem_release(m_pRenderBuff);
	mem_release(m_pRenderBuffQuad);

	m_pArr = new CommonParticle[m_iCount];

	m_pTransVertBuf = pe_data::pDXDevice->createVertexBuffer(m_iCount * sizeof(CommonParticleDecl2), GX_BUFFER_USAGE_STREAM | GX_BUFFER_ALLOWDISCARD);

	createGeomData();
}

int CEmitter::getCount()
{
	return m_iCount;
}

int CEmitter::getCountLife()
{
	return m_iCountLifeParticle;
}

void CEmitter::setEnable(bool isEnable)
{
	if (!m_isEnable && isEnable)
	{
		createParticles();

		for (int i = 0; i < m_iCount; ++i)
		{
			if (m_pArr[i].IsAlife)
			{
				m_vCurrMax = m_pArr[i].Pos;
				m_vCurrMin = m_pArr[i].Pos;
				break;
			}
		}
	}

	m_isAlife = isEnable;
	m_dwOldTime = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	m_isEnable = isEnable;

	if (!m_isEnable)
	{
		for (int i = 0; i < m_iCount; ++i)
		{
			m_pArr[i].IsAlife = false;
		}
	}
}

bool CEmitter::getEnable()
{
	return m_isEnable;
}

void CEmitter::modifyVertexBuff()
{
	if (!m_pVertexBuff)
		return;

	CommonParticleDecl *pVertices;
	if(m_pVertexBuff->lock((void**)&pVertices, GXBL_WRITE))
	{

		float prev_angle_x = 0;
		float prev_angle_y = 0;
		float prev_angle_z = 0;

		int countvert = 0;

		static float4x4 mat;
		mat = SMMatrixIdentity();

		float x = m_oData.m_vSize.x * 0.5f;
		float y = m_oData.m_vSize.y * 0.5f;

		for(int i = 0; i < m_oData.m_iFigureCountQuads; ++i)
		{
			float3 v0(-x, -y, 0.f);
			float3 v1(-x, y, 0.f);
			float3 v2(x, y, 0.f);
			float3 v3(x, -y, 0.f);

			if(m_oData.m_useFigureTapX)
			{
				if(m_oData.m_useFigureRotRand)
					prev_angle_x = randf(0, SM_PI);
				else
					prev_angle_x = SM_PI / float(m_oData.m_iFigureCountQuads) * i;

				mat *= SMMatrixRotationX(prev_angle_x);
			}

			if(m_oData.m_useFigureTapY)
			{
				if(m_oData.m_useFigureRotRand)
					prev_angle_y = randf(0, SM_PI);
				else
					prev_angle_y = SM_PI / float(m_oData.m_iFigureCountQuads) * i;

				mat *= SMMatrixRotationY(prev_angle_y);
			}

			if(m_oData.m_useFigureTapZ)
			{
				if(m_oData.m_useFigureRotRand)
					prev_angle_z = randf(0, SM_PI);
				else
					prev_angle_z = SM_PI / float(m_oData.m_iFigureCountQuads) * i;

				mat *= SMMatrixRotationZ(prev_angle_z);
			}

			v0 = SMVector3Transform(v0, mat);
			v1 = SMVector3Transform(v1, mat);
			v2 = SMVector3Transform(v2, mat);
			v3 = SMVector3Transform(v3, mat);

			pVertices[countvert + 0] = CommonParticleDecl(v0.x, v0.y, v0.z, 0.0f, 1.0f);
			pVertices[countvert + 1] = CommonParticleDecl(v1.x, v1.y, v1.z, 0.0f, 0.0f);
			pVertices[countvert + 2] = CommonParticleDecl(v2.x, v2.y, v2.z, 1.0f, 0.0f);
			pVertices[countvert + 3] = CommonParticleDecl(v3.x, v3.y, v3.z, 1.0f, 1.0f);

			countvert += 4;
		}

		m_pVertexBuff->unlock();
	}
}

void CEmitter::createGeomData()
{
	mem_release(m_pVertexBuff);
	mem_release(m_pIndexBuff);
	mem_release(m_pRenderBuff);
	mem_release(m_pVertexBuffQuad);
	mem_release(m_pIndexBuffQuad);
	mem_release(m_pRenderBuffQuad);

	if(!m_iCount)
	{
		return;
	}

	//@FIXME: Consider to make it static
	m_pVertexBuff = pe_data::pDXDevice->createVertexBuffer(4 * m_oData.m_iFigureCountQuads * sizeof(CommonParticleDecl), GX_BUFFER_USAGE_DYNAMIC);
	m_pIndexBuff = pe_data::pDXDevice->createIndexBuffer(6 * m_oData.m_iFigureCountQuads * sizeof(WORD), GX_BUFFER_USAGE_DYNAMIC, GXIT_USHORT);
	IGXVertexBuffer *pvb[] = {m_pVertexBuff, m_pTransVertBuf};
	m_pRenderBuff = pe_data::pDXDevice->createRenderBuffer(2, pvb, pe_data::pVertexDeclarationParticles);

	modifyVertexBuff();

	WORD* indices = 0;
	if(m_pIndexBuff->lock((void**)&indices, GXBL_WRITE))
	{

		int countind = 0;
		int countvert = 0;

		for(int i = 0; i < m_oData.m_iFigureCountQuads; ++i)
		{
			indices[countind + 0] = countvert + 0; indices[countind + 1] = countvert + 1; indices[countind + 2] = countvert + 2;
			indices[countind + 3] = countvert + 0; indices[countind + 4] = countvert + 2; indices[countind + 5] = countvert + 3;

			countind += 6;
			countvert += 4;
		}

		m_pIndexBuff->unlock();
	}

	//@FIXME: Consider to make it static
	m_pVertexBuffQuad = pe_data::pDXDevice->createVertexBuffer(4 * sizeof(CommonParticleDecl), GX_BUFFER_USAGE_DYNAMIC);
	m_pIndexBuffQuad = pe_data::pDXDevice->createIndexBuffer(6 * sizeof(WORD), GX_BUFFER_USAGE_DYNAMIC, GXIT_USHORT);
	IGXVertexBuffer *pvbq[] = {m_pVertexBuffQuad, m_pTransVertBuf};
	m_pRenderBuffQuad = pe_data::pDXDevice->createRenderBuffer(2, pvbq, pe_data::pVertexDeclarationParticles);

	CommonParticleDecl* vertices;
	if(m_pVertexBuffQuad->lock((void**)&vertices, GXBL_WRITE))
	{
		vertices[0] = CommonParticleDecl(-0.5, 0, -0.5, 0.0f, 1.0f);
		vertices[1] = CommonParticleDecl(-0.5, 0, 0.5, 0.0f, 0.0f);
		vertices[2] = CommonParticleDecl(0.5, 0, 0.5, 1.0f, 0.0f);
		vertices[3] = CommonParticleDecl(0.5, 0, -0.5, 1.0f, 1.0f);

		m_pVertexBuffQuad->unlock();
	}

	indices = 0;
	if(m_pIndexBuffQuad->lock((void**)&indices, GXBL_WRITE))
	{

		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 0; indices[4] = 2; indices[5] = 3;

		m_pIndexBuffQuad->unlock();
	}
}

void CEmitter::createParticles()
{
	m_iCountReCreate2 = 0;
	m_iCountLifeParticle = 0;
	
	for (int i = 0; i<m_iCount; i++)
	{
		if (abs(m_oData.m_iReCreateCount) > m_iCountReCreate2 || m_oData.m_iReCreateCount == 0)
		{
			reCreateParticles(i);
			m_iCountReCreate2++;
			m_iCountLifeParticle++;
		}
		else
		{
			m_iCountReCreate2 = 0;
			i = m_iCount;
			if (m_oData.m_uiSpawnNextTime)
				m_dwTimeNextSpawnParticle = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) + m_oData.m_uiSpawnNextTime;//(m_oData.SpawnNextTime + (m_oData.SpawnNextTimeDisp > 0 ? rand()%m_oData.SpawnNextTimeDisp : 0));
		}
	}

}

void CEmitter::reCreateParticles(ID id)
{
	//если разброс недопустим то спавним только в точке
	if (m_oData.m_typeSpawnPos == PARTICLESTYPE_SPAWNPOS_STRICTLY)
	{
		m_pArr[id].Pos = m_oData.m_vSpawnOrigin;
	}
	//разрешен рандомный спавн
	else if (m_oData.m_typeSpawnPos == PARTICLESTYPE_SPAWNPOS_BOUND)
	{
		if (m_oData.m_typeBound == PARTICLESTYPE_BOUND_CONE)
		{
			if (m_oData.m_shouldSpawnBoundBindCreateYNeg && m_oData.m_shouldSpawnBoundBindCreateYPos)
				m_pArr[id].Pos.y = randf(m_oData.m_vBoundVec1.y, m_oData.m_vBoundVec2.y);
			else if (m_oData.m_shouldSpawnBoundBindCreateYNeg)
				m_pArr[id].Pos.y = m_oData.m_vBoundVec1.y;
			else if (m_oData.m_shouldSpawnBoundBindCreateYPos)
				m_pArr[id].Pos.y = m_oData.m_vBoundVec2.y;
			else
				m_pArr[id].Pos.y = m_oData.m_vSpawnOrigin.y;

			float tmplerp = (m_oData.m_vBoundVec2.y - m_pArr[id].Pos.y) / (m_oData.m_vBoundVec2.y - m_oData.m_vBoundVec1.y);

			float tmpradius = vlerp(m_oData.m_vBoundVec2.w, m_oData.m_vBoundVec1.w, tmplerp);
			float3 tmpcoord = float3(m_oData.m_vBoundVec1.x, 0, m_oData.m_vBoundVec1.z);

			if (m_oData.m_shouldSpawnBoundBindCreateXNeg && m_oData.m_shouldSpawnBoundBindCreateXPos)
				m_pArr[id].Pos.x = randf(tmpcoord.x - tmpradius, tmpcoord.x + tmpradius);
			else if (m_oData.m_shouldSpawnBoundBindCreateXNeg)
				m_pArr[id].Pos.x = tmpcoord.x - tmpradius;
			else if (m_oData.m_shouldSpawnBoundBindCreateXPos)
				m_pArr[id].Pos.x = tmpcoord.x + tmpradius;
			else
				m_pArr[id].Pos.x = m_oData.m_vSpawnOrigin.x;

			if (m_oData.m_shouldSpawnBoundBindCreateZNeg && m_oData.m_shouldSpawnBoundBindCreateZPos)
				m_pArr[id].Pos.z = randf(tmpcoord.z - tmpradius, tmpcoord.z + tmpradius);
			else if (m_oData.m_shouldSpawnBoundBindCreateZNeg)
				m_pArr[id].Pos.z = tmpcoord.z - tmpradius;
			else if (m_oData.m_shouldSpawnBoundBindCreateZPos)
				m_pArr[id].Pos.z = tmpcoord.z + tmpradius;
			else
				m_pArr[id].Pos.z = m_oData.m_vSpawnOrigin.z;
		}
		else if (m_oData.m_typeBound == PARTICLESTYPE_BOUND_BOX)
		{
			if (m_oData.m_shouldSpawnBoundBindCreateXNeg && m_oData.m_shouldSpawnBoundBindCreateXPos)
				m_pArr[id].Pos.x = randf(m_oData.m_vBoundVec1.x, m_oData.m_vBoundVec2.x);
			else if (m_oData.m_shouldSpawnBoundBindCreateXNeg)
				m_pArr[id].Pos.x = m_oData.m_vBoundVec1.x;
			else if (m_oData.m_shouldSpawnBoundBindCreateXPos)
				m_pArr[id].Pos.x = m_oData.m_vBoundVec2.x;
			else
				m_pArr[id].Pos.x = m_oData.m_vSpawnOrigin.x;

			if (m_oData.m_shouldSpawnBoundBindCreateYNeg && m_oData.m_shouldSpawnBoundBindCreateYPos)
				m_pArr[id].Pos.y = randf(m_oData.m_vBoundVec1.y, m_oData.m_vBoundVec2.y);
			else if (m_oData.m_shouldSpawnBoundBindCreateYNeg)
				m_pArr[id].Pos.y = m_oData.m_vBoundVec1.y;
			else if (m_oData.m_shouldSpawnBoundBindCreateYPos)
				m_pArr[id].Pos.y = m_oData.m_vBoundVec2.y;
			else
				m_pArr[id].Pos.y = m_oData.m_vSpawnOrigin.y;

			if (m_oData.m_shouldSpawnBoundBindCreateZNeg && m_oData.m_shouldSpawnBoundBindCreateZPos)
				m_pArr[id].Pos.z = randf(m_oData.m_vBoundVec1.z, m_oData.m_vBoundVec2.z);
			else if (m_oData.m_shouldSpawnBoundBindCreateZNeg)
				m_pArr[id].Pos.z = m_oData.m_vBoundVec1.z;
			else if (m_oData.m_shouldSpawnBoundBindCreateZPos)
				m_pArr[id].Pos.z = m_oData.m_vBoundVec2.z;
			else
				m_pArr[id].Pos.z = m_oData.m_vSpawnOrigin.z;
		}
		else if (m_oData.m_typeBound == PARTICLESTYPE_BOUND_SPHERE)
		{
			if (m_oData.m_shouldSpawnBoundBindCreateXNeg && m_oData.m_shouldSpawnBoundBindCreateXPos)
				m_pArr[id].Pos.x = randf(m_oData.m_vBoundVec1.x - m_oData.m_vBoundVec1.w, m_oData.m_vBoundVec1.x + m_oData.m_vBoundVec1.w);
			else if (m_oData.m_shouldSpawnBoundBindCreateXNeg)
				m_pArr[id].Pos.x = m_oData.m_vBoundVec1.x - m_oData.m_vBoundVec1.w;
			else if (m_oData.m_shouldSpawnBoundBindCreateXPos)
				m_pArr[id].Pos.x = m_oData.m_vBoundVec1.x + m_oData.m_vBoundVec1.w;
			else
				m_pArr[id].Pos.x = m_oData.m_vSpawnOrigin.x;

			if (m_oData.m_shouldSpawnBoundBindCreateYNeg && m_oData.m_shouldSpawnBoundBindCreateYPos)
				m_pArr[id].Pos.y = randf(m_oData.m_vBoundVec1.y - m_oData.m_vBoundVec1.w, m_oData.m_vBoundVec1.y + m_oData.m_vBoundVec1.w);
			else if (m_oData.m_shouldSpawnBoundBindCreateYNeg)
				m_pArr[id].Pos.y = m_oData.m_vBoundVec1.y - m_oData.m_vBoundVec1.w;
			else if (m_oData.m_shouldSpawnBoundBindCreateYPos)
				m_pArr[id].Pos.y = m_oData.m_vBoundVec1.y + m_oData.m_vBoundVec1.w;
			else
				m_pArr[id].Pos.y = m_oData.m_vSpawnOrigin.y;

			if (m_oData.m_shouldSpawnBoundBindCreateZNeg && m_oData.m_shouldSpawnBoundBindCreateZPos)
				m_pArr[id].Pos.z = randf(m_oData.m_vBoundVec1.z - m_oData.m_vBoundVec1.w, m_oData.m_vBoundVec1.z + m_oData.m_vBoundVec1.w);
			else if (m_oData.m_shouldSpawnBoundBindCreateZNeg)
				m_pArr[id].Pos.z = m_oData.m_vBoundVec1.z - m_oData.m_vBoundVec1.w;
			else if (m_oData.m_shouldSpawnBoundBindCreateZPos)
				m_pArr[id].Pos.z = m_oData.m_vBoundVec1.z + m_oData.m_vBoundVec1.w;
			else
				m_pArr[id].Pos.z = m_oData.m_vSpawnOrigin.z;
		}
		else if (m_oData.m_typeBound == PARTICLESTYPE_BOUND_NONE)
		{
			if (m_oData.m_fSpawnOriginDisp != 0.0f)
			{
				if (m_oData.m_shouldSpawnBoundBindCreateXNeg && m_oData.m_shouldSpawnBoundBindCreateXPos)
					m_pArr[id].Pos.x = randf(m_oData.m_vSpawnOrigin.x - m_oData.m_fSpawnOriginDisp, m_oData.m_vSpawnOrigin.x + m_oData.m_fSpawnOriginDisp);
				else if (m_oData.m_shouldSpawnBoundBindCreateXNeg)
					m_pArr[id].Pos.x = randf(m_oData.m_vSpawnOrigin.x - m_oData.m_fSpawnOriginDisp, m_oData.m_vSpawnOrigin.x);
				else if (m_oData.m_shouldSpawnBoundBindCreateXPos)
					m_pArr[id].Pos.x = randf(m_oData.m_vSpawnOrigin.x, m_oData.m_vSpawnOrigin.x + m_oData.m_fSpawnOriginDisp);
				else
					m_pArr[id].Pos.x = m_oData.m_vSpawnOrigin.x;

				if (m_oData.m_shouldSpawnBoundBindCreateYNeg && m_oData.m_shouldSpawnBoundBindCreateYPos)
					m_pArr[id].Pos.y = randf(m_oData.m_vSpawnOrigin.y - m_oData.m_fSpawnOriginDisp, m_oData.m_vSpawnOrigin.y + m_oData.m_fSpawnOriginDisp);
				else if (m_oData.m_shouldSpawnBoundBindCreateYNeg)
					m_pArr[id].Pos.y = randf(m_oData.m_vSpawnOrigin.y - m_oData.m_fSpawnOriginDisp, m_oData.m_vSpawnOrigin.y);
				else if (m_oData.m_shouldSpawnBoundBindCreateYPos)
					m_pArr[id].Pos.y = randf(m_oData.m_vSpawnOrigin.y, m_oData.m_vSpawnOrigin.y + m_oData.m_fSpawnOriginDisp);
				else
					m_pArr[id].Pos.y = m_oData.m_vSpawnOrigin.y;

				if (m_oData.m_shouldSpawnBoundBindCreateZNeg && m_oData.m_shouldSpawnBoundBindCreateZPos)
					m_pArr[id].Pos.z = randf(m_oData.m_vSpawnOrigin.z - m_oData.m_fSpawnOriginDisp, m_oData.m_vSpawnOrigin.z + m_oData.m_fSpawnOriginDisp);
				else if (m_oData.m_shouldSpawnBoundBindCreateZNeg)
					m_pArr[id].Pos.z = randf(m_oData.m_vSpawnOrigin.z - m_oData.m_fSpawnOriginDisp, m_oData.m_vSpawnOrigin.z);
				else if (m_oData.m_shouldSpawnBoundBindCreateZPos)
					m_pArr[id].Pos.z = randf(m_oData.m_vSpawnOrigin.z, m_oData.m_vSpawnOrigin.z + m_oData.m_fSpawnOriginDisp);
				else
					m_pArr[id].Pos.z = m_oData.m_vSpawnOrigin.z;
			}
		}
	}

	m_pArr[id].PosCreate = m_pArr[id].Pos;
	m_pArr[id].IsAlife = true;
	m_pArr[id].AlphaAgeDependCoef = 1.f;
	m_pArr[id].AlphaDeath = 1.f;

	m_pArr[id].Age = 0;
	m_pArr[id].TimeLife = m_oData.m_uiTimeLife + (m_oData.m_uiTimeLifeDisp != 0 ? ((rand() % (m_oData.m_uiTimeLifeDisp / 2)) - (m_oData.m_uiTimeLifeDisp / 2)) : 0);//GetRandomFloat(5000,30000);

	if (m_oData.m_iAnimTexCountCadrsX == 0 || m_oData.m_iAnimTexCountCadrsY == 0)
	{
		m_pArr[id].AnimTexSizeCadrAndBias.x = m_pArr[id].AnimTexSizeCadrAndBias.y = 1.f;
		m_pArr[id].AnimTexSizeCadrAndBias.z = m_pArr[id].AnimTexSizeCadrAndBias.w = 0.f;
		m_pArr[id].AnimTexRateMls = 0;
	}
	else
	{
		m_pArr[id].AnimTexCurrentCadr = m_oData.m_iAnimTexStartCadr + (m_oData.m_iAnimTexStartCadrDisp>0 ? ((rand() % (m_oData.m_iAnimTexStartCadrDisp / 2)) - (m_oData.m_iAnimTexStartCadrDisp / 2)) : 0);
		m_pArr[id].AnimTexCurrentMls = 0;
		m_pArr[id].AnimTexRateMls = m_oData.m_iAnimTexRate + (m_oData.m_iAnimTexRateDisp>0 ? ((rand() % (m_oData.m_iAnimTexRateDisp / 2)) - (m_oData.m_iAnimTexRateDisp / 2)) : 0);

		if (m_pArr[id].AnimTexCurrentCadr < m_oData.m_iAnimTexCountCadrsX*m_oData.m_iAnimTexCountCadrsY)
		{
			m_pArr[id].AnimTexCurrentCadr++;

			WORD tmpy = m_pArr[id].AnimTexCurrentCadr / m_oData.m_iAnimTexCountCadrsX;
			WORD tmpx = m_pArr[id].AnimTexCurrentCadr - (m_oData.m_iAnimTexCountCadrsX * tmpy);

			if ((int(m_pArr[id].AnimTexCurrentCadr) - int(m_oData.m_iAnimTexCountCadrsX * tmpy)) == 0)
				tmpy -= 1;

			if (tmpx == 0)
				tmpx = m_oData.m_iAnimTexCountCadrsX;

			tmpx -= 1;

			m_pArr[id].AnimTexSizeCadrAndBias.x = m_vAnimSizeCadr.z;
			m_pArr[id].AnimTexSizeCadrAndBias.y = m_vAnimSizeCadr.w;
			m_pArr[id].AnimTexSizeCadrAndBias.z = (float(tmpx) * m_vAnimSizeCadr.x) / m_vAnimTexSize.x;
			m_pArr[id].AnimTexSizeCadrAndBias.w = (float(tmpy) * m_vAnimSizeCadr.y) / m_vAnimTexSize.y;
		}
	}

	m_pArr[id].CharacterRotateAngleZ = 0;

	if (m_oData.m_fSizeDisp > 0.f)
	{
		float tmprand = randf(0.0, m_oData.m_fSizeDisp);
		m_pArr[id].Size.x = m_oData.m_vSize.x + tmprand;
		m_pArr[id].Size.y = m_oData.m_vSize.y + tmprand;

		if (m_oData.m_typeSizeDependAge == PARTICLESTYPE_DEPEND_DIRECT)
		{
			m_pArr[id].Size.y = m_pArr[id].Size.x;
			m_pArr[id].Size.x = m_oData.m_fSizeDisp;
		}
	}
	else
	{
		m_pArr[id].Size.y = m_oData.m_vSize.x;
		m_pArr[id].Size.x = (m_oData.m_typeSizeDependAge == PARTICLESTYPE_DEPEND_DIRECT ? 0 : m_oData.m_vSize.x);
	}


	m_pArr[id].Velocity.x = m_oData.m_vVelocity.x;
	m_pArr[id].Velocity.y = m_oData.m_vVelocity.y;
	m_pArr[id].Velocity.z = m_oData.m_vVelocity.z;

	if (m_oData.m_fVelocityDisp != 0.0f)
	{
		if (m_pArr[id].Velocity.x != 0.f)
		{
			if (m_oData.m_shouldVelocityDispXNeg)
				m_pArr[id].Velocity.x = m_oData.m_vVelocity.x + (randf(-m_oData.m_fVelocityDisp*0.5, m_oData.m_fVelocityDisp*0.5));
			else
				m_pArr[id].Velocity.x = m_oData.m_vVelocity.x + (randf(0, m_oData.m_fVelocityDisp));
		}

		if (m_pArr[id].Velocity.y != 0.f)
		{
			if (m_oData.m_shouldVelocityDispYNeg)
				m_pArr[id].Velocity.y = m_oData.m_vVelocity.y + (randf(-m_oData.m_fVelocityDisp*0.5, m_oData.m_fVelocityDisp*0.5));
			else
				m_pArr[id].Velocity.y = m_oData.m_vVelocity.y + (randf(0, m_oData.m_fVelocityDisp));
		}

		if (m_pArr[id].Velocity.z != 0.f)
		{
			if (m_oData.m_shouldVelocityDispZNeg)
				m_pArr[id].Velocity.z = m_oData.m_vVelocity.z + (randf(-m_oData.m_fVelocityDisp*0.5, m_oData.m_fVelocityDisp*0.5));
			else
				m_pArr[id].Velocity.z = m_oData.m_vVelocity.z + (randf(0, m_oData.m_fVelocityDisp));
		}
	}


	if (m_oData.m_fAccelerationDisp == 0.0f)
	{
		m_pArr[id].Acceleration.x = m_oData.m_vAcceleration.x;
		m_pArr[id].Acceleration.y = m_oData.m_vAcceleration.y;
		m_pArr[id].Acceleration.z = m_oData.m_vAcceleration.z;
	}
	else if (m_oData.m_fAccelerationDisp != 0.0f)
	{
		if (m_pArr[id].Acceleration.x != 0.f)
		{
			if (m_oData.m_shouldAccelerationDispXNeg)
				m_pArr[id].Acceleration.x = m_oData.m_vAcceleration.x + (randf(-m_oData.m_fAccelerationDisp*0.5, m_oData.m_fAccelerationDisp*0.5));
			else
				m_pArr[id].Acceleration.x = m_oData.m_vAcceleration.x + (randf(0, m_oData.m_fAccelerationDisp));
		}

		if (m_pArr[id].Acceleration.y != 0.f)
		{
			if (m_oData.m_shouldAccelerationDispYNeg)
				m_pArr[id].Acceleration.y = m_oData.m_vAcceleration.x + (randf(-m_oData.m_fAccelerationDisp*0.5, m_oData.m_fAccelerationDisp*0.5));
			else
				m_pArr[id].Acceleration.y = m_oData.m_vAcceleration.x + (randf(0, m_oData.m_fAccelerationDisp));
		}

		if (m_pArr[id].Acceleration.z != 0.f)
		{
			if (m_oData.m_shouldAccelerationDispZNeg)
				m_pArr[id].Acceleration.z = m_oData.m_vAcceleration.z + (randf(-m_oData.m_fAccelerationDisp*0.5, m_oData.m_fAccelerationDisp*0.5));
			else
				m_pArr[id].Acceleration.z = m_oData.m_vAcceleration.z + (randf(0, m_oData.m_fAccelerationDisp));
		}
	}
}

void CEmitter::updateAnimTex(ID idParticle, DWORD dwTime)
{
	if (!m_isTexInit)
		return;

	//если подошло время обновления анимации текстуры
	if (m_pArr[idParticle].AnimTexRateMls <= m_pArr[idParticle].AnimTexCurrentMls)
	{
		m_pArr[idParticle].AnimTexCurrentMls = 0;

		//если номер текущего кадра меньше чем общее количество кадров, то обрабатываем
		if (m_pArr[idParticle].AnimTexCurrentCadr < m_oData.m_iAnimTexCountCadrsX*m_oData.m_iAnimTexCountCadrsY)
		{
			m_pArr[idParticle].AnimTexCurrentCadr++;

			WORD tmpy = m_pArr[idParticle].AnimTexCurrentCadr / m_oData.m_iAnimTexCountCadrsX;
			WORD tmpx = m_pArr[idParticle].AnimTexCurrentCadr - (m_oData.m_iAnimTexCountCadrsX * tmpy);

			if ((int(m_pArr[idParticle].AnimTexCurrentCadr) - int(m_oData.m_iAnimTexCountCadrsX * tmpy)) == 0)
				tmpy -= 1;

			if (tmpx == 0)
				tmpx = m_oData.m_iAnimTexCountCadrsX;

			tmpx -= 1;

			m_pArr[idParticle].AnimTexSizeCadrAndBias.x = m_vAnimSizeCadr.z;
			m_pArr[idParticle].AnimTexSizeCadrAndBias.y = m_vAnimSizeCadr.w;
			m_pArr[idParticle].AnimTexSizeCadrAndBias.z = (float(tmpx) * m_vAnimSizeCadr.x) / m_vAnimTexSize.x;
			m_pArr[idParticle].AnimTexSizeCadrAndBias.w = (float(tmpy) * m_vAnimSizeCadr.y) / m_vAnimTexSize.y;
		}
		else
			m_pArr[idParticle].AnimTexCurrentCadr = 0;
	}
	else
	{
		m_pArr[idParticle].AnimTexCurrentMls += dwTime - m_dwOldTime;
	}
}

bool CEmitter::isPointInCone(const float3 *pPoint)
{
	if (pPoint->y >= m_oData.m_vBoundVec1.y && pPoint->y <= m_oData.m_vBoundVec2.y)
	{
		float fLerp = (m_oData.m_vBoundVec2.y - pPoint->y) / (m_oData.m_vBoundVec2.y - m_oData.m_vBoundVec1.y);
		float fRadius = vlerp(m_oData.m_vBoundVec2.w, m_oData.m_vBoundVec1.w, fLerp);
		float3 vCoord = float3(m_oData.m_vBoundVec1.x, 0, m_oData.m_vBoundVec1.z);

		if (
			vCoord.x + fRadius >= pPoint->x && vCoord.x - fRadius <= pPoint->x &&
			vCoord.z + fRadius >= pPoint->z && vCoord.z - fRadius <= pPoint->z
			)
			return true;
	}

	return false;
}

bool CEmitter::isPointInSphere(const float3 *pPoint)
{
	float fDistSqr = SMVector3Dot(m_oData.m_vBoundVec1 - *pPoint);
	if (fDistSqr <= m_oData.m_vBoundVec1.w*m_oData.m_vBoundVec1.w)
		return true;
	else
		return false;
}

bool CEmitter::isPointInBox(const float3 *pPoint)
{
	return (pPoint->x >= m_oData.m_vBoundVec1.x && pPoint->y >= m_oData.m_vBoundVec1.y && pPoint->z >= m_oData.m_vBoundVec1.z && pPoint->x <= m_oData.m_vBoundVec2.x && pPoint->y <= m_oData.m_vBoundVec2.y && pPoint->z <= m_oData.m_vBoundVec2.z);
}

void CEmitter::compute(const float4x4 *pMat)
{
	if (!m_isEnable)
		return;

	m_fSizeAdd = (m_oData.m_vSize.x > m_oData.m_vSize.y ? m_oData.m_vSize.x : m_oData.m_vSize.y) + m_oData.m_fSizeDisp;

	m_iCountReCreate2 = 0;
	if (m_isAlife && m_dwOldTime > 0 && m_oData.m_iReCreateCount > 0 && m_oData.m_iReCreateCount <= m_iCount - m_iCountLifeParticle)
	{
		if (TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) > m_dwTimeNextSpawnParticle)
		{
			for (int i = 0; i<m_iCount; i++)
			{
				if (!(m_pArr[i].IsAlife) && m_oData.m_iReCreateCount > m_iCountReCreate2)
				{
					reCreateParticles(i);
					m_iCountReCreate2++;
				}
				else if (m_oData.m_iReCreateCount <= m_iCountReCreate2)
				{
					m_iCountReCreate2 = 0;
					i = m_iCount;
					if (m_oData.m_uiSpawnNextTime)
						m_dwTimeNextSpawnParticle = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) + (m_oData.m_uiSpawnNextTime + (m_oData.m_uiSpawnNextTimeDisp > 0 ? rand() % m_oData.m_uiSpawnNextTimeDisp : 0));
				}
			}
		}
	}

	if (!m_isAlife)
	{
		m_dwTimeNextSpawnParticle = 0;
		if (m_dwTimerDeath == 0)
			m_dwTimerDeath = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
		else
		{
			GTransparency = 1.f - float(TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - m_dwTimerDeath) / float(SXPARTICLES_DEADTH_TIME);
			if (TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - m_dwTimerDeath > SXPARTICLES_DEADTH_TIME)
			{
				m_isEnable = false;
				GTransparency = 1.f;
				m_dwTimerDeath = 0;
				for (int i = 0; i < m_iCount; ++i)
				{
					m_pArr[i].IsAlife = false;
				}
				return;
			}
		}
	}


	DWORD tmptime = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	m_iCountLifeParticle = 0;

	float3 tmpoldpos,tmpnewpos;

	for (int i = 0; i<m_iCount && m_dwOldTime != 0; i++)
	{
		//если время жизни частицы больше либо равно назначенному то значит она уже умерла
		if (m_pArr[i].IsAlife && m_pArr[i].TimeLife > 0 && m_pArr[i].Age >= m_pArr[i].TimeLife)
			m_pArr[i].IsAlife = false;

		//иначе инкремент общего количества живых частиц
		else if (m_pArr[i].IsAlife)
		{
			//если установлен ограничивающий объем то проверяем не выходит ли частица за его пределы, если выходит то значит она умерла

			if (m_oData.m_typeBound == PARTICLESTYPE_BOUND_SPHERE && isPointInSphere(&m_pArr[i].Pos))
				m_iCountLifeParticle++;
			else if (m_oData.m_typeBound == PARTICLESTYPE_BOUND_BOX && isPointInBox(&m_pArr[i].Pos))
				m_iCountLifeParticle++;
			else if (m_oData.m_typeBound == PARTICLESTYPE_BOUND_CONE && isPointInCone(&(m_pArr[i].Pos)))
				m_iCountLifeParticle++;
			else if (m_oData.m_typeBound != PARTICLESTYPE_BOUND_NONE)
				m_pArr[i].IsAlife = false;
			else if (m_oData.m_typeBound == PARTICLESTYPE_BOUND_NONE)
				m_iCountLifeParticle++;
		}

		
		//если частица жива то обрабатываем поведение
		if (m_pArr[i].IsAlife)
		{
			tmpoldpos = m_pArr[i].Pos;
			//обрабокта возраста
			m_pArr[i].Age += tmptime - m_dwOldTime;

			//обработка зависимости прозрачности от возраста
			if (m_oData.m_typeAlphaDependAge == PARTICLESTYPE_DEPEND_DIRECT)
				m_pArr[i].AlphaAgeDependCoef = 1.f - (float(m_pArr[i].Age) / float(m_pArr[i].TimeLife));
			else if (m_oData.m_typeAlphaDependAge == PARTICLESTYPE_DEPEND_INVERSE)
				m_pArr[i].AlphaAgeDependCoef = (float(m_pArr[i].Age) / float(m_pArr[i].TimeLife));
			else
				m_pArr[i].AlphaAgeDependCoef = 1;

			if (m_pArr[i].AlphaAgeDependCoef > 1.f)
				m_pArr[i].AlphaAgeDependCoef = 1.f;
			else if (m_pArr[i].AlphaAgeDependCoef < 0.f)
				m_pArr[i].AlphaAgeDependCoef = 0.f;

			if (!m_isAlife)
				m_pArr[i].AlphaDeath = GTransparency;

			//обработка зависимости размера от возвраста
			if (m_oData.m_typeSizeDependAge == PARTICLESTYPE_DEPEND_DIRECT)
			{
				m_pArr[i].Size.x = m_pArr[i].Size.y * (float(m_pArr[i].Age) / float(m_pArr[i].TimeLife));
			}
			else if (m_oData.m_typeSizeDependAge == PARTICLESTYPE_DEPEND_INVERSE)
			{
				m_pArr[i].Size.x = m_pArr[i].Size.y * (1.f - (float(m_pArr[i].Age) / float(m_pArr[i].TimeLife)));
			}

			float tmpcountsec = float(tmptime - m_dwOldTime) * 0.001f;
			//обработка скорости и ускорения
			m_pArr[i].Pos += m_pArr[i].Velocity * tmpcountsec + m_pArr[i].Acceleration*(float(m_pArr[i].Age) * tmpcountsec);

			//если назначена анимация текстуры то обрабатываем
			if (m_pArr[i].AnimTexRateMls > 0)
				updateAnimTex(i, tmptime);
			/*else if (m_oData.AnimTexRate > 0 && m_oData.AnimTexCountCadrsX > 0 && m_oData.AnimTexCountCadrsY > 0)
			{
				m_pArr[i].AnimTexRateMls = m_oData.AnimTexRate + (m_oData.AnimTexRateDisp>0 ? ((rand() % (m_oData.AnimTexRateDisp / 2)) - (m_oData.AnimTexRateDisp / 2)) : 0);
			}*/

			//CHARACTER

			//движение по кругу
			if (m_oData.m_useCharacterCircle)
			{
				float tmpangle = 0;

				//если возможна дисперсия
				if (m_oData.m_fCharacterCircleAngleDisp != 0.0f)
				{
					//если возможно брать отрицательные значения
					if (m_oData.m_useCharacterCircleAngleDispNeg)
						tmpangle = randf(-m_oData.m_fCharacterCircleAngleDisp*0.5, m_oData.m_fCharacterCircleAngleDisp*0.5);
					else
						tmpangle = randf(0.f, m_oData.m_fCharacterCircleAngleDisp);
				}

				tmpangle += m_oData.m_fCharacterCircleAngle;

				static float4x4 matrot;

				if (m_oData.m_typeCharacterCircleAxis == PARTICLES_AXIS_X)
					matrot = SMMatrixRotationX(tmpangle);
				else if (m_oData.m_typeCharacterCircleAxis == PARTICLES_AXIS_Y)
					matrot = SMMatrixRotationY(tmpangle);
				else if (m_oData.m_typeCharacterCircleAxis == PARTICLES_AXIS_Z)
					matrot = SMMatrixRotationZ(tmpangle);

				m_pArr[i].Pos = SMVector3Transform(m_pArr[i].Pos, matrot);
			}

			//если есть хараткер поворота
			if (m_oData.m_useCharacterRotate)
			{
				float tmpangle = 0;

				//если возможна дисперсия
				if (m_oData.m_fCharacterRotateAngleDisp != 0.0f)
				{
					//если возможно брать отрицательные значения
					if (m_oData.m_useCharacterRotateAngleDispNeg)
						tmpangle = randf(-m_oData.m_fCharacterRotateAngleDisp*0.5, m_oData.m_fCharacterRotateAngleDisp*0.5);
					else
						tmpangle = randf(0.f, m_oData.m_fCharacterRotateAngleDisp);
				}

				m_pArr[i].CharacterRotateAngleZ += m_oData.m_fCharacterRotateAngle + tmpangle;

				if (m_pArr[i].CharacterRotateAngleZ >= SM_2PI)
					m_pArr[i].CharacterRotateAngleZ = m_pArr[i].CharacterRotateAngleZ - SM_2PI;

				m_pArr[i].rot_1.x = sinf(m_pArr[i].CharacterRotateAngleZ);
				m_pArr[i].rot_1.y = cosf(m_pArr[i].CharacterRotateAngleZ);
			}

			//если определен характер движения
			if (m_oData.m_useCharacterDeviation)
			{
				//если определен волновой тип движения
				if (m_oData.m_typeCharacterDeviation == PARTICLESTYPE_DEVIATION_WAVE)
				{
					float tmpdist = 0;

					//определяем ось для волны
					if (m_oData.m_typeCharacterDeviationAxis == PARTICLES_AXIS_X)
						tmpdist = m_pArr[i].Pos.x;
					else if (m_oData.m_typeCharacterDeviationAxis == PARTICLES_AXIS_Y)
						tmpdist = m_pArr[i].Pos.y;
					else if (m_oData.m_typeCharacterDeviationAxis == PARTICLES_AXIS_Z)
						tmpdist = m_pArr[i].Pos.z;

					//если разрешена дисперсия
					if (m_oData.m_fCharacterDeviationCoefAngleDisp != 0.0f)
					{
						if (m_oData.m_useCharacterDeviationCoefAngleDispNeg)
						{
							tmpdist += randf(-m_oData.m_fCharacterDeviationCoefAngleDisp*0.5, m_oData.m_fCharacterDeviationCoefAngleDisp*0.5);
						}
						else
						{
							//если амплитуда отрицательная, значит и рандомное значение тоже
							if (m_oData.m_fCharacterDeviationAmplitude < 0)
								tmpdist += randf(m_oData.m_fCharacterDeviationCoefAngleDisp, 0);
							else
								tmpdist += randf(0, m_oData.m_fCharacterDeviationCoefAngleDisp);
						}
						
					}

					//домножаем на коэфициент
					tmpdist *= m_oData.m_fCharacterDeviationCoefAngle;

					if (m_oData.m_useCharacterDeviationTapX)
						m_pArr[i].DeviationVector.x = m_oData.m_fCharacterDeviationAmplitude * sinf(tmpdist);

					if (m_oData.m_useCharacterDeviationTapY)
						m_pArr[i].DeviationVector.y = m_oData.m_fCharacterDeviationAmplitude * sinf(tmpdist);

					if (m_oData.m_useCharacterDeviationTapZ)
						m_pArr[i].DeviationVector.z = m_oData.m_fCharacterDeviationAmplitude * sinf(tmpdist);

					m_pArr[i].Pos += m_pArr[i].DeviationVector;
				}


				//если пришло время обновления
				else if (m_pArr[i].CharacterDeviationCountDelayMls2 >= m_oData.m_uiCharacterDeviationCountDelayMls)
				{
					//обнуляем вектор отклонения
					m_pArr[i].DeviationVector.x = m_pArr[i].DeviationVector.y = m_pArr[i].DeviationVector.z = 0;

					//если простое смещение
					if (m_oData.m_typeCharacterDeviation == PARTICLESTYPE_DEVIATION_RAND)
					{
						//если разрешена дисперсия то генерируем
						if (m_oData.m_fCharacterDeviationCoefAngleDisp != 0.0f)
						{
							if (m_oData.m_useCharacterDeviationTapX)
								m_pArr[i].DeviationVector.x = randf(0, m_oData.m_fCharacterDeviationAmplitude) - (m_oData.m_useCharacterDeviationCoefAngleDispNeg ? m_oData.m_fCharacterDeviationAmplitude*0.5f : 0);

							if (m_oData.m_useCharacterDeviationTapY)
								m_pArr[i].DeviationVector.y = randf(0, m_oData.m_fCharacterDeviationAmplitude) - (m_oData.m_useCharacterDeviationCoefAngleDispNeg ? m_oData.m_fCharacterDeviationAmplitude*0.5f : 0);

							if (m_oData.m_useCharacterDeviationTapZ)
								m_pArr[i].DeviationVector.z = randf(0, m_oData.m_fCharacterDeviationAmplitude) - (m_oData.m_useCharacterDeviationCoefAngleDispNeg ? m_oData.m_fCharacterDeviationAmplitude*0.5f : 0);
						}
						else
						{
							if (m_oData.m_useCharacterDeviationTapX)
								m_pArr[i].DeviationVector.x = m_oData.m_fCharacterDeviationAmplitude;

							if (m_oData.m_useCharacterDeviationTapY)
								m_pArr[i].DeviationVector.y = m_oData.m_fCharacterDeviationAmplitude;

							if (m_oData.m_useCharacterDeviationTapZ)
								m_pArr[i].DeviationVector.z = m_oData.m_fCharacterDeviationAmplitude;
						}
					}
					//иначе если равномерное смещение вдоль
					else if (m_oData.m_typeCharacterDeviation == PARTICLESTYPE_DEVIATION_ALONG)
					{
						m_oData.m_fCharacterDeviationAmplitude = -m_oData.m_fCharacterDeviationAmplitude;

						if (m_oData.m_useCharacterDeviationTapX)
							m_pArr[i].DeviationVector.x = m_oData.m_fCharacterDeviationAmplitude;

						if (m_oData.m_useCharacterDeviationTapY)
							m_pArr[i].DeviationVector.y = m_oData.m_fCharacterDeviationAmplitude;

						if (m_oData.m_useCharacterDeviationTapZ)
							m_pArr[i].DeviationVector.z = m_oData.m_fCharacterDeviationAmplitude;
					}

					m_pArr[i].CharacterDeviationCountDelayMls2 = 0;
				}
				else
				{
					m_pArr[i].Pos += m_pArr[i].DeviationVector * (float(m_pArr[i].CharacterDeviationCountDelayMls2) / float(m_oData.m_uiCharacterDeviationCountDelayMls));
				}
				m_pArr[i].CharacterDeviationCountDelayMls2 += tmptime - m_dwOldTime;
			}

			if (m_oData.m_useCollisionDelete)
			{
				tmpoldpos = SMVector3Transform(tmpoldpos, *pMat);
				tmpnewpos = SMVector3Transform(m_pArr[i].Pos, *pMat);

				if (g_fnParticlesPhyCollision(&tmpoldpos, &tmpnewpos, &m_pArr[i].TrackPos, &m_pArr[i].TrackNormal /*(!m_pArr[i].Track ? &m_pArr[i].TrackPos : 0), (!m_pArr[i].Track ? &m_pArr[i].TrackNormal : 0)*/))
				{
					m_pArr[i].IsAlife = false;

					if (m_oData.m_useTrack)
					{
						m_pArr[i].TrackPos += m_pArr[i].TrackNormal * SXPARTICLES_TRACK_ADD;
						float3 f = SXPARTICLES_TRACK_BASE_DIR;
						float3 a = SMVector3Cross(f, m_pArr[i].TrackNormal);
						float ang = acosf(SMVector3Dot(f, m_pArr[i].TrackNormal));

						SMQuaternion q(a, ang);
						m_pArr[i].TrackNormal.x = q.x;
						m_pArr[i].TrackNormal.y = q.y;
						m_pArr[i].TrackNormal.z = q.z;
						m_pArr[i].TrackNormal.w = q.w;
						m_pArr[i].Track = true;
						m_pArr[i].TrackTime = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
					}
				}
			}

			if (!(m_pArr[i].IsAlife))
				continue;

			//считаем ограничивающий объем
			float tmpmaxedge = m_pArr[i].Size.x;
			if (m_pArr[i].Size.x < m_pArr[i].Size.y)
				tmpmaxedge = m_pArr[i].Size.y;

			if (m_iCountLifeParticle == 1)
			{
				m_vCurrMax.x = m_pArr[i].Pos.x + tmpmaxedge;
				m_vCurrMax.y = m_pArr[i].Pos.y + tmpmaxedge;
				m_vCurrMax.z = m_pArr[i].Pos.z + tmpmaxedge;

				m_vCurrMin.x = m_pArr[i].Pos.x - tmpmaxedge;
				m_vCurrMin.y = m_pArr[i].Pos.y - tmpmaxedge;
				m_vCurrMin.z = m_pArr[i].Pos.z - tmpmaxedge;
			}
			else
			{
			if (m_pArr[i].Pos.x + tmpmaxedge > m_vCurrMax.x)
					m_vCurrMax.x = m_pArr[i].Pos.x + tmpmaxedge;

				if (m_pArr[i].Pos.x - tmpmaxedge < m_vCurrMin.x)
					m_vCurrMin.x = m_pArr[i].Pos.x - tmpmaxedge;

			if (m_pArr[i].Pos.y + tmpmaxedge > m_vCurrMax.y)
					m_vCurrMax.y = m_pArr[i].Pos.y + tmpmaxedge;

				if (m_pArr[i].Pos.y - tmpmaxedge < m_vCurrMin.y)
					m_vCurrMin.y = m_pArr[i].Pos.y - tmpmaxedge;

			if (m_pArr[i].Pos.z + tmpmaxedge > m_vCurrMax.z)
					m_vCurrMax.z = m_pArr[i].Pos.z + tmpmaxedge;

				if (m_pArr[i].Pos.z - tmpmaxedge < m_vCurrMin.z)
					m_vCurrMin.z = m_pArr[i].Pos.z - tmpmaxedge;
			}
		}
		else
		{

		}
	}

	//все частицы отыграли свое и будущего спавна нет
	if (m_dwOldTime != 0 && m_iCountLifeParticle == 0 && m_dwTimeNextSpawnParticle == 0)
		m_isEnable = false;

	if (!m_isEnable)
		m_isAlife = m_isEnable;

	m_dwOldTime = tmptime;

	/*m_vCurrMin.x -= m_fSizeAdd;
	m_vCurrMin.y -= m_fSizeAdd;
	m_vCurrMin.z -= m_fSizeAdd;

	m_vCurrMax.x += m_fSizeAdd;
	m_vCurrMax.y += m_fSizeAdd;
	m_vCurrMax.z += m_fSizeAdd;*/
}



void CEmitter::render(DWORD timeDelta, const float4x4 *mRot, const float4x4 *mPos)
{
	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	if (!m_isEnable)
		return;

	if (!m_isTexInit)
	{
		if (SGCore_LoadTexGetTex(m_idTex))
		{
			m_isTexInit = true;
			initAnimTexData();
		}
	}

	if (m_iCountLifeParticle > 0)
	{
		//если размер частиц изменился то меняем данные в вершинном буфере на новые
		if (m_oData.m_vSize.x != m_vOldSize.x || m_oData.m_vSize.y != m_vOldSize.y)
		{
			m_vOldSize = m_oData.m_vSize;
			modifyVertexBuff();
		}

		CommonParticleDecl2* RTGPUArrVerteces;
		DWORD tmpcount = 0;
		if(m_pTransVertBuf->lock((void**)&RTGPUArrVerteces, GXBL_WRITE))
		{
			for(int i = 0; i < m_iCount; ++i)
			{
				if(m_pArr[i].IsAlife)
				{
					RTGPUArrVerteces[tmpcount].pos = m_pArr[i].Pos;
					RTGPUArrVerteces[tmpcount].tex = m_pArr[i].AnimTexSizeCadrAndBias;
					RTGPUArrVerteces[tmpcount].alpha = m_pArr[i].AlphaAgeDependCoef * m_pArr[i].AlphaDeath * m_oData.m_fTransparencyCoef;
					RTGPUArrVerteces[tmpcount].size = m_pArr[i].Size.x;
					RTGPUArrVerteces[tmpcount].lighting = m_pArr[i].LightingIntens;

					RTGPUArrVerteces[tmpcount].rot_1 = m_pArr[i].rot_1;
					tmpcount++;
				}
			}

			m_pTransVertBuf->unlock();
		}

		pe_data::pDXDevice->setRenderBuffer(m_pRenderBuff);
		pe_data::pDXDevice->setIndexBuffer(m_pIndexBuff);

		static float4x4 MCamView;
		static float4x4 MCamProj;
		static float2_t NearFar;
		static float3 ConstCamPos;
		Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &ConstCamPos);
		NearFar.x = *r_near;
		NearFar.y = *r_far;

		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &MCamView);
		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &MCamProj);

		float4x4 cammat = SMMatrixIdentity();

		static float4x4 tmpmatrot = SMMatrixIdentity();
		tmpmatrot = (mRot ? (*mRot) : SMMatrixIdentity());

		if (m_oData.m_typeFigure == PARTICLESTYPE_FIGURE_BILLBOARD)
		{
			float determ = 0;
			cammat = SMMatrixInverse(&determ, tmpmatrot * MCamView);
			cammat._41 = cammat._42 = cammat._43 = 0;
		}

		/*static float tmpangle = 0.0;
		tmpangle += 0.01;
		tmpmatrot = SMMatrixRotationY(tmpangle);*/

		static float4x4 tmpmatpos = SMMatrixIdentity();
		tmpmatpos = (mPos ? (*mPos) : SMMatrixIdentity());
		
		float4x4 worldmat = tmpmatrot * tmpmatpos;

		float4x4 vp = MCamView * MCamProj;
		pe_data::pDXDevice->setTexture(SGCore_LoadTexGetTex(m_idTex));
		if (m_oData.m_isSoft)
		{
			if (pe_data::rt_id::idDepthScene >= 0)
				pe_data::pDXDevice->setTexture(SGCore_RTGetTexture(pe_data::rt_id::idDepthScene), 1);
			else
				LibReport(REPORT_MSG_LEVEL_WARNING, "sxparticles - not init depth map\n");
		}

	//	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticles);

		static ID psid = -1;

		if (m_oData.m_isSoft && !m_oData.m_useRefraction && !m_oData.m_isLighting)
		{
			psid = pe_data::shader_id::ps::idParticlesSoft;
			SGCore_ShaderBind(pe_data::shader_id::kit::idParticlesSoft);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoft, "SoftCoef", &m_oData.m_fSoftCoef);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoft, "NearFar", &NearFar);
		}
		else if (m_oData.m_isSoft && m_oData.m_useRefraction && !m_oData.m_isLighting)
		{
			psid = pe_data::shader_id::ps::idParticlesSoftRefraction;
			SGCore_ShaderBind(pe_data::shader_id::kit::idParticlesSoftRefraction);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftRefraction, "SoftCoef", &m_oData.m_fSoftCoef);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftRefraction, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftRefraction, "RefractCoef", &m_oData.m_fRefractionCoef);
		}
		else if (m_oData.m_isSoft && m_oData.m_useRefraction && m_oData.m_isLighting)
		{
			psid = pe_data::shader_id::ps::idParticlesSoftRefractionLight;
			SGCore_ShaderBind(pe_data::shader_id::kit::idParticlesSoftRefractionLight);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftRefractionLight, "SoftCoef", &m_oData.m_fSoftCoef);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftRefractionLight, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftRefractionLight, "RefractCoef", &m_oData.m_fRefractionCoef);
		}
		else if (m_oData.m_isSoft && !m_oData.m_useRefraction && m_oData.m_isLighting)
		{
			psid = pe_data::shader_id::ps::idParticlesSoftLight;
			SGCore_ShaderBind(pe_data::shader_id::kit::idParticlesSoftLight);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftLight, "SoftCoef", &m_oData.m_fSoftCoef);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftLight, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesSoftLight, "RefractCoef", &m_oData.m_fRefractionCoef);
		}
		else if (!m_oData.m_isSoft && m_oData.m_useRefraction && m_oData.m_isLighting)
		{
			psid = pe_data::shader_id::ps::idParticlesRefractionLight;
			SGCore_ShaderBind(pe_data::shader_id::kit::idParticlesRefractionLight);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesRefractionLight, "RefractCoef", &m_oData.m_fRefractionCoef);
		}
		else if (!m_oData.m_isSoft && !m_oData.m_useRefraction && m_oData.m_isLighting)
		{
			psid = pe_data::shader_id::ps::idParticlesLight;
			SGCore_ShaderBind(pe_data::shader_id::kit::idParticlesLight);
		}
		else if (!m_oData.m_isSoft && m_oData.m_useRefraction && !m_oData.m_isLighting)
		{
			psid = pe_data::shader_id::ps::idParticlesRefraction;
			SGCore_ShaderBind(pe_data::shader_id::kit::idParticlesRefraction);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesRefraction, "RefractCoef", &m_oData.m_fRefractionCoef);
		}
		else
		{
			psid = pe_data::shader_id::ps::idParticles;
			SGCore_ShaderBind(pe_data::shader_id::kit::idParticles);
		}

		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticles, "ViewProjection", &SMMatrixTranspose(vp));
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticles, "WorldViewProjection", &SMMatrixTranspose(worldmat * vp));
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticles, "CamRot", &SMMatrixTranspose(cammat));
		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticles, "World", &SMMatrixTranspose(world));
		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticles, "MatRot", &SMMatrixTranspose(tmpmatrot));
		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticles, "MatPos", &SMMatrixTranspose(tmpmatpos));
		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticles, "PosCam", &ConstCamPos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, psid, "ColorCoef", &m_oData.m_fColorCoef);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, psid, "Color", &m_oData.m_vColor);

		/*pe_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		if (m_oData.m_typeAlphaBlend == PARTICLESTYPE_ALPHABLEND_ALPHA)
		{
			pe_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			pe_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			pe_data::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pe_data::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		else if (m_oData.m_typeAlphaBlend == PARTICLESTYPE_ALPHABLEND_ADD)
		{
			pe_data::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			pe_data::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			pe_data::pDXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		}*/

		pe_data::pDXDevice->setBlendState(pe_data::pBlendStates[m_oData.m_typeAlphaBlend]);

		pe_data::pDXDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);

		if(m_oData.m_typeFigure == PARTICLESTYPE_FIGURE_QUAD_COMPOSITE)
			pe_data::pDXDevice->drawIndexedInstanced(tmpcount, 4 * m_oData.m_iFigureCountQuads, 2 * m_oData.m_iFigureCountQuads, 0, 0);
		else
			pe_data::pDXDevice->drawIndexedInstanced(tmpcount, 4, 2, 0, 0);

		pe_data::pDXDevice->setBlendState(NULL);
		//pe_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		SGCore_ShaderUnBind();
	}

	if (!m_oData.m_useTrack)
		return;

	bool exists_track = false;

	for (int i = 0; i < m_iCount; ++i)
	{
		if (m_pArr[i].Track)
		{
			exists_track = true;
			break;
		}
	}

	if (exists_track)
	{
		CommonParticleDecl2* RTGPUArrVerteces;
		int tmpcount = 0;
		if(m_pTransVertBuf->lock((void**)&RTGPUArrVerteces, GXBL_WRITE))
		{
			for(int i = 0; i<m_iCount; ++i)
			{
				if(TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - m_pArr[i].TrackTime > m_oData.m_uiTrackTime)
					m_pArr[i].Track = false;

				if(m_pArr[i].Track)
				{
					RTGPUArrVerteces[tmpcount].pos = m_pArr[i].TrackPos;
					RTGPUArrVerteces[tmpcount].tex = (float4_t)m_pArr[i].TrackNormal;
					RTGPUArrVerteces[tmpcount].alpha = 1.f - (float(TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - m_pArr[i].TrackTime) / float(m_oData.m_uiTrackTime));
					RTGPUArrVerteces[tmpcount].size = m_oData.m_fTrackSize;
					++tmpcount;
				}
			}

			m_pTransVertBuf->unlock();
		}

		if (tmpcount <= 0)
			return;

		pe_data::pDXDevice->setRenderBuffer(m_pRenderBuffQuad);
		pe_data::pDXDevice->setIndexBuffer(m_pIndexBuffQuad);
		
		SGCore_ShaderBind(pe_data::shader_id::kit::idParticlesTrack);

		/*pe_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		pe_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pe_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		pe_data::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pe_data::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/

		pe_data::pDXDevice->setBlendState(pe_data::pBlendStates[PARTICLESTYPE_ALPHABLEND_ALPHA]);

		static float4x4 MCamView;
		static float4x4 MCamProj;
		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &MCamView);
		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &MCamProj);

		float4x4 vp = MCamView * MCamProj;
		pe_data::pDXDevice->setTexture(SGCore_LoadTexGetTex(m_idTexTrack));

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pe_data::shader_id::vs::idParticlesTrack, "WorldViewProjection", &SMMatrixTranspose(vp));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesTrack, "Color", &m_oData.m_vColor);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pe_data::shader_id::ps::idParticlesTrack, "ColorCoef", &m_oData.m_fColorCoef);


		pe_data::pDXDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);
		pe_data::pDXDevice->drawIndexedInstanced(tmpcount, 4, 2, 0, 0);

		pe_data::pDXDevice->setBlendState(NULL);
		//pe_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		SGCore_ShaderUnBind();
	}
}

int CEmitter::getTrackCount()
{
	if (!m_isEnable || !m_oData.m_useTrack || !m_oData.m_useCollisionDelete)
		return 0;

	int iCountTrack = 0;

	for (int i = 0; i < m_iCount; ++i)
	{
		if (m_pArr[i].Track)
			++iCountTrack;
	}

	return iCountTrack;
}

int CEmitter::getTrackPos(float3 **pArr, int iCount)
{
	if (!pArr || !m_isEnable || !m_oData.m_useTrack || !m_oData.m_useCollisionDelete)
		return 0;

	int iCountTrack = 0;

	for (int i = 0; i < m_iCount; ++i)
	{
		if (m_pArr[i].Track)
		{
			if (iCountTrack >= iCount)
				break;

			(*pArr)[iCountTrack] = m_pArr[i].TrackPos;
			++iCountTrack;
		}
	}

	return iCountTrack;
}