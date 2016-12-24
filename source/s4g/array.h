#ifndef Array_H
#define Array_H

/*
	Внимание:
		Элемент массива не имеет гарантированного расположения в памяти.
*/
template <typename T, int BlockSize = 16>
class s4g_stack;

template <typename T, int BlockSize = 16>
class MemAllocator;

template<typename T, int BlockSize = 16>
class s4g_stack_register;

class s4g_table;
class s4g_gc;

template<typename T, int BlockSize=16>
class Array
{
public:
	Array():Size(0),AllocSize(0),Data(NULL)
	{
		Alloc();
	}

	Array(const Array & arr):Size(0), AllocSize(0), Data(NULL)
	{
		Alloc();
		//this->AllocSize = arr.AllocSize;
		//this->Size = arr.Size;
			for(int i = arr.Size - 1; i >= 0; i--)
			{
				(*this)[i] = arr[i];
			}
	}

	void resize(UINT NewSize)
	{
		Realloc(NewSize);
		//ConstructInterval(this->Size, key);
			if(this->Size < NewSize)
			{
				ConstructInterval(this->Size, NewSize - 1);
			}
		this->Size = NewSize;
	}

	void reserve(UINT size)
	{
		Realloc(size);
	}

	inline UINT size() const
	{
		return(Size);
	}

	inline void push_back(const T & data)
	{
		(*this)[this->Size] = data;
	}

	void erase(UINT key)
	{
			/*if(key < 0)
			{
				_asm
				{
					int 3;
				};
			}*/

			if(key < this->Size)
			{
				(&this->Data[key])->~T();
				memcpy(&this->Data[key], &this->Data[key + 1], sizeof(T) * (this->Size - key - 1));
				this->Size--;
			}
			else
			{
				_asm
				{
					int 3;
				};
			}
	}

	Array & operator=(T* arr)
	{
		//this->AllocSize = arr.AllocSize;
		//this->Size = arr.Size;
			/*for(int i = arr.Size - 1; i >= 0; i--)
			{
				(*this)[i] = arr[i];
			}*/
		return(*this);
	}

	Array & operator=(const Array<T, BlockSize> & arr)
	{
		//this->AllocSize = arr.AllocSize;
		//this->Size = arr.Size;
			for(int i = arr.Size - 1; i >= 0; i--)
			{
				(*this)[i] = arr[i];
			}
		return(*this);
	}

	inline T & operator[](UINT key)
	{
		if(key > ((UINT)-1) - 128)
		{
			
		}
			if(key >= this->Size)
			{
					if(key >= this->AllocSize)
					{
						Realloc(max(this->AllocSize, key) + BlockSize);
					}
				ConstructInterval(this->Size, key);
				this->Size = key + 1;
			}
		return(Data[key]);
	}


	inline T & GetKeyOC(UINT key)
	{
		return(Data[key]);
	}

	inline void SetKeyOC(UINT key, T& val)
	{
		Data[key] = val;
	}

	inline const T & operator[](UINT key) const
	{
			if(key >= this->Size)
			{
				/*_asm
				{
					int 3;
				};*/
				//SkyXEngine::Core::InError("exit in array");
			}
		return(Data[key]);
	}
	

	~Array()
	{
			if(Size)
			{
				DestructInterval(0, Size - 1);
			}
		free(Data);
	}

	void clear()
	{
			if(Size)
			{
				DestructInterval(0, Size - 1);
			}
		free(Data);
		Size = 0;
		AllocSize = 0;
		Data = NULL;
		Alloc();
	}

	inline UINT GetAllocSize()
	{
		return AllocSize;
	}

protected:
	friend MemAllocator<T, BlockSize>;
	friend s4g_stack<T, BlockSize>;
	friend s4g_stack_register<T, BlockSize>;
	friend s4g_table;
	friend s4g_gc;
	void Alloc()
	{
		Realloc(BlockSize);
	}

	void Realloc(UINT NewSize)
	{
		T * tmpData = (T*)malloc(sizeof(T) * NewSize);
		memcpy(tmpData, this->Data, min(NewSize, this->Size) * sizeof(T));
			if(this->Size > NewSize)
			{
				DestructInterval(NewSize, this->Size - 1);
				this->Size = NewSize;
			}
		
		this->AllocSize = NewSize;
		T * tmpDel = this->Data;
		this->Data = tmpData;
		free(tmpDel);
	}

	void ConstructInterval(UINT start, UINT end)
	{
		//this->Data + start = new(this->Data + start) T[end - start + 1];
			for(UINT i = start; i <= end; i++)
			{
				new(&this->Data[i]) T;
			}
	}

	void DestructInterval(UINT start, UINT end)
	{
			for(UINT i = start; i <= end; i++)
			{
				(&this->Data[i])->~T();
			}
	}

	T * Data;

	UINT Size;
	UINT AllocSize;
};


#endif
