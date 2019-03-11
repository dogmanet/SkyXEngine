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
	IXShaderVariant **ppShaderVariant;

	/*! Массив макроопределений для вершинного шейдера, может быть NULL.
	    Последний элемент массива должен быть {NULL, NULL}
	*/
	GXMACRO *pMacrosVS;

	/*! Массив макроопределений для пиксельного шейдера, может быть NULL.
	Последний элемент массива должен быть {NULL, NULL}
	*/
	GXMACRO *pMacrosPS;
};

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
	virtual void loadMaterial(const char *szName, IXMaterial **ppMaterial, XSHADER_DEFAULT_DESC *pDefaultShaders, UINT uVariantCount = 0, XSHADER_VARIANT_DESC *pVariantsDesc = NULL) = 0;

	//! Запрашивает материал, если не существует - вернет false
	virtual bool getMaterial(const char *szName, IXMaterial **ppMaterial) = 0;

	//! Запрашивает текстуру, если не существует - вернет false
	virtual bool getTexture(const char *szName, IXTexture **ppTexture) = 0;

	//! Добавить текстуру
	virtual void addTexture(const char *szName, IGXTexture2D *pTexture) = 0;

	//! Установить материал для отрисовки
	virtual void bindMaterial(IXMaterial *pMaterial, IXShaderVariant *pShaderVariant = NULL) = 0;

	//! Установить мировую матрицу для отрисовки
	virtual void setWorld(const SMMATRIX &mWorld) = 0;

	//! Установить текстуру в слот
	virtual void bindTexture(IXTexture *pTexture, UINT slot = 0) = 0;
};

#endif
