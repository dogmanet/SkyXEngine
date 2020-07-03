#ifndef __SCENE_H
#define __SCENE_H

#include <xcommon/IXScene.h>
#include <xcommon/IXCore.h>
#include <common/aastring.h>
#include <common/queue.h>

#define BVH_CHILD_COUNT 27

typedef UINT NodeFeature;
typedef UINT NodeType;

//##########################################################################

class CScene;
class CSceneNode;
class CSceneObjectType;
class CSceneObject final: public IXUnknownImplementation<IXSceneObject>
{
	friend class CScene;
public:

	CSceneObject(CScene *pScene, const SMAABB &aabb, void *pUserData, NodeType bmType, NodeFeature bmFeatures);
	~CSceneObject();
	
	void XMETHODCALLTYPE update(const SMAABB &aabb) override;
	void updateFeatures(NodeFeature bmFeatures);

	void XMETHODCALLTYPE setFeature(IXSceneFeature *pFeat, bool isSet) override;
	void XMETHODCALLTYPE setFeatures(IXSceneFeature **ppFeatures) override;

	void setNode(CSceneNode *pNode);
	CSceneNode* getNode();

	const SMAABB& getAABB() const;

	void* getUserData() const;

	NodeFeature getFeatures() const
	{
		return(m_bmFeatures);
	}
	NodeType getType() const
	{
		return(m_bmType);
	}

private:
	SMAABB m_aabb;

	CScene *m_pScene;
	CSceneNode *m_pNode = NULL;

	NodeFeature m_bmFeatures;
	NodeType m_bmType;

	void *m_pUserData;

	void XMETHODCALLTYPE FinalRelease() override;
};

//##########################################################################

class CSceneFeature final: public IXUnknownImplementation<IXSceneFeature>
{
public:
	CSceneFeature(UINT uId):
		m_bmFeature(1 << uId)
	{
	}

	NodeFeature getFeature()
	{
		return(m_bmFeature);
	}

private:
	NodeFeature m_bmFeature;
};

//##########################################################################

class CSceneQuery final: public IXUnknownImplementation<IXSceneQuery>
{
public:
	CSceneQuery(CScene *pScene, CSceneObjectType *pObjectType);
	~CSceneQuery();

	UINT XMETHODCALLTYPE execute(const IXFrustum *pFrustum, void ***pppObjects, IXOcclusionCuller *pOcclusionCuller = NULL) override;

	void XMETHODCALLTYPE setOP(XSCENE_QUERY_OP op) override;

	void XMETHODCALLTYPE setFeature(IXSceneFeature *pFeat, XSCENE_QUERY_FEATURE mode) override;

private:
	CScene *m_pScene;
	NodeType m_bmType;
	Array<void*> m_aQueryResponse;

	XSCENE_QUERY_OP m_op = SQO_AND;
	NodeFeature m_bmSet = 0;
	NodeFeature m_bmUnset = 0;

	void queryObjectsInternal(CSceneNode *pNode, const IXFrustum *pFrustum, bool isFullyVisible = false, IXOcclusionCuller *pOcclusionCuller = NULL);
	bool testFeatures(NodeFeature bmFeatures, bool isStrict = true);
};

//##########################################################################

class CSceneObjectType final: public IXUnknownImplementation<IXSceneObjectType>
{
public:
	CSceneObjectType(CScene *pScene, UINT uId);
	~CSceneObjectType();

	IXSceneObject* XMETHODCALLTYPE newObject(const SMAABB &aabb, void *pUserData, IXSceneFeature **ppFeatures = NULL) override;
	IXSceneQuery* XMETHODCALLTYPE newQuery() override;

	NodeType getType()
	{
		return(m_bmType);
	}

private:
	CScene *m_pScene;

	NodeType m_bmType;
};

//##########################################################################

class CSceneNode final
{
	friend class CScene;
public:

	CSceneNode(CScene *pScene, CSceneNode *pParent = NULL);
	~CSceneNode();

	SX_ALIGNED_OP_MEM2();

