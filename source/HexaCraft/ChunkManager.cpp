#include "ChunkManager.h"


CChunkManager::CChunkManager(IXCore *pCore):
	m_pCore(pCore),
	m_updateTask(this)
{
	memset(m_aapChunks, 0, sizeof(m_aapChunks));

	m_pResourceManager = pCore->getResourceManager();
	
	m_iTotalTextureCount = CBlockManager::CountMaterials();

	m_pTaskRunner = pCore->newAsyncTaskRunner();

	m_isDone.store(true);
}

CChunkManager::~CChunkManager()
{
	mem_release(m_pTaskRunner);
	/*for(std::map<int64_t, std::map<int64_t, DSHCCCelem>>::iterator i = m_mChunks.begin(); i != m_mChunks.end(); i++)
	{
	for(std::map<int64_t, DSHCCCelem>::iterator j = i->second.begin(); j != i->second.end(); j++)
	{
	i->second[j->first].pChunk->Release();
	m_dChunks.Delete(i->second[j->first].pChunk);
	}
	i = m_mChunks.erase(i);
	}*/
	
//	mem_delete_a(m_pLODs);
}

static UINT GetChunkIndex(int64_t id)
{
	while(id < 0)
	{
		id += HC_CHUNK_CACHE_SIZE;
	}
	return(id % HC_CHUNK_CACHE_SIZE);
}

