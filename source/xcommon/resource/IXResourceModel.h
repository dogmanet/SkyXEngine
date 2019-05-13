#ifndef __IXRESOURCEMODEL_H
#define __IXRESOURCEMODEL_H

#include <gdefines.h>
#include <anim/ModelFile.h>

/*! Типы физбоксов
*/
enum XPHYSBOXTYPE
{
	XPBT_BOX = 0,     /*!< Параллелепипед */
	XPBT_SPHERE,      /*!< Сфера */
	XPBT_CONVEX,

	// XPBT_CYLINDER, /*!< Цилиндр */
	// XPBT_CAPSULE   /*!< Капсула */
};

/*! Типы хитбоксов
*/
enum XHITBOXTYPE
{
	XHT_BOX = 0,  /*!< Параллелепипед */
	XHT_CYLINDER, /*!< Цилиндр */
	XHT_CAPSULE,  /*!< Капсула */
	XHT_SPHERE    /*!< Сфера */
};

/*! Типы частей тела для хитбоксов
*/
enum XHITBOXBODYPART
{
	XHBP_DEFAULT = 0, /*!< нет */
	XHBP_HEAD,        /*!< Голова */
	XHBP_CHEST,       /*!< Грудь */
	XHBP_STOMACH,     /*!< Живот */
	XHBP_LEFTARM,     /*!< Левая рука */
	XHBP_RIGHTARM,    /*!< Правая рука */
	XHBP_LEFTLEG,     /*!< Левая нога */
	XHBP_RIGHTLEG     /*!< Правая нога */
};

/*! Типы топологий примитивов в модели
*/
enum XPT_TOPOLOGY
{
	XPT_TRIANGLELIST = 0,  /*!< Списки треугольников */
	XPT_TRIANGLESTRIP = 1, /*!< Полоски треугольников */
};


/*! Типы ресурсов моделей
*/
enum XMODELTYPE
{
	XMT_STATIC = 0,
	XMT_ANIMATED = 1,
};

//##########################################################################

struct XResourceModelStaticVertex
{
	float3_t vPos;      /*!< Позиция */
	float2_t vTex;      /*!< Текстурные координаты */
	float3_t vNorm;     /*!< Нормаль */
	float3_t vTangent;  /*!< Тангент */
	float3_t vBinorm;   /*!< Бинормаль */
};

struct XResourceModelStaticSubset
{
	uint32_t iMaterialID; //!< Идентификатор материала
	uint32_t iVectexCount; //!< Количество вертексов в сабсете
	uint32_t iIndexCount; //!< Количество индексов в сабсете
	XResourceModelStaticVertex *pVertices; //!< Указатель на массив вертексов
	UINT *pIndices; //!< Массив индексов
};

struct XResourceModelAnimatedVertex
{
	float3_t vPos;      /*!< Позиция */
	float2_t vTex;      /*!< Текстурные координаты */
	float3_t vNorm;     /*!< Нормаль */
	float3_t vTangent;  /*!< Тангент */
	float3_t vBinorm;   /*!< Бинормаль */
	byte u8BoneIndices[4];  /*!< Индексы костей */
	float4_t vBoneWeights; /*!< Веса костей */
};

struct XResourceModelAnimatedSubset
{
	uint32_t iMaterialID; //!< Идентификатор материала
	uint32_t iVectexCount; //!< Количество вертексов в сабсете
	uint32_t iIndexCount; //!< Количество индексов в сабсете
	XResourceModelAnimatedVertex * pVertices; //!< Указатель на массив вертексов
	UINT * pIndices; //!< Массив индексов
};

struct XResourceModelHitbox
{
	XHITBOXTYPE type;      /*!< Тип хитбокса */
	XHITBOXBODYPART part;  /*!< Часть тела */
	float3_t lwh;          /*!< Размеры */
	float3_t pos;          /*!< Положение */
	SMQuaternion rot;      /*!< Ориентация */
	int bone_id;           /*!< Идентификатор кости */
	const char *szName;    /*!< Имя хитбокса */
};

struct XResourceModelBone
{
	SMQuaternion orient; /*!< Вращение */
	float3_t position;   /*!< Позиция */
};

