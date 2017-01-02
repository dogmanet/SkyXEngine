#ifndef SX_AssotiativeArray_H
#define SX_AssotiativeArray_H
//#include "DSbase.h"
#include <core\MemAlloc.h>
#include <core\Stack.h>

#ifndef NULL
#define NULL 0
#endif

//#define SX_KEYTYPE int
//#define SX_VALTYPE int
#if defined(_WINDOWS)
#	pragma warning(disable:4700)
#endif


template<typename SX_KEYTYPE, typename SX_VALTYPE, bool searchCache=false>
class AssotiativeArray
{
public:
	struct Node
	{
		Node * Parent;
		Node * Left;
		Node * Right;
		bool IsBlack;
		SX_KEYTYPE Key;
		SX_VALTYPE * Val;
	};
	mutable Node * TmpNode;
private:
	unsigned int Size_;
	Node * RootNode;


	Node * TreeSearch(const SX_KEYTYPE & key) const
	{
		if(searchCache && TmpNode && TmpNode->Key == key)
		{
			return(TmpNode);
		}
		Node * tmpCurNode = this->RootNode;

			while(tmpCurNode)
			{
					if(tmpCurNode->Key == key)
					{
						TmpNode = tmpCurNode;
						return(tmpCurNode);
					}
				tmpCurNode = tmpCurNode->Key < key ? tmpCurNode->Right : tmpCurNode->Left;
			}
		TmpNode = NULL;
		return(NULL);
	}
	void TreeRotateLeft(Node * node)
	{
		Node * tmpNode = node->Right;

		node->Right = tmpNode->Left;
			if(tmpNode->Left)
			{
				tmpNode->Left->Parent = node;
			}

			if(tmpNode)
			{
				tmpNode->Parent = node->Parent;
			}
			if(node->Parent)
			{
					if(node == node->Parent->Left)
					{
						node->Parent->Left = tmpNode;
					}
					else
					{
						node->Parent->Right = tmpNode;
					}
			}
			else
			{
				this->RootNode = tmpNode;
			}

		tmpNode->Left = node;
			if(node)
			{
				node->Parent = tmpNode;
			}
	}
	void TreeRotateRight(Node * node)
	{
		Node * tmpNode = node->Left;

		node->Left = tmpNode->Right;
			if(tmpNode->Right)
			{
				tmpNode->Right->Parent = node;
			}

			if(tmpNode)
			{
				tmpNode->Parent = node->Parent;
			}
			if(node->Parent)
			{
					if(node == node->Parent->Right)
					{
						node->Parent->Right = tmpNode;
					}
					else
					{
						node->Parent->Left = tmpNode;
					}
			}
			else
			{
				this->RootNode = tmpNode;
			}

		tmpNode->Right = node;
			if(node)
			{
				node->Parent = tmpNode;
			}
	}

	void TreeFixIns(Node * node)
	{
			while(node != this->RootNode && !node->Parent->IsBlack)
			{
					if(node->Parent == node->Parent->Parent->Left)
					{
						Node * tmpNode = node->Parent->Parent->Right;
							if(tmpNode && !tmpNode->IsBlack)
							{
								node->Parent->IsBlack = true;
								tmpNode->IsBlack = true;
								node->Parent->Parent->IsBlack = false;
								node = node->Parent->Parent;
							}
							else
							{
									if(node == node->Parent->Right)
									{
										node = node->Parent;
										TreeRotateLeft(node);
									}
								node->Parent->IsBlack = true;
								node->Parent->Parent->IsBlack = false;
								TreeRotateRight(node->Parent->Parent);
							}
					}
					else
					{
						Node * tmpNode = node->Parent->Parent->Left;
							if(tmpNode && !tmpNode->IsBlack)
							{
								node->Parent->IsBlack = true;
								tmpNode->IsBlack = true;
								node->Parent->Parent->IsBlack = false;
								node = node->Parent->Parent;
							}
							else
							{
									if(node == node->Parent->Left)
									{
										node = node->Parent;
										TreeRotateRight(node);
									}
								node->Parent->IsBlack = true;
								node->Parent->Parent->IsBlack = false;
								TreeRotateLeft(node->Parent->Parent);
							}
					}
			}
		this->RootNode->IsBlack = true;
	}
	void TreeFixDel(Node * node)
	{
			while(node != this->RootNode && node->IsBlack)
			{
					if(node == node->Parent->Left)
					{
						Node * tmpNode = node->Parent->Right;
							if(!tmpNode->IsBlack)
							{
								tmpNode->IsBlack = true;
								node->Parent->IsBlack = false;
								TreeRotateLeft(node->Parent);
								tmpNode = node->Parent->Right;
							}
							if(tmpNode->Left->IsBlack && tmpNode->Right->IsBlack)
							{
								tmpNode->IsBlack = false;
								node = node->Parent;
							}
							else
							{
									if(tmpNode->Right->IsBlack)
									{
										tmpNode->Left->IsBlack = true;
										tmpNode->IsBlack = false;
										TreeRotateRight(tmpNode);
										tmpNode = node->Parent->Right;
									}
								tmpNode->IsBlack = node->Parent->IsBlack;
								node->Parent->IsBlack = true;
								tmpNode->Right->IsBlack = true;
								TreeRotateLeft(node->Parent);
								node = this->RootNode;
							}
					}
					else
					{
						Node * tmpNode = node->Parent->Left;
							if(!tmpNode->IsBlack)
							{
								tmpNode->IsBlack = true;
								node->Parent->IsBlack = false;
								TreeRotateRight(node->Parent);
								tmpNode = node->Parent->Left;
							}
							if(tmpNode->Right->IsBlack && tmpNode->Left->IsBlack)
							{
								tmpNode->IsBlack = false;
								node = node->Parent;
							}
							else
							{
									if(tmpNode->Left->IsBlack)
									{
										tmpNode->Right->IsBlack = true;
										tmpNode->IsBlack = false;
										TreeRotateLeft(tmpNode);
										tmpNode = node->Parent->Left;
									}
								tmpNode->IsBlack = node->Parent->IsBlack;
								node->Parent->IsBlack = true;
								tmpNode->Left->IsBlack = true;
								TreeRotateRight(node->Parent);
								node = this->RootNode;
							}
					}
			}
		node->IsBlack = true;
	}

