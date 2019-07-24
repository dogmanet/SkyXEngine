#ifndef __SETTINGS_WRITER_H
#define __SETTINGS_WRITER_H

#include <gdefines.h>
#include <common/assotiativearray.h>
#include <common/string.h>
#include <xcommon/IFileSystem.h>

class CSettingsWriter
{
public:
	CSettingsWriter(IFileSystem *pFileSystem);

	void loadFile(const char *szName);
	void saveFile(const char *szFile);

	void setCVar(const char *szKey, const char *szValue);
	void setBind(const char *szKey, const char *szCmd);
	void unBind(const char *szKey);

protected:
	IFileSystem *m_pFileSystem;
	AssotiativeArray<String, String> m_mCvarList;
	AssotiativeArray<String, String> m_mBindList;
};

#endif
