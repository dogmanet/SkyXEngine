#ifndef SX_Stack_H
#define SX_Stack_H

#include <core\MemAlloc.h>

template <typename T, int pageSize=256>
class Stack
{
	struct StackNode
	{
		StackNode * Parent;
		T data;
	};

	MemAlloc<StackNode, pageSize> Data;
	int SP;

	StackNode * CurrentNode;

public:

	Stack():SP(0), CurrentNode(NULL)
	{
		//printf("Stack()\n");
	}

	Stack(const Stack<T> & st)
	{
		//printf("Stack()\n");
		this->CurrentNode = st.CurrentNode;
		this->Data = st.Data;
		this->SP = st.SP;
	}

	Stack & operator=(const Stack<T> & st)
	{
		this->CurrentNode = st.CurrentNode;
		this->Data = st.Data;
		this->SP = st.SP;
		return(*this);
	}

	~Stack()
	{
		//printf("~Stack()\n");
	}

	void push(const T & data)
	{
		this->SP++;
		StackNode * tmp = this->CurrentNode;
		this->CurrentNode = this->Data.Alloc();
		this->CurrentNode->Parent = tmp;
		this->CurrentNode->data = data;
	}

	T pop()
	{
			if(SP)
			{
				StackNode * tmp = this->CurrentNode;
				CurrentNode = this->CurrentNode->Parent;
				this->SP--;
				T data = tmp->data;
				//remove tmp
				Data.Delete(tmp);
				return(data);
			}
		//return(NULL);
	}

	void pop(int countpop)
	{
		for (int i = 0; i < countpop; ++i)
		{
			if (SP)
			{
				StackNode * tmp = this->CurrentNode;
				CurrentNode = this->CurrentNode->Parent;
				this->SP--;
				T data = tmp->data;
				Data.Delete(tmp);
			}
		}
	}

	bool IsEmpty()
	{
		return(this->SP == 0);
	}

	T * GetAt(int id)
	{
		if(id < 0)
		{
			id += this->SP;
		}
		if(id < 0 || id >= this->SP)
		{
			return(NULL);
		}
		int page = id / pageSize;
		int pageOffset = id % pageSize;
		return(Data.GetAt(page, pageOffset));
	}

	T get(int id)
	{
		if (id < 0)
		{
			id += this->SP;
		}
		--id;
		if (id < 0 || id >= this->SP)
		{
			return(NULL);
		}

		int page = id / pageSize;
		int pageOffset = id % pageSize;
		return(*Data.GetAt(page, pageOffset));
	}

	inline int count()
	{
		return SP;
	}
};

#endif
