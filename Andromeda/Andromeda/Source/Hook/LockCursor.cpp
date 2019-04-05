#include "Hook.hpp"

#include "../Feature/Gui.hpp"

namespace source
{
	namespace hook
	{
		auto WINAPI Hook_LockCursor() -> void
		{
			source::Surface.Set( false );

			auto& gui = feature::Gui::Instance();

			if ( gui.m_gui_open )
				source::m_surface->UnlockCursor();
			else
				source::m_surface->LockCursor();

			source::Surface.Set( true );
		}
	}
}