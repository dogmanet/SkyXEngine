#ifndef _IHTMLparser_H_
#define _IHTMLparser_H_

#include "GUIbase.h"
#include "DOM.h"
#include "DOMdocument.h"

namespace gui
{
	namespace dom
	{
		class IHTMLparser
		{
		public:
			IHTMLparser():m_pDocument(NULL)
			{
			}
			IDOMnode * createFromString(const wchar_t * str, int * pNextSym);
			IDOMnode * createFromFile(const wchar_t * file);

			wchar_t parseMetaSym(const StringW & str, bool isNumeric);

			static AssotiativeArray<StringW, wchar_t> m_mMetaSyms;
			static void fillMetaTable();

			void setDocument(IDOMdocument * doc)
			{
				m_pDocument = doc;
			}

		protected:
			IDOMdocument * m_pDocument;
		};
	};
};

#endif
