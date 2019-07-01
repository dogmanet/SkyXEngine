#ifndef _COMMAND_CREATE_H_
#define _COMMAND_CREATE_H_

#include "Command.h"
#include "terrax.h"

#include <common/assotiativearray.h>
#include <common/string.h>
#include <xcommon/editor/IXEditable.h>

class CCommandCreate: public CCommand
{
public:
	CCommandCreate(const float3_t vPos, const char *szTypeName, const char *szClassName, bool useRandomScaleYaw=false);
	~CCommandCreate();

	bool exec();
	bool undo();

	const char *getText()
	{
		return("create");
	}

protected:
	float3_t m_vPos;
	SMQuaternion m_qOrient;
	float m_fScale = 1.0f;
	String m_sClassName;
	IXEditable *m_pEditable = NULL;
	IXEditorObject *m_pObject = NULL;
};

#endif
