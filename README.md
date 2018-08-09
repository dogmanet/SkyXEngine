SkyXEngine - движок для создания 3D игр с real-time рендером, использует технологии DirectX 9.
Автодокументация по использованию: docs/html/index.html 
Онлайн версия автодокументации: https://doc.skyxengine.com/
Официальный сайт: https://skyxengine.com/
Wiki ресурс с обучающими материалами: https://wiki.skyxengine.com/

# ИДЕОЛОГИЯ
Основной технической идеей при разработке движка была идея о том, что программист должен иметь контроль над объектами, которыми он оперирует, НО этот контроль должен быть в меру.
Основной идеей предназначения служила и служит идея о том, чтобы предоставляемый инструментарий мог полностью удовлетворять потребностям разработчика, без необходимости со стороны разработчика вникать в детали реализации, но чтобы этот инструментарий имел прозрачную, открытую, свободную, бесплатную лицензию, и разработчик мог спокойно податься в стихию сотворения своего мира.

# ЦЕЛЬ
Самая главная цель, которую мы преследуем это разработка движка, на котором в дальнейшем можно разрабатывать игры, ибо в первую очередь разработка движка ведется в своем инетресе.
Дополнительная и немаловажная цель это оценка нашего труда со стороны таких же как и мы - разработчиков (без разницы, игр или другого ПО), которым  нужен подобный движок.

# ОСОБЕННОСТИ
Если углубится в суть разработки, то можно выявить несколько концептуальных элементов, отличающих наш движок от других:
 - open-source
 - стремление сделать из движка инструмент для модмейкеров с помощью которого, можно будет только разрабатывать игры, без вмешательства в код игры
 - монолитная архитектура
 - только те технологии, которые действительно нужны

# ТЕХНОЛОГИИ
Мы используем достаточный набор современных технологий для возможности реализация красивой и современной 3d игры жанра FPS. реализованы:
 - статическая геометрия
 - растительность
 - скелетная анимация
 - освещение (deferred shading)
 - HDR
 - система материалов PBR
 - постпроцесс
 - декали
 - физика
 - звук
 - частицы
 - искусственный интеллект
 - эффекты окружения
 - игровой движок
 - редакторы

# СТРУКТУРА ПРОЕКТА
- build - директория для сборки движка (сюда приходят после компиляции exe, dll), здесь же игровые ресурсы
- docs - документация, настройки для Doxygen
	- html - документация в виде html файлов, открывать index.html
	- settings_doxygen - файл настроек для Doxygen
- libs - директория с файлами lib (сюда приходят все сгенерированные lib)
- proj - директория с проектами для сред разработки
- sdks - сторонние библиотеки, которые используются
- source - директория с файлами исходного кода

----------------------------------------------------------------------------

**!!!Текст ниже, переведен при помощи средства автоматического перевода и может содержать ошибки!!!**

SkyXEngine - engine for creating 3D games with real-time rendering, uses DirectX 9 technologies.
Autodocumentation for use: docs / html / index.html
Online version of auto-documentation: https://doc.skyxengine.com/
Official website: https://skyxengine.com/
Wiki resource with training materials: https://wiki.skyxengine.com/

# IDEOLOGY
The main technical idea in the development of the engine was the idea that the programmer must have control over the objects he operates with, but this control should be in moderation.
The main idea of ​​the purpose was served by the idea that the provided toolkit could fully satisfy the developer's needs, without the need for the developer to delve into the implementation details, but that this toolkit had a transparent, open, free, free license, and the developer could calmly move into the elements the creation of his world.

# OBJECTIVE
The most important goal that we pursue is the development of the engine, on which it is possible to develop games in the future, for the first time the engine is developed in its Internet.
Additional and important goal is to evaluate our work on the part of the same as we - the developers (no difference, games or other software) that need a similar engine.

# FEATURES
If you delve into the essence of development, you can identify several conceptual elements that differentiate our engine from others:
 - open-source
 - the desire to make the engine tool for modmeykerov with which you can only develop games, without interfering with the code of the game
 - monolithic architecture
 - only those technologies that are really needed
 
# TECHNOLOGIES
We use a sufficient set of modern technologies for the possibility of realizing a beautiful and modern 3d game of the FPS genre. implemented:
 - static geometry
 - vegetation
 - skeletal animation
 - lighting (deferred shading)
 - HDR
 - Material system PBR
 - postprocess
 - decals
 - Physics
 - sound
 - particles
 - artificial intelligence
 - environment effects
 - game engine
 - Editors

# PROJECT STRUCTURE
- build - directory for building the engine (here comes after compiling exe, dll), here are the game resources
- docs - documentation, settings for Doxygen
	- html - documentation in the form of html files, open index.html
	- settings_doxygen - settings file for Doxygen
- libs - directory with lib files (here all the generated libs come)
- proj - directory with projects for development environments
- sdks are third-party libraries that are used
- source - directory with source files