void CChunkManager::shift(int x, int z)
{
	m_iXdelta += x;
	m_iZdelta += z;
}
void CChunkManager::shift()
{
#if 0
	//return;
	XMFLOAT3 cp = DS::pPlayer->GetPos();

	static float dx = (float)(3 * (HC_CHUNK_SIZE_X >> 1) * (UINT)HC_R);
	static float dz = (float)(HC_CHUNK_SIZE_Z * Rcos30x2);

	static float fdx = dx * (float)HC_SIZE / 4.0f;
	static float fdz = dz * (float)HC_SIZE / 4.0f;

	if(abs(cp.x) > fdx || abs(cp.z) > fdz)
	{
		//static XMFLOAT3 lp = pCamera->GetPos();
		//XMFLOAT3 cp = pCamera->GetPos();

		//while(cp.x - lp.x > (float)dx)
		while(cp.x >= dx)
		{
			cp.x -= dx;
			DS::pDSHCCM->Shift(1, 0);
		}
		while(cp.x < 0.0f)
		{
			cp.x += dx;
			DS::pDSHCCM->Shift(-1, 0);
		}

		while(cp.z >= dz)
		{
			cp.z -= dz;
			DS::pDSHCCM->Shift(0, 1);
		}
		while(cp.z < 0.0f)
		{
			cp.z += dz;
			DS::pDSHCCM->Shift(0, -1);
		}
		
	}
	if(!m_iXshift && !m_iZshift)
	{
		return;
	}
	DS_WAIT(m_bSemRendering);
	//DS_WAIT(DS::bInRendering);
	m_bSemShifting = true;
	m_rRenderRect.Xmin += m_iXshift;
	m_rRenderRect.Xmax += m_iXshift;

	m_rRenderRect.Zmin += m_iZshift;
	m_rRenderRect.Zmax += m_iZshift;
	//DSphysics::Lock();
	
	//DSphysics::Unlock();
	UINT cI = (m_iCurRC + 1) & 1;
	int64_t X, Z;
	for(int i = m_rRenderRect.Xmin, ii = 0; i <= m_rRenderRect.Xmax && DS::bRunning; i++, ii++)//to south
	{
		for(int j = m_rRenderRect.Zmin, jj = 0; j <= m_rRenderRect.Zmax && DS::bRunning; j++, jj++) // to east
		{
			CChunk * ch = GetChunk((int64_t)i, (int64_t)j);
			//ch->GetPos(&X, &Z);
			//DSconsole::write("R (%d, %d) -> (%lld, %lld)\n", i, j, X, Z);
			m_pppRenderChunks[cI][ii][jj] = ch;
			//m_pppRenderChunks[cI][ii][jj]->GetPos(&X, &Z);
			//DSconsole::write("(%d, %d) -> (%lld, %lld)\n", ii, jj, X, Z);
		}
		//DSconsole::write("\n");
	}

	/*for(int i = m_rRenderRect.Xmin - m_iXshift, ii = 0; i <= m_rRenderRect.Xmax - m_iXshift && DS::bRunning; i++, ii++)//to south
	{
		for(int j = m_rRenderRect.Zmin - m_iZshift, jj = 0; j <= m_rRenderRect.Zmax - m_iZshift && DS::bRunning; j++, jj++) // to east
		{
			CChunk * ch = GetChunk(i, j);
			ch->UpdatePos();
		}
	}*/

	m_iXdelta += m_iXshift;
	m_iZdelta += m_iZshift;

	for(int i = m_rRenderRect.Xmin, ii = 0; i <= m_rRenderRect.Xmax && DS::bRunning; i++, ii++)//to south
	{
		for(int j = m_rRenderRect.Zmin, jj = 0; j <= m_rRenderRect.Zmax && DS::bRunning; j++, jj++) // to east
		{
			CChunk * ch = GetChunk(i, j);
			ch->UpdatePos();
		}
	}

	float fx = dx * (float)m_iXshift;
	float fz = dz * (float)m_iZshift;
	DSphysics::Shift(-fx, -fz);
	XMFLOAT3 cpos = DS::pCamera->GetPos();
	cpos.x -= fx;
	cpos.z -= fz;
	DS::pCamera->SetPos(cpos);
	//float fX = m_iZshift;
	//DS::pPlayer->SetPos(DS::pPlayer->GetPos() + dp);

	m_iXshift = m_iZshift = 0;
	m_iCurRC = (m_iCurRC + 1) & 1;
	m_bSemShifting = false;

	{
		UINT cc = 0;
		//DSconsole::write("Preloading %dx%d chunks\n", cc * 2, cc * 2);
		int64_t cx = (m_rRenderRect.Xmax + m_rRenderRect.Xmin) / 2;
		int64_t cz = (m_rRenderRect.Zmax + m_rRenderRect.Zmin) / 2;

		cc = HC_SIZE / 2 + 2;

		for(std::map<int64_t, std::map<int64_t, DSHCCCelem>>::iterator i = m_mChunks.begin(); i != m_mChunks.end(); i++)
		{
			if(i->first < cx - cc || i->first >= cx + cc)
			{
				for(std::map<int64_t, DSHCCCelem>::iterator j = i->second.begin(); j != i->second.end(); j++)
				{
					//FreeChunk(i->first, j->first, true);
					//i->second[j->first].pChunk->Release();
					//m_dChunks.Delete(i->second[j->first].pChunk);
					i->second[j->first].pChunk->ReleasePhysics();
				}
				//i = m_mChunks.erase(i);
			}
			else
			{
				for(std::map<int64_t, DSHCCCelem>::iterator j = i->second.begin(); j != i->second.end(); j++)
				{
					if(j->first < cz - cc || j->first >= cz + cc)
					{
						//FreeChunk(i->first, j->first, true);
						//i->second[j->first].pChunk->Release();
						//m_dChunks.Delete(i->second[j->first].pChunk);
						//j = i->second.erase(j);
						i->second[j->first].pChunk->ReleasePhysics();
					}
				}
				if(i->second.size() == 0)
				{
					i = m_mChunks.erase(i);
				}
			}
		}
	}


	for(int i = m_rRenderRect.Xmin - 1, ii = 0 - 1; i <= m_rRenderRect.Xmax + 1 && DS::bRunning && !m_iXshift && !m_iZshift; i++, ii++)//to south
	{
		for(int j = m_rRenderRect.Zmin - 1, jj = 0 - 1; j <= m_rRenderRect.Zmax + 1 && DS::bRunning && !m_iXshift && !m_iZshift; j++, jj++) // to east
		{
			CChunk * ch = GetChunk(i, j);
			ch->SetNeighbour(GetChunk(i - 1, j), HC_CHUNK_NEIGHBOR_NORTH);
			ch->SetNeighbour(GetChunk(i + 1, j), HC_CHUNK_NEIGHBOR_SOUTH);
			ch->SetNeighbour(GetChunk(i, j + 1), HC_CHUNK_NEIGHBOR_EAST);
			ch->SetNeighbour(GetChunk(i, j - 1), HC_CHUNK_NEIGHBOR_WEST);
		}
	}
	
	Generate(0);
	/*for(int i = m_rRenderRect.Xmin - ((HC_LOD_SIZE - (m_rRenderRect.Xmax - m_rRenderRect.Xmin)) >> 1); i < m_rRenderRect.Xmax + ((HC_LOD_SIZE - (m_rRenderRect.Xmax - m_rRenderRect.Xmin)) >> 1) && DS::bRunning; i++)//to south
	{
		for(int j = m_rRenderRect.Zmin - ((HC_LOD_SIZE - (m_rRenderRect.Zmax - m_rRenderRect.Zmin)) >> 1); j < m_rRenderRect.Zmax + ((HC_LOD_SIZE - (m_rRenderRect.Zmax - m_rRenderRect.Zmin)) >> 1) && DS::bRunning; j++) // to east
		{
			GetChunk(i, j)->LODinit(&m_pLODs[0]);
		}
	}*/
	//m_pLODs[0].Build();
#endif
}