struct XResourceModelSequence
{
	const char *szName; /*!< Имя */
	bool isLooped; /*!< Анимация зациклена */
	int iFramerate; /*!< Скорость кадров в секунду */
	int iActivity; /*!< Идентификатор активности */
	UINT uNumFrames; /*!< Количество кадров */
	UINT uActivityChance; /*!< Шанс воспроизведения этой анимации в активности */
	XResourceModelBone **m_ppSequenceData; /*!< Данные анимации */
};

struct XResourceModelController
{
	const char *szName;    /*!< Имя контроллера */
	const UINT uBoneCount; /*!< Количество костей, которыми управляет контроллер */

	SMQuaternion qMinRot;  /*!< Вращение ОТ */
	SMQuaternion qMaxRot;  /*!< Вращение ДО */
	float3_t vMinTrans;    /*!< Перемещение ОТ */
	float3_t vMaxTrans;    /*!< Перемещение ДО */
	UINT *pBones;          /*!< Массив индексов костей */
};

//##########################################################################

class IModelPhysboxBox;
class IModelPhysboxSphere;
class IModelPhysboxConvex;
class IModelPhysbox: public IXUnknown
{
public:
	virtual XPHYSBOXTYPE XMETHODCALLTYPE getType() const = 0;

	virtual const IModelPhysboxBox * XMETHODCALLTYPE asBox() const = 0;
	virtual const IModelPhysboxSphere * XMETHODCALLTYPE asSphere() const = 0;
	virtual const IModelPhysboxConvex * XMETHODCALLTYPE asConvex() const = 0;

	virtual float3 XMETHODCALLTYPE getPosition() const = 0;
	virtual void XMETHODCALLTYPE setPosition(const float3 &vPos) = 0;

	virtual SMQuaternion XMETHODCALLTYPE getOrientation() const = 0;
	virtual void XMETHODCALLTYPE setOrientation(const SMQuaternion &vPos) = 0;
};

class IModelPhysboxBox: public IModelPhysbox
{
public:
	virtual float3 XMETHODCALLTYPE getSize() const = 0;
	virtual void XMETHODCALLTYPE setSize(const float3 &vSize) = 0;
};

class IModelPhysboxSphere: public IModelPhysbox
{
public:
	virtual float XMETHODCALLTYPE getRadius() const = 0;
	virtual void XMETHODCALLTYPE setRadius(float fRadius) = 0;
};

class IModelPhysboxConvex: public IModelPhysbox
{
public:
	virtual UINT XMETHODCALLTYPE getVertexCount() const = 0;
	virtual const float3_t * XMETHODCALLTYPE getData() const = 0;

	virtual void XMETHODCALLTYPE initData(UINT uVertexCount, const float3_t *pData = NULL) = 0;
	virtual float3_t * XMETHODCALLTYPE getData() = 0;
};

//##########################################################################

class IXResourceModelStatic;
class IXResourceModelAnimated;
// Implemented in core
class IXResourceModel: public IXUnknown
{
public:

	virtual void XMETHODCALLTYPE setPrimitiveTopology(XPT_TOPOLOGY topology) = 0;
	virtual XPT_TOPOLOGY XMETHODCALLTYPE getPrimitiveTopology() const = 0;


	virtual UINT XMETHODCALLTYPE getPhysboxCount() const = 0;
	virtual const IModelPhysbox * XMETHODCALLTYPE getPhysbox(UINT uPart) const = 0;
	virtual void XMETHODCALLTYPE addPhysbox(IModelPhysbox *pPhysbox) = 0;


	virtual UINT XMETHODCALLTYPE getMaterialCount() const = 0;
	virtual UINT XMETHODCALLTYPE getSkinCount() const = 0;
	virtual const char * XMETHODCALLTYPE getMaterial(UINT uMaterial, UINT uSkin = 0) const = 0;
	virtual void XMETHODCALLTYPE setMaterialCount(UINT uMaterialCount, UINT uSkinCount) = 0;
	virtual void XMETHODCALLTYPE setMaterial(UINT uMaterial, UINT uSkin, const char *szName) = 0;


