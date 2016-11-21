
#ifndef SX_POST_PROCESS_H
#define SX_POST_PROCESS_H

		/*#ifndef EXPORT_DATA
				#ifdef SX_EXE
					#define EXPORT_DATA __declspec (dllimport)
				#else
					#define EXPORT_DATA __declspec (dllexport)
				#endif
		#endif*/
#define EXPORT_DATA

//добавить возможность альфаблединга
//еще раз посмотреть загрузку шойдеров
//добавить версии для шойдеров

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//доступные форматы для рендер тарджетов
#define SXPP_RT_FORMAT_RGBA8	"RGBA8"		// D3DFMT_A8R8G8B8
#define SXPP_RT_FORMAT_R5G6B5	"R5G6B5"	// D3DFMT_R5G6B5
#define SXPP_RT_FORMAT_R32F		"R32F"		// D3DFMT_R32F
#define SXPP_RT_FORMAT_GR32F	"GR32F"		// D3DFMT_G32R32F
#define SXPP_RT_FORMAT_ABGR32F	"ABGR32F"	// D3DFMT_A32B32G32R32F
#define SXPP_RT_FORMAT_R16F		"R16F"		// D3DFMT_R16F
#define SXPP_RT_FORMAT_GR16F	"GR16F"		// D3DFMT_G16R16F
#define SXPP_RT_FORMAT_ABGR16F	"ABGR16F"	// D3DFMT_A16B16G16R16F

//типы текстур (строковое значение)
#define SXPP_TEX_TYPE_LOAD_STR		"load"
#define SXPP_TEX_TYPE_ENGINE_STR	"engine"
#define SXPP_TEX_TYPE_PP_STR		"pp"

//типы текстур (числовое значение)
#define SXPP_TEX_TYPE_LOAD_NUM		0
#define SXPP_TEX_TYPE_ENGINE_NUM	1
#define SXPP_TEX_TYPE_PP_NUM		2

//обозначение типа текстуры которая является финальным рендер тарджетом
#define SXPP_TEX_ID_SCREEN		"screen"

//доступные типы данных, для инициализации в шейдер
#define SXPP_SHADER_DATA_TYPE_FLOAT		"float"
#define SXPP_SHADER_DATA_TYPE_FLOAT2	"float2"
#define SXPP_SHADER_DATA_TYPE_FLOAT3	"float3"
#define SXPP_SHADER_DATA_TYPE_FLOAT4	"float4"
#define SXPP_SHADER_DATA_TYPE_INT		"int"
#define SXPP_SHADER_DATA_TYPE_MATRIX	"float4x4"
#define SXPP_SHADER_DATA_TYPE_MACRO		"macro"

//тип фильтрации текстур
#define SXPP_PASS_TEX_FILTER_NONE		"none"
#define SXPP_PASS_TEX_FILTER_LINEAR		"linear"
#define SXPP_PASS_TEX_FILTER_POINT		"point"
#define SXPP_PASS_TEX_FILTER_ANISOTROPY "anisotropy"

//тип адресации текстур
#define SXPP_PASS_TEX_ADRESS_WRAP		"wrap"
#define SXPP_PASS_TEX_ADRESS_CLAMP		"clamp"
#define SXPP_PASS_TEX_ADRESS_MIRROR		"mirror"

#define SXPP_ERROR_G_MESSAGE "SkyX Engine - PostProcess library"
#define SXPP_ERROR_G "Error"
#define SXPP_ERROR_MESSAGE_NO_VALUE "no value"

//типы шейдеров
#define SXPP_SHADER_VERTEX 0
#define SXPP_SHADER_PIXEL 1

//коды ошибок
enum SXPP_ERROR_CODE
{
	SXPP_OK = 0,							//все хорошо
	SXPP_ERR_NOT_FILE = -1,					//не найден файл
	SXPP_ERR_INVALID_VALUE = -2,			//невалидное значение
	SXPP_ERR_NEW_BEGIN_PP = -3,				//найдено начало следующего постпроцееса в незакрытом процессе
	SXPP_ERR_NOT_FOUND_NAME = -4,			//не найдено имя
	SXPP_ERR_NOT_FOUND_ITEM = -5,			//не найден элемент пост-процесса
	
	SXPP_ERR_NOT_FOUND_PP = -6,				//не найден пост-процесс
	SXPP_ERR_INVALID_NUMBER_OF_PP = -7,		//невалидный номер постпроцесса

	SXPP_ERR_TEX_RT = -8,					//передаваемая текстура есть текущий рендер тарджет
	SXPP_ERR_NON_UNIC_ITEM = -9,			//не уникальный итем
	
