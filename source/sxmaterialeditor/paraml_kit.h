
#ifndef __PARAML_KIT_H
#define __PARAML_KIT_H

#include <common/string.h>
#include <common/array.h>
#include <core/sxcore.h>

class CParamLigthKit
{
public:
	CParamLigthKit();
	~CParamLigthKit();

	void load(const char *szPath);

	UINT getCount() const;
	const char* getName(ID id) const;
	float getThickness(ID id) const;
	float getRoughness(ID id) const;
	float getF0(ID id) const;

	ID find(float fThickness, float fRoughness, float fF0) const;

protected:

	struct CParamLigth
	{
		String m_sName;
		float m_fThickness;
		float m_fRoughness;
		float m_fF0;
	};

	Array<CParamLigth*> m_aParameters;
};

#endif