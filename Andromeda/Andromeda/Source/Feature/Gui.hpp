#pragma once

#include "../Engine.hpp"

#include "Fonts.hpp"

namespace source
{
	namespace feature
	{
		using GuiRenderMenuFn = void( *)( );

		// Begin( "Debug##Default" , 0 , ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs );

		class Gui : public Singleton< Gui >
		{
		public:
			auto Create( IDirect3DDevice9* direct_device , Andromeda::Input& input ) -> bool;
			auto Destroy() -> void;

			auto AddMenuRenderFn( GuiRenderMenuFn Fn ) -> void;
			
			auto InitFont() -> bool;
			auto InitStyle() -> void;

			auto Begin() -> void;
			auto End() -> void;

			auto DeviceLost() -> void;
			auto DeviceReset() -> void;

		public:
			bool			m_gui_open;
			bool			m_gui_create;

		private:
			static auto Event( UINT message , WPARAM wparam , LPARAM lparam ) -> bool;

		public:
			ImFont*			m_font_unicode_ms;
			ImFont*			m_font_color_tube;
			ImFont*			m_font_awesome_icon;

		private:
			string			m_gui_file;
			GuiRenderMenuFn m_render_menu_fn;	
		};
	}
}