	SXPP_ERR_INVALID_VER_SHADER = -10,		//невалидная версия шейдера
	SXPP_ERR_DISCREPANCY_VER_SHADER = -11,	//несоответсвие версий шейдеров
	SXPP_ERR_MAX_NUM = -12,					//превышен лимит элемента
	SXPP_ERR_INVALID_RT_SAVE = -13,			//неправильно сформирована строка сохранения
	SXPP_ERR_REDIFINITION_GDATA = -14,		//переобъявление global_namespace_data_pp
	SXPP_ERR_GDATA_PASS = -15,				//в глобальном пространстве имен найден пасс
	SXPP_ERR_GDATA_SEQUENCE = -16,			//в глобальном пространстве имен найдена последовательность
	SXPP_ERR_G_NON_UNIC_ITEM = -17,			//не уникальный итем, сравнение по всем постпроцессам

	SXPP_ERR_MAX_NUM_REC = -18,				//превышен лимит рекурсиных вызовов
	SXPP_ERR_NON_END_REC = -19				//бесконечная рекусрия, дочерний элемент в родителях указывает себя родителем
};

//синтаксис
#define SXPP_DET_ITEM_GDATA	"global_namespace_data_pp"
#define SXPP_DET_ITEM_POSTPROCESS	"postprocess"
#define SXPP_DET_ITEM_RENDER_TARGET	"render_target"
	#define SXPP_DET_ITEM_RENDER_TARGET_WIDTH			"width"
	#define SXPP_DET_ITEM_RENDER_TARGET_HEIGHT			"height"
		#define SXPP_DET_ITEM_RENDER_TARGET_SIZE_FULL	"full"
	#define SXPP_DET_ITEM_RENDER_TARGET_FORMAT			"format"
	#define SXPP_DET_ITEM_RENDER_TARGET_BLEND_SCREEN	"blend_screen"

#define SXPP_DET_ITEM_TEXTURES		"textures"
	#define SXPP_DET_ITEM_TEXTURES_NF_TYPE		"texture type"
	#define SXPP_DET_ITEM_TEXTURES_NF_NAME		"texture name"
	#define SXPP_DET_ITEM_TEXTURES_NF_ID		"texture id"

#define SXPP_DET_ITEM_SHADER_VS		"shader_vs"
#define SXPP_DET_ITEM_SHADER_PS		"shader_ps"
	#define SXPP_DET_ITEM_SHADER_PATH		"path"
	#define SXPP_DET_ITEM_SHADER_VERSION	"version"
	#define SXPP_DET_ITEM_SHADER_MACRO		"macro"
	#define SXPP_DET_ITEM_SHADER_NF_DEF		"macro definition"
	#define SXPP_DET_ITEM_SHADER_INIT_DATA	"init_data"
	#define SXPP_DET_ITEM_SHADER_INIT_DATA_NF_TYPE	"type"
	#define SXPP_DET_ITEM_SHADER_INIT_DATA_NF_VALUE	"value"

#define SXPP_DET_ITEM_PASS			"pass"
	#define SXPP_DET_ITEM_PASS_TEXTURE				"texture"
	#define SXPP_DET_ITEM_PASS_TEXTURE_NF_SLOT		"texture slot"
	#define SXPP_DET_ITEM_PASS_TEXTURE_NF_ADRESS	"texture adress"
	#define SXPP_DET_ITEM_PASS_TEXTURE_NF_FILTER	"texture filter"

#define SXPP_DET_ITEM_SEQUENCE		"sequence"

//количество объектов на 1 постэффект
#define SXPP_SETTINGS_SB_RENDER_TARGET	16	//количество рендер тарджетов
#define SXPP_SETTINGS_SB_TEXTURES		16	//количество текстур не считая рендер тарджетов
#define SXPP_SETTINGS_SB_PASS			16	//количество пассов
#define SXPP_SETTINGS_SB_PASS_TEX		8	//количество текстур в пассе
#define SXPP_SETTINGS_SB_SEQUENCE		16	//количество пассов в последовательности
#define SXPP_SETTINGS_SB_SHADER			16	//количество шейдеров
#define SXPP_SETTINGS_SB_SHADER_DATA	32	//количество переменных которые можно инициализровать
#define SXPP_SETTINGS_SB_SHADER_MACRO	32	//количество макросов которое передается в шейдер

#define SXPP_SETTINGS_SB_PP				64	//количество постэффектов

#define SXPP_SETTINGS_SB_MAX_COUNT_REC	64	//количество вложенности при рекурсии (при наследовании текстур)
#define SXPP_SETTINGS_SB_COUNT_TEX_FOR_USING 64	//количество постэффектов

#define SXPP_SETTINGS_SB_ERROR_MSG 1024			//размер буффера для отладочного сообщения
#define SXPP_SETTINGS_SB_SRC_ERR 512			//размер буффера для источника ошибки

//пространство имен c данными для работы постпроцесса
namespace SXPostProcessGD
{
	//структура для хранения и идентификации движковых текстур
	struct EXPORT_DATA EngineTexture
	{
		EngineTexture();