#if 0
void CChunkManager::ShiftX(std::string prm)
{
	DS::pDSHCCM->Shift(strToNum(prm), 0);
}

void CChunkManager::ShiftZ(std::string prm)
{
	DS::pDSHCCM->Shift(0, strToNum(prm));
}
#endif


#if 0
float CChunkManager::isOnGround(XMFLOAT3 pos)
{
	if(!m_bInitiated)
	{
		return(-1.0f);
	}
	CChunk * pc = FindRenderableChunkByXZ(pos.x, pos.z);
	if(!pc)
	{
		return(-1.0f);
	}
	return(pc->IsOnGround(pos));
	/*for(int i = 0; i < HC_SIZE && DS::bRunning; i++)//to south
	{
		for(int j = 0; j < HC_SIZE && DS::bRunning; j++) // to east
		{
			if(m_pppRenderChunks[i][j])
			{
				if(m_pppRenderChunks[i][j]->InMyInterval(pos))
				{
					return(m_pppRenderChunks[i][j]->IsOnGround(pos));
				}
			}
		}
	}*/
	return(-1.0f);
}

float CChunkManager::isOnGround(DSHCplayer * pl)
{
	if(pl)
	{
		CChunk * pc;
		if((pc = pl->GetChunk()))
		{
			return(pc->IsOnGround(pl->GetPos()));
			//return(pc->IsOnGround(pl));
		}
	}
	return(-1.0f);
}
#endif

