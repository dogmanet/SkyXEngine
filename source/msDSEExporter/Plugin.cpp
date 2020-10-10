#include "stdafx.h"
#include "Plugin.h"
#include <msLib/msLib.h>


#pragma comment(lib, "../sdks/msLib/lib/msModelLib.lib")

#include <stdio.h>
#include <process.h>

#include "Exporter.h"


cMsPlugIn* CreatePlugIn()
{
    return(new CPlugin());
}

CPlugin::CPlugin()
{
}

CPlugin::~CPlugin()
{
	if(m_pMdl)
	{
		msModel_Destroy(m_pMdl);
	}
}

int CPlugin::GetType()
{
    return(cMsPlugIn::eTypeExport);
}

const char* CPlugin::GetTitle()
{
	return("SkyXEngine DSE");
}


int CPlugin::Execute(msModel *pModel)
{
	if(!pModel)
	{
		return(-1);
	}

	//
	// check, if we have something to export
	//
//	if(msModel_GetMeshCount(pModel) == 0)
//	{
//		::MessageBox(NULL, "The model is empty!  Nothing exported!", "SkyXEngine DSE Export", MB_OK | MB_ICONWARNING);
//		return(0);
//	}

	//
	// choose filename
	//
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));

	char szFile[MS_MAX_PATH];
	char szFileTitle[MS_MAX_PATH];
	char szDefExt[32] = "txt";
	char szFilter[128] = "SkyXEngine Model (*.dse)\0*.dse\0\0";
	szFile[0] = '\0';
	szFileTitle[0] = '\0';

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrDefExt = szDefExt;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MS_MAX_PATH;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = MS_MAX_PATH;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	ofn.lpstrTitle = "Export DSE model";

	if(!::GetSaveFileName(&ofn))
	{
		return(0);
	}

	m_pMdl = pModel;

	CExporter exp(szFile, false, NULL);
	exp.setEndFrame(msModel_GetTotalFrames(pModel));

	char tmp[4096];
	for(int i = 0; i < msModel_GetMaterialCount(pModel); i++)
	{
		msMaterial_GetDiffuseTexture(msModel_GetMaterialAt(pModel, i), tmp, sizeof(tmp));
		exp.addTexture(tmp);
	}

	return(exp.execute());

