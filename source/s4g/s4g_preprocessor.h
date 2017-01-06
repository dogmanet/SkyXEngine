#ifndef s4g_preprocessor_h
#define s4g_preprocessor_h

#include <sxtypes.h>
#include <string/string.h>
#include <core/array.h>
#include <core/assotiativearray.h>

class s4g_preprocessor
{
public:
	void Define(const char * def, int len = 0, bool bTemp = false);
	void Undef(const String & def);
	void AddIncPath(const String & s);

	String Process(const char * src, const char * file=0);

	void UndefTemp();

	const char * GetError();
	bool IsError();

//protected:
	struct _define
	{
		Array<String> vArgs;
		bool isMacro;
		String sVal;
		bool isUndef;
		bool isTemp;
	};
	struct _include_path
	{
		String sPath;
	};
	struct _include
	{
		String sText;
		String sPath;
	};
	AssotiativeArray<String, _define> m_mDefs;
	Array<_include_path> m_vIncludes;
	AssotiativeArray<String, _include> m_mIncludes;

	String m_sError;

	bool isDefined(const String & def);
	bool isTrue(const String & expr);
	

	typedef AssotiativeArray<String, bool> ReplList;

	String MakeExpansion(const String & expr);
	String MakeExpansion(const String & expr, ReplList & rlist);

	String ReplArgs(const String & tok, const _define * pDef, const Array<String> & szArgs);

	String CheckDefines(const String & expr);

	String Stringify(const String & expr);

	String GetInclude(const String & name, String * path = 0);
};

#endif
