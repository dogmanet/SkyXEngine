
#ifndef __SHADERS_KIT_H
#define __SHADERS_KIT_H

#include <common/string.h>
#include <common/array.h>
#include <core/sxcore.h>

class CShadersKit
{
public:
	CShadersKit();
	~CShadersKit();

	void load(const char *szPath);

	UINT getCount() const;
	const char* getName(ID id) const;
	const char* getVS(ID id) const;
	const char* getPS(ID id) const;

	ID find(const char *m_szVS, const char *m_szPS) const;

protected:

	struct CShaders
	{
		String m_sName;
		String m_sVS;
		String m_sPS;
	};

	Array<CShaders*> m_aShaders;
};

#endif