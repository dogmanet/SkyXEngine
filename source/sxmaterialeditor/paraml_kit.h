
#ifndef __PARAML_KIT_H
#define __PARAML_KIT_H

#include <common/string.h>
#include <common/array.h>
#include <core/sxcore.h>

class ParamLKit
{
public:
	ParamLKit();
	~ParamLKit();

	void Load(const char* path);

	UINT GetCount() const;
	const char* GetName(ID id) const;
	float GetThickness(ID id) const;
	float GetRoughness(ID id) const;
	float GetF0(ID id) const;

	ID Find(float thickness, float roughness, float f0) const;

protected:

	struct paraml
	{
		String name;
		float thickness;
		float roughness;
		float f0;
	};

	Array<paraml*> Arr;
};

#endif