void CChunkManager::generate(IEventChannel<XEventLevelProgress> *pEventChannel, XEventLevelProgress *pEv)
{
	static UINT c = 0;
	/*for(int i = m_rRenderRect.Xmin; i <= m_rRenderRect.Xmax && DS::bRunning && !m_iXshift && !m_iZshift; i++)//to south
	{
		for(int j = m_rRenderRect.Zmin; j <= m_rRenderRect.Zmax && DS::bRunning && !m_iXshift && !m_iZshift; j++) // to east
		{
			if(module && ((i - m_rRenderRect.Xmin) * (m_rRenderRect.Xmax - m_rRenderRect.Xmin) + (j - m_rRenderRect.Zmin)) % (module & 0xFF) != (module >> 8))
			{
				continue;
			}
			GetChunk(i, j, false)->BuildBuffer2();
			//c++;
		}
	}*/


	UINT uMaxChunks = (HC_SIZE * 2 + 1) * (HC_SIZE * 2 + 1), uChunk = 0;
	for(int64_t i = -HC_SIZE; i <= HC_SIZE; ++i)//to south
	{
		for(int64_t j = -HC_SIZE; j <= HC_SIZE; ++j) // to east
		{
			getChunk(i + m_iXplayer, j + m_iZplayer, false)->buildBuffer2();
			++uChunk;
			if(pEv && pEventChannel)
			{
				pEv->fProgress = (float)uChunk / (float)uMaxChunks;
				pEventChannel->broadcastEvent(pEv);
			}
		}
	}

	//c = c;
	//HC_LOD_SIZE
	//if(module >> 8 == 0)
	
	/*for(int i = 0; i < HC_GEN_SIZE && DS::bRunning; i++)//to south
	{
		for(int j = 0; j < HC_GEN_SIZE && DS::bRunning; j++) // to east
		{
			if(module && (i * HC_GEN_SIZE + j) % (module & 0xFF) != (module >> 8))
			{
				continue;
			}
			if(!m_vvChunks[i][j]->ReadFile())
			{
				m_vvChunks[i][j]->MakeRandom();
				//m_vvChunks[i][j]->BuildBuffer2();
				//DSconsole::write("Generated %3d %3d\n", i, j);
				m_vvChunks[i][j]->WriteFile(numToStr(i) + "_" + numToStr(j));
			}
			m_vvChunks[i][j]->LODinit(&m_pLODs[0]);
		}
		//DSconsole::write("\n");
	}*/
	/*for(int i = (HC_GEN_SIZE - HC_SIZE) / 2; i < (HC_GEN_SIZE - HC_SIZE) / 2 + HC_SIZE && DS::bRunning; i++)//to south
	{
		for(int j = (HC_GEN_SIZE - HC_SIZE) / 2; j < (HC_GEN_SIZE - HC_SIZE) / 2 + HC_SIZE && DS::bRunning; j++) // to east
		{
			if(module && (i * HC_GEN_SIZE + j) % (module & 0xFF) != (module >> 8))
			{
				continue;
			}
			if(m_vvChunks[i][j])
			{
				m_vvChunks[i][j]->BuildBuffer2();
			}
		}
	}*/
	/*for(int i = 0; i < HC_SIZE && DS::bRunning; i++)//to south
	{
		for(int j = 0; j < HC_SIZE && DS::bRunning; j++) // to east
		{
			if(m_vvChunks[i][j])
			{
				m_vvChunks[i][j]->BuildBuffer2();
			}
		}
	}*/

#if 0
	static UINT iDone = 0;
	if(iDone != HC_GEN_THREADS)
	{
		iDone++;
	}
	if(iDone == HC_GEN_THREADS)
	{
		//{
		if(module)
		{
			/*for(int i = m_rRenderRect.Xmin - ((HC_LOD_SIZE - (m_rRenderRect.Xmax - m_rRenderRect.Xmin)) >> 1); i < m_rRenderRect.Xmax + ((HC_LOD_SIZE - (m_rRenderRect.Xmax - m_rRenderRect.Xmin)) >> 1) && DS::bRunning; i++)//to south
			{
				for(int j = m_rRenderRect.Zmin - ((HC_LOD_SIZE - (m_rRenderRect.Zmax - m_rRenderRect.Zmin)) >> 1); j < m_rRenderRect.Zmax + ((HC_LOD_SIZE - (m_rRenderRect.Zmax - m_rRenderRect.Zmin)) >> 1) && DS::bRunning; j++) // to east
				{
					//if(module && ((i - (m_rRenderRect.Xmin - ((HC_LOD_SIZE - (m_rRenderRect.Xmax - m_rRenderRect.Xmin)) >> 1))) * (m_rRenderRect.Xmax - m_rRenderRect.Xmin + 2 * ((HC_LOD_SIZE - (m_rRenderRect.Xmax - m_rRenderRect.Xmin)) >> 1)) + (j - (m_rRenderRect.Zmin - ((HC_LOD_SIZE - (m_rRenderRect.Zmax - m_rRenderRect.Zmin)) >> 1)))) % (module & 0xFF) != (module >> 8))
					//if(module >> 8 != 0)
					//{
					//	continue;
					//}
					GetChunk(i, j)->LODinit(&m_pLODs[0]);
					c++;
				}
			}
			//}
			m_pLODs[0].Build();*/
		}
		while(DS::bRunning && module)
		{
			DS::pDSHCCM->Shift();
			DS::sleep(10);
		}
	}
#endif
}

