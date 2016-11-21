#ifndef SX_Stack_H
#define SX_Stack_H


template <typename T>
class Stack
{
public:
	Stack(){Arr.reserve(1024);}

	inline int push(T val)
	{
		Arr.push_back(val);
		return Arr.size();
	}

	inline void pop(int count)
	{
			for(int i=0;i<count && Arr.size() > 0;i++)
				Arr.erase(Arr.size()-1);
	}

	inline T& get(int id)
	{
			if(id-1 >= 0 && id-1 < Arr.size())
			{
				return Arr[id-1];
			}
			else if(id < 0 && abs(id)-1 < Arr.size())
			{
				return Arr[Arr.size()+id];
			}
	}

	inline long count()
	{
		return Arr.size();
	}

protected:
	Array<T> Arr;
};

#endif