		int CountTextures;	//количество
		//добавление движковой текстуры
		int AddTexture(
						const char* name,		//имя по которой текстура будет достпуна из файла постпроцесса	 
						IDirect3DTexture9* tex);//указатель на текстуру	
		//массивы с данными
		char Name[256][SXPP_SETTINGS_SB_COUNT_TEX_FOR_USING];
		IDirect3DTexture9* Texture[SXPP_SETTINGS_SB_COUNT_TEX_FOR_USING];
	};


	struct EXPORT_DATA SXPPGDData
	{
		SXPPGDData();

		float2_t WidthHeightBackBuffer;

		IDirect3DDevice9* Device;

		char* PathForShaders;		//путь до папки с шейдерами
		char* PathForTextures;		//путь до папки с текстурами
		char* PathForSave;			//путь до папки куда сохраняем промежуточные результаты
		
		//эти данные необходимы для множественного смешивания изображения постэффекта со сценой
		//текстуры должны быть абсолютно идентичны, для нормальной работы (если не эксперемент)
		IDirect3DTexture9*	FinalScene;		//карта финальной сцены
		IDirect3DTexture9*	FinalScene2;	//карта финальной сцены
		
		char KeySave;	//код кнопки, при нажатии на которую будут сохраняться рендер тарджеты

		float2_t SizeBackBuffer;	//размер бэк буффера (в большиснвте случаев окна)
	} EXPORT_DATA Data;

	EXPORT_DATA EngineTexture DataTex;

	EXPORT_DATA bool IsCurrentTex;			//переменная регулирующая смешивание с финальной сценой

	EXPORT_DATA void InitData(SXPPGDData *data);	//инициализация данных

	//получение указателя на текстуры
	EXPORT_DATA IDirect3DTexture9* GetCurrentRenderTexture();		//текущую текстуур в который будет осуществлен рендер
	EXPORT_DATA IDirect3DTexture9** GetCurrentRenderTextureLP();
	EXPORT_DATA IDirect3DTexture9* GetCurrentSendTexture();
	
	//меш - квадрат на который одеваем рендер тарджет (смещения для антиблюра сделаны)
	EXPORT_DATA ID3DXMesh* ScreenTexture;

	EXPORT_DATA void CreateScreenTexture(); //создаем меш-квадрат
};

//структура для формирования отчета об ошибке
struct EXPORT_DATA SXPostProcessError
{
	SXPostProcessError();

	SXPP_ERROR_CODE Code;		//код ошибки
	DWORD GenStrError;			//номер строки на которой генерируется ошибка
	char GenFileError[256];		//файл в котром генерируется ошибка
	DWORD NumStr;				//строка на которой происходит ошибка, для чтения файла
	char Str[256];				//
	char Str2[256];				//
	char Str3[256];				//
	char Str4[256];				//
	char Str5[256];				//
	char Str6[256];				//
};

//структура описывающая постпроцесс
struct EXPORT_DATA SXPostProcessData
{
	SXPostProcessData();
	~SXPostProcessData();

	struct SXPPRenderTarget
	{
		SXPPRenderTarget();
		~SXPPRenderTarget();

		char Name[256];
		int Width,Height;				//ширина, длина
		char WidthStr[32],HeightStr[32];//тоже только строками
		DWORD Format;					//формат
		bool BlendScreen;
		IDirect3DTexture9*	Texture;
	} RenderTarget[SXPP_SETTINGS_SB_RENDER_TARGET];

	struct SXPPShader
	{
		SXPPShader();
		~SXPPShader();

		char Name[256];		//имя шейдера
		char Path[256];		//путь загрузки
		char Version[256];	//версия шейдера
		DWORD Shader;

		//структура для инициализации данных в шейдере
		struct InitData
		{
			char NameVar[256];	//имя переменной
			char Type[256];		//тип
			char ValueStr[256];	//строчное значение
			void* ValueData;	//указатель на значение
			DWORD CountBytes;	//количество байт занимаемое данной переменной
		} Data[SXPP_SETTINGS_SB_SHADER_DATA];

		int NumData;	//количество инициалзируемых переменных

		void SetMacro(const char* name,const char* def);

		void SetEndMacro();

		D3DXMACRO Macro[SXPP_SETTINGS_SB_SHADER_MACRO];
		int CountMacro;
		
	} ShaderVS[SXPP_SETTINGS_SB_SHADER], ShaderPS[SXPP_SETTINGS_SB_SHADER];

	//
	struct SXPPSequence
	{
		char Pass[SXPP_SETTINGS_SB_SEQUENCE][256];
		int PassNum[SXPP_SETTINGS_SB_SEQUENCE];
	} Sequence;

	//структура описывающая текстуру
	struct SXPPTextures
	{
		SXPPTextures();
		~SXPPTextures();

		char Name[256];
		int Type;		//тип текстуры
		char ID[256];	//если Type == engine то имя текстуры в движке, иначе это путь загрузки
		bool IsOtherPP;	//эта текстура из другого эффекта? если используем тип pp
		bool IsTex;		//это текстура аля рендер тарджет
		int HowPP;		//если указан тип pp, из какого эффекта берем текстуру	
		int HowNum;		//порядковый номер
		IDirect3DTexture9*	Texture;
	} Textures[SXPP_SETTINGS_SB_TEXTURES];

