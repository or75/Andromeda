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
		private:
			auto ButtonIcon( const char* icon , const char* text , ImVec2 size = ImVec2( -1.f , 0.f ) , float icon_offset = 0.f ) -> bool;

		public:
			float	m_start_width;
			float	m_start_height;

		public:
			size_t		m_select_script_index;
		};
	}
}