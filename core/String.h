#ifndef STRING_H
#define STRING_H

#include <cstdio>
#include <string.h>

#define SX_SAFE_DELETE_A(obj) delete obj;obj=0;

long StrFind(const char* src_str, char *str, unsigned long pos);
long StrGetStr(const char* src_str, char *str,long size, unsigned long pos);
long StrReplace(char* src_str, char *str, char *replace, unsigned long pos);
long StrReplaceAll(char* src_str, char *str, char *replace);
char * StrSubStr(const char* src_str, unsigned long pos, unsigned long lenght);
char * StrDelete(char* src_str, unsigned long pos, unsigned long lenght);


//проверка строки на валидность данных
inline bool StrValidate(const char* str);
//вырезка из пути имени файла
void StrCutName(const char* path,char* name);
void StrCutNameNEx(const char* path,char* name);


class StringW;

class String
{
public:
	friend class StringW;
	String();
	String(const char *	str);
	String(const char		sym);
	String(const int		num);
	String(const unsigned short		num);
	String(const unsigned long	num);
	String(const long		num);
	String(const double	num);
	String(const bool		bf);
	String(const String & str);
	String(const String * str);

	~String();

	void Release();

	////////////////////////////////////
	//операции сложения строк
	//возвращается образуется новый объект String 
	//который и содержит результат сложения строк

	String operator+(const String & str) const;
	String operator+(const String * str);

	String operator+(const char *	str);
	String operator+(const char &	sym);
	String operator+(const int &	num);
	String operator+(const unsigned short &	num);
	String operator+(const unsigned long &num);
	String operator+(const long &	num);
	String operator+(const double &num);
	String operator+(const bool &	bf);

	////////////////////////////////////
	//операции присвоения

	String & operator=(const String & str);
	String & operator=(const String * str);

	String & operator=(const char *	str);
	String & operator=(const char &	sym);
	String & operator=(const int &	num);
	String & operator=(const unsigned short &	num);
	String & operator=(const unsigned long &	num);
	String & operator=(const long &	num);
	String & operator=(const double &	num);
	String & operator=(const bool &	bf);

	////////////////////////////////////
	//операции сложения строк объекта к которому применяется сложение и слагаемого
	String & operator+=(const String &str);
	String & operator+=(const String *str);

	String & operator+=(const char *	str);
	String & operator+=(const char &	sym);
	String & operator+=(const int &	num);
	String & operator+=(const unsigned short &	num);
	String & operator+=(const unsigned long &	num);
	String & operator+=(const long &	num);
	String & operator+=(const double &num);
	String & operator+=(const bool &	bf);

	////////////////////////////////////
	//искоючить первое нахождение из строки
	//возвращается новый объект String, который содержит результат
	String operator-(const String & str);
	String operator-(const String * str);

	String operator-(const char *	str);
	String operator-(const char &	sym);
	String operator-(const int &	num);
	String operator-(const unsigned short &	num);
	String operator-(const unsigned long &	num);
	String operator-(const long &	num);
	String operator-(const double &	num);
	String operator-(const bool &	bf);

	////////////////////////////////////
	//исключает первое нахождение из данного объекта
	String & operator-=(const String &str);
	String & operator-=(const String *str);

	String & operator-=(const char *	str);
	String & operator-=(const char &	sym);
	String & operator-=(const int &	num);
	String & operator-=(const unsigned short &	num);
	String & operator-=(const unsigned long &	num);
	String & operator-=(const long &	num);
	String & operator-=(const double &num);
	String & operator-=(const bool &	bf);

	//////////////////////////////////////
	//исключить все похожие нахождения
	//возвращается новый объект String
	String operator / (const String &str);
	String operator / (const String *str);

	String operator/(const char *	str);
	String operator/(const char &	sym);
	String operator/(const int &	num);
	String operator/(const unsigned short &	num);
	String operator/(const unsigned long &	num);
	String operator/(const long &	num);
	String operator/(const double &	num);
	String operator/(const bool &	bf);

	////////////////////////////////////
	//исключить все похожие нахождения
	String & operator/=(const String &str);
	String & operator/=(const String *str);

