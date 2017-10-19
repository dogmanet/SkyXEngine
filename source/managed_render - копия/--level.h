
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл level - пространства имен для управления уровнем
*/

/*! \defgroup managed_render_level level - пространство имен для управления уровнем
 \ingroup managed_render
@{*/

#ifndef __LEVEL_H
#define __LEVEL_H

#include <gdefines.h>
#include <common/string.h>
#include <geom/sxgeom.h>
#include <mtllight/sxmtllight.h>
#include <aigrid/sxaigrid.h>
#include <physics/sxphysics.h>
#include <game/sxgame.h>
#include <particles/sxparticles.h>

//! пространство имен для управления уровнем
namespace Level
{
	/*! \name Загрузка/сохранение
	 \note Загрузочный файл уровня представлен простым конфигурационным файлом (ini) который можно обрабатывать посредством ISXLConfig, 
	однако составные части уровня могут быть представлены бинарными файлами.
	 \note Каждый уровень имеет свою собственную директорию в общей директории уровней, директория уровня именуется также как и название уровня. \n
	Загрузочный файл уровня содержит имена файлов которые необходимо подгрузить для уровня из той же директории уровня.
	Каждый дополнительно загружаемый файл сторого ассоциирован с какой-либо сущностью или элементом уровня.  \n
	К примеру строка в заголовочном файле уровня: geometry = test_level.geom \n
	загрузит геометрию уровня из файла с именем и расширением test_level.geom из той же директории уровня в котором находится заголовочный файл уровня
	 \todo Расписать организацию конфигурационного файла загрузки/сохранения для уровня
	 @{*/
	extern char Name[OBJECT_NAME_MAX_LEN];	//!< текущее имя уровня
	extern String StrAmbientSounds;
	extern String StrWeather;
	void Clear();					//!< очистка уровня
	void Load(const char* name);	//!< Загрузка уровня с именем name
	void Save(const char* name);	//!< Сохранение уровня под именем name

	void LoadParticles();			//!< загрузка файла с инфой о частицах
	void SaveParticles();			//!< сохранение файла с инфой о частицах

	//!@}
};

#endif

//!@} managed_render_level