CChunk* CChunkManager::getChunk(const int64_t x, const int64_t z, bool addRef)
{
	int64_t X, Z;

	CChunk *pChunk = m_aapChunks[GetChunkIndex(x)][GetChunkIndex(z)];

	if(pChunk)
	{
		pChunk->getPos(&X, &Z);
		if(x == X && z == Z)
		{
			if(addRef)
			{
				add_ref(pChunk);
			}
			return(pChunk);
		}
	}
	
	pChunk = m_dChunks.Alloc(x, z, this);
	if(!pChunk->readFile())
	{
		pChunk->makeRandom();
		//el.pChunk->WriteFile();
	}

	mem_release(m_aapChunks[GetChunkIndex(x)][GetChunkIndex(z)]);

	m_aapChunks[GetChunkIndex(x)][GetChunkIndex(z)] = pChunk;

	return(pChunk);
}

void CChunkManager::freeChunk(int64_t x, int64_t z, bool bForce)
{
	CChunk *pChunk = m_aapChunks[GetChunkIndex(x)][GetChunkIndex(z)];
	if(pChunk)
	{
		int64_t X, Z;
		pChunk->getPos(&X, &Z);
		if(x == X && z == Z)
		{
			mem_release(m_aapChunks[GetChunkIndex(x)][GetChunkIndex(z)]);
		}
	}
}

void CChunkManager::saveAll()
{
	for(UINT i = 0; i < HC_CHUNK_CACHE_SIZE; ++i)
	{
		for(UINT j = 0; j < HC_CHUNK_CACHE_SIZE; ++j)
		{
			SAFE_CALL(m_aapChunks[i][j], save);
		}
	}

	saveState();
}

void CChunkManager::preLoad()
{
	//return;
	saveAll();

	int64_t x, z, dx, dz;

	int iUnloadDelta = HC_SIZE + HC_HYSTER_SIZE;

	for(UINT i = 0; i < HC_CHUNK_CACHE_SIZE; ++i)
	{
		for(UINT j = 0; j < HC_CHUNK_CACHE_SIZE; ++j)
		{
			if(m_aapChunks[i][j])
			{
				m_aapChunks[i][j]->getPos(&x, &z);

				dx = x - m_iXplayer;
				dz = z - m_iZplayer;
				if(dx > iUnloadDelta || dx < -iUnloadDelta || dz > iUnloadDelta || dz < -iUnloadDelta)
				{
					m_aapChunks[i][j]->dematerialize();
				}
			}
		}
	}

	x = m_iXplayer;
	z = m_iZplayer;

	for(int s = 0; s < HC_SIZE; ++s)
	{
		for(int64_t i = -s; i <= s; ++i) //to south
		{
			for(int64_t j = -s; j <= s; ++j) // to east
			{
				getChunk(i + x, j + z, false)->buildBuffer2();
			}
		}

		if(x != m_iXplayer || z != m_iZplayer)
		{
			x = m_iXplayer;
			z = m_iZplayer;
			s = 0;
		}
	}

	m_isDone.store(true);

#if 0
	if(m_uLastSize)
	{
		forceUnload();
	}
	else
	{
		UINT cs = CChunk::GetSize();
		DSMEMINFO mi;
		DS::GetMemInfo(&mi);
		UINT cc;
		if(mi.uAvailVirtual > 1024000000)
		{
			cc = (mi.uAvailVirtual - 1024000000) / cs;
		}
		else
		{
			cc = mi.uAvailVirtual / cs;
		}
		cc = sqrt(cc) / 2;
		cc = 4 + HC_SIZE;
		m_uLastSize = cc;
	}
	m_uLastSize = 10;
	UINT cc = m_uLastSize;
	//DSconsole::write("Preloading %dx%d chunks\n", cc * 2, cc * 2);
	int64_t cx = (m_rRenderRect.Xmax + m_rRenderRect.Xmin) / 2;
	int64_t cz = (m_rRenderRect.Zmax + m_rRenderRect.Zmin) / 2;
	for(int64_t i = cx - cc; i < cx + cc; i++)
	{
		for(int64_t j = cz - cc; j < cz + cc; j++)
		{
			getChunk(i, j);
		}
	}
#endif
}

