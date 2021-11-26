#ifndef _COMMAND_MOVE_H_
#define _COMMAND_MOVE_H_

#include <xcommon/editor/IXEditorExtension.h>
#include "terrax.h"

class CCommandMove final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	~CCommandMove();

	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override;

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(false);
	}

	void addObject(IXEditorObject *pObj);
	void setStartPos(const float3 &vPos);

	void setCurrentPos(const float3 &vPos);

protected:
	struct _move_obj
	{
		IXEditorObject *pObj;
		float3_t vStartPos;
		float3_t vEndPos;
	};
	Array<_move_obj> m_aObjects;
	float3_t m_vStartPos;
};

#endif