#if 0
	//
	// export
	//
	FILE *file = fopen(szFile, "wt");
	if(!file)
		return -1;

	int i, j;
	char szName[MS_MAX_NAME];

	fprintf(file, "// MilkShape 3D ASCII\n\n");
	fprintf(file, "Frames: %d\n", msModel_GetTotalFrames(pModel));
	fprintf(file, "Frame: %d\n\n", msModel_GetFrame(pModel));
	fprintf(file, "Meshes: %d\n", msModel_GetMeshCount(pModel));
	for(i = 0; i < msModel_GetMeshCount(pModel); i++)
	{
		msMesh *pMesh = msModel_GetMeshAt(pModel, i);
		msMesh_GetName(pMesh, szName, MS_MAX_NAME);
		if(strlen(szName) == 0)
			strcpy(szName, " ");

		fprintf(file, "\"%s\" %d %d\n", szName, msMesh_GetFlags(pMesh), msMesh_GetMaterialIndex(pMesh));

		//
		// vertices
		//
		fprintf(file, "%d\n", msMesh_GetVertexCount(pMesh));
		for(j = 0; j < msMesh_GetVertexCount(pMesh); j++)
		{
			msVertex *pVertex = msMesh_GetVertexAt(pMesh, j);
			msVec3 Vertex;
			msVec2 uv;

			msVertex_GetVertex(pVertex, Vertex);
			msVertex_GetTexCoords(pVertex, uv);

			fprintf(file, "%d %f %f %f %f %f %d\n",
				msVertex_GetFlags(pVertex),
				Vertex[0], Vertex[1], Vertex[2],
				uv[0], uv[1],
				msVertex_GetBoneIndex(pVertex)
				);
		}

		//
		// vertex normals
		//
		fprintf(file, "%d\n", msMesh_GetVertexNormalCount(pMesh));
		for(j = 0; j < msMesh_GetVertexNormalCount(pMesh); j++)
		{
			msVec3 Normal;
			msMesh_GetVertexNormalAt(pMesh, j, Normal);
			fprintf(file, "%f %f %f\n", Normal[0], Normal[1], Normal[2]);
		}

		//
		// triangles
		//
		fprintf(file, "%d\n", msMesh_GetTriangleCount(pMesh));
		for(j = 0; j < msMesh_GetTriangleCount(pMesh); j++)
		{
			msTriangle *pTriangle = msMesh_GetTriangleAt(pMesh, j);

			word nIndices[3];
			msTriangle_GetVertexIndices(pTriangle, nIndices);

			fprintf(file, "%d %d %d %d %d %d %d %d\n",
				msTriangle_GetFlags(pTriangle),
				nIndices[0], nIndices[1], nIndices[2],
				pTriangle->nNormalIndices[0], pTriangle->nNormalIndices[1], pTriangle->nNormalIndices[2],
				msTriangle_GetSmoothingGroup(pTriangle)
				);
		}
	}

	//
	// materials
	//
	fprintf(file, "\nMaterials: %d\n", msModel_GetMaterialCount(pModel));
	for(i = 0; i < msModel_GetMaterialCount(pModel); i++)
	{
		msMaterial *pMaterial = msModel_GetMaterialAt(pModel, i);
		msMaterial_GetName(pMaterial, szName, MS_MAX_NAME);
		if(strlen(szName) == 0)
			strcpy(szName, " ");
		fprintf(file, "\"%s\"\n", szName);

		msVec4 vec4;
		msMaterial_GetAmbient(pMaterial, vec4);
		fprintf(file, "%f %f %f %f\n", vec4[0], vec4[1], vec4[2], vec4[3]);
		msMaterial_GetDiffuse(pMaterial, vec4);
		fprintf(file, "%f %f %f %f\n", vec4[0], vec4[1], vec4[2], vec4[3]);
		msMaterial_GetSpecular(pMaterial, vec4);
		fprintf(file, "%f %f %f %f\n", vec4[0], vec4[1], vec4[2], vec4[3]);
		msMaterial_GetEmissive(pMaterial, vec4);
		fprintf(file, "%f %f %f %f\n", vec4[0], vec4[1], vec4[2], vec4[3]);
		fprintf(file, "%f\n", msMaterial_GetShininess(pMaterial));
		fprintf(file, "%f\n", msMaterial_GetTransparency(pMaterial));

		char szTexture[MS_MAX_PATH];
		msMaterial_GetDiffuseTexture(pMaterial, szTexture, MS_MAX_PATH);
		fprintf(file, "\"%s\"\n", szTexture);
		msMaterial_GetAlphaTexture(pMaterial, szTexture, MS_MAX_PATH);
		fprintf(file, "\"%s\"\n", szTexture);
	}

	//
	// bones
	//
	fprintf(file, "\nBones: %d\n", msModel_GetBoneCount(pModel));
	for(i = 0; i < msModel_GetBoneCount(pModel); i++)
	{
		msBone *pBone = msModel_GetBoneAt(pModel, i);
		msBone_GetName(pBone, szName, MS_MAX_NAME);
		if(strlen(szName) == 0)
			strcpy(szName, " ");
		fprintf(file, "\"%s\"\n", szName);
		msBone_GetParentName(pBone, szName, MS_MAX_NAME);
		fprintf(file, "\"%s\"\n", szName);
		msVec3 Position, Rotation;
		msBone_GetPosition(pBone, Position);
		msBone_GetRotation(pBone, Rotation);
		fprintf(file, "%d %f %f %f %f %f %f\n",
			msBone_GetFlags(pBone),
			Position[0], Position[1], Position[2],
			Rotation[0], Rotation[1], Rotation[2]
			);

		fprintf(file, "%d\n", msBone_GetPositionKeyCount(pBone));
		for(j = 0; j < msBone_GetPositionKeyCount(pBone); j++)
		{
			msPositionKey *pKey = msBone_GetPositionKeyAt(pBone, j);
			//fprintf (file, "Time: %f, Position Key: %f %f %f\n",
			fprintf(file, "%f %f %f %f\n",
				pKey->fTime, pKey->Position[0], pKey->Position[1], pKey->Position[2]
				);
		}

		fprintf(file, "%d\n", msBone_GetRotationKeyCount(pBone));
		for(j = 0; j < msBone_GetRotationKeyCount(pBone); j++)
		{
			msRotationKey *pKey = msBone_GetRotationKeyAt(pBone, j);
			//fprintf (file, "Time: %f, Rotation Key: %f %f %f\n",
			fprintf(file, "%f %f %f %f\n",
				pKey->fTime, pKey->Rotation[0], pKey->Rotation[1], pKey->Rotation[2]
				);
		}
	}

	unsigned int nNumComments;

	// group comments
	{
		nNumComments = 0;
		for(i = 0; i < msModel_GetMeshCount(pModel); i++)
		{
			msMesh *pMesh = msModel_GetMeshAt(pModel, i);
			int nCommentLength = msMesh_GetComment(pMesh, NULL, 0);
			if(nCommentLength > 0)
				++nNumComments;
		}

		fprintf(file, "GroupComments: %d\n", nNumComments);

		for(i = 0; i < msModel_GetMeshCount(pModel); i++)
		{
			msMesh *pMesh = msModel_GetMeshAt(pModel, i);
			int nCommentLength = msMesh_GetComment(pMesh, NULL, 0);
			if(nCommentLength > 0)
			{
				fprintf(file, "%d\n", i);
				char *pszComment = new char[nCommentLength + 1];
				msMesh_GetComment(pMesh, pszComment, nCommentLength);
				pszComment[nCommentLength] = '\0';
				while(true)
				{
					char * p = strstr(pszComment, "\r\n");
					if(p)
					{
						p[0] = '\\';
						p[1] = 'n';
					}
					else
						break;
				}
				fprintf(file, "%s\n", pszComment);
				delete[] pszComment;
			}
		}
	}

	// material comments
	{
		nNumComments = 0;
		for(i = 0; i < msModel_GetMaterialCount(pModel); i++)
		{
			msMaterial *pMaterial = msModel_GetMaterialAt(pModel, i);
			int nCommentLength = msMaterial_GetComment(pMaterial, NULL, 0);
			if(nCommentLength > 0)
				++nNumComments;
		}

		fprintf(file, "MaterialComments: %d\n", nNumComments);

		for(i = 0; i < msModel_GetMaterialCount(pModel); i++)
		{
			msMaterial *pMaterial = msModel_GetMaterialAt(pModel, i);
			int nCommentLength = msMaterial_GetComment(pMaterial, NULL, 0);
			if(nCommentLength > 0)
			{
				fprintf(file, "%d\n", i);
				char *pszComment = new char[nCommentLength + 1];
				msMaterial_GetComment(pMaterial, pszComment, nCommentLength);
				pszComment[nCommentLength] = '\0';
				while(true)
				{
					char * p = strstr(pszComment, "\r\n");
					if(p)
					{
						p[0] = '\\';
						p[1] = 'n';
					}
					else
						break;
				}
				fprintf(file, "%s\n", pszComment);
				delete[] pszComment;
			}
		}
	}

	// joint comments
	{
		nNumComments = 0;
		for(i = 0; i < msModel_GetBoneCount(pModel); i++)
		{
			msBone *pBone = msModel_GetBoneAt(pModel, i);
			int nCommentLength = msBone_GetComment(pBone, NULL, 0);
			if(nCommentLength > 0)
				++nNumComments;
		}

		fprintf(file, "BoneComments: %d\n", nNumComments);

		for(i = 0; i < msModel_GetBoneCount(pModel); i++)
		{
			msBone *pBone = msModel_GetBoneAt(pModel, i);
			int nCommentLength = msBone_GetComment(pBone, NULL, 0);
			if(nCommentLength > 0)
			{
				fprintf(file, "%d\n", i);
				char *pszComment = new char[nCommentLength + 1];
				msBone_GetComment(pBone, pszComment, nCommentLength);
				pszComment[nCommentLength] = '\0';
				while(true)
				{
					char * p = strstr(pszComment, "\r\n");
					if(p)
					{
						p[0] = '\\';
						p[1] = 'n';
					}
					else
						break;
				}
				fprintf(file, "%s\n", pszComment);
				delete[] pszComment;
			}
		}
	}

	// model comments
	{
		nNumComments = 0;
		int nCommentLength = msModel_GetComment(pModel, NULL, 0);
		if(nCommentLength > 0)
			nNumComments = 1;
		fprintf(file, "ModelComment: %d\n", nNumComments);

		if(nCommentLength > 0)
		{
			char *pszComment = new char[nCommentLength + 1];
			msModel_GetComment(pModel, pszComment, nCommentLength);
			pszComment[nCommentLength] = '\0';
			while(true)
			{
				char * p = strstr(pszComment, "\r\n");
				if(p)
				{
					p[0] = '\\';
					p[1] = 'n';
				}
				else
					break;
			}
			fprintf(file, "%s\n", pszComment);
			delete[] pszComment;
		}
	}

	fclose(file);

	// dont' forget to destroy the model
	msModel_Destroy(pModel);
