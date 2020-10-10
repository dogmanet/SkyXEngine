#ifndef __CSSPROPERTY_H
#define __CSSPROPERTY_H

#include <xcommon/gui/IXCSSProperty.h>

enum CSS_AST_NODE_TYPE
{
	CSSASTNT_UNKNOWN,
	CSSASTNT_CONST_INT,
	CSSASTNT_CONST_FLOAT,
	CSSASTNT_CONST_STRING,
	CSSASTNT_LIST_4,
	CSSASTNT_LIST_8,
	CSSASTNT_FUNCTION,
	CSSASTNT_KW,
};

struct CSSASTNode
{
	CSS_AST_NODE_TYPE type = CSSASTNT_UNKNOWN;
	UINT uStructSize = 0;
};

#define INIT_CSS_AST_NODE(cls, typ, ...) cls(){type=typ;uStructSize=sizeof(cls); __VA_ARGS__}

struct CSSASTNodeConstInt: public CSSASTNode
{
	int iValue = 0;

	INIT_CSS_AST_NODE(CSSASTNodeConstInt, CSSASTNT_CONST_INT);
};

struct CSSASTNodeConstFloat: public CSSASTNode
{
	float fValue = 0.0f;
	XCSS_VAR_DIM dim = XCSS_VAR_DIM_NONE;

	INIT_CSS_AST_NODE(CSSASTNodeConstFloat, CSSASTNT_CONST_FLOAT);
};

struct CSSASTNodeConstString: public CSSASTNode
{
	int iStringIndex = -1; // or handler?

	INIT_CSS_AST_NODE(CSSASTNodeConstString, CSSASTNT_CONST_STRING);
};

struct CSSASTNodeKeyword: public CSSASTNode
{
	int iStringIndex = -1; // or handler?

	INIT_CSS_AST_NODE(CSSASTNodeKeyword, CSSASTNT_KW);
};

struct CSSASTNodeList4: public CSSASTNode
{
	CSSASTNode *apNodes[4];
	
	INIT_CSS_AST_NODE(CSSASTNodeList4, CSSASTNT_LIST_4, 
		for(UINT i = 0; i < ARRAYSIZE(apNodes); ++i)
		{
			apNodes[i] = NULL;
		}
	);
};

struct CSSASTNodeList8: public CSSASTNode
{
	CSSASTNode *apNodes[8];

	INIT_CSS_AST_NODE(CSSASTNodeList8, CSSASTNT_LIST_8,
		for(UINT i = 0; i < ARRAYSIZE(apNodes); ++i)
		{
			apNodes[i] = NULL;
		}
	);
};

struct CSSASTNodeFunction: public CSSASTNode
{
	CSSASTNode *pName = NULL;
	CSSASTNode *pArgs = NULL;

	INIT_CSS_AST_NODE(CSSASTNodeFunction, CSSASTNT_FUNCTION);
};

//##########################################################################

class CCSSProperty: public IXUnknownImplementation<IXCSSProperty>
{
public:
	bool XMETHODCALLTYPE isList() const override;

	void XMETHODCALLTYPE setInt(int iVal) override;
	void XMETHODCALLTYPE setFloat(float fVal, XCSS_VAR_DIM d = XCSS_VAR_DIM_NONE) override;
	void XMETHODCALLTYPE setString(const char *szVal) override;
	void XMETHODCALLTYPE setMatrix(const SMMATRIX &mVal) override;
	void XMETHODCALLTYPE setVariant(int enumItem) override;

	void XMETHODCALLTYPE setFromCSS(const char *szCSSString) override;


	int XMETHODCALLTYPE getInt() const override;
	float XMETHODCALLTYPE getFloat() const override;
	const char* XMETHODCALLTYPE getString() const override;
	const SMMATRIX& XMETHODCALLTYPE getMatrix() const override;
	float4_t XMETHODCALLTYPE getColor() const override;
	int XMETHODCALLTYPE getPX(int iBase) override;

	XCSS_PROPERTY_TYPE XMETHODCALLTYPE getType() const override; //?
	XCSS_VAR_DIM XMETHODCALLTYPE getDim() const override;

	bool XMETHODCALLTYPE isSet() const override;

	void XMETHODCALLTYPE unset() override;
};

class CCSSListProperty: public IXUnknownImplementation<IXCSSListProperty>
{
public:
};

// IXCSSListProperty
#if 0
	CCSSproperty():m_type(TYPE_UNDEFINED), m_isSet(false), m_isChanged(false)
	{
		m_value.i = 0;
	}

	CCSSproperty(TYPE type):m_type(type), m_isSet(false), m_isChanged(true)
	{
		m_value.i = 0;
	}
	CCSSproperty(int value):m_type(TYPE_VAR_INT), m_isSet(true), m_isChanged(true)
	{
		m_value.i = value;
	}
	CCSSproperty(float value):m_type(TYPE_VAR_FLOAT), m_isSet(true), m_isChanged(true)
	{
		m_value.f = value;
	}
	/*CCSSproperty(const StringW & value):m_type(TYPE_VAR_STRING), m_isSet(true), m_isChanged(true)
	{
		int len = value.length();
		m_value.s = new WCHAR[len + 1];
		memcpy(m_value.s, value.c_str(), (len + 1) * sizeof(WCHAR));
	}
	CCSSproperty(const WCHAR * value):m_type(TYPE_VAR_STRING), m_isSet(true), m_isChanged(true)
	{
		int len = lstrlenW(value);
		m_value.s = new WCHAR[len + 1];
		memcpy(m_value.s, value, (len + 1) * sizeof(WCHAR));
	}*/
	CCSSproperty(const SMMATRIX & val):m_type(TYPE_VAR_MATRIX), m_isSet(true), m_isChanged(true)
	{
		memcpy(m_value.m, val.m, sizeof(val.m));
	}

	~CCSSproperty()
	{
		if(m_type == TYPE_VAR_STRING)
		{
			//mem_delete_a(m_value.s);
		}
	}

	TYPE getType() const
	{
		return(m_type);
	}

	void set(int val)
	{
		if(m_type == TYPE_VAR_STRING)
		{
			//mem_delete_a(m_value.s);
		}
		m_isSet = true;
		if(m_type != TYPE_VAR_INT || m_value.i != val)
		{
			m_isChanged = true;
		}
		m_value.i = val;
		m_type = TYPE_VAR_INT;
	}
	void set(float val)
	{
		if(m_type == TYPE_VAR_STRING)
		{
			//mem_delete_a(m_value.s);
		}
		m_isSet = true;
		if(m_type != TYPE_VAR_FLOAT || m_value.f != val)
		{
			m_isChanged = true;
		}
		m_value.f = val;
		m_type = TYPE_VAR_FLOAT;
	}
	void set(const StringW & val)
	{
		set(val.c_str());
	}
	void set(const WCHAR * val)
	{
		int idx = CCSSstringManager::getStringID(val);
		if(m_type == TYPE_VAR_STRING)
		{
			if(m_value.i != idx)
			{
				m_isChanged = true;
			}
			//mem_delete_a(m_value.s);
		}
		else
		{
			m_isChanged = true;
		}
		m_isSet = true;
		m_value.i = idx;
		/*int len = lstrlenW(val);
		m_value.s = new WCHAR[len + 1];
		memcpy(m_value.s, val, (len + 1) * sizeof(WCHAR));*/
		m_type = TYPE_VAR_STRING;
	}
	void set(const SMMATRIX & val)
	{
		if(m_type == TYPE_VAR_STRING)
		{
			//mem_delete_a(m_value.s);
		}
		m_isSet = true;
		if(m_type != TYPE_VAR_MATRIX || memcmp(m_value.m, val.m, sizeof(val.m)) != 0)
		{
			m_isChanged = true;
		}
		memcpy(m_value.m, val.m, sizeof(val.m));
		m_type = TYPE_VAR_MATRIX;
	}

	void setExt(int val)
	{
		if(m_value.i != val)
		{
			m_isChanged = true;
		}
		m_value.i = val;
		m_isSet = true;
	}

	int getInt() const
	{
		if(m_type == gui::css::ICSSproperty::TYPE_VAR_FLOAT)
		{
			return((int)m_value.f);
		}
		return(m_value.i);
	}
	float getFloat() const
	{
		if(m_type == gui::css::ICSSproperty::TYPE_VAR_INT)
		{
			return((float)m_value.i);
		}
		return(m_value.f);
	}
	const WCHAR * getString() const
	{
		return(CCSSstringManager::getString(m_value.i));
	}
	SMMATRIX getMatrix() const
	{
		SMMATRIX m;
		memcpy(m.m, m_value.m, sizeof(m_value.m));
		return(m);
	}

	void setDim(DIM d)
	{
		m_dim = d;
	}

	DIM getDim() const
	{
		return(m_dim);
	}

	void setChanged(bool b)
	{
		m_isChanged = b;
	}

	void unChange()
	{
		m_isChanged = false;
	}

	float4_t getColor()const
	{
		return(float4_t(
			(float)(((UINT)m_value.i & 0xFF000000) >> 24) / 255.0f,
			(float)(((UINT)m_value.i & 0x00FF0000) >> 16) / 255.0f,
			(float)(((UINT)m_value.i & 0x0000FF00) >> 8) / 255.0f,
			(float)(((UINT)m_value.i & 0x000000FF)) / 255.0f
			));
	}

	bool isSet() const
	{
		return(m_isSet);
	}

	bool isChanged() const
	{
		return(m_isChanged);
	}

	bool operator==(const ICSSproperty &_b)
	{
		const CCSSproperty * b = (CCSSproperty*)&_b;
		bool bbb;
		/*if(m_type == TYPE_VAR_STRING)
		{
			bbb = lstrcmpW(m_value.s, b->m_value.s) == 0;
		}
		else */if(m_type == TYPE_VAR_MATRIX)
		{
			bbb = memcmp(m_value.m, b->m_value.m, sizeof(m_value.m)) == 0;
		}
		else
		{
			bbb = m_value.i == b->m_value.i;
		}
		return(m_type == b->m_type && m_dim == b->m_dim && bbb);
	}

	static StringW getDimName(DIM dim)
	{
		switch(dim)
		{
		case DIM_EM:
			return(L"em");
		case DIM_MM:
			return(L"mm");
		case DIM_PC:
			return(L"%");
		case DIM_PT:
			return(L"pt");
		case DIM_PX:
			return(L"px");
		}
		return(L"");
	}

	CCSSproperty & operator=(const CCSSproperty & p)
	{
		if(!(*this == p))
		{
			if(m_type == TYPE_VAR_STRING)
			{
				//mem_delete_a(m_value.s);
			}
			/*if(p.m_type == TYPE_VAR_STRING)
			{
				m_value.s = new WCHAR[wcslen(p.m_value.s) + 1];
				memcpy(m_value.s, p.m_value.s, (wcslen(p.m_value.s) + 1) * sizeof(WCHAR));
				m_type = TYPE_VAR_STRING;
			}
			else */if(p.m_type == TYPE_VAR_MATRIX)
			{
				memcpy(m_value.m, p.m_value.m, sizeof(p.m_value.m));
			}
			else
			{
				m_value.i = p.m_value.i;
			}
		}
		m_isSet = p.m_isSet;
		m_type = p.m_type;
		m_dim = p.m_dim;
		m_isChanged = p.m_isChanged;
		return(*this);
	}

	void unset()
	{
		if(m_type == TYPE_VAR_MATRIX)
		{
			set(SMMatrixIdentity());
			m_isSet = false;
			return;
		}
		TYPE t = m_type;
		set(0);
		m_isSet = false;
		if(m_type != TYPE_VAR_STRING &&
			m_type != TYPE_VAR_INT &&
			m_type != TYPE_VAR_FLOAT)
		{
			m_type = t;
		}
		else
		{
			m_type = TYPE_UNDEFINED;
		}

		if(m_flags & FLAG_DO_TRANSITION)
		{
			m_flags ^= FLAG_DO_TRANSITION;
		}
	}

	UINT getFlags()
	{
		return(m_flags);
	}

	void setFlags(UINT f)
	{
		m_flags = f;
	}

	int getPX(int base)
	{
		if(m_type == css::ICSSproperty::TYPE_UNDEFINED)
		{
			return(0);
		}
		float falue = getFloat();
		switch(getDim())
		{
		case css::ICSSproperty::DIM_PC:
			return((int)_round((float)base * (falue * 0.01f)));
		case css::ICSSproperty::DIM_PX:
		case css::ICSSproperty::DIM_NONE:
			return((int)_round(falue));
		case css::ICSSproperty::DIM_PT:
			return((int)_round((float)falue * 4.0f / 3.0f));
		case css::ICSSproperty::DIM_MM:
			return((int)_round((float)falue * 3.7795f));
			break;
		default:
			return(base);
		}
	}

	void setupTransition(const ICSSproperty * pOldProperty, dom::IDOMdocument *pDoc);

	void allowTransition(float fDuration, float fDelay, TIMING_FUNCTION_e timingFunction)
	{
		if(m_flags & ICSSproperty::FLAG_ALLOW_TRANSITION)
		{
			m_flags |= ICSSproperty::FLAG_DO_TRANSITION;
			m_fTransitionSpeed = 1.0f / fDuration;
			m_fTransitionDelay = fDelay;
			m_transitionTimingFunction = timingFunction;
		}
	}

	bool updateTransition(float fTimeDelta)
	{
		if(m_flags & ICSSproperty::FLAG_DO_TRANSITION)
		{
			m_fTransitionFrac += fTimeDelta * m_fTransitionSpeed;
			if(m_fTransitionFrac > 1.0f)
			{
				m_fTransitionFrac = 1.0f;
			}

			//P = (1−t)^3P1 + 3(1−t)^2tP2 + 3(1−t)t^2P3 + t^3P4
			float2 p1(0.0f, 0.0f);
			float2 p2(0.0f, 0.0f);
			float2 p3(1.0f, 1.0f);
			float2 p4(1.0f, 1.0f);

			float t = m_fTransitionFrac;
			switch(m_transitionTimingFunction)
			{
			case TIMING_FUNCTION_EASE:
				p2 = float2(0.25f, 0.1f);
				p3 = float2(0.25f, 1.0f);
				break;
			case TIMING_FUNCTION_EASE_IN:
				p2 = float2(0.42f, 0.0f);
				p3 = float2(1.0f, 1.0f);
				break;
			case TIMING_FUNCTION_EASE_IN_OUT:
				p2 = float2(0.42f, 0.0f);
				p3 = float2(0.58f, 1.0f);
				break;
			case TIMING_FUNCTION_EASE_OUT:
				p2 = float2(0.0f, 0.0f);
				p3 = float2(0.58f, 1.0f);
				break;
			case TIMING_FUNCTION_LINEAR:
				// empty
				break;
			case TIMING_FUNCTION_STEP_START:
				t = 1.0f;
				break;
			case TIMING_FUNCTION_STEP_END:
				if(t < 1.0f)
				{
					t = 0.0f;
				}
				break;
			}

			float2 p = powf(1.0f - t, 3.0f) * p1 + 3.0f * powf(1.0f - t, 2.0f) * t * p2 + 3.0f * (1.0f - t) * powf(t, 2.0f) * p3 + powf(t, 3.0f) * p4;
			t = p.y;

			switch(m_type)
			{
			case TYPE_VAR_FLOAT:
				m_value.f = lerpf(m_valueOld.f, m_valueTo.f, t);
				break;
			case TYPE_VAR_INT:
				if(m_flags & FLAG_PROP_COLOR)
				{
					UINT clr = *((UINT*)(&(m_valueOld.i)));

					UINT r1 = ((clr & 0xFF000000) >> 24);
					UINT g1 = ((clr & 0x00FF0000) >> 16);
					UINT b1 = ((clr & 0x0000FF00) >> 8);
					UINT a1 = (clr & 0x000000FF);

					clr = *((UINT*)(&(m_valueTo.i)));

					UINT r2 = ((clr & 0xFF000000) >> 24);
					UINT g2 = ((clr & 0x00FF0000) >> 16);
					UINT b2 = ((clr & 0x0000FF00) >> 8);
					UINT a2 = (clr & 0x000000FF);

					r1 = (int)roundf(lerpf(r1, r2, t));
					g1 = (int)roundf(lerpf(g1, g2, t));
					b1 = (int)roundf(lerpf(b1, b2, t));
					a1 = (int)roundf(lerpf(a1, a2, t));

					*((UINT*)(&(m_value.i))) = (r1 << 24) | (g1 << 16) | (b1 << 8) | a1;
				}
				else
				{
					m_value.i = (int)roundf(lerpf(m_valueOld.i, m_valueTo.i, t));
				}
				break;
			case TYPE_VAR_MATRIX:
				for(int i = 0; i < 4; ++i)
				{
					for(int j = 0; j < 4; ++j)
					{
						m_value.m[i][j] = lerpf(m_valueOld.m[i][j], m_valueTo.m[i][j], t);
					}
				}
			}
			setChanged(true);
			return(m_fTransitionFrac == 1.0f);
		}
		return(true);
	}

	void cancelTransitions(dom::IDOMdocument *pDoc);

protected:
	TYPE m_type;
	DIM m_dim;
	bool m_isSet;
	bool m_isChanged;
	UINT m_flags;
	union
	{
		int i;
		float f;
		WCHAR * s;
		float m[4][4];
	} 
		m_value, //!< Действующее значение
		m_valueOld, //!< Начальное значение, для transiton
		m_valueTo; //!< Конечное значение, для transition

	float m_fTransitionFrac;
	float m_fTransitionDelay;
	float m_fTransitionSpeed;
	TIMING_FUNCTION_e m_transitionTimingFunction;
};
#endif

#endif
