#ifndef _SXString_H_
#define _SXString_H_

#include "C:\DSe\SX\SXstring\defines.h"


//#include "GUIbase.h"
//#include <string.h>
//#include "defines.h"
//#include <cstdio>
/*
unsigned long SXStrFind(char* src_str, char *str, unsigned long pos);
unsigned long SXStrReplace(char* src_str, char *str, char *replace, unsigned long pos);
unsigned long SXStrReplaceAll(char* src_str, char *str, char *replace);
char * SXStrSubStr(char* src_str, unsigned long pos, unsigned long lenght);
char * SXStrDelete(char* src_str, unsigned long pos, unsigned long lenght);
*/

class SXStringW;

class SXString
{
public:
	friend class SXStringW;
	SXString();
	SXString(const char *	str);
	SXString(const char		sym);
	SXString(const int		num);
	SXString(const unsigned short		num);
	SXString(const unsigned long	num);
	SXString(const long		num);
	SXString(const double	num);
	SXString(const bool		bf);
	SXString(const SXString & str);
	SXString(const SXString * str);

	~SXString();

	void Release();

	////////////////////////////////////
	//операции сложения строк
	//возвращается образуется новый объект SXString 
	//который и содержит результат сложения строк

	SXString operator+(const SXString & str) const;
	SXString operator+(const SXString * str);

	SXString operator+(const char *	str);
	SXString operator+(const char &	sym);
	SXString operator+(const int &	num);
	SXString operator+(const unsigned short &	num);
	SXString operator+(const unsigned long &num);
	SXString operator+(const long &	num);
	SXString operator+(const double &num);
	SXString operator+(const bool &	bf);

	////////////////////////////////////
	//операции присвоения

	SXString & operator=(const SXString & str);
	SXString & operator=(const SXString * str);

	SXString & operator=(const char *	str);
	SXString & operator=(const char &	sym);
	SXString & operator=(const int &	num);
	SXString & operator=(const unsigned short &	num);
	SXString & operator=(const unsigned long &	num);
	SXString & operator=(const long &	num);
	SXString & operator=(const double &	num);
	SXString & operator=(const bool &	bf);

	////////////////////////////////////
	//операции сложения строк объекта к которому применяется сложение и слагаемого
	SXString & operator+=(const SXString &str);
	SXString & operator+=(const SXString *str);

	SXString & operator+=(const char *	str);
	SXString & operator+=(const char &	sym);
	SXString & operator+=(const int &	num);
	SXString & operator+=(const unsigned short &	num);
	SXString & operator+=(const unsigned long &	num);
	SXString & operator+=(const long &	num);
	SXString & operator+=(const double &num);
	SXString & operator+=(const bool &	bf);

	////////////////////////////////////
	//искоючить первое нахождение из строки
	//возвращается новый объект SXString, который содержит результат
	SXString operator-(const SXString & str);
	SXString operator-(const SXString * str);

	SXString operator-(const char *	str);
	SXString operator-(const char &	sym);
	SXString operator-(const int &	num);
	SXString operator-(const unsigned short &	num);
	SXString operator-(const unsigned long &	num);
	SXString operator-(const long &	num);
	SXString operator-(const double &	num);
	SXString operator-(const bool &	bf);

	////////////////////////////////////
	//исключает первое нахождение из данного объекта
	SXString & operator-=(const SXString &str);
	SXString & operator-=(const SXString *str);

	SXString & operator-=(const char *	str);
	SXString & operator-=(const char &	sym);
	SXString & operator-=(const int &	num);
	SXString & operator-=(const unsigned short &	num);
	SXString & operator-=(const unsigned long &	num);
	SXString & operator-=(const long &	num);
	SXString & operator-=(const double &num);
	SXString & operator-=(const bool &	bf);

	//////////////////////////////////////
	//исключить все похожие нахождения
	//возвращается новый объект SXString
	SXString operator / (const SXString &str);
	SXString operator / (const SXString *str);

	SXString operator/(const char *	str);
	SXString operator/(const char &	sym);
	SXString operator/(const int &	num);
	SXString operator/(const unsigned short &	num);
	SXString operator/(const unsigned long &	num);
	SXString operator/(const long &	num);
	SXString operator/(const double &	num);
	SXString operator/(const bool &	bf);

	////////////////////////////////////
	//исключить все похожие нахождения
	SXString & operator/=(const SXString &str);
	SXString & operator/=(const SXString *str);

	SXString & operator/=(const char *	str);
	SXString & operator/=(const char &	sym);
	SXString & operator/=(const int &	num);
	SXString & operator/=(const unsigned short &	num);
	SXString & operator/=(const unsigned long &	num);
	SXString & operator/=(const long &	num);
	SXString & operator/=(const double &num);
	SXString & operator/=(const bool &	bf);

	////////////////////////////////////

	bool operator==(const SXString &str) const;
	bool operator==(const SXString *str) const;

