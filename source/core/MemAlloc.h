#ifndef MEMALLOC_H
#define MEMALLOC_H

#define mem_free(a) free(a)
#define mem_alloc(a) malloc(a)

#if defined(_WINDOWS)
#	include <new.h>
#else
#	include <new>
#endif

#if defined(_WINDOWS)
#	pragma warning(disable:4018)
#endif

#if defined(_LINUX) || defined(_MAC)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wsign-compare"
#endif

template <typename T, int SizeBlock = 256>
class MemAlloc
{
	struct MemCell
	{
		UINT IsFree;
		T data;
	};
	struct MemBlock
	{
		MemCell * mem;
		unsigned int size;
		unsigned int pos;
	};
	MemBlock * memblocks;
	int NumCurBlock;
	int NumCurBlockCount;
public:
	MemAlloc():memblocks(NULL), NumCurBlock(0), NumCurBlockCount(0)
	{
		//printf("MemAlloc()\n");
		AllocBlock();
	}

	MemAlloc(const MemAlloc & al)
	{
		//printf("MemAlloc()\n");
		this->NumCurBlock = al.NumCurBlock;
		this->NumCurBlockCount = al.NumCurBlockCount;
		this->memblocks = new MemBlock[this->NumCurBlockCount];
		//SX_DBG_NEW(this->NumCurBlockCount * sizeof(MemBlock), this->memblocks);
		for(int i = 0; i < this->NumCurBlockCount; i++)
		{
			this->memblocks[i].size = al.memblocks[i].size;
			this->memblocks[i].pos = al.memblocks[i].pos;
			this->memblocks[i].mem = (MemCell*)malloc(al.memblocks[i].size * sizeof(MemCell));
			//SX_DBG_NEW(al.memblocks[i].size, this->memblocks[i].mem);
			for(int j = 0; j < al.memblocks[i].size; j++)
			{
				this->memblocks[i].mem[j].IsFree = al.memblocks[i].mem[j].IsFree;
				this->memblocks[i].mem[j].data = al.memblocks[i].mem[j].data;
			}
			//SX_SAFE_DELETE_A(memblocks[i].mem);
		}
	}

	MemAlloc & operator=(const MemAlloc & al)
	{//clear mem
		for(int i = 0; i < this->NumCurBlockCount; i++)
		{
			if(this->memblocks[i].mem)
			{
				for(int j = 0; j < this->memblocks[i].size; j++)
				{
					if(!(this->memblocks[i].mem[j].IsFree & 0x80000000))
					{
						this->memblocks[i].mem[j].data.~T();
					}

				}
				mem_free(this->memblocks[i].mem);
			}
			//SX_SAFE_DELETE_A(this->memblocks[i].mem);
		}
		mem_delete_a(this->memblocks);

		this->NumCurBlock = al.NumCurBlock;
		this->NumCurBlockCount = al.NumCurBlockCount;
		this->memblocks = new MemBlock[this->NumCurBlockCount];
		//SX_DBG_NEW(this->NumCurBlockCount * sizeof(MemBlock), this->memblocks);
		for(int i = 0; i < this->NumCurBlockCount; i++)
		{
			this->memblocks[i].size = al.memblocks[i].size;
			this->memblocks[i].pos = al.memblocks[i].pos;

			if(!this->memblocks[i].size)
			{
				this->memblocks[i].mem = NULL;
			}
			else
			{
				this->memblocks[i].mem = (MemCell*)malloc(al.memblocks[i].size * sizeof(MemCell));
				//SX_DBG_NEW(al.memblocks[i].size, this->memblocks[i].mem);
				for(int j = 0; (unsigned int)j < al.memblocks[i].size; j++)
				{
					this->memblocks[i].mem[j].IsFree = al.memblocks[i].mem[j].IsFree;
					if(!(al.memblocks[i].mem[j].IsFree & 0x80000000))
					{
						this->memblocks[i].mem[j].data = al.memblocks[i].mem[j].data;
					}
				}
			}
			//SX_SAFE_DELETE_A(memblocks[i].mem);
		}
		return(*this);
	}

	~MemAlloc()
	{
		//printf("~MemAlloc()\n");
		for(int i = 0; i < this->NumCurBlockCount; i++)
		{
			if(this->memblocks[i].mem)
			{
				for(int j = 0; j < this->memblocks[i].size; j++)
				{
					if(!(this->memblocks[i].mem[j].IsFree & 0x80000000))
					{
						this->memblocks[i].mem[j].data.~T();
					}

				}
				mem_free(this->memblocks[i].mem);
			}
			//SX_SAFE_DELETE_A(this->memblocks[i].mem);
		}
		mem_delete_a(this->memblocks);
		this->NumCurBlockCount = 0;
		this->NumCurBlock = 0;
	}

