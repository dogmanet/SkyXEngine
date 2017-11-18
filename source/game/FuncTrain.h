/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Поезда класс
*/
#ifndef _FuncTrain_H_
#define _FuncTrain_H_

#include "SXpointEntity.h"

class PathCorner;

/*! Поезда класс
\ingroup cpointentity
*/
class FuncTrain: public SXpointEntity
{
	DECLARE_CLASS(FuncTrain, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	FuncTrain(EntityManager * pMgr);

	//! Остановить
	void Stop();
	//! Запустить
	void Start();

protected:
	void OnPostLoad();
	void MoveFunc(float dt);

	//! Начальная точка движения
	PathCorner * m_pStartStop;
	//! Текущая точка
	PathCorner * m_pCurStop;

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