	bool operator==(const char *	str) const;
	bool operator==(const char &	sym) const;
	bool operator==(const int &		num) const;
	bool operator==(const unsigned short &	num) const;
	bool operator==(const unsigned long &	num) const;
	bool operator==(const long &	num) const;
	bool operator==(const double &	num) const;
	bool operator==(const bool &	bf) const;

	//////////////////////////////////////

	bool operator!= (const SXString &str) const;
	bool operator!= (const SXString *str) const;

	bool operator!=(const char *	str) const;
	bool operator!=(const char &	sym) const;
	bool operator!=(const int &		num) const;
	bool operator!=(const unsigned short &	num) const;
	bool operator!=(const unsigned long &	num) const;
	bool operator!=(const long &	num) const;
	bool operator!=(const double &	num) const;
	bool operator!=(const bool &	bf) const;

	//////////////////////////////////////
	char & operator[](const unsigned long & num);
	//////////////////////////////////////

	unsigned long length() const;
	unsigned long find(const char & c, unsigned long pos = 0) const;
	unsigned long find(const char * str, unsigned long pos = 0) const;
	unsigned long find(const SXString & str, unsigned long pos = 0) const;

	unsigned long find_last_of(const char & c, unsigned long pos = 0) const;
	unsigned long find_last_of(const char * str, unsigned long pos = 0) const;
	unsigned long find_last_of(const SXString & str, unsigned long pos = 0) const;

	unsigned long Replace(const char * str, const char * replace, unsigned long pos);
	unsigned long Replace(const SXString & str, const SXString & replace, unsigned long pos);

	unsigned long ReplaceAll(const char * str, const char * replace);
	unsigned long ReplaceAll(const SXString & str, const SXString & replace);

	//char * SubStr(unsigned long pos, unsigned long lenght); // так делать не нужно
	SXString substr(unsigned long pos, unsigned long lenght = 0);
	unsigned int Delete(unsigned long pos, unsigned long lenght);

	const char * c_str() const;

	void Reserve(int length);

	int		ToInt() const;
	long	ToLongInt() const;
	unsigned long	ToUnsLongInt() const;
	double	ToDouble() const;
	bool	ToBool() const;

	operator SXStringW() const;

protected:
	char * string;
};

bool operator<(const SXString & a, const SXString & b);


class SXStringW
{
public:
	friend class SXString;
	SXStringW();
	SXStringW(const wchar_t *	str);
	SXStringW(const wchar_t		sym);
	SXStringW(const int		num);
	SXStringW(const unsigned short		num);
	SXStringW(const unsigned long	num);
	SXStringW(const long		num);
	SXStringW(const double	num);
	SXStringW(const bool		bf);
	SXStringW(const SXStringW & str);
	SXStringW(const SXStringW * str);

	~SXStringW();

	void Release();

	////////////////////////////////////
	//операции сложения строк
	//возвращается образуется новый объект SXStringW 
	//который и содержит результат сложения строк

	SXStringW operator+(const SXStringW & str) const;
	SXStringW operator+(const SXStringW * str);

	SXStringW operator+(const wchar_t *	str);
	SXStringW operator+(const wchar_t &	sym);
	SXStringW operator+(const int &	num);
	SXStringW operator+(const unsigned short &	num);
	SXStringW operator+(const unsigned long &num);
	SXStringW operator+(const long &	num);
	SXStringW operator+(const double &num);
	SXStringW operator+(const bool &	bf);

	////////////////////////////////////
	//операции присвоения

	SXStringW & operator=(const SXStringW & str);
	SXStringW & operator=(const SXStringW * str);

	SXStringW & operator=(const wchar_t *	str);
	SXStringW & operator=(const wchar_t &	sym);
	SXStringW & operator=(const int &	num);
	SXStringW & operator=(const unsigned short &	num);
	SXStringW & operator=(const unsigned long &	num);
	SXStringW & operator=(const long &	num);
	SXStringW & operator=(const double &	num);
	SXStringW & operator=(const bool &	bf);

	////////////////////////////////////
	//операции сложения строк объекта к которому применяется сложение и слагаемого
	SXStringW & operator+=(const SXStringW &str);
	SXStringW & operator+=(const SXStringW *str);

	SXStringW & operator+=(const wchar_t *	str);
	SXStringW & operator+=(const wchar_t &	sym);
	SXStringW & operator+=(const int &	num);
	SXStringW & operator+=(const unsigned short &	num);
	SXStringW & operator+=(const unsigned long &	num);
	SXStringW & operator+=(const long &	num);
	SXStringW & operator+=(const double &num);
	SXStringW & operator+=(const bool &	bf);

	////////////////////////////////////
	//искоючить первое нахождение из строки
	//возвращается новый объект SXStringW, который содержит результат
	SXStringW operator-(const SXStringW & str);
	SXStringW operator-(const SXStringW * str);

