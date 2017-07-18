
#ifndef __shaders_kit_h
#define __shaders_kit_h

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