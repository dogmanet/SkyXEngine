
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __PROPTABLE_H
#define __PROPTABLE_H

#include <common/Math.h>

class CBaseEntity;

#define ENT_FLAG_0 0x00010000
#define ENT_FLAG_1 0x00020000
#define ENT_FLAG_2 0x00040000
#define ENT_FLAG_3 0x00080000
#define ENT_FLAG_4 0x00100000
#define ENT_FLAG_5 0x00200000
#define ENT_FLAG_6 0x00400000
#define ENT_FLAG_7 0x00800000
#define ENT_FLAG_8 0x01000000
#define ENT_FLAG_9 0x02000000
#define ENT_FLAG_10 0x04000000
#define ENT_FLAG_11 0x08000000
#define ENT_FLAG_12 0x10000000
#define ENT_FLAG_13 0x20000000
#define ENT_FLAG_14 0x40000000
#define ENT_FLAG_15 0x80000000

//! типы полей данных
enum PDF_TYPE
{
	PDF_NONE,
	PDF_INT,
	PDF_FLOAT,
	PDF_VECTOR,
	PDF_BOOL,
	PDF_STRING,
	PDF_ANGLES,
	PDF_ENTITY,
	PDF_PARENT,
	PDF_FLAGS,

	PDF_FLAG,

	PDF_OUTPUT
};

//! типы редакторов полей
enum PDE_TYPE
{
	PDE_NONE = 0,
	PDE_TEXTFIELD,
	PDE_COMBOBOX,
	PDE_FILEFIELD,
	PDE_FLAGS
};

enum PDF_FLAG
{
	PDFF_NONE       = 0x00,
	PDFF_NOEXPORT   = 0x01, //!< Не экспортировать поле в файл
	PDFF_NOEDIT     = 0x02, //!< Не отображать поле в редакторе
	PDFF_INPUT      = 0x04, //!< Поле входа
	PDFF_OUTPUT     = 0x08, //!< Поле выхода
	PDFF_MESSAGE    = 0x10, //!< Поле сообщения
};

enum ENT_FLAG
{
	EF_NONE            = 0x0000,
	EF_EXPORT          = 0x0001,
	EF_LEVEL           = 0x0002,
	EF_REMOVED         = 0x0004,
	EF_NO_WORLD_LOOKUP = 0x0008,

	EF_LAST            = 0x8000
};

typedef int CBaseEntity::*fieldtype;

typedef void (CBaseEntity::*PFNFIELDSETV3)(const float3&);
typedef void (CBaseEntity::*PFNFIELDSETF)(float);
typedef void (CBaseEntity::*PFNFIELDSETSZ)(const char*);
typedef void (CBaseEntity::*PFNFIELDSETI)(int);
typedef void (CBaseEntity::*PFNFIELDSETB)(bool);
typedef void (CBaseEntity::*PFNFIELDSETQ)(const SMQuaternion&);
typedef void (CBaseEntity::*PFNFIELDSETE)(CBaseEntity*);
union PFNFIELDSET
{
	PFNFIELDSET():
		__(0)
	{
	}
	PFNFIELDSET(PFNFIELDSETV3 arg):
		v3(arg)
	{
	}
	PFNFIELDSET(PFNFIELDSETF arg):
		f(arg)
	{
	}
	PFNFIELDSET(PFNFIELDSETSZ arg):
		sz(arg)
	{
	}
	PFNFIELDSET(PFNFIELDSETI arg):
		i(arg)
	{
	}
	PFNFIELDSET(PFNFIELDSETB arg):
		b(arg)
	{
	}
	PFNFIELDSET(PFNFIELDSETQ arg):
		q(arg)
	{
	}
	PFNFIELDSET(PFNFIELDSETE arg):
		e(arg)
	{
	}
	PFNFIELDSETV3 v3;
	PFNFIELDSETF f;
	PFNFIELDSETSZ sz;
	PFNFIELDSETI i;
	PFNFIELDSETB b;
	PFNFIELDSETQ q;
	PFNFIELDSETE e;
	int __;
};

struct editor_kv
{
	const char * key;
	const char * value;
};

struct prop_editor_t
{
	PDE_TYPE type;
	void * pData;
};


struct inputdata_t
{
	CBaseEntity *pInflictor; //!< Косвенный активатор (вызвавший эту цепочку активаций)
	CBaseEntity *pActivator; //!< Непосредственный активатор
	PDF_TYPE type; //!< Тип аргумента
	union
	{
		int i;
		bool b;
		float f;
		const char * str;
	}
	parameter;
	float3_t v3Parameter;
};

typedef void(CBaseEntity::*input_func)(inputdata_t * pInputData);

struct propdata_t
{
	propdata_t():
		pField(NULL),
		type(PDF_NONE),
		flags(0),
		szKey(NULL),
		szEdName(NULL),
		editor({})
	{}
	propdata_t(fieldtype f, PDF_TYPE t, int fl, const char *key, const char *edname, prop_editor_t ed):
		pField(f),
		type(t),
		flags(fl),
		szKey(key),
		szEdName(edname),
		editor(ed)
	{}
	propdata_t(fieldtype f, PDF_TYPE t, int fl, const char *key, const char *edname, PFNFIELDSET _fnSet, prop_editor_t ed):
		pField(f),
		type(t),
		flags(fl),
		szKey(key),
		szEdName(edname),
		editor(ed),
		fnSet(_fnSet)
	{}
	propdata_t(input_func d, PDF_TYPE t, int fl, const char *key, const char *edname, prop_editor_t ed):
		fnInput(d),
		type(t),
		flags(fl),
		szKey(key),
		szEdName(edname),
		editor(ed)
	{}
	union
	{
		fieldtype pField;
		input_func fnInput;
	};
	PDF_TYPE type;
	int flags;
	const char * szKey;
	const char * szEdName;
	prop_editor_t editor;
	PFNFIELDSET fnSet;
};


struct input_t
{
	input_func fnInput;
	CBaseEntity *pTarget;
	inputdata_t data;
};

struct named_output_t
{
	float fDelay = 0.0f;
	const char *szTargetName = NULL;
	const char *szTargetInput = NULL;
	const char *szTargetData = NULL;

	int iOutCount = 0;
	input_t *pOutputs = NULL;
};

struct output_t
{
	output_t():
		iOutCount(0),
		pOutputs(NULL),
		pData(NULL),
		bDirty(false)
	{
	}
	void fire(CBaseEntity *pInflictor, CBaseEntity *pActivator);

	bool bDirty;
	int iOutCount;
	named_output_t * pOutputs;
	void * pData;
};

#define FIRE_OUTPUT(output, inflictor) (output).fire((inflictor), this)

struct proptable_t
{
	propdata_t * pData;
	int numFields;
	proptable_t * pBaseProptable;
};

#define ED_COMBO_MAXELS 256

prop_editor_t _GetEditorCombobox(int start, ...);
prop_editor_t _GetEditorFilefield(int start, ...);

#define DECLARE_PROPTABLE() \
	\
	friend class CEntityFactoryMap; \
	friend class CEntityFactory<ThisClass>; \
	static proptable_t m_pPropTable; \
	\
protected:\
	static void InitPropData(); \
	virtual proptable_t * getPropTable(); \
	static proptable_t * SGetPropTable(); \
	static void ReleasePropData();\
public:\
	virtual propdata_t * getField(const char * name);\
private:

#define _BEGIN_PROPTABLE(cls, bclpt) \
proptable_t cls::m_pPropTable = {0,0,0}; \
void cls::ReleasePropData()\
{\
	for(int i = 0; i < m_pPropTable.numFields; ++i)\
	{\
		char *szData = (char*)m_pPropTable.pData[i].editor.pData; mem_delete_a(szData); m_pPropTable.pData[i].editor.pData = NULL; \
	}\
}\
proptable_t * cls::SGetPropTable()\
{\
	if(!m_pPropTable.numFields)\
		InitPropData();\
	return(&m_pPropTable);\
}\
\
proptable_t * cls::getPropTable()\
{\
	if(!m_pPropTable.numFields)\
		InitPropData(); \
	return(&m_pPropTable); \
}\
propdata_t * cls::getField(const char * name)\
{\
	proptable_t * pt = getPropTable();\
	while(pt)\
	{\
		for(int i = 0; i < pt->numFields; ++i)\
		{\
			if(pt->pData[i].szKey && !strcmp(pt->pData[i].szKey, name))\
			{\
				return(&pt->pData[i]);\
			}\
		}\
		pt = pt->pBaseProptable;\
	}\
	return(NULL);\
}\
void cls::InitPropData() \
{ \
	bclpt \
	typedef cls DataClass; \
	static propdata_t pData[] = {propdata_t()

#define BEGIN_PROPTABLE(cls) \
	_BEGIN_PROPTABLE(cls, m_pPropTable.pBaseProptable = BaseClass::SGetPropTable();)

#define BEGIN_PROPTABLE_NOBASE(cls) \
	_BEGIN_PROPTABLE(cls, m_pPropTable.pBaseProptable = NULL;)

#define END_PROPTABLE() \
	}; \
	if(ARRAYSIZE(pData) > 1) \
	{\
		m_pPropTable.pData = &pData[1]; \
		m_pPropTable.numFields = ARRAYSIZE(pData) - 1; \
	} \
	else \
	{ \
		m_pPropTable.pData = pData; \
		m_pPropTable.numFields = 1; \
	} \
} 

#define DECLARE_TRIVIAL_CONSTRUCTOR() ThisClass(CEntityManager * pMgr):BaseClass(pMgr){}
#define DECLARE_CONSTRUCTOR() ThisClass(CEntityManager * pMgr);

const char * GetEmptyString();

#define EDITOR_NONE {PDE_NONE, NULL}}
#define EDITOR_TEXTFIELD {PDE_TEXTFIELD, NULL}}
#define EDITOR_TIMEFIELD EDITOR_TEXTFIELD
#define EDITOR_ANGLES EDITOR_TEXTFIELD
#define EDITOR_FLAGS {PDE_FLAGS, NULL}}

#define EDITOR_COMBOBOX _GetEditorCombobox(0
#define COMBO_OPTION(name, value) , name, value
#define EDITOR_COMBO_END() , NULL)}

#define EDITOR_FILEFIELD _GetEditorFilefield(0
#define FILE_OPTION(name, value) , name, value
#define EDITOR_FILE_END() , NULL)}

#define EDITOR_YESNO EDITOR_COMBOBOX COMBO_OPTION("Yes", "1") COMBO_OPTION("No", "0") EDITOR_COMBO_END()
#define EDITOR_MODEL EDITOR_FILEFIELD FILE_OPTION("Select model", "dse") EDITOR_FILE_END()

#define DEFINE_FIELD_STRING(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_STRING, flags, keyname, edname, editor
#define DEFINE_FIELD_VECTOR(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_VECTOR, flags, keyname, edname, editor
#define DEFINE_FIELD_ANGLES(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_ANGLES, flags, keyname, edname, editor
#define DEFINE_FIELD_INT(field, flags, keyname, edname, editor)    , {(fieldtype)&DataClass::field, PDF_INT,    flags, keyname, edname, editor
#define DEFINE_FIELD_FLOAT(field, flags, keyname, edname, editor)  , {(fieldtype)&DataClass::field, PDF_FLOAT,  flags, keyname, edname, editor
#define DEFINE_FIELD_BOOL(field, flags, keyname, edname, editor)   , {(fieldtype)&DataClass::field, PDF_BOOL,   flags, keyname, edname, editor
#define DEFINE_FIELD_ENTITY(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_ENTITY, flags, keyname, edname, editor
#define DEFINE_FIELD_PARENT(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_PARENT, flags, keyname, edname, editor
#define DEFINE_FIELD_FLAGS(field, flags, keyname, edname, editor)  , {(fieldtype)&DataClass::field, PDF_FLAGS,  flags, keyname, edname, editor

#define DEFINE_FIELD_STRINGFN(field, flags, keyname, edname, fn, editor) , {(fieldtype)&DataClass::field, PDF_STRING, flags, keyname, edname, (void(CBaseEntity::*)(const char*))&ThisClass::fn, editor
#define DEFINE_FIELD_VECTORFN(field, flags, keyname, edname, fn, editor) , {(fieldtype)&DataClass::field, PDF_VECTOR, flags, keyname, edname, (void(CBaseEntity::*)(const float3&))&ThisClass::fn, editor
#define DEFINE_FIELD_ANGLESFN(field, flags, keyname, edname, fn, editor) , {(fieldtype)&DataClass::field, PDF_ANGLES, flags, keyname, edname, (void(CBaseEntity::*)(const SMQuaternion&))&ThisClass::fn, editor
#define DEFINE_FIELD_INTFN(field, flags, keyname, edname, fn, editor)    , {(fieldtype)&DataClass::field, PDF_INT,    flags, keyname, edname, (void(CBaseEntity::*)(int))&ThisClass::fn, editor
#define DEFINE_FIELD_FLOATFN(field, flags, keyname, edname, fn, editor)  , {(fieldtype)&DataClass::field, PDF_FLOAT,  flags, keyname, edname, (void(CBaseEntity::*)(float))&ThisClass::fn, editor
#define DEFINE_FIELD_BOOLFN(field, flags, keyname, edname, fn, editor)   , {(fieldtype)&DataClass::field, PDF_BOOL,   flags, keyname, edname, (void(CBaseEntity::*)(bool))&ThisClass::fn, editor
#define DEFINE_FIELD_ENTITYFN(field, flags, keyname, edname, fn, editor) , {(fieldtype)&DataClass::field, PDF_ENTITY, flags, keyname, edname, (void(CBaseEntity::*)(CBaseEntity*))&ThisClass::fn, editor
//#define DEFINE_FIELD_PARENTFN(field, flags, keyname, edname, fn, editor) , {(fieldtype)&DataClass::field, PDF_PARENT, flags, keyname, edname, fn, editor
//#define DEFINE_FIELD_FLAGSFN(field, flags, keyname, edname, fn, editor)  , {(fieldtype)&DataClass::field, PDF_FLAGS,  flags, keyname, edname, fn, editor

#define DEFINE_INPUT(method, keyname, edname, argtype) , {(input_func)&DataClass::method, argtype, PDFF_NOEDIT | PDFF_INPUT, keyname, edname, EDITOR_NONE
#define DEFINE_OUTPUT(field, keyname, edname) , {(fieldtype)&DataClass::field, PDF_OUTPUT, PDFF_NOEDIT | PDFF_OUTPUT, keyname, edname, EDITOR_NONE
#define DEFINE_MESSAGE(method, keyname, edname, argtype) , {(input_func)&DataClass::method, argtype, PDFF_NOEDIT | PDFF_MESSAGE, keyname, edname, EDITOR_NONE

#define DEFINE_FLAG(value, edname) , {(fieldtype)NULL, PDF_FLAG, value, NULL, edname, {PDE_FLAGS, NULL}}

#endif