#endif
	return 0;
}

std::vector<std::string> CPlugin::getListTextures()
{
	std::vector<std::string> tmp;
	for(auto i = m_mList.m_mTextures.begin(); i != m_mList.m_mTextures.end(); ++i)
	{
		tmp.push_back(i->first);
	}
	return(tmp);
}

bool CPlugin::getConfigBool(const char *szKey, bool bDefault)
{
	char tmp[32];
	getConfigWStr(szKey, tmp, sizeof(tmp));
	int i = 0;
	sscanf(tmp, "%d", &i);
	return(i != 0);
}
void CPlugin::setConfigBool(const char *szKey, bool bvalue)
{
	char tmp[2];
	tmp[0] = bvalue ? '1' : '0';
	tmp[1] = 0;

	setConfigWStr(szKey, tmp);
}

float CPlugin::getConfigFloat(const char *szKey, float fDefault)
{
	char tmp[32];
	getConfigWStr(szKey, tmp, sizeof(tmp));
	float f = 0;
	sscanf(tmp, "%f", &f);
	return(f);
}
void CPlugin::setConfigFloat(const char *szKey, float fvalue)
{
	char tmp[32];
	sprintf(tmp, "%f", fvalue);

	setConfigWStr(szKey, tmp);
}

int CPlugin::getConfigInt(const char *szKey, int iDefault)
{
	char tmp[32];
	getConfigWStr(szKey, tmp, sizeof(tmp));
	int i = 0;
	sscanf(tmp, "%d", &i);
	return(i);
}
void CPlugin::setConfigInt(const char *szKey, int ivalue)
{
	char tmp[32];
	sprintf(tmp, "%d", ivalue);

	setConfigWStr(szKey, tmp);
}

void CPlugin::getConfigWStr(const char *szKey, char *szOut, int iMaxOut)
{
	char tmp[4096];
	msModel_GetComment(m_pMdl, tmp, 4096);
	
	int iRows = parse_str(tmp, NULL, 0, '\n');
	char **pszParts = (char**)alloca(sizeof(char*) * iRows);
	parse_str(tmp, pszParts, iRows, '\n');

	size_t l0 = strlen(szKey);

	for(int i = 0; i < iRows; ++i)
	{
		if(pszParts[i][0] == '#')
		{
			size_t l1 = strlen(pszParts[i]);
			if(l0 >= l1)
			{
				continue;
			}

			if(!memcmp(pszParts[i] + 1, szKey, l0) && pszParts[i][1 + l0] == ' ')
			{
				strncpy(szOut, pszParts[i] + 1 + l0 + 1, iMaxOut);
				szOut[iMaxOut - 1] = 0;
				return;
			}
		}
	}
}
void CPlugin::setConfigWStr(const char *szKey, const char *szValue)
{
	char tmp[4096], tmp2[4096];
	tmp2[0] = 0;

	msModel_GetComment(m_pMdl, tmp, 4096);

	int iRows = parse_str(tmp, NULL, 0, '\n');
	char **pszParts = (char**)alloca(sizeof(char*) * iRows);
	parse_str(tmp, pszParts, iRows, '\n');

	size_t l0 = strlen(szKey);

	bool isFound = false;
	char *ptr = tmp2;
	for(int i = 0; i < iRows; ++i)
	{
		size_t l1 = strlen(pszParts[i]);
		if(pszParts[i][0] == '#' && l0 < l1 && !memcmp(pszParts[i] + 1, szKey, l0) && pszParts[i][1 + l0] == ' ')
		{
			ptr += sprintf(ptr, "#%s %s\r\n", szKey, szValue);
			isFound = true;
		}
		else
		{
			ptr += sprintf(ptr, "%s\r\n", pszParts[i]);
		}
	}

	if(!isFound)
	{
		ptr += sprintf(ptr, "#%s %s\r\n", szKey, szValue);
	}

	msModel_SetComment(m_pMdl, tmp2);
}

void CPlugin::ProcessThread(void *p)
{
	CPlugin *exp = (CPlugin*)p;
	
	//uint64_t iSize = 0;

	exp->m_iProgress += 10;

#if 0
	if(exp->m_bExportGibs)
	{
		int iMeshesCount = msModel_GetMeshCount(exp->m_pMdl);

		if(iMeshesCount == 0)
		{
			EndDialog(exp->m_hDlgWnd, 0);
			return;
		}
		int iProgress = 0;
		SendMessage(exp->m_hProgressBar, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, iMeshesCount * 100));
		int len = lstrlen(exp->m_szName) + 1;
		TCHAR *szName = new TCHAR[len];
		lstrcpy(szName, exp->m_szName);
		int nn = 2;
		const char *szMask = "%s_gib%02d.dse";
		if(iMeshesCount > 99)
		{
			nn = 3;
			szMask = "%s_gib%03d.dse";
		}
		szName[len - 5] = 0;
		TCHAR *szTmpName = new TCHAR[len + 4 + nn]; // _gibnn
		for(int i = 0; i < iMeshesCount; i++)
		{
			msMesh *pMesh = msModel_GetMeshAt(exp->m_pMdl, i);
			CPlugin exporter;
			exporter.m_bExportMesh = true;
			exporter.m_bExportStatic = exp->m_bExportStatic;
			exporter.m_bExportTaBi = exp->m_bExportTaBi;
			//pExporter->options = exp->options;
			//pExporter->exportSelected = exp->exportSelected;
			exporter.m_bSuppressPrompts = true;
			exporter.m_pOnlyMesh = pMesh;
			//pExporter->in = exp->in;
			//pExporter->pEI = exp->pEI;
			exporter.m_bSkipInitIGame = true;

			wsprintf(szTmpName, szMask, szName, i);

			exporter.m_szName = szTmpName;

			ProcessThread(&exporter);

			iProgress += 100;
			SendMessage(exp->m_hProgressBar, PBM_SETPOS, (WPARAM)iProgress, 0);
		}
		delete[] szName;
		delete[] szTmpName;
		MessageBoxW(exp->m_hDlgWnd, L"Export done!", L"DSE Model export", NULL);
		EndDialog(exp->m_hDlgWnd, 0);
		return;
	}