	struct SXPPPass
	{
		SXPPPass();

		char Name[256];	//имя прохода
		char RT[256];	//имя render target
		int RTNum;		//порядковый номер рендер тарджета
		bool IsCurrentPPRT;	//из текущего ли процесса берем, аля из глобальных данных
		//структура для установки текстур
		struct TexParam
		{
			char Name[256];	//имя текстуры
			int Slot;		//слот
			int Filter[3];	//фильтр
			int Adress;		//адресация
		} Texture[SXPP_SETTINGS_SB_PASS_TEX];
		int TextureNum[SXPP_SETTINGS_SB_PASS_TEX];		//порядковые номера для текстур
		int TextureTypeNum[SXPP_SETTINGS_SB_PASS_TEX];	//типы текстур, 0 - текстура, 1 - рендер тарджет
		bool IsCurrentPPTex[SXPP_SETTINGS_SB_PASS_TEX];	//из текущего ли процесса берем, аля из глобальных данных

		char ShaderVS[256];	//имя вершинного шейдера
		int ShaderVSNum;	//порядковый номер шейдера
		bool IsCurrentPPVS;	//в текущем ли эффекте искать шейдер, аля в глобальных данных?

		char ShaderPS[256];	//имя пиксельного шейдера
		int ShaderPSNum;	//порядковый номер шейдера
		bool IsCurrentPPPS;	//в текущем ли эффекте искать шейдер, аля в глобальных данных?

		int CountTextures;	//количество текстур
	} Pass[SXPP_SETTINGS_SB_PASS];

	char Name[256];	//имя постпроцесса
	int RTNum;		//количество рендер тарджетов
	int SVSNum;		//количество вершинных шейдеров
	int SPSNum;		//количество пиксельных шейдеров
	int SeqNum;		//количество пассов в последовательности
	int TexNum;		//количество текстур
	int PassNum;	//количество пассов
};

class EXPORT_DATA SXFileText
{
public:
	SXFileText();
	~SXFileText();
	int LoadFile(const char* path);

	bool IsEndFile();
	void GetStr(char* buf,int size);

	void GetText(char* buf,int size); //test
	void DeleteAll(char* str_b,char* str_e,bool size_end_elem,bool paste_new_str);

	void SetPosition(DWORD pos);
	DWORD GetPosition();
	DWORD GetCurrentStr();

private:
	DWORD Position;
	long Size;
	char* FileText;
};

//стрктура используемая для контроля вложений рекурсий
struct SXRecValidate
{
	SXRecValidate();
	void Add(int pp,int num);

	int Validate();

	//в данные массивы записывается то что уже обработали
	int PP[SXPP_SETTINGS_SB_MAX_COUNT_REC];
	int Num[SXPP_SETTINGS_SB_MAX_COUNT_REC];
	int Count;	//количество ключей записанных в массивы
};

//структура для передачи данных в шейдер
struct SXComTransferShaderData
{
	SXComTransferShaderData();
	int CountData;		//количество данных

	int AddData(const char* name,void* data,long size);

	struct SXShaderData
	{
		char NameVar[256];	//имя переменной
		void* Data;			//указатель на переменную
		long CountBytes;	//размер в байтах
	} ShaderData[64];
};

class EXPORT_DATA SXLoaderPPFile
{
public:
	SXLoaderPPFile();
	~SXLoaderPPFile();
	//загрузка постпроцесса
	int Load(const char* path,SXPostProcessError *error);
	
	//отрисовка
	//поддержка рендер тарджетов для сохранения сделана в качестве отладочного средства, 
	//чтобы можно было сохранить результаты проходов на диск и просмотреть
	int Render(								//главная функция отрисовки, только через нее идет отрисовка, остальное обертки
				int how_pp,					//номер постпроцесса
				SXPostProcessError *error,
				int count_rt_save = 0,		//количество ключей в массиве rt_save_arr
				int* rt_save_arr = 0);		//массив с номерами рендер тарджетов которые подлежат сохранению
	int Render(
				const char* name_pp,		//имя постпроцесса
				const char* how_rt_save,	//строка с именами рендер тарджетов (разделенных \n и в конце тоже) которые подлежат сохранению 
				SXPostProcessError *error);

	//объявлен ли макрос в шейдере
	bool IsDefined(
					const char* pp,		//имя постпроцесса
					int shader_type,	//тип шейдера, вершинный аля пиксельный, -1 - искать и в том и другом
					const char* shader,	//имя шейдера
					const char* define);//имя дефиниции	
	//получить дефиницию макроса
	//аргументы аналогичны IsDefined
	const char* GetDefine(const char* pp,int shader_type,const char* shader,const char* define);

	int GetPPNum();	//возвращает количество загруженных постпроцессов

