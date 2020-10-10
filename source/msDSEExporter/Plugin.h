#ifndef __PLUGIN_H
#define __PLUGIN_H

#include "stdafx.h"

#include <msLib/msPlugIn.h>
#include <msLib/msLib.h>

#include <vector>
#include <map>

#define SXPROP_EXP_GIBS "SXPROP/EXP_GIBS"
#define SXPROP_EXP_TABI "SXPROP/EXP_TABI"
#define SXPROP_EXP_STAT "SXPROP/EXP_STAT"
#define SXPROP_EXP_WGHT "SXPROP/EXP_WGHT"
#define SXPROP_EXP_SCNT "SXPROP/EXP_SCNT"
#define SXPROP_EXP_SKN_PREF "SXPROP/SKN/"

class CPlugin;
class MtlList
{
public:
	void Add(msMaterial *mtl, CPlugin *exp);
	int GetID(msMaterial *mtl);
	int Count();
	std::string GetTexture(msMaterial *mtl, int ind, CPlugin *exp);

	int TexCount();

	std::map<msMaterial*, int> m_mList;
	std::map<std::string, int> m_mTextures;
};

class CPlugin: public cMsPlugIn
{
	friend INT_PTR CALLBACK ExportDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CPlugin();
	~CPlugin();

    int GetType();
    const char* GetTitle();
    int Execute(msModel *pModel);

private:
	HWND m_hProgressBar = NULL;
	HWND m_hDlgWnd = NULL;

	msModel *m_pMdl = NULL;

	bool m_bExportMesh = true;
	bool m_bExportStatic = true;
	bool m_bExportGibs = false;
	bool m_bSkipInitIGame = false;
	bool m_bExportTaBi = true;

	int m_iStartFrame = 0;
	int m_iEndFrame = 1;

	float m_fWeight = 1.0f;

	static void ProcessThread(void *p);

	int m_iProgress = 0;

	bool m_bSuppressPrompts = false;

	msMesh *m_pOnlyMesh = NULL;

	MtlList m_mList;

protected:
	std::vector<std::string> getListTextures();

	bool getConfigBool(const char *szKey, bool bDefault = false);
	void setConfigBool(const char *szKey, bool bvalue);

	float getConfigFloat(const char *szKey, float fDefault = 0.0f);
	void setConfigFloat(const char *szKey, float fvalue);

	int getConfigInt(const char *szKey, int iDefault = 0);
	void setConfigInt(const char *szKey, int ivalue);

	void getConfigWStr(const char *szKey, char *szOut, int iMaxOut);
	void setConfigWStr(const char *szKey, const char *szValue);
};

#endif
