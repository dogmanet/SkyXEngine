#ifndef _IHTMLparser_H_
#define _IHTMLparser_H_

#include "GUIbase.h"
#include "CDOM.h"
#include "CDOMdocument.h"

namespace GUI
{
	namespace DOM
	{
		class IHTMLparser
		{
		public:
			IHTMLparser():m_pDocument(NULL)
			{
			}
			IDOMnode * CreateFromString(const wchar_t * str, int * pNextSym);
			IDOMnode * CreateFromFile(const wchar_t * file);

			wchar_t ParseMetaSym(const StringW & str, bool isNumeric);

			static AssotiativeArray<StringW, wchar_t> m_mMetaSyms;
			static void FillMetaTable();

			void SetDocument(IDOMdocument * doc)
			{
				m_pDocument = doc;
			}

		protected:
			IDOMdocument * m_pDocument;
		};
	};
};

#endif
