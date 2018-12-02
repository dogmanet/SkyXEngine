#include "CSSstyle.h"

#include "DOMdocument.h"

namespace gui
{
	namespace css
	{
		void CCSSproperty::setupTransition(const ICSSproperty * pOldProperty, dom::IDOMdocument *_pDoc)
		{
			if(m_flags & ICSSproperty::FLAG_DO_TRANSITION)
			{
				dom::CDOMdocument *pDoc = (dom::CDOMdocument*)_pDoc;
				const CCSSproperty * pProp = (CCSSproperty*)pOldProperty;
				m_valueTo = m_value;
				m_value = m_valueOld = pProp->m_value;
				m_fTransitionFrac = 0.0f;

				pDoc->addTransitionProperty(this);
			}
		}

		void CCSSproperty::cancelTransitions(dom::IDOMdocument *_pDoc)
		{
			dom::CDOMdocument *pDoc = (dom::CDOMdocument*)_pDoc;
			pDoc->removeTransitionProperty(this);
		}

		//##########################################################################

		CCSSstyle::PropsMap CCSSstyle::m_mPropPtrs;

		CCSSstyle::PropsMap *CCSSstyle::getPropsMap()
		{
			if(!m_mPropPtrs.Size())
			{
				m_mPropPtrs[L"display"] = {&CCSSstyle::p_display};
				m_mPropPtrs[L"color"] = {&CCSSstyle::p_color};
				m_mPropPtrs[L"cursor"] = {&CCSSstyle::p_cursor};
				m_mPropPtrs[L"direction"] = {&CCSSstyle::p_direction};
				m_mPropPtrs[L"opacity"] = {&CCSSstyle::p_opacity};
				m_mPropPtrs[L"z-index"] = {&CCSSstyle::p_z_index};

				m_mPropPtrs[L"overflow-x"] = {&CCSSstyle::p_overflow_x};
				m_mPropPtrs[L"overflow-y"] = {&CCSSstyle::p_overflow_y};
				m_mPropPtrs[L"overflow"] = {&CCSSstyle::p_overflow_x, &CCSSstyle::p_overflow_y};

				m_mPropPtrs[L"margin-left"] = {&CCSSstyle::p_margin_left};
				m_mPropPtrs[L"margin-right"] = {&CCSSstyle::p_margin_right};
				m_mPropPtrs[L"margin-top"] = {&CCSSstyle::p_margin_top};
				m_mPropPtrs[L"margin-bottom"] = {&CCSSstyle::p_margin_bottom};
				m_mPropPtrs[L"margin"] = {&CCSSstyle::p_margin_top, &CCSSstyle::p_margin_right, &CCSSstyle::p_margin_bottom, &CCSSstyle::p_margin_left};
				
				m_mPropPtrs[L"padding-left"] = {&CCSSstyle::p_padding_left};
				m_mPropPtrs[L"padding-right"] = {&CCSSstyle::p_padding_right};
				m_mPropPtrs[L"padding-top"] = {&CCSSstyle::p_padding_top};
				m_mPropPtrs[L"padding-bottom"] = {&CCSSstyle::p_padding_bottom};
				m_mPropPtrs[L"padding"] = {&CCSSstyle::p_padding_top, &CCSSstyle::p_padding_right, &CCSSstyle::p_padding_bottom, &CCSSstyle::p_padding_right};

				m_mPropPtrs[L"top"] = {&CCSSstyle::p_top};
				m_mPropPtrs[L"left"] = {&CCSSstyle::p_left};
				m_mPropPtrs[L"right"] = {&CCSSstyle::p_right};
				m_mPropPtrs[L"bottom"] = {&CCSSstyle::p_bottom};

				m_mPropPtrs[L"width"] = {&CCSSstyle::p_width};
				m_mPropPtrs[L"height"] = {&CCSSstyle::p_height};

				m_mPropPtrs[L"max-width"] = {&CCSSstyle::p_max_width};
				m_mPropPtrs[L"max-height"] = {&CCSSstyle::p_max_height};

				m_mPropPtrs[L"min-width"] = {&CCSSstyle::p_min_width};
				m_mPropPtrs[L"min-height"] = {&CCSSstyle::p_min_height};

				m_mPropPtrs[L"position"] = {&CCSSstyle::p_position};

				m_mPropPtrs[L"visibility"] = {&CCSSstyle::p_visibility};

				m_mPropPtrs[L"float"] = {&CCSSstyle::p_float};

				m_mPropPtrs[L"background-image"] = {&CCSSstyle::p_background_image};
				m_mPropPtrs[L"background-color"] = {&CCSSstyle::p_background_color};
				m_mPropPtrs[L"background-position-x"] = {&CCSSstyle::p_background_position_x};
				m_mPropPtrs[L"background-position-y"] = {&CCSSstyle::p_background_position_y};
				m_mPropPtrs[L"background-position"] = {&CCSSstyle::p_background_position_x, &CCSSstyle::p_background_position_y};
				m_mPropPtrs[L"background-attachment"] = {&CCSSstyle::p_background_attachment};
				m_mPropPtrs[L"background-repeat"] = {&CCSSstyle::p_background_repeat};
				m_mPropPtrs[L"background-size-x"] = {&CCSSstyle::p_background_size_x};
				m_mPropPtrs[L"background-size-y"] = {&CCSSstyle::p_background_size_y};
				m_mPropPtrs[L"background-size"] = {&CCSSstyle::p_background_size_x, &CCSSstyle::p_background_size_y};
				
				m_mPropPtrs[L"background"] = {
					&CCSSstyle::p_background_image, 
					&CCSSstyle::p_background_color,
					&CCSSstyle::p_background_position_x,
					&CCSSstyle::p_background_position_y,
					&CCSSstyle::p_background_attachment,
					&CCSSstyle::p_background_repeat,
					&CCSSstyle::p_background_size_x,
					&CCSSstyle::p_background_size_y
				};


				m_mPropPtrs[L"border-top-style"] = {&CCSSstyle::p_border_top_style};
				m_mPropPtrs[L"border-top-width"] = {&CCSSstyle::p_border_top_width};
				m_mPropPtrs[L"border-top-color"] = {&CCSSstyle::p_border_top_color};
				m_mPropPtrs[L"border-top-left-radius"] = {&CCSSstyle::p_border_top_left_radius};
				m_mPropPtrs[L"border-top-right-radius"] = {&CCSSstyle::p_border_top_right_radius};
				m_mPropPtrs[L"border-top-left-method"] = {&CCSSstyle::p_border_top_left_method};
				m_mPropPtrs[L"border-top-right-method"] = {&CCSSstyle::p_border_top_right_method};

				m_mPropPtrs[L"border-right-style"] = {&CCSSstyle::p_border_right_style};
				m_mPropPtrs[L"border-right-width"] = {&CCSSstyle::p_border_right_width};
				m_mPropPtrs[L"border-right-color"] = {&CCSSstyle::p_border_right_color};

				m_mPropPtrs[L"border-bottom-style"] = {&CCSSstyle::p_border_bottom_style};
				m_mPropPtrs[L"border-bottom-width"] = {&CCSSstyle::p_border_bottom_width};
				m_mPropPtrs[L"border-bottom-color"] = {&CCSSstyle::p_border_bottom_color};
				m_mPropPtrs[L"border-bottom-left-radius"] = {&CCSSstyle::p_border_bottom_left_radius};
				m_mPropPtrs[L"border-bottom-right-radius"] = {&CCSSstyle::p_border_bottom_right_radius};
				m_mPropPtrs[L"border-bottom-left-method"] = {&CCSSstyle::p_border_bottom_left_method};
				m_mPropPtrs[L"border-bottom-right-method"] = {&CCSSstyle::p_border_bottom_right_method};

				m_mPropPtrs[L"border-left-style"] = {&CCSSstyle::p_border_left_style};
				m_mPropPtrs[L"border-left-width"] = {&CCSSstyle::p_border_left_width};
				m_mPropPtrs[L"border-left-color"] = {&CCSSstyle::p_border_left_color};

				m_mPropPtrs[L"border"] = {
					&CCSSstyle::p_border_top_style,
					&CCSSstyle::p_border_top_width,
					&CCSSstyle::p_border_top_color,
					&CCSSstyle::p_border_top_left_radius,
					&CCSSstyle::p_border_top_right_radius,
					&CCSSstyle::p_border_top_left_method,
					&CCSSstyle::p_border_top_right_method,

					&CCSSstyle::p_border_right_style,
					&CCSSstyle::p_border_right_width,
					&CCSSstyle::p_border_right_color,

					&CCSSstyle::p_border_bottom_style,
					&CCSSstyle::p_border_bottom_width,
					&CCSSstyle::p_border_bottom_color,
					&CCSSstyle::p_border_bottom_left_radius,
					&CCSSstyle::p_border_bottom_right_radius,
					&CCSSstyle::p_border_bottom_left_method,
					&CCSSstyle::p_border_bottom_right_method,

					&CCSSstyle::p_border_left_style,
					&CCSSstyle::p_border_left_width,
					&CCSSstyle::p_border_left_color
				};

				m_mPropPtrs[L"border-radius"] = {
					&CCSSstyle::p_border_top_left_radius,
					&CCSSstyle::p_border_top_right_radius,
					&CCSSstyle::p_border_bottom_left_radius,
					&CCSSstyle::p_border_bottom_right_radius
				};
				m_mPropPtrs[L"border-width"] = {
					&CCSSstyle::p_border_top_width,
					&CCSSstyle::p_border_right_width,
					&CCSSstyle::p_border_bottom_width,
					&CCSSstyle::p_border_left_width
				};
				m_mPropPtrs[L"border-style"] = {
					&CCSSstyle::p_border_top_style,
					&CCSSstyle::p_border_right_style,
					&CCSSstyle::p_border_bottom_style,
					&CCSSstyle::p_border_left_style
				};
				m_mPropPtrs[L"border-color"] = {
					&CCSSstyle::p_border_top_color,
					&CCSSstyle::p_border_right_color,
					&CCSSstyle::p_border_bottom_color,
					&CCSSstyle::p_border_left_color
				};



				m_mPropPtrs[L"font-name"] = {&CCSSstyle::p_font_name};
				m_mPropPtrs[L"font-size"] = {&CCSSstyle::p_font_size};
				m_mPropPtrs[L"font-style"] = {&CCSSstyle::p_font_style};
				m_mPropPtrs[L"font-weight"] = {&CCSSstyle::p_font_weight};

				m_mPropPtrs[L"text-align"] = {&CCSSstyle::p_text_align};
				m_mPropPtrs[L"text-decoration-line"] = {&CCSSstyle::p_text_decoration_line};

				m_mPropPtrs[L"text-shadow-offset-x"] = {&CCSSstyle::p_text_shadow_offset_x};
				m_mPropPtrs[L"text-shadow-offset-y"] = {&CCSSstyle::p_text_shadow_offset_y};
				m_mPropPtrs[L"text-shadow-offset"] = {&CCSSstyle::p_text_shadow_offset_x, &CCSSstyle::p_text_shadow_offset_y};
				m_mPropPtrs[L"text-shadow-blur-radius"] = {&CCSSstyle::p_text_shadow_blur_radius};
				m_mPropPtrs[L"text-shadow-color"] = {&CCSSstyle::p_text_shadow_color};
				m_mPropPtrs[L"text-shadow"] = {
					&CCSSstyle::p_text_shadow_offset_x,
					&CCSSstyle::p_text_shadow_offset_y,
					&CCSSstyle::p_text_shadow_blur_radius,
					&CCSSstyle::p_text_shadow_color
				};

				m_mPropPtrs[L"-gui-text-appearance"] = {&CCSSstyle::p__gui_text_appearance};
				m_mPropPtrs[L"-gui-text-cursor"] = {&CCSSstyle::p__gui_text_cursor};
				m_mPropPtrs[L"-gui-text-format"] = {&CCSSstyle::p__gui_text_format};
				m_mPropPtrs[L"-gui-z"] = {&CCSSstyle::p__gui_z};
				m_mPropPtrs[L"-gui-spin-percent"] = {&CCSSstyle::p__gui_spin_percent};

				m_mPropPtrs[L"transform"] = {&CCSSstyle::p_transform};

				m_mPropPtrs[L"none"] = CPropPtr();

				m_mPropPtrs[L"all"] = {
					&CCSSstyle::p_display,
					&CCSSstyle::p_color,
					&CCSSstyle::p_cursor,
					&CCSSstyle::p_direction,
					&CCSSstyle::p_opacity,
					&CCSSstyle::p_z_index,
					&CCSSstyle::p_overflow_x,
					&CCSSstyle::p_overflow_y,
					&CCSSstyle::p_margin_left,
					&CCSSstyle::p_margin_right,
					&CCSSstyle::p_margin_top,
					&CCSSstyle::p_margin_bottom,
					&CCSSstyle::p_padding_left,
					&CCSSstyle::p_padding_right,
					&CCSSstyle::p_padding_top,
					&CCSSstyle::p_padding_bottom,
					&CCSSstyle::p_top,
					&CCSSstyle::p_left,
					&CCSSstyle::p_right,
					&CCSSstyle::p_bottom,
					&CCSSstyle::p_width,
					&CCSSstyle::p_height,
					&CCSSstyle::p_max_width,
					&CCSSstyle::p_max_height,
					&CCSSstyle::p_min_width,
					&CCSSstyle::p_min_height,
					&CCSSstyle::p_position,
					&CCSSstyle::p_visibility,
					&CCSSstyle::p_float,


					&CCSSstyle::p_background_image,
					&CCSSstyle::p_background_color,
					&CCSSstyle::p_background_position_x,
					&CCSSstyle::p_background_position_y,
					&CCSSstyle::p_background_attachment,
					&CCSSstyle::p_background_repeat,
					&CCSSstyle::p_background_size_x,
					&CCSSstyle::p_background_size_y,

					&CCSSstyle::p_border_top_style,
					&CCSSstyle::p_border_top_width,
					&CCSSstyle::p_border_top_color,
					&CCSSstyle::p_border_top_left_radius,
					&CCSSstyle::p_border_top_right_radius,
					&CCSSstyle::p_border_top_left_method,
					&CCSSstyle::p_border_top_right_method,

					&CCSSstyle::p_border_right_style,
					&CCSSstyle::p_border_right_width,
					&CCSSstyle::p_border_right_color,

					&CCSSstyle::p_border_bottom_style,
					&CCSSstyle::p_border_bottom_width,
					&CCSSstyle::p_border_bottom_color,
					&CCSSstyle::p_border_bottom_left_radius,
					&CCSSstyle::p_border_bottom_right_radius,
					&CCSSstyle::p_border_bottom_left_method,
					&CCSSstyle::p_border_bottom_right_method,

					&CCSSstyle::p_border_left_style,
					&CCSSstyle::p_border_left_width,
					&CCSSstyle::p_border_left_color,


					&CCSSstyle::p_font_name,
					&CCSSstyle::p_font_size,
					&CCSSstyle::p_font_style,
					&CCSSstyle::p_font_weight,
					&CCSSstyle::p_text_align,
					&CCSSstyle::p_text_decoration_line,
					&CCSSstyle::p_text_shadow_offset_x,
					&CCSSstyle::p_text_shadow_offset_y,
					&CCSSstyle::p_text_shadow_blur_radius,
					&CCSSstyle::p_text_shadow_color,
					&CCSSstyle::p__gui_text_appearance,
					&CCSSstyle::p__gui_text_cursor,
					&CCSSstyle::p__gui_text_format,
					&CCSSstyle::p__gui_z,
					&CCSSstyle::p__gui_spin_percent,


					&CCSSstyle::p_transform
				};
				
			}

			return(&m_mPropPtrs);
		}