	Node * TreeInsert(const SX_KEYTYPE & key)
	{
		Node * tmpCur;
		Node * tmpParent;
		Node * tmpNode;

		tmpCur = this->RootNode;
		tmpParent = NULL;
			while(tmpCur)
			{
					if(key == tmpCur->Key)
					{
						//tmpCur->Val = val;
						//memcpy(tmpCur->Val, &val, sizeof(SX_VALTYPE));
						//*tmpNode->Val = val;
						return(tmpCur);
					}
				tmpParent = tmpCur;
				tmpCur = key < tmpCur->Key ? tmpCur->Left : tmpCur->Right;
			}
		this->Size_++;
		// tmpNode = new Node;
		tmpNode = this->MemNodes.Alloc();
			if(!tmpNode)
			{
				//printf("Unable to allocate memory\n");
#if defined(_WINDOWS)
				_asm
				{
					int 3;
				};
#else
				asm("int $3");
#endif
			}
		tmpNode->Key = key;
		//tmpNode->Val = new SX_VALTYPE;
		tmpNode->Val = this->MemVals.Alloc();
		//tmpNode->Val = AllocVal();
		//*tmpNode->Val = val;
		//memcpy(tmpNode->Val, &val, sizeof(SX_VALTYPE));
		tmpNode->Parent = tmpParent;
		tmpNode->Left = NULL;
		tmpNode->Right = NULL;
		tmpNode->IsBlack = false;

			if(tmpParent)
			{
					if(key < tmpParent->Key)
					{
						tmpParent->Left = tmpNode;
					}
					else
					{
						tmpParent->Right = tmpNode;
					}
			}
			else
			{
				this->RootNode = tmpNode;
			}

		TreeFixIns(tmpNode);
		return(tmpNode);
	}

	//void TreeDelete(Node * node);

	
	MemAlloc<Node> MemNodes;
	MemAlloc<SX_VALTYPE> MemVals;

	void TreeReleaseNode(Node * node)
	{
			if(node->Left)
			{
				TreeReleaseNode(node->Left);
			}
			if(node->Right)
			{
				TreeReleaseNode(node->Right);
			}
		SX_SAFE_DELETE(node->Val);
		SX_SAFE_DELETE(node);
	}

public:

	class Iterator
	{
	private:
		Stack<const Node*> nodes;
		const Node * CurNode;

		bool IsEnd;
	public:
		const SX_KEYTYPE * first;
		SX_VALTYPE * second;

		Iterator(const Iterator & it)
		{
			this->nodes = it.nodes;
			this->first = it.first;
			this->second = it.second;
			this->IsEnd = it.IsEnd;
			this->CurNode = it.CurNode;
		}

