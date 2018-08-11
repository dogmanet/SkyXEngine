#ifndef SX_SXStack_H
#define SX_SXStack_H

#include "SXMemAlloc.h"

template <typename T>
class SXStack
{
	struct StackNode
	{
		StackNode * Parent;
		T data;
	};

	SXMemAlloc<StackNode> Data;
	int SP;

	StackNode * CurrentNode;

public:

	SXStack():SP(0), CurrentNode(NULL)
	{
		//printf("SXStack()\n");
	}

	SXStack(const SXStack<T> & st)
	{
		//printf("SXStack()\n");
		this->CurrentNode = st.CurrentNode;
		this->Data = st.Data;
		this->SP = st.SP;
	}

	SXStack & operator=(const SXStack<T> & st)
	{
		this->CurrentNode = st.CurrentNode;
		this->Data = st.Data;
		this->SP = st.SP;
		return(*this);
	}

	~SXStack()
	{
		//printf("~SXStack()\n");
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

	bool IsEmpty()
	{
		return(this->SP == 0);
	}
};

#endif
