#ifndef SX_AATABLE_H
#define SX_AATABLE_H

#include <string/string.h>
#include <core/assotiativearray.h>

template<typename T>
class AATable
{
public:
	AATable(){}
	~AATable()
	{
		for(int i = 0; i < Arr.size(); i++)
		{
			mem_delete(Arr[i]);
		}
	}

	inline void Add(T* data)
	{
		DWORD tmpsize = Arr.size();
		AATDesc* tmpaa = new AATDesc();
		tmpaa->Name = NULL;
		tmpaa->Value = data;
		//memcpy(tmpaa->Value,data,sizeof(s4g_value));
		Arr.push_back(tmpaa);
	}

	inline void Add(const char* name, T* data)
	{
		long tmpkey = GetKey(name);
		if (tmpkey != -1)
			Arr[tmpkey]->Value = data;
		else
		{
			DWORD tmpsize = Arr.size();
			AATDesc* tmpaa = new AATDesc();
			tmpaa->Value = data;
			Keys[name] = Arr.size();
			tmpaa->Name = Keys.TmpNode->Key.c_str();
			Arr.push_back(tmpaa);
		}
	}

	inline void Add(UINT key, T* data)
	{
		if (key > ((UINT)-1) - 128)
		{
			_asm
			{
				int 3;
			};
		}
		else if (key < Arr.size())
			Arr[key]->Value = data;
		else
		{
			DWORD tmpsize = Arr.size();
			AATDesc* tmpaa = new AATDesc();
			tmpaa->Value = data;
			tmpaa->Name = NULL;
			Arr.push_back(tmpaa);
		}
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
			else if(key < Arr.size())
			{
				Keys[Arr[key]->Name] = -1;
				Arr.erase(key);
			}
	}

	inline void Del(const char* name)
	{
		long tmpkey = GetKey(name);
			if(tmpkey != -1)
			{
				Arr.erase(tmpkey);
				Keys[name] = -1;
			}
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
				tmpaa->Name = NULL;
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
				Keys[name] = Arr.size();
				tmpaa->Name = Keys.TmpNode->Key.c_str();
				Arr.push_back(tmpaa);
				return Arr[tmpsize]->Value;
			}
	}

	inline long GetKey(const char* name)
	{
			if(Keys.KeyExists(name))
			{
				return(Keys[name]);
			}
		return -1;
	}

	inline bool IsExists(const char* name)
	{
		return(Keys.KeyExists(name));
	}

	inline const char* GetNameID(long id)
	{
			if(id < Arr.size() && id >= 0 && Arr[id])
				return Arr[id]->Name;
		return(NULL);
	}

	inline long GetSize()
	{
		return Arr.size();
	}

	inline void reserve(int count)
	{
		Arr.reserve(count);
	}

protected:
	struct AATDesc
	{
		//char Name[S4G_MAX_LEN_VAR_NAME];
		const char * Name;
		T* Value;
	};

	Array<AATDesc*> Arr;
	AssotiativeArray<String, long> Keys;
};

#endif
