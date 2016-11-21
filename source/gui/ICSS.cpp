#include "GUIbase.h"

#include "ICSS.h"
#include "CCSSstyle.h"
#include "CDOMdocument.h"


namespace GUI
{
	namespace CSS
	{

		const UINT ICSSrule::PSEUDOCLASS_COUNT = 20;

		void ICSSrule::ICSSselectorItem::SetPseudoClass(const StringW & str)
		{
			if(str == L"active")
			{
				pseudoclass |= PSEUDOCLASS_ACTIVE;
			}
			else if(str == L"checked")
			{
				pseudoclass |= PSEUDOCLASS_CHECKED;
			}
			else if(str == L"disabled")
			{
				pseudoclass |= PSEUDOCLASS_DISABLED;
			}
			else if(str == L"focus")
			{
				pseudoclass |= PSEUDOCLASS_FOCUS;
			}
			else if(str == L"empty")
			{
				pseudoclass |= PSEUDOCLASS_EMPTY;
			}
			else if(str == L"enabled")
			{
				pseudoclass |= PSEUDOCLASS_ENABLED;
			}
			else if(str == L"first-child")
			{
				pseudoclass |= PSEUDOCLASS_FIRST_CHILD;
			}
			else if(str == L"first-of-type")
			{
				pseudoclass |= PSEUDOCLASS_FIRST_OF_TYPE;
			}
			else if(str == L"hover")
			{
				pseudoclass |= PSEUDOCLASS_HOVER;
			}
			else if(str == L"last-child")
			{
				pseudoclass |= PSEUDOCLASS_LAST_CHILD;
			}
			else if(str == L"last-of-type")
			{
				pseudoclass |= PSEUDOCLASS_LAST_OF_TYPE;
			}
			else if(str == L"only-of-type")
			{
				pseudoclass |= PSEUDOCLASS_ONLY_OF_TYPE;
			}
			else if(str == L"only-child")
			{
				pseudoclass |= PSEUDOCLASS_ONLY_CHILD;
			}
			else if(str == L"read-only")
			{
				pseudoclass |= PSEUDOCLASS_READ_ONLY;
			}
			else if(str == L"read-write")
			{
				pseudoclass |= PSEUDOCLASS_READ_WRITE;
			}
			else
			{
				if(str.substr(0, 9) == L"nth-child")
				{
					pseudoclass |= PSEUDOCLASS_NTH_CHILD;
				}
				else if(str.substr(0, 14) == L"nth-last-child")
				{
					pseudoclass |= PSEUDOCLASS_NTH_LAST_CHILD;
				}
				else if(str.substr(0, 11) == L"nth-of-type")
				{
					pseudoclass |= PSEUDOCLASS_NTH_OF_TYPE;
				}
				else if(str.substr(0, 16) == L"nth-last-of-type")
				{
					pseudoclass |= PSEUDOCLASS_NTH_LAST_OF_TYPE;
				}
				if(pseudoclass & (PSEUDOCLASS_NTH_CHILD | PSEUDOCLASS_NTH_LAST_CHILD | PSEUDOCLASS_NTH_OF_TYPE | PSEUDOCLASS_NTH_LAST_OF_TYPE))
				{
					UINT pos = str.find(L"(");
					if(pos != (UINT)(-1))
					{
						UINT d = 0, o = 0;
						WCHAR cn;
						StringW prm = str.substr(pos + 1);
						UINT c = swscanf_s(prm.c_str(), L"%d%c-%d", &d, &cn, sizeof(cn), &o);
						if(c >= 1)
						{
							nth_num = d;
							if(c >= 2)
							{
								if(cn == L'n')
								{
									pseudoclass |= PSEUDOCLASS_NTH_REPEAT;
								}
								if(c >= 3)
								{
									nth_offset = o;
								}
							}
						}
					}
				}
			}
		}

		void ICSSrule::ICSSselectorItem::SetPseudoElement(const StringW & str)
		{
			if(str == L"first-letter")
			{
				pseudoelement = PSEUDOELEMENT_FIRST_LETTER;
			}
			else if(str == L"first-line")
			{
				pseudoelement = PSEUDOELEMENT_FIRST_LINE;
			}
			else if(str == L"selection")
			{
				pseudoelement = PSEUDOELEMENT_SELECTION;
			}
		}

		void ICSSrule::ICSSselectorItem::BuildIndex(ICSS * css)
		{
			DOM::CDOMdocument * doc = css->GetDoc();
			if(s_tag.length())
			{
				dom_tag = doc->GetIndexForTagString(s_tag);
			}
			if(s_id.length())
			{
				dom_id = doc->GetIndexForIdString(s_id);
			}
			for(UINT i = 0; i < s_class.size(); i++)
			{
				dom_class.push_back(doc->GetIndexForClassString(s_class[i]));
			}
		}

		void ICSS::DebugDumpStyles()
		{
			for(AssotiativeArray<StringW, ICSSstyleSet>::Iterator i = m_styleSets.begin(); i; i++)
			{
				wprintf(L"########################################\n");
				wprintf(L"%s\n", i.first->c_str());
				wprintf(L"########################################\n");
				i.second->DebugDumpStyles();
			}
		}

		void ICSSstyleSet::DebugDumpStyles()
		{
			for(UINT i = 0; i < m_pRules.size(); i++)
			{
				((CCSSstyle*)&m_pRules[i])->DebugDumpStyles();
			}
		}

