
#ifndef __SHADERS_KIT_H
#define __SHADERS_KIT_H

#include <common/string.h>
#include <common/array.h>
#include <core/sxcore.h>

class ShadersKit
{
public:
	ShadersKit();
	~ShadersKit();

	void Load(const char* path);

	UINT GetCount() const;
	const char* GetName(ID id) const;
	const char* GetVS(ID id) const;
	const char* GetPS(ID id) const;

	ID Find(const char* vs, const char* ps) const;

protected:

	struct shaders
	{
		String name;
		String vs;
		String ps;
	};

	Array<shaders*> Arr;
};

#endif