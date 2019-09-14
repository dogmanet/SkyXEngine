#ifndef _COMMAND_SCALE_H_
#define _COMMAND_SCALE_H_

#include "Command.h"
#include "terrax.h"

class CCommandScale: public CCommand
{
public:
	bool exec();
	bool undo();

	const char *getText()
	{
		return("scale");
	}

	void addObject(ID idObject);
	void setStartAABB(const float3 &vAABBmin, const float3 &vAABBmax);
	void setStartPos(const float3 &vPos);
	void setTransformDir(X_DIR dir);

	void setCurrentPos(const float3 &vPos);

protected:
	struct _scale_obj
	{
		ID idObject;
		float3_t vStartPos;
		float3_t vEndPos;
		float3_t vStartScale;
		float3_t vEndScale;
	};
	Array<_scale_obj> m_aObjects;
	X_DIR m_xDir;
	float3_t m_vDelta;

	float3_t m_vBoundMin;
	float3_t m_vBoundMax;
};

#endif