void CChunkManager::forceUnload()
{
#if 0
	UINT cc = m_uLastSize;
	//DSconsole::write("Preloading %dx%d chunks\n", cc * 2, cc * 2);
	int64_t cx = (m_rRenderRect.Xmax + m_rRenderRect.Xmin) / 2;
	int64_t cz = (m_rRenderRect.Zmax + m_rRenderRect.Zmin) / 2;

	for(std::map<int64_t, std::map<int64_t, DSHCCCelem>>::iterator i = m_mChunks.begin(); i != m_mChunks.end(); i++)
	{
		if(i->first < cx - cc || i->first >= cx + cc)
		{
			for(std::map<int64_t, DSHCCCelem>::iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				//FreeChunk(i->first, j->first, true);
				i->second[j->first].pChunk->Release();
				m_dChunks.Delete(i->second[j->first].pChunk);
			}
			i = m_mChunks.erase(i);
		}
		else
		{
			for(std::map<int64_t, DSHCCCelem>::iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				if(j->first < cz - cc || j->first >= cz + cc)
				{
					//FreeChunk(i->first, j->first, true);
					i->second[j->first].pChunk->Release();
					m_dChunks.Delete(i->second[j->first].pChunk);
					j = i->second.erase(j);
				}
			}
			if(i->second.size() == 0)
			{
				i = m_mChunks.erase(i);
			}
		}
	}
#endif
}

