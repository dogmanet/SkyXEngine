#ifndef __UICLASSCOMMAND_H 
#define __UICLASSCOMMAND_H 

#include "IUICommand.h"

class CUIClassCommand : public IXUnknownImplementation<IUICommand>
{
private:

	class BaseFunc
	{
	public:
		BaseFunc() {}

		virtual ~BaseFunc() {}

		virtual void execute() = 0;
	};

	template <typename FuncT, typename Class>
	class DerivedFunc : public BaseFunc
	{
	private:
		FuncT function;
		Class *obj;

	public:
		DerivedFunc(FuncT f, Class *obj)
			: BaseFunc(), function(f), obj(obj)
		{}

		virtual void execute()
		{
			return (obj->*function)();
		}
	};

	BaseFunc *func;

public:

	template <typename Class>
	CUIClassCommand(void(Class::*fuction)(), Class *obj)
		: func(new DerivedFunc<void(Class::*)(), Class>(fuction, obj)) {}

	void execute()
	{
		return func->execute();
	}

	~CUIClassCommand()
	{
		mem_delete(func);
	}
};

#endif
