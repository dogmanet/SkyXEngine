#ifndef _CCSSstyle_H_
#define _CCSSstyle_H_


#include "ICSS.h"

namespace GUI
{
	namespace CSS
	{
		class CCSSproperty: public ICSSproperty
		{
		public:
			CCSSproperty():m_type(TYPE_UNDEFINED), isSet(false), isChanged(false)
			{
				m_value.i = 0;
			};

			CCSSproperty(TYPE type):m_type(type), isSet(false), isChanged(true)
			{
				m_value.i = 0;
			};
			CCSSproperty(int value):m_type(TYPE_VAR_INT), isSet(true), isChanged(true)
			{
				m_value.i = value;
			};
			CCSSproperty(float value):m_type(TYPE_VAR_FLOAT), isSet(true), isChanged(true)
			{
				m_value.f = value;
			};
			CCSSproperty(const StringW & value):m_type(TYPE_VAR_STRING), isSet(true), isChanged(true)
			{
				m_value.s = new WCHAR[value.length()];
				memcpy(m_value.s, value.c_str(), (value.length() + 1) * sizeof(WCHAR));
			};
			~CCSSproperty()
			{
				if(m_type == TYPE_VAR_STRING)
				{
					SX_SAFE_DELETE_A(m_value.s);
				}
			}

			TYPE GetType() const
			{
				return(m_type);
			}
			void Set(int val)
			{
				if(m_type == TYPE_VAR_STRING)
				{
					SX_SAFE_DELETE_A(m_value.s);
				}
				isSet = true;
				if(m_type != TYPE_VAR_INT || m_value.i != val)
				{
					isChanged = true;
				}
				m_value.i = val;
				m_type = TYPE_VAR_INT;
			}
			void Set(float val)
			{
				if(m_type == TYPE_VAR_STRING)
				{
					SX_SAFE_DELETE_A(m_value.s);
				}
				isSet = true;
				if(m_type != TYPE_FLOAT || m_value.f != val)
				{
					isChanged = true;
				}
				m_value.f = val;
				m_type = TYPE_FLOAT;
			}
			void Set(const StringW & val)
			{
				if(m_type == TYPE_VAR_STRING)
				{
					if(lstrcmpW(m_value.s, val.c_str()) != 0)
					{
						isChanged = true;
					}
					SX_SAFE_DELETE_A(m_value.s);
				}
				else
				{
					isChanged = true;
				}
				isSet = true;
				m_value.s = new WCHAR[val.length() + 1];
				memcpy(m_value.s, val.c_str(), (val.length() + 1) * sizeof(WCHAR));
				m_type = TYPE_VAR_STRING;
			}

			void SetExt(int val)
			{
				if(m_value.i != val)
				{
					isChanged = true;
				}
				m_value.i = val;
				isSet = true;
			}

			int GetInt() const
			{
				return(m_value.i);
			}
			float GetFloat() const
			{
				return(m_value.f);
			}
			const WCHAR * GetString() const
			{
				return(m_value.s);
			}

			void SetDim(DIM d)
			{
				m_dim = d;
			}

			DIM GetDim() const
			{
				return(m_dim);
			}

			void SetChanged(bool b)
			{
				isChanged = b;
			}

			void UnChange()
			{
				isChanged = false;
			}

			float4_t GetColor()const
			{
				return(float4_t(
					(float)((m_value.i & 0xFF000000) >> 24) / 255.0f,
					(float)((m_value.i & 0x00FF0000) >> 16) / 255.0f,
					(float)((m_value.i & 0x0000FF00) >> 8) / 255.0f,
					(float)((m_value.i & 0x000000FF)) / 255.0f
					));
			}

			bool IsSet() const
			{
				return(isSet);
			}

			bool IsChanged() const
			{
				return(isChanged);
			}

			bool operator==(const ICSSproperty &_b)
			{
				const CCSSproperty * b = (CCSSproperty*)&_b;
				bool bbb;
				if(m_type == TYPE_VAR_STRING)
				{
					bbb = lstrcmpW(m_value.s, b->m_value.s) == 0;
				}
				else
				{
					bbb = m_value.i == b->m_value.i;
				}
				return(m_type == b->m_type && m_dim == b->m_dim && bbb);
			}

			static StringW GetDimName(DIM dim)
			{
				switch(dim)
				{
				case DIM_EM:
					return(L"em");
				case DIM_MM:
					return(L"mm");
				case DIM_PC:
					return(L"%");
				case DIM_PT:
					return(L"pt");
				case DIM_PX:
					return(L"px");
				}
				return(L"");
			}