void CChunkManager::findChunkByCoord(const float3 &vPos, int64_t *pX, int64_t *pZ)
{
	float x = vPos.x + HC_R * 0.75f;
	float z = vPos.z + HC_R * cos30 * 1.5f;

	float fSizeX = HC_R * HC_CHUNK_SIZE_X * 1.5f;
	float fSizeZ = HC_CHUNK_SIZE_Z * 2 * HC_R * cos30;

	*pX = (int64_t)(x / fSizeX) + m_iXdelta;
	*pZ = (int64_t)(z / fSizeZ) + m_iZdelta;




#if 0
	float x = vPos.x;
	float z = vPos.z;
	z += Rcos30;
	CChunk * res = NULL;
	int64_t m_iXpos = 0;
	int64_t m_iZpos = 0;
	//m_pppRenderChunks[m_iCurRC][0][0]->getPos(&m_iXpos, &m_iZpos);
	int64_t px, pz;
	getDelta(&px, &pz);
	float fXcur = (float)(HC_CHUNK_SIZE_X * (m_iXpos - px) / 2) * 3.0f * HC_R;
	float fZcur = (float)(HC_CHUNK_SIZE_Z * (m_iZpos - pz)) * Rcos30x2;
	float fXofs = (float)(HC_CHUNK_SIZE_X >> 1) * 3.0f * HC_R;
	float fZofs = (float)(HC_CHUNK_SIZE_X) * Rcos30x2;
	float fZofs0 = Rcos30x2;
	//fZcur -= Rcos30x2;
	int iXstripe = -1;
	int iZstripe = -1;
	for(int i = 0; i < HC_SIZE; i++) // through X dimension
	{
		if(x > fXcur - HC_R && x <= fXcur + fXofs - Rdiv2)
		{
			if(x > fXcur + fXofs - HC_R && x <= fXcur + fXofs * 2.0f - Rdiv2) // in case of border
			{
				//border checks
				int indz = (z - fZcur) / fZofs0;
				float cz = z - (fZcur + fZofs0 * indz);
				//float cx = fXcur + fXofs - HC_R;
				float cx = x - (fXcur + fXofs - HC_R);
				if(cz < Rcos30)
				{
					if(cx < cz / (2 * cos30))
					{
						iXstripe = i;
					}
					else
					{
						iXstripe = i + 1;
						fXcur += fXofs;
					}
				}
				else
				{
					if(cx < Rdiv2 - (cz - Rcos30) / (2 * cos30))
					{
						iXstripe = i;
					}
					else
					{
						iXstripe = i + 1;
						fXcur += fXofs;
					}
				}
			}
			else
			{
				iXstripe = i;
			}
			break;
		}
		fXcur += fXofs;
	}
	if(iXstripe >= 0)
	{
		for(int j = 0; j < HC_SIZE; j++)
		{
			if(z > fZcur - Rcos30 && z <= fZcur + fZofs)
			{
				if(z > fZcur + fZofs - Rcos30 && z <= fZcur + fZofs * 2.0f)
				{
					float cz = z - (fZcur + fZofs - Rcos30);
					int indx = (x - fXcur) / (3.0f * HC_R);
					float cx = x - (3.0f * HC_R) * indx;
					if(cx < Rdiv2 || cx >= 2.5f * HC_R) // ++
					{
						iZstripe = j + 1;
					}
					else if(cx >= HC_R && cx < 2.0f * HC_R) // --
					{
						iZstripe = j;
					}
					else if(cx < HC_R && cx >= Rdiv2) // +- zOne
					{
						if(cx >= Rdiv2 + 0.5f * cz) // --
						{
							iZstripe = j;
						}
						else // ++
						{
							iZstripe = j + 1;
						}
					}
					else // -+ zOne
					{
						if(cx >= 2.5f * HC_R - 0.5f * cz) // ++
						{
							iZstripe = j + 1;
						}
						else // --
						{
							iZstripe = j;
						}
					}
				}
				else
				{
					iZstripe = j;
				}
				break;
			}
			fZcur += fZofs;
		}
		if(iZstripe >= 0)
		{
			res = m_pppRenderChunks[m_iCurRC][iXstripe][iZstripe];
		}
	}
	return(res);
#endif
}

#if 0
XMFLOAT3 CChunkManager::fixPosition(XMFLOAT3 pos)
{
	CChunk * ch = FindRenderableChunkByXZ(pos.x, pos.z);
	if(ch)
	{
		return(ch->FixPlayerPos(pos));
	}
	return(pos);
}

void CChunkManager::fixPosition(DSHCplayer * pl)
{
	if(pl)
	{
		CChunk * pc = pl->GetChunk();
		if(pc)
		{
			pc->FixPlayerPos(pl);
		}
	}
}
#endif

void CChunkManager::findAimingBlock(const float3 &pos, const float3 &vec, float len, CChunk *chunk, DSHCaimPos *pAP)
{
	chunk->findAimingBlock(pos, vec, len, pAP);
}

int CChunkManager::ClearMem(size_t)
{
#if 0
	DS::pDSHCCM->ForceUnload();
#endif
	return(1);
}

#if 0
void CChunkManager::DumpRenderChunkCMD(std::string)
{
	DS::pDSHCCM->DumpRenderChunk();
}
#endif

void CChunkManager::unload()
{
	saveAll();

	for(UINT i = 0; i < HC_CHUNK_CACHE_SIZE; ++i)
	{
		for(UINT j = 0; j < HC_CHUNK_CACHE_SIZE; ++j)
		{
			mem_release(m_aapChunks[i][j]);
		}
	}
}

void CChunkManager::enqueueChunkDelete(CChunk *pChunk)
{
	int64_t x, y;
	pChunk->getPos(&x, &y);

	if(m_aapChunks[GetChunkIndex(x)][GetChunkIndex(y)] == pChunk)
	{
		m_aapChunks[GetChunkIndex(x)][GetChunkIndex(y)] = NULL;
	}

	m_dChunks.Delete(pChunk);
}