		void CCSSstyle::DebugDumpStyles()
		{
			UINT len = m_pRules.size();
			for(UINT i = 0; i < len; i++)
			{
				m_pRules[i].DebugDumpStyles();
				if(i + 1 < len)
				{
					wprintf(L",\n");
				}
			}
			wprintf(L"\n{\n");
			if(p_display.IsSet())
			{
				wprintf(L"\tdisplay: ");
				switch(p_display.GetInt())
				{
				case ICSSproperty::DISPLAY_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::DISPLAY_BLOCK:
					wprintf(L"block;\n");
					break;
				case ICSSproperty::DISPLAY_INLINE:
					wprintf(L"inline;\n");
					break;
				case ICSSproperty::DISPLAY_INLINE_BLOCK:
					wprintf(L"inline-block;\n");
					break;
				case ICSSproperty::DISPLAY_LIST_ITEM:
					wprintf(L"list-item;\n");
					break;
				case ICSSproperty::DISPLAY_TABLE:
					wprintf(L"table;\n");
					break;
				case ICSSproperty::DISPLAY_TABLE_CAPTION:
					wprintf(L"table-caption;\n");
					break;
				case ICSSproperty::DISPLAY_TABLE_COLUMN:
					wprintf(L"table-column;\n");
					break;
				case ICSSproperty::DISPLAY_TABLE_ROW:
					wprintf(L"table-row;\n");
					break;
				case ICSSproperty::DISPLAY_TABLE_CELL:
					wprintf(L"table-cell;\n");
					break;
				case ICSSproperty::DISPLAY_INHERIT:
					wprintf(L"inherit;\n");
					break;
				}
			}
			if(p_border_top_left_method.IsSet())
			{
				wprintf(L"\tborder-top-left-method: ");
				switch(p_border_top_left_method.GetInt())
				{
				case ICSSproperty::BORDER_METHOD_RADIUS:
					wprintf(L"radius;\n");
					break;
				case ICSSproperty::BORDER_METHOD_CUT:
					wprintf(L"cut;\n");
					break;
				}
			}
			if(p_border_top_right_method.IsSet())
			{
				wprintf(L"\tborder-top-right-method: ");
				switch(p_border_top_right_method.GetInt())
				{
				case ICSSproperty::BORDER_METHOD_RADIUS:
					wprintf(L"radius;\n");
					break;
				case ICSSproperty::BORDER_METHOD_CUT:
					wprintf(L"cut;\n");
					break;
				}
			}
			if(p_border_bottom_left_method.IsSet())
			{
				wprintf(L"\tborder-bottom-left-method: ");
				switch(p_border_bottom_left_method.GetInt())
				{
				case ICSSproperty::BORDER_METHOD_RADIUS:
					wprintf(L"radius;\n");
					break;
				case ICSSproperty::BORDER_METHOD_CUT:
					wprintf(L"cut;\n");
					break;
				}
			}
			if(p_border_bottom_right_method.IsSet())
			{
				wprintf(L"\tborder-bottom-right-method: ");
				switch(p_border_bottom_right_method.GetInt())
				{
				case ICSSproperty::BORDER_METHOD_RADIUS:
					wprintf(L"radius;\n");
					break;
				case ICSSproperty::BORDER_METHOD_CUT:
					wprintf(L"cut;\n");
					break;
				}
			}
			if(p_color.IsSet())
			{
				wprintf(L"\tcolor: #%08X;\n", p_color.GetInt());
			}
			if(p_opacity.IsSet())
			{
				wprintf(L"\topacity: %f;\n", p_opacity.GetFloat());
			}
			if(p_z_index.IsSet())
			{
				wprintf(L"\tz-index: %d;\n", p_z_index.GetInt());
			}
			if(p_overflow_x.IsSet())
			{
				if(p_overflow_x == p_overflow_y)
				{
					wprintf(L"\toverflow: ");
					switch(p_overflow_x.GetInt())
					{
					case ICSSproperty::OVERFLOW_AUTO:
						wprintf(L"auto;\n");
						break;
					case ICSSproperty::OVERFLOW_HIDDEN:
						wprintf(L"hidden;\n");
						break;
					case ICSSproperty::OVERFLOW_SCROLL:
						wprintf(L"scroll;\n");
						break;
					case ICSSproperty::OVERFLOW_VISIBLE:
						wprintf(L"visible;\n");
						break;
					}
				}
				else
				{
					wprintf(L"\toverflow-x: ");
					switch(p_overflow_x.GetInt())
					{
					case ICSSproperty::OVERFLOW_AUTO:
						wprintf(L"auto;\n");
						break;
					case ICSSproperty::OVERFLOW_HIDDEN:
						wprintf(L"hidden;\n");
						break;
					case ICSSproperty::OVERFLOW_SCROLL:
						wprintf(L"scroll;\n");
						break;
					case ICSSproperty::OVERFLOW_VISIBLE:
						wprintf(L"visible;\n");
						break;
					}
				}
			}
			if(p_overflow_y.IsSet())
			{
				if(!(p_overflow_x == p_overflow_y))
				{
					wprintf(L"\toverflow-y: ");
					switch(p_overflow_x.GetInt())
					{
					case ICSSproperty::OVERFLOW_AUTO:
						wprintf(L"auto;\n");
						break;
					case ICSSproperty::OVERFLOW_HIDDEN:
						wprintf(L"hidden;\n");
						break;
					case ICSSproperty::OVERFLOW_SCROLL:
						wprintf(L"scroll;\n");
						break;
					case ICSSproperty::OVERFLOW_VISIBLE:
						wprintf(L"visible;\n");
						break;
					}
				}
			}

			if(p_margin_left.IsSet())
			{
				wprintf(L"\tmargin-left: %f%s;\n", p_margin_left.GetFloat(), CCSSproperty::GetDimName(p_margin_left.GetDim()));
			}
			if(p_margin_right.IsSet())
			{
				wprintf(L"\tmargin-right: %f%s;\n", p_margin_right.GetFloat(), CCSSproperty::GetDimName(p_margin_right.GetDim()));
			}
			if(p_margin_top.IsSet())
			{
				wprintf(L"\tmargin-top: %f%s;\n", p_margin_top.GetFloat(), CCSSproperty::GetDimName(p_margin_top.GetDim()));
			}
			if(p_margin_bottom.IsSet())
			{
				wprintf(L"\tmargin-bottom: %f%s;\n", p_margin_bottom.GetFloat(), CCSSproperty::GetDimName(p_margin_bottom.GetDim()));
			}

			if(p_padding_left.IsSet())
			{
				wprintf(L"\tpadding-left: %f%s;\n", p_padding_left.GetFloat(), CCSSproperty::GetDimName(p_padding_left.GetDim()));
			}
			if(p_padding_right.IsSet())
			{
				wprintf(L"\tpadding-right: %f%s;\n", p_padding_right.GetFloat(), CCSSproperty::GetDimName(p_padding_right.GetDim()));
			}
			if(p_padding_top.IsSet())
			{
				wprintf(L"\tpadding-top: %f%s;\n", p_padding_top.GetFloat(), CCSSproperty::GetDimName(p_padding_top.GetDim()));
			}
			if(p_padding_bottom.IsSet())
			{
				wprintf(L"\tpadding-bottom: %f%s;\n", p_padding_bottom.GetFloat(), CCSSproperty::GetDimName(p_padding_bottom.GetDim()));
			}

			if(p_left.IsSet())
			{
				wprintf(L"\tleft: %f%s;\n", p_left.GetFloat(), CCSSproperty::GetDimName(p_left.GetDim()));
			}
			if(p_right.IsSet())
			{
				wprintf(L"\tright: %f%s;\n", p_right.GetFloat(), CCSSproperty::GetDimName(p_right.GetDim()));
			}
			if(p_top.IsSet())
			{
				wprintf(L"\ttop: %f%s;\n", p_top.GetFloat(), CCSSproperty::GetDimName(p_top.GetDim()));
			}
			if(p_bottom.IsSet())
			{
				wprintf(L"\tbottom: %f%s;\n", p_bottom.GetFloat(), CCSSproperty::GetDimName(p_bottom.GetDim()));
			}

			if(p_width.IsSet())
			{
				wprintf(L"\twidth: %f%s;\n", p_width.GetFloat(), CCSSproperty::GetDimName(p_width.GetDim()));
			}
			if(p_height.IsSet())
			{
				wprintf(L"\theight: %f%s;\n", p_height.GetFloat(), CCSSproperty::GetDimName(p_height.GetDim()));
			}

			if(p_max_width.IsSet())
			{
				wprintf(L"\tmax-width: %f%s;\n", p_max_width.GetFloat(), CCSSproperty::GetDimName(p_max_width.GetDim()));
			}
			if(p_max_height.IsSet())
			{
				wprintf(L"\tmax-height: %f%s;\n", p_max_height.GetFloat(), CCSSproperty::GetDimName(p_max_height.GetDim()));
			}

			if(p_min_width.IsSet())
			{
				wprintf(L"\tmin-width: %f%s;\n", p_min_width.GetFloat(), CCSSproperty::GetDimName(p_min_width.GetDim()));
			}
			if(p_min_height.IsSet())
			{
				wprintf(L"\tmin-height: %f%s;\n", p_min_height.GetFloat(), CCSSproperty::GetDimName(p_min_height.GetDim()));
			}

			if(p_cursor.IsSet())
			{
				wprintf(L"\tcursor: ");
				switch(p_cursor.GetInt())
				{
				case ICSSproperty::CURSOR_AUTO:
					wprintf(L"auto;\n");
					break;
				case ICSSproperty::CURSOR_CELL:
					wprintf(L"cell;\n");
					break;
				case ICSSproperty::CURSOR_COL_RESIZE:
					wprintf(L"col-resize;\n");
					break;
				case ICSSproperty::CURSOR_COPY:
					wprintf(L"copy;\n");
					break;
				case ICSSproperty::CURSOR_CROSSHAIR:
					wprintf(L"crosshair;\n");
					break;
				case ICSSproperty::CURSOR_DEFAULT:
					wprintf(L"default;\n");
					break;
				case ICSSproperty::CURSOR_E_RESIZE:
					wprintf(L"e-resize;\n");
					break;
				case ICSSproperty::CURSOR_EW_RESIZE:
					wprintf(L"ew-resize;\n");
					break;
				case ICSSproperty::CURSOR_HELP:
					wprintf(L"help;\n");
					break;
				case ICSSproperty::CURSOR_MOVE:
					wprintf(L"move;\n");
					break;
				case ICSSproperty::CURSOR_N_RESIZE:
					wprintf(L"n-resize;\n");
					break;
				case ICSSproperty::CURSOR_NE_RESIZE:
					wprintf(L"ne-resize;\n");
					break;
				case ICSSproperty::CURSOR_NESW_RESIZE:
					wprintf(L"nesw-resize;\n");
					break;
				case ICSSproperty::CURSOR_NO_DROP:
					wprintf(L"no-drop;\n");
					break;
				case ICSSproperty::CURSOR_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::CURSOR_NOT_ALLOWED:
					wprintf(L"not-allowed;\n");
					break;
				case ICSSproperty::CURSOR_NS_RESIZE:
					wprintf(L"ns-resize;\n");
					break;
				case ICSSproperty::CURSOR_NW_RESIZE:
					wprintf(L"nw-resize;\n");
					break;
				case ICSSproperty::CURSOR_NWSE_RESIZE:
					wprintf(L"nwse-resize;\n");
					break;
				case ICSSproperty::CURSOR_POINTER:
					wprintf(L"pointer;\n");
					break;
				case ICSSproperty::CURSOR_PROGRESS:
					wprintf(L"progress;\n");
					break;
				case ICSSproperty::CURSOR_ROW_RESIZE:
					wprintf(L"row-resize;\n");
					break;
				case ICSSproperty::CURSOR_S_RESIZE:
					wprintf(L"s-resize;\n");
					break;
				case ICSSproperty::CURSOR_SE_RESIZE:
					wprintf(L"se-resize;\n");
					break;
				case ICSSproperty::CURSOR_SW_RESIZE:
					wprintf(L"sw-resize;\n");
					break;
				case ICSSproperty::CURSOR_TEXT:
					wprintf(L"text;\n");
					break;
				case ICSSproperty::CURSOR_VERTICAL_TEXT:
					wprintf(L"vertical-text;\n");
					break;
				case ICSSproperty::CURSOR_W_RESIZE:
					wprintf(L"w-resize;\n");
					break;
				case ICSSproperty::CURSOR_WAIT:
					wprintf(L"wait;\n");
					break;
				case ICSSproperty::CURSOR_INHERIT:
					wprintf(L"inherit;\n");
					break;
				case ICSSproperty::CURSOR_GRAB:
					wprintf(L"grab;\n");
					break;
				case ICSSproperty::CURSOR_GRABBING:
					wprintf(L"grabbing;\n");
					break;
				case ICSSproperty::CURSOR_ZOOM_IN:
					wprintf(L"zoom-in;\n");
					break;
				case ICSSproperty::CURSOR_ZOOM_OUT:
					wprintf(L"zoom-out;\n");
					break;
				case ICSSproperty::CURSOR_ALIAS:
					wprintf(L"alias;\n");
					break;
				case ICSSproperty::CURSOR_ALL_SCROLL:
					wprintf(L"all-scroll;\n");
					break;
				}
			}

			if(p_direction.IsSet())
			{
				wprintf(L"\tdirection: ");
				switch(p_direction.GetInt())
				{
				case ICSSproperty::DIRECTION_INHERIT:
					wprintf(L"inherit;\n");
					break;
				case ICSSproperty::DIRECTION_LTR:
					wprintf(L"ltr;\n");
					break;
				case ICSSproperty::DIRECTION_RTL:
					wprintf(L"rtl;\n");
					break;
				}
			}

			if(p_position.IsSet())
			{
				wprintf(L"\tposition: ");
				switch(p_position.GetInt())
				{
				case ICSSproperty::POSITION_STATIC:
					wprintf(L"static;\n");
					break;
				case ICSSproperty::POSITION_ABSOLUTE:
					wprintf(L"absolute;\n");
					break;
				case ICSSproperty::POSITION_RELATIVE:
					wprintf(L"relative;\n");
					break;
				case ICSSproperty::POSITION_FIXED:
					wprintf(L"fixed;\n");
					break;
				}
			}

			if(p_visibility.IsSet())
			{
				wprintf(L"\tvisibility: ");
				switch(p_visibility.GetInt())
				{
				case ICSSproperty::VISIBILITY_VISIBLE:
					wprintf(L"visible;\n");
					break;
				case ICSSproperty::VISIBILITY_HIDDEN:
					wprintf(L"hidden;\n");
					break;
				}
			}

			if(p_float.IsSet())
			{
				wprintf(L"\tfloat: ");
				switch(p_float.GetInt())
				{
				case ICSSproperty::FLOAT_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::FLOAT_LEFT:
					wprintf(L"left;\n");
					break;
				case ICSSproperty::FLOAT_RIGHT:
					wprintf(L"right;\n");
					break;
				}
			}

			if(p_float.IsSet())
			{
				wprintf(L"\tfloat: ");
				switch(p_float.GetInt())
				{
				case ICSSproperty::FLOAT_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::FLOAT_LEFT:
					wprintf(L"left;\n");
					break;
				case ICSSproperty::FLOAT_RIGHT:
					wprintf(L"right;\n");
					break;
				}
			}
			if(p_background_image.IsSet())
			{
				wprintf(L"\tbackground-image: %s;\n", p_background_image.GetString());
			}

			if(p_background_color.IsSet())
			{
				wprintf(L"\tcolor: #%08X;\n", p_background_color.GetInt());
			}

			if(p_background_position_x.IsSet())
			{
				wprintf(L"\tbackground-position-x: %f%s;\n", p_background_position_x.GetFloat(), CCSSproperty::GetDimName(p_background_position_x.GetDim()));
			}
			if(p_background_position_y.IsSet())
			{
				wprintf(L"\tbackground-position-y: %f%s;\n", p_background_position_y.GetFloat(), CCSSproperty::GetDimName(p_background_position_y.GetDim()));
			}

			if(p_background_size_x.IsSet())
			{
				wprintf(L"\tbackground-size-x: %f%s;\n", p_background_size_x.GetFloat(), CCSSproperty::GetDimName(p_background_size_x.GetDim()));
			}
			if(p_background_size_y.IsSet())
			{
				wprintf(L"\tbackground-size-y: %f%s;\n", p_background_size_y.GetFloat(), CCSSproperty::GetDimName(p_background_size_y.GetDim()));
			}

			if(p_background_attachment.IsSet())
			{
				wprintf(L"\tbackground-attachment: ");
				switch(p_background_attachment.GetInt())
				{
				case ICSSproperty::BACKGROUND_ATTACHMENT_FIXED:
					wprintf(L"fixed;\n");
					break;
				case ICSSproperty::BACKGROUND_ATTACHMENT_LOCAL:
					wprintf(L"local;\n");
					break;
				case ICSSproperty::BACKGROUND_ATTACHMENT_SCROLL:
					wprintf(L"scroll;\n");
					break;
				}
			}

			if(p_background_repeat.IsSet())
			{
				wprintf(L"\tbackground-repeat: ");
				switch(p_background_repeat.GetInt())
				{
				case ICSSproperty::BACKGROUND_REPEAT_NO_REPEAT:
					wprintf(L"no-repeat;\n");
					break;
				case ICSSproperty::BACKGROUND_REPEAT_REPEAT_X:
					wprintf(L"repeat-x;\n");
					break;
				case ICSSproperty::BACKGROUND_REPEAT_REPEAT_Y:
					wprintf(L"repeat-y;\n");
					break;
				case ICSSproperty::BACKGROUND_REPEAT_REPEAT:
					wprintf(L"repeat;\n");
					break;
				}
			}

			if(p_border_top_style.IsSet())
			{
				wprintf(L"\tborder-top-style: ");
				switch(p_border_top_style.GetInt())
				{
				case ICSSproperty::BORDER_STYLE_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::BORDER_STYLE_INSET:
					wprintf(L"inset;\n");
					break;
				case ICSSproperty::BORDER_STYLE_OUTSET:
					wprintf(L"outset;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DASHED:
					wprintf(L"dashed;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DOTTED:
					wprintf(L"dotted;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DOUBLE:
					wprintf(L"double;\n");
					break;
				case ICSSproperty::BORDER_STYLE_GROOVE:
					wprintf(L"groove;\n");
					break;
				case ICSSproperty::BORDER_STYLE_RIDGE:
					wprintf(L"ridge;\n");
					break;
				case ICSSproperty::BORDER_STYLE_SOLID:
					wprintf(L"solid;\n");
					break;
				}
			}
			if(p_border_left_style.IsSet())
			{
				wprintf(L"\tborder-left-style: ");
				switch(p_border_left_style.GetInt())
				{
				case ICSSproperty::BORDER_STYLE_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::BORDER_STYLE_INSET:
					wprintf(L"inset;\n");
					break;
				case ICSSproperty::BORDER_STYLE_OUTSET:
					wprintf(L"outset;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DASHED:
					wprintf(L"dashed;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DOTTED:
					wprintf(L"dotted;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DOUBLE:
					wprintf(L"double;\n");
					break;
				case ICSSproperty::BORDER_STYLE_GROOVE:
					wprintf(L"groove;\n");
					break;
				case ICSSproperty::BORDER_STYLE_RIDGE:
					wprintf(L"ridge;\n");
					break;
				case ICSSproperty::BORDER_STYLE_SOLID:
					wprintf(L"solid;\n");
					break;
				}
			}
			if(p_border_right_style.IsSet())
			{
				wprintf(L"\tborder-right-style: ");
				switch(p_border_right_style.GetInt())
				{
				case ICSSproperty::BORDER_STYLE_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::BORDER_STYLE_INSET:
					wprintf(L"inset;\n");
					break;
				case ICSSproperty::BORDER_STYLE_OUTSET:
					wprintf(L"outset;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DASHED:
					wprintf(L"dashed;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DOTTED:
					wprintf(L"dotted;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DOUBLE:
					wprintf(L"double;\n");
					break;
				case ICSSproperty::BORDER_STYLE_GROOVE:
					wprintf(L"groove;\n");
					break;
				case ICSSproperty::BORDER_STYLE_RIDGE:
					wprintf(L"ridge;\n");
					break;
				case ICSSproperty::BORDER_STYLE_SOLID:
					wprintf(L"solid;\n");
					break;
				}
			}
			if(p_border_bottom_style.IsSet())
			{
				wprintf(L"\tborder-bottom-style: ");
				switch(p_border_bottom_style.GetInt())
				{
				case ICSSproperty::BORDER_STYLE_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::BORDER_STYLE_INSET:
					wprintf(L"inset;\n");
					break;
				case ICSSproperty::BORDER_STYLE_OUTSET:
					wprintf(L"outset;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DASHED:
					wprintf(L"dashed;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DOTTED:
					wprintf(L"dotted;\n");
					break;
				case ICSSproperty::BORDER_STYLE_DOUBLE:
					wprintf(L"double;\n");
					break;
				case ICSSproperty::BORDER_STYLE_GROOVE:
					wprintf(L"groove;\n");
					break;
				case ICSSproperty::BORDER_STYLE_RIDGE:
					wprintf(L"ridge;\n");
					break;
				case ICSSproperty::BORDER_STYLE_SOLID:
					wprintf(L"solid;\n");
					break;
				}
			}

			if(p_border_top_width.IsSet())
			{
				wprintf(L"\tborder-top-width: %f%s;\n", p_border_top_width.GetFloat(), CCSSproperty::GetDimName(p_border_top_width.GetDim()));
			}
			if(p_border_right_width.IsSet())
			{
				wprintf(L"\tborder-right-width: %f%s;\n", p_border_right_width.GetFloat(), CCSSproperty::GetDimName(p_border_right_width.GetDim()));
			}
			if(p_border_bottom_width.IsSet())
			{
				wprintf(L"\tborder-bottom-width: %f%s;\n", p_border_bottom_width.GetFloat(), CCSSproperty::GetDimName(p_border_bottom_width.GetDim()));
			}
			if(p_border_left_width.IsSet())
			{
				wprintf(L"\tborder-left-width: %f%s;\n", p_border_left_width.GetFloat(), CCSSproperty::GetDimName(p_border_left_width.GetDim()));
			}

			if(p_border_top_color.IsSet())
			{
				wprintf(L"\tborder_top_color: #%08X;\n", p_border_top_color.GetInt());
			}
			if(p_border_right_color.IsSet())
			{
				wprintf(L"\tborder_right_color: #%08X;\n", p_border_right_color.GetInt());
			}
			if(p_border_bottom_color.IsSet())
			{
				wprintf(L"\tborder_bottom_color: #%08X;\n", p_border_bottom_color.GetInt());
			}
			if(p_border_left_color.IsSet())
			{
				wprintf(L"\tborder_left_color: #%08X;\n", p_border_left_color.GetInt());
			}

			if(p_border_top_left_radius.IsSet())
			{
				wprintf(L"\tborder-top-left-radius: %f%s;\n", p_border_top_left_radius.GetFloat(), CCSSproperty::GetDimName(p_border_top_left_radius.GetDim()));
			}
			if(p_border_top_right_radius.IsSet())
			{
				wprintf(L"\tborder-top-right-radius: %f%s;\n", p_border_top_right_radius.GetFloat(), CCSSproperty::GetDimName(p_border_top_right_radius.GetDim()));
			}

			if(p_border_bottom_left_radius.IsSet())
			{
				wprintf(L"\tborder-bottom-left-radius: %f%s;\n", p_border_bottom_left_radius.GetFloat(), CCSSproperty::GetDimName(p_border_bottom_left_radius.GetDim()));
			}
			if(p_border_bottom_right_radius.IsSet())
			{
				wprintf(L"\tborder-bottom-right-radius: %f%s;\n", p_border_bottom_right_radius.GetFloat(), CCSSproperty::GetDimName(p_border_bottom_right_radius.GetDim()));
			}

			if(p_font_name.IsSet())
			{
				wprintf(L"\tfont-name: %s;\n", p_font_name.GetString());
			}

			if(p_font_size.IsSet())
			{
				wprintf(L"\tfont-size: %f%s;\n", p_font_size.GetFloat(), CCSSproperty::GetDimName(p_font_size.GetDim()));
			}

			if(p_font_style.IsSet())
			{
				wprintf(L"\tfont-style: ");
				switch(p_font_style.GetInt())
				{
				case ICSSproperty::FONT_STYLE_NORMAL:
					wprintf(L"normal;\n");
					break;
				case ICSSproperty::FONT_STYLE_ITALIC:
					wprintf(L"italic;\n");
					break;
				case ICSSproperty::FONT_STYLE_INHERIT:
					wprintf(L"inherit;\n");
					break;
				}
			}

			if(p_font_weight.IsSet())
			{
				wprintf(L"\tfont-weight: ");
				switch(p_font_weight.GetInt())
				{
				case ICSSproperty::FONT_WEIGHT_NORMAL:
					wprintf(L"normal;\n");
					break;
				case ICSSproperty::FONT_WEIGHT_BOLD:
					wprintf(L"bold;\n");
					break;
				case ICSSproperty::FONT_WEIGHT_INHERIT:
					wprintf(L"inherit;\n");
					break;
				}
			}

			if(p_text_align.IsSet())
			{
				wprintf(L"\ttext-align: ");
				switch(p_text_align.GetInt())
				{
				case ICSSproperty::TEXT_ALIGN_LEFT:
					wprintf(L"left;\n");
					break;
				case ICSSproperty::TEXT_ALIGN_RIGHT:
					wprintf(L"right;\n");
					break;
				case ICSSproperty::TEXT_ALIGN_INHERIT:
					wprintf(L"inherit;\n");
					break;
				case ICSSproperty::TEXT_ALIGN_CENTER:
					wprintf(L"center;\n");
					break;
				}
			}

			if(p_text_decoration_line.IsSet())
			{
				wprintf(L"\ttext-decoration-line: ");
				switch(p_text_decoration_line.GetInt())
				{
				case ICSSproperty::TEXT_DECORATION_LINE_NONE:
					wprintf(L"none;\n");
					break;
				case ICSSproperty::TEXT_DECORATION_LINE_UNDERLINE:
					wprintf(L"underline;\n");
					break;
				case ICSSproperty::TEXT_DECORATION_LINE_LINE_THROUGH:
					wprintf(L"line-through;\n");
					break;
				case ICSSproperty::TEXT_DECORATION_LINE_OVERLINE:
					wprintf(L"overline;\n");
					break;
				}
			}

			wprintf(L"}\n\n");
		}

		void ICSSrule::DebugDumpStyles()
		{
			UINT len = m_selectors.size();
			for(UINT i = 0; i < len; i++)
			{
				ICSSselectorItem * sel = &m_selectors[i];
				if(sel->s_tag.length())
				{
					wprintf(L"%s", sel->s_tag.c_str());
				}
				if(sel->s_id.length())
				{
					wprintf(L"#%s", sel->s_id.c_str());
				}
				for(UINT c = 0; c < sel->s_class.size(); c++)
				{
					wprintf(L".%s", sel->s_class[c].c_str());
				}

				if(sel->pseudoclass & PSEUDOCLASS_ACTIVE)
				{
					wprintf(L":active");
				}
				if(sel->pseudoclass & PSEUDOCLASS_CHECKED)
				{
					wprintf(L":checked");
				}
				if(sel->pseudoclass & PSEUDOCLASS_DISABLED)
				{
					wprintf(L":disabled");
				}
				if(sel->pseudoclass & PSEUDOCLASS_EMPTY)
				{
					wprintf(L":empty");
				}
				if(sel->pseudoclass & PSEUDOCLASS_ENABLED)
				{
					wprintf(L":enabled");
				}
				if(sel->pseudoclass & PSEUDOCLASS_FIRST_CHILD)
				{
					wprintf(L":first-child");
				}
				if(sel->pseudoclass & PSEUDOCLASS_FIRST_OF_TYPE)
				{
					wprintf(L":first-of-type");
				}
				if(sel->pseudoclass & PSEUDOCLASS_FOCUS)
				{
					wprintf(L":focus");
				}
				if(sel->pseudoclass & PSEUDOCLASS_HOVER)
				{
					wprintf(L":hover");
				}
				if(sel->pseudoclass & PSEUDOCLASS_LAST_CHILD)
				{
					wprintf(L":last-child");
				}
				if(sel->pseudoclass & PSEUDOCLASS_LAST_OF_TYPE)
				{
					wprintf(L":last-of-type");
				}
				if(sel->pseudoclass & PSEUDOCLASS_ONLY_CHILD)
				{
					wprintf(L":only-child");
				}
				if(sel->pseudoclass & PSEUDOCLASS_ONLY_OF_TYPE)
				{
					wprintf(L":only-of-type");
				}
				if(sel->pseudoclass & PSEUDOCLASS_READ_ONLY)
				{
					wprintf(L":read-only");
				}
				if(sel->pseudoclass & PSEUDOCLASS_READ_WRITE)
				{
					wprintf(L":read-write");
				}
				if(sel->pseudoclass & PSEUDOCLASS_NTH_CHILD)
				{
					wprintf(L":nth-child");
				}
				if(sel->pseudoclass & PSEUDOCLASS_NTH_LAST_CHILD)
				{
					wprintf(L":nth-last-child");
				}
				if(sel->pseudoclass & PSEUDOCLASS_NTH_LAST_OF_TYPE)
				{
					wprintf(L":nth-last-of-type");
				}
				if(sel->pseudoclass & PSEUDOCLASS_NTH_OF_TYPE)
				{
					wprintf(L":nth-of-type");
				}
				if(sel->pseudoclass & (PSEUDOCLASS_NTH_CHILD | PSEUDOCLASS_NTH_LAST_CHILD | PSEUDOCLASS_NTH_LAST_OF_TYPE | PSEUDOCLASS_NTH_OF_TYPE))
				{
					wprintf(L"(%d", sel->nth_num, sel->pseudoclass & PSEUDOCLASS_NTH_REPEAT ? L"n" : L"", sel->nth_offset ? L"+" : L"", sel->nth_offset);
					if(sel->nth_num, sel->pseudoclass & PSEUDOCLASS_NTH_REPEAT)
					{
						wprintf(L"n");
						if(sel->nth_offset)
						{
							wprintf(L"-%d", sel->nth_offset);
						}
					}
					wprintf(L")");
				}

				switch(sel->pseudoelement)
				{
				case PSEUDOELEMENT_FIRST_LETTER:
					wprintf(L"::first-letter");
					break;
				case PSEUDOELEMENT_FIRST_LINE:
					wprintf(L"::first-line");
					break;
				case PSEUDOELEMENT_SELECTION:
					wprintf(L"::selection");
					break;
				}
				wprintf(L" ");
				switch(sel->connector)
				{
				case CONNECTOR_AFTER:
					wprintf(L"~ ");
					break;
				case CONNECTOR_CHILD:
					wprintf(L"> ");
					break;
				case CONNECTOR_NEXT:
					wprintf(L"+ ");
					break;
				}
			}
		}

		void ICSSstyleSet::LoadFromString(const WCHAR * wstr)
		{
			UINT len = wcslen(wstr);

			bool is_wait_begin = true;
			bool is_read_pseudoclass = false;
			bool is_read_pseudoelement = false;
			bool is_read_tag = false;
			bool is_read_class = false;
			bool is_read_id = false;
			bool is_read_connector = false;
			bool is_connector_read = false;

			Array<ICSSrule> rules;

			ICSSrule::ICSSselectorItem curSelector;
			ICSSrule curRule;

			StringW str;

			WCHAR c;
			for(UINT i = 0; i < len; i++)
			{
				c = wstr[i];
				// tag#id.class1.class2 > tag2#id2 + tag3#id3 ~ tags
				if(iswspace(c))
				{
					if(is_read_pseudoclass)
					{
						curSelector.SetPseudoClass(str);
						str = L"";
						is_read_pseudoclass = false;
						is_read_connector = true;
					}
					if(is_read_pseudoelement)
					{
						curSelector.SetPseudoElement(str);
						str = L"";
						is_read_pseudoelement = false;
						is_read_connector = true;
					}
					if(is_read_tag)
					{
						curSelector.s_tag = str;
						str = L"";
						is_read_tag = false;
						is_read_connector = true;
					}
					if(is_read_class)
					{
						curSelector.AddClass(str);
						str = L"";
						is_read_class = false;
						is_read_connector = true;
					}
					if(is_read_id)
					{
						curSelector.s_id = str;
						str = L"";
						is_read_id = false;
						is_read_connector = true;
					}
				}
				else
				{
					if(c == L'>' || c == L'+' || c == L'~' || c == ',')
					{
						if(is_read_pseudoclass)
						{
							curSelector.SetPseudoClass(str);
							str = L"";
							is_read_pseudoclass = false;
							is_read_connector = true;
						}
						if(is_read_pseudoelement)
						{
							curSelector.SetPseudoElement(str);
							str = L"";
							is_read_pseudoelement = false;
							is_read_connector = true;
						}
						if(is_read_tag)
						{
							curSelector.s_tag = str;
							str = L"";
							is_read_tag = false;
							is_read_connector = true;
						}
						if(is_read_class)
						{
							curSelector.AddClass(str);
							str = L"";
							is_read_class = false;
							is_read_connector = true;
						}
						if(is_read_id)
						{
							curSelector.s_id = str;
							str = L"";
							is_read_id = false;
							is_read_connector = true;
						}
						if(c == L',')
						{
							curSelector.BuildIndex(m_pCSS);
							curRule.AddSelector(curSelector);
							curSelector = ICSSrule::ICSSselectorItem();
							curSelector.dom_class.clear();
							curSelector.s_class.clear();
							rules.push_back(curRule);
							curRule.Release();
							is_read_connector = false;
						}
						else
						{
							if(is_read_connector)
							{
								switch(c)
								{
								case L'>':
									curSelector.connector = ICSSrule::CONNECTOR_CHILD;
									break;
								case L'+':
									curSelector.connector = ICSSrule::CONNECTOR_NEXT;
									break;
								case L'~':
									curSelector.connector = ICSSrule::CONNECTOR_AFTER;
									break;
								};
								is_read_connector = false;
								is_connector_read = true;
								curSelector.BuildIndex(m_pCSS);
								curRule.AddSelector(curSelector);
								curSelector = ICSSrule::ICSSselectorItem();
								curSelector.dom_class.clear();
								curSelector.s_class.clear();
							}
						}
					}
					else
					{
						if(is_read_connector)
						{
							if(c != '{')
							{
								curSelector.connector = ICSSrule::CONNECTOR_SIMPLE;
							}
							is_read_connector = false;
							is_connector_read = true;
							curSelector.BuildIndex(m_pCSS);
							curRule.AddSelector(curSelector);
							curSelector = ICSSrule::ICSSselectorItem();
							curSelector.dom_class.clear();
							curSelector.s_class.clear();
						}
						if(c == L'.' || c == L'#' || c == L':' || c == '{')
						{
							bool r = false;
							if(is_read_pseudoclass)
							{
								curSelector.SetPseudoClass(str);
								str = L"";
								is_read_pseudoclass = false;
								r = true;
							}
							if(is_read_pseudoelement)
							{
								curSelector.SetPseudoElement(str);
								str = L"";
								is_read_pseudoelement = false;
								r = true;
							}
							if(is_read_tag)
							{
								curSelector.s_tag = str;
								str = L"";
								is_read_tag = false;
								r = true;
							}
							if(is_read_class)
							{
								curSelector.AddClass(str);
								str = L"";
								is_read_class = false;
								r = true;
							}
							if(is_read_id)
							{
								curSelector.s_id = str;
								str = L"";
								is_read_id = false;
								r = true;
							}
							switch(c)
							{
							case L'.':
								is_read_class = true;
								break;

							case L'#':
								is_read_id = true;
								break;
							case L':':
								if(wstr[i + 1] == L':')
								{
									i++;
									is_read_pseudoelement = true;
								}
								else
								{
									is_read_pseudoclass = true;
								}
								break;
							}
							//finish read
							if(c == L'{')
							{
								if(r)
								{
									curSelector.BuildIndex(m_pCSS);
									curRule.AddSelector(curSelector);
									curSelector = ICSSrule::ICSSselectorItem();
									curSelector.dom_class.clear();
									curSelector.s_class.clear();
								}
								rules.push_back(curRule);
								//curRule = ICSSrule();
								curRule.Release();
								CCSSstyle css;
								UINT u;
								css.LoadStyleFromString(wstr + i + 1, &u);
								for(UINT r = 0; r < rules.size(); r++)
								{
									css.AddRule(rules[r]);
								}
								m_pRules.push_back(css);
								i += u;
								is_wait_begin = true;
								is_read_pseudoclass = false;
								is_read_pseudoelement = false;
								is_read_tag = false;
								is_read_class = false;
								is_read_id = false;
								is_read_connector = false;
								is_connector_read = false;
								rules.clear();
							}
						}
						else
						{
							if(is_read_class || is_read_id || is_read_pseudoclass || is_read_pseudoelement || is_read_tag)
							{
								str += c;
							}
							else
							{
								is_read_tag = true;
								str += c;
							}
						}
					}
				}
			}
		}

		void CCSSstyle::LoadStyleFromString(const WCHAR * str, UINT * pNumSyms)
		{
			StringW key;
			StringW val;
			bool readKeyBgn = true;
			bool readKey = false;
			bool readValBgn = false;
			bool readVal = false;
			UINT len = wcslen(str);
			WCHAR c;
			*pNumSyms = 0;
			for(UINT i = 0; i < len; i++)
			{
				(*pNumSyms)++;
				c = str[i];
				if(c == L'}')
				{
					break;
				}
				if(!iswspace(c))
				{
					if(c == L';') // end pair
					{
						if(!readVal)
						{
							wprintf(L"[Error]: Unexpected symbol ';' in css string\n");
						}
						else
						{
							readKey = false;
							readVal = false;
							readKeyBgn = true;
							readValBgn = false;
							//Process pair
							ParsePair(key, val);
						}
					}
					else if(c == L':') // end key begin value
					{
						if(!readKey)
						{
							wprintf(L"[Error]: Unexpected symbol ':' in css string\n");
						}
						else
						{
							readKey = false;
							readValBgn = true;
						}
					}
					else
					{
						if(readKey)
						{
							key += c;
						}
						else if(readValBgn)
						{
							readValBgn = false;
							readVal = true;
							val = c;
						}
						else if(readKeyBgn)
						{
							readKeyBgn = false;
							readKey = true;
							key = c;
						}
						else if(readVal)
						{
							val += c;
						}
					}
				}
				else
				{
					if(readVal)
					{
						val += c;
					}
				}
			}
			if(readVal)
			{
				ParsePair(key, val);
				//Process pair
			}
		}

		void CCSSstyle::ParsePair(const StringW & key, const StringW & val)
		{
			if(key == L"display")
			{
				if(val == L"none")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_NONE);
				}
				else if(val == L"block")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_BLOCK);
				}
				else if(val == L"inline")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_INLINE);
				}
				else if(val == L"inline-block")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_INLINE_BLOCK);
				}
				else if(val == L"list-item")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_LIST_ITEM);
				}
				else if(val == L"table")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_TABLE);
				}
				else if(val == L"table-caption")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_TABLE_CAPTION);
				}
				else if(val == L"table-column")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_TABLE_COLUMN);
				}
				else if(val == L"table-row")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_TABLE_ROW);
				}
				else if(val == L"table-cell")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_TABLE_CELL);
				}
				else if(val == L"inherit")
				{
					p_display.SetExt(ICSSproperty::DISPLAY_INHERIT);
				}
			}
			else if(key == L"color")
			{
				ParseColor(&p_color, val);
			}
			else if(key == L"cursor")
			{
				if(val == L"auto")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_AUTO);
				}
				else if(val == L"cell")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_CELL);
				}
				else if(val == L"col-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_COL_RESIZE);
				}
				else if(val == L"copy")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_COPY);
				}
				else if(val == L"crosshair")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_CROSSHAIR);
				}
				else if(val == L"default")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_DEFAULT);
				}
				else if(val == L"e-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_E_RESIZE);
				}
				else if(val == L"ew-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_EW_RESIZE);
				}
				else if(val == L"help")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_HELP);
				}
				else if(val == L"move")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_MOVE);
				}
				else if(val == L"n-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_N_RESIZE);
				}
				else if(val == L"ne-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_NE_RESIZE);
				}
				else if(val == L"nesw-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_NESW_RESIZE);
				}
				else if(val == L"no-drop")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_NO_DROP);
				}
				else if(val == L"none")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_NONE);
				}
				else if(val == L"not-allowed")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_NOT_ALLOWED);
				}
				else if(val == L"ns-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_NS_RESIZE);
				}
				else if(val == L"nw-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_NW_RESIZE);
				}
				else if(val == L"nwse-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_NWSE_RESIZE);
				}
				else if(val == L"pointer")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_POINTER);
				}
				else if(val == L"progress")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_PROGRESS);
				}
				else if(val == L"row-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_ROW_RESIZE);
				}
				else if(val == L"s-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_S_RESIZE);
				}
				else if(val == L"se-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_SE_RESIZE);
				}
				else if(val == L"sw-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_SW_RESIZE);
				}
				else if(val == L"text")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_TEXT);
				}
				else if(val == L"vertical-text")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_VERTICAL_TEXT);
				}
				else if(val == L"w-resize")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_W_RESIZE);
				}
				else if(val == L"wait")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_WAIT);
				}
				else if(val == L"inherit")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_INHERIT);
				}
				else if(val == L"grab")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_GRAB);
				}
				else if(val == L"grabbing")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_GRABBING);
				}
				else if(val == L"zoom-in")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_ZOOM_IN);
				}
				else if(val == L"zoom-out")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_ZOOM_OUT);
				}
				else if(val == L"alias")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_ALIAS);
				}
				else if(val == L"all-scroll")
				{
					p_cursor.SetExt(ICSSproperty::CURSOR_ALL_SCROLL);
				}
				// FIXME: Add support for custom cursor
			}
			else if(key == L"direction")
			{
				if(val == L"inherit")
				{
					p_direction.SetExt(ICSSproperty::DIRECTION_INHERIT);
				}
				else if(val == L"ltr")
				{
					p_direction.SetExt(ICSSproperty::DIRECTION_LTR);
				}
				else if(val == L"rtl")
				{
					p_direction.SetExt(ICSSproperty::DIRECTION_RTL);
				}
			}
			else if(key == L"opacity")
			{
				float p;
				swscanf_s(val.c_str(), L"%f", &p);
				p_opacity.Set(p);
			}
			else if(key == L"z-index")
			{
				int p;
				swscanf_s(val.c_str(), L"%d", &p);
				p_z_index.Set(p);
			}
			else if(key == L"overflow-x")
			{
				if(val == L"auto")
				{
					p_overflow_x.SetExt(ICSSproperty::OVERFLOW_AUTO);
				}
				else if(val == L"hidden")
				{
					p_overflow_x.SetExt(ICSSproperty::OVERFLOW_HIDDEN);
				}
				else if(val == L"scroll")
				{
					p_overflow_x.SetExt(ICSSproperty::OVERFLOW_SCROLL);
				}
				else if(val == L"visible")
				{
					p_overflow_x.SetExt(ICSSproperty::OVERFLOW_VISIBLE);
				}
			}
			else if(key == L"overflow-y")
			{
				if(val == L"auto")
				{
					p_overflow_y.SetExt(ICSSproperty::OVERFLOW_AUTO);
				}
				else if(val == L"hidden")
				{
					p_overflow_y.SetExt(ICSSproperty::OVERFLOW_HIDDEN);
				}
				else if(val == L"scroll")
				{
					p_overflow_y.SetExt(ICSSproperty::OVERFLOW_SCROLL);
				}
				else if(val == L"visible")
				{
					p_overflow_y.SetExt(ICSSproperty::OVERFLOW_VISIBLE);
				}
			}
			else if(key == L"overflow")
			{
				if(val == L"auto")
				{
					p_overflow_y.SetExt(ICSSproperty::OVERFLOW_AUTO);
				}
				else if(val == L"hidden")
				{
					p_overflow_y.SetExt(ICSSproperty::OVERFLOW_HIDDEN);
				}
				else if(val == L"scroll")
				{
					p_overflow_y.SetExt(ICSSproperty::OVERFLOW_SCROLL);
				}
				else if(val == L"visible")
				{
					p_overflow_y.SetExt(ICSSproperty::OVERFLOW_VISIBLE);
				}
				p_overflow_x = p_overflow_y;
			}
			else if(key == L"margin-left")
			{
				ParseValue(&p_margin_left, val);
			}
			else if(key == L"margin-right")
			{
				ParseValue(&p_margin_right, val);
			}
			else if(key == L"margin-top")
			{
				ParseValue(&p_margin_top, val);
			}
			else if(key == L"margin-bottom")
			{
				ParseValue(&p_margin_bottom, val);
			}
			else if(key == L"margin")
			{
				// 1px				partcount = 1
				// 1px 5px
				// 1px 5px 7px
				// 1px 5px 7px 9px
				UINT l = val.length();
				UINT partcount = 1;
				bool issp = true;
				UINT parts[4] = {0};
				for(UINT i = 0; i < l; i++)
				{
					if(val[i] == L' ')
					{
						if(!issp)
						{
							issp = true;
							partcount++;
						}
					}
					else
					{
						if(issp)
						{
							issp = false;
							parts[partcount - 1] = i;
						}
					}
				}
				switch(partcount)
				{
				case 1:
					ParseValue(&p_margin_bottom, val);
					p_margin_top = p_margin_left = p_margin_right = p_margin_bottom;
					break;
				case 2:
					ParseValue(&p_margin_top, val);
					ParseValue(&p_margin_right, StringW(&(val.c_str()[parts[1]])));
					p_margin_bottom = p_margin_top;
					p_margin_left = p_margin_right;
					break;
				case 3:
					ParseValue(&p_margin_top, val);
					ParseValue(&p_margin_right, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_margin_bottom, StringW(&(val.c_str()[parts[2]])));
					p_margin_left = p_margin_right;
					break;
				default: // 4 or more
					ParseValue(&p_margin_top, val);
					ParseValue(&p_margin_right, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_margin_bottom, StringW(&(val.c_str()[parts[2]])));
					ParseValue(&p_margin_left, StringW(&(val.c_str()[parts[3]])));
				}
			}
			else if(key == L"padding-left")
			{
				ParseValue(&p_padding_left, val);
			}
			else if(key == L"padding-right")
			{
				ParseValue(&p_padding_right, val);
			}
			else if(key == L"padding-top")
			{
				ParseValue(&p_padding_top, val);
			}
			else if(key == L"padding-bottom")
			{
				ParseValue(&p_padding_bottom, val);
			}
			else if(key == L"padding")
			{
				// 1px				partcount = 1
				// 1px 5px
				// 1px 5px 7px
				// 1px 5px 7px 9px
				UINT l = val.length();
				UINT partcount = 1;
				bool issp = true;
				UINT parts[4] = {0};
				for(UINT i = 0; i < l; i++)
				{
					if(val[i] == L' ')
					{
						if(!issp)
						{
							issp = true;
							partcount++;
						}
					}
					else
					{
						if(issp)
						{
							issp = false;
							parts[partcount - 1] = i;
						}
					}
				}
				switch(partcount)
				{
				case 1:
					ParseValue(&p_padding_bottom, val);
					p_padding_top = p_padding_left = p_padding_right = p_padding_bottom;
					break;
				case 2:
					ParseValue(&p_padding_top, val);
					ParseValue(&p_padding_right, StringW(&(val.c_str()[parts[1]])));
					p_padding_bottom = p_padding_top;
					p_padding_left = p_padding_right;
					break;
				case 3:
					ParseValue(&p_padding_top, val);
					ParseValue(&p_padding_right, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_padding_bottom, StringW(&(val.c_str()[parts[2]])));
					p_padding_left = p_padding_right;
					break;
				default: // 4 or more
					ParseValue(&p_padding_top, val);
					ParseValue(&p_padding_right, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_padding_bottom, StringW(&(val.c_str()[parts[2]])));
					ParseValue(&p_padding_left, StringW(&(val.c_str()[parts[3]])));
				}
			}
			else if(key == L"top")
			{
				ParseValue(&p_top, val);
			}
			else if(key == L"left")
			{
				ParseValue(&p_left, val);
			}
			else if(key == L"right")
			{
				ParseValue(&p_right, val);
			}
			else if(key == L"bottom")
			{
				ParseValue(&p_bottom, val);
			}
			else if(key == L"width")
			{
				ParseValue(&p_width, val);
			}
			else if(key == L"height")
			{
				ParseValue(&p_height, val);
			}
			else if(key == L"min-width")
			{
				ParseValue(&p_min_width, val);
			}
			else if(key == L"min-height")
			{
				ParseValue(&p_min_height, val);
			}
			else if(key == L"max-width")
			{
				ParseValue(&p_max_width, val);
			}
			else if(key == L"max-height")
			{
				ParseValue(&p_max_height, val);
			}
			else if(key == L"position")
			{
				if(val == L"static")
				{
					p_position.SetExt(ICSSproperty::POSITION_STATIC);
				}
				else if(val == L"absolute")
				{
					p_position.SetExt(ICSSproperty::POSITION_ABSOLUTE);
				}
				else if(val == L"relative")
				{
					p_position.SetExt(ICSSproperty::POSITION_RELATIVE);
				}
				else if(val == L"fixed")
				{
					p_position.SetExt(ICSSproperty::POSITION_FIXED);
				}
			}
			else if(key == L"visibility")
			{
				if(val == L"visible")
				{
					p_visibility.SetExt(ICSSproperty::VISIBILITY_VISIBLE);
				}
				else if(val == L"hidden")
				{
					p_visibility.SetExt(ICSSproperty::VISIBILITY_HIDDEN);
				}
			}
			else if(key == L"float")
			{
				if(val == L"none")
				{
					p_float.SetExt(ICSSproperty::FLOAT_NONE);
				}
				else if(val == L"left")
				{
					p_float.SetExt(ICSSproperty::FLOAT_LEFT);
				}
				else if(val == L"right")
				{
					p_float.SetExt(ICSSproperty::FLOAT_RIGHT);
				}
			}
			else if(key == L"background-image")
			{
				p_background_image.Set(val);
			}
			else if(key == L"background-color")
			{
				ParseColor(&p_background_color, val);
			}
			else if(key == L"background-position-x")
			{
				ParseValue(&p_background_position_x, val);
			}
			else if(key == L"background-position-y")
			{
				ParseValue(&p_background_position_y, val);
			}
			else if(key == L"background-attachment")
			{
				if(val == L"fixed")
				{
					p_background_attachment.SetExt(ICSSproperty::BACKGROUND_ATTACHMENT_FIXED);
				}
				else if(val == L"local")
				{
					p_background_attachment.SetExt(ICSSproperty::BACKGROUND_ATTACHMENT_LOCAL);
				}
				else if(val == L"scroll")
				{
					p_background_attachment.SetExt(ICSSproperty::BACKGROUND_ATTACHMENT_SCROLL);
				}
			}
			else if(key == L"background-repeat")
			{
				if(val == L"no-repeat")
				{
					p_background_repeat.SetExt(ICSSproperty::BACKGROUND_REPEAT_NO_REPEAT);
				}
				else if(val == L"repeat-x")
				{
					p_background_repeat.SetExt(ICSSproperty::BACKGROUND_REPEAT_REPEAT_X);
				}
				else if(val == L"repeat-y")
				{
					p_background_repeat.SetExt(ICSSproperty::BACKGROUND_REPEAT_REPEAT_Y);
				}
				else if(val == L"repeat")
				{
					p_background_repeat.SetExt(ICSSproperty::BACKGROUND_REPEAT_REPEAT);
				}
			}
			else if(key == L"background-size-x")
			{
				ParseValue(&p_background_size_x, val);
			}
			else if(key == L"background-size-y")
			{
				ParseValue(&p_background_size_y, val);
			}
			else if(key == L"border-top-style")
			{
				if(val == L"none")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_NONE);
				}
				else if(val == L"inset")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_INSET);
				}
				else if(val == L"outset")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_OUTSET);
				}
				else if(val == L"dashed")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_DASHED);
				}
				else if(val == L"dotted")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_DOTTED);
				}
				else if(val == L"double")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_DOUBLE);
				}
				else if(val == L"groove")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_GROOVE);
				}
				else if(val == L"ridge")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_RIDGE);
				}
				else if(val == L"solid")
				{
					p_border_top_style.SetExt(ICSSproperty::BORDER_STYLE_SOLID);
				}
			}
			else if(key == L"border-top-width")
			{
				ParseValue(&p_border_top_width, val);
			}
			else if(key == L"border-top-left-radius")
			{
				ParseValue(&p_border_top_left_radius, val);
			}
			else if(key == L"border-top-right-radius")
			{
				ParseValue(&p_border_top_right_radius, val);
			}
			else if(key == L"border-radius")
			{
				// 1px				partcount = 1
				// 1px 5px
				// 1px 5px 7px
				// 1px 5px 7px 9px
				UINT l = val.length();
				UINT partcount = 1;
				bool issp = true;
				UINT parts[4] = {0};
				for(UINT i = 0; i < l; i++)
				{
					if(val[i] == L' ')
					{
						if(!issp)
						{
							issp = true;
							partcount++;
						}
					}
					else
					{
						if(issp)
						{
							issp = false;
							parts[partcount - 1] = i;
						}
					}
				}
				switch(partcount)
				{
				case 1:
					ParseValue(&p_border_top_left_radius, val);
					p_border_bottom_right_radius = p_border_top_right_radius = p_border_bottom_left_radius = p_border_top_left_radius;
					break;
				case 2:
					ParseValue(&p_border_top_left_radius, val);
					ParseValue(&p_border_top_right_radius, StringW(&(val.c_str()[parts[1]])));
					p_border_bottom_left_radius = p_border_top_left_radius;
					p_border_bottom_right_radius = p_border_top_right_radius;
					break;
				case 3:
					ParseValue(&p_border_top_left_radius, val);
					ParseValue(&p_border_top_right_radius, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_border_bottom_right_radius, StringW(&(val.c_str()[parts[2]])));
					p_border_bottom_left_radius = p_border_bottom_right_radius;
					break;
				default: // 4 or more
					ParseValue(&p_border_top_left_radius, val);
					ParseValue(&p_border_top_right_radius, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_border_bottom_right_radius, StringW(&(val.c_str()[parts[2]])));
					ParseValue(&p_border_bottom_left_radius, StringW(&(val.c_str()[parts[3]])));
				}
			}
			/*else if(key == L"border-method")
			{
				// 1px				partcount = 1
				// 1px 5px
				// 1px 5px 7px
				// 1px 5px 7px 9px
				UINT l = val.length();
				UINT partcount = 1;
				bool issp = true;
				UINT parts[4] = {0};
				for(UINT i = 0; i < l; i++)
				{
					if(val[i] == L' ')
					{
						if(!issp)
						{
							issp = true;
							partcount++;
						}
					}
					else
					{
						if(issp)
						{
							issp = false;
							parts[partcount - 1] = i;
						}
					}
				}
				switch(partcount)
				{
				case 1:
					if(val == L"radius")
					{
						p_border_top_left_method.SetExt(ICSSproperty::BORDER_METHOD_RADIUS);
					}
					else if(val == L"cut")
					{
						p_border_top_left_method.SetExt(ICSSproperty::BORDER_METHOD_CUT);
					}
					p_border_bottom_right_method = p_border_top_right_method = p_border_bottom_left_method = p_border_top_left_method;
					break;
				case 2:
					if(val == L"radius")
					{
						p_border_top_left_method.SetExt(ICSSproperty::BORDER_METHOD_RADIUS);
					}
					else if(val == L"cut")
					{
						p_border_top_left_method.SetExt(ICSSproperty::BORDER_METHOD_CUT);
					}
					ParseValue(&p_border_top_left_method, val);
					ParseValue(&p_border_top_right_method, StringW(&(val.c_str()[parts[1]])));
					p_border_bottom_left_method = p_border_top_left_method;
					p_border_bottom_right_method = p_border_top_right_method;
					break;
				case 3:
					ParseValue(&p_border_top_left_method, val);
					ParseValue(&p_border_top_right_method, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_border_bottom_right_method, StringW(&(val.c_str()[parts[2]])));
					p_border_bottom_left_method = p_border_bottom_right_method;
					break;
				default: // 4 or more
					ParseValue(&p_border_top_left_method, val);
					ParseValue(&p_border_top_right_method, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_border_bottom_right_method, StringW(&(val.c_str()[parts[2]])));
					ParseValue(&p_border_bottom_left_method, StringW(&(val.c_str()[parts[3]])));
				}
			}*/
			else if(key == L"border-top-right-method")
			{
				if(val == L"radius")
				{
					p_border_top_right_method.SetExt(ICSSproperty::BORDER_METHOD_RADIUS);
				}
				else if(val == L"cut")
				{
					p_border_top_right_method.SetExt(ICSSproperty::BORDER_METHOD_CUT);
				}
			}
			else if(key == L"border-top-left-method")
			{
				if(val == L"radius")
				{
					p_border_top_left_method.SetExt(ICSSproperty::BORDER_METHOD_RADIUS);
				}
				else if(val == L"cut")
				{
					p_border_top_left_method.SetExt(ICSSproperty::BORDER_METHOD_CUT);
				}
			}
			else if(key == L"border-bottom-right-method")
			{
				if(val == L"radius")
				{
					p_border_bottom_right_method.SetExt(ICSSproperty::BORDER_METHOD_RADIUS);
				}
				else if(val == L"cut")
				{
					p_border_bottom_right_method.SetExt(ICSSproperty::BORDER_METHOD_CUT);
				}
			}
			else if(key == L"border-bottom-left-method")
			{
				if(val == L"radius")
				{
					p_border_bottom_left_method.SetExt(ICSSproperty::BORDER_METHOD_RADIUS);
				}
				else if(val == L"cut")
				{
					p_border_bottom_left_method.SetExt(ICSSproperty::BORDER_METHOD_CUT);
				}
			}
			else if(key == L"border-right-style")
			{
				if(val == L"none")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_NONE);
				}
				else if(val == L"inset")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_INSET);
				}
				else if(val == L"outset")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_OUTSET);
				}
				else if(val == L"dashed")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_DASHED);
				}
				else if(val == L"dotted")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_DOTTED);
				}
				else if(val == L"double")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_DOUBLE);
				}
				else if(val == L"groove")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_GROOVE);
				}
				else if(val == L"ridge")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_RIDGE);
				}
				else if(val == L"solid")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_SOLID);
				}
			}
			else if(key == L"border-right-width")
			{
				ParseValue(&p_border_right_width, val);
			}
			else if(key == L"border-left-style")
			{
				if(val == L"none")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_NONE);
				}
				else if(val == L"inset")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_INSET);
				}
				else if(val == L"outset")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_OUTSET);
				}
				else if(val == L"dashed")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_DASHED);
				}
				else if(val == L"dotted")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_DOTTED);
				}
				else if(val == L"double")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_DOUBLE);
				}
				else if(val == L"groove")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_GROOVE);
				}
				else if(val == L"ridge")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_RIDGE);
				}
				else if(val == L"solid")
				{
					p_border_left_style.SetExt(ICSSproperty::BORDER_STYLE_SOLID);
				}
			}
			else if(key == L"border-style")
			{
				if(val == L"none")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_NONE);
				}
				else if(val == L"inset")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_INSET);
				}
				else if(val == L"outset")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_OUTSET);
				}
				else if(val == L"dashed")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_DASHED);
				}
				else if(val == L"dotted")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_DOTTED);
				}
				else if(val == L"double")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_DOUBLE);
				}
				else if(val == L"groove")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_GROOVE);
				}
				else if(val == L"ridge")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_RIDGE);
				}
				else if(val == L"solid")
				{
					p_border_right_style.SetExt(ICSSproperty::BORDER_STYLE_SOLID);
				}
				p_border_left_style = p_border_top_style = p_border_bottom_style = p_border_right_style;
			}
			else if(key == L"border-left-width")
			{
				ParseValue(&p_border_left_width, val);
			}
			else if(key == L"border-bottom-style")
			{
				if(val == L"none")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_NONE);
				}
				else if(val == L"inset")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_INSET);
				}
				else if(val == L"outset")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_OUTSET);
				}
				else if(val == L"dashed")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_DASHED);
				}
				else if(val == L"dotted")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_DOTTED);
				}
				else if(val == L"double")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_DOUBLE);
				}
				else if(val == L"groove")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_GROOVE);
				}
				else if(val == L"ridge")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_RIDGE);
				}
				else if(val == L"solid")
				{
					p_border_bottom_style.SetExt(ICSSproperty::BORDER_STYLE_SOLID);
				}
			}
			else if(key == L"border-bottom-width")
			{
				ParseValue(&p_border_bottom_width, val);
			}
			else if(key == L"border-width")
			{
				// 1px				partcount = 1
				// 1px 5px
				// 1px 5px 7px
				// 1px 5px 7px 9px
				UINT l = val.length();
				UINT partcount = 1;
				bool issp = true;
				UINT parts[4] = {0};
				for(UINT i = 0; i < l; i++)
				{
					if(val[i] == L' ')
					{
						if(!issp)
						{
							issp = true;
							partcount++;
						}
					}
					else
					{
						if(issp)
						{
							issp = false;
							parts[partcount - 1] = i;
						}
					}
				}
				switch(partcount)
				{
				case 1:
					ParseValue(&p_border_top_width, val);
					p_border_left_width = p_border_right_width = p_border_bottom_width = p_border_top_width;
					break;
				case 2:
					ParseValue(&p_border_top_width, val);
					ParseValue(&p_border_right_width, StringW(&(val.c_str()[parts[1]])));
					p_border_bottom_width = p_border_top_width;
					p_border_left_width = p_border_right_width;
					break;
				case 3:
					ParseValue(&p_border_top_width, val);
					ParseValue(&p_border_right_width, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_border_bottom_width, StringW(&(val.c_str()[parts[2]])));
					p_border_left_width = p_border_right_width;
					break;
				default: // 4 or more
					ParseValue(&p_border_top_width, val);
					ParseValue(&p_border_right_width, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_border_bottom_width, StringW(&(val.c_str()[parts[2]])));
					ParseValue(&p_border_left_width, StringW(&(val.c_str()[parts[3]])));
				}
			}
			else if(key == L"border-width")
			{
				// 1px				partcount = 1
				// 1px 5px
				// 1px 5px 7px
				// 1px 5px 7px 9px
				UINT l = val.length();
				UINT partcount = 1;
				bool issp = true;
				UINT parts[4] = {0};
				for(UINT i = 0; i < l; i++)
				{
					if(val[i] == L' ')
					{
						if(!issp)
						{
							issp = true;
							partcount++;
						}
					}
					else
					{
						if(issp)
						{
							issp = false;
							parts[partcount - 1] = i;
						}
					}
				}
				switch(partcount)
				{
				case 1:
					ParseValue(&p_border_top_left_radius, val);
					p_border_top_right_radius = p_border_bottom_left_radius = p_border_bottom_right_radius = p_border_top_left_radius;
					break;
				case 2:
					ParseValue(&p_border_top_left_radius, val);
					ParseValue(&p_border_top_right_radius, StringW(&(val.c_str()[parts[1]])));
					p_border_bottom_left_radius = p_border_top_left_radius;
					p_border_bottom_right_radius = p_border_top_right_radius;
					break;
				case 3:
					ParseValue(&p_border_top_left_radius, val);
					ParseValue(&p_border_top_right_radius, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_border_bottom_right_radius, StringW(&(val.c_str()[parts[2]])));
					p_border_bottom_left_radius = p_border_top_left_radius;
					break;
				default: // 4 or more
					ParseValue(&p_border_top_left_radius, val);
					ParseValue(&p_border_top_right_radius, StringW(&(val.c_str()[parts[1]])));
					ParseValue(&p_border_bottom_right_radius, StringW(&(val.c_str()[parts[2]])));
					ParseValue(&p_border_bottom_left_radius, StringW(&(val.c_str()[parts[3]])));
				}
			}
			else if(key == L"border-bottom-left-radius")
			{
				ParseValue(&p_border_bottom_left_radius, val);
			}
			else if(key == L"border-bottom-right-radius")
			{
				ParseValue(&p_border_bottom_right_radius, val);
			}
			else if(key == L"font-name")
			{
				p_font_name.Set(val);
			}
			else if(key == L"font-size")
			{
				ParseValue(&p_font_size, val);
			}
			else if(key == L"font-style")
			{
				if(val == L"inherit")
				{
					p_font_style.SetExt(ICSSproperty::FONT_STYLE_INHERIT);
				}
				else if(val == L"italic")
				{
					p_font_style.SetExt(ICSSproperty::FONT_STYLE_ITALIC);
				}
				else if(val == L"normal")
				{
					p_font_style.SetExt(ICSSproperty::FONT_STYLE_NORMAL);
				}
			}
			else if(key == L"-gui-text-appearance")
			{
				if(val == L"multi-line")
				{
					p__gui_text_appearance.SetExt(ICSSproperty::_GUI_TEXT_APPEARANCE_MULTI_LINE);
				}
				else if(val == L"one-line")
				{
					p__gui_text_appearance.SetExt(ICSSproperty::_GUI_TEXT_APPEARANCE_ONE_LINE);
				}
			}
			else if(key == L"font-weight")
			{
				if(val == L"bold")
				{
					p_font_weight.SetExt(ICSSproperty::FONT_WEIGHT_BOLD);
				}
				else if(val == L"inherit")
				{
					p_font_weight.SetExt(ICSSproperty::FONT_WEIGHT_INHERIT);
				}
				else if(val == L"normal")
				{
					p_font_weight.SetExt(ICSSproperty::FONT_WEIGHT_NORMAL);
				}
			}
			else if(key == L"text-align")
			{
				if(val == L"inherit")
				{
					p_text_align.SetExt(ICSSproperty::TEXT_ALIGN_INHERIT);
				}
				else if(val == L"left")
				{
					p_text_align.SetExt(ICSSproperty::TEXT_ALIGN_LEFT);
				}
				else if(val == L"right")
				{
					p_text_align.SetExt(ICSSproperty::TEXT_ALIGN_RIGHT);
				}
				else if(val == L"center")
				{
					p_text_align.SetExt(ICSSproperty::TEXT_ALIGN_CENTER);
				}
			}
			else if(key == L"text-decoration-line")
			{
				if(val == L"none")
				{
					p_text_decoration_line.SetExt(ICSSproperty::TEXT_DECORATION_LINE_NONE);
				}
				else if(val == L"underline")
				{
					p_text_decoration_line.SetExt(ICSSproperty::TEXT_DECORATION_LINE_UNDERLINE);
				}
				else if(val == L"line-through")
				{
					p_text_decoration_line.SetExt(ICSSproperty::TEXT_DECORATION_LINE_LINE_THROUGH);
				}
				else if(val == L"overline")
				{
					p_text_decoration_line.SetExt(ICSSproperty::TEXT_DECORATION_LINE_OVERLINE);
				}
			}
			else if(key == L"border-color")
			{
				// 1px				partcount = 1
				// 1px 5px
				// 1px 5px 7px
				// 1px 5px 7px 9px
				UINT l = val.length();
				UINT partcount = 1;
				bool issp = true;
				UINT parts[4] = {0};
				for(UINT i = 0; i < l; i++)
				{
					if(val[i] == L' ')
					{
						if(!issp)
						{
							issp = true;
							partcount++;
						}
					}
					else
					{
						if(issp)
						{
							issp = false;
							parts[partcount - 1] = i;
						}
					}
				}
				switch(partcount)
				{
				case 1:
					ParseColor(&p_border_top_color, val);
					p_border_left_color = p_border_bottom_color = p_border_right_color = p_border_top_color;
					break;
				case 2:
					ParseColor(&p_border_top_color, val);
					ParseColor(&p_border_right_color, StringW(&(val.c_str()[parts[1]])));
					p_border_bottom_color = p_border_top_color;
					p_border_left_color = p_border_right_color;
					break;
				case 3:
					ParseColor(&p_border_top_color, val);
					ParseColor(&p_border_right_color, StringW(&(val.c_str()[parts[1]])));
					ParseColor(&p_border_bottom_color, StringW(&(val.c_str()[parts[2]])));
					p_border_bottom_color = p_border_top_color;
					break;
				default: // 4 or more
					ParseColor(&p_border_top_color, val);
					ParseColor(&p_border_top_right_radius, StringW(&(val.c_str()[parts[1]])));
					ParseColor(&p_border_bottom_color, StringW(&(val.c_str()[parts[2]])));
					ParseColor(&p_border_left_color, StringW(&(val.c_str()[parts[3]])));
				}
			}
			else if(key == L"border-top-color")
			{
				ParseColor(&p_border_top_color, val);
			}
			else if(key == L"border-left-color")
			{
				ParseColor(&p_border_left_color, val);
			}
			else if(key == L"border-bottom-color")
			{
				ParseColor(&p_border_bottom_color, val);
			}
			else if(key == L"border-right-color")
			{
				ParseColor(&p_border_right_color, val);
			}
			else if(key == L"-gui-text-cursor")
			{
				if(val == L"hide")
				{
					p__gui_text_cursor.SetExt(ICSSproperty::_GUI_TEXT_CURSOR_HIDE);
				}
				else if(val == L"show")
				{
					p__gui_text_cursor.SetExt(ICSSproperty::_GUI_TEXT_CURSOR_SHOW);
				}
			}
			else if(key == L"-gui-text-format")
			{
				if(val == L"default")
				{
					p__gui_text_format.SetExt(ICSSproperty::_GUI_TEXT_FORMAT_DEFAULT);
				}
				else if(val == L"preformatted")
				{
					p__gui_text_format.SetExt(ICSSproperty::_GUI_TEXT_FORMAT_PREFORMATTED);
				}
				
			}
		}

		void CCSSstyle::ParseColor(ICSSproperty * clr, const StringW & val)
		{
			if(val.c_str()[0] == L'#') // hex color
			{
				UINT c;
				WCHAR str[9];
				str[8] = 0;
				if(val.length() == 4)
				{
					str[0] = str[1] = val[1];
					str[2] = str[3] = val[2];
					str[4] = str[5] = val[3];
					str[6] = str[7] = L'F';
				}
				else if(val.length() == 7)
				{
					memcpy(str, &(val.c_str()[1]), sizeof(WCHAR) * 6);
					str[6] = str[7] = L'F';
				}
				else if(val.length() == 9)
				{
					memcpy(str, &(val.c_str()[1]), sizeof(WCHAR) * 8);
				}
				else
				{
					str[0] = str[1] = str[2] = str[3] = str[4] = str[5] = L'0';
					str[6] = str[7] = L'F';
				}
				swscanf_s(str, L"%x", &c);
				clr->Set((int)c);
			}
			else
			{
				wprintf(L"[Error]: Unsupported color type in css: '%s'\n", val.c_str());
			}
		}

		void CCSSstyle::ParseValue(ICSSproperty * clr, const StringW & val)
		{
			float p;
			WCHAR a = 0, b;
			StringW _val = val;
			UINT l = val.length();
			UINT i;
			for(i = 0; i < l; i++)
			{
				if(!((val[i] >= L'0' && val[i] <= L'9') || val[i] == L'.'))
				{
					a = val[i];
					if(a == L'e')
					{
						_val[i] = L'!';
					}
					break;
				}
			}

			int n = swscanf_s(_val.c_str(), L"%f%c%c", &p, &a, sizeof(a), &b, sizeof(b));
			clr->Set(p);
			if(n == 1)
			{
				clr->SetDim(ICSSproperty::DIM_NONE);
			}
			else if(n == 2)
			{
				if(a == L'%')
				{
					clr->SetDim(ICSSproperty::DIM_PC);
				}
				else
				{
					wprintf(L"[Error]: Unknown dimension '%c' in css string\n", a);
				}
			}
			else if(n == 3)
			{
				if(a == L'!' && b == L'm')
				{
					clr->SetDim(ICSSproperty::DIM_EM);
				}
				else if(a == L'm' && b == L'm')
				{
					clr->SetDim(ICSSproperty::DIM_MM);
				}
				else if(a == L'p' && b == L't')
				{
					clr->SetDim(ICSSproperty::DIM_PT);
				}
				else if(a == L'p' && b == L'x')
				{
					clr->SetDim(ICSSproperty::DIM_PX);
				}
				else
				{
					wprintf(L"[Error]: Unknown dimension '%c%c' in css string\n", a, b);
				}
			}
		}
	
		void ICSS::AddFile(const WCHAR * szFile)
		{
			StringW file(StringW(GetGUI()->GetResourceDir()) + L"/css/" + szFile);
			FILE * pF;
			_wfopen_s(&pF, file.c_str(), L"rt, ccs=UTF-8");
			if(!pF)
			{
				wprintf(L"[Error]: Unable to open \"%s\" file for reading\n", file.c_str());
				return;
			}

			fseek(pF, 0, SEEK_END);
			UINT fs = ftell(pF);
			fseek(pF, 0, SEEK_SET);
			wchar_t * str = new wchar_t[fs + 1];
			if(!str)
			{
				wprintf(L"[Error]: Unable to allocate memory\n");
				return;
			}
			str[fread(str, sizeof(wchar_t), fs, pF)] = 0;
			fclose(pF);
			ICSSstyleSet css(this);
			css.LoadFromString(str);
			m_styleSets[szFile] = css;
			m_styleOrder.push_back(&m_styleSets[szFile]);
			SX_SAFE_DELETE_A(str);
			
		}


		CCSSstyle::CCSSstyle():
			p_display(ICSSproperty::TYPE_DISPLAY),
			p_visibility(ICSSproperty::TYPE_VISIBILITY),
			p_float(ICSSproperty::TYPE_FLOAT),
			p_position(ICSSproperty::TYPE_POSITION),
			p_background_attachment(ICSSproperty::TYPE_BACKGROUND_ATTACHMENT),
			p_background_repeat(ICSSproperty::TYPE_BACKGROUND_REPEAT),
			p_border_top_style(ICSSproperty::TYPE_BORDER_STYLE),
			p_border_right_style(ICSSproperty::TYPE_BORDER_STYLE),
			p_border_bottom_style(ICSSproperty::TYPE_BORDER_STYLE),
			p_border_left_style(ICSSproperty::TYPE_BORDER_STYLE),
			p_cursor(ICSSproperty::TYPE_CURSOR),
			p_direction(ICSSproperty::TYPE_DIRECTION),
			p_font_style(ICSSproperty::TYPE_FONT_STYLE),
			p_font_weight(ICSSproperty::TYPE_FONT_WEIGHT),
			p_text_align(ICSSproperty::TYPE_TEXT_ALIGN),
			p_text_decoration_line(ICSSproperty::TYPE_TEXT_DECORATION_LINE),
			p_overflow_x(ICSSproperty::TYPE_OVERFLOW),
			p_overflow_y(ICSSproperty::TYPE_OVERFLOW),
			p_border_top_left_method(ICSSproperty::TYPE_BORDER_METHOD),
			p_border_top_right_method(ICSSproperty::TYPE_BORDER_METHOD),
			p_border_bottom_left_method(ICSSproperty::TYPE_BORDER_METHOD),
			p_border_bottom_right_method(ICSSproperty::TYPE_BORDER_METHOD),
			p__gui_text_appearance(ICSSproperty::TYPE__GUI_TEXT_APPEARANCE),
			p__gui_text_cursor(ICSSproperty::TYPE__GUI_TEXT_CURSOR),
			p__gui_text_format(ICSSproperty::TYPE__GUI_TEXT_FORMAT)
		{
			p_display.SetFlags(ICSSproperty::FLAG_UPDATE_STRUCTURE);

			p_color.SetFlags(ICSSproperty::FLAG_UPDATE_VIEW);

			p_cursor.SetFlags(ICSSproperty::FLAG_UPDATE_CURSOR);

			p_direction.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_opacity.SetFlags(ICSSproperty::FLAG_UPDATE_VIEW);

			p_z_index.SetFlags(0);

			p_overflow_x.SetFlags(ICSSproperty::FLAG_UPDATE_VIEW);
			p_overflow_y.SetFlags(ICSSproperty::FLAG_UPDATE_VIEW);

			p_margin_left.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_margin_right.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_margin_top.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_margin_bottom.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_padding_left.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_padding_right.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_padding_top.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_padding_bottom.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_top.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_left.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_right.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_bottom.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_width.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_height.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_max_width.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_max_height.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_min_width.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_min_height.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_position.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_visibility.SetFlags(ICSSproperty::FLAG_UPDATE_VIEW);

			p_float.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_background_image.SetFlags(ICSSproperty::FLAG_UPDATE_BACKGROUNDLAYOUT);
			p_background_color.SetFlags(ICSSproperty::FLAG_UPDATE_BACKGROUNDVIEW);
			p_background_position_x.SetFlags(ICSSproperty::FLAG_UPDATE_BACKGROUNDVIEW);
			p_background_position_y.SetFlags(ICSSproperty::FLAG_UPDATE_BACKGROUNDVIEW);
			p_background_attachment.SetFlags(ICSSproperty::FLAG_UPDATE_BACKGROUNDLAYOUT); //
			p_background_repeat.SetFlags(ICSSproperty::FLAG_UPDATE_BACKGROUNDVIEW); //
			p_background_size_x.SetFlags(ICSSproperty::FLAG_UPDATE_BACKGROUNDLAYOUT);
			p_background_size_y.SetFlags(ICSSproperty::FLAG_UPDATE_BACKGROUNDLAYOUT);


			p_border_top_style.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT); //
			p_border_top_width.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_top_color.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERVIEW);
			p_border_top_left_radius.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_top_right_radius.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_top_left_method.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_top_right_method.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);

			p_border_right_style.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT); //
			p_border_right_width.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_right_color.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERVIEW);

			p_border_bottom_style.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT); //
			p_border_bottom_width.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_bottom_color.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERVIEW);
			p_border_bottom_left_radius.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_bottom_right_radius.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_bottom_left_method.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_bottom_right_method.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);

			p_border_left_style.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT); //
			p_border_left_width.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERLAYOUT);
			p_border_left_color.SetFlags(ICSSproperty::FLAG_UPDATE_BORDERVIEW);


			p_font_name.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_font_size.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_font_style.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_font_weight.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p_text_align.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p_text_decoration_line.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);

			p__gui_text_appearance.SetFlags(ICSSproperty::FLAG_UPDATE_LAYOUT);
			p__gui_text_cursor.SetFlags(0);
			p__gui_text_format.SetFlags(0);




			display = &p_display;
			color = &p_color;
			cursor = &p_cursor;
			direction = &p_direction;
			opacity = &p_opacity;
			z_index = &p_z_index;
			overflow_x = &p_overflow_x;
			overflow_y = &p_overflow_y;
			margin_left = &p_margin_left;
			margin_right = &p_margin_right;
			margin_top = &p_margin_top;
			margin_bottom = &p_margin_bottom;
			padding_left = &p_padding_left;
			padding_right = &p_padding_right;
			padding_top = &p_padding_top;
			padding_bottom = &p_padding_bottom;
			top = &p_top;
			left = &p_left;
			right = &p_right;
			bottom = &p_bottom;
			width = &p_width;
			height = &p_height;
			max_width = &p_max_width;
			max_height = &p_max_height;
			min_width = &p_min_width;
			min_height = &p_min_height;
			position = &p_position;
			visibility = &p_visibility;
			_float = &p_float;
			background_image = &p_background_image;
			background_color = &p_background_color;
			background_position_x = &p_background_position_x;
			background_position_y = &p_background_position_y;
			background_attachment = &p_background_attachment;
			background_repeat = &p_background_repeat;
			background_size_x = &p_background_size_x;
			background_size_y = &p_background_size_y;
			border_top_style = &p_border_top_style;
			border_top_width = &p_border_top_width;
			border_top_color = &p_border_top_color;
			border_top_left_radius = &p_border_top_left_radius;
			border_top_right_radius = &p_border_top_right_radius;
			border_top_left_method = &p_border_top_left_method;
			border_top_right_method = &p_border_top_right_method;
			border_right_style = &p_border_right_style;
			border_right_width = &p_border_right_width;
			border_right_color = &p_border_right_color;
			border_bottom_style = &p_border_bottom_style;
			border_bottom_width = &p_border_bottom_width;
			border_bottom_color = &p_border_bottom_color;
			border_bottom_left_radius = &p_border_bottom_left_radius;
			border_bottom_right_radius = &p_border_bottom_right_radius;
			border_bottom_left_method = &p_border_bottom_left_method;
			border_bottom_right_method = &p_border_bottom_right_method;
			border_left_style = &p_border_left_style;
			border_left_width = &p_border_left_width;
			border_left_color = &p_border_left_color;
			font_name = &p_font_name;
			font_size = &p_font_size;
			font_style = &p_font_style;
			font_weight = &p_font_weight;
			text_align = &p_text_align;
			text_decoration_line = &p_text_decoration_line;
			_gui_text_appearance = &p__gui_text_appearance;
			_gui_text_cursor = &p__gui_text_cursor;
			_gui_text_format = &p__gui_text_format;

		};

		UINT CCSSstyle::GetChangesFlags()
		{
			return((p_display.IsChanged() ? p_display.GetFlags() : 0) |

				(p_color.IsChanged() ? p_color.GetFlags() : 0) |

				(p_cursor.IsChanged() ? p_cursor.GetFlags() : 0) |

				(p_direction.IsChanged() ? p_direction.GetFlags() : 0) |

				(p_opacity.IsChanged() ? p_opacity.GetFlags() : 0) |

				(p_z_index.IsChanged() ? p_z_index.GetFlags() : 0) |

				(p_overflow_x.IsChanged() ? p_overflow_x.GetFlags() : 0) |
				(p_overflow_y.IsChanged() ? p_overflow_y.GetFlags() : 0) |

				(p_margin_left.IsChanged() ? p_margin_left.GetFlags() : 0) |
				(p_margin_right.IsChanged() ? p_margin_right.GetFlags() : 0) |
				(p_margin_top.IsChanged() ? p_margin_top.GetFlags() : 0) |
				(p_margin_bottom.IsChanged() ? p_margin_bottom.GetFlags() : 0) |

				(p_padding_left.IsChanged() ? p_padding_left.GetFlags() : 0) |
				(p_padding_right.IsChanged() ? p_padding_right.GetFlags() : 0) |
				(p_padding_top.IsChanged() ? p_padding_top.GetFlags() : 0) |
				(p_padding_bottom.IsChanged() ? p_padding_bottom.GetFlags() : 0) |

				(p_top.IsChanged() ? p_top.GetFlags() : 0) |
				(p_left.IsChanged() ? p_left.GetFlags() : 0) |
				(p_right.IsChanged() ? p_right.GetFlags() : 0) |
				(p_bottom.IsChanged() ? p_bottom.GetFlags() : 0) |

				(p_width.IsChanged() ? p_width.GetFlags() : 0) |
				(p_height.IsChanged() ? p_height.GetFlags() : 0) |

				(p_max_width.IsChanged() ? p_max_width.GetFlags() : 0) |
				(p_max_height.IsChanged() ? p_max_height.GetFlags() : 0) |

				(p_min_width.IsChanged() ? p_min_width.GetFlags() : 0) |
				(p_min_height.IsChanged() ? p_min_height.GetFlags() : 0) |

				(p_position.IsChanged() ? p_position.GetFlags() : 0) |

				(p_visibility.IsChanged() ? p_visibility.GetFlags() : 0) |

				(p_float.IsChanged() ? p_float.GetFlags() : 0) |

				(p_background_image.IsChanged() ? p_background_image.GetFlags() : 0) |
				(p_background_color.IsChanged() ? p_background_color.GetFlags() : 0) |
				(p_background_position_x.IsChanged() ? p_background_position_x.GetFlags() : 0) |
				(p_background_position_y.IsChanged() ? p_background_position_y.GetFlags() : 0) |
				(p_background_attachment.IsChanged() ? p_background_attachment.GetFlags() : 0) |//
				(p_background_repeat.IsChanged() ? p_background_repeat.GetFlags() : 0) |//
				(p_background_size_x.IsChanged() ? p_background_size_x.GetFlags() : 0) |
				(p_background_size_y.IsChanged() ? p_background_size_y.GetFlags() : 0) |


				(p_border_top_style.IsChanged() ? p_border_top_style.GetFlags() : 0) |//
				(p_border_top_width.IsChanged() ? p_border_top_width.GetFlags() : 0) |
				(p_border_top_color.IsChanged() ? p_border_top_color.GetFlags() : 0) |
				(p_border_top_left_radius.IsChanged() ? p_border_top_left_radius.GetFlags() : 0) |
				(p_border_top_right_radius.IsChanged() ? p_border_top_right_radius.GetFlags() : 0) |
				(p_border_top_left_method.IsChanged() ? p_border_top_left_method.GetFlags() : 0) |
				(p_border_top_right_method.IsChanged() ? p_border_top_right_method.GetFlags() : 0) |

				(p_border_right_style.IsChanged() ? p_border_right_style.GetFlags() : 0) |//
				(p_border_right_width.IsChanged() ? p_border_right_width.GetFlags() : 0) |
				(p_border_right_color.IsChanged() ? p_border_right_color.GetFlags() : 0) |

				(p_border_bottom_style.IsChanged() ? p_border_bottom_style.GetFlags() : 0) |//
				(p_border_bottom_width.IsChanged() ? p_border_bottom_width.GetFlags() : 0) |
				(p_border_bottom_color.IsChanged() ? p_border_bottom_color.GetFlags() : 0) |
				(p_border_bottom_left_radius.IsChanged() ? p_border_bottom_left_radius.GetFlags() : 0) |
				(p_border_bottom_right_radius.IsChanged() ? p_border_bottom_right_radius.GetFlags() : 0) |
				(p_border_bottom_left_method.IsChanged() ? p_border_bottom_left_method.GetFlags() : 0) |
				(p_border_bottom_right_method.IsChanged() ? p_border_bottom_right_method.GetFlags() : 0) |

				(p_border_left_style.IsChanged() ? p_border_left_style.GetFlags() : 0) |//
				(p_border_left_width.IsChanged() ? p_border_left_width.GetFlags() : 0) |
				(p_border_left_color.IsChanged() ? p_border_left_color.GetFlags() : 0) |


				(p_font_name.IsChanged() ? p_font_name.GetFlags() : 0) |
				(p_font_size.IsChanged() ? p_font_size.GetFlags() : 0) |
				(p_font_style.IsChanged() ? p_font_style.GetFlags() : 0) |
				(p_font_weight.IsChanged() ? p_font_weight.GetFlags() : 0) |

				(p_text_align.IsChanged() ? p_text_align.GetFlags() : 0) |
				(p_text_decoration_line.IsChanged() ? p_text_decoration_line.GetFlags() : 0) |
				(p__gui_text_appearance.IsChanged() ? p__gui_text_appearance.GetFlags() : 0) |
				(p__gui_text_cursor.IsChanged() ? p__gui_text_cursor.GetFlags() : 0) |
				(p__gui_text_format.IsChanged() ? p__gui_text_format.GetFlags() : 0)
			);
		}

		void CCSSstyle::ResetChanges()
		{
			p_display.UnChange();

			p_color.UnChange();

			p_cursor.UnChange();

			p_direction.UnChange();

			p_opacity.UnChange();

			p_z_index.UnChange();

			p_overflow_x.UnChange();
			p_overflow_y.UnChange();

			p_margin_left.UnChange();
			p_margin_right.UnChange();
			p_margin_top.UnChange();
			p_margin_bottom.UnChange();

			p_padding_left.UnChange();
			p_padding_right.UnChange();
			p_padding_top.UnChange();
			p_padding_bottom.UnChange();

			p_top.UnChange();
			p_left.UnChange();
			p_right.UnChange();
			p_bottom.UnChange();

			p_width.UnChange();
			p_height.UnChange();

			p_max_width.UnChange();
			p_max_height.UnChange();

			p_min_width.UnChange();
			p_min_height.UnChange();

			p_position.UnChange();

			p_visibility.UnChange();

			p_float.UnChange();

			p_background_image.UnChange();
			p_background_color.UnChange();
			p_background_position_x.UnChange();
			p_background_position_y.UnChange();
			p_background_attachment.UnChange(); //
			p_background_repeat.UnChange(); //
			p_background_size_x.UnChange();
			p_background_size_y.UnChange();


			p_border_top_style.UnChange(); //
			p_border_top_width.UnChange();
			p_border_top_color.UnChange();
			p_border_top_left_radius.UnChange();
			p_border_top_right_radius.UnChange();
			p_border_top_left_method.UnChange();
			p_border_top_right_method.UnChange();

			p_border_right_style.UnChange(); //
			p_border_right_width.UnChange();
			p_border_right_color.UnChange();

			p_border_bottom_style.UnChange(); //
			p_border_bottom_width.UnChange();
			p_border_bottom_color.UnChange();
			p_border_bottom_left_radius.UnChange();
			p_border_bottom_right_radius.UnChange();
			p_border_bottom_left_method.UnChange();
			p_border_bottom_right_method.UnChange();

			p_border_left_style.UnChange(); //
			p_border_left_width.UnChange();
			p_border_left_color.UnChange();


			p_font_name.UnChange();
			p_font_size.UnChange();
			p_font_style.UnChange();
			p_font_weight.UnChange();

			p_text_align.UnChange();
			p_text_decoration_line.UnChange();

			p__gui_text_appearance.UnChange();
			p__gui_text_cursor.UnChange();
			p__gui_text_format.UnChange();
		}

		void ICSS::DropStyles()
		{
			m_styleSets.clear();
			AddFile(L"system.css");
		}