	//инициализация данных постпроцесса
	int InitData(const char* name_pp,SXPostProcessError *error=0);	//по имени
	int InitData(int how_pp,SXPostProcessError *error=0);			//по порядковому номеру
	int InitDataAll(SXPostProcessError *error);						//всех загруженных

	//добалвение/обновление данных передаваемых в шейдер
	//0 - добавленно
	//1 - обновлено
	//-1 - превышен лимит
	//-2 - не найдено чтото

	int AddShaderData(
						const char* pp,				//имя постпроцесса
						int type_shader,			//тип шейдера, вершинный аля пиксельный? можно указать -1 тогда поиск будет осуществлен и среди вершинных и среди пиксельных шейдеров

						//далее все по приоритетам: passnum,pass,shader_name,shader_name_pp
						//чем дальше от начала списка тем меньше приоритет
						//значит если указан passnum то все остальное игнорируется
						//дефолтные занчения: passnum = -1, shader_name_pp,shader_name,pass = 0 аля [0] = 0
						const char* shader_name_pp,	//имя шейдера в файле постпроцесса
						const char* shader_name,	//имя файла шейдера с расширением
						const char* pass,			//имя прохода в котором устанавливаем данные
						int passnum,				//номер прохода (pass) из последовательности (sequence) начиная с 0

						const char* name_var,		//имя переменной
						void* data,					//указатель на значение
						long size);					//количество байт

	//упрощенные варианты AddShaderData, использует все тот же функционал
	//обозначения аргументов читать из AddShaderData
	int AddShaderData(const char* pp,int type_shader,int passnum,const char* name_var,void* data,long size);
	int AddShaderData(const char* pp,int type_shader,const char* pass,const char* name_var,void* data,long size);
	int AddShaderData(const char* pp,int type_shader,const char* shader_name_pp,const char* shader_name,const char* name_var,void* data,long size);


	int GetIndexNumPP(const char* pp);//возвращает порядковый номер постпроцесса по имени

	//возвращаемое значение:
	//-2 - не найдена текстура
	//-1 - не найден постпроцесс либо он невалиден
	//0 - все хорошо
	int GetTexInfo(const char* ppname,int ppnum,const char* name,D3DSURFACE_DESC* desc,int type = -1); //получить размер текстуры/рендер тарджета

	//проверка загруженных данных
	int CheckValidate(const char* name_pp,SXPostProcessError *error=0);	//по имени постпроцесса
	int CheckValidate(int how_pp,SXPostProcessError *error=0);			//по порядковому номеру постпроцесса
	int CheckValidateAll(SXPostProcessError *error=0);					//проверка всех постпроцессов, должна быть вызвана после загрузки всех потспроцессов

	//рекурсивная функция, вложенность рекурсии до SXPP_SETTINGS_SB_MAX_COUNT_REC
	bool IsValidTexturePP(
							int pp,							//порядковый номер постпроцесса
							int numtex,						//порядковый номер текстуры
							SXRecValidate *rec_validate,	//структура которая контролирует вложенность рекурсии
							int *error);					//номер ошибки вложенности рекурсии

	bool IsPPThere(int pp);	//имеется ли постпроцесс с номер pp

	//возвращают помимо самих объектов текстур еще и рендер тарджеты

	//получить текстуру по имени и типу(опционально), используя GetTexturePP если есть необходимость
	//-1 поиск среди текстур и рендер тарджетов, 0 - текстуры (даже если текстура ссылается на рендер тарджет, для менеджера она текстура)
	//1 - поиск среди рендер тарджетов
	//данные функции используются без ограничителей и проверок, дабы не повлиять на производительность при рендере
	//ибо предполагается что юзер после загрузки всех постэффектов вызвал функцию валидации и она успешно завершилась
	IDirect3DTexture9* GetTexture(const char* name,int type=-1);
	IDirect3DTexture9* GetTexture(
									const char* ppname,	//имя постпроцесса, может быть 0 в случае если ppnum отличен от -1
									int ppnum,			//порядковый номер постпроцесса, если указано значение отличное от -1 то ppname игнорируется, иначе поиск идет по ppname
									const char* name,	//имя текстуры/рендер тарджета
									int type=-1);		//тип текстура(0)/рендер тарджет(1), -1 - в обоих случая ищем

	IDirect3DTexture9* GetTexturePP(int pp,int numtex);			//получить текстуру указанную в другом постэффекте, рекурсивный поиск

private:
	int PPNum;									//количество загруженных постпроцессов
	int HowGlobalNSData;						//порядковый номер глобального пространства имен
	char PathForGData[256];						//путь до файла из которого было загружено глобальное пространство
	SXPostProcessData PP[SXPP_SETTINGS_SB_PP];	//массив с информацией о постпроцессах

	//данные для передачи в шейдеры, для каждого шейдера отдельно
	SXComTransferShaderData ComTrShaderDataVS[SXPP_SETTINGS_SB_PP][SXPP_SETTINGS_SB_SEQUENCE];
	SXComTransferShaderData ComTrShaderDataPS[SXPP_SETTINGS_SB_PP][SXPP_SETTINGS_SB_SEQUENCE];
};