	String & operator/=(const char *	str);
	String & operator/=(const char &	sym);
	String & operator/=(const int &	num);
	String & operator/=(const unsigned short &	num);
	String & operator/=(const unsigned long &	num);
	String & operator/=(const long &	num);
	String & operator/=(const double &num);
	String & operator/=(const bool &	bf);

	////////////////////////////////////

	bool operator==(const String &str) const;
	bool operator==(const String *str) const;

	bool operator==(const char *	str) const;
	bool operator==(const char &	sym) const;
	bool operator==(const int &		num) const;
	bool operator==(const unsigned short &	num) const;
	bool operator==(const unsigned long &	num) const;
	bool operator==(const long &	num) const;
	bool operator==(const double &	num) const;
	bool operator==(const bool &	bf) const;

	//////////////////////////////////////

	bool operator!= (const String &str) const;
	bool operator!= (const String *str) const;

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
	unsigned long find(const String & str, unsigned long pos = 0) const;

	unsigned long find_last_of(const char & c, unsigned long pos = 0) const;
	unsigned long find_last_of(const char * str, unsigned long pos = 0) const;
	unsigned long find_last_of(const String & str, unsigned long pos = 0) const;

	unsigned long Replace(const char * str, const char * replace, unsigned long pos);
	unsigned long Replace(const String & str, const String & replace, unsigned long pos);

	unsigned long ReplaceAll(const char * str, const char * replace);
	unsigned long ReplaceAll(const String & str, const String & replace);

	//char * SubStr(unsigned long pos, unsigned long lenght); // так делать не нужно
	String substr(unsigned long pos, unsigned long lenght = 0);
	unsigned int Delete(unsigned long pos, unsigned long lenght);
	void insert(unsigned long pos,const char* str);

	const char * c_str() const;

	void Reserve(int length);

	int		ToInt() const;
	long	ToLongInt() const;
	unsigned long	ToUnsLongInt() const;
	double	ToDouble() const;
	bool	ToBool() const;

	operator StringW() const;

protected:
	char * string;
};

bool operator<(const String & a, const String & b);


class StringW
{
public:
	friend class String;
	StringW();
	StringW(const wchar_t *	str);
	StringW(const wchar_t		sym);
	StringW(const int		num);
	StringW(const unsigned short		num);
	StringW(const unsigned long	num);
	StringW(const long		num);
	StringW(const double	num);
	StringW(const bool		bf);
	StringW(const StringW & str);
	StringW(const StringW * str);

	~StringW();

	void Release();

	////////////////////////////////////
	//операции сложения строк
	//возвращается образуется новый объект StringW 
	//который и содержит результат сложения строк

	StringW operator+(const StringW & str) const;
	StringW operator+(const StringW * str);

	StringW operator+(const wchar_t *	str);
	StringW operator+(const wchar_t &	sym);
	StringW operator+(const int &	num);
	StringW operator+(const unsigned short &	num);
	StringW operator+(const unsigned long &num);
	StringW operator+(const long &	num);
	StringW operator+(const double &num);
	StringW operator+(const bool &	bf);

	////////////////////////////////////
	//операции присвоения

	StringW & operator=(const StringW & str);
	StringW & operator=(const StringW * str);

	StringW & operator=(const wchar_t *	str);
	StringW & operator=(const wchar_t &	sym);
	StringW & operator=(const int &	num);
	StringW & operator=(const unsigned short &	num);
	StringW & operator=(const unsigned long &	num);
	StringW & operator=(const long &	num);
	StringW & operator=(const double &	num);
	StringW & operator=(const bool &	bf);

	////////////////////////////////////
	//операции сложения строк объекта к которому применяется сложение и слагаемого
	StringW & operator+=(const StringW &str);
	StringW & operator+=(const StringW *str);

	StringW & operator+=(const wchar_t *	str);
	StringW & operator+=(const wchar_t &	sym);
	StringW & operator+=(const int &	num);
	StringW & operator+=(const unsigned short &	num);
	StringW & operator+=(const unsigned long &	num);
	StringW & operator+=(const long &	num);
	StringW & operator+=(const double &num);
	StringW & operator+=(const bool &	bf);