#define CSS_CAPTURE(p) if(!((p) == pOldStyles->p)){(p).SetChanged(true);}else{(p).SetChanged(false);}
		void CCSSstyle::CaptureChanges(const ICSSstyle * _pOldStyles)
		{
			const CCSSstyle * pOldStyles = (const CCSSstyle *)_pOldStyles;
			CSS_CAPTURE(p_display);

			CSS_CAPTURE(p_color);

			CSS_CAPTURE(p_cursor);

			CSS_CAPTURE(p_direction);

			CSS_CAPTURE(p_opacity);

			CSS_CAPTURE(p_z_index);

			CSS_CAPTURE(p_overflow_x);
			CSS_CAPTURE(p_overflow_y);

			CSS_CAPTURE(p_margin_left);
			CSS_CAPTURE(p_margin_right);
			CSS_CAPTURE(p_margin_top);
			CSS_CAPTURE(p_margin_bottom);

			CSS_CAPTURE(p_padding_left);
			CSS_CAPTURE(p_padding_right);
			CSS_CAPTURE(p_padding_top);
			CSS_CAPTURE(p_padding_bottom);

			CSS_CAPTURE(p_top);
			CSS_CAPTURE(p_left);
			CSS_CAPTURE(p_right);
			CSS_CAPTURE(p_bottom);

			CSS_CAPTURE(p_width);
			CSS_CAPTURE(p_height);

			CSS_CAPTURE(p_max_width);
			CSS_CAPTURE(p_max_height);

			CSS_CAPTURE(p_min_width);
			CSS_CAPTURE(p_min_height);

			CSS_CAPTURE(p_position);

			CSS_CAPTURE(p_visibility);

			CSS_CAPTURE(p_float);

			CSS_CAPTURE(p_background_image);
			CSS_CAPTURE(p_background_color);
			CSS_CAPTURE(p_background_position_x);
			CSS_CAPTURE(p_background_position_y);
			CSS_CAPTURE(p_background_attachment); //
			CSS_CAPTURE(p_background_repeat); //
			CSS_CAPTURE(p_background_size_x);
			CSS_CAPTURE(p_background_size_y);


			CSS_CAPTURE(p_border_top_style); //
			CSS_CAPTURE(p_border_top_width);
			CSS_CAPTURE(p_border_top_color);
			CSS_CAPTURE(p_border_top_left_radius);
			CSS_CAPTURE(p_border_top_right_radius);
			CSS_CAPTURE(p_border_top_left_method);
			CSS_CAPTURE(p_border_top_right_method);

			CSS_CAPTURE(p_border_right_style); //
			CSS_CAPTURE(p_border_right_width);
			CSS_CAPTURE(p_border_right_color);

			CSS_CAPTURE(p_border_bottom_style); //
			CSS_CAPTURE(p_border_bottom_width);
			CSS_CAPTURE(p_border_bottom_color);
			CSS_CAPTURE(p_border_bottom_left_radius);
			CSS_CAPTURE(p_border_bottom_right_radius);
			CSS_CAPTURE(p_border_bottom_left_method);
			CSS_CAPTURE(p_border_bottom_right_method);

			CSS_CAPTURE(p_border_left_style); //
			CSS_CAPTURE(p_border_left_width);
			CSS_CAPTURE(p_border_left_color);


			CSS_CAPTURE(p_font_name);
			CSS_CAPTURE(p_font_size);
			CSS_CAPTURE(p_font_style);
			CSS_CAPTURE(p_font_weight);

			CSS_CAPTURE(p_text_align);
			CSS_CAPTURE(p_text_decoration_line);

			CSS_CAPTURE(p__gui_text_appearance);
			CSS_CAPTURE(p__gui_text_cursor);
			
			CSS_CAPTURE(p__gui_text_format);
		}