	SXStringW operator-(const wchar_t *	str);
	SXStringW operator-(const wchar_t &	sym);
	SXStringW operator-(const int &	num);
	SXStringW operator-(const unsigned short &	num);
	SXStringW operator-(const unsigned long &	num);
	SXStringW operator-(const long &	num);
	SXStringW operator-(const double &	num);
	SXStringW operator-(const bool &	bf);

	////////////////////////////////////
	//исключает первое нахождение из данного объекта
	SXStringW & operator-=(const SXStringW &str);
	SXStringW & operator-=(const SXStringW *str);

	SXStringW & operator-=(const wchar_t *	str);
	SXStringW & operator-=(const wchar_t &	sym);
	SXStringW & operator-=(const int &	num);
	SXStringW & operator-=(const unsigned short &	num);
	SXStringW & operator-=(const unsigned long &	num);
	SXStringW & operator-=(const long &	num);
	SXStringW & operator-=(const double &num);
	SXStringW & operator-=(const bool &	bf);

	//////////////////////////////////////
	//исключить все похожие нахождения
	//возвращается новый объект SXStringW
	SXStringW operator / (const SXStringW &str);
	SXStringW operator / (const SXStringW *str);

	SXStringW operator/(const wchar_t *	str);
	SXStringW operator/(const wchar_t &	sym);
	SXStringW operator/(const int &	num);
	SXStringW operator/(const unsigned short &	num);
	SXStringW operator/(const unsigned long &	num);
	SXStringW operator/(const long &	num);
	SXStringW operator/(const double &	num);
	SXStringW operator/(const bool &	bf);

	////////////////////////////////////
	//исключить все похожие нахождения
	SXStringW & operator/=(const SXStringW &str);
	SXStringW & operator/=(const SXStringW *str);

	SXStringW & operator/=(const wchar_t *	str);
	SXStringW & operator/=(const wchar_t &	sym);
	SXStringW & operator/=(const int &	num);
	SXStringW & operator/=(const unsigned short &	num);
	SXStringW & operator/=(const unsigned long &	num);
	SXStringW & operator/=(const long &	num);
	SXStringW & operator/=(const double &num);
	SXStringW & operator/=(const bool &	bf);

	////////////////////////////////////

	bool operator==(const SXStringW &str) const;
	bool operator==(const SXStringW *str) const;

	bool operator==(const wchar_t *	str) const;
	bool operator==(const wchar_t &	sym) const;
	bool operator==(const int &		num) const;
	bool operator==(const unsigned short &	num) const;
	bool operator==(const unsigned long &	num) const;
	bool operator==(const long &	num) const;
	bool operator==(const double &	num) const;
	bool operator==(const bool &	bf) const;

	//////////////////////////////////////

	bool operator!= (const SXStringW &str) const;
	bool operator!= (const SXStringW *str) const;

	bool operator!=(const wchar_t *	str) const;
	bool operator!=(const wchar_t &	sym) const;
	bool operator!=(const int &		num) const;
	bool operator!=(const unsigned short &	num) const;
	bool operator!=(const unsigned long &	num) const;
	bool operator!=(const long &	num) const;
	bool operator!=(const double &	num) const;
	bool operator!=(const bool &	bf) const;

	//////////////////////////////////////
	wchar_t & operator[](const unsigned long & num);
	const wchar_t & operator[](const unsigned long & num) const;
	//////////////////////////////////////

	void insert(unsigned int pos, const wchar_t * data);
	void insert(unsigned int pos, const SXStringW & data);

	unsigned long length() const;
	unsigned long find(const wchar_t & c, unsigned long pos = 0) const;
	unsigned long find(const wchar_t * str, unsigned long pos = 0) const;
	unsigned long find(const SXStringW & str, unsigned long pos = 0) const;

	unsigned long find_last_of(const wchar_t & c, unsigned long pos = 0) const;
	unsigned long find_last_of(const wchar_t * str, unsigned long pos = 0) const;
	unsigned long find_last_of(const SXStringW & str, unsigned long pos = 0) const;

	unsigned long Replace(const wchar_t * str, const wchar_t * replace, unsigned long pos);
	unsigned long Replace(const SXStringW & str, const SXStringW & replace, unsigned long pos);

	unsigned long ReplaceAll(const wchar_t * str, const wchar_t * replace);
	unsigned long ReplaceAll(const SXStringW & str, const SXStringW & replace);

	SXStringW substr(unsigned long pos, unsigned long lenght = 0) const;
	unsigned int Delete(unsigned long pos, unsigned long lenght);

	const wchar_t * c_str() const;

	void Reserve(int length);

	int		ToInt() const;
	long	ToLongInt() const;
	unsigned long	ToUnsLongInt() const;
	double	ToDouble() const;
	bool	ToBool() const;

	operator SXString() const;

protected:
	wchar_t * string;
};

bool operator<(const SXStringW & a, const SXStringW & b);

#endif
