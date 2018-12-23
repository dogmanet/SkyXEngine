
#include "ICSSstyle.h"

namespace gui
{
	namespace css
	{
		Array<const WCHAR *> CCSSstringManager::m_aStrings;

		int CCSSstringManager::getStringID(const WCHAR * szValue)
		{
			for(int i = 0, l = m_aStrings.size(); i < l; ++i)
			{
				if(!lstrcmpW(szValue, m_aStrings[i]))
				{
					return(i);
				}
			}
			int len = lstrlenW(szValue);
			WCHAR * str = new WCHAR[len + 1];
			memcpy(str, szValue, (len + 1) * sizeof(WCHAR));
			m_aStrings.push_back(str);
			return(m_aStrings.size() - 1);
		}

		const WCHAR * CCSSstringManager::getString(int id)
		{
			assert(id >= 0 && id < (int)m_aStrings.size());
			return(m_aStrings[id]);
		}
	};
};
