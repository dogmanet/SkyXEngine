#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <common/sxmath.h>

/*!
	Генератор случайных чисел
*/
class CRandom
{
public:
	CRandom(int iSeed=0)
	{
	}
	float getRandomFloat(float fMinVal = 0.0f, float fMaxVal = 1.0f)
	{
		return(randf(fMinVal, fMaxVal));
	}
};

#endif
