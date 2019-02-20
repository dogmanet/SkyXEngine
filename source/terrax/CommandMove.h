#ifndef _COMMAND_MOVE_H_
#define _COMMAND_MOVE_H_

#include "Command.h"
#include "terrax.h"

class CCommandMove: public CCommand
{
public:
	bool exec();
	bool undo();

	const char *getText();

	void addObject(ID idObject);
	void setStartPos(const float3 &vPos);

	void setCurrentPos(const float3 &vPos);

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
