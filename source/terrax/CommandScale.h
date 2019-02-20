#ifndef _COMMAND_SCALE_H_
#define _COMMAND_SCALE_H_

#include "Command.h"
#include "terrax.h"

class CCommandScale: public CCommand
{
public:
	bool exec()
	{
		return(false);
	}
	bool undo()
	{
		return(false);
	}

	const char *getText()
	{
		return("scale");
	}

	void addObject(ID idObject)
	{}
	void setStartAABB(const float3 &vAABBmin, const float3 &vAABBmax)
	{}

	void setCurrentAABB(const float3 &vAABBmin, const float3 &vAABBmax)
	{}

protected:
	struct _move_obj
	{
		ID idObject;
		float3_t vStartPos;
		float3_t vEndPos;
	};
	Array<_move_obj> m_aObjects;
	float3_t m_vStartPos;
};

#endif
