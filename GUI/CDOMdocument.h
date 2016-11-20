#ifndef _CDOMdocument_H_
#define _CDOMdocument_H_

#include "IDOM.h"
#include "IRenderFrame.h"
#include "IDOMdocument.h"

namespace GUI
{
	class CDesktop;
	namespace DOM
	{
		class CDOMdocument: public IDOMdocument
		{
		public:
			CDOMdocument():
				m_pRootNode(NULL),
				m_pRTroot(NULL)
			{
				m_pCSS = new CSS::ICSS(this);
			}

			~CDOMdocument()
			{
				SX_SAFE_DELETE(m_pCSS);
			}

			IDOMnode * CreateNode(const wchar_t * tag);
			IDOMnode * CreateNode(UINT nid);

			void SetRootNode(IDOMnode * pNode);

			IDOMnode * GetElementById(const StringW & id);
			IDOMnode * GetElementById(UINT iid);

			void IndexBuild();
			void IndexAdd(IDOMnode * pNode);
			void IndexRemove(IDOMnode * pNode);

			UINT GetIndexForIdString(const StringW & id);
			UINT GetIndexForClassString(const StringW & id);
			UINT GetIndexForTagString(const StringW & id);

			static void BuildIndexFunc(IDOMdocument * doc, IDOMnode * node);

			const IDOMnodeCollection * GetElementsByClass(const StringW & id);
			const IDOMnodeCollection * GetElementsByClass(UINT cid);

			const IDOMnodeCollection * GetElementsByPseudoClass(UINT cid);

			const IDOMnodeCollection * GetElementsByTag(const StringW & id);
			const IDOMnodeCollection * GetElementsByTag(UINT tid);

			IDOMnodeCollection QuerySelectorAll(const CSS::ICSSRuleSet * rules);

			CSS::ICSS * GetCSS();

			void LoadStyles();

			void CalculateStyles();

			void IndexUnsetId(UINT id);
			void IndexSetId(UINT id, IDOMnode * pNode);

			void IndexUnsetClass(UINT id, IDOMnode * pNode);
			void IndexSetClass(UINT id, IDOMnode * pNode);

			void IndexUnsetPseudoClass(UINT id, IDOMnode * pNode);
			void IndexSetPseudoClass(UINT id, IDOMnode * pNode);

			void IndexUnsetNode(CDOMnode * pNode);
			void IndexSetNode(CDOMnode * pNode);

			void UpdateStyleSubtree(IDOMnode * pStartNode);
			void UpdateStyles();

			IDOMnodeCollection QuerySelectorSimple(const CSS::ICSSrule::ICSSselectorItem * sel);

			void BuildRenderThree();

			void Render();

			IDOMnode * GetElementByXY(int x, int y, bool sendEnterLeave = false);

			//	IDesktop * GetDesktop();
			void SetDesktop(IDesktop * pdp);

			void RequestFocus(IDOMnode * pn);
			IDOMnode * GetFocus();

			void AddReflowItem(Render::IRenderFrame * rf);
			void Reflow();

		protected:
			IDOMnode * m_pRootNode;

			CSS::ICSS * m_pCSS;

			CDesktop * m_pDesktop;

			//index
			AssotiativeArray<StringW, UINT> m_IndexStringById;
			AssotiativeArray<StringW, UINT> m_IndexStringByClass;
			IDOMnodeCollection m_IndexById;
			Array<IDOMnodeCollection> m_IndexByClass;
			Array<IDOMnodeCollection> m_IndexByPseudoClass;
			Array<IDOMnodeCollection> m_IndexByTagName;

			Array<Render::IRenderFrame*> m_ReflowQueue;

			IDOMnodeCollection m_UpdateStyleQueue;

			//void ApplyCSSrules(const CSS::ICSSstyle * style, IDOMnode * pNode);

			Render::IRenderFrame * m_pRTroot;
		};
	};
};

#endif
