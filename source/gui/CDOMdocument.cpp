#include "GUIbase.h"
#include "CDOMdocument.h"
#include "ITEXT.h"

#include "IScrollBar.h"

#define SCROLL_SPEED 32
#define SCROLL_SPEED_MAX 64

namespace GUI
{
	namespace DOM
	{
		IDOMnode * CDOMdocument::CreateNode(const wchar_t * tag)
		{
			/*if(_wcsicmp(tag, L"text") == 0)
			{
				return(new IDOMnodeText());
			}
			else
			{*/
				return(IDOMnodeTag::CreateNode(tag));
			/*}*/
		}

		IDOMnode * CDOMdocument::CreateNode(UINT nid)
		{
			/*if(nid == 0)
			{
				return(new IDOMnodeText());
			}
			else
			{*/
				return(CDOMnode::CreateNode(nid));
			/*}*/
		}

		void CDOMdocument::SetRootNode(IDOMnode * pNode)
		{
			SX_SAFE_DELETE(m_pRootNode);
			m_pRootNode = pNode;
			IndexSetNode((CDOMnode*)pNode);
		//	IndexBuild();
			LoadStyles();
		//	GetCSS()->DebugDumpStyles();
			CalculateStyles();
			BuildRenderThree();
		}

		void CDOMdocument::IndexBuild()
		{
		//	m_IndexStringById.clear();
			m_IndexById.clear();
		//	m_IndexStringByClass.clear();
			m_IndexByTagName.clear();

			//@@TODO:Invalidate CSS cache

			CDOMdocument::BuildIndexFunc(this, m_pRootNode);
		}

		void CDOMdocument::IndexAdd(IDOMnode * pNode)
		{
			IndexBuild();
		}

		void CDOMdocument::IndexRemove(IDOMnode * pNode)
		{
			IndexBuild();
		}

		UINT CDOMdocument::GetIndexForIdString(const StringW & id)
		{
			if(id.length() == 0)
			{
				return(0);
			}
			if(m_IndexStringById.KeyExists(id))
			{
				return(m_IndexStringById[id]);
			}
			else
			{
				UINT i = m_IndexStringById.Size() + 1;
				m_IndexStringById[id] = i;
				return(i);
			}
		}

		UINT CDOMdocument::GetIndexForClassString(const StringW & id)
		{
			if(id.length() == 0)
			{
				return(0);
			}
			if(m_IndexStringByClass.KeyExists(id))
			{
				return(m_IndexStringByClass[id]);
			}
			else
			{
				UINT i = m_IndexStringByClass.Size() + 1;
				m_IndexStringByClass[id] = i;
				return(i);
			}
		}

		UINT CDOMdocument::GetIndexForTagString(const StringW & id)
		{
			return(CDOMnode::GetNodeIdByName(id));
		}

		void CDOMdocument::BuildIndexFunc(IDOMdocument * _doc, IDOMnode * __node)
		{
			return;
			CDOMdocument * doc = (CDOMdocument*)_doc;
			CDOMnode * _node = (CDOMnode*)__node;
			if(_node->IsTextNode())
			{
				return;
			}
			//IDOMnodeTag * pNode = (IDOMnodeTag*)_node;

			StringW id = _node->GetAttribute(L"id");
			StringW cls = _node->GetAttribute(L"class");

			if(id.length())
			{
				UINT iid = doc->GetIndexForIdString(id);
				doc->m_IndexById[iid] = _node;
			}

			if(cls.length())
			{
				UINT pos = 0;
				while(true)
				{
					pos = cls.find(L" ");
					if(pos != (UINT)(-1))
					{
						StringW c = cls.substr(0, pos);
						UINT icls = doc->GetIndexForClassString(cls);
						doc->m_IndexByClass[icls].push_back(_node);
						cls = cls.substr(pos);
					}
					else
					{
						break;
					}
				}
			//	UINT icls = doc->GetIndexForClassString(cls);
			//	doc->m_IndexByClass[icls].push_back(_node);
			}

			UINT itid = _node->m_iNodeId;
			doc->m_IndexByTagName[itid].push_back(_node);

			for(UINT i = 0, c = _node->m_vChilds.size(); i < c; i++)
			{
				CDOMdocument::BuildIndexFunc(doc, _node->m_vChilds[i]);
			}
		}

		IDOMnode * CDOMdocument::GetElementById(const StringW & id)
		{
			if(m_IndexStringById.KeyExists(id))
			{
				return(GetElementById(m_IndexStringById[id]));
			}
			return(NULL);
		}

		IDOMnode * CDOMdocument::GetElementById(UINT iid)
		{
			if(m_IndexById.size() > iid)
			{
				return(m_IndexById[iid]);
			}
			return(NULL);
		}

		const IDOMnodeCollection * CDOMdocument::GetElementsByClass(const StringW & id)
		{
			if(m_IndexStringByClass.KeyExists(id))
			{
				return(GetElementsByClass(m_IndexStringByClass[id]));
			}
			return(NULL);
		}

		const IDOMnodeCollection * CDOMdocument::GetElementsByClass(UINT cid)
		{
			if(m_IndexByClass.size() > cid)
			{
				return(&m_IndexByClass[cid]);
			}
			return(NULL);
		}

		void CDOMnode::SetText(const StringW & text, BOOL build)
		{
			if(m_vChilds.size() && m_vChilds[0]->IsTextNode())
			{
				((IDOMnodeText*)(m_vChilds[0]))->SetText(text);
				if(build)
				{
					UpdateLayout();
				}
			}
		}

//#define GEVT_DPTC(TYPE, name) case TYPE: if(name){(name->isUnicode ? name->fnptrW(&ev, 0, NULL) : name->fnptr(&ev, 0, NULL));} break;

