#include "stdafx.h"
#include <process.h>

#include "Exporter.h"

CActListener::CActListener(CExporter *pExporter):
	m_pExporter(pExporter)
{
}
void CActListener::onCommitted()
{
	m_pExporter->onActsCommitted();
}
void CActListener::onChanged()
{
	m_pExporter->onActsChanged();
}


CLodListener::CLodListener(CExporter *pExporter):
	m_pExporter(pExporter)
{
}
void CLodListener::onCommitted()
{
	m_pExporter->onLodsCommitted();
}


CExporter::CExporter(const char *szFile, bool bSuppressPrompts, IExporterCallback *pCallback):
	m_szFile(szFile),
	m_bSuppressPrompts(bSuppressPrompts),
	m_pCallback(pCallback)
{
	m_pExtPhys = new CExtendedPhys();
	m_pExtAnim = new CExtendedAnim();
	m_pExtSkin = new CExtendedSkin();
	m_pExtActs = new CExtendedActs();
	m_pExtLods = new CExtendedLods();

	m_pExtActsListener = new CActListener(this);
	m_pExtActs->addListener(m_pExtActsListener);

	m_pExtLodListener = new CLodListener(this);
	m_pExtLods->addListener(m_pExtLodListener);

	memset(&m_hdr, 0, sizeof(m_hdr));
	memset(&m_hdr2, 0, sizeof(m_hdr2));
	tryLoadModel();

	m_hdr.iFlags |= MODEL_FLAG_COMPILED;
	m_hdr.iFlags |= MODEL_FLAG_NEW_STYLE_DEPS;
	m_hdr.iFlags |= MODEL_FLAG_NEW_PACKED_ANIMS;
	m_hdr.iFlags |= MODEL_FLAG_NORMALIZED_NORMALS;
}

CExporter::~CExporter()
{
	m_pExtActs->removeListener(m_pExtActsListener);
	mem_delete(m_pExtActsListener);

	m_pExtLods->removeListener(m_pExtLodListener);
	mem_delete(m_pExtLodListener);

	mem_delete(m_pExtended);

	clearPhysparts();

	clearLods();
}

int CExporter::execute()
{
	return(DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), GetActiveWindow(), DlgProc, (LPARAM)this));
}

INT_PTR CALLBACK CExporter::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CExporter *pThis = (CExporter*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(msg == WM_INITDIALOG)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)lParam);
		pThis = (CExporter*)lParam;
		pThis->m_hDlgWnd = hWnd;
	}
	if(pThis)
	{
		return(pThis->dlgProc(hWnd, msg, wParam, lParam));
	}
	return(FALSE);
}

INT_PTR CALLBACK CExporter::dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND ed1 = NULL;
	HWND ed2 = NULL;
	switch(msg)
	{
	case WM_INITDIALOG:
		m_hProgressBar = GetDlgItem(m_hDlgWnd, IDC_PROGRESS1);
		//SendMessage(m_hProgressBar, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 1000));
		//SendMessage(m_hProgressBar, PBM_SETSTEP, (WPARAM)1, 0);
		{
			RECT rc;
			GetWindowRect(hWnd, &rc);

			POINT pt;
			pt.x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
			pt.y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;

			MoveWindow(hWnd, pt.x, pt.y, rc.right - rc.left, rc.bottom - rc.top, FALSE);

		}

		m_pExtended = new CExtended(m_hDlgWnd);
		m_pExtended->addCustomTab(m_pExtPhys);
		m_pExtended->addCustomTab(m_pExtAnim);
		m_pExtended->addCustomTab(m_pExtSkin);
		m_pExtended->addCustomTab(m_pExtActs);
		m_pExtended->addCustomTab(m_pExtLods);

		loadDefaults();

		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			startExport();
			break;

		case IDC_EXPORT_MESH:
		case IDC_MESH_FULL:
		case IDC_MESH_STATIC:
			updateSectionMesh();
			break;

		case IDC_EXPORT_ANIMATION:
			updateSectionAnim();
			break;

		case IDC_EXPORT_GIBS:
			updateSectionGibs();
			break;

		case IDC_EXTENDED:
			m_pExtended->show();
			break;

		case IDCANCEL:
			EndDialog(hWnd, 0);
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(m_hDlgWnd, 0);
		return(TRUE);
	default:
		return(FALSE);
	}
	return(TRUE);
}