		Iterator(const Node * node)
		{
				if(node)
				{
					const Node * tmpNode = node;
						while(tmpNode)
						{
							nodes.push(tmpNode);
							if(!tmpNode->Left)
							{
								this->IsEnd = false;
								this->first = &(tmpNode->Key);
								this->second = tmpNode->Val;
								this->CurNode = tmpNode;
								nodes.pop();
							}
							tmpNode = tmpNode->Left;
						}
				}
				else
				{
					IsEnd = true;
					this->first = false;
					this->second = false;
					this->CurNode = false;
				}
		}
		~Iterator()
		{

		}

		Iterator & operator=(Iterator const & it)
		{
			this->nodes = it.nodes;
			this->first = it.first;
			this->second = it.second;
			this->IsEnd = it.IsEnd;
			this->CurNode = it.CurNode;
			return(*this);
		}

		bool operator==(const Iterator & c)
		{
			return(c.CurNode == this->CurNode && c.IsEnd == this->IsEnd);
		}

		bool operator!=(const Iterator & c)
		{
			return(!(c.CurNode == this->CurNode && c.IsEnd == this->IsEnd));
		}

		operator bool()
		{
			return(!this->IsEnd);
		}

		Iterator operator++(int)
		{
				if(!this->CurNode && this->nodes.IsEmpty())
				{
					IsEnd = true;
					this->first = false;
					this->second = false;
					this->CurNode = false;
					return(*this);
				}

				if(this->CurNode->Right)
				{
					this->CurNode = this->CurNode->Right;
				}
				else
				{
					this->CurNode = NULL;
				}
				while(this->CurNode)
				{
					this->nodes.push(this->CurNode);
					this->CurNode = this->CurNode->Left;
				}

				if(!this->nodes.IsEmpty())
				{
					this->CurNode = this->nodes.pop();
					this->first = &(this->CurNode->Key);
					this->second = this->CurNode->Val;
					this->CurNode = this->CurNode;
					return(*this);
				}
				if(!this->CurNode)
				{
					IsEnd = true;
					this->first = false;
					this->second = false;
					this->CurNode = false;
				}
			return(*this);
		}
	};
/*
	class iterator
	{
	private:
		//Stack<const Node*> nodes;
		const Node * CurNode;
		//const Node * NextNode;

		bool IsEnd;
		bool OnRight;
		struct pair
		{
			const SX_KEYTYPE * first;
			SX_VALTYPE * second;
		};

		pair Data;
	public:
		//const SX_KEYTYPE * first;
		//SX_VALTYPE * second;

		iterator(const Iterator & it)
		{
			this->nodes = it.nodes;
			this->Data = it.Data;
			this->IsEnd = it.IsEnd;
			this->CurNode = it.CurNode;
			//this->NextNode = it.NextNode;
			this->OnRight = it.OnRight;
		}

		iterator(const Node * node)
		{
				if(node)
				{
					const Node * tmpNode = node;
						while(tmpNode)
						{
							//nodes.push(tmpNode);
								if(!tmpNode->Left)
								{
									this->IsEnd = false;
									this->Data.first = &(tmpNode->Key);
									this->Data.second = tmpNode->Val;
									this->CurNode = tmpNode;
									//this->NextNode = this->CurNode->Parent;
									this->OnRight = false;
									//nodes.pop();
								}
							tmpNode = tmpNode->Left;
						}
				}
				else
				{
					IsEnd = true;
					this->Data.first = NULL;
					this->Data.second = NULL;
					this->CurNode = NULL;
					//this->NextNode = NULL;
				}
		}
		~iterator()
		{

		}

		iterator & operator=(Iterator const & it)
		{
			this->nodes = it.nodes;
			this->Data = it.Data;
			this->IsEnd = it.IsEnd;
			this->CurNode = it.CurNode;
			//this->NextNode = it.NextNode;
			this->OnRight = it.OnRight;
			return(*this);
		}

		bool operator==(const Iterator & c)
		{
			return(c.CurNode == this->CurNode / *&& c.NextNode == this->NextNode * /&& c.IsEnd == this->IsEnd);
		}

		bool operator!=(const Iterator & c)
		{
			return(!(c.CurNode == this->CurNode / *&& c.NextNode == this->NextNode * /&& c.IsEnd == this->IsEnd));
		}

		operator bool()
		{
			return(!this->IsEnd);
		}

		pair * operator->()
		{
			return(&Data);
		}

		iterator operator++(int)
		{
				if(this->CurNode->Right && !this->OnRight)
				{
					const Node * tmpNode = this->CurNode->Right;
						while(tmpNode)
						{
							//nodes.push(tmpNode);
								if(!tmpNode->Left)
								{
									this->IsEnd = false;
									this->Data.first = &(tmpNode->Key);
									this->Data.second = tmpNode->Val;
									this->CurNode = tmpNode;
									//this->NextNode = this->CurNode->Parent;
									this->OnRight = true;
									//nodes.pop();
								}
							tmpNode = tmpNode->Left;
						}
				}
				else
				{
					this->CurNode = this->CurNode->Parent;
					this->IsEnd = false;
					this->Data.first = &(tmpNode->Key);
					this->Data.second = tmpNode->Val;
					this->OnRight = false;
				}
			

				if(!this->CurNode)
				{
					IsEnd = true;
					this->Data.first = NULL;
					this->Data.second = NULL;
					this->CurNode = NULL;
					this->NextNode = NULL;
				}
			return(*this);
		}
	};
*/