class SXWrapLoaderPPFile
{
public:
	SXWrapLoaderPPFile();
	~SXWrapLoaderPPFile();
	void Load(const char* path);

	void Render(int how_pp,int count_rt_save = 0,int* rt_save_arr = 0);
	void Render(const char* name_pp,const char* how_rt_save);
	void Render(const char* name_pp);

	void RenderComError(SXPostProcessError *error);

	bool IsDefined(const char* pp,int shader_type,const char* shader,const char* define);		//объявлен ли макрос в шейдере
	const char* GetDefine(const char* pp,int shader_type,const char* shader,const char* define);//получить дефиницию макроса

	int GetPPNum();	//возвращает количество загруженных постпроцессов

	//упрощенные варианты AddShaderData, использует все тот же функционал
	//обозначения аргументов читать из AddShaderData
	void AddShaderData(const char* pp,int type_shader,int passnum,const char* name_var,void* data,long size);
	void AddShaderData(const char* pp,int type_shader,const char* pass,const char* name_var,void* data,long size);
	void AddShaderData(const char* pp,int type_shader,const char* shader_name_pp,const char* shader_name,const char* name_var,void* data,long size);

	int GetIndexNumPP(const char* pp);//возвращает порядковый номер постпроцесса по имени

	//инициализация данных
	void InitData(const char* name_pp);
	void InitData(int how_pp);
	void InitDataAll();

	void InitDataComError(SXPostProcessError *error);

	//проверка загруженных данных
	void CheckValidate(const char* name_pp);
	void CheckValidate(int how_pp);
	void CheckValidateAll();

	void CheckValidateComError(SXPostProcessError *error);

	bool IsPPThere(int pp);	//имеется ли постпроцесс с номер pp

	IDirect3DTexture9* GetTexture(const char* name,int type=-1);
	IDirect3DTexture9* GetTexture(const char* ppname,int ppnum,const char* name,int type=-1);

	void GetTexInfo(const char* ppname,int ppnum,const char* name,D3DSURFACE_DESC* desc,int type = -1); //получить размер текстуры/рендер тарджета

private:
	SXLoaderPPFile* PP;
};

//-------------------------------------------

SXWrapLoaderPPFile::SXWrapLoaderPPFile()
{
	PP = new SXLoaderPPFile();
}

SXWrapLoaderPPFile::~SXWrapLoaderPPFile()
{
	SX_SAFE_DELETE(this->PP);
}

int SXWrapLoaderPPFile::GetPPNum()
{
	return PP->GetPPNum();
}

void SXWrapLoaderPPFile::Load(const char *path)
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->Load(path,&error);

		if(error.Code != SXPP_OK)
		{
			char src_gen[SXPP_SETTINGS_SB_SRC_ERR];
			sprintf(src_gen,"%s%s%s%d","Источник генерации ошибки: [",error.GenFileError,"] : ",error.GenStrError);
			char mess[SXPP_SETTINGS_SB_ERROR_MSG];
				if(error.Code == SXPP_ERR_NOT_FILE)
					sprintf(mess,"%s%s%s%s","Файл пост-процесса [",error.Str,"] не удалось открыть\n\n",src_gen);
				//доделать
				else if(error.Code == SXPP_ERR_NEW_BEGIN_PP)
				{
						//if(result == error.Code)	//error
							sprintf(mess,"%s%s%s%d%s%s%s%s","[",path,"] : ",error.NumStr," - в пост-процессе [",error.Str,"] найдено начало другого пост-процесса\n\n",src_gen);
						//else	//warning
				}
						else if(error.Code == SXPP_ERR_NOT_FOUND_NAME)
					sprintf(mess,"%s%s%s%d%s%s%s%s","[",path,"] : ",error.NumStr," - не найдено обозначение [",error.Str,"]\n\n",src_gen);
				else if(error.Code == SXPP_ERR_INVALID_VALUE)
					sprintf(mess,"%s%s%s%d%s%s%s%s%s%s%s%s","[",path,"] : ",error.NumStr," - невалидное значение [",error.Str,"] : [",error.Str2,"] : [",error.Str3,"] \n\n",src_gen);
				else if(error.Code == SXPP_ERR_MAX_NUM)
					sprintf(mess,"%s%s%s%d%s%s%s%s%s%s","[",path,"] : ",error.NumStr," - превышен лимит элемента [",error.Str,"] для [",error.Str2,"] \n\n",src_gen);
				else if(error.Code == SXPP_ERR_REDIFINITION_GDATA)
					sprintf(mess,"%s%s%s%d%s%s%s%s%s%s%s%s","[",path,"] : ",error.NumStr," - переобъявление элемента [",SXPP_DET_ITEM_GDATA, "] в файле [",error.Str,"], впервые этот элемент был объявлен в [",error.Str2,"] \n\n",src_gen);
				else if(error.Code == SXPP_ERR_GDATA_PASS)
					sprintf(mess,"%s%s%s%d%s%s%s%s%s%s","[",path,"] : ",error.NumStr," - недопустимое объявление  [",SXPP_DET_ITEM_PASS, "] для ["SXPP_DET_ITEM_GDATA,"] \n\n",src_gen);
				else if(error.Code == SXPP_ERR_GDATA_SEQUENCE)
					sprintf(mess,"%s%s%s%d%s%s%s%s%s%s","[",path,"] : ",error.NumStr," - недопустимое объявление  [",SXPP_DET_ITEM_SEQUENCE, "] для ["SXPP_DET_ITEM_GDATA,"] \n\n",src_gen);
			MessageBox(0,mess,SXPP_ERROR_G_MESSAGE,MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		}
}