			CCSSproperty & operator=(const CCSSproperty & p)
			{
				if(m_type == TYPE_VAR_STRING)
				{
					SX_SAFE_DELETE_A(m_value.s);
				}
				isSet = p.isSet;
				if(p.m_type == TYPE_VAR_STRING)
				{
					m_value.s = new WCHAR[wcslen(p.m_value.s) + 1];
					memcpy(m_value.s, p.m_value.s, (wcslen(p.m_value.s) + 1) * sizeof(WCHAR));
					m_type = TYPE_VAR_STRING;
				}
				else
				{
					m_value.i = p.m_value.i;
				}
				m_type = p.m_type;
				m_dim = p.m_dim;
				isChanged = p.isChanged;
				return(*this);
			}

			void Unset()
			{
				TYPE t = m_type;
				Set(0);
				isSet = false;
				if(m_type != TYPE_VAR_STRING &&
					m_type != TYPE_VAR_INT &&
					m_type != TYPE_VAR_FLOAT)
				{
					m_type = t;
				}
				else
				{
					m_type = TYPE_UNDEFINED;
				}
			}

			UINT GetFlags()
			{
				return(m_flags);
			}

			void SetFlags(UINT f)
			{
				m_flags = f;
			}

			int GetPX(int base)
			{
				if(m_type == CSS::ICSSproperty::TYPE_UNDEFINED)
				{
					return(0);
				}
				float falue = GetFloat();
				switch(GetDim())
				{
				case CSS::ICSSproperty::DIM_PC:
					return((int)_round((float)base * (falue * 0.01f)));
				case CSS::ICSSproperty::DIM_PX:
				case CSS::ICSSproperty::DIM_NONE:
					return((int)_round(falue));
				case CSS::ICSSproperty::DIM_PT:
					return((int)_round((float)falue * 4.0f / 3.0f));
				case CSS::ICSSproperty::DIM_MM:
					return((int)_round((float)falue * 3.7795f));
					break;
				default:
					return(base);
				}
			}

		protected:
			TYPE m_type;
			DIM m_dim;
			bool isSet;
			bool isChanged;
			UINT m_flags;
			union
			{
				int i;
				float f;
				WCHAR * s;
			} m_value;
		};

		class CCSSstyle: public ICSSstyle
		{
		public:
			CCSSstyle();

			void LoadStyleFromString(const WCHAR * str, UINT * pNumSyms);
			void ParsePair(const StringW & key, const StringW & val);
			void ParseColor(ICSSproperty * clr, const StringW & val);
			void ParseValue(ICSSproperty * clr, const StringW & val);
			//const String & operator[](const String & key);
			//void Set(const String & key, const String & val);

			UINT GetChangesFlags();

			void ResetChanges();

			void AddRule(const ICSSrule & rule)
			{
				m_pRules.push_back(rule);
			};

			void Reset();

			void DebugDumpStyles();

			void CaptureChanges(const ICSSstyle * pOldStyles);

#define TMP_COPY_RULE(r) r = p.r
			CCSSstyle & operator=(const CCSSstyle & p)
			{
				m_pRules = p.m_pRules;

				
				TMP_COPY_RULE(p_display);

				TMP_COPY_RULE(p_color);

				TMP_COPY_RULE(p_cursor);

				TMP_COPY_RULE(p_direction);

				TMP_COPY_RULE(p_opacity);

				TMP_COPY_RULE(p_z_index);

				TMP_COPY_RULE(p_overflow_x);
				TMP_COPY_RULE(p_overflow_y);

				TMP_COPY_RULE(p_margin_left);
				TMP_COPY_RULE(p_margin_right);
				TMP_COPY_RULE(p_margin_top);
				TMP_COPY_RULE(p_margin_bottom);

				TMP_COPY_RULE(p_padding_left);
				TMP_COPY_RULE(p_padding_right);
				TMP_COPY_RULE(p_padding_top);
				TMP_COPY_RULE(p_padding_bottom);

				TMP_COPY_RULE(p_top);
				TMP_COPY_RULE(p_left);
				TMP_COPY_RULE(p_right);
				TMP_COPY_RULE(p_bottom);

				TMP_COPY_RULE(p_width);
				TMP_COPY_RULE(p_height);

				TMP_COPY_RULE(p_max_width);
				TMP_COPY_RULE(p_max_height);

				TMP_COPY_RULE(p_min_width);
				TMP_COPY_RULE(p_min_height);

				TMP_COPY_RULE(p_position);

				TMP_COPY_RULE(p_visibility);

				TMP_COPY_RULE(p_float);

				TMP_COPY_RULE(p_background_image);
				TMP_COPY_RULE(p_background_color);
				TMP_COPY_RULE(p_background_position_x);
				TMP_COPY_RULE(p_background_position_y);
				TMP_COPY_RULE(p_background_attachment); //
				TMP_COPY_RULE(p_background_repeat); //
				TMP_COPY_RULE(p_background_size_x);
				TMP_COPY_RULE(p_background_size_y);


				TMP_COPY_RULE(p_border_top_style); //
				TMP_COPY_RULE(p_border_top_width);
				TMP_COPY_RULE(p_border_top_color);
				TMP_COPY_RULE(p_border_top_left_radius);
				TMP_COPY_RULE(p_border_top_right_radius);
				TMP_COPY_RULE(p_border_top_left_method);
				TMP_COPY_RULE(p_border_top_right_method);

				TMP_COPY_RULE(p_border_right_style); //
				TMP_COPY_RULE(p_border_right_width);
				TMP_COPY_RULE(p_border_right_color);

				TMP_COPY_RULE(p_border_bottom_style); //
				TMP_COPY_RULE(p_border_bottom_width);
				TMP_COPY_RULE(p_border_bottom_color);
				TMP_COPY_RULE(p_border_bottom_left_radius);
				TMP_COPY_RULE(p_border_bottom_right_radius);
				TMP_COPY_RULE(p_border_bottom_left_method);
				TMP_COPY_RULE(p_border_bottom_right_method);

				TMP_COPY_RULE(p_border_left_style); //
				TMP_COPY_RULE(p_border_left_width);
				TMP_COPY_RULE(p_border_left_color);


				TMP_COPY_RULE(p_font_name);
				TMP_COPY_RULE(p_font_size);
				TMP_COPY_RULE(p_font_style);
				TMP_COPY_RULE(p_font_weight);

				TMP_COPY_RULE(p_text_align);
				TMP_COPY_RULE(p_text_decoration_line);

				TMP_COPY_RULE(p__gui_text_appearance);
				TMP_COPY_RULE(p__gui_text_cursor);
				TMP_COPY_RULE(p__gui_text_format);



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

				return(*this);
			}
#undef TMP_COPY_RULE
			//protected:
			ICSSRuleSet m_pRules;