	virtual float XMETHODCALLTYPE getMass() const = 0;
	virtual void XMETHODCALLTYPE setMass(float fMass) = 0;


	virtual UINT XMETHODCALLTYPE getGibsCount() const = 0;
	virtual const char * XMETHODCALLTYPE getGibName(UINT uIndex) const = 0;
	virtual UINT XMETHODCALLTYPE addGibName(const char *szFileName) = 0;

	virtual const IXResourceModel * XMETHODCALLTYPE getGib(UINT uIndex) const = 0;
	virtual void XMETHODCALLTYPE setGib(UINT uIndex, IXResourceModel *pResource) = 0;

	virtual XMODELTYPE XMETHODCALLTYPE getType() const = 0;
	virtual const IXResourceModelStatic * XMETHODCALLTYPE asStatic() const = 0;
	virtual const IXResourceModelAnimated * XMETHODCALLTYPE asAnimated() const = 0;


	virtual UINT XMETHODCALLTYPE getSubsetCount(UINT uLod) const = 0;
	virtual UINT XMETHODCALLTYPE addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount) = 0;
};

// Implemented in core
class IXResourceModelStatic: public IXResourceModel
{
public:

	virtual const XResourceModelStaticSubset * XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) const = 0;
	virtual XResourceModelStaticSubset * XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) = 0;
};

// Implemented in core
class IXResourceModelAnimated: public IXResourceModel
{
public:
	virtual const XResourceModelAnimatedSubset * XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) const = 0;
	virtual XResourceModelAnimatedSubset * XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) = 0;


	virtual void XMETHODCALLTYPE setBoneCount(UINT uCount) = 0;
	virtual UINT XMETHODCALLTYPE getBoneCount() const = 0;

	virtual void XMETHODCALLTYPE setBoneInfo(int iBone, const char *szName, int iParent, const float3 &vTranslation, const SMQuaternion &vRotation) = 0;
	virtual int XMETHODCALLTYPE getBoneParent(int iBone) const = 0;
	virtual const char * XMETHODCALLTYPE getBoneName(int iBone) const = 0;
	virtual float3 XMETHODCALLTYPE getBoneTranslation(int iBone) const = 0;
	virtual SMQuaternion XMETHODCALLTYPE getBoneRotation(int iBone) const = 0;


	virtual UINT XMETHODCALLTYPE getSequenceCount() const = 0;
	virtual const XResourceModelSequence * XMETHODCALLTYPE getSequence(UINT uIndex) const = 0;

	virtual void XMETHODCALLTYPE setSequenceCount() = 0;
	virtual XResourceModelSequence * XMETHODCALLTYPE getSequence(UINT uIndex) = 0;
	virtual void XMETHODCALLTYPE setSequenceName(UINT uIndex, const char *szName) = 0;
	virtual void XMETHODCALLTYPE setSequenceFrameCount(UINT uIndex, UINT uFrameCount) = 0;
	

	virtual UINT XMETHODCALLTYPE addActivity(const char *szName) = 0;
	virtual const char * XMETHODCALLTYPE getActivityName(UINT uIndex) const = 0;
	virtual UINT XMETHODCALLTYPE getActivitiesCount() const = 0;


	virtual UINT XMETHODCALLTYPE getControllersCount() const = 0;
	virtual void XMETHODCALLTYPE setControllersCount() = 0;
	virtual void XMETHODCALLTYPE setControllerInfo(UINT uIndex, const char *szName, UINT uAffectedBonesCount) = 0;
	virtual const XResourceModelController * XMETHODCALLTYPE getController(UINT uIndex) const = 0;
	virtual XResourceModelController * XMETHODCALLTYPE getController(UINT uIndex) = 0;


	virtual UINT XMETHODCALLTYPE getHitboxCount() const = 0;
	virtual void XMETHODCALLTYPE setHitboxCount() = 0;
	virtual void XMETHODCALLTYPE setHitboxName(const char *szHitbox) = 0;
	virtual XResourceModelHitbox * XMETHODCALLTYPE getHitbox(UINT uIndex) = 0;
	virtual const XResourceModelHitbox * XMETHODCALLTYPE getHitbox(UINT uIndex) const = 0;
};


#endif
