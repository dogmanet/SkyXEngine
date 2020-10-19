#ifndef __EXPORTER_H
#define __EXPORTER_H

#include "stdafx.h"
#include "Extended.h"
#include <dseplugin/ModelFile.h>

#include "ExtendedPhys.h"
#include "ExtendedAnim.h"
#include "ExtendedSkin.h"
#include "ExtendedActs.h"
#include "ExtendedLods.h"

class CExporter;
class CActListener: public CExtendedActs::IListener
{
public:
	CActListener(CExporter *pExporter);
	void onCommitted() override;
	void onChanged() override;

private:
	CExporter *m_pExporter;
};

class CLodListener: public CExtendedLods::IListener
{
public:
	CLodListener(CExporter *pExporter);
	void onCommitted() override;

private:
	CExporter *m_pExporter;
};

class IProgress
{
public:
	virtual void setProgress() = 0;
};

class IExporterCallback
{
public:
	virtual bool canExportTB() = 0;

	virtual void prepare(IProgress *pProgress) = 0;

	virtual UINT getLayerCount() = 0;
	virtual const char* getLayerName(UINT uLayer) = 0;
	virtual UINT getLayerObjectCount(UINT uLayer) = 0;

	virtual float3 getObjectPosition(UINT uLayer, UINT uObject) = 0;
	virtual SMQuaternion getObjectRotation(UINT uLayer, UINT uObject) = 0;

	virtual UINT getObjectSubsetCount(UINT uLayer, UINT uObject) = 0;
	virtual const char* getObjectSubsetTexture(UINT uLayer, UINT uObject, UINT uSubset) = 0;

	virtual UINT getObjectSubsetVertexCount(UINT uLayer, UINT uObject, UINT uSubset) = 0;
	virtual UINT getObjectSubsetIndexCount(UINT uLayer, UINT uObject, UINT uSubset) = 0;

	virtual UINT* getObjectSubsetIndices(UINT uLayer, UINT uObject, UINT uSubset) = 0;

	virtual vertex_static* getObjectSubsetStaticVertices(UINT uLayer, UINT uObject, UINT uSubset) = 0;
	virtual vertex_static_ex* getObjectSubsetStaticExVertices(UINT uLayer, UINT uObject, UINT uSubset) = 0;
	virtual vertex_animated* getObjectSubsetAnimatedVertices(UINT uLayer, UINT uObject, UINT uSubset) = 0;
	virtual vertex_animated_ex* getObjectSubsetAnimatedExVertices(UINT uLayer, UINT uObject, UINT uSubset) = 0;

	// skeleton
	// animation
	// gibs
};

class CExporter
{
public:
	CExporter(const char *szFile, bool bSuppressPrompts, IExporterCallback *pCallback);
	~CExporter();

	int execute();

	void setFramerate(int iFramerate)
	{
		m_iDefaultFramerate = iFramerate;
	}
	void setEndFrame(int iFrame)
	{
		m_iEndFrame = iFrame;
	}

	void addTexture(const char *szName);

public:
	void onActsCommitted();
	void onActsChanged();
	void onLodsCommitted();

	void logWarning(const char *szFormat, ...)
	{
	}

private:
	
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void updateSectionMesh();
	void updateSectionAnim();
	void updateSectionGibs();

	void loadDefaults();

	void tryLoadModel();

	void loadPhysdata(FILE *pf);
	void loadMaterials(FILE *pf);
	void loadLoDs(FILE *pf);
	void loadDeps(FILE *pf);
	void loadActivities(FILE *pf);
	void loadBones(FILE *pf);
	void loadControllers(FILE *pf);
	void loadAnimations(FILE *pf);
	void loadHitboxes(FILE *pf);

	void writePhysdata(FILE *pf);
	void writeMaterials(FILE *pf);
	void writeLoDs(FILE *pf);
	void writeDeps(FILE *pf);
	void writeActivities(FILE *pf);
	void writeBones(FILE *pf);
	void writeControllers(FILE *pf);
	void writeAnimations(FILE *pf);
	void writeHitboxes(FILE *pf);

	void startExport();

	static void ProcessThread(void *p);
	void doExport();

	void updateLodList();

	void performRetopology();

	void clearLods();
	void clearPhysparts();

	bool checkSettings();

	void prepareFullMesh();

private:
	HWND m_hDlgWnd = NULL;
	CExtended *m_pExtended = NULL;

	const char *m_szFile = NULL;

	bool m_bSuppressPrompts = false;

	HWND m_hProgressBar = NULL;

	int m_iDefaultFramerate = 30;
	int m_iEndFrame = 1;

	IExporterCallback *m_pCallback = NULL;
private:

	CExtendedPhys *m_pExtPhys = NULL;
	CExtendedAnim *m_pExtAnim = NULL;
	CExtendedSkin *m_pExtSkin = NULL;
	CExtendedActs *m_pExtActs = NULL;
	CExtendedLods *m_pExtLods = NULL;

	CActListener *m_pExtActsListener = NULL;
	CLodListener *m_pExtLodListener = NULL;
private:
	ModelHeader m_hdr;
	ModelHeader2 m_hdr2;

	bool m_isLoaded = false;

	Array<ModelPhyspart> m_aPhysParts;

	Array<Array<String>> m_aasMaterials; // [skin][material]

	Array<ModelLoD> m_aLods;
	
	Array<ModelBoneName> m_aBones;

	Array<ModelBoneController> m_aControllers;

	Array<ModelHitbox> m_aHitboxes;

	Array<ModelPart> m_aParts;
};

#endif