#undef CSS_CAPTURE

		void CCSSstyle::Reset()
		{
			p_display.Unset();

			p_color.Unset();

			p_cursor.Unset();

			p_direction.Unset();

			p_opacity.Unset();

			p_z_index.Unset();

			p_overflow_x.Unset();
			p_overflow_y.Unset();

			p_margin_left.Unset();
			p_margin_right.Unset();
			p_margin_top.Unset();
			p_margin_bottom.Unset();

			p_padding_left.Unset();
			p_padding_right.Unset();
			p_padding_top.Unset();
			p_padding_bottom.Unset();

			p_top.Unset();
			p_left.Unset();
			p_right.Unset();
			p_bottom.Unset();

			p_width.Unset();
			p_height.Unset();

			p_max_width.Unset();
			p_max_height.Unset();

			p_min_width.Unset();
			p_min_height.Unset();

			p_position.Unset();

			p_visibility.Unset();

			p_float.Unset();

			p_background_image.Unset();
			p_background_color.Unset();
			p_background_position_x.Unset();
			p_background_position_y.Unset();
			p_background_attachment.Unset(); //
			p_background_repeat.Unset(); //
			p_background_size_x.Unset();
			p_background_size_y.Unset();


			p_border_top_style.Unset(); //
			p_border_top_width.Unset();
			p_border_top_color.Unset();
			p_border_top_left_radius.Unset();
			p_border_top_right_radius.Unset();
			p_border_top_left_method.Unset();
			p_border_top_right_method.Unset();

			p_border_right_style.Unset(); //
			p_border_right_width.Unset();
			p_border_right_color.Unset();

			p_border_bottom_style.Unset(); //
			p_border_bottom_width.Unset();
			p_border_bottom_color.Unset();
			p_border_bottom_left_radius.Unset();
			p_border_bottom_right_radius.Unset();
			p_border_bottom_left_method.Unset();
			p_border_bottom_right_method.Unset();

			p_border_left_style.Unset(); //
			p_border_left_width.Unset();
			p_border_left_color.Unset();


			p_font_name.Unset();
			p_font_size.Unset();
			p_font_style.Unset();
			p_font_weight.Unset();

			p_text_align.Unset();
			p_text_decoration_line.Unset();

			p__gui_text_appearance.Unset();
			p__gui_text_cursor.Unset();
			p__gui_text_format.Unset();
		}

		/*void ICSS::ReloadAllStyles()
		{
			for(AssotiativeArray<StringW, ICSSstyleSet>::Iterator i = m_styleSets.begin(); i; i++)
			{
				ICSSstyleSet css(this);
				css.LoadFromString(*i.first);
				m_styleSets[*i.first] = css;
			}
		}*/
	};
};