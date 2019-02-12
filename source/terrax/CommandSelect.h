#ifndef _COMMAND_SELECT_H_
#define _COMMAND_SELECT_H_

#include "Command.h"
#include "terrax.h"

class CCommandSelect: public CCommand
{
public:
	bool exec();
	bool undo();

	const char *getText();

	void addSelected(ID idObject);
	void addDeselected(ID idObject);

protected:
	Array<ID> m_aidSelected;
	Array<ID> m_aidDeselected;
}

#endif
