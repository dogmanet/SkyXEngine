
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл данных моделей для редактора материалов
*/

#ifndef __ME_MODELS_H
#define __ME_MODELS_H

#include <gcore/sxgcore.h>

//! создает и возвращает статическую модель сферы
ISXDataStaticModel* CreateModelSphere();

//! создает и возвращает статическую модель куба
ISXDataStaticModel* CreateModelCube();

//! создает и возвращает статическую модель наклоенной плоскости
ISXDataStaticModel* CreateModelPlane();

#endif