		void CCSSstyle::setupTransitions(const CCSSstyle *pStyle)
		{
			getPropsMap();

			/*CPropPtr *allProps = &m_mPropPtrs[L"all"];

			for(int i = 0, l = allProps->ptrs.size(); i < l; ++i)
			{
				ICSSproperty * pProp = &(this->*(allProps->ptrs[i]));
				if(pProp->getFlags() & ICSSproperty::FLAG_DO_TRANSITION)
				{
					pProp->setFlags(pProp->getFlags() ^ ICSSproperty::FLAG_DO_TRANSITION);
				}
			}*/

			if(pStyle != this)
			{
				inheritTransitions(pStyle);
			}

			for(int i = 0, l = pStyle->m_aTransitions.size(); i < l; ++i)
			{
				const CTransitionItem * pTI = &pStyle->m_aTransitions[i];
				
				const PropsMap::Node * pNode;
				if(m_mPropPtrs.KeyExists(pTI->m_szProperty, &pNode))
				{
					CPropPtr *curProps = pNode->Val;

					for(int j = 0, jl = curProps->ptrs.size(); j < jl; ++j)
					{
						ICSSproperty * pProp = &(this->*(curProps->ptrs[j]));
						pProp->allowTransition(pTI->m_fDuration, pTI->m_fDelay, pTI->m_timingFunction);
					}
				}
			}
		}

		void CCSSstyle::cancelTransitions()
		{
			if(!m_pDoc)
			{
				return;
			}
			getPropsMap();

			CPropPtr *allProps = &m_mPropPtrs[L"all"];

			for(int i = 0, l = allProps->ptrs.size(); i < l; ++i)
			{
				ICSSproperty * pProp = &(this->*(allProps->ptrs[i]));
				if(pProp->getFlags() & ICSSproperty::FLAG_DO_TRANSITION)
				{
					pProp->cancelTransitions(m_pDoc);
				}
			}
		}

		void CCSSstyle::inheritTransitions(const CCSSstyle *pOther)
		{
			const CTransitionItem *pTransition;
			bool bFound;
			for(int i = 0, l = pOther->m_aTransitions.size(); i < l; ++i)
			{
				pTransition = &pOther->m_aTransitions[i];
				bFound = false;
				for(int j = 0, jl = m_aTransitions.size(); j < jl; ++j)
				{
					if(pTransition->m_szProperty == m_aTransitions[j].m_szProperty)
					{
						bFound = true;
						break;
					}
				}
				if(!bFound)
				{
					m_aTransitions.push_back(*pTransition);
				}
			}
		}
	};
};