		void CDOMnode::DispatchClientEvent(IEvent ev, bool * preventDefault)
		{
			ev.propagate = true;
			StringW cmd;
			switch(ev.type)
			{
#define GEVT_DPTC(TYPE, name) case TYPE: cmd = GetAttribute(L"on" name); break;
				GEVT_DPTC(GUI_EVENT_TYPE_CLICK, L"click");
				GEVT_DPTC(GUI_EVENT_TYPE_MOUSEDOWN, L"mousedown");
				GEVT_DPTC(GUI_EVENT_TYPE_MOUSEUP, L"mouseup");
				GEVT_DPTC(GUI_EVENT_TYPE_MOUSEENTER, L"mouseover");
				GEVT_DPTC(GUI_EVENT_TYPE_MOUSELEAVE, L"mouseout");
				GEVT_DPTC(GUI_EVENT_TYPE_KEYDOWN, L"keydown");
				GEVT_DPTC(GUI_EVENT_TYPE_KEYUP, L"keyup");
				GEVT_DPTC(GUI_EVENT_TYPE_KEYPRESS, L"keypress");
				//GEVT_DPTC(GUI_EVENT_TYPE_SCROLL, onScroll);
				GEVT_DPTC(GUI_EVENT_TYPE_MOUSEMOVE, L"mousemove");
				GEVT_DPTC(GUI_EVENT_TYPE_MOUSEWHEELUP, L"mousewheelup");
				GEVT_DPTC(GUI_EVENT_TYPE_MOUSEWHEELDOWN, L"mousewheeldown");
				GEVT_DPTC(GUI_EVENT_TYPE_FOCUS, L"focus");
				GEVT_DPTC(GUI_EVENT_TYPE_BLUR, L"blur");
			}
			if(cmd.length())
			{
				GetGUI()->ExecCallback(cmd, &ev);
			}
#undef GEVT_DPTC

			if(preventDefault)
			{
				*preventDefault = ev.preventDefault;
			}

			if(ev.propagate && m_pParent)
			{
				ev.currentTarget = m_pParent;
				m_pParent->DispatchClientEvent(ev, NULL);
			}
		}

		//void CDOMdocument::ExecCallback(const StringW cmd, IEvent * ev)
		//{
			//GetGUI()->
		//}

		void CDOMnode::UpdateStyles()
		{
			m_pDocument->UpdateStyleSubtree(this);
		}

