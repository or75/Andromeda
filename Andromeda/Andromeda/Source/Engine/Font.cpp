#include "Font.hpp"

namespace source
{
	namespace engine
	{
		auto Font::Create( IDirect3DDevice9* direct_device , const string & font_name , int font_size , DWORD font_flags ) -> void
		{
			m_direct_device = direct_device;
			m_font = nullptr;
			m_font_name = font_name;
			m_font_size = font_size;
			m_font_flags = font_flags;

			if ( !m_font )
			{
				UINT Weight = FW_NORMAL;

				if ( m_font_flags & FONT_CREATE_EXTRALIGHT )
					Weight = FW_EXTRALIGHT;
				else if ( m_font_flags & FONT_CREATE_MEDIUM )
					Weight = FW_MEDIUM;
				else if ( m_font_flags & FONT_CREATE_SEMIBOLD )
					Weight = FW_SEMIBOLD;

				BOOL Italic = ( m_font_flags & FONT_CREATE_ITALIC ) ? TRUE : FALSE;

				D3DXCreateFontA( m_direct_device ,
								 m_font_size ,
								 0 ,
								 Weight ,
								 0 ,
								 Italic ,
								 RUSSIAN_CHARSET ,
								 OUT_DEFAULT_PRECIS ,
								 ANTIALIASED_QUALITY ,
								 DEFAULT_PITCH ,
								 m_font_name.c_str() ,
								 &m_font );
			}
		}

		auto Font::Destroy() -> void
		{
			m_direct_device = nullptr;

			if ( m_font )
			{
				m_font->Release();
				m_font = nullptr;
			}
		}

		auto Font::OnLost() -> void
		{
			if ( m_font )
				m_font->OnLostDevice();
		}

		auto Font::OnReset() -> void
		{
			if ( m_font )
				m_font->OnResetDevice();
		}

		auto Font::Text( int x , int y , Color color , const char* text ) -> void
		{
			if ( !m_direct_device )
				return;

			if ( !m_font )
				return;

			if ( !text )
				return;

			if ( m_font_flags & FONT_CREATE_OUTLINE )
			{
				RECT Out[2] =
				{
					{ x + 1, y , 0, 0 },
					{ x + 1 , y + 1, 0, 0 },
				};

				for ( auto& Rect : Out ) 
				{
					m_font->DrawTextW( 0 , Andromeda::ansi_to_unicode( text ).c_str() , -1 , &Rect , DT_TOP | DT_LEFT | DT_NOCLIP , Color( 5 , 5 , 5 , 255 ).Code() );
				}
			}

			RECT Rect = { x, y, 0, 0 };

			m_font->DrawTextW( 0 , Andromeda::ansi_to_unicode( text ).c_str() , -1 , &Rect , DT_NOCLIP , color.Code() );
		}

		auto Font::GetTextSize( const char* text , int & width , int & height ) -> void
		{
			if ( !m_direct_device )
				return;

			if ( !m_font )
				return;

			if ( !text )
				return;

			RECT Rect = { 0, 0, 0, 0 };

			m_font->DrawTextW( 0 , Andromeda::ansi_to_unicode( text ).c_str() , -1 , &Rect , DT_CALCRECT , Color::None().Code() );

			width = Rect.right - Rect.left;
			height = Rect.bottom - Rect.top;
		}

		auto Font::GetFontSize() const -> int
		{
			return m_font_size;
		}
	}
}