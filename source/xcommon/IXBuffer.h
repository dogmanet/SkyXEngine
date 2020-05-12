/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __IXBUFFER_H
#define __IXBUFFER_H

#include <gdefines.h>

//##########################################################################

//! интерфейс буфера с выделяемой памятью
class IXBuffer : public IXUnknown
{
public:

	/*! выделение памяти размером size
		@note если память уже была выделена произойдет ее освобождение и новое выделение
	*/
	virtual void XMETHODCALLTYPE alloc(size_t size) = 0;

	//! очистка ранее выделенной памяти (если была)
	virtual void XMETHODCALLTYPE free() = 0;

	//! возвращает размер выделенной памяти в байтах
	virtual size_t XMETHODCALLTYPE size() = 0;

	//! возвращает указатель на выделенную память
	virtual BYTE* XMETHODCALLTYPE get() = 0;
};

#endif