		void CDOMnode::DispatchEvent(IEvent & ev)
		{
			if(ev.target == this)
			{
				DispatchClientEvent(ev, &ev.preventDefault);
			}
			if(ev.preventDefault)
			{
				return;
			}
			if(this->m_pRenderFrame->m_pScrollBarVert)
			{
				this->m_pRenderFrame->m_pScrollBarVert->DispatchEvent(ev);
			}
			switch(ev.type)
			{
			case GUI_EVENT_TYPE_MOUSEDOWN:
				m_pDocument->RequestFocus(this);
				if(ev.key == KEY_LBUTTON)
				{
					if(m_bToggleable)
					{
						if(m_bToggleState && !m_bToggleOnly)
						{
							RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
							m_bToggleState = false;
						}
						else
						{
							AddPseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
							m_bToggleState = true;
							if(m_bToggleOnly && m_vDOMcls.size())
							{
								const IDOMnodeCollection * deactnode = GetDocument()->GetElementsByClass(m_vDOMcls[0]);
								for(UINT i = 0; i < deactnode->size(); i++)
								{
									IDOMnode * cn = (*deactnode)[i];
									if(cn != this && ((CDOMnode*)cn)->m_bToggleState)
									{
										cn->RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
										((CDOMnode*)cn)->m_bToggleState = false;
									}
								}
							}
						}

					}
					else
					{
						AddPseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
					}

					if(m_vChilds.size() && m_vChilds[0]->IsTextNode())
					{
						((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->PlaceCaretByXY(ev.offsetX - GetRenderFrame()->GetInnerLeft() + GetRenderFrame()->GetScrollLeft(), ev.offsetY - GetRenderFrame()->GetInnerTop() + GetRenderFrame()->GetScrollTop());
						((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->SelectionStart();
					}
				}
				ev.StopPropagation();
				break;
			case GUI_EVENT_TYPE_MOUSEUP:
				if(ev.key == KEY_LBUTTON)
				{
					
					if(!m_bToggleable)
					{
						RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
					}
					if(m_vChilds.size() && m_vChilds[0]->IsTextNode())
					{
						((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->SelectionEnd();
					}
				}
				break;
			case GUI_EVENT_TYPE_CLICK:
				//wprintf(L"Clicked\n");
				break;

			case GUI_EVENT_TYPE_FOCUS:
				AddPseudoclass(CSS::ICSSrule::PSEUDOCLASS_FOCUS);
				ev.StopPropagation();
				break;

			case GUI_EVENT_TYPE_BLUR:
				RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_FOCUS);
				ev.StopPropagation();
				break;

			case GUI_EVENT_TYPE_MOUSEWHEELUP:
				if(m_pRenderFrame->m_iScrollTopMax != 0)
				{
					if(m_pRenderFrame->m_iScrollTop != 0)
					{
						ev.StopPropagation();
					}
					//m_pRenderFrame->m_iScrollTop -= 32;
							
					if(m_pRenderFrame->m_iScrollSpeedY > 0)
					{
						m_pRenderFrame->m_iScrollSpeedY = -SCROLL_SPEED;
					}
					else
					{
						m_pRenderFrame->m_iScrollSpeedY -= SCROLL_SPEED;
						if(m_pRenderFrame->m_iScrollSpeedY < -SCROLL_SPEED_MAX)
						{
							m_pRenderFrame->m_iScrollSpeedY = -SCROLL_SPEED_MAX;
						}
					}
				}
				break;
			case GUI_EVENT_TYPE_MOUSEWHEELDOWN:
				if(m_pRenderFrame->m_iScrollTopMax != 0)
				{
					if(m_pRenderFrame->m_iScrollTopMax != m_pRenderFrame->m_iScrollTop)
					{
						ev.StopPropagation();
					}
					//m_pRenderFrame->m_iScrollTop += 32;
					if(m_pRenderFrame->m_iScrollSpeedY < 0)
					{
						m_pRenderFrame->m_iScrollSpeedY = SCROLL_SPEED;
					}
					else
					{
						m_pRenderFrame->m_iScrollSpeedY += SCROLL_SPEED;
						if(m_pRenderFrame->m_iScrollSpeedY > SCROLL_SPEED_MAX)
						{
							m_pRenderFrame->m_iScrollSpeedY = SCROLL_SPEED_MAX;
						}
					}
					
				}
				break;

			case GUI_EVENT_TYPE_KEYPRESS:
				//((Render::IRenderTextNew*)(m_pRenderFrame->GetChild(0)))->
				/*if(!m_vChilds.size())
				{
					IDOMnode * pTextNode = IDOMnode::CreateNode(L"text");
					pTextNode->SetDocument(m_pDocument);
					pTextNode->
				}*/
				if(ev.key == KEY_SPACE || ev.key == KEY_ENTER)
				{
					if(m_bToggleable)
					{
						if(m_bToggleState && !m_bToggleOnly)
						{
							RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
							m_bToggleState = false;
						}
						else
						{
							AddPseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
							m_bToggleState = true;
							if(m_bToggleOnly && m_vDOMcls.size())
							{
								const IDOMnodeCollection * deactnode = GetDocument()->GetElementsByClass(m_vDOMcls[0]);
								for(UINT i = 0; i < deactnode->size(); i++)
								{
									CDOMnode * cn = (CDOMnode*)((*deactnode)[i]);
									if(cn != this && cn->m_bToggleState)
									{
										cn->RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
										cn->m_bToggleState = false;
									}
								}
							}
						}
					}
					else
					{

					}
				}
				if(m_bEditable && m_vChilds.size() && m_vChilds[0]->IsTextNode())
				{
					if(ev.syskey)
					{
						switch(ev.key)
						{
						case KEY_LEFT:
							((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->MoveCaretPos(-1);
							break;
						case KEY_RIGHT:
							((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->MoveCaretPos(1);
							break;

						case KEY_UP:
							((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->MoveCaretLine(-1);
							break;
						case KEY_DOWN:
							((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->MoveCaretLine(1);
							break;

						case KEY_HOME:
							((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->SetCaretPos(0);
							break;

						case KEY_END:
							((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->SetCaretPos(((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->GetCaretMaxPos());
							break;


						case KEY_DELETE:
							if(((IDOMnodeText*)(m_vChilds[0]))->HasSelection())
							{
								((IDOMnodeText*)(m_vChilds[0]))->DeleteSelection();
							}
							else
							{
								((IDOMnodeText*)(m_vChilds[0]))->DeleteChar(ev.key == KEY_BACKSPACE);
							}

							GetDocument()->AddReflowItem(((CDOMnode*)m_vChilds[0])->GetRenderFrame());

							break;
						
						}
					}
					else
					{
						if(KeyMap::KeyState(KEY_CTRL))
						{
							switch(ev.key + 'A' - 1)
							{
							case 'V': //Ctrl + V
								if(KeyMap::KeyState(KEY_CTRL))
								{
									((IDOMnodeText*)(m_vChilds[0]))->DeleteSelection();
									((IDOMnodeText*)(m_vChilds[0]))->FromClipboard();
								}
								break;

							case 'C': //Ctrl + C
								if(KeyMap::KeyState(KEY_CTRL))
								{
									((IDOMnodeText*)(m_vChilds[0]))->ToClipboard(false);
								}
								break;

							case 'X': //Ctrl + X
								if(KeyMap::KeyState(KEY_CTRL))
								{
									((IDOMnodeText*)(m_vChilds[0]))->ToClipboard(true);
								}
								break;

							case 'A': //Ctrl + A
								if(KeyMap::KeyState(KEY_CTRL))
								{
									((IDOMnodeText*)(m_vChilds[0]))->SelectAll();
								}
								break;
							}
						}
						else
						{
							switch(ev.key)
							{
							case KEY_BACKSPACE:
								if(((IDOMnodeText*)(m_vChilds[0]))->HasSelection())
								{
									((IDOMnodeText*)(m_vChilds[0]))->DeleteSelection();
								}
								else
								{
									((IDOMnodeText*)(m_vChilds[0]))->DeleteChar(ev.key == KEY_BACKSPACE);
								}
								break;
							default:
								((IDOMnodeText*)(m_vChilds[0]))->DeleteSelection();
								((IDOMnodeText*)(m_vChilds[0]))->InsertChar(ev.key);
							}
						}
						
					//	((IDOMnodeText*)(m_vChilds[0]))->SetText(((IDOMnodeText*)(m_vChilds[0]))->GetText() + (WCHAR)ev.key);
						((IDOMnodeText*)(m_vChilds[0]))->UpdateLayout();
					}
					//m_pRenderFrame->Layout();
				}
				{
					static UINT tagButton = GetNodeIdByName(L"button");
					if(GetNodeId() == tagButton)
					{
						if(ev.syskey)
						{
							CDOMnode * pEl = NULL;
							switch(ev.key)
							{
							case KEY_LEFT:
								pEl = (CDOMnode*)this->PrevSibling();
								while(pEl)
								{
									if(pEl->GetNodeId() == tagButton)
									{
										GetDocument()->RequestFocus(pEl);
										break;
									}
									pEl = (CDOMnode*)pEl->PrevSibling();
								}
								break;
							case KEY_RIGHT:
								pEl = (CDOMnode*)this->NextSibling();
								while(pEl)
								{
									if(pEl->GetNodeId() == tagButton)
									{
										GetDocument()->RequestFocus(pEl);
										break;
									}
									pEl = (CDOMnode*)pEl->NextSibling();
								}
								break;
							}
						}
						if(ev.key == KEY_ENTER)
						{
							IEvent ev;
							ev.clientX = ev.clientY = 0;
							ev.type = GUI_EVENT_TYPE_CLICK;
							ev.key = KEY_ENTER;
							ev.target = this;
							DispatchEvent(ev);
						}
					}
				}
				break;

			case GUI_EVENT_TYPE_MOUSEENTER:
				AddPseudoclass(CSS::ICSSrule::PSEUDOCLASS_HOVER);

				//if(m_css.p_cursor.IsSet())
				{
					/*switch(m_css.p_cursor.GetInt())
					{
					case CSS::ICSSproperty::CURSOR_AUTO:
					case CSS::ICSSproperty::CURSOR_DEFAULT:
						SetCursor(LoadCursor(NULL, IDC_ARROW));
						break;
					case CSS::ICSSproperty::CURSOR_POINTER:
						SetCursor(LoadCursor(NULL, IDC_HAND));
						break;

					case CSS::ICSSproperty::CURSOR_TEXT:
						SetCursor(LoadCursor(NULL, IDC_WAIT));
						break;
					}*/
				}
				
				ev.StopPropagation();
				break;

			case GUI_EVENT_TYPE_MOUSELEAVE:
				RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_HOVER);
				ev.StopPropagation();
				break;

			case GUI_EVENT_TYPE_MOUSEMOVE:
				if(m_vChilds.size() && m_vChilds[0]->IsTextNode())
				{
					if(m_pDocument->GetFocus() == this && KeyMap::KeyState(KEY_LBUTTON))
					{
						((Render::IRenderTextNew*)(((CDOMnode*)m_vChilds[0])->GetRenderFrame()))->PlaceCaretByXY(ev.offsetX - GetRenderFrame()->GetInnerLeft() + GetRenderFrame()->GetScrollLeft(), ev.offsetY - GetRenderFrame()->GetInnerTop() + GetRenderFrame()->GetScrollTop());
					}
				}
				break;
			}
					
					
			if(m_bDelegateEvents)
			{
				StringW to = GetAttribute(L"for");
				if(to.length())
				{
					IDOMnode * pTO = m_pDocument->GetElementById(to);
					if(pTO)
					{
						pTO->DispatchEvent(ev);
					}
				}
			}
			if(ev.propagate && m_pParent)
			{
				m_pParent->DispatchEvent(ev);
			}
		}

		

		void CDOMdocument::SetDesktop(IDesktop * pdp)
		{
			m_pDesktop = (CDesktop*)pdp;
		}

		void CDOMnode::UpdateLayout()
		{
			GetDocument()->AddReflowItem(GetRenderFrame());
		}


		const IDOMnodeCollection * CDOMdocument::GetElementsByPseudoClass(UINT _cid)
		{
			UINT cid = _log2(_cid);
			if(m_IndexByPseudoClass.size() > cid)
			{
				return(&m_IndexByPseudoClass[cid]);
			}
			return(NULL);
		}

		const IDOMnodeCollection * CDOMdocument::GetElementsByTag(const StringW & id)
		{
			UINT _nid = CDOMnode::GetNodeIdByName(id);
			return(GetElementsByTag(_nid));
		}

		const IDOMnodeCollection * CDOMdocument::GetElementsByTag(UINT tid)
		{
			if(m_IndexByTagName.size() > tid)
			{
				return(&m_IndexByTagName[tid]);
			}
			return(NULL);
		}

		CSS::ICSS * CDOMdocument::GetCSS()
		{
			return(m_pCSS);
		}

		void CDOMdocument::LoadStyles()
		{
			const IDOMnodeCollection * nodes = GetElementsByTag(CDOMnode::GetNodeIdByName(L"link"));
			if(nodes)
			{
				UINT c = nodes->size();
				for(UINT i = 0; i < c; i++)
				{
					IDOMnode * node = (*nodes)[i];
					if(node->GetAttribute(L"rel") == L"stylesheet")
					{
						m_pCSS->AddFile(node->GetAttribute(L"href").c_str());
					}
				}
			}
		}

		void CDOMdocument::CalculateStyles()
		{
			UpdateStyleSubtree(m_pRootNode);
			UpdateStyles();
		}
#define APPLY_RULE(rule) if(style->rule.IsSet()){pNode->m_css.rule = style->rule;}
		void CDOMnode::ApplyCSSrules(const CSS::ICSSstyle * _style, CDOMnode * pNode)
		{
			const CSS::CCSSstyle * style = (const CSS::CCSSstyle *)_style;
			APPLY_RULE(p_display);
			APPLY_RULE(p_color);
			APPLY_RULE(p_cursor);

			APPLY_RULE(p_direction);

			APPLY_RULE(p_opacity);

			APPLY_RULE(p_z_index);

			APPLY_RULE(p_overflow_x);
			APPLY_RULE(p_overflow_y);

			APPLY_RULE(p_margin_left);
			APPLY_RULE(p_margin_right);
			APPLY_RULE(p_margin_top);
			APPLY_RULE(p_margin_bottom);

			APPLY_RULE(p_padding_left);
			APPLY_RULE(p_padding_right);
			APPLY_RULE(p_padding_top);
			APPLY_RULE(p_padding_bottom);

			APPLY_RULE(p_top);
			APPLY_RULE(p_left);
			APPLY_RULE(p_right);
			APPLY_RULE(p_bottom);

			APPLY_RULE(p_width);
			APPLY_RULE(p_height);

			APPLY_RULE(p_max_width);
			APPLY_RULE(p_max_height);

			APPLY_RULE(p_min_width);
			APPLY_RULE(p_min_height);

			APPLY_RULE(p_position);

			APPLY_RULE(p_visibility);

			APPLY_RULE(p_float);

			APPLY_RULE(p_background_image);
			APPLY_RULE(p_background_color);
			APPLY_RULE(p_background_position_x);
			APPLY_RULE(p_background_position_y);
			APPLY_RULE(p_background_attachment);
			APPLY_RULE(p_background_repeat);
			APPLY_RULE(p_background_size_x);
			APPLY_RULE(p_background_size_y);


			APPLY_RULE(p_border_top_style); //
			APPLY_RULE(p_border_top_width);
			APPLY_RULE(p_border_top_color);
			APPLY_RULE(p_border_top_left_radius);
			APPLY_RULE(p_border_top_right_radius);

			APPLY_RULE(p_border_right_style); //
			APPLY_RULE(p_border_right_width);
			APPLY_RULE(p_border_right_color);

			APPLY_RULE(p_border_bottom_style); //
			APPLY_RULE(p_border_bottom_width);
			APPLY_RULE(p_border_bottom_color);
			APPLY_RULE(p_border_bottom_left_radius);
			APPLY_RULE(p_border_bottom_right_radius);

			APPLY_RULE(p_border_top_left_method);
			APPLY_RULE(p_border_top_right_method);
			APPLY_RULE(p_border_bottom_left_method);
			APPLY_RULE(p_border_bottom_right_method);

			APPLY_RULE(p_border_left_style); //
			APPLY_RULE(p_border_left_width);
			APPLY_RULE(p_border_left_color);


			APPLY_RULE(p_font_name);
			APPLY_RULE(p_font_size);
			APPLY_RULE(p_font_style);
			APPLY_RULE(p_font_weight);

			APPLY_RULE(p_text_align);
			APPLY_RULE(p_text_decoration_line);

			APPLY_RULE(p__gui_text_appearance);
			APPLY_RULE(p__gui_text_cursor);
			APPLY_RULE(p__gui_text_format);
		}
#undef APPLY_RULE

		void CDOMdocument::IndexUnsetId(UINT id)
		{
			m_IndexById[id] = NULL;
		}

		void CDOMdocument::IndexSetId(UINT id, IDOMnode * pNode)
		{
			m_IndexById[id] = pNode;
		}

		void CDOMdocument::IndexUnsetClass(UINT id, IDOMnode * pNode)
		{
			IDOMnodeCollection * els = &m_IndexByClass[id];
			for(UINT i = 0; i < els->size(); i++)
			{
				if((*els)[i] == pNode)
				{
					els->erase(i);
					break;
				}
			}
		}

		void CDOMdocument::IndexSetClass(UINT id, IDOMnode * pNode)
		{
			m_IndexByClass[id].push_back(pNode);
		}

		void CDOMdocument::IndexUnsetPseudoClass(UINT _id, IDOMnode * pNode)
		{
			UINT id = _log2(_id);
			IDOMnodeCollection * els = &m_IndexByPseudoClass[id];
			for(UINT i = 0; i < els->size(); i++)
			{
				if((*els)[i] == pNode)
				{
					els->erase(i);
					break;
				}
			}
		}

		void CDOMdocument::IndexSetPseudoClass(UINT _id, IDOMnode * pNode)
		{
			UINT id = _log2(_id);
			m_IndexByPseudoClass[id].push_back(pNode);
		}


		void CDOMdocument::IndexUnsetNode(CDOMnode * pNode)
		{
			IDOMnodeCollection * els = &m_IndexByTagName[pNode->m_iNodeId];
			for(UINT i = 0; i < els->size(); i++)
			{
				if((*els)[i] == pNode)
				{
					els->erase(i);
					break;
				}
			}
		}

		void CDOMdocument::IndexSetNode(CDOMnode * pNode)
		{
			m_IndexByTagName[pNode->m_iNodeId].push_back(pNode);
		}

		void CDOMnode::SetAttribute(const StringW & name, const StringW & value)
		{
			if(name == L"id")
			{
				m_pDocument->IndexUnsetId(m_iDOMid);
				m_iDOMid = m_pDocument->GetIndexForIdString(value);
				m_pDocument->IndexSetId(m_iDOMid, this);
			}

			else if(name == L"style")
			{
				UINT n;
				m_css_self.LoadStyleFromString(value.c_str(), &n);
			}


			else if(name == L"class")
			{
				StringW cls = value+L" ";
				UINT pos = 0;
				Array<UINT> vNewCls;
				Array<UINT> vAddCls;
				Array<UINT> vRemoveCls;
				while(true)
				{
					pos = cls.find(L" ");
					if(pos != ~0)
					{
						if(pos != 0)
						{
							StringW c = cls.substr(0, pos);
							UINT icls = m_pDocument->GetIndexForClassString(c);
							vNewCls.push_back(icls);
							bool bFound = false;
							for(UINT i = 0; i < m_vDOMcls.size(); i++)
							{
								if(m_vDOMcls[i] == icls)
								{
									bFound = true;
									break;
								}
							}
							if(!bFound)
							{
								vAddCls.push_back(icls);
							}
						}
						//doc->m_IndexByClass[icls].push_back(_node);
						cls = cls.substr(pos + 1);
					}
					else
					{
						break;
					}
				}
				for(UINT j = 0; j < m_vDOMcls.size(); j++)
				{
					UINT icls = m_vDOMcls[j];
					bool bFound = false;
					for(UINT i = 0; i < vNewCls.size(); i++)
					{
						if(vNewCls[i] == icls)
						{
							bFound = true;
							break;
						}
					}
					if(!bFound)
					{
						vRemoveCls.push_back(icls);
					}
				}
				m_vDOMcls = vNewCls;
				for(UINT i = 0; i < vRemoveCls.size(); i++)
				{
					m_pDocument->IndexUnsetClass(vRemoveCls[i], this);
				}
				for(UINT i = 0; i < vAddCls.size(); i++)
				{
					m_pDocument->IndexSetClass(vAddCls[i], this);
				}
				//m_pDocument->IndexUnsetClass(m_iDOMid);
				//m_iDOMid = m_pDocument->GetIndexForIdString(value);
				//m_pDocument->IndexSetId(m_iDOMid, this);
			}

			m_mAttributes[name] = value;
		}



		void CDOMnode::AppendChild(IDOMnode * _pEl, bool regen)
		{
			CDOMnode * pEl = (CDOMnode*)_pEl;
			if(!pEl)
			{
				return;
			}
			if(m_vChilds.size() > 0)
			{
				CDOMnode * last = (CDOMnode*)m_vChilds[m_vChilds.size() - 1];
				last->m_pNextSibling = pEl;
				pEl->m_pPrevSibling = last;
			}
			m_vChilds.push_back(pEl);
			pEl->m_pParent = this;
			if(m_pDocument)
			{
				m_pDocument->IndexSetNode(pEl);
			}
			if(regen && GetRenderFrame())
			{
				GetDocument()->UpdateStyleSubtree(pEl);
				GetDocument()->UpdateStyles();
				Render::IRenderFrame * pNewRF;
				if(pEl->IsTextNode())
				{
					pNewRF = Render::IRenderFrame::CreateNode(NULL, GetRenderFrame()->m_pRootNode);
					pNewRF->AddChild(Render::IRenderFrame::CreateNode(pEl, GetRenderFrame()->m_pRootNode));
				}
				else
				{
					pNewRF = Render::IRenderFrame::CreateNode(pEl, GetRenderFrame()->m_pRootNode);
				}
				if(pNewRF)
				{
					GetRenderFrame()->AddChild(pNewRF);
					GetDocument()->AddReflowItem(pNewRF);
				}
			}
		}

		void CDOMnode::ClassAdd(const StringW & cls)
		{
			this->SetAttribute(L"class", this->GetAttribute(L"class")+L" "+cls);

		}
		/*void CDOMnode::ClassAdd(UINT cls)
		{

		}*/

		void CDOMnode::ClassRemove(const StringW & cls)
		{
			//@TODO: Implement me
		}
		/*void CDOMnode::ClassRemove(UINT cls)
		{

		}*/

		void CDOMnode::ClassToggle(const StringW & cls, int set)
		{
			//@TODO: Implement me
		}
		/*void CDOMnode::ClassToggle(UINT cls, int set = -1)
		{

		}*/

		BOOL CDOMnode::ClassExists(const StringW & cls)
		{
			//@TODO: Implement me
			return(FALSE);
		}
		/*BOOL CDOMnode::ClassExists(UINT cls)
		{

		}*/

		void CDOMnode::RemoveChild(IDOMnode * _pEl, bool regen)
		{
			CDOMnode * pEl = (CDOMnode*)_pEl;
			for(UINT i = 0, l = m_vChilds.size(); i < l; ++i)
			{
				if(m_vChilds[i] == pEl)
				{
					if(GetDocument()->GetFocus() == pEl)
					{
						GetDocument()->RequestFocus(GetDocument()->GetElementsByTag(L"body")[0][0]);
					}
					m_pDocument->IndexUnsetNode(pEl);
					while(pEl->m_vChilds.size() > 0)
					{
						pEl->RemoveChild(pEl->m_vChilds[0], false);
					}
					if(i > 0)
					{
						((CDOMnode*)m_vChilds[i - 1])->m_pNextSibling = ((CDOMnode*)m_vChilds[i])->m_pNextSibling;
					}
					if(i + 1 < m_vChilds.size())
					{
						((CDOMnode*)m_vChilds[i + 1])->m_pPrevSibling = ((CDOMnode*)m_vChilds[i])->m_pPrevSibling;
					}
					if(pEl->GetRenderFrame() && regen)
					{
						pEl->GetRenderFrame()->GetParent()->RemoveChild(pEl->GetRenderFrame());
					}
					SX_SAFE_DELETE(m_vChilds[i]);
					m_vChilds.erase(i);
					break;
				}
			}
		}


		void CDOMnode::AddPseudoclass(UINT id)
		{
			if(!(m_pseudoclasses & id))
			{
				m_pseudoclasses |= id;
				m_pDocument->IndexSetPseudoClass(id, this);
				m_pDocument->UpdateStyleSubtree(this);
			}
		}
		void CDOMnode::RemovePseudoclass(UINT id)
		{
			if(m_pseudoclasses & id)
			{
				m_pseudoclasses &= ~id;
				m_pDocument->IndexUnsetPseudoClass(id, this);
				m_pDocument->UpdateStyleSubtree(this);
			}
		}

		bool CDOMnode::PseudoclassExists(UINT id)
		{
			return((m_pseudoclasses & id) != 0);
		}

		IDOMnodeCollection CDOMdocument::QuerySelectorSimple(const CSS::ICSSrule::ICSSselectorItem * sel)
		{
			IDOMnodeCollection current;
			if(sel->dom_id > 0)
			{
				current.push_back(GetElementById(sel->dom_id));
			}
			if(sel->dom_class.size())
			{
				for(UINT k = 0; k < sel->dom_class.size(); k++)
				{
					UINT cid = sel->dom_class[k];
					const IDOMnodeCollection * c = GetElementsByClass(cid);
					if(c)
					{
						for(UINT l = 0; l < c->size(); l++)
						{
							IDOMnode * n = (*c)[l];
							bool bFound = false;
							for(UINT m = 0; m < current.size(); m++)
							{
								if(current[m] == n)
								{
									bFound = true;
									break;
								}
							}
							if(!bFound)
							{
								current.push_back(n);
							}
						}
					}
				}
			}
			if(sel->dom_tag > 0)
			{
				const IDOMnodeCollection * c = GetElementsByTag(sel->dom_tag);
				if(c)
				{
					for(UINT l = 0; l < c->size(); l++)
					{
						IDOMnode * n = (*c)[l];
						bool bFound = false;
						for(UINT m = 0; m < current.size(); m++)
						{
							if(current[m] == n)
							{
								bFound = true;
								break;
							}
						}
						if(!bFound)
						{
							current.push_back(n);
						}
					}
				}
			}

			if(sel->pseudoclass != 0)
			{
				for(UINT k = 0; k < CSS::ICSSrule::PSEUDOCLASS_COUNT; k++)
				{
					UINT cid = 1 << k;
					if(sel->pseudoclass & cid)
					{
						const IDOMnodeCollection * c = GetElementsByPseudoClass(cid);
						if(c)
						{
							for(UINT l = 0; l < c->size(); l++)
							{
								IDOMnode * n = (*c)[l];
								bool bFound = false;
								for(UINT m = 0; m < current.size(); m++)
								{
									if(current[m] == n)
									{
										bFound = true;
										break;
									}
								}
								if(!bFound)
								{
									current.push_back(n);
								}
							}
						}
					}
				}
			}

			//@@TODO: make selection by pseudoclass

			if(sel->dom_id > 0)
			{
				for(UINT k = 0; k < current.size(); k++)
				{
					if(!current[k] || ((CDOMnode*)current[k])->m_iDOMid != sel->dom_id)
					{
						current.erase(k);
						k--;
					}
				}
			}

			if(sel->dom_class.size())
			{
				for(UINT k = 0; k < current.size(); k++)
				{
					UINT matches = 0;
					for(UINT l = 0; l < sel->dom_class.size(); l++)
					{
						UINT cid = sel->dom_class[l];
						for(UINT m = 0; m < ((CDOMnode*)current[k])->m_vDOMcls.size(); m++)
						{
							if(((CDOMnode*)current[k])->m_vDOMcls[m] == cid)
							{
								matches++;
								break;
							}
						}
					}
					if(matches != sel->dom_class.size())
					{
						current.erase(k);
						k--;
					}
				}
			}

			if(sel->pseudoclass != 0)
			{
				for(UINT k = 0; k < current.size(); k++)
				{
					if((((CDOMnode*)current[k])->m_pseudoclasses & sel->pseudoclass) != sel->pseudoclass)
					{
						current.erase(k);
						k--;
					}
				}
			}


			if(sel->dom_tag > 0)
			{
				for(UINT k = 0; k < current.size(); k++)
				{
					if(((CDOMnode*)current[k])->m_iNodeId != sel->dom_tag)
					{
						current.erase(k);
						k--;
					}
				}
			}
			return(current);
		}

		IDOMnodeCollection CDOMdocument::QuerySelectorAll(const CSS::ICSSRuleSet * rules)
		{
			IDOMnodeCollection result;

			for(UINT i = 0; i < rules->size(); i++)
			{
				const CSS::ICSSrule * rule = &(*rules)[i];

				IDOMnodeCollection current, prev;
				CSS::ICSSrule::CONNECTOR connector = CSS::ICSSrule::CONNECTOR_NONE;
				for(UINT j = 0; j < rule->m_selectors.size(); j++)
				{
					const CSS::ICSSrule::ICSSselectorItem * sel = &rule->m_selectors[j];
					if(connector == CSS::ICSSrule::CONNECTOR_NONE)
					{
						current = QuerySelectorSimple(sel);
					}
					else
					{
						if(!prev.size())
						{
							return(prev);
						}
						current = QuerySelectorSimple(sel);
						for(UINT k = 0; k < current.size(); k++)
						{
							bool bFound = false;
							switch(connector)
							{
							case CSS::ICSSrule::CONNECTOR_SIMPLE:
								for(UINT l = 0; l < prev.size(); l++)
								{
									if(current[k]->IsChildOf(prev[l]))
									{
										bFound = true;
										break;
									}
								}
								if(!bFound)
								{
									current.erase(k);
									k--;
								}
								break;
							case CSS::ICSSrule::CONNECTOR_AFTER:
								for(UINT l = 0; l < prev.size(); l++)
								{
									if(current[k]->IsAfter(prev[l]))
									{
										bFound = true;
										break;
									}
								}
								if(!bFound)
								{
									current.erase(k);
									k--;
								}
								break;
								break;
							case CSS::ICSSrule::CONNECTOR_CHILD:
								for(UINT l = 0; l < prev.size(); l++)
								{
									if(current[k]->ParentNode() == prev[l])
									{
										bFound = true;
										break;
									}
								}
								if(!bFound)
								{
									current.erase(k);
									k--;
								}
								break;
							case CSS::ICSSrule::CONNECTOR_NEXT:
								for(UINT l = 0; l < prev.size(); l++)
								{
									if(current[k]->PrevSibling() == prev[l])
									{
										bFound = true;
										break;
									}
								}
								if(!bFound)
								{
									current.erase(k);
									k--;
								}
								break;
							}
						}
					}
					connector = sel->connector;
					prev = current;
				}
				for(UINT k = 0; k < current.size(); k++)
				{
					result.push_back(current[k]);
				}
			}

			return(result);
		}

		void CDOMdocument::UpdateStyleSubtree(IDOMnode * pStartNode)
		{
			bool f = false;
			for(int i = 0, l = m_UpdateStyleQueue.size(); i < l; ++i)
			{
				if(m_UpdateStyleQueue[i] == pStartNode || pStartNode->IsChildOf(m_UpdateStyleQueue[i]))
				{
					return;
				}
				if(m_UpdateStyleQueue[i]->IsChildOf(pStartNode))
				{
					if(!f)
					{
						f = true;
						m_UpdateStyleQueue[i] = pStartNode;
					}
					else
					{
						m_UpdateStyleQueue.erase(i);
						--i;
					}
				}
			}
			if(!f)
			{
				m_UpdateStyleQueue.push_back(pStartNode);
			}
			return;
		}
		void CDOMdocument::UpdateStyles()
		{
			if(!m_pRootNode || m_UpdateStyleQueue.size() == 0)
			{
				return;
			}
			bool IsIncremental = m_UpdateStyleQueue[0] != m_pRootNode; // incremental update
			for(int i = 0, l = m_UpdateStyleQueue.size(); i < l; ++i)
			{
				if(IsIncremental)
				{
					((CDOMnode*)m_UpdateStyleQueue[i])->StoreStyles();
				}
				((CDOMnode*)m_UpdateStyleQueue[i])->ResetStyles();
			}
					
			UINT icount = m_pCSS->m_styleOrder.size();
			CSS::ICSSstyleSet * css;
			for(UINT i = 0; i < icount; i++)
			{
				css = m_pCSS->m_styleOrder[i];
				UINT iRuleCount = css->m_pRules.size();
				CSS::ICSSstyle * pStyle;
				for(UINT j = 0; j < iRuleCount; j++)
				{
					pStyle = &css->m_pRules[j];
					IDOMnodeCollection els = QuerySelectorAll(&((CSS::CCSSstyle*)pStyle)->m_pRules);
					UINT iNodeCount = els.size();
					for(UINT k = 0; k < iNodeCount; k++)
					{
						bool cf = false;
						for(int ii = 0, l = m_UpdateStyleQueue.size(); ii < l && !cf; ++ii)
						{
							if(!IsIncremental || els[k] == m_UpdateStyleQueue[ii] || els[k]->IsChildOf(m_UpdateStyleQueue[ii]))
							{
								CDOMnode::ApplyCSSrules(pStyle, (CDOMnode*)els[k]);
								cf = true;
							}
						}
					}
				}
			}
			for(int i = 0, l = m_UpdateStyleQueue.size(); i < l; ++i)
			{
				if(m_UpdateStyleQueue[i]->ParentNode())
				{
					((CDOMnode*)m_UpdateStyleQueue[i]->ParentNode())->ApplyChildStyle();
				}
				else
				{
					((CDOMnode*)m_UpdateStyleQueue[i])->ApplyChildStyle();
				}
			}
					
					
			if(IsIncremental) // incremental update
			{
				if(m_pRTroot)
				{
					for(int i = 0, l = m_UpdateStyleQueue.size(); i < l; ++i)
					{
						((CDOMnode*)m_UpdateStyleQueue[i])->CaptureStyleChanges();
					}
					m_pRTroot->UpdateStyles();

//					m_pRTroot->DebugPrint();
				}
			}
			((CDOMnode*)m_pRootNode)->ResetStyleChanges();
			m_UpdateStyleQueue.clear();
		}
	
		void CDOMdocument::BuildRenderThree()
		{
			SX_SAFE_DELETE(m_pRTroot);
			m_pRTroot = Render::IRenderFrame::CreateNode((CDOMnode*)m_pRootNode, NULL);
			m_pRTroot->SetWidth(GetGUI()->GetScreenWidth());
			m_pRTroot->SetHeight(GetGUI()->GetScreenHeight());
			m_pRTroot->Layout();
			//m_pRTroot->DebugPrint();
		}

		void CDOMdocument::Render()
		{
			UpdateStyles();
			Reflow();
			ITextureManager::BindTexture(NULL);
			m_pRTroot->Render(0);
		}

		IDOMnode * CDOMdocument::GetElementByXY(int x, int y, bool sendEnterLeave)
		{
			if(!m_pRTroot)
			{
				return(NULL);
			}
			IDOMnode * node = NULL;
			m_pRTroot->FindElementByXY(x, y, &node, sendEnterLeave);
			return(node);
		}

		void CDOMdocument::AddReflowItem(Render::IRenderFrame * rf)
		{
			//Render::IRenderFrame * pLayoutBlock = rf->GetParent();
			Render::IRenderFrame * pLayoutBlock = rf;
			while(pLayoutBlock && pLayoutBlock->GetParent() && !pLayoutBlock->HasFixedSize() && !pLayoutBlock->IsOutOfFlow() && !(pLayoutBlock->GetParent()->HasFixedSize() && pLayoutBlock->IsLastChild() && !pLayoutBlock->GetNode()->IsTextNode()))
			{
				pLayoutBlock = pLayoutBlock->GetParent();
			}
			if(!pLayoutBlock)
			{
				pLayoutBlock = ((CDOMnode*)m_pRootNode)->GetRenderFrame();
			}

			for(UINT i = 0; i < m_ReflowQueue.size(); i++)
			{
				if(pLayoutBlock == m_ReflowQueue[i])
				{
					return;
				}
				if(pLayoutBlock->IsChildOf(m_ReflowQueue[i]))
				{
					return;
				}
				if(m_ReflowQueue[i]->IsChildOf(pLayoutBlock))
				{
					m_ReflowQueue[i] = pLayoutBlock;
					return;
				}
			}

			m_ReflowQueue.push_back(pLayoutBlock);
		}
		void CDOMdocument::Reflow()
		{
			if(!m_ReflowQueue.size())
			{
				return;
			}
		//	wprintf(L"I: Layout changed\n");
			for(UINT i = 0; i < m_ReflowQueue.size(); i++)
			{
				Render::IRenderFrame * pLayoutBlock = m_ReflowQueue[i];
				if(!pLayoutBlock->GetParent())
				{
					pLayoutBlock->SetWidth(GetGUI()->GetScreenWidth());
					pLayoutBlock->SetHeight(GetGUI()->GetScreenHeight());
				}
				pLayoutBlock->ResetLayout();
				pLayoutBlock->Layout();
			}
			m_ReflowQueue.clear();
		}

	};
};