#endif

	std::vector<ModelPhyspart> vPhysparts;

	ModelHeader Header;
	ModelHeader2 Header2;
	memset(&Header, 0, sizeof(ModelHeader));
	memset(&Header2, 0, sizeof(ModelHeader2));
	Header.Magick = SX_MODEL_MAGICK;
	Header.iVersion = SX_MODEL_VERSION;
	Header.iFlags = MODEL_FLAG_SOURCED | MODEL_FLAG_NEW_PACKED_ANIMS;
	if(exp->m_bExportTaBi)
	{
		Header.iFlags |= MODEL_FLAG_HAS_TANGENT_BINORM;
	}
	if(exp->m_bExportStatic)
	{
		Header.iFlags |= MODEL_FLAG_STATIC;
	}
	Header.iSkinCount = 1;

#if 0
	//Header.iPhysicsDataOffset = NULL;
	if(exp->m_bExportMesh)
	{
		Header.iFlags |= MODEL_FLAG_NORMALIZED_NORMALS;
		int iMeshesCount = msModel_GetMeshCount(exp->m_pMdl);

		if(iMeshesCount == 0)
		{
			if(!exp->m_bSuppressPrompts)
			{
				EndDialog(exp->m_hDlgWnd, 0);
			}
			return;
		}

		for(i = 0; i < msModel_GetMaterialCount(exp->m_pMdl); i++)
		{
			exp->m_mList.Add(msModel_GetMaterialAt(exp->m_pMdl, i));
		}


		int nCurNode = 0;
		exp->nTotalNodeCount = 0;
		//exp->m_mBoneIndices[NULL] = {-1, -1};
		UINT boneid = 0;
		for(int i = 0; i < pIgame->GetTopLevelNodeCount(); i++)
		{
			exp->BuildBoneHierarchy(pIgame->GetTopLevelNode(i), boneid, pIgame->GetTopLevelNode(i));
		}
#endif
#if 0
		int iCurProgr = 0;
		if(!exp->m_bSuppressPrompts)
		{
			SendMessage(exp->m_hProgressBar, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, iMeshesCount * 100));
		}
		for(int i = 0; i < iMeshesCount; i++)
		{
			//meshes[i]->GetNodeParent();
			msMesh *pMesh = msModel_GetMeshAt(exp->m_pMdl, i);
			if(exp->m_pOnlyMesh && exp->m_pOnlyMesh != pMesh)
			{
				continue;
			}

			int iLod = 0;

			ILayer *pLayer = (ILayer*)pINode->GetReference(NODE_LAYER_REF);
			if(pLayer)
			{
				ILayerProperties *pLayerProps = (ILayerProperties*)pLayer->GetInterface(LAYERPROPERTIES_INTERFACE);

				const TCHAR *szLayerName = pLayerProps->getName();

				//MSTR str = pLayer->GetName();
				//const wchar_t *szLayerName = str.ToMCHAR();
				OutputDebugString(szLayerName);
				if(!lstrcmp(szLayerName, "#physbox"))
				{
					if(mesh->InitializeData())
					{
						float3_t vCenter;
						float3_t vLWH;

						ModelPhyspart physPart;
						memset(&physPart, 0, sizeof(ModelPhyspart));
						if(CGeomDetector::IsBox(mesh, &vCenter, &vLWH))
						{ // HT_BOX
							physPart.type = HT_BOX;
							physPart.lwh = vLWH;
							physPart.pos = vCenter; // + world mat
						}
						else if(CGeomDetector::IsSphere(mesh, &vCenter, &vLWH))
						{ // HT_ELIPSOID
							physPart.type = HT_ELIPSOID;
							physPart.lwh = vLWH;
							physPart.pos = vCenter; // + world mat
						}
						else // HT_CONVEX
						{
							physPart.type = HT_CONVEX;
							ModelPhyspartDataConvex *pDataConvex = new ModelPhyspartDataConvex();
							pDataConvex->iVertCount = mesh->GetNumberOfVerts();
							pDataConvex->pVerts = new float3_t[pDataConvex->iVertCount];

							Point3 pt;
							for(int j = 0; j < pDataConvex->iVertCount; ++j)
							{
								pt = mesh->GetVertex(j);
								pDataConvex->pVerts[j].x = pt.x;
								pDataConvex->pVerts[j].y = pt.y;
								pDataConvex->pVerts[j].z = pt.z;
							}
							physPart.rot.w = 1.0f;
							physPart.pData = pDataConvex;
						}

						if(physPart.type != HT_CONVEX)
						{
							GMatrix worldlocaltran = meshes[i]->GetWorldTM();
							worldlocaltran;

							Quat r = worldlocaltran.Rotation();
							Point3 or = r.Vector();
							physPart.rot.x = or.x;
							physPart.rot.y = or.y;
							physPart.rot.z = or.z;
							physPart.rot.w = r.Scalar();
							Point3 tr = worldlocaltran.Translation();
							physPart.pos.x += tr.x;
							physPart.pos.y += tr.y;
							physPart.pos.z += tr.z;
						}

						IGameSkin *skin = meshes[i]->GetIGameObject()->GetIGameSkin();
						if(skin)
						{
							int iBone = -1;
							for(int j = 0, jl = mesh->GetNumberOfVerts(); j < jl; ++j)
							{
								if(skin->GetNumberOfBones(i) != 1)
								{
									if(!exp->suppressPrompts)
									{
										MessageBox(exp->hDlgWnd, "All vertices of physbox should be skinned to the same bone", meshes[i]->GetName(), MB_OK | MB_ICONSTOP);
										EndDialog(exp->hDlgWnd, IMPEXP_CANCEL);
									}
									return;
								}
								if(iBone < 0)
								{
									iBone = skin->GetBoneID(i, 0);
								}
								if(iBone != skin->GetBoneID(i, 0))
								{
									if(!exp->suppressPrompts)
									{
										MessageBox(exp->hDlgWnd, _T("All vertices of physbox should be skinned to the same bone"), meshes[i]->GetName(), MB_OK | MB_ICONSTOP);
										EndDialog(exp->hDlgWnd, IMPEXP_CANCEL);
									}
									return;
								}
							}
							physPart.bone_id = iBone;
							const TCHAR *szBone = skin->GetBone(iBone)->GetName();
							int len = lstrlen(szBone) + 1;
							if(len > MODEL_BONE_MAX_NAME)
							{
								len = MODEL_BONE_MAX_NAME;
							}
							memcpy(physPart.bone, toString(szBone).c_str(), lstrlen(szBone) + 1);
							physPart.bone[MODEL_BONE_MAX_NAME - 1] = 0;
						}
						else
						{
							physPart.bone_id = -1;
						}

						vPhysparts.push_back(physPart);
					}
					continue;
				}

				_stscanf(szLayerName, "#lod%d", &iLod);

				if(iLod < 0)
				{
					if(!exp->m_bSuppressPrompts)
					{
						MessageBox(exp->m_hDlgWnd, "Invalid LOD index < 0", _T("Error!"), MB_OK | MB_ICONSTOP);
						EndDialog(exp->m_hDlgWnd, 0);
					}
					return;
				}
			}

			if(mesh->InitializeData())
			{
				int iProgr = 0;
				//GMatrix tm = meshes[i]->GetWorldTM();
				GMatrix worldlocaltran = meshes[i]->GetWorldTM();
				//GMatrix worldlocaltran = meshes[i]->GetWorldTM().Inverse();
				GMatrix worldlocalrotonly = worldlocaltran;
				worldlocalrotonly.SetRow(3, Point4(0, 0, 0, 1));

				IGameSkin * skin = NULL;

				if(!(Header.iFlags & MODEL_FLAG_STATIC))
				{
					skin = meshes[i]->GetIGameObject()->GetIGameSkin();
					if(!skin)
					{

						std_tstring msg = _T("All vertices must be skinned to a bone\nMesh \"");
#	ifdef _MAX_2010
						//msg += mesh->GetClassName();
						msg += meshes[i]->GetMaxNode()->GetName();
#	endif
						msg += _T("\" has vertices not skinned to bone");
						if(!exp->suppressPrompts)
						{
							MessageBox(exp->hDlgWnd, msg.c_str(), _T("Error!"), MB_OK | MB_ICONSTOP);
							EndDialog(exp->hDlgWnd, IMPEXP_CANCEL);
						}
						return;
					}
				}
				int iFacesCount = mesh->GetNumberOfFaces();
				for(int iFace = 0; iFace < iFacesCount; iFace++)
				{
					FaceEx * pFace = mesh->GetFace(iFace);
					for(int i = 0; i < 3; i++)
					{
						Point3 pos = mesh->GetVertex(pFace->vert[i], false)/* * worldlocaltran*/;
						//pos = pos * tm;
						int iIndexTB = mesh->GetFaceVertexTangentBinormal(iFace, i);
						Point3 tangent = mesh->GetTangent(iIndexTB).Normalize();
						Point3 binormal = mesh->GetBinormal(iIndexTB).Normalize();
						Point3 normal = mesh->GetNormal(pFace->norm[i], false).Normalize()/* * worldlocalrotonly*/;

						Point3 color = mesh->GetColorVertex(pFace->color[i]);

						int iTexIndex = mesh->GetFaceTextureVertex(iFace, i, 1);
						Point2 tc1 = mesh->GetTexVertex(iTexIndex);


						vertex v;
						memset(&v.BoneIndices, 0, sizeof(v.BoneIndices));
						memset(&v.BoneWeights, 0, sizeof(v.BoneWeights));
						if(skin)
						{
							int nbones = skin->GetNumberOfBones(pFace->vert[i]);
							for(int b = 0; b < nbones; b++)
							{
								for(int j = 0; j < 4; j++)
								{
									if((&(v.BoneWeights.x))[j] < skin->GetWeight(pFace->vert[i], b))
									{
										for(int k = 3; k > j; k--)
										{
											(&(v.BoneWeights.x))[k] = (&(v.BoneWeights.x))[k - 1];
											//(&(v.BoneWeights.x))[k] = (&(v.BoneWeights.x))[k - 1];
											v.BoneIndices[k] = v.BoneIndices[k - 1];
										}
										(&(v.BoneWeights.x))[j] = skin->GetWeight(pFace->vert[i], b);
										v.BoneIndices[j] = exp->m_mBoneIndices[skin->GetIGameBone(pFace->vert[i], b)].id;

										// Get bones in the bone map to be assigned later
										//if(!g_bonemap.count(bones[j]))
										//	g_bonemap[bones[j]] = 0;

										break;
									}
								}
							}
						}

						v.Pos.x = pos.x; v.Pos.y = pos.y; v.Pos.z = pos.z;
						v.Norm.x = normal.x; v.Norm.y = normal.y, v.Norm.z = normal.z;
						v.Tangent.x = tangent.x; v.Tangent.y = tangent.y, v.Tangent.z = tangent.z;
						v.Binorm.x = binormal.x; v.Binorm.y = binormal.y, v.Binorm.z = binormal.z;
						v.Tex.x = tc1.x; v.Tex.y = tc1.y;


						IGameMaterial * mtl = mesh->GetMaterialFromFace(iFace);

						//const wchar_t *szMtlName = mtl->GetMaterialName();

						Mtl * pmmtl = mtl->GetMaxMaterial();

						std_tstring s = exp->m_mList.GetTexture(pmmtl, 0, exp);

						SubMesh * pSub = exp->AOFSMBM(iLod, s);

						pSub->AddVertex(v);
					}
					if(iProgr <= (iFace + 1) * 100 / iFacesCount)
					{
						iCurProgr += (iFace + 1) * 100 / iFacesCount - iProgr;
						iProgr = (iFace + 1) * 100 / iFacesCount;
						if(!exp->suppressPrompts)
						{
							SendMessage(exp->hProgressBar, PBM_SETPOS, (WPARAM)iCurProgr, 0);
						}
					}
				}
				iCurProgr += 100 - iProgr;
				if(!exp->m_bSuppressPrompts)
				{
					SendMessage(exp->m_hProgressBar, PBM_SETPOS, (WPARAM)iCurProgr, 0);
				}
			}
			else
			{
				iCurProgr += 100;
				if(!exp->m_bSuppressPrompts)
				{
					SendMessage(exp->m_hProgressBar, PBM_SETPOS, (WPARAM)iCurProgr, 0);
				}
			}

		}

		std::vector<ModelLoD> vLODheader;

		for(int k = 0, kl = exp->m_vmpSM.size(); k < kl; ++k)
		{
			ModelLoD lodHeader;

			lodHeader.iSubMeshCount = exp->m_vmpSM[k].size();
			lodHeader.pSubLODmeshes = new ModelLoDSubset[lodHeader.iSubMeshCount];
			//uint64_t iLODsize = sizeof(uint32_t);
			int ind = 0;
			for(std::map<std::string, SubMesh*>::iterator i = exp->m_vmpSM[k].begin(); i != exp->m_vmpSM[k].end(); i++, ind++)
			{
				lodHeader.pSubLODmeshes[ind].iMaterialID = exp->m_mList.m_mTextures[i->first];
				SubMesh * pSub = i->second;
				lodHeader.pSubLODmeshes[ind].iIndexCount = pSub->m_indices.size();
				lodHeader.pSubLODmeshes[ind].iVectexCount = pSub->m_vertices.size();
				lodHeader.pSubLODmeshes[ind].pIndices = new UINT[lodHeader.pSubLODmeshes[ind].iIndexCount];
				lodHeader.pSubLODmeshes[ind].pVertices = new vertex[lodHeader.pSubLODmeshes[ind].iVectexCount];
				for(uint64_t j = 0; j < lodHeader.pSubLODmeshes[ind].iVectexCount; j++)
				{
					lodHeader.pSubLODmeshes[ind].pVertices[j] = pSub->m_vertices[j];
				}
				for(uint64_t j = 0; j < lodHeader.pSubLODmeshes[ind].iIndexCount; j++)
				{
					lodHeader.pSubLODmeshes[ind].pIndices[j] = pSub->m_indices[j];
				}
				//iLODsize += sizeof(uint32_t)* 3 + sizeof(vertex)* lodHeader.pSubLODmeshes[ind].iIndexCount + sizeof(UINT)* lodHeader.pSubLODmeshes[ind].iIndexCount;
			}

			vLODheader.push_back(lodHeader);
		}






		Header.iFlags |= MODEL_FLAG_SOURCE_MESH;
		Header.iMaterialCount = exp->m_mList.TexCount();
		Header.iMaterialsOffset = NULL;
		Header.iLODcount = vLODheader.size();
		Header.iLODoffset = NULL;
		Header.iBoneCount = exp->m_mBoneIndices.size();
		Header.iBonesOffset = NULL;
		Header.iAnimationCount = 0;
		Header.iAnimationsOffset = NULL;

		Header2.iBoneTableCount = Header.iBoneCount;
		Header2.topology = MDLPT_TRIANGLELIST;


		FILE *pF = fopen(exp->m_szName, "wb");
		//MessageBox(exp->hDlgWnd, exp->name, "", NULL);
		if(!pF)
		{
			if(!exp->m_bSuppressPrompts)
			{
				EndDialog(exp->m_hDlgWnd, 0);
			}
			return;
		}

		//Write header
		fwrite((void*)&Header, sizeof(ModelHeader), 1, pF);
		Header.iSecondHeaderOffset = ftell(pF);
		fwrite((void*)&Header2, sizeof(ModelHeader2), 1, pF);

		//Write materials

		struct names_s
		{
			char szName[MODEL_MAX_NAME];
		} *names;
		names = new names_s[exp->m_mList.m_mTextures.size()];
		memset(names, 0, sizeof(names_s) * exp->m_mList.m_mTextures.size());
		//std::vector<std::string> vs;
		//vs.resize(exp->m_mList.m_mTextures.size());
		Header.iMaterialsOffset = ftell(pF);
		for(std::map<std::string, int>::iterator i = exp->m_mList.m_mTextures.begin(); i != exp->m_mList.m_mTextures.end(); i++)
		{
			//vs[i->second] = i->first;
			//fwrite(i->first.c_str(), 1, i->first.length() + 1, pF);
			memcpy(&names[i->second], toString(i->first).c_str(), i->first.length() + 1);
		}
		fwrite(names, sizeof(names_s), exp->m_mList.m_mTextures.size(), pF);

		int iSkinCount = exp->getConfigInt(SXPROP_EXP_SCNT);
		Header.iSkinCount = iSkinCount + 1;
		TCHAR tex[128], tkey[128];
		for(int j = 0; j < iSkinCount; ++j)
		{
			memset(names, 0, sizeof(names_s)* exp->m_mList.m_mTextures.size());
			for(std::map<std::string, int>::iterator i = exp->m_mList.m_mTextures.begin(); i != exp->m_mList.m_mTextures.end(); i++)
			{
				wsprintf(tkey, SXPROP_EXP_SKN_PREF "%s/%d", i->first.c_str(), j + 1);
				exp->getConfigWStr(tkey, tex, sizeof(tex));
				if(tex[0]) // use default tex
				{
					sprintf(names[i->second].szName, "%s", toString(tex).c_str());
				}
				else
				{
					memcpy(&names[i->second], toString(i->first).c_str(), i->first.length() + 1);
				}
			}
			fwrite(names, sizeof(names_s), exp->m_mList.m_mTextures.size(), pF);

		}
		delete[] names;

		//for(std::vector<std::string>::iterator i = vs.begin(); i != vs.end(); i++)
		//{
		//	fwrite(i->c_str(), 1, i->length() + 1, pF);
		//}



		//Write Bones
		Header2.iBoneTableOffset = ftell(pF);
		ModelBoneName * mbns = new ModelBoneName[exp->m_mBoneIndices.size()];

		for(std::map<IGameNode*, ModelBone>::iterator i = exp->m_mBoneIndices.begin(); i != exp->m_mBoneIndices.end(); i++)
		{
			//ModelBoneName mbn;
			mbns[i->second.id].bone = i->second;
			memcpy(mbns[i->second.id].szName, toString(exp->m_mBoneNames[i->first]).c_str(), exp->m_mBoneNames[i->first].length() + 1);
			//fwrite((void*)&i->second, sizeof(boneID)/*-sizeof(std::string)*/, 1, pF);
			//fwrite(exp->m_mBoneNames[i->first].c_str(), 1, exp->m_mBoneNames[i->first].length() + 1, pF);
			//fwrite(&mbn, sizeof(
		}
		fwrite(mbns, sizeof(ModelBoneName), exp->m_mBoneIndices.size(), pF);
		delete[] mbns;

		//Write LOD
		Header.iLODoffset = ftell(pF);


		for(int idx = 0, idxl = vLODheader.size(); idx < idxl; ++idx)
		{
			DSmodelLODheader &lodHeader = vLODheader[idx];
			ModelLoD pLod;
			pLod.iSubMeshCount = lodHeader.iSubMeshCount;

			fwrite(&pLod, MODEL_LOD_STRUCT_SIZE, 1, pF);

			for(int i = 0; i < lodHeader.iSubMeshCount; i++)
			{

				fwrite((void*)&lodHeader.pSubLODmeshes[i].iMaterialID, sizeof(uint32_t), 1, pF);
				fwrite((void*)&lodHeader.pSubLODmeshes[i].iVectexCount, sizeof(uint32_t), 1, pF);
				fwrite((void*)&lodHeader.pSubLODmeshes[i].iIndexCount, sizeof(uint32_t), 1, pF);

				if(Header.iFlags & MODEL_FLAG_STATIC)
				{
					if(Header.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
					{
						vertex_static_ex va;
						for(int j = 0; j < lodHeader.pSubLODmeshes[i].iVectexCount; j++)
						{
							va.Pos = lodHeader.pSubLODmeshes[i].pVertices[j].Pos;
							va.Norm = lodHeader.pSubLODmeshes[i].pVertices[j].Norm;
							va.Tex = lodHeader.pSubLODmeshes[i].pVertices[j].Tex;
							va.Binorm = lodHeader.pSubLODmeshes[i].pVertices[j].Binorm;
							va.Tangent = lodHeader.pSubLODmeshes[i].pVertices[j].Tangent;
							fwrite(&va, sizeof(vertex_static_ex), 1, pF);
						}
					}
					else
					{
						vertex_static va;
						for(int j = 0; j < lodHeader.pSubLODmeshes[i].iVectexCount; j++)
						{
							va.Pos = lodHeader.pSubLODmeshes[i].pVertices[j].Pos;
							va.Norm = lodHeader.pSubLODmeshes[i].pVertices[j].Norm;
							va.Tex = lodHeader.pSubLODmeshes[i].pVertices[j].Tex;
							fwrite(&va, sizeof(vertex_static), 1, pF);
						}
					}
				}
				else
				{
					if(Header.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
					{
						//vertex_animated_ex
						fwrite((void*)lodHeader.pSubLODmeshes[i].pVertices, sizeof(vertex)* lodHeader.pSubLODmeshes[i].iVectexCount, 1, pF);
					}
					else
					{
						vertex_animated va;
						for(int j = 0; j < lodHeader.pSubLODmeshes[i].iVectexCount; j++)
						{
							va.Pos = lodHeader.pSubLODmeshes[i].pVertices[j].Pos;
							va.Norm = lodHeader.pSubLODmeshes[i].pVertices[j].Norm;
							for(int k = 0; k < 4; k++)
							{
								va.BoneIndices[k] = lodHeader.pSubLODmeshes[i].pVertices[j].BoneIndices[k];
							}
							va.BoneWeights = lodHeader.pSubLODmeshes[i].pVertices[j].BoneWeights;
							va.Tex = lodHeader.pSubLODmeshes[i].pVertices[j].Tex;
							fwrite(&va, sizeof(vertex_animated), 1, pF);
						}
					}
				}
				fwrite((void*)lodHeader.pSubLODmeshes[i].pIndices, sizeof(UINT)* lodHeader.pSubLODmeshes[i].iIndexCount, 1, pF);
			}
		}

		Header2.iPhysicsDataOffset = ftell(pF);
		ModelPhysData physData;
		physData.fWeight = exp->m_fWeight;
		physData.iPhyspartCount = vPhysparts.size();
		physData.iPartsOffset = Header2.iPhysicsDataOffset + MODEL_PHYSDATA_STRUCT_SIZE;
		fwrite((void*)&physData, MODEL_PHYSDATA_STRUCT_SIZE, 1, pF);

		int iOffset = physData.iPartsOffset + MODEL_PHYSPART_STRUCT_SIZE * vPhysparts.size();
		for(int i = 0, l = vPhysparts.size(); i < l; ++i)
		{
			if(vPhysparts[i].type == HT_CONVEX)
			{
				vPhysparts[i].iDataOffset = iOffset;
				iOffset += sizeof(uint32_t) + sizeof(float3_t) * ((ModelPhyspartDataConvex*)vPhysparts[i].pData)->iVertCount;
			}
		}
		for(int i = 0, l = vPhysparts.size(); i < l; ++i)
		{
			fwrite((void*)&vPhysparts[i], MODEL_PHYSPART_STRUCT_SIZE, 1, pF);
		}
		for(int i = 0, l = vPhysparts.size(); i < l; ++i)
		{
			if(vPhysparts[i].type == HT_CONVEX)
			{
				fwrite((void*)vPhysparts[i].pData, MODEL_PHYSPART_DATA_CONVEX_STRUCT_SIZE, 1, pF);
				fwrite((void*)((ModelPhyspartDataConvex*)vPhysparts[i].pData)->pVerts, sizeof(float3_t), ((ModelPhyspartDataConvex*)vPhysparts[i].pData)->iVertCount, pF);
			}
		}


		fseek(pF, 0, SEEK_SET);

		fwrite((void*)&Header, sizeof(ModelHeader), 1, pF);
		fseek(pF, Header.iSecondHeaderOffset, SEEK_SET);
		fwrite((void*)&Header2, sizeof(ModelHeader2), 1, pF);

		for(int i = 0, l = vPhysparts.size(); i < l; ++i)
		{
			if(vPhysparts[i].type == HT_CONVEX)
			{
				delete[]((ModelPhyspartDataConvex*)vPhysparts[i].pData)->pVerts;
				delete vPhysparts[i].pData;
			}
		}

		fclose(pF);
	}
#endif
#if 0
	else
	{
		//exp->m_mBoneIndices[NULL] = {-1, -1};
		UINT boneid = 0;
		for(int i = 0; i < pIgame->GetTopLevelNodeCount(); i++)
		{
			exp->BuildBoneHierarchy(pIgame->GetTopLevelNode(i), boneid, pIgame->GetTopLevelNode(i));
		}

		exp->m_mBoneAnimation.resize(exp->m_iEndFrame - exp->m_iStartFrame + 1);
		for(int i = 0; i < exp->m_iEndFrame - exp->m_iStartFrame + 1; i++)
		{
			//exp->m_mBoneAnimation[i][NULL] = {-1, -1};
		}
		exp->m_iTicksPerFrame = pIgame->GetSceneTicks();
		for(int i = exp->m_iStartFrame; i <= exp->m_iEndFrame; i++)
		{
			boneid = 0;
			for(int j = 0; j < pIgame->GetTopLevelNodeCount(); j++)
			{
				exp->BuildBoneAnimation(pIgame->GetTopLevelNode(j), i, boneid, pIgame->GetTopLevelNode(j), exp->m_iStartFrame);
			}
		}

		/*int iBN = 0;
		for(std::map<IGameNode*, ModelBone>::iterator i = exp->m_mBoneIndices.begin(); i != exp->m_mBoneIndices.end(); i++)
		{
		iBN += exp->m_mBoneNames[i->first].length() + 1;
		}*/


		Header.iLODcount = 0;
		Header.iLODoffset = NULL;
		Header.iFlags |= DS_MODEL_FLAG_SOURCE_ANIMATION;
		Header.iMaterialCount = 0;
		Header.iMaterialsOffset = NULL;
		Header.iBoneCount = exp->m_mBoneIndices.size();
		Header.iBonesOffset = NULL;
		Header.iAnimationCount = 1; // ?
		Header.iAnimationsOffset = NULL;

		Header2.iBoneTableCount = Header.iBoneCount;

		FILE * pF = _tfopen(exp->name, _T("wb"));
		//MessageBox(exp->hDlgWnd, exp->name, "", NULL);
		if(!pF)
		{
			if(!exp->suppressPrompts)
			{
				EndDialog(exp->hDlgWnd, IMPEXP_FAIL);
			}
			return;
		}

		//Write header
		fwrite((void*)&Header, sizeof(ModelHeader), 1, pF);
		Header.iSecondHeaderOffset = ftell(pF);
		fwrite((void*)&Header2, sizeof(ModelHeader2), 1, pF);

		//Write Bones
		Header2.iBoneTableOffset = ftell(pF);
		ModelBoneName * mbns = new ModelBoneName[exp->m_mBoneIndices.size()];

		for(std::map<IGameNode*, ModelBone>::iterator i = exp->m_mBoneIndices.begin(); i != exp->m_mBoneIndices.end(); i++)
		{
			//ModelBoneName mbn;
			mbns[i->second.id].bone = i->second;
			memcpy(mbns[i->second.id].szName, toString(exp->m_mBoneNames[i->first]).c_str(), exp->m_mBoneNames[i->first].length() + 1);
			//fwrite((void*)&i->second, sizeof(boneID)/*-sizeof(std::string)*/, 1, pF);
			//fwrite(exp->m_mBoneNames[i->first].c_str(), 1, exp->m_mBoneNames[i->first].length() + 1, pF);
			//fwrite(&mbn, sizeof(
		}
		fwrite(mbns, sizeof(ModelBoneName), exp->m_mBoneIndices.size(), pF);
		delete[] mbns;

		//Write Anim
		Header.iAnimationsOffset = ftell(pF);
		int16_t fc = exp->m_iEndFrame - exp->m_iStartFrame + 1;

		ModelSequence mseq;
		memset(&mseq, 0, sizeof(ModelSequence));
		mseq.iNumFrames = fc;

		fwrite(&mseq, MODEL_SEQUENCE_STRUCT_SIZE, 1, pF);

		ModelBone * mbs = new ModelBone[Header.iBoneCount];
		for(int i = exp->m_iStartFrame; i <= exp->m_iEndFrame; i++)
		{
			for(std::map<IGameNode*, ModelBone>::iterator j = exp->m_mBoneAnimation[i - exp->m_iStartFrame].begin(); j != exp->m_mBoneAnimation[i - exp->m_iStartFrame].end(); j++)
			{
				mbs[j->second.id] = j->second;
			}
			fwrite(mbs, sizeof(ModelBone), Header.iBoneCount, pF);
		}
		delete[] mbs;

		fseek(pF, 0, SEEK_SET);

		fwrite((void*)&Header, sizeof(ModelHeader), 1, pF);
		fseek(pF, Header.iSecondHeaderOffset, SEEK_SET);
		fwrite((void*)&Header2, sizeof(ModelHeader2), 1, pF);

		fclose(pF);
	}

#endif

	if(!exp->m_bSuppressPrompts)
	{
		MessageBox(exp->m_hDlgWnd, "Export done!", "DSE Model export", NULL);
	}

	EndDialog(exp->m_hDlgWnd, 0);
}

static TCHAR* FixName(const TCHAR* name)
{
	static TCHAR buffer[256];
	TCHAR* cPtr;

	strcpy(buffer, name);
	cPtr = buffer;

	while(*cPtr)
	{
		if(*cPtr == '"')
			*cPtr = 39;
		else if(*cPtr <= 31)
			*cPtr = '_';
		cPtr++;
	}

	return buffer;
}

void MtlList::Add(msMaterial *mtl, CPlugin *exp)
{
	if(!mtl)
	{
		return;
	}
	//
	if(!m_mList.count(mtl))
	{
		m_mList[mtl] = Count();

		char tmp[4096];

		msMaterial_GetDiffuseTexture(mtl, tmp, sizeof(tmp));

		char *mapName = tmp;
		mapName = FixName(mapName);
		int len = lstrlen(mapName) + 1;
		TCHAR * namp = new TCHAR[len];

		lstrcpy(namp, mapName);

		std::string s = namp;
		delete[] namp;
		s = s.substr(s.find_last_of('\\') + 1);
		s = s.substr(0, s.find_last_of('.'));
		if(!m_mTextures.count(s))
		{
			m_mTextures[s] = m_mTextures.size();
			//MessageBox(exp->hDlgWnd, s.c_str(), "", NULL);
		}
		
	}
}

std::string MtlList::GetTexture(msMaterial *mtl, int ind, CPlugin *exp)
{
	char tmp[4096];

	msMaterial_GetDiffuseTexture(mtl, tmp, sizeof(tmp));

	char *mapName = tmp;
	std::string s = FixName(mapName);
	s = s.substr(s.find_last_of('\\') + 1);
	s = s.substr(0, s.find_last_of('.'));
	return(s);
}

int MtlList::GetID(msMaterial *mtl)
{
	if(m_mList.count(mtl))
	{
		return(m_mList[mtl]);
	}
	else
	{
		return(-1);
	}
}

int MtlList::Count()
{
	return(m_mList.size());
}

int MtlList::TexCount()
{
	return(m_mTextures.size());
}
