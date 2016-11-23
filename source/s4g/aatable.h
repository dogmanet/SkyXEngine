#ifndef SX_AssotiativeArray_H
#define SX_AssotiativeArray_H


template<typename T>
class AATable
{
public:
	AATable(){}

	inline void Add(T* data)
	{
		DWORD tmpsize = Arr.size();
		AATDesc* tmpaa = new AATDesc();
		tmpaa->Name[0] = 0;
		tmpaa->Value = new T();
		memcpy(tmpaa->Value,data,sizeof(s4g_value));
		Arr.push_back(tmpaa);
		//T* tmpval = Arr[tmpsize]->Value;
		//strcpy(Arr[tmpsize]->Name,"\0");
		//return tmpval;
	}

	inline void Del(UINT key)
	{
			if (key > ((UINT)-1) - 128)
			{
				_asm
				{
					int 3;
				};
			}
			else if (key < Arr.size())
				Arr.erase(key);
	}

	inline void Del(const char* name)
	{
		long tmpkey = GetKey(name);
			if (tmpkey != -1)
				Arr.erase(tmpkey);
	}

	inline T* & operator[](UINT key)
	{
			if(key > ((UINT)-1) - 128)
			{
				_asm
				{
					int 3;
				};
			}
			else if(key < Arr.size())
				return(Arr[key]->Value);
			else
			{
				DWORD tmpsize = Arr.size();
				AATDesc* tmpaa = new AATDesc();
				tmpaa->Value = new T();
				tmpaa->Name[0] = 0;
				Arr.push_back(tmpaa);
				T* tmpval = Arr[tmpsize]->Value;
				//strcpy(Arr[tmpsize]->Name,name);
				return tmpval;
			}
	}

	inline T* & operator[](const char* name)
	{
		long tmpkey = GetKey(name);
			if(tmpkey != -1)
				return(Arr[tmpkey]->Value);
			else
			{
				DWORD tmpsize = Arr.size();
				AATDesc* tmpaa = new AATDesc();
				tmpaa->Value = new T();
				Arr.push_back(tmpaa);
				T* tmpval = Arr[tmpsize]->Value;
				strcpy(Arr[tmpsize]->Name,name);
				return tmpval;
			}
	}

	inline long GetKey(const char* name)
	{
			for(long i=0;i<Arr.size();i++)
			{
					if(strcmp(Arr[i]->Name,name) == 0)
					{
						return i;
					}
			}
		return -1;
	}

	inline bool IsExists(const char* name)
	{
			for(long i=0;i<Arr.size();i++)
			{
					if(strcmp(Arr[i]->Name,name) == 0)
					{
						return true;
					}
			}
		return false;
	}

	inline long GetSize()
	{
		return Arr.size();
	}

protected:
	struct AATDesc
	{
		char Name[S4G_MAX_LEN_VAR_NAME];
		T* Value;
	};

	Array<AATDesc*> Arr;
};

#endif