	void addObject(CSceneObject *pObject, bool force = false);
	void removeObject(CSceneObject *pObject);

	bool updateObject(CSceneObject *pObject, const SMAABB &aabbOld);

	const SMAABB& getAABB(bool doSplit = true);

	CSceneNode* getChild(int idx, bool shouldCreate = true);
	
	Array<CSceneObject*>& getObjects();

	NodeFeature getFeatures() const
	{
		return(m_bmFeatures);
	}
	NodeType getTypes() const
	{
		return(m_bmTypes);
	}

protected:
	int selectChild(const SMAABB &aabb);
	CSceneNode* findNode(const SMAABB &aabb);
	void insertObject(CSceneObject *pObject);
	bool removeObject(CSceneObject *pObject, const SMAABB &aabbOld);

	void growExtents(const SMAABB &aabb);
	void shrinkExtents(const SMAABB &aabb);
	void updateExtents();

	void testSuicide();
	void unsplit();
	void doSplit();

	void updateFeatures();

	void removeChild(CSceneNode *pNode);

	CSceneNode *m_pParent = NULL;
	CSceneNode *m_pChildren[BVH_CHILD_COUNT];

	CScene *m_pScene;

	SMAABB m_aabb;

	Array<CSceneObject*> m_aObjects;

	volatile bool m_isSplit = false;
	float3 m_vSplit;

	volatile bool m_isExtentsCorrect = false;

	SpinLock m_lock;

	NodeFeature m_bmFeatures = 0;
	NodeType m_bmTypes = 0;
};

//##########################################################################

class CDevBVHrenderInc;
class CDevBVHrenderDec;
class CCvarListener;
class CScene final: public IXUnknownImplementation<IXScene>
{
	friend class CSceneObject;
	friend class CSceneQuery;
public:
	CScene(IXCore *pCore);
	~CScene();

	IXSceneObjectType* XMETHODCALLTYPE registerObjectType(const char *szName) override;
	IXSceneObjectType* XMETHODCALLTYPE getObjectType(const char *szName) override;

	IXSceneFeature* XMETHODCALLTYPE registerObjectFeature(const char *szName) override;
	IXSceneFeature* XMETHODCALLTYPE getObjectFeature(const char *szName) override;

	IXSceneObject* newObject(const SMAABB &aabb, void *pUserData, NodeType bmType, IXSceneFeature **ppFeatures);

	UINT XMETHODCALLTYPE getTreeHeight() override;

	void drawLevel(int iLvl);

	CSceneNode* newNode(CSceneNode *pParent);
	void deleteNode(CSceneNode *pNode);

	void enqueueObjectUpdate(CSceneObject* pObject, const SMAABB &aabb);
	void enqueueObjectUpdateFeatures(CSceneObject* pObject, NodeFeature bmFeatures);
	void enqueueObjectDelete(CSceneObject* pObject);

	void sync();
	
protected:
	void addObject(CSceneObject *pObject);
	void removeObject(CSceneObject *pObject);

	IXCore *m_pCore;
private:
	CSceneNode *m_pRootNode = NULL;

	MemAlloc<CSceneNode> m_poolNodes;
	MemAlloc<CSceneObject> m_poolObjects;
	SpinLock m_lockPoolObjects;

	struct UpdateItem
	{
		SMAABB aabbNew;
		CSceneObject *pObject;
		NodeFeature bmFeatures;
		enum
		{
			UPDATE,
			UPDATE_FEATURES,
			REMOVE,
			ADD
		} action;
	};

	Queue<UpdateItem> m_qUpdate;

	CDevBVHrenderInc *m_pDevBVHrenderInc = NULL;
	CDevBVHrenderDec *m_pDevBVHrenderDec = NULL;

	CCvarListener *m_pCvarListener = NULL;

	Array<IXModel*> m_aModels;

	AssotiativeArray<AAString, CSceneObjectType*> m_mapTypes;
	AssotiativeArray<AAString, CSceneFeature*> m_mapFeatures;

	void drawLevelInternal(CSceneNode *pNode, int iLvl, int iCurLvl = 0);
};

#endif
