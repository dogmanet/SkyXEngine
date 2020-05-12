/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __IXAUDIOCODEC_H
#define __IXAUDIOCODEC_H

#include <gdefines.h>
#include <xcommon/IXBuffer.h>
#include <xcommon/IXSoundSystem.h>
#include <common/AudioRawDesc.h>

//##########################################################################

// {7D338709-3BC1-4008-8AA0-74305E337D7F}
#define IXAUDIOCODEC_GUID DEFINE_XGUID(0x7d338709, 0x3bc1, 0x4008, 0x8a, 0xa0, 0x74, 0x30, 0x5e, 0x33, 0x7d, 0x7f)

#define IXAUDIOCODEC_VERSION 1

//##########################################################################

class IXAudioCodecTarget;

/*!
 * интерфейс аудио кодека выдающего целевой обьект для работы с файлом
*/
class IXAudioCodec: public IXUnknown
{
public:

	/*! возвращает формат кодека
		@note например ogg-vorbis, wav-pcm
	*/
	virtual const char* XMETHODCALLTYPE getFormat() const = 0;

	//************************************************************************

	/*! возвращает расширение файлов кодека
		@note кодек может принимать несколько расширений, но это не значит что идентификация формата идет на основании расширения
		@param uIndex порядковый номер
		@return строку если uIndex корректный (находится в пределах массива расширений), NULL в ином случае
	*/
	virtual const char* XMETHODCALLTYPE getExt(UINT uIndex=0) const = 0;

	//************************************************************************

	//! возвращает количество поддерживаемых расширений файлов
	virtual UINT XMETHODCALLTYPE getExtCount() const = 0;

	/*! возвращает может ли кодек декодировать (прочитать) файл pFile
		@param szFile путь до файла
		@param szArg аргументы в строке, игнорируется
		@param ppTarget указатель на неиницилизированный целевой обьект кодека
		@param forSave открыть ли файл для записи
		@return true если может открыть (для чтения или записи) и декодировать (для чтения), иначе false
	*/
	virtual bool XMETHODCALLTYPE open(const char *szFile, const char *szArg, IXAudioCodecTarget **ppTarget, bool forSave = false) = 0;

	//************************************************************************

	//! возвращает может ли кодек кодировать (записывать)
	virtual bool XMETHODCALLTYPE canSave(UINT uIndex=0) const = 0;
};

//##########################################################################

/*!
	интерфейс целевого кодека работающего с файлом
*/
class IXAudioCodecTarget: public IXUnknown
{
public:

	virtual void getDesc(AudioRawDesc *pDesc) const = 0;

	//************************************************************************

	/*! возвращает текущую позицию (в байтах) в PCM данных
		@return >= 0 в случае успеха, -1 в случае ошибки
	*/
	virtual int64_t XMETHODCALLTYPE getPos() const = 0;

	//************************************************************************

	/*! установка позиции в PCM данных
		@param uPos позиция в байтах
	*/
	virtual void XMETHODCALLTYPE setPos(int64_t uPos) = 0;

	//************************************************************************

	/*! декодирование (считывание) PCM данные из pFile в ppData начиная с iPos длиной в uLen
		@param iPos позиция в PCM данных, в байтах
		@param uLen сколько PCM данных надо считать, в байтах (является ограничением, а не требованием)
		@param pFile указатель на FILE (чтение)
		@param ppData указатель на выделенный массив байтов размером не менее uLen
		@return размер считанных PCM данных в байтах
	*/
	virtual size_t XMETHODCALLTYPE decode(int64_t iPos, uint64_t uLen, void **ppData) = 0;

	//************************************************************************

	/*! кодирование PCM данных pBufferPCM с требуемым описанием в формат кодека
		@param pFile указатель на FILE (запись)
		@param pBufferPCM буфер с PCM данными
		@param pOutDesc описание PCM данных для вывода
		@return true в случае успеха, иначе false
	*/
	virtual bool XMETHODCALLTYPE encode(IXBuffer *pBufferPCM, AudioRawDesc *pOutDesc) = 0;
};

#endif