IFileSystem* CChunkManager::getFileSystem()
{
	return(m_pCore->getFileSystem());
}

/** How to find block in chunk
for(int i = 0; i < HC_SIZE; i++) // through X dimension
{
if(x > fXcur - Rdiv2 && x <= fXcur + Rdiv2)
{
if(x > (fXcur + HC_R * 1.5) - Rdiv2 && x <= (fXcur + HC_R * 1.5) + Rdiv2)
{

}
}
fXcur += HC_R * 1.5;
}
*/


void CChunkManager::setCameraPos(const float3 &vPos)
{
	m_vCameraPos = vPos;

	m_pTaskRunner->runCallbacks();

	int64_t x, z;
	findChunkByCoord(vPos, &x, &z);

	if(x != m_iXplayer || z != m_iZplayer)
	{
		m_iXplayer = x;
		m_iZplayer = z;

		// start update task
		if(m_isDone.load())
		{
			m_isDone.store(false);
			m_pTaskRunner->runTask(&m_updateTask);
		}
	}
}

void CChunkManager::saveState()
{
	/*int64_t m_iXdelta = 0;
	int64_t m_iZdelta = 0;

	int64_t m_iXplayer = 0;
	int64_t m_iZplayer = 0;
	
	m_vCameraPos
	*/

	char szPath[256], szVal[256];
	sprintf(szPath, "levels/%s/hcState.ini", m_sLevelName);

	IXConfig *pConfig = m_pCore->newConfig();
	pConfig->open(szPath);

	const char *szSection = "state";

	sprintf(szVal, "%f %f %f", m_vCameraPos.x, m_vCameraPos.y - 1.6f, m_vCameraPos.z);
	pConfig->set(szSection, "vCamPos", szVal);

	sprintf(szVal, "%lld", m_iXdelta);
	pConfig->set(szSection, "iXdelta", szVal);

	sprintf(szVal, "%lld", m_iZdelta);
	pConfig->set(szSection, "iZdelta", szVal);

	sprintf(szVal, "%lld", m_iXplayer);
	pConfig->set(szSection, "iXplayer", szVal);

	sprintf(szVal, "%lld", m_iZplayer);
	pConfig->set(szSection, "iZplayer", szVal);

	pConfig->save();

	mem_release(pConfig);
}
void CChunkManager::loadState()
{
	if(!m_pSpawnPoint)
	{
		m_pSpawnPoint = SGame_CreateEntity("info_player_spawn");
	}

	char szPath[256];
	sprintf(szPath, "levels/%s/hcState.ini", m_sLevelName);

	const char *szSection = "state", *szVal;
	IXConfig *pConfig = m_pCore->newConfig();
	if(pConfig->open(szPath))
	{
		szVal = pConfig->getKey(szSection, "vCamPos");
		m_vCameraPos = float3_t();
		if(szVal)
		{
			sscanf(szVal, "%f %f %f", &m_vCameraPos.x, &m_vCameraPos.y, &m_vCameraPos.z);
		}

		szVal = pConfig->getKey(szSection, "iXdelta");
		m_iXdelta = 0;
		if(szVal)
		{
			sscanf(szVal, "%lld", &m_iXdelta);
		}

		szVal = pConfig->getKey(szSection, "iZdelta");
		m_iZdelta = 0;
		if(szVal)
		{
			sscanf(szVal, "%lld", &m_iZdelta);
		}

		szVal = pConfig->getKey(szSection, "iXplayer");
		m_iXplayer = 0;
		if(szVal)
		{
			sscanf(szVal, "%lld", &m_iXplayer);
		}

		szVal = pConfig->getKey(szSection, "iZplayer");
		m_iZplayer = 0;
		if(szVal)
		{
			sscanf(szVal, "%lld", &m_iZplayer);
		}

		m_pSpawnPoint->setPos(m_vCameraPos);
	}

	mem_release(pConfig);
}

//##########################################################################

void XMETHODCALLTYPE CUpdateTask::exec()
{
	m_pChunkManager->preLoad();
}

