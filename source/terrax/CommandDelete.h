#ifndef _COMMAND_DELETE_H_
#define _COMMAND_DELETE_H_

#include <xcommon/editor/IXEditorExtension.h>
#include "terrax.h"

#include <common/assotiativearray.h>
#include <common/string.h>

class CCommandDelete final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override
	{
		return("delete");
	}

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(false);
	}

	void addObject(ID idObject);

protected:
	struct _del_obj
	{
		ID idObject;
		IXEditorObject *pObject;
		AssotiativeArray<String, String> mKeyValues;

		float3_t vPos;
		float3_t vScale;
		SMQuaternion qRotate;
	};
	Array<_del_obj> m_aObjects;
};

#endif
