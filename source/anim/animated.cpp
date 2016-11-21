namespace SkyXEngine
{
namespace Graphics
{
namespace ThreeD
{

/**
 *
 * ModelFile implementation
 *
 */
	ModelFile::ModelFile(const char * name):
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
		m_ppIndexBuffer(NULL)
	{
		memset(&m_hdr, 0, sizeof(ModelHeader));
		memset(&m_hdr2, 0, sizeof(ModelHeader2));
		Load(name);
	}

	void ModelFile::Load(const char * name)
	{
		FILE * fp = fopen(name, "rb");
		if(!fp)
		{
			printf("Err: Unable to open \"%s\"\n", name);
		}

		fread(&m_hdr, sizeof(ModelHeader), 1, fp);

		if(m_hdr.Magick != SX_MODEL_MAGICK)
		{
			printf("Err: Corrupt model \"%s\"\n", name);
			fclose(fp);
			return;
		}

		if(m_hdr.iVersion != SX_MODEL_VERSION)
		{
			printf("Err: Invalid version %d file \"%s\"\n", m_hdr.iVersion, name);
			fclose(fp);
			return;
		}

		if(m_hdr.iSecondHeaderOffset)
		{
			fseek(fp, m_hdr.iSecondHeaderOffset, SEEK_SET);
			fread(&m_hdr2, sizeof(ModelHeader2), 1, fp);
		}

		if(m_hdr2.iDepsCount && m_hdr2.iDependensiesOffset)
		{
			fseek(fp, m_hdr2.iDependensiesOffset, SEEK_SET);

			ModelDependensy md;

			m_pDeps = new const ModelFile*[m_hdr2.iDepsCount];

			for(int i = 0; i < m_hdr2.iDepsCount; i++)
			{
				fread(&md, sizeof(ModelDependensy), 1, fp);
				m_pDeps[i] = AnimationManager::LoadModel(md.szName);
			}
		}

		if(m_hdr2.iBoneTableOffset)
		{
			fseek(fp, m_hdr2.iBoneTableOffset, SEEK_SET);
			m_pBones = new ModelBoneName[m_hdr2.iBoneTableCount];
			fread(m_pBones, sizeof(ModelBoneName), m_hdr2.iBoneTableCount, fp);
		}

		if(m_hdr2.iActivitiesTableCount && m_hdr2.iActivitiesTableOffset)
		{
			fseek(fp, m_hdr2.iActivitiesTableOffset, SEEK_SET);
			pActivities = new ModelActivity[m_hdr2.iActivitiesTableCount];
			fread(pActivities, sizeof(ModelActivity), m_hdr2.iActivitiesTableCount, fp);
		}

		if(m_hdr2.iControllersCount && m_hdr2.iControllersOffset)
		{
			fseek(fp, m_hdr2.iControllersOffset, SEEK_SET);
			m_pControllers = new ModelBoneController[m_hdr2.iControllersCount];
			fread(m_pControllers, sizeof(ModelBoneController), m_hdr2.iControllersCount, fp);
		}

		if(m_hdr.iMaterialsOffset)
		{
			fseek(fp, m_hdr.iMaterialsOffset, SEEK_SET);

			m_iMaterials = new ModelMatrial*[m_hdr.iSkinCount];

			for(UINT j = 0; j < m_hdr.iSkinCount; j++)
			{
				m_iMaterials[j] = new ModelMatrial[m_hdr.iMaterialCount];
				for(int i = 0; i < m_hdr.iMaterialCount; i++)
				{
					fread(m_iMaterials[j][i].szName, 1, MODEL_MAX_NAME, fp);
					
					m_iMaterials[j][i].iMat = SkyXEngine::Core::Data::MaterialsManager->Load((String(m_iMaterials[j][i].szName)+".dds").c_str(), 3); //3
				}
			}
		}
		

		if(m_hdr.iAnimationsOffset)
		{
			fseek(fp, m_hdr.iAnimationsOffset, SEEK_SET);
			m_pSequences = new ModelSequence[m_hdr.iAnimationsOffset];

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
			fseek(fp, m_hdr.iLODoffset, SEEK_SET);

			m_pLods = new ModelLoD[m_hdr.iLODcount];

			for(int i = 0; i < m_hdr.iLODcount; i++)
			{
				fread(&m_pLods[i], MODEL_LOD_STRUCT_SIZE, 1, fp);
				int iVC = 0;
				m_pLods[i].pSubLODmeshes = new ModelLoDSubset[m_pLods[i].iSubMeshCount];
				for(int j = 0; j < m_pLods[i].iSubMeshCount; j++)
				{
					
					fread(&m_pLods[i].pSubLODmeshes[j].iMaterialID, sizeof(int), 1, fp);
					fread(&m_pLods[i].pSubLODmeshes[j].iVectexCount, sizeof(int), 1, fp);
					fread(&m_pLods[i].pSubLODmeshes[j].iIndexCount, sizeof(int), 1, fp);
					m_pLods[i].pSubLODmeshes[j].pVertices = new vertex_animated[m_pLods[i].pSubLODmeshes[j].iVectexCount];
					m_pLods[i].pSubLODmeshes[j].pIndices = new UINT[m_pLods[i].pSubLODmeshes[j].iIndexCount];
					fread(m_pLods[i].pSubLODmeshes[j].pVertices, sizeof(vertex_animated), m_pLods[i].pSubLODmeshes[j].iVectexCount, fp);
					fread(m_pLods[i].pSubLODmeshes[j].pIndices, sizeof(UINT), m_pLods[i].pSubLODmeshes[j].iIndexCount, fp);

					iVC += m_pLods[i].pSubLODmeshes[j].iVectexCount;
				}
			}
		}

		fclose(fp);
		m_bLoaded = true;
	}

	UINT ModelFile::GetBoneID(const String & name)
	{
			for(int i = 0; i < m_hdr2.iBoneTableCount; i++)
			{
					if(stricmp(m_pBones[i].szName,name.c_str()) == 0)
					{
						return(m_pBones[i].bone.id);
					}
			}
		return(-1);
	}

	void ModelFile::Assembly()
	{
		//Assumed all dependensies already assembled

		for(int i = 0; i < m_hdr2.iDepsCount; i++)
		{
			MergeModel(m_pDeps[i]);
		}
	}

	void ModelFile::MergeModel(const ModelFile * mdl)
	{
		//merge bones
		ModelBoneList local;
		ModelBoneList remote;
		ModelBoneRelinkList relink;
		ModelBoneRelinkList relink_activities;
		ModelBoneRelinkList relink_materials;
		ModelBoneChildrenList local_children;
		ModelBoneChildrenList remote_children;
		ModelBoneIdList id_local;
		ModelBoneIdList id_remote;
		ModelBones result;
		//AssotiativeArray<String, ModelBone> new_list;
		bool bLocalRoot = true;
		bool bDualRoot = false;
		for(int i = 0; i < m_hdr.iBoneCount; i++)
		{
			local[m_pBones[i].szName] = &m_pBones[i];
			local_children[m_pBones[i].bone.pid].push_back(m_pBones[i].bone.id);
			id_local[m_pBones[i].bone.id] = &m_pBones[i];
		}
		for(int i = 0; i < mdl->m_hdr.iBoneCount; i++)
		{
			remote[mdl->m_pBones[i].szName] = &mdl->m_pBones[i];
			remote_children[mdl->m_pBones[i].bone.pid].push_back(mdl->m_pBones[i].bone.id);
			id_remote[mdl->m_pBones[i].bone.id] = &mdl->m_pBones[i];
			if(mdl->m_pBones[i].bone.pid == -1 && local.KeyExists(mdl->m_pBones[i].szName))
			{
				bLocalRoot = true;
				bDualRoot = false;
			}
		}
		if(!bLocalRoot)
		{
			for(int i = 0; i < m_hdr.iBoneCount; i++)
			{
				if(m_pBones[i].bone.pid == -1 && remote.KeyExists(m_pBones[i].szName))
				{
					bDualRoot = false;
				}
			}
		}
		if(bDualRoot)
		{
			printf("Err: Invalid model bones hierarchy\n");
			return;
		}
		MergeActivities(mdl, relink_activities);
		if(bLocalRoot)
		{
			//remote relink
			MergeBones(remote_children, local, id_remote, relink, result);
			ImportControllers(mdl, &relink);
			ImportSequences(mdl, &relink, relink_activities);
		}
		else
		{
			//local relink
			MergeBones(local_children, remote, id_local, relink, result);
			for(int i = 0; i < m_hdr2.iControllersCount; i++)
			{
				for(int j = 0; j < m_pControllers[i].iBoneCount; j++)
				{
					if(relink.KeyExists(m_pControllers[i].bones[j]))
					{
						m_pControllers[i].bones[j] = relink[m_pControllers[i].bones[j]];
					}
				}
			}
			for(int i = 0; i < m_hdr.iAnimationCount; i++)
			{
				for(int f = 0; f < m_pSequences[i].iNumFrames; f++)
				{
					for(int j = 0; j < m_hdr.iBoneCount; j++)
					{
						if(relink.KeyExists(m_pSequences[i].m_vmAnimData[f][j].id))
						{
							m_pSequences[i].m_vmAnimData[f][j].id = relink[m_pSequences[i].m_vmAnimData[f][j].id];
						}
						if(relink.KeyExists(m_pSequences[i].m_vmAnimData[f][j].pid))
						{
							m_pSequences[i].m_vmAnimData[f][j].pid = relink[m_pSequences[i].m_vmAnimData[f][j].id];
						}
					}
				}
			}
			ImportControllers(mdl, NULL);
			ImportSequences(mdl, NULL, relink_activities);
		}

		//relink:
		//m_pControllers
		//m_pSequences

		SX_SAFE_DELETE_A(m_pBones);
		m_hdr2.iBoneTableCount = m_hdr.iBoneCount = result.Size();
		m_pBones = new ModelBoneName[result.Size()];
		for(ModelBones::Iterator i = result.begin(); i; i++)
		{
			m_pBones[i.second->bone.id] = *(i.second);
		}

//		for(ModelBoneList::Iterator i = local

		//merge texture lists

		MergeMaterials(mdl, relink_materials);

		//merge lods
		MergeLods(mdl, relink_materials);
	}

	void ModelFile::MergeBones(
		ModelBoneChildrenList & child_remote,
		ModelBoneList & local,
		ModelBoneIdList & id_remote,
		ModelBoneRelinkList & relink_remote,
		ModelBones & res
		)
	{
		for(ModelBoneList::Iterator i = local.begin(); i; i++)
		{
			res[*(i.first)] = **(i.second);
		}
		//Init base struct
		DoMerge(0, child_remote, id_remote, relink_remote, res);
	}

	void ModelFile::DoMerge(
		int startId,
		ModelBoneChildrenList & child_remote,
		ModelBoneIdList & id_remote,
		ModelBoneRelinkList & relink_remote,
		ModelBones & res
		)
	{
		for(int i = 0; i < child_remote[startId].size(); i++)
		{
			//ModelBoneName out = *id_local[child_local[startId][i]];
			if(res.KeyExists(id_remote[child_remote[startId][i]]->szName))
			{
				ModelBoneName * rem_bone = &res[id_remote[child_remote[startId][i]]->szName];
				relink_remote[child_remote[startId][i]] = rem_bone->bone.id;
				relink_remote[id_remote[child_remote[startId][i]]->bone.pid] = rem_bone->bone.pid;
			}
			else
			{
				res[id_remote[child_remote[startId][i]]->szName] = *id_remote[child_remote[startId][i]];
				ModelBoneName * rem_bone = &res[id_remote[child_remote[startId][i]]->szName];
				rem_bone->bone.pid = relink_remote[rem_bone->bone.pid];
				int newId = res.Size();
				relink_remote[rem_bone->bone.id] = newId;
				rem_bone->bone.id = newId;
			}

			DoMerge(child_remote[startId][i], child_remote, id_remote, relink_remote, res);
		}
	}

	void ModelFile::MergeActivities(const ModelFile * mdl, ModelBoneRelinkList & relink)
	{
		if(mdl->m_hdr2.iActivitiesTableCount)
		{
			AssotiativeArray<String, int> list;
			Array<ModelActivity> data;
			if(m_hdr2.iActivitiesTableCount)
			{
				for(int i = 0; i < m_hdr2.iActivitiesTableCount; i++)
				{
					list[pActivities[i].szName] = i; // save original IDs
					data.push_back(pActivities[i]);
				}
			}
			for(int i = 0; i < mdl->m_hdr2.iActivitiesTableCount; i++)
			{
				if(list.KeyExists(mdl->pActivities[i].szName))
				{
					relink[i] = list[mdl->pActivities[i].szName];
				}
				else
				{
					data.push_back(mdl->pActivities[i]);
					relink[i] = data.size() - 1;
				}
			}
			m_hdr2.iActivitiesTableCount = data.size();
			SX_SAFE_DELETE_A(pActivities);
			pActivities = new ModelActivity[m_hdr2.iActivitiesTableCount];
			memcpy(pActivities, &data[0], sizeof(ModelActivity) * m_hdr2.iActivitiesTableCount);
		}
	}

	void ModelFile::ImportControllers(const ModelFile * mdl, ModelBoneRelinkList * relink)
	{
		if(mdl->m_hdr2.iControllersCount)
		{
			Array<ModelBoneController> list;
			if(m_hdr2.iControllersCount)
			{
				list.resize(m_hdr2.iControllersCount);
				memcpy(&list[0], m_pControllers, sizeof(ModelBoneController) * m_hdr2.iControllersCount);
			}
			for(int i = 0; i < mdl->m_hdr2.iControllersCount; i++)
			{
				list.push_back(mdl->m_pControllers[i]);
				if(relink)
				{
					ModelBoneRelinkList & rlist = *relink;
					ModelBoneController & ctrl = list[list.size() - 1];
					for(int j = 0; j < ctrl.iBoneCount; j++)
					{
						if(rlist.KeyExists(ctrl.bones[j]))
						{
							ctrl.bones[j] = rlist[ctrl.bones[j]];
						}
					}
				}
			}
			m_hdr2.iControllersCount = list.size();
			SX_SAFE_DELETE_A(m_pControllers);
			m_pControllers = new ModelBoneController[m_hdr2.iControllersCount];
			memcpy(m_pControllers, &list[0], sizeof(ModelBoneController) * m_hdr2.iControllersCount);
		}
	}
	void ModelFile::ImportSequences(const ModelFile * mdl, ModelBoneRelinkList * relink, ModelBoneRelinkList & relink_activities)
	{
		if(mdl->m_hdr.iAnimationCount)
		{
			Array<ModelSequence> list;
			if(m_hdr.iAnimationCount)
			{
				list.resize(m_hdr.iAnimationCount);
				memcpy(&list[0], m_pSequences, sizeof(ModelSequence) * m_hdr.iAnimationCount);
			}

			for(int i = 0; i < mdl->m_hdr.iAnimationCount; i++)
			{
				list.push_back(mdl->m_pSequences[i]);
				ModelSequence & seq = list[list.size() - 1];

				seq.m_vmAnimData = new ModelBone*[seq.iNumFrames];

				for(UINT j = 0; j < seq.iNumFrames; j++)
				{
					seq.m_vmAnimData[j] = new ModelBone[m_hdr.iBoneCount];
					memcpy(&seq.m_vmAnimData[j], &mdl->m_pSequences[i].m_vmAnimData[j], sizeof(ModelBone) * m_hdr.iBoneCount);

					if(relink)
					{
						ModelBoneRelinkList & rlist = *relink;
						for(int k = 0; k < m_hdr.iBoneCount; k++)
						{
							if(rlist.KeyExists(seq.m_vmAnimData[j][k].id))
							{
								seq.m_vmAnimData[j][k].id = rlist[seq.m_vmAnimData[j][k].id];
							}
							if(rlist.KeyExists(seq.m_vmAnimData[j][k].pid))
							{
								seq.m_vmAnimData[j][k].pid = rlist[seq.m_vmAnimData[j][k].pid];
							}
						}
					}
				}
			}

			m_hdr.iAnimationCount = list.size();
			SX_SAFE_DELETE_A(m_pSequences);
			m_pSequences = new ModelSequence[m_hdr.iAnimationCount];
			memcpy(m_pSequences, &list[0], sizeof(ModelSequence) * m_hdr.iAnimationCount);
		}
	}

	void ModelFile::MergeMaterials(const ModelFile * mdl, ModelBoneRelinkList & relink)
	{
		if(mdl->m_hdr.iMaterialCount)
		{
			Array<Array<ModelMatrial>> _data;
			for(int i = 0; i < max(mdl->m_hdr.iSkinCount, m_hdr.iSkinCount); i++)
			{
				UINT iLocalSkin = i >= m_hdr.iSkinCount ? 0 : i;
				UINT iRemoteSkin = i >= mdl->m_hdr.iSkinCount ? 0 : i;

				Array<ModelMatrial> data;
				for(int j = 0; j < m_hdr.iMaterialCount; j++)
				{
					data.push_back(m_iMaterials[iLocalSkin][j]);
				}
				for(int j = 0; j < mdl->m_hdr.iMaterialCount; j++)
				{
					data.push_back(mdl->m_iMaterials[iRemoteSkin][j]);
					if(i == 0)
					{
						relink[i] = data.size() - 1;
					}
				}
				_data.push_back(data);
			}

			for(int i = 0; i < m_hdr.iSkinCount; i++)
			{
				SX_SAFE_DELETE_A(m_iMaterials[i]);
			}
			SX_SAFE_DELETE_A(m_iMaterials);
			m_hdr.iSkinCount = _data.size();
			m_iMaterials = new ModelMatrial*[m_hdr.iSkinCount];
			m_hdr.iMaterialCount = _data[0].size();
			for(int i = 0; i < m_hdr.iSkinCount; i++)
			{
				m_iMaterials[i] = new ModelMatrial[m_hdr.iMaterialCount];
				memcpy(m_iMaterials[i], &_data[i][0], sizeof(ModelMatrial) * m_hdr.iMaterialCount);
			}
		}
	}

	void ModelFile::MergeLods(const ModelFile * mdl, ModelBoneRelinkList & relink)
	{
		if(mdl->m_hdr.iLODcount)
		{
			Array<Array<ModelLoDSubset>> _list;

			for(int i = 0; i < max(m_hdr.iLODcount, mdl->m_hdr.iLODcount); i++)
			{
				Array<ModelLoDSubset> list;
				bool bHasLocalLod = i < m_hdr.iLODcount;
				bool bHasRemoteLod = i < mdl->m_hdr.iLODcount;
				if(bHasLocalLod)
				{
					for(int j = 0; j < m_pLods[i].iSubMeshCount; j++)
					{
						list.push_back(m_pLods[i].pSubLODmeshes[j]);
					}
				}
				if(bHasRemoteLod)
				{
					for(int j = 0; j < mdl->m_pLods[i].iSubMeshCount; j++)
					{
						list.push_back(mdl->m_pLods[i].pSubLODmeshes[j]);
						ModelLoDSubset & lss = list[list.size() - 1];
						lss.pIndices = new UINT[lss.iIndexCount];
						lss.pVertices = new vertex_animated[lss.iVectexCount];
						memcpy(lss.pIndices, mdl->m_pLods[i].pSubLODmeshes[j].pIndices, sizeof(UINT) * lss.iIndexCount);
						memcpy(lss.pVertices, mdl->m_pLods[i].pSubLODmeshes[j].pVertices, sizeof(vertex_animated) * lss.iVectexCount);
						if(relink.KeyExists(lss.iMaterialID))
						{
							lss.iMaterialID = relink[lss.iMaterialID];
						}
					}
				}
				_list.push_back(list);
			}

			for(int i = 0; i < m_hdr.iLODcount; i++)
			{
				SX_SAFE_DELETE_A(m_pLods[i].pSubLODmeshes);
			}
			SX_SAFE_DELETE_A(m_pLods);
			m_hdr.iLODcount = _list.size();
			m_pLods = new ModelLoD[m_hdr.iLODcount];
			for(int i = 0; i < m_hdr.iLODcount; i++)
			{
				m_pLods[i].iSubMeshCount = _list[i].size();
				m_pLods[i].pSubLODmeshes = new ModelLoDSubset[m_pLods[i].iSubMeshCount];
				memcpy(m_pLods[i].pSubLODmeshes, &_list[i][0], sizeof(ModelLoDSubset) * m_pLods[i].iSubMeshCount);
			}
		}
	}

	const ModelSequence * ModelFile::GetSequence(UINT id) const
	{
		if(GetSequenceCount() <= id)
		{
			return(NULL);
		}
		return(&m_pSequences[id]);
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

	void ModelFile::Render(int howrender,int render_forward,SMMATRIX * mWorld, UINT nSkin, UINT nLod)
	{
		if(nSkin >= m_hdr.iSkinCount)
		{
			nSkin = 0;
		}

		if(nLod >= m_hdr.iLODcount)
		{
			return;
		}

		if(howrender == 0)
		{

		}
		else if(howrender == 2)
		{
			D3DXMATRIX tmpviewproj;
			Core::Data::Device->GetTransform(D3DTS_WORLD1,&tmpviewproj);

			float4x4 wvp = *mWorld * float4x4(tmpviewproj);
			wvp = SMMatrixTranspose(wvp);
			
			Core::Data::ShadersManager->SetValueRegisterF(0,"shadowcreatedepthskin","WorldViewProjection",&wvp);
			Core::Data::ShadersManager->SetValueRegisterF(0,"shadowcreatedepthskin","World",&SMMatrixTranspose(*mWorld));
			
			Core::Data::ShadersManager->Bind(0,"shadowcreatedepthskin");
		}
		else if(howrender == 3)
		{

		}

		UINT iVertSize[] = {
			sizeof(vertex_static),
			sizeof(vertex_static_ex),
			sizeof(vertex_animated),
			sizeof(vertex_animated_ex)
		};

		Core::Data::Device->SetIndices(m_ppIndexBuffer[nLod]);
		Core::Data::Device->SetStreamSource(0, m_ppVertexBuffer[nLod], 0, iVertSize[MVT_DYNAMIC]);

		AnimationManager::SetVertexDeclaration(MVT_DYNAMIC);

		for(UINT i = 0; i < m_pLods[nLod].iSubMeshCount; i++)
		{
				if(
					((howrender == 2 || howrender == 3) || (//если подгруппа освещаема и мы рисуем освещаемую геометрию, либо наоборот
						render_forward == 2 ||
						(render_forward == 0 && !(SkyXEngine::Core::Data::MaterialsManager->IsForward(m_iMaterials[nSkin][m_pLods[nLod].pSubLODmeshes[i].iMaterialID].iMat))) ||
						(render_forward == 1 && (SkyXEngine::Core::Data::MaterialsManager->IsForward(m_iMaterials[nSkin][m_pLods[nLod].pSubLODmeshes[i].iMaterialID].iMat)))
					)) &&
					(SkyXEngine::Core::Data::Settings::IsComEditors && //если обработка редакторная
						(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 0 ||	//если рисуем все подгруппы
							(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 1 &&	//если рисуем выделенную подгруппу 
							i == SkyXEngine::Core::Data::Settings::EditorsSelectedSubset) ||	//и если текущая подгруппа есть выделенная
						(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 2 &&	//если рисуем все кроме выделенной подгруппы 
						i != SkyXEngine::Core::Data::Settings::EditorsSelectedSubset)) ||	//и если текущая подгруппа не есть выделенная
					!SkyXEngine::Core::Data::Settings::IsComEditors)	//если обработка не редакторная
					)
				{
						if(howrender == 1)
							SkyXEngine::Core::Data::MaterialsManager->Render(m_iMaterials[nSkin][m_pLods[nLod].pSubLODmeshes[i].iMaterialID].iMat,mWorld,0);
						else
							SkyXEngine::Core::Data::MaterialsManager->SetMainTexture(0,m_iMaterials[nSkin][m_pLods[nLod].pSubLODmeshes[i].iMaterialID].iMat);

					Core::Data::Device->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						m_pLods[nLod].pSubLODmeshes[i].iStartVertex,
						0,
						m_pLods[nLod].pSubLODmeshes[i].iVectexCount,
						m_pLods[nLod].pSubLODmeshes[i].iStartIndex,
						m_pLods[nLod].pSubLODmeshes[i].iIndexCount / 3);
				}
		}
	}

	void ModelFile::BuildMeshBuffers()
	{
		UINT m_iLodCount = m_hdr.iLODcount;
		if(m_ppIndexBuffer && m_ppVertexBuffer)
		{
			for(UINT j = 0; j < m_iLodCount; j++)
			{
				SX_SAFE_RELEASE(m_ppIndexBuffer[j]);
				SX_SAFE_RELEASE(m_ppVertexBuffer[j]);
			}
		}
		SX_SAFE_DELETE_A(m_ppIndexBuffer);
		SX_SAFE_DELETE_A(m_ppVertexBuffer);

		m_ppIndexBuffer = new IDirect3DIndexBuffer9 *[m_iLodCount];
		m_ppVertexBuffer = new IDirect3DVertexBuffer9 *[m_iLodCount];

		DWORD tmpCountVert = 0;
		for(UINT j = 0; j < m_iLodCount; j++)
		{
			UINT iStartIndex = 0;
			UINT iStartVertex = 0;
			for(int i = 0; i < m_pLods[j].iSubMeshCount; i++)
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
			for(int i = 0; i < m_pLods[j].iSubMeshCount; i++)
			{
				pSM = &m_pLods[j].pSubLODmeshes[i];
				memcpy(pIndices + m_pLods[j].pSubLODmeshes[i].iStartIndex, m_pLods[j].pSubLODmeshes[i].pIndices, sizeof(UINT) * m_pLods[j].pSubLODmeshes[i].iIndexCount);
				memcpy(pVertices + m_pLods[j].pSubLODmeshes[i].iStartVertex, m_pLods[j].pSubLODmeshes[i].pVertices, sizeof(vertex_animated) * m_pLods[j].pSubLODmeshes[i].iVectexCount);
			}

			Core::Data::Device->CreateVertexBuffer(sizeof(vertex_animated) * iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &m_ppVertexBuffer[j], 0);
			VOID * pData;
			if(!FAILED(m_ppVertexBuffer[j]->Lock(0, sizeof(vertex_animated) * iStartVertex, (void**)&pData, 0)))
			{
				memcpy(pData, pVertices, sizeof(vertex_animated) * iStartVertex);
				m_ppVertexBuffer[j]->Unlock();
			}

			Core::Data::Device->CreateIndexBuffer(sizeof(UINT) * iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_ppIndexBuffer[j], 0);
			if(!FAILED(m_ppIndexBuffer[j]->Lock(0, sizeof(UINT) * iStartIndex, (void**)&pData, 0)))
			{
				memcpy(pData, pIndices, sizeof(UINT) * iStartIndex);
				m_ppIndexBuffer[j]->Unlock();
			}
			SX_SAFE_DELETE_A(pVertices);
			SX_SAFE_DELETE_A(pIndices);
		}

		Core::WorkModel::ComputeBoundingBox2(m_ppVertexBuffer[0],&BoundVol,tmpCountVert,0);
	}

	

/**
 *
 * Animation implementation
 *
 */

	Animation::Animation():
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
		m_mfBoneControllerValues(NULL),
		m_iCurrentSkin(0)
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
			t[i] = 0;
		}

		myId = AnimationManager::Register(this);
	}

	Animation::~Animation()
	{
		SX_SAFE_DELETE_A(m_pBoneControllers);
		SX_SAFE_DELETE_A(m_mfBoneControllerValues);
		SX_SAFE_DELETE_A(m_pBoneMatrix);
		SX_SAFE_DELETE_A(m_FinalBones);
		AnimationManager::UnRegister(myId);
	}

	SMMATRIX Animation::GetBoneTransform(UINT id)
	{
		//id *= 2;
		float3 pos = m_pBoneMatrix[id].position;
		SMQuaternion q = m_pBoneMatrix[id].orient;
		return(q.GetMatrix() * SMMatrixTranslation(pos));
	}

	bool Animation::IsVisibleFrustum(Core::ControllMoving::Frustum* frustum)
	{
		m_pMdl->BoundVol.GetSphere(&jcenter2,&jradius);
		jcenter = SMVector3Transform(jcenter2,GetWorldTM());

			if((frustum->SphereInFrustum(&jcenter,jradius)))
				return true;
			else
				return false;
	}

	void Animation::SetModel(const char * file)
	{
		m_pMdl = const_cast<ModelFile*>(AnimationManager::LoadModel(file));
		if(!m_pMdl)
		{
			return;
		}

		m_iBoneCount = m_pMdl->GetBoneCount();

		int seqCount = m_pMdl->GetSequenceCount();
		for(int i = 0; i < seqCount; i++)
		{
			const ModelSequence * pSeq = m_pMdl->GetSequence(i);
			m_mSeqIds[pSeq->name] = i;
		}

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

	void Animation::Advance(unsigned long int dt)
	{
		UINT deltat = dt - m_iCurTime;
		m_iCurTime = dt;
		UINT deltat_orig = deltat;
		
		for(int slot = 0; slot < BLEND_MAX; slot++)
		{
			deltat = deltat_orig;
			if(m_bInFade[slot])
			{
				if(m_iFadeCurTime[slot] == 0)
				{
					for(UINT i = 0; i < m_iBoneCount; i++)
					{
						m_LastFrameBones[slot][i] = m_CurrentBones[slot][i];
					}
				}

				m_iFadeCurTime[slot] += deltat;

				const ModelSequence * pCurAnim = m_pMdl->GetSequence(m_iPlayingAnim[slot]);

				float delta = (float)m_iFadeCurTime[slot] / (float)m_iFadeTime[slot];
				for(UINT i = 0; i < m_iBoneCount; i++)
				{
					m_CurrentBones[slot][i] = m_LastFrameBones[slot][i];
					m_CurrentBones[slot][i].orient = SMquaternionSlerp(m_LastFrameBones[slot][i].orient, pCurAnim->m_vmAnimData[0][i].orient * m_pBoneControllers[i].rot, delta).Normalize();
					m_CurrentBones[slot][i].position = (float3_t)(m_LastFrameBones[slot][i].position + (pCurAnim->m_vmAnimData[0][i].position + m_pBoneControllers[i].tr - m_LastFrameBones[slot][i].position) * delta);
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
					continue;
				}
				// DSconsole::write("SXbaseAnimating::Advance(%d);\n", deltat);

				const ModelSequence * pCurAnim = m_pMdl->GetSequence(m_iPlayingAnim[slot]);
				//static UINT t[BLEND_MAX] = {0};
				if(m_bNewAnimPlayed[slot])
				{
					t[slot] = 0;
					m_bNewAnimPlayed[slot] = false;
				}
				UINT fr = 1;
				if(pCurAnim->framerate)
				{
					t[slot] += deltat;
					fr = 1000 / pCurAnim->framerate;
					bool cont = false;
					while(t[slot] >= fr)
					{
						m_iCurrentFrame[slot]++;
						if(!pCurAnim->bLooped && m_iCurrentFrame[slot] >= m_iAnimFrameCount[slot] - 1)
						{
							m_bIsAnimationPlaying[slot] = false;
							//StopAnimations();
							cont = true;
							break;
						}
						t[slot] -= fr;
					}
					if(cont)
					{
						continue;
					}
					m_iCurrentFrame[slot] %= m_iAnimFrameCount[slot];
				}

				//printf("=%d\n", m_iCurrentFrame[slot]);
				UINT nextFrame = m_iCurrentFrame[slot] + 1;
				nextFrame %= m_iAnimFrameCount[slot];

				float delta = (float)t[slot] / (float)fr;
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
			for(int i=0;i<BLEND_MAX;i++)
			{
					if(m_bIsAnimationPlaying[i])
						return true;
			}
		return false;
	}

	void Animation::StopAnimations()
	{
		memset(&m_bIsAnimationPlaying, 0, sizeof(m_bIsAnimationPlaying));
	}

	void Animation::FillBoneMatrix()
	{
		//AssotiativeArray<int, SXmodel::boneID> * mBonesOrig = m_pMdl->GetBindPoseOrig();
		//const AssotiativeArray<int, SXmodel::boneID> * mBonesBind = m_pMdl->GetBindPose();

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
			for(int i=0;i<BLEND_MAX;i++)
			{
					if(m_bIsAnimationPlaying[i] && m_mSeqIds[name] == m_iPlayingAnim[i])
						return true;
			}
		return false;
	}

	void Animation::PlayAnimation(const char * name, UINT iFadeTime, UINT slot) // name: Animation name; changeTime: time to fade to this animation from previous
	{
		if(!m_pMdl)
		{
			return;
		}
		if(slot >= BLEND_MAX)
		{
			SkyXEngine::Core::InLog("Unable to play animation \"%s\" Invalid slot %d, max valid slot is %d\n", name, slot, BLEND_MAX - 1);
			return;
		}
		if(!m_mSeqIds.KeyExists(name))
		{
			SkyXEngine::Core::InLog("Unable to play animation \"%s\"\n", name);
			return;
		}
		UINT sid = m_mSeqIds[name];
		m_iPlayingAnim[slot] = sid;

		m_bIsAnimationPlaying[slot] = true;
		m_iAnimFrameCount[slot] = m_pMdl->GetSequence(sid)->iNumFrames;
		m_iCurrentFrame[slot] = 0;
		m_bNewAnimPlayed[slot] = true;

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

	void Animation::StartActivity(const String & name, UINT iFadeTime)
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
		return(m_vOrientation.GetMatrix() * SMMatrixTranslation(m_vPosition));
	}

	void Animation::SetOrient(const SMQuaternion & pos)
	{
		m_vOrientation = pos;
	}

	SMQuaternion Animation::GetOrient()
	{
		return(m_vOrientation);
	}

	void Animation::Render(int howrender,bool render_forward)
	{
			if(m_bBoneMatrixReFilled)
			{
				m_bBoneMatrixReFilled = false;
			}

		SkyXEngine::Core::Data::Device->SetVertexShaderConstantF(16, (float*)m_pBoneMatrix, sizeof(ModelBoneShader) * m_iBoneCount / sizeof(float) / 4);

		m_pMdl->Render(howrender,render_forward,&(SMMatrixScaling(0.01, 0.01, 0.01) * GetWorldTM()), m_iCurrentSkin);
	}

	bool Animation::IsVisible()
	{
		return(m_bVisible);
	}
	void Animation::SetVisible(bool bdo)
	{
		m_bVisible = bdo;
	}

	bool Animation::IsRenderForShadow()
	{
		return(m_bRenderForShadow);
	}
	void Animation::SetRenderForShadow(bool bdo)
	{
		m_bRenderForShadow = bdo;
	}


/**
 * 
 * AnimationManager implementation
 * 
 */

	const ModelFile * AnimationManager::LoadModel(const char * name)
	{
		if(m_pModels.KeyExists(name))
		{
			return(m_pModels[name]);
		}
		else
		{
			ModelFile * mdl = new ModelFile(name);
			mdl->Assembly();
			mdl->BuildMeshBuffers();
			m_pModels[name] = mdl;
			return(mdl);
		}
	}

	AssotiativeArray<String, ModelFile*> AnimationManager::m_pModels;
	IDirect3DVertexDeclaration9 * AnimationManager::pVertexDeclaration[MVT_SIZE];

	void AnimationManager::InitVertexDeclarations()
	{
		D3DVERTEXELEMENT9 layoutStatic[] =
		{
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
			D3DDECL_END()
		};
		SkyXEngine::Core::Data::Device->CreateVertexDeclaration(layoutStatic, &pVertexDeclaration[MVT_STATIC]);

		D3DVERTEXELEMENT9 layoutStaticEx[] =
		{
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
			{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
			{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
			D3DDECL_END()
		};
		SkyXEngine::Core::Data::Device->CreateVertexDeclaration(layoutStaticEx, &pVertexDeclaration[MVT_STATIC_EX]);

		D3DVERTEXELEMENT9 layoutDynamic[] =
		{
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
			{0, 32, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
			{0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
			D3DDECL_END()
		};
		SkyXEngine::Core::Data::Device->CreateVertexDeclaration(layoutDynamic, &pVertexDeclaration[MVT_DYNAMIC]);

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
		SkyXEngine::Core::Data::Device->CreateVertexDeclaration(layoutDynamicEx, &pVertexDeclaration[MVT_DYNAMIC_EX]);
	}

	Array<Animation*> AnimationManager::m_pAnimatedList;
	Array<bool> AnimationManager::ArrIsVisible;

	void AnimationManager::Init()
	{
		InitVertexDeclarations();
	}

	void AnimationManager::SetVertexDeclaration(MODEL_VERTEX_TYPE nDecl)
	{
		if(nDecl >= MVT_SIZE)
		{
			printf("Unknown vertex declaration %d in AnimationManager::SetVertexDeclaration()\n", nDecl);
			return;
		}
		SkyXEngine::Core::Data::Device->SetVertexDeclaration(pVertexDeclaration[nDecl]);
	}

	UINT AnimationManager::Register(Animation * pAnim)
	{
		m_pAnimatedList.push_back(pAnim);
		ArrIsVisible.push_back(true);
		return(m_pAnimatedList.size() - 1);
	}
	void AnimationManager::UnRegister(UINT id)
	{
		m_pAnimatedList.erase(id);
	}
	void AnimationManager::Render(int howrender,int render_forward,Core::ControllMoving::Frustum* frustum)
	{
		Animation * pAnim;
			for(int i = 0; i < m_pAnimatedList.size(); i++)
			{
				pAnim = m_pAnimatedList[i];
					if(pAnim->IsVisible() && (!(howrender == 2 || howrender == 3) || pAnim->IsRenderForShadow()))
					{
							if((howrender == 0 || howrender == 1) && ArrIsVisible[i])
								pAnim->Render(howrender,render_forward);
							else if((howrender == 2 || howrender == 3) && frustum)
							{
									if(pAnim->IsVisibleFrustum(frustum))
										pAnim->Render(howrender,render_forward);
							}
					}
			}
	}
	void AnimationManager::Update()
	{
			for(int i = 0; i < m_pAnimatedList.size(); i++)
			{
					if(m_pAnimatedList[i]->IsVisible())
						m_pAnimatedList[i]->Advance(timeGetTime());
			}
	}

	void AnimationManager::ComVisible()
	{
		float4x4 mat = Core::Data::View * Core::Data::ProjectionStencil;
		float3 sizemapdepth = float3(Core::Data::WidthHeight.x * Core::Data::CoefSizeOC,Core::Data::WidthHeight.y * Core::Data::CoefSizeOC,Core::Data::NearFar.y);
		float tmpdepth;
		float3 center,center2;
		float radius;

		int tmpkey;
		DWORD sizewarrdepth = Core::Data::WidthHeight.x * Core::Data::CoefSizeOC;
		DWORD sizeharrdepth = Core::Data::WidthHeight.y * Core::Data::CoefSizeOC;
		PosBBScreen res;

			for(DWORD i=0;i<m_pAnimatedList.size();i++)
			{
				ArrIsVisible[i] = false;
					//если модель видна камере
					if(m_pAnimatedList[i]->IsVisibleFrustum(Core::Data::ObjCamera->ObjFrustum))
					{
						
						m_pAnimatedList[i]->m_pMdl->BoundVol.GetPosBBScreen(&res,&Core::Data::ConstCurrentCameraPosition,&sizemapdepth,&(m_pAnimatedList[i]->GetWorldTM() * mat));
						
							if(Core::Data::Settings::IsUpdateCamera)
								ArrIsVisible[i] = false;
							
						tmpdepth = Core::Data::CurrentMaxDepth2;
							//если минимальная глубина модели меньше ем максимальная глубина сцены
							if(tmpdepth > res.mindepth*Core::Data::NearFar.y)
							{
								m_pAnimatedList[i]->m_pMdl->BoundVol.GetSphere(&center2,&radius);
								center = SMVector3Transform(center2,m_pAnimatedList[i]->GetWorldTM());
								//определяем расстояние до камеры
								//Arr[i]->ArrSegments->DistForCamera = SMVector3Length((center - Core::Data::ConstCurrentCameraPosition)) - radius;;//res.mindepth*Core::Data::NearFar.y;

									/*if(Arr[i]->ArrSegments->DistForCamera < 0.f)
										Arr[i]->ArrSegments->DistForCamera = 0.f;

									if(Arr[i]->ArrSegments->DistForCamera > Arr[i]->DistForLod && Arr[i]->IsLoded)
										Arr[i]->IsCurrentRenderLod = true;
									else
										Arr[i]->IsCurrentRenderLod = false;*/


									if(res.x < 0 && res.x + res.width > 0)
									{
										res.width = res.width + res.x;
										res.x = 0;
									}

									if(res.y < 0 && res.y + res.height > 0)
									{
										res.height = res.height + res.y;
										res.y = 0;
									}

									if(res.IsIn)
									{
											if(Core::Data::Settings::IsUpdateCamera)
												ArrIsVisible[i] = true;
									}
									else if(res.maxdepth > 0.f)
									{
											for(int x=res.x;x<res.width+res.x;x++)
											{
													for(int y=res.y;y<res.height+res.y;y++)
													{
														tmpkey = ((y) * sizewarrdepth) + (x);
														tmpdepth = Core::Data::ArrDepthOC[tmpkey];

															if(tmpdepth >= res.mindepth)
															{
																x=res.width+res.x;
																y=res.height+res.y;

																	if(Core::Data::Settings::IsUpdateCamera)
																		ArrIsVisible[i] = true;
															}
													}
											}

											if(!(ArrIsVisible[i]))
												Core::Data::Statistics::CountModelExcluded++;
									}
							}

							//если объект виден, но не делен тогда говорим что все его подгруппы видны
							/*if(Arr[i]->ArrSegments->IsRenderForCamera && !Arr[i]->ArrSegments->BFNonEnd)
							{
									for(DWORD j=0;j<Arr[i]->Data->SubsetCount;j++)
									{
											if(Arr[i]->ArrReflection[j])
												Arr[i]->IsVisibleGroup[j] = true;
									}

								ArrComForCamera[i]->Arr[0] = Arr[i]->ArrSegments;
								ArrComForCamera[i]->CountCom = 1;
							}*/
							
								
					}
			}
	}

};
};
};
