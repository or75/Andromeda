#pragma once

#include "../Engine.hpp"
#include "../Client.hpp"

namespace source
{
	namespace feature
	{
		class Menu : public Singleton< Menu >
		{
		public:
			auto Create() -> void;

			auto RenderStartMenu() -> void;

			auto RenderSettingsMenu() -> void;
			auto RenderAboutMenu() -> void;

		private:
			auto ButtonIcon( const char* icon , const char* text , ImVec2 size = ImVec2( -1.f , 0.f ) , float icon_offset = 0.f ) -> bool;

			auto CheckBox( const char* title , const char* str_id , int* current_item , float left_padding = 90.f ) -> void;
			auto SliderInt( const char* title , const char* str_id , int* v , int min , int max , float left_padding = 90.f ) -> void;
			auto HotKey( const char* title , const char* str_id , int* k , float left_padding = 90.f ) -> bool;

			auto SettingsMenuHandle() -> void;

		public:
			float	m_start_width;
			float	m_start_height;

			bool	m_show_settings;
			bool	m_show_about;

		public:
			size_t		m_select_script_index;
		};
	}
}