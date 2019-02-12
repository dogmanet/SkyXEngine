#ifndef _COMMAND_H_
#define _COMMAND_H_

class CCommand
{
public:
	virtual ~CCommand()
	{}

	virtual bool exec() = 0;
	virtual bool undo() = 0;

	virtual const char *getText() = 0;
};

#endif
