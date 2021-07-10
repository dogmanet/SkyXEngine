#ifndef _DSHCLOD_H_
#define _DSHCLOD_H_

#include "DSbase.h"

class DSHCLOD
{
public:
	class DSHCLODstripe
	{
		UINT * m_pHeigts;
		UINT m_iLength;

	public:
		DSHCLODstripe(UINT size);
		DSHCLODstripe();
		~DSHCLODstripe();

		void Init(UINT size);

		const UINT & operator[](int64_t index) const;
		UINT & operator[](int64_t index);

		UINT & at(int64_t index);

		UINT length() const;
	};
protected:
	DSHCLODstripe * m_pHeights;
	UINT m_iLevel;

	UINT m_iStripeCount;
	UINT m_iStripeLength;

	bool m_bBuilded;

	ID3D11Buffer * m_pIndexBuffer;
	ID3D11Buffer * m_pVertexBuffer;

	ID3D11Buffer * m_pVertexNormalBuffer;
	UINT m_iNormalCount;

	UINT m_iIndexCount;
	XMMATRIX m_mPos;

	bool m_bInRender;
	bool m_bInConstruct;

	DSmaterialSystem::vertex * pVertices;
	UINT * pIndices;
public:

	DSHCLOD();
	~DSHCLOD();

	void Init();

	void Build();

	void Render();

	void Release();

	UINT GetLevel() const;

	DSHCLODstripe & operator[](int64_t index);
	const DSHCLODstripe & operator[](int64_t index) const;

};

#endif