void CExporter::updateSectionMesh()
{
	BOOL isChecked = IsDlgButtonChecked(m_hDlgWnd, IDC_EXPORT_MESH);
	BOOL isFull = IsDlgButtonChecked(m_hDlgWnd, IDC_MESH_FULL);
	BOOL isStatic = FALSE;
	if(m_isLoaded && (m_hdr.iFlags & MODEL_FLAG_STATIC))
	{
		isStatic = TRUE;
	}

	if(!isFull && m_isLoaded)
	{
		CheckDlgButton(m_hDlgWnd, IDC_MESH_TBN, (m_hdr.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM));
	}

	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_MESH_STATIC), isChecked && !isStatic);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_MESH_TBN), isChecked && (isFull || !m_isLoaded));
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_MESH_FULL), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_MESH_LOD), isChecked && !isFull);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_MESH_LOD_ADD), isChecked && !isFull);

	updateSectionAnim();
}

void CExporter::updateSectionAnim()
{
	BOOL isChecked = IsDlgButtonChecked(m_hDlgWnd, IDC_EXPORT_ANIMATION);
	BOOL isStatic = IsDlgButtonChecked(m_hDlgWnd, IDC_EXPORT_MESH) && IsDlgButtonChecked(m_hDlgWnd, IDC_MESH_STATIC);

	if(m_isLoaded && (m_hdr.iFlags & MODEL_FLAG_STATIC))
	{
		isStatic = TRUE;
	}
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_EXPORT_ANIMATION), !isStatic);

	if(isStatic)
	{
		isChecked = FALSE;
	}

	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_FROM), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_TO), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_SPEED), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_LOOP), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_NAME), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_NAME_ADD), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_ACTIVITY), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_ACTIVITY_ADD), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_ANIMATION_ACTIVITY_CHANCE), isChecked);
}

void CExporter::updateSectionGibs()
{
	BOOL isChecked = IsDlgButtonChecked(m_hDlgWnd, IDC_EXPORT_GIBS);

	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_GIB_MARKED), isChecked);
	EnableWindow(GetDlgItem(m_hDlgWnd, IDC_GIB_OBJECT), isChecked);
}

void CExporter::loadDefaults()
{
	CheckDlgButton(m_hDlgWnd, IDC_MESH_TBN, !m_isLoaded || (m_hdr.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM));
	CheckDlgButton(m_hDlgWnd, IDC_GIB_OBJECT, TRUE);
	CheckDlgButton(m_hDlgWnd, IDC_MESH_FULL, TRUE);
	CheckDlgButton(m_hDlgWnd, IDC_MESH_STATIC, m_isLoaded && (m_hdr.iFlags & MODEL_FLAG_STATIC));

	char tmp[32];
	sprintf(tmp, "%d", m_iDefaultFramerate);
	SetDlgItemText(m_hDlgWnd, IDC_ANIMATION_FROM, "0");
	SetDlgItemText(m_hDlgWnd, IDC_ANIMATION_SPEED, tmp);
	sprintf(tmp, "%d", m_iEndFrame);
	SetDlgItemText(m_hDlgWnd, IDC_ANIMATION_TO, tmp);

	updateSectionAnim();

	updateLodList();
}

void CExporter::tryLoadModel()
{
	FILE *pf = fopen(m_szFile, "rb");
	if(!pf)
	{
		return;
	}

	memset(&m_hdr, 0, sizeof(m_hdr));
	memset(&m_hdr2, 0, sizeof(m_hdr2));
	fread(&m_hdr, sizeof(m_hdr), 1, pf);

	if(m_hdr.Magick != SX_MODEL_MAGICK)
	{
		MessageBox(m_hDlgWnd, "Corrupt model. Invalid magick", "Load error", MB_OK | MB_ICONSTOP);
		return;
	}

	if(m_hdr.iVersion != SX_MODEL_VERSION)
	{
		MessageBox(m_hDlgWnd, "Invalid version", "Load error", MB_OK | MB_ICONSTOP);
		return;
	}

	if(m_hdr.iSecondHeaderOffset)
	{
		_fseeki64(pf, m_hdr.iSecondHeaderOffset, SEEK_SET);
		fread(&m_hdr2, sizeof(m_hdr2), 1, pf);
	}

	loadPhysdata(pf);
	loadMaterials(pf);
	loadLoDs(pf);
	loadDeps(pf);
	loadActivities(pf);
	loadBones(pf);
	loadControllers(pf);
	loadAnimations(pf);
	loadHitboxes(pf);

	m_hdr.iFlags &= ~MODEL_FLAG_SOURCED;
	m_hdr.iFlags &= ~MODEL_FLAG_SOURCE_ANIMATION;
	m_hdr.iFlags &= ~MODEL_FLAG_SOURCE_MESH;

	m_isLoaded = true;

	fclose(pf);
}

