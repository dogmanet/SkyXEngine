#ifndef _COMMAND_ROTATE_H_
#define _COMMAND_ROTATE_H_

#include "Command.h"
#include "terrax.h"

class CCommandRotate: public CCommand
{
public:
	bool exec();
	bool undo();

	const char *getText()
	{
		return("rotate");
	}

	void addObject(ID idObject);
	void setStartOrigin(const float3 &vOrigin);
	void setStartPos(const float3 &vPos);

	void setCurrentPos(const float3 &vPos, bool useSnap=false);

protected:
	struct _rot_obj
	{
		ID idObject;
		float3_t vStartPos;
		float3_t vEndPos;
		SMQuaternion vStartOrient;
		SMQuaternion vEndOrient;
	};
	Array<_rot_obj> m_aObjects;
	float3_t m_vOrigin;
	float3_t m_vStartPos;
};

#endif
