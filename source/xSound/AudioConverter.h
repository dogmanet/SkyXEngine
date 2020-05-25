/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __AUDIOCONVERTER_H
#define __AUDIOCONVERTER_H

#include "SoundTypes.h"

#include <mital_fn.h>

//##########################################################################

inline bool AudioResampling(void *pIn, AudioRawDesc *pInDesc, IXBuffer *pOut, AudioRawDesc *pToDesc)
{
	if (pToDesc->uSampleRate == pInDesc->uSampleRate)
		return false;

	BYTE *pSrcData = 0;

	if (pOut->size() > 0)
		pSrcData = pOut->get();
	else
		pSrcData = (BYTE*)pIn;

	int iCountElem = pInDesc->uSize / AudioGetFormatBytes(pInDesc->fmtSample);
	double fCoef = double(pToDesc->uSampleRate) / double(pInDesc->uSampleRate);
	int iNewCountElem = (int)ceil(double(iCountElem) * fCoef);

	uint32_t uSize = AudioGetFormatBytes(pInDesc->fmtSample) * iNewCountElem;
	BYTE *pDestData = new BYTE[uSize];

	//считаем коэффициент добавления на каждом семпле
	double fAdd = double(iCountElem) / double(iNewCountElem);

	//проход по каналам
	for (auto iChannels = 0u; iChannels < pInDesc->u8Channels; ++iChannels)
	{
		int iCurrNewpos = 0;
		int iLast = 0;
		uint32_t iKey;

		//проход по количеству семплов в каждом канале
		for (auto i = 0, il = iNewCountElem / pInDesc->u8Channels; i < il; ++i)
		{
			//текущая позиция до которой надо дойти в исходном массиве
			int iCurr = (int)ceil(fAdd*((i + 1) * pInDesc->u8Channels));

			//сколько элементов из исходного массива надо смешать
			int iCount = (int)max(iCurr - iLast, (int)max(fAdd, 1.0));

			double fCountMixed = 0;

			//берем данные из исходного массива и смешиваем
			for (auto k = 0; k < iCount; ++k)
			{
				iKey = iChannels + ((iLast + k) / pInDesc->u8Channels) * pInDesc->u8Channels;
				iKey = clamp<int>(iKey, 0, iCountElem - 1);

				double fOrigin = RawDataRead(pSrcData, iKey, pInDesc->fmtSample);
				fCountMixed += fOrigin;
			}

			//записываем микс в целевой массив
			iCurrNewpos = iChannels + i * pInDesc->u8Channels;
			RawDataWrite(pDestData, iCurrNewpos, pInDesc->fmtSample, (fCountMixed / double(iCount)));

			//запоминаем до куда дошли в этой итреации
			iLast = iCurr;
		}

		int iSub = iCountElem - iLast;
		int iSub3 = (int)(iNewCountElem * fAdd);
		int qwerty = 0;
	}

	int qwerty = 0;

	if (pOut->size() > 0)
		pOut->free();

	pOut->alloc(uSize);
	memcpy(pOut->get(), pDestData, uSize);

	mem_delete_a(pDestData);

	return true;
}

//**************************************************************************

inline bool AudioRechannels(void *pIn, AudioRawDesc *pInDesc, IXBuffer *pOut, AudioRawDesc *pToDesc)
{
	return false;
	/*BYTE *pSrcData = 0;

	if (pOut->size() > 0)
	pSrcData = pOut->get();
	else
	pSrcData = (BYTE*)pIn;

	uint32_t uSize = 0;

	if (pInDesc->u8Channels )*/
}

//**************************************************************************

inline bool AudioReformat(void *pIn, AudioRawDesc *pInDesc, IXBuffer *pOut, AudioRawDesc *pToDesc)
{
	if (pInDesc->fmtSample == pToDesc->fmtSample)
		return false;

	BYTE *pSrcData = 0;

	if (pOut->size() > 0)
		pSrcData = pOut->get();
	else
		pSrcData = (BYTE*)pIn;

	uint32_t uCountElem = (pInDesc->uSize / AudioGetFormatBytes(pInDesc->fmtSample));
	uint32_t uSize = AudioGetFormatBytes(pToDesc->fmtSample) * uCountElem;
	BYTE *pDestData = new BYTE[uSize];

	for (auto i = 0u; i < uCountElem; ++i)
	{
		double fSample = RawDataRead(pSrcData, i, pInDesc->fmtSample);
		RawDataWrite(pDestData, i, pToDesc->fmtSample, fSample);
	}

	if (pOut->size() > 0)
		pOut->free();

	pOut->alloc(uSize);
	memcpy(pOut->get(), pDestData, uSize);

	mem_delete_a(pDestData);

	return true;
}

//**************************************************************************

inline bool AudioConverter(void *pIn, AudioRawDesc *pInDesc, IXBuffer *pOut, AudioRawDesc *pToDesc)
{
	if (pInDesc->uSampleRate != pToDesc->uSampleRate)
		AudioResampling(pIn, pInDesc, pOut, pToDesc);

	if (pInDesc->u8Channels != pToDesc->u8Channels && pToDesc->u8Channels <= 2 && pToDesc->u8Channels >= 1)
		AudioRechannels(pIn, pInDesc, pOut, pToDesc);

	if (pInDesc->fmtSample != pToDesc->fmtSample)
		AudioReformat(pIn, pInDesc, pOut, pToDesc);

	return true;
}

#endif