			CCSSproperty p_display;

			CCSSproperty p_color;

			CCSSproperty p_cursor;

			CCSSproperty p_direction;

			CCSSproperty p_opacity;

			CCSSproperty p_z_index;

			CCSSproperty p_overflow_x;
			CCSSproperty p_overflow_y;

			CCSSproperty p_margin_left;
			CCSSproperty p_margin_right;
			CCSSproperty p_margin_top;
			CCSSproperty p_margin_bottom;

			CCSSproperty p_padding_left;
			CCSSproperty p_padding_right;
			CCSSproperty p_padding_top;
			CCSSproperty p_padding_bottom;

			CCSSproperty p_top;
			CCSSproperty p_left;
			CCSSproperty p_right;
			CCSSproperty p_bottom;

			CCSSproperty p_width;
			CCSSproperty p_height;

			CCSSproperty p_max_width;
			CCSSproperty p_max_height;

			CCSSproperty p_min_width;
			CCSSproperty p_min_height;

			CCSSproperty p_position;

			CCSSproperty p_visibility;

			CCSSproperty p_float;

			CCSSproperty p_background_image;
			CCSSproperty p_background_color;
			CCSSproperty p_background_position_x;
			CCSSproperty p_background_position_y;
			CCSSproperty p_background_attachment; //
			CCSSproperty p_background_repeat; //
			CCSSproperty p_background_size_x;
			CCSSproperty p_background_size_y;


			CCSSproperty p_border_top_style; //
			CCSSproperty p_border_top_width;
			CCSSproperty p_border_top_color;
			CCSSproperty p_border_top_left_radius;
			CCSSproperty p_border_top_right_radius;
			CCSSproperty p_border_top_left_method;
			CCSSproperty p_border_top_right_method;

			CCSSproperty p_border_right_style; //
			CCSSproperty p_border_right_width;
			CCSSproperty p_border_right_color;

			CCSSproperty p_border_bottom_style; //
			CCSSproperty p_border_bottom_width;
			CCSSproperty p_border_bottom_color;
			CCSSproperty p_border_bottom_left_radius;
			CCSSproperty p_border_bottom_right_radius;
			CCSSproperty p_border_bottom_left_method;
			CCSSproperty p_border_bottom_right_method;

			CCSSproperty p_border_left_style; //
			CCSSproperty p_border_left_width;
			CCSSproperty p_border_left_color;


			CCSSproperty p_font_name;
			CCSSproperty p_font_size;
			CCSSproperty p_font_style;
			CCSSproperty p_font_weight;

			CCSSproperty p_text_align;
			CCSSproperty p_text_decoration_line;

			CCSSproperty p__gui_text_appearance;
			CCSSproperty p__gui_text_cursor;
			CCSSproperty p__gui_text_format;
		};
	};
};

#endif
