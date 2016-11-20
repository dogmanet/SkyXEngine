#ifndef Array_H
#define Array_H

/*
	��������:
		������� ������� �� ����� ���������������� ������������ � ������.
*/

#ifdef _DBG_MEM
#	undef new
#endif

#ifndef dbg_break
#define dbg_break
#endif

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

	UINT size() const
	{
		return(Size);
	}

	void push_back(const T & data)
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

	T & operator[](UINT key)
	{
		if(key > ((UINT)-1) - 128)
		{
			dbg_break
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

	const T & operator[](UINT key) const
	{
			if(key >= this->Size)
			{
				dbg_break
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

private:

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

#ifdef _DBG_MEM
#	define new DS_NEW
#endif


#endif
