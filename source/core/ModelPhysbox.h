#ifndef __MODELPHYSBOX_H
#define __MODELPHYSBOX_H

#include <xcommon/resource/IXResourceModel.h>

template<class T>
class IModelPhysboxImplementation: public IXUnknownImplementation<T>
{
public:
	const IModelPhysboxBox * XMETHODCALLTYPE asBox() const override
	{
		return(NULL);
	}
	const IModelPhysboxSphere * XMETHODCALLTYPE asSphere() const override
	{
		return(NULL);
	}
	const IModelPhysboxConvex * XMETHODCALLTYPE asConvex() const override
	{
		return(NULL);
	}
	const IModelPhysboxCylinder * XMETHODCALLTYPE asCylinder() const override
	{
		return(NULL);
	}
	const IModelPhysboxCapsule * XMETHODCALLTYPE asCapsule() const override
	{
		return(NULL);
	}

	float3 XMETHODCALLTYPE getPosition() const override
	{
		return(m_vPos);
	}
	void XMETHODCALLTYPE setPosition(const float3 &vPos) override
	{
		m_vPos = vPos;
	}

	SMQuaternion XMETHODCALLTYPE getOrientation() const override
	{
		return(m_qRot);
	}
	void XMETHODCALLTYPE setOrientation(const SMQuaternion &qRot) override
	{
		m_qRot = qRot;
	}

private:
	float3_t m_vPos;
	SMQuaternion m_qRot;
};

class CModelPhysboxBox: public IModelPhysboxImplementation<IModelPhysboxBox>
{
public:
	XPHYSBOXTYPE XMETHODCALLTYPE getType() const override;

	const IModelPhysboxBox * XMETHODCALLTYPE asBox() const override;

	float3 XMETHODCALLTYPE getSize() const override;
	void XMETHODCALLTYPE setSize(const float3 &vSize) override;

protected:
	float3_t m_vSize;
};

class CModelPhysboxSphere: public IModelPhysboxImplementation<IModelPhysboxSphere>
{
public:
	XPHYSBOXTYPE XMETHODCALLTYPE getType() const override;

	const IModelPhysboxSphere * XMETHODCALLTYPE asSphere() const override;

	float XMETHODCALLTYPE getRadius() const override;
	void XMETHODCALLTYPE setRadius(float fRadius) override;

protected:
	float m_fRadius = 0.0f;
};

class CModelPhysboxConvex: public IModelPhysboxImplementation<IModelPhysboxConvex>
{
public:
	~CModelPhysboxConvex();

	XPHYSBOXTYPE XMETHODCALLTYPE getType() const override;

	const IModelPhysboxConvex * XMETHODCALLTYPE asConvex() const override;

	UINT XMETHODCALLTYPE getVertexCount() const override;
	const float3_t * XMETHODCALLTYPE getData() const override;

	void XMETHODCALLTYPE initData(UINT uVertexCount, const float3_t *pData = NULL) override;
	float3_t * XMETHODCALLTYPE getData() override;

protected:
	UINT m_uVertexCount = 0;
	float3_t *m_pData = NULL;
};

class CModelPhysboxCylinder: public IModelPhysboxImplementation<IModelPhysboxCylinder>
{
public:
	XPHYSBOXTYPE XMETHODCALLTYPE getType() const override;

	const IModelPhysboxCylinder * XMETHODCALLTYPE asCylinder() const override;

	float XMETHODCALLTYPE getRadius() const override;
	void XMETHODCALLTYPE setRadius(float fRadius) override;

	float XMETHODCALLTYPE getHeight() const override;
	void XMETHODCALLTYPE setHeight(float fHeight) override;

protected:
	float m_fRadius = 0.0f;
	float m_fHeight = 0.0f;
};

class CModelPhysboxCapsule: public IModelPhysboxImplementation<IModelPhysboxCapsule>
{
public:
	XPHYSBOXTYPE XMETHODCALLTYPE getType() const override;

	const IModelPhysboxCapsule * XMETHODCALLTYPE asCapsule() const override;

	float XMETHODCALLTYPE getRadius() const override;
	void XMETHODCALLTYPE setRadius(float fRadius) override;

	float XMETHODCALLTYPE getHeight() const override;
	void XMETHODCALLTYPE setHeight(float fHeight) override;

protected:
	float m_fRadius = 0.0f;
	float m_fHeight = 0.0f;
};

#endif
