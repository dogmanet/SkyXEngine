
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Поезда класс
*/
#ifndef __FUNC_TRAIN_H
#define __FUNC_TRAIN_H

#include "PointEntity.h"

class CPathCorner;

/*! Поезда класс
\ingroup cpointentity
*/
class CFuncTrain: public CPointEntity
{
	DECLARE_CLASS(CFuncTrain, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

	//! Остановить
	void stop();
	//! Запустить
	void start();

protected:
	void onPostLoad();
	void moveFunc(float dt);

	//! Начальная точка движения
	CPathCorner *m_pStartStop = NULL;
	//! Текущая точка
	CPathCorner *m_pCurStop = NULL;

	//! Скорость
	float m_fSpeed = 0.0f;
	//! Текущая дистанция от начала пути
	float m_fCurDist = 0.0f;

	//! Запущен ли
	bool m_bRunning = false;

	//! ID таймера
	ID m_iPostIval = -1;
};

#endif