	void clear()
	{
		for(int i = 0; i < this->NumCurBlockCount; i++)
		{
			if(this->memblocks[i].mem)
			{
				for(int j = 0; j < this->memblocks[i].size; j++)
				{
					if(!(this->memblocks[i].mem[j].IsFree & 0x80000000))
					{
						(&(this->memblocks[i].mem[j].data))->~T();
					}
				}
				mem_free(this->memblocks[i].mem);
			}
			//SX_SAFE_DELETE_A(this->memblocks[i].mem);
		}
		mem_delete_a(this->memblocks);
		this->NumCurBlockCount = 0;
		this->NumCurBlock = 0;
		AllocBlock();
	}

	void AllocBlock(UINT size = SizeBlock)
	{
		MemBlock * tmpMB = this->memblocks;

		NumCurBlockCount += 256;

		this->memblocks = new MemBlock[NumCurBlockCount];
		//SX_DBG_NEW(this->NumCurBlockCount * sizeof(MemBlock), this->memblocks);
		if(tmpMB)
		{
			memcpy(this->memblocks, tmpMB, (NumCurBlockCount - 256) * sizeof(MemBlock));
		}
		memset(this->memblocks + (NumCurBlockCount - 256), 0, 256 * sizeof(MemBlock));
		mem_delete_a(tmpMB);


		FillCurrentBlock(size);
	}

	__forceinline void FillCurrentBlock(UINT size = SizeBlock)
	{
		if(!this->memblocks[NumCurBlock].mem)
		{
			this->memblocks[NumCurBlock].mem = (MemCell*)mem_alloc(size * sizeof(MemCell));
			//SX_DBG_NEW(size * sizeof(MemCell), this->memblocks[NumCurBlock].mem);
			for(int i = 0; i < size; i++)
			{
				this->memblocks[NumCurBlock].mem[i].IsFree = 0x80000000 | (i + 1);
			}
			this->memblocks[NumCurBlock].size = size;
		}
	}

	T * Alloc()
	{
		T * tmpNewNode = NULL;
		if(!NumCurBlockCount)
		{
			AllocBlock();
		}
		if(this->memblocks[NumCurBlock].pos < this->memblocks[NumCurBlock].size)
		{
			MemCell * mc = &(this->memblocks[NumCurBlock].mem[this->memblocks[NumCurBlock].pos]);
			if(mc->IsFree & 0x80000000)
			{
				this->memblocks[NumCurBlock].pos = mc->IsFree & 0x7FFFFFFF;
				mc->IsFree = NumCurBlock;
				tmpNewNode = &(mc->data);
			}
			else // find next free cell
			{
				while(true)
				{
					++this->memblocks[NumCurBlock].pos;
					if(this->memblocks[NumCurBlock].pos < this->memblocks[NumCurBlock].size)
					{
						mc = &(this->memblocks[NumCurBlock].mem[this->memblocks[NumCurBlock].pos]);
						if(mc->IsFree)
						{
							this->memblocks[NumCurBlock].pos = mc->IsFree & 0x7FFFFFFF;
							mc->IsFree = NumCurBlock;
							tmpNewNode = &(mc->data);
							break;
						}
					}
					else
					{
						//next block
						NumCurBlock++;
						if(NumCurBlock < NumCurBlockCount)
						{
							FillCurrentBlock();
						}
						else
						{
							AllocBlock();
						}
						tmpNewNode = Alloc();
						break;
					}
				}
			}
		}
		else
		{
			NumCurBlock++;
			if(NumCurBlock < NumCurBlockCount)
			{
				FillCurrentBlock();
			}
			else
			{
				AllocBlock();
			}
			tmpNewNode = Alloc();
		}
		tmpNewNode = new (tmpNewNode)T;
		return(tmpNewNode);
	}

	T * GetAt(int page, int offset)
	{
		if(!(memblocks[page].mem[offset].IsFree & 0x80000000))
		{
			return(memblocks[page].mem[offset].data);
		}
		return(NULL);
	}

	void Delete(T * pointer)
	{
		//find cell
		//mark cell as free

		UINT * bnum = (UINT*)((intptr_t)pointer - sizeof(UINT));
		UINT blockID = *bnum;
		UINT curPos = ((intptr_t)bnum - (intptr_t)memblocks[blockID].mem) / sizeof(MemCell);

		pointer->~T();

		if(NumCurBlock > blockID)
		{
			NumCurBlock = blockID;
		}
		if(memblocks[blockID].pos > curPos)
		{
			*bnum = memblocks[blockID].pos | 0x80000000;
			memblocks[blockID].pos = curPos;
		}
		else
		{
			MemCell * mc = &(memblocks[blockID].mem[memblocks[blockID].pos]);
			*bnum = mc->IsFree;
			mc->IsFree = curPos | 0x80000000;
		}
	}

	void Delete(void * ptr)
	{
		Delete((T*)ptr);
	}
};

#if defined(_WINDOWS)
#	pragma warning(default:4018)
#endif
#if defined(_LINUX) || defined(_MAC)
#	pragma GCC diagnostic pop
#endif

#endif
