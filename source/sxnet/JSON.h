#ifndef JSON_H
#define JSON_H

#include <sxtypes.h>
#include <core/array.h>
#include <core/assotiativearray.h>
#include <string/string.h>


#ifdef GetObject
#undef GetObject
#endif

class JSON
{
public:
	enum TYPE
	{
		NUL,
		BOOLEAN,
		STRING,
		NUMBER,
		OBJECT,
		ARRAY
	};

	virtual ~JSON()
	{
	};

	virtual TYPE GetType() const = 0;
	virtual int GetInt() const = 0;
	virtual float GetFloat() const = 0;
	virtual BOOL GetBool() const = 0;
	virtual const Array<JSON*> & GetArray() const = 0;
	virtual const AssotiativeArray<StringW, JSON*> & GetObject() const = 0;
	virtual const StringW & GetString() const = 0;
};

typedef Array<JSON*> JSON_ARRAY;
typedef AssotiativeArray<StringW, JSON*> JSON_OBJECT;

#endif