	AssotiativeArray():RootNode(NULL), Size_(0),TmpNode(NULL)
	{
		//printf("AssotiativeArray()\n");
	}

	AssotiativeArray(const AssotiativeArray & a):RootNode(NULL), Size_(0),TmpNode(NULL)
	{
		for(AssotiativeArray<SX_KEYTYPE, SX_VALTYPE>::Iterator i = a.begin(); i; i++)
		{
			(*this)[*(i.first)] = *(i.second);
		}
	}

	/*AssotiativeArray(const AssotiativeArray & a)
	{
		DS_ASSERT(false);
		this->Size_ = a.Size_;
		this->MemNodes = a.MemNodes;
		this->MemVals = a.MemVals;
		//printf("AssotiativeArray()\n");
	}*/
	~AssotiativeArray()
	{
		//printf("~AssotiativeArray()\n");
		Release();
	}

	bool KeyExists(const SX_KEYTYPE & key, const Node ** pNode = NULL) const
	{
		TmpNode = TreeSearch(key);
		if(pNode)
		{
			*pNode = TmpNode;
		}
		return(TmpNode != NULL);
	}

	bool KeyExists(const SX_KEYTYPE & key, const Node ** pNode=NULL, bool create=false) 
	{
		TmpNode = TreeSearch(key);
		if(pNode)
		{
			*pNode = TmpNode;
		}
		//tmpNode
		if(!TmpNode && create)
		{
			*pNode = TreeInsert(key);
		}
		return(TmpNode != NULL);
	}

	AssotiativeArray & operator=(const AssotiativeArray & a)
	{
		for(AssotiativeArray<SX_KEYTYPE, SX_VALTYPE>::Iterator i = a.begin(); i; i++)
		{
			this->operator[](*i.first) = *i.second;
		}
		return(*this);
		/*_asm
		{
			int 3;
		};
		//DS_ASSERT(false);
		this->Size_ = a.Size_;
		this->MemNodes = a.MemNodes;
		this->MemVals = a.MemVals;*/
		//printf("AssotiativeArray()\n");
	}

	SX_VALTYPE & operator[](const SX_KEYTYPE & key)
	{
		TmpNode = TreeSearch(key);
			if(!TmpNode)
			{
				//SX_VALTYPE tmpVal = {0};
				TmpNode = TreeInsert(key);
			}
		return(*TmpNode->Val);
	}

	const SX_VALTYPE & operator[](const SX_KEYTYPE & key) const
	{
		TmpNode = TreeSearch(key);
		return(*TmpNode->Val);
	}

	const SX_VALTYPE * at(const SX_KEYTYPE & key) const
	{
		TmpNode = TreeSearch(key);
		return(TmpNode ? TmpNode->Val : NULL);
	}

	void Insert(const SX_KEYTYPE & key, const SX_VALTYPE & val);

	unsigned int Size() const
	{
		return(this->Size_);
	}

	/*void Delete(SX_KEYTYPE key)
	{
		Node * tmpN = TreeSearch(key);
		if(tmpN)
		{
			TreeDelete(tmpN);
		}
	}*/

	void erase(SX_KEYTYPE val)
	{
		//@@TODO: Implement me
	}

	void clear()
	{
		this->RootNode = NULL;
		MemNodes.clear();
		MemVals.clear();
		this->Size_ = 0;
	}

	Iterator begin() const
	{
		return(this->RootNode);
		//Iterator i(this->RootNode);
		//return(i);
		/*Node * tmpNode = this->RootNode;
		while(tmpNode)
		{
			tmpNode = tmpNode->Left;
			if(!tmpNode->Left)
			{
				return(Iterator(tmpNode));
			}
		}
		return(Iterator(NULL));*/
	}

	/*const Node * GetRootNode()
	{
		return(RootNode);
	}*/

	Iterator end()
	{
		return(NULL);
	}

	void Release()
	{
		//TreeReleaseNode(this->RootNode);
		this->RootNode = NULL;
	}
};
#if defined(_WINDOWS)
#	pragma warning(default:4700)
#endif

#endif
