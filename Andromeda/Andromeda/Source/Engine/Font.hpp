#pragma once

#include "../Engine.hpp"

#include "Color.hpp"

namespace source
{
	typedef unsigned long hfont;

	enum FontCreateFlag_t
	{
		FONT_CREATE_NONE = 0 ,
		FONT_CREATE_EXTRALIGHT = 1 ,
		FONT_CREATE_MEDIUM = 2 ,
		FONT_CREATE_SEMIBOLD = 4 ,
		FONT_CREATE_ITALIC = 8 ,
		FONT_CREATE_OUTLINE = 16
	};

	namespace engine
	{
		class Font : public Singleton< Font >
		{
		public:

			auto Create( IDirect3DDevice9* direct_device , const string& font_name , int font_size , DWORD font_flags = FONT_CREATE_NONE ) -> void;
			auto Destroy() -> void;

			auto OnLost() -> void;
			auto OnReset() -> void;

			auto Text( int x , int y , Color color , const char* text ) -> void;
			auto GetTextSize( const char* text , int& width , int& height ) -> void;
			auto GetFontSize() const -> int;

		private:
			IDirect3DDevice9*	m_direct_device;
			ID3DXFont*			m_font;
			string				m_font_name;
			int					m_font_size;
			DWORD				m_font_flags;
		};
	}
}