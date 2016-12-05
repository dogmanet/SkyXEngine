#ifndef INETservice_H
#define INETservice_H

#include <sxtypes.h>
namespace NET
{
	class INETservice
	{
	public:
		INETservice();
		virtual ~INETservice();

		virtual void Update() = 0;
		virtual void Syncronize() = 0;

	private:
		UINT m_iID;
	};
};

#endif
