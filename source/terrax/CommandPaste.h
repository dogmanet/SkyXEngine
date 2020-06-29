#ifndef _COMMAND_PASTE_H_
#define _COMMAND_PASTE_H_

#include "Command.h"
#include "terrax.h"

#include <common/assotiativearray.h>
#include <common/string.h>
#include <xcommon/editor/IXEditable.h>
#include "CommandSelect.h"

class CCommandPaste: public CCommand
{
public:
	~CCommandPaste();

	bool exec();
	bool undo();

	const char *getText()
	{
		return("paste");
	}

	UINT addObject(const char *szTypeName, const char *szClassName, const float3 &vPos, const float3 &vScale, const SMQuaternion &qRotate);
	void setKV(UINT uId, const char *szKey, const char *szValue);

protected:
	struct _paste_obj
	{
	//	ID idObject;
		IXEditorObject *pObject;
		AssotiativeArray<String, String> mKeyValues;

		float3_t vPos;
		float3_t vScale;
		SMQuaternion qRotate;
	};
	Array<_paste_obj> m_aObjects;

	CCommandSelect *m_pCommandSelect = NULL;
};

#endif
