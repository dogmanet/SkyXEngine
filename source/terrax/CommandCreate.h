#ifndef _COMMAND_CREATE_H_
#define _COMMAND_CREATE_H_

#include <xcommon/editor/IXEditorExtension.h>
#include "terrax.h"

#include <common/assotiativearray.h>
#include <common/string.h>
#include <xcommon/editor/IXEditable.h>

class CCommandCreate final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	CCommandCreate(const float3_t &vPos, const char *szTypeName, const char *szClassName, bool useRandomScaleYaw=false);
	~CCommandCreate();

	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override
	{
		return("create");
	}

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(false);
	}

	IXEditorObject* getObject()
	{
		return(m_pObject);
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
