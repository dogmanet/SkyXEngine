#ifndef _CDOM_H_
#define _CDOM_H_

#include "IDOM.h"
#include "CCSSstyle.h"

#define LINK_DOM_NODE_TO_TAG(cls, name) static IDOMnodeFactory<cls> C##cls(name)
#define DECLARE_DOM_NODE(cls) static CDOMnode * GetInstance(UINT nid);
#define DEFINE_DOM_NODE(cls)							\
	CDOMnode * cls::GetInstance(UINT nid)				\
	{													\
		CDOMnode * node = new cls();					\
		node->SetNodeID(nid);							\
		return(node);									\
	}

namespace GUI
{
	namespace DOM
	{
		class CDOMdocument;
		class CDOMnode;
		namespace Render
		{
			class IRenderFrame;
		};

		struct vTable
		{
			CDOMnode*(*getInstance)(UINT);
			StringW wsNodeName;
		};
		class CDOMnode: public IDOMnode
		{
		public:
			friend class CDOMdocument;

			CDOMnode():
				m_bDelegateEvents(false),
				m_bToggleOnly(false),
				m_bToggleable(false),
				m_bToggleState(false),
				m_pParent(NULL),
				m_iNodeId(0),
				m_pDocument(NULL),
				m_iDOMid(0),
				m_pPrevSibling(NULL),
				m_pNextSibling(NULL),
				m_pseudoclasses(0),
				m_pRenderFrame(NULL),
				m_bEditable(false),
				m_bFocusable(false)
			{};
			~CDOMnode();
			void AppendChild(IDOMnode * pEl, bool regen = true);
			void RemoveChild(IDOMnode * pEl, bool regen = true);
			IDOMnode * ParentNode();

			void SetDocument(CDOMdocument * doc)
			{
				m_pDocument = doc;
			}

			void SetNodeID(UINT nid)
			{
				m_iNodeId = nid;
			}

			virtual void DebugPrintMe(UINT lvl = 0);

			static const StringW & GetNodeNameById(UINT nid);
			static UINT GetNodeIdByName(const StringW & tag);

			void SetText(const StringW & text, BOOL build = FALSE);

			static IDOMnode * CreateNode(const wchar_t * tag)
			{
				WCHAR * t = new WCHAR[wcslen(tag) + 1];
				memcpy(t, tag, (wcslen(tag) + 1) * sizeof(wchar_t));
				_wcslwr_s(t, wcslen(tag) + 1);
				StringW s(t);
				SX_SAFE_DELETE_A(t);
				if(nodeIDs.KeyExists(s))
				{
					int nid = nodeIDs[s];
					vTable * vt = &nodes[nid];
					return(vt->getInstance(nid));
				}
				return(NULL);
			}

			static IDOMnode * CreateNode(UINT nid)
			{
				if(nodeIDs.Size() > nid/* && nid > 0*/)
				{
					vTable * vt = &nodes[nid];
					return(vt->getInstance(nid));
				}
				return(NULL);
			}

			virtual bool IsTextNode()
			{
				return(false);
			}

			virtual void SetAttribute(const StringW & name, const StringW & value);
			const StringW & GetAttribute(const StringW & name);

			static AssotiativeArray<StringW, UINT> nodeIDs;
			static Array<vTable> nodes;

			void ApplyChildStyle();
			void ResetStyles();
			void ResetStyleChanges();

			bool IsChildOf(IDOMnode * pNode);
			bool IsAfter(IDOMnode * pNode);

			IDOMnode * PrevSibling()
			{
				return(m_pPrevSibling);
			}

			IDOMnode * NextSibling()
			{
				return(m_pNextSibling);
			}

			CSS::ICSSstyle * GetStyle()
			{
				return(&m_css);
			}

			CSS::ICSSstyle * GetStyleSelf()
			{
				return(&m_css_self);
			}

			const Array<IDOMnode*> * GetChilds()
			{
				return(&m_vChilds);
			}

			UINT GetNodeId()
			{
				return(m_iNodeId);
			}

			void AddPseudoclass(UINT id);
			bool PseudoclassExists(UINT id);
			void RemovePseudoclass(UINT id);

			void StoreStyles();
			void CaptureStyleChanges();

			CDOMdocument * GetDocument()
			{
				return(m_pDocument);
			}

			Render::IRenderFrame * GetRenderFrame()
			{
				return(m_pRenderFrame);
			}
			void SetRenderFrame(Render::IRenderFrame * prf)
			{
				m_pRenderFrame = prf;
			}
			void UpdateStyles();
			void UpdateLayout();


			void DispatchEvent(IEvent & ev);
			void DispatchClientEvent(IEvent ev, bool * preventDefault);

			static void ApplyCSSrules(const CSS::ICSSstyle * style, CDOMnode * pNode);

		protected:
			IDOMnode * m_pParent;
			IDOMnode * m_pPrevSibling;
			IDOMnode * m_pNextSibling;
			Array<IDOMnode*> m_vChilds;
			CDOMdocument * m_pDocument;
			UINT m_iNodeId;

			UINT m_iDOMid;
			Array<UINT> m_vDOMcls;

			UINT m_pseudoclasses;

			AssotiativeArray<StringW, StringW> m_mAttributes;

			CSS::CCSSstyle m_css;
			CSS::CCSSstyle m_cssOld;
			CSS::CCSSstyle m_css_self;

			Render::IRenderFrame * m_pRenderFrame;

			bool m_bToggleable;
			bool m_bToggleState;
			bool m_bToggleOnly;
			bool m_bEditable;
			bool m_bFocusable;

			bool m_bDelegateEvents;
		};


		typedef Array<IDOMnode*> IDOMnodeCollection;


		class IDOMnodeTag: public CDOMnode
		{
		public:
			DECLARE_CLASS(IDOMnodeTag, CDOMnode);

		//	CSS::ICSSstyle * GetStyle();

			/*const String & GetID();
			void SetID(const String & str);*/

		//protected:
		//	CSS::ICSSstyle * m_pStyle;

		};


		template<class T> class IDOMnodeFactory
		{
		public:

			IDOMnodeFactory(const wchar_t * name)
			{
				vTable vt;
				vt.getInstance = &(T::GetInstance);
				vt.wsNodeName = name;
				UINT nid = CDOMnode::nodes.size();
				CDOMnode::nodeIDs[vt.wsNodeName] = nid;
				CDOMnode::nodes[nid] = vt;
			};
		};
	};
};

#endif