void SXWrapLoaderPPFile::AddShaderData(const char* pp,int type_shader,int passnum,const char* name_var,void* data,long size)
{
	int error = this->PP->AddShaderData(pp,type_shader,passnum,name_var,data,size);
		if(error < 0)
		{
			//ERROR 
		}
}

void SXWrapLoaderPPFile::AddShaderData(const char* pp,int type_shader,const char* pass,const char* name_var,void* data,long size)
{
	int error = this->PP->AddShaderData(pp,type_shader,pass,name_var,data,size);
		if(error < 0)
		{
			//ERROR 
		}
}

void SXWrapLoaderPPFile::AddShaderData(const char* pp,int type_shader,const char* shader_name_pp,const char* shader_name,const char* name_var,void* data,long size)
{
	int error = this->PP->AddShaderData(pp,type_shader,shader_name_pp,shader_name,name_var,data,size);
		if(error < 0)
		{
			//ERROR 
		}
}


bool SXWrapLoaderPPFile::IsPPThere(int pp)
{
	return this->PP->IsPPThere(pp);
}

IDirect3DTexture9* SXWrapLoaderPPFile::GetTexture(const char* name,int type)
{
	return this->PP->GetTexture(name,type);
}

IDirect3DTexture9* SXWrapLoaderPPFile::GetTexture(const char* ppname,int ppnum,const char* name,int type)
{
	return this->PP->GetTexture(ppname,ppnum,name,type);
}

void SXWrapLoaderPPFile::GetTexInfo(const char* ppname,int ppnum,const char* name,D3DSURFACE_DESC* desc,int type)
{
	this->PP->GetTexInfo(ppname,ppnum,name,desc,type);
}


void SXWrapLoaderPPFile::Render(const char *name_pp)
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->Render(name_pp,0,&error);

	RenderComError(&error);
}

void SXWrapLoaderPPFile::Render(int how_pp,int count_rt_save,int* rt_save_arr)
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->Render(how_pp,&error,count_rt_save,rt_save_arr);

	RenderComError(&error);
}

void SXWrapLoaderPPFile::Render(const char* name_pp,const char* how_rt_save)
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->Render(name_pp,how_rt_save,&error);

	RenderComError(&error);
}

void SXWrapLoaderPPFile::RenderComError(SXPostProcessError *error)
{
		if(error->Code != SXPP_OK)
		{
			char src_gen[SXPP_SETTINGS_SB_SRC_ERR];
			sprintf(src_gen,"%s%s%s%d","Источник генерации ошибки: [",error->GenFileError,"] : ",error->GenStrError);
			char mess[SXPP_SETTINGS_SB_ERROR_MSG];
				if(error->Code == SXPP_ERR_NOT_FOUND_PP)
					sprintf(mess,"%s%s%s%s","Не найден пост-процесс [",error->Str,"] \n\n",src_gen);
				else if(error->Code == SXPP_ERR_INVALID_RT_SAVE)
					sprintf(mess,"%s%s%s%s","Неверно сформирована строка сохранения [",error->Str,"], либо неверно указаны сохраняемые render_target \n\n",src_gen);
				else if(error->Code == SXPP_ERR_INVALID_NUMBER_OF_PP)
					sprintf(mess,"%s%s%s%s%s%s","Неверно указан номер пост-процесса [",error->Str,"], всего пост-процессов[",error->Str2,"] \n\n",src_gen);
			MessageBox(0,mess,SXPP_ERROR_G_MESSAGE,MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		}
}

bool SXWrapLoaderPPFile::IsDefined(const char* pp,int shader_type,const char* shader,const char* define)
{
	return PP->IsDefined(pp,shader_type,shader,define);
}

const char* SXWrapLoaderPPFile::GetDefine(const char* pp,int shader_type,const char* shader,const char* define)
{
	return PP->GetDefine(pp,shader_type,shader,define);
}

void SXWrapLoaderPPFile::CheckValidate(const char* name_pp)
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->CheckValidate(name_pp,&error);

	CheckValidateComError(&error);
}

void SXWrapLoaderPPFile::CheckValidate(int how_pp)
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->CheckValidate(how_pp,&error);

	CheckValidateComError(&error);
}

