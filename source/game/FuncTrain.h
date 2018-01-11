/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Поезда класс
*/
#ifndef _CFuncTrain_H_
#define _CFuncTrain_H_

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
	CFuncTrain(CEntityManager * pMgr);

	//! Остановить
	void stop();
	//! Запустить
	void start();

protected:
	void onPostLoad();
	void moveFunc(float dt);

	//! Начальная точка движения
	CPathCorner * m_pStartStop;
	//! Текущая точка
	CPathCorner * m_pCurStop;

	//! Скорость
	float m_fSpeed;
	//! Текущая дистанция от начала пути
	float m_fCurDist;

	//! Запущен ли
	bool m_bRunning;

	//! ID таймера
	ID m_iPostIval;
};

#endif