	////////////////////////////////////
	//искоючить первое нахождение из строки
	//возвращается новый объект StringW, который содержит результат
	StringW operator-(const StringW & str);
	StringW operator-(const StringW * str);

	StringW operator-(const wchar_t *	str);
	StringW operator-(const wchar_t &	sym);
	StringW operator-(const int &	num);
	StringW operator-(const unsigned short &	num);
	StringW operator-(const unsigned long &	num);
	StringW operator-(const long &	num);
	StringW operator-(const double &	num);
	StringW operator-(const bool &	bf);

	////////////////////////////////////
	//исключает первое нахождение из данного объекта
	StringW & operator-=(const StringW &str);
	StringW & operator-=(const StringW *str);

	StringW & operator-=(const wchar_t *	str);
	StringW & operator-=(const wchar_t &	sym);
	StringW & operator-=(const int &	num);
	StringW & operator-=(const unsigned short &	num);
	StringW & operator-=(const unsigned long &	num);
	StringW & operator-=(const long &	num);
	StringW & operator-=(const double &num);
	StringW & operator-=(const bool &	bf);

	//////////////////////////////////////
	//исключить все похожие нахождения
	//возвращается новый объект StringW
	StringW operator / (const StringW &str);
	StringW operator / (const StringW *str);

	StringW operator/(const wchar_t *	str);
	StringW operator/(const wchar_t &	sym);
	StringW operator/(const int &	num);
	StringW operator/(const unsigned short &	num);
	StringW operator/(const unsigned long &	num);
	StringW operator/(const long &	num);
	StringW operator/(const double &	num);
	StringW operator/(const bool &	bf);

	////////////////////////////////////
	//исключить все похожие нахождения
	StringW & operator/=(const StringW &str);
	StringW & operator/=(const StringW *str);

	StringW & operator/=(const wchar_t *	str);
	StringW & operator/=(const wchar_t &	sym);
	StringW & operator/=(const int &	num);
	StringW & operator/=(const unsigned short &	num);
	StringW & operator/=(const unsigned long &	num);
	StringW & operator/=(const long &	num);
	StringW & operator/=(const double &num);
	StringW & operator/=(const bool &	bf);

	////////////////////////////////////

	bool operator==(const StringW &str) const;
	bool operator==(const StringW *str) const;

	bool operator==(const wchar_t *	str) const;
	bool operator==(const wchar_t &	sym) const;
	bool operator==(const int &		num) const;
	bool operator==(const unsigned short &	num) const;
	bool operator==(const unsigned long &	num) const;
	bool operator==(const long &	num) const;
	bool operator==(const double &	num) const;
	bool operator==(const bool &	bf) const;

	//////////////////////////////////////

	bool operator!= (const StringW &str) const;
	bool operator!= (const StringW *str) const;

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
	void insert(unsigned int pos, const StringW & data);

	unsigned long length() const;
	unsigned long find(const wchar_t & c, unsigned long pos = 0) const;
	unsigned long find(const wchar_t * str, unsigned long pos = 0) const;
	unsigned long find(const StringW & str, unsigned long pos = 0) const;

	unsigned long find_last_of(const wchar_t & c, unsigned long pos = 0) const;
	unsigned long find_last_of(const wchar_t * str, unsigned long pos = 0) const;
	unsigned long find_last_of(const StringW & str, unsigned long pos = 0) const;

	unsigned long Replace(const wchar_t * str, const wchar_t * replace, unsigned long pos);
	unsigned long Replace(const StringW & str, const StringW & replace, unsigned long pos);

	unsigned long ReplaceAll(const wchar_t * str, const wchar_t * replace);
	unsigned long ReplaceAll(const StringW & str, const StringW & replace);

	StringW substr(unsigned long pos, unsigned long lenght = 0) const;
	unsigned int Delete(unsigned long pos, unsigned long lenght);

	const wchar_t * c_str() const;

	void Reserve(int length);

	int		ToInt() const;
	long	ToLongInt() const;
	unsigned long	ToUnsLongInt() const;
	double	ToDouble() const;
	bool	ToBool() const;

	operator String() const;

protected:
	wchar_t * string;
};

bool operator<(const StringW & a, const StringW & b);

#endif
