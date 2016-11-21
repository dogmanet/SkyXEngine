#include "GUIbase.h"

#include "CDOM.h"


namespace GUI
{
	namespace DOM
	{
		CDOMnode::~CDOMnode()
		{
			for(UINT i = 0, l = m_vChilds.size(); i < l; ++i)
			{
				RemoveChild(m_vChilds[i]);
				SX_SAFE_DELETE(m_vChilds[i]);
			}
		}



		IDOMnode * CDOMnode::ParentNode()
		{
			return(m_pParent);
		}

		AssotiativeArray<StringW, UINT> IDOMnodeTag::nodeIDs;
		Array<vTable> IDOMnodeTag::nodes;

		/*CSS::ICSSstyle * IDOMnodeTag::GetStyle()
		{
			return(m_pStyle);
		}*/

		/*const String & IDOMnodeTag::GetID()
		{
		return("");
		}

		void IDOMnodeTag::SetID(const String & str)
		{

		}*/

		const StringW & CDOMnode::GetNodeNameById(UINT nid)
		{
			/*if(nid == 0)
			{
			static StringW text(L"text");
			return(text);
			}
			else */if(IDOMnodeTag::nodes.size() > nid)
			{
				return(IDOMnodeTag::nodes[nid].wsNodeName);
			}
			else
			{
				static StringW empty(L"");
				return(empty);
			}
		}

		UINT CDOMnode::GetNodeIdByName(const StringW & tag)
		{
			if(IDOMnodeTag::nodeIDs.KeyExists(tag))
			{
				return(IDOMnodeTag::nodeIDs[tag]);
			}
			return(0);
		}

		void CDOMnode::DebugPrintMe(UINT lvl)
		{
			if(m_iNodeId == 0)
			{
				return;
			}
			for(UINT i = 0; i < lvl; i++)
			{
				wprintf(L"  ");
			}
			wprintf(L"<%s", CDOMnode::GetNodeNameById(m_iNodeId));
			for(AssotiativeArray<StringW, StringW>::Iterator i = m_mAttributes.begin(); i; i++)
			{
				wprintf(L" %s=\"%s\"", i.first->c_str(), i.second->c_str());
			}
			wprintf(L">\n");
			for(int i = 0, s = m_vChilds.size(); i < s; i++)
			{
				((CDOMnode*)m_vChilds[i])->DebugPrintMe(lvl + 1);
			}
			for(UINT i = 0; i < lvl; i++)
			{
				wprintf(L"  ");
			}
			wprintf(L"</%s>\n", CDOMnode::GetNodeNameById(m_iNodeId));
		}


		void CDOMnode::ResetStyles()
		{
			m_css.Reset();
			for(int i = 0, s = m_vChilds.size(); i < s; i++)
			{
				((CDOMnode*)m_vChilds[i])->ResetStyles();
			}
		}

		void CDOMnode::StoreStyles()
		{
			m_cssOld = m_css;
			for(int i = 0, s = m_vChilds.size(); i < s; i++)
			{
				((CDOMnode*)m_vChilds[i])->StoreStyles();
			}
		}

		void CDOMnode::CaptureStyleChanges()
		{
			m_css.CaptureChanges(&m_cssOld);
			for(int i = 0, s = m_vChilds.size(); i < s; i++)
			{
				((CDOMnode*)m_vChilds[i])->CaptureStyleChanges();
			}
		}

		void CDOMnode::ResetStyleChanges()
		{
			m_css.ResetChanges();
			for(int i = 0, s = m_vChilds.size(); i < s; i++)
			{
				((CDOMnode*)m_vChilds[i])->ResetStyleChanges();
			}
		}


		const StringW & CDOMnode::GetAttribute(const StringW & name)
		{
			if(m_mAttributes.KeyExists(name))
			{
				return(m_mAttributes[name]);
			}
			static StringW empt(L"");
			return(empt);
		}

#define APPLY_RULE(rule) if(!node->m_css.rule.IsSet()){node->m_css.rule = m_css.rule;}
		void CDOMnode::ApplyChildStyle()
		{
			CDOMnode::ApplyCSSrules(&m_css_self, this);
			UINT c = m_vChilds.size();
			for(UINT i = 0; i < c; i++)
			{
				CDOMnode * node = (CDOMnode*)m_vChilds[i];
				APPLY_RULE(p_color);
				APPLY_RULE(p_cursor);
				APPLY_RULE(p_direction);

				APPLY_RULE(p_font_name);
				APPLY_RULE(p_font_size);
				APPLY_RULE(p_font_style);
				APPLY_RULE(p_font_weight);

				APPLY_RULE(p_text_align);
				APPLY_RULE(p_text_decoration_line);
				APPLY_RULE(p_visibility);

				node->ApplyChildStyle();
			}
		}
#undef APPLY_RULE

		bool CDOMnode::IsChildOf(IDOMnode * pNode)
		{
			IDOMnode * pCur = ParentNode();
			while(pCur)
			{
				if(pCur == pNode)
				{
					return(true);
				}
				pCur = pCur->ParentNode();
			}
			return(false);
		}

		bool CDOMnode::IsAfter(IDOMnode * pNode)
		{
			IDOMnode * pCur = PrevSibling();
			while(pCur)
			{
				if(pCur == pNode)
				{
					return(true);
				}
				pCur = pCur->PrevSibling();
			}
			return(false);
		}
	};
};