void SXWrapLoaderPPFile::CheckValidateComError(SXPostProcessError *error)
{
		if(error->Code != SXPP_OK)
		{
			char src_gen[SXPP_SETTINGS_SB_SRC_ERR];
			sprintf(src_gen,"%s%s%s%d","Источник генерации ошибки: [",error->GenFileError,"] : ",error->GenStrError);
			char mess[SXPP_SETTINGS_SB_ERROR_MSG];
				if(error->Code == SXPP_ERR_NOT_FOUND_PP)
					sprintf(mess,"%s%s%s%s","Не найден пост-процесс [",error->Str,"] \n\n",src_gen);
				else if(error->Code == SXPP_ERR_INVALID_NUMBER_OF_PP)
					sprintf(mess,"%s%s%s%s%s%s","Неверно указан номер пост-процесса [",error->Str,"], всего пост-процессов[",error->Str2,"] \n\n",src_gen);
				else if(error->Code == SXPP_ERR_NON_UNIC_ITEM)
					sprintf(mess,"%s%s%s%s%s%s","Не уникальный элемент [",error->Str,"] : [",error->Str2,"] \n\n",src_gen);
				else if(error->Code == SXPP_ERR_TEX_RT)
					sprintf(mess,"%s%s%s%s","Передаваемая текстура [",error->Str,"] является текущим render_target \n\n",src_gen);
				else if(error->Code == SXPP_ERR_NOT_FOUND_ITEM)
					sprintf(mess,"%s%s%s%s%s%s%s%s","Не найден элемент [",error->Str,"] : [",error->Str2,"] постпроцесс [",error->Str3,"]  \n\n",src_gen);
				else if(error->Code == SXPP_ERR_DISCREPANCY_VER_SHADER)
					sprintf(mess,"%s%s%s%s%s%s%s%s%s%s","Несовместимые версии шейдеров [",error->Str,"]:[",error->Str2,"] & [",error->Str3,"]: [",error->Str4,"]  \n\n",src_gen);
				else if(error->Code == SXPP_ERR_G_NON_UNIC_ITEM)
					sprintf(mess,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s","Одинаковые имена элементов [",error->Str,"]:[",error->Str2,"] type [",error->Str3,"] & [",error->Str4,"]:[",error->Str5,"] type [",error->Str6,"]  \n\n",src_gen);
			MessageBox(0,mess,SXPP_ERROR_G_MESSAGE,MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		}
}

void SXWrapLoaderPPFile::InitData(const char* name_pp)
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->InitData(name_pp,&error);

	InitDataComError(&error);
}

void SXWrapLoaderPPFile::InitData(int how_pp)
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->InitData(how_pp,&error);

	InitDataComError(&error);
}

void SXWrapLoaderPPFile::InitDataComError(SXPostProcessError *error)
{
		if(error->Code != SXPP_OK)
		{
			char src_gen[SXPP_SETTINGS_SB_SRC_ERR];
			sprintf(src_gen,"%s%s%s%d","Источник генерации ошибки: [",error->GenFileError,"] : ",error->GenStrError);
			char mess[SXPP_SETTINGS_SB_ERROR_MSG];
				if(error->Code == SXPP_ERR_NOT_FOUND_PP)
					sprintf(mess,"%s%s%s%s","Не найден пост-процесс [",error->Str,"] \n\n",src_gen);
				else if(error->Code == SXPP_ERR_INVALID_NUMBER_OF_PP)
					sprintf(mess,"%s%s%s%s%s%s","Неверно указан номер пост-процесса [",error->Str,"], всего пост-процессов[",error->Str2,"] \n\n",src_gen);
				else if(error->Code == SXPP_ERR_NOT_FOUND_ITEM)
					sprintf(mess,"%s%s%s%s%s%s%s%s","Не найден элемент [",error->Str,"] : [",error->Str2,"] в пост-процессе [",error->Str3,"] \n\n",src_gen);
				else if(error->Code == SXPP_ERR_NON_END_REC)
					sprintf(mess,"%s%s%s%s%s%s%s%s%s%s","Элемент [",error->Str,"] : [",error->Str2,"] ссылается на элемент [",error->Str3,"] : [",error->Str4,"] которые сам на него и ссылается (рекусивная отсылка) \n\n",src_gen);
				else if(error->Code == SXPP_ERR_MAX_NUM_REC)
					sprintf(mess,"%s%s%s%s%s%s","Преышен лимит рекурсивного вызова определения конечного родителя для [",error->Str,"] : [",error->Str2,"] \n\n",src_gen);
				MessageBox(0,mess,SXPP_ERROR_G_MESSAGE,MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		}
}

void SXWrapLoaderPPFile::InitDataAll()
{
	SXPostProcessError error = SXPostProcessError();
	int result = PP->InitDataAll(&error);

	InitDataComError(&error);
}

void SXWrapLoaderPPFile::CheckValidateAll()
{
		for(int i=0;i<this->GetPPNum();i++)
			this->CheckValidate(i);
}

#endif