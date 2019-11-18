#ifndef __IXMATERIALSYSTEM_H
#define __IXMATERIALSYSTEM_H

#include <gdefines.h>
#include <common/math.h>

#include "IXMaterial.h"

// {7EC7E7F7-E0BE-4CB7-ABFE-9FC0C10880A5}
#define IXMATERIALSYSTEM_GUID DEFINE_XGUID(0x7ec7e7f7, 0xe0be, 0x4cb7, 0xab, 0xfe, 0x9f, 0xc0, 0xc1, 0x8, 0x80, 0xa5)
#define IXMATERIALSYSTEM_VERSION 1

/*! Набор стандартных шейдеров для материала
    Если нужно задать макроопределения - используйте варианты IXShaderVariant
*/
struct XSHADER_DEFAULT_DESC
{
	//! Имя файла шейдера, с расширением
	const char *szFileVS = NULL;

	//! Имя файла шейдера, с расширением
	const char *szFilePS = NULL;
};

/*! Вариант шейдера для материала. 
    Каждый материал может иметь несколько вариантов одного и того же шейдера, 
	собранных с разными наборами макроопределений

    Зачем это нужно? Простой пример: Система может рисовать одну и ту же геометрию пообъектно, и инстансингом. 
    Вершинные шейдеры в этом случае будут различаться, и система должна иметь возможность выбрать, какой вариант
	шейдера использовать при конкретной установке материала
*/
class IXShaderVariant: public IXUnknown
{
public:
};

struct XSHADER_VARIANT_DESC
{
	//! Дескриптор варианта шейдера, используется для передачи в IXMaterialSystem::bindMaterial()
	IXShaderVariant *pShaderVariant;

	/*! Массив макроопределений для вершинного шейдера, может быть NULL.
	    Последний элемент массива должен быть {NULL, NULL}
	*/
	GXMacro *pMacrosVS;
};

struct XVertexOutputElement
{
	const char *szName;

	//! Тип
	GXDECLTYPE type;

	//! Семантика
	GXDECLUSAGE usage;
};
#define XVERTEX_OUTPUT_DECL_END() {NULL,GXDECLTYPE_UNUSED,(GXDECLUSAGE)0}

struct XVertexFormatHandler {};
struct XVertexShaderHandler {};
struct XGeometryShaderHandler {};

class IXMaterialSystem: public IXUnknown
{
public:
	/*! Загружает материал, если уже загружен - возвращает имеющийся
	    pDefaultShaders - дефолтные шейдеры, на случай, если материала не существует (есть только текстура)
	    uVariantCount - количество вариантов шейдеров
	    pVariantsDesc - массив структур в количестве uVariantCount, описывающий каждый вариант шейдера

	    Каждый материал может иметь несколько вариантов одного и того же шейдера,
	    собранных с разными наборами макроопределений
	*/
	virtual void XMETHODCALLTYPE loadMaterial(const char *szName, IXMaterial **ppMaterial, XSHADER_DEFAULT_DESC *pDefaultShaders, UINT uVariantCount = 0, XSHADER_VARIANT_DESC *pVariantsDesc = NULL) = 0;

	//! Запрашивает материал, если не существует - вернет false
	virtual bool XMETHODCALLTYPE getMaterial(const char *szName, IXMaterial **ppMaterial) = 0;

	//! Загружает текстуру, если уже загружен - возвращает имеющийся
	virtual bool XMETHODCALLTYPE loadTexture(const char *szName, IXTexture **ppTexture) = 0;

	//! Запрашивает текстуру, если не существует - вернет false
	virtual bool XMETHODCALLTYPE getTexture(const char *szName, IXTexture **ppTexture) = 0;

	//! Добавить текстуру
	//virtual void XMETHODCALLTYPE addTexture(const char *szName, IGXTexture2D *pTexture) = 0;

	//! Установить материал для отрисовки
	virtual void XMETHODCALLTYPE bindMaterial(IXMaterial *pMaterial, IXShaderVariant *pShaderVariant = NULL) = 0;

	//! Установить мировую матрицу для отрисовки
	virtual void XMETHODCALLTYPE setWorld(const SMMATRIX &mWorld) = 0;

	//! Установить текстуру в слот
	virtual void XMETHODCALLTYPE bindTexture(IXTexture *pTexture, UINT slot = 0) = 0;

	//! Переопределить пиксельный шейдер для последующих вызовов bindMaterial. Действует до тех пор, пока не будет отменено (-1)
	virtual void XMETHODCALLTYPE overridePixelShader(ID id) = 0;

	//! Переопределить геометрический шейдер для последующих вызовов bindMaterial. Действует до тех пор, пока не будет отменено (-1)
	virtual void XMETHODCALLTYPE overrideGeometryShader(ID id) = 0;


	virtual XVertexFormatHandler* XMETHODCALLTYPE registerVertexFormat(const char *szName, XVertexOutputElement *pDecl) = 0;
	virtual void XMETHODCALLTYPE unregisterVertexFormat(const char *szName) = 0;
	virtual XVertexFormatHandler* XMETHODCALLTYPE getVertexFormat(const char *szName) = 0;

	virtual XVertexShaderHandler* XMETHODCALLTYPE registerVertexShader(XVertexFormatHandler *pVertexFormat, const char *szShaderFile, GXMacro *pDefines = NULL) = 0;
	virtual void XMETHODCALLTYPE bindVS(XVertexShaderHandler *pVertexShader) = 0;

	virtual XGeometryShaderHandler* XMETHODCALLTYPE registerGeometryShader(const char *szShaderFile, const char **aszRequiredParameters, GXMacro *pDefines = NULL) = 0;
	virtual void XMETHODCALLTYPE bindGS(XGeometryShaderHandler *pGeometryShader) = 0;
};

#endif
