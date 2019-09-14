#ifndef _COMMAND_DELETE_H_
#define _COMMAND_DELETE_H_

#include "Command.h"
#include "terrax.h"

#include <common/assotiativearray.h>
#include <common/string.h>

class CCommandDelete: public CCommand
{
public:
	bool exec();
	bool undo();

	const char *getText()
	{
		return("delete");
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