void CExporter::loadPhysdata(FILE *pf)
{
	if(m_hdr2.iPhysicsDataOffset)
	{
		_fseeki64(pf, m_hdr2.iPhysicsDataOffset, SEEK_SET);

		ModelPhysData physData = {};
		fread(&physData, MODEL_PHYSDATA_STRUCT_SIZE, 1, pf);
		_fseeki64(pf, physData.iPartsOffset, SEEK_SET);

		m_pExtPhys->setWeight(physData.fWeight);

		m_aPhysParts.resize(physData.iPhyspartCount);
		for(UINT i = 0; i < physData.iPhyspartCount; ++i)
		{
			fread(&m_aPhysParts[i], MODEL_PHYSPART_STRUCT_SIZE, 1, pf);
		}

		for(UINT i = 0; i < physData.iPhyspartCount; ++i)
		{
			switch(m_aPhysParts[i].type)
			{
			case HT_CONVEX:
				{
					_fseeki64(pf, m_aPhysParts[i].iDataOffset, SEEK_SET);

					ModelPhyspartDataConvex *pData = new ModelPhyspartDataConvex();
					m_aPhysParts[i].pData = pData;
					fread(m_aPhysParts[i].pData, MODEL_PHYSPART_DATA_CONVEX_STRUCT_SIZE, 1, pf);

					pData->pVerts = new float3_t[pData->iVertCount];
					fread(pData->pVerts, sizeof(float3_t), pData->iVertCount, pf);
				}
				break;
			}
		}
		
		m_hdr.iFlags |= MODEL_FLAG_PHYSIC;
		if(physData.iPhyspartCount)
		{
			m_hdr.iFlags |= MODEL_FLAG_HAS_PHYSBOX;
		}
	}
}
void CExporter::loadMaterials(FILE *pf)
{
	if(m_hdr.iMaterialsOffset)
	{
		_fseeki64(pf, m_hdr.iMaterialsOffset, SEEK_SET);

		char szName[MODEL_MAX_NAME];

		for(UINT j = 0; j < m_hdr.iSkinCount; j++)
		{
			for(uint32_t i = 0; i < m_hdr.iMaterialCount; i++)
			{
				fread(szName, sizeof(char), MODEL_MAX_NAME, pf);

				m_aasMaterials[j][i] = szName;

				if(j == 0)
				{
					m_pExtSkin->addColumn(szName);
				}
				else
				{
					m_pExtSkin->addOverride(j - 1, i, szName);
				}
			}
		}
	}
}
void CExporter::loadLoDs(FILE *pf)
{
	if(m_hdr.iLODcount && m_hdr.iLODoffset)
	{
		_fseeki64(pf, m_hdr.iLODoffset, SEEK_SET);

		for(uint32_t i = 0; i < m_hdr.iLODcount; i++)
		{
			ModelLoD lod;
			fread(&lod, MODEL_LOD_STRUCT_SIZE, 1, pf);

			UINT uTris = 0;

			lod.pSubLODmeshes = new ModelLoDSubset[lod.iSubMeshCount];
			for(uint32_t j = 0; j < lod.iSubMeshCount; j++)
			{
				ModelLoDSubset &lodSubset = lod.pSubLODmeshes[j];
				fread(&lodSubset.iMaterialID, sizeof(uint32_t), 1, pf);
				fread(&lodSubset.iVectexCount, sizeof(uint32_t), 1, pf);
				fread(&lodSubset.iIndexCount, sizeof(uint32_t), 1, pf);

				size_t elSize = 0;
				if(m_hdr.iFlags & MODEL_FLAG_STATIC)
				{
					if(m_hdr.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
					{
						elSize = sizeof(vertex_static_ex);
						lodSubset.pVertices = new vertex_static_ex[lodSubset.iVectexCount];
					}
					else
					{
						elSize = sizeof(vertex_static);
						lodSubset.pVertices = new vertex_static[lodSubset.iVectexCount];
					}
				}
				else
				{
					if(m_hdr.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
					{
						elSize = sizeof(vertex_animated_ex);
						lodSubset.pVertices = new vertex_animated_ex[lodSubset.iVectexCount];
					}
					else
					{
						elSize = sizeof(vertex_animated);
						lodSubset.pVertices = new vertex_animated[lodSubset.iVectexCount];
					}
				}
				lodSubset.pIndices = new UINT[lodSubset.iIndexCount];
				fread(lodSubset.pVertices, elSize, lodSubset.iVectexCount, pf);
				fread(lodSubset.pIndices, sizeof(UINT), lodSubset.iIndexCount, pf);

				if(!(m_hdr.iFlags & MODEL_FLAG_NORMALIZED_NORMALS))
				{
					for(UINT k = 0; k < lodSubset.iVectexCount; ++k)
					{
						lodSubset.pVertices[k].Norm = SMVector3Normalize(lodSubset.pVertices[k].Norm);
					}
					m_hdr.iFlags |= MODEL_FLAG_NORMALIZED_NORMALS;
				}

				switch(m_hdr2.topology)
				{
				case MDLPT_TRIANGLELIST:
					uTris += lodSubset.iIndexCount / 3;
					break;
				case MDLPT_TRIANGLESTRIP:
					uTris += lodSubset.iIndexCount - 2;
					break;
				}
			}

			m_aLods.push_back(lod);
			m_pExtLods->addLod(uTris, lod.iSubMeshCount);
		}
	}
}
void CExporter::loadDeps(FILE *pf)
{
	if(m_hdr2.iDepsCount && m_hdr2.iDependensiesOffset)
	{
		_fseeki64(pf, m_hdr2.iDependensiesOffset, SEEK_SET);

		if(m_hdr.iFlags & MODEL_FLAG_NEW_STYLE_DEPS)
		{
			m_aParts.resize(m_hdr2.iDepsCount);
			fread(m_aParts, sizeof(ModelPart), m_hdr2.iDepsCount, pf);
		}
		else
		{
			ModelDependensy md;
			for(uint32_t i = 0; i < m_hdr2.iDepsCount; ++i)
			{
				fread(&md, sizeof(ModelDependensy), 1, pf);
				ModelPart pt = {};
				strcpy(pt.name, md.szName);
				strcpy(pt.file, md.szName);
				pt.attachDesc.type = MA_SKIN;
				pt.uFlags = MP_COLLISIONS | MP_RAYTRACE | MP_IMPORTED;
				pt.uImportFlags = MI_ALL;
			}
		}
	}
}
void CExporter::loadActivities(FILE *pf)
{
	if(m_hdr2.iActivitiesTableCount && m_hdr2.iActivitiesTableOffset)
	{
		_fseeki64(pf, m_hdr2.iActivitiesTableOffset, SEEK_SET);

		ModelActivity ma = {};
		for(UINT i = 0, l = m_hdr2.iActivitiesTableCount; i < l; ++i)
		{
			fread(&ma, sizeof(ModelActivity), 1, pf);
			m_pExtActs->addActivity(ma.szName);
		}
	}
}
void CExporter::loadBones(FILE *pf)
{
	if(m_hdr2.iBoneTableOffset)
	{
		_fseeki64(pf, m_hdr2.iBoneTableOffset, SEEK_SET);
		m_aBones.resize(m_hdr2.iBoneTableCount);
		fread(m_aBones, sizeof(ModelBoneName), m_hdr2.iBoneTableCount, pf);
	}
}
void CExporter::loadControllers(FILE *pf)
{
	if(m_hdr2.iControllersCount && m_hdr2.iControllersOffset)
	{
		_fseeki64(pf, m_hdr2.iControllersOffset, SEEK_SET);
		m_aControllers.resize(m_hdr2.iControllersOffset);
		fread(m_aControllers, sizeof(ModelBoneController), m_hdr2.iControllersCount, pf);
	}
}
void CExporter::loadAnimations(FILE *pf)
{
	if(m_hdr.iAnimationsOffset)
	{
		_fseeki64(pf, m_hdr.iAnimationsOffset, SEEK_SET);

		ModelSequence ms = {0};
		for(UINT i = 0; i < m_hdr.iAnimationCount; ++i)
		{
			if(m_hdr.iFlags & MODEL_FLAG_NEW_PACKED_ANIMS)
			{
				fread(&ms, MODEL_SEQUENCE_STRUCT_SIZE, 1, pf);
			}
			else
			{
				fread(&ms, MODEL_SEQUENCE_STRUCT_SIZE_OLD, 1, pf);
			}

			ms.m_vmAnimData = new ModelBone*[ms.iNumFrames];
			
			ModelBone mb;
			for(UINT j = 0; j < ms.iNumFrames; ++j)
			{
				ms.m_vmAnimData[j] = new ModelBone[m_hdr2.iBoneTableCount];
				fread(ms.m_vmAnimData[j], sizeof(ModelBone), m_hdr2.iBoneTableCount, pf);
			}

			m_pExtAnim->addSequence(ms);
		}
	}
}
void CExporter::loadHitboxes(FILE *pf)
{
	if(m_hdr2.iHitboxCount && m_hdr2.iHitboxesOffset)
	{
		_fseeki64(pf, m_hdr2.iHitboxesOffset, SEEK_SET);
		m_aHitboxes.resize(m_hdr2.iHitboxCount);
		fread(m_aHitboxes, sizeof(ModelHitbox), m_hdr2.iHitboxCount, pf);
	}
}

void CExporter::startExport()
{
	if(!checkSettings())
	{
		return;
	}

	EnumChildWindows(m_hDlgWnd, [](HWND hWnd, LPARAM lParam)
	{
		EnableWindow(hWnd, FALSE);
		return(TRUE);
	}, NULL);

	_beginthread(ProcessThread, 0, this);
}

void CExporter::ProcessThread(void *p)
{
	CExporter *exp = (CExporter*)p;
	exp->doExport();
}

void CExporter::doExport()
{
	if(IsDlgButtonChecked(m_hDlgWnd, IDC_EXPORT_MESH))
	{
		performRetopology();
		m_hdr2.topology = MDLPT_TRIANGLELIST;

		bool isStatic = IsDlgButtonChecked(m_hDlgWnd, IDC_MESH_STATIC);
		bool includeTB = IsDlgButtonChecked(m_hDlgWnd, IDC_MESH_TBN);
		bool isFullExport = IsDlgButtonChecked(m_hDlgWnd, IDC_MESH_FULL);

		if(isFullExport)
		{
			clearLods();
			clearPhysparts();

			// export by layers
		}
		else
		{
			// parse lod for export
			// export ignore layers
		}
	}
	if(IsDlgButtonChecked(m_hDlgWnd, IDC_EXPORT_ANIMATION))
	{
	}
	if(IsDlgButtonChecked(m_hDlgWnd, IDC_EXPORT_GIBS))
	{
	}

	FILE *pf = fopen(m_szFile, "wb");
	if(!pf)
	{
		MessageBox(m_hDlgWnd, "Unable to write file!", "Save error", MB_OK | MB_ICONSTOP);
		PostMessage(m_hDlgWnd, WM_CLOSE, 0, NULL);
		return;
	}
	m_hdr.Magick = SX_MODEL_MAGICK;
	m_hdr.iVersion = SX_MODEL_VERSION;
	m_hdr2.iBboxInfoOffset = 0;
	fwrite(&m_hdr, sizeof(m_hdr), 1, pf);

	m_hdr.iSecondHeaderOffset = _ftelli64(pf);
	fwrite(&m_hdr2, sizeof(m_hdr2), 1, pf);

	writePhysdata(pf);
	writeMaterials(pf);
	writeLoDs(pf);
	writeDeps(pf);
	writeActivities(pf);
	writeBones(pf);
	writeControllers(pf);
	writeAnimations(pf);
	writeHitboxes(pf);

	_fseeki64(pf, 0, SEEK_SET);
	fwrite(&m_hdr, sizeof(m_hdr), 1, pf);
	fwrite(&m_hdr2, sizeof(m_hdr2), 1, pf);

	fclose(pf);

	PBM_GETRANGE;
	PBRANGE rng;
	SendMessage(m_hProgressBar, PBM_GETRANGE, FALSE, (LPARAM)&rng);
	SendMessage(m_hProgressBar, PBM_SETPOS, (WPARAM)rng.iHigh, 0);

	if(!m_bSuppressPrompts)
	{
		MessageBox(m_hDlgWnd, "Export done!", "DSE Model export", NULL);
	}

	PostMessage(m_hDlgWnd, WM_CLOSE, 0, NULL);
}

void CExporter::writePhysdata(FILE *pf)
{
	m_hdr2.iPhysicsDataOffset = _ftelli64(pf);

	ModelPhysData physData = {};
	physData.fWeight = m_pExtPhys->getWeight();
	physData.iPhyspartCount = m_aPhysParts.size();
	physData.iPartsOffset = m_hdr2.iPhysicsDataOffset + MODEL_PHYSDATA_STRUCT_SIZE;
	fwrite(&physData, MODEL_PHYSDATA_STRUCT_SIZE, 1, pf);

	size_t uBaseOffset = physData.iPartsOffset + MODEL_PHYSPART_STRUCT_SIZE * physData.iPhyspartCount;

	for(UINT i = 0; i < physData.iPhyspartCount; ++i)
	{
		switch(m_aPhysParts[i].type)
		{
		case HT_CONVEX:
			{
				m_aPhysParts[i].iDataOffset = uBaseOffset;
				uBaseOffset += MODEL_PHYSPART_DATA_CONVEX_STRUCT_SIZE;
				ModelPhyspartDataConvex *pData = (ModelPhyspartDataConvex*)m_aPhysParts[i].pData;
				uBaseOffset += sizeof(float3_t) * pData->iVertCount;
			}
			break;
		}
	}

	for(UINT i = 0; i < physData.iPhyspartCount; ++i)
	{
		fwrite(&m_aPhysParts[i], MODEL_PHYSPART_STRUCT_SIZE, 1, pf);
	}

	for(UINT i = 0; i < physData.iPhyspartCount; ++i)
	{
		switch(m_aPhysParts[i].type)
		{
		case HT_CONVEX:
			{
				fwrite(m_aPhysParts[i].pData, MODEL_PHYSPART_DATA_CONVEX_STRUCT_SIZE, 1, pf);

				ModelPhyspartDataConvex *pData = (ModelPhyspartDataConvex*)m_aPhysParts[i].pData;

				fwrite(pData->pVerts, sizeof(float3_t), pData->iVertCount, pf);
			}
			break;
		}
	}

	m_hdr.iFlags |= MODEL_FLAG_PHYSIC;
	if(physData.iPhyspartCount)
	{
		m_hdr.iFlags |= MODEL_FLAG_HAS_PHYSBOX;
	}
}
void CExporter::writeMaterials(FILE *pf)
{
	m_hdr.iMaterialsOffset = _ftelli64(pf);
	char szName[MODEL_MAX_NAME];

	m_hdr.iSkinCount = m_pExtSkin->getRowCount() + 1;

	for(UINT j = 0; j < m_hdr.iSkinCount; j++)
	{
		for(uint32_t i = 0; i < m_hdr.iMaterialCount; i++)
		{
			strncpy(szName, j == 0 ? m_aasMaterials[j][i].c_str() : m_pExtSkin->getCell(j - 1, i), MODEL_MAX_NAME);
			szName[MODEL_MAX_NAME - 1] = 0;
			fwrite(szName, sizeof(char), MODEL_MAX_NAME, pf);
		}
	}
}
void CExporter::writeLoDs(FILE *pf)
{
	m_hdr.iLODoffset = _ftelli64(pf);

	m_hdr.iLODcount = m_aLods.size();

	for(uint32_t i = 0; i < m_hdr.iLODcount; i++)
	{
		ModelLoD &lod = m_aLods[i];
		fwrite(&lod, MODEL_LOD_STRUCT_SIZE, 1, pf);

		for(uint32_t j = 0; j < lod.iSubMeshCount; j++)
		{
			ModelLoDSubset &lodSubset = lod.pSubLODmeshes[j];
			fwrite(&lodSubset.iMaterialID, sizeof(uint32_t), 1, pf);
			fwrite(&lodSubset.iVectexCount, sizeof(uint32_t), 1, pf);
			fwrite(&lodSubset.iIndexCount, sizeof(uint32_t), 1, pf);

			size_t elSize = 0;
			if(m_hdr.iFlags & MODEL_FLAG_STATIC)
			{
				if(m_hdr.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
				{
					elSize = sizeof(vertex_static_ex);
				}
				else
				{
					elSize = sizeof(vertex_static);
				}
			}
			else
			{
				if(m_hdr.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
				{
					elSize = sizeof(vertex_animated_ex);
				}
				else
				{
					elSize = sizeof(vertex_animated);
				}
			}
			fwrite(lodSubset.pVertices, elSize, lodSubset.iVectexCount, pf);
			fwrite(lodSubset.pIndices, sizeof(UINT), lodSubset.iIndexCount, pf);
		}

	}
}
void CExporter::writeDeps(FILE *pf)
{
	m_hdr2.iDependensiesOffset = _ftelli64(pf);
	m_hdr2.iDepsCount = m_aParts.size();

	fwrite(m_aParts, sizeof(ModelPart), m_hdr2.iDepsCount, pf);
}
void CExporter::writeActivities(FILE *pf)
{
	m_hdr2.iActivitiesTableOffset = _ftelli64(pf);
	m_hdr2.iActivitiesTableCount = m_pExtActs->getList().size();

	auto &list = m_pExtActs->getList();
	ModelActivity ma;
	for(UINT i = 0; i < m_hdr2.iActivitiesTableCount; ++i)
	{
		strncpy(ma.szName, list[i].c_str(), MODEL_MAX_NAME);
		ma.szName[MODEL_MAX_NAME - 1] = 0;
		fwrite(&ma, sizeof(ModelActivity), 1, pf);
	}
}
void CExporter::writeBones(FILE *pf)
{
	m_hdr2.iBoneTableOffset = _ftelli64(pf);
	m_hdr2.iBoneTableCount = m_aBones.size();
	fwrite(m_aBones, sizeof(ModelBoneName), m_hdr2.iBoneTableCount, pf);
}
void CExporter::writeControllers(FILE *pf)
{
	m_hdr2.iControllersOffset = _ftelli64(pf);
	m_hdr2.iControllersCount = m_aControllers.size();
	fwrite(m_aControllers, sizeof(ModelBoneController), m_hdr2.iControllersCount, pf);
}
void CExporter::writeAnimations(FILE *pf)
{
	m_hdr.iAnimationsOffset = _ftelli64(pf);
	m_hdr.iAnimationCount = m_pExtAnim->getSequenceCount();

	for(UINT i = 0; i < m_hdr.iAnimationCount; ++i)
	{
		const ModelSequence &ms = m_pExtAnim->getSequence(i);
		fwrite(&ms, MODEL_SEQUENCE_STRUCT_SIZE, 1, pf);

		ModelBone mb;
		for(UINT j = 0; j < ms.iNumFrames; ++j)
		{
			fwrite(ms.m_vmAnimData[j], sizeof(ModelBone), m_hdr2.iBoneTableCount, pf);
		}
	}
}
void CExporter::writeHitboxes(FILE *pf)
{
	m_hdr2.iHitboxesOffset = _ftelli64(pf);
	m_hdr2.iHitboxCount = m_aHitboxes.size();
	fwrite(m_aHitboxes, sizeof(ModelHitbox), m_hdr2.iHitboxCount, pf);
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


void CExporter::addTexture(const char *szName)
{
	char *mapName = FixName(szName);
	int iDotPos = -1;
	char *tmp = mapName;
	while(*tmp)
	{
		if(*tmp == '.')
		{
			iDotPos = tmp - mapName;
		}
		++tmp;
	}

	if(iDotPos >= 0)
	{
		mapName[iDotPos] = 0;
	}

	const char *szBaseName = basename(mapName);

	for(uint32_t i = 0; i < m_hdr.iMaterialCount; i++)
	{
		if(!fstrcmp(m_aasMaterials[0][i].c_str(), szBaseName))
		{
			return;
		}
	}

	for(UINT j = 0; j < m_hdr.iSkinCount; j++)
	{
		m_aasMaterials[j][m_hdr.iMaterialCount] = j == 0 ? szBaseName : "";
	}
	m_pExtSkin->addColumn(szBaseName);
	++m_hdr.iMaterialCount;
}

void CExporter::onActsCommitted()
{
	HWND hCombo = GetDlgItem(m_hDlgWnd, IDC_ANIMATION_ACTIVITY);

	ComboBox_ResetContent(hCombo);

	ComboBox_AddString(hCombo, "");
	auto &list = m_pExtActs->getList();
	for(UINT i = 0, l = list.size(); i < l; ++i)
	{
		ComboBox_AddString(hCombo, list[i].c_str());
	}

	m_pExtAnim->setActivityList(m_pExtActs->getList());
}

void CExporter::onActsChanged()
{
	m_pExtAnim->setActivityList(m_pExtActs->getUncommittedList());
}

void CExporter::onLodsCommitted()
{
	UINT uCount = m_pExtLods->getCount();

	Array<ModelLoD> aNewLods(uCount);
	for(UINT i = 0; i < uCount; ++i)
	{
		int idx = m_pExtLods->getItem(i);
		aNewLods.push_back(m_aLods[idx]);
		m_aLods[idx].pSubLODmeshes = NULL;
	}

	for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
	{
		ModelLoD &lod = m_aLods[i];
		if(lod.pSubLODmeshes)
		{
			for(UINT j = 0; j < lod.iSubMeshCount; ++j)
			{
				mem_delete_a(lod.pSubLODmeshes[j].pIndices);
				mem_delete_a(lod.pSubLODmeshes[j].pVertices);
			}
			mem_delete_a(lod.pSubLODmeshes);
		}
	}

	m_aLods = aNewLods;

	updateLodList();
}

void CExporter::updateLodList()
{
	HWND hCombo = GetDlgItem(m_hDlgWnd, IDC_MESH_LOD);

	ComboBox_ResetContent(hCombo);

	ComboBox_AddString(hCombo, "Physbox");
	char tmp[32];
	for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
	{
		sprintf(tmp, "Lod #%u", i);
		ComboBox_AddString(hCombo, tmp);
	}
	ComboBox_AddString(hCombo, "New lod");
}

void CExporter::performRetopology()
{
	if(m_hdr2.topology == MDLPT_TRIANGLESTRIP)
	{
		m_hdr2.topology = MDLPT_TRIANGLELIST;

		for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
		{
			ModelLoD &lod = m_aLods[i];
			for(UINT j = 0; j < lod.iSubMeshCount; ++j)
			{
				UINT uNewIndexCount = (lod.pSubLODmeshes[j].iIndexCount - 2) * 3;
				UINT *pNewIndices = new UINT[uNewIndexCount];

				UINT uIdx = 0;
				for(UINT k = 0, kl = lod.pSubLODmeshes[j].iIndexCount - 2; k < kl;  ++k)
				{
					pNewIndices[uIdx++] = lod.pSubLODmeshes[j].pIndices[k];
					pNewIndices[uIdx++] = lod.pSubLODmeshes[j].pIndices[k + 1];
					pNewIndices[uIdx++] = lod.pSubLODmeshes[j].pIndices[k + 2];
				}

				assert(uIdx == uNewIndexCount);

				mem_delete_a(lod.pSubLODmeshes[j].pIndices);
				lod.pSubLODmeshes[j].pIndices = pNewIndices;
				lod.pSubLODmeshes[j].iIndexCount = uNewIndexCount;
			}
		}
	}
}

void CExporter::clearLods()
{
	for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
	{
		ModelLoD &lod = m_aLods[i];
		for(UINT j = 0; j < lod.iSubMeshCount; ++j)
		{
			mem_delete_a(lod.pSubLODmeshes[j].pIndices);
			mem_delete_a(lod.pSubLODmeshes[j].pVertices);
		}
		mem_delete_a(lod.pSubLODmeshes);
	}
	m_aLods.clearFast();
}

void CExporter::clearPhysparts()
{
	for(UINT i = 0, l = m_aPhysParts.size(); i < l; ++i)
	{
		switch(m_aPhysParts[i].type)
		{
		case HT_CONVEX:
			{
				ModelPhyspartDataConvex *pData = (ModelPhyspartDataConvex*)m_aPhysParts[i].pData;
				mem_delete_a(pData->pVerts);
				mem_delete(pData);
			}
			break;
		}
	}
	m_aPhysParts.clearFast();
}

bool CExporter::checkSettings()
{
	if(IsDlgButtonChecked(m_hDlgWnd, IDC_EXPORT_MESH) && !IsDlgButtonChecked(m_hDlgWnd, IDC_MESH_FULL))
	{
		char tmp[32];
		GetDlgItemText(m_hDlgWnd, IDC_MESH_LOD, tmp, sizeof(tmp));

		if(!tmp[0])
		{
			MessageBox(m_hDlgWnd, "Please select LOD", "Error", MB_OK | MB_ICONSTOP);
			return(false);
		}
	}

	return(true);
}
