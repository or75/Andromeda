#pragma once

#include "../Include/Auto.hpp"

namespace Andromeda
{
	using EventFn = bool( *)( UINT message , WPARAM wparam , LPARAM lparam );

	class Input : public Singleton< Input >
	{
	public:
		auto Create() -> bool;
		auto Destroy() -> void;

		auto SetWindow( HWND window ) -> void;
		auto GetWindow() const->HWND;

		auto AddEvent( EventFn procedure ) -> void;
		auto ExecuteEventArray( HWND window , UINT message , WPARAM wparam , LPARAM lparam )->LRESULT;

	private:
		static auto WINAPI ProcedureEnum( HWND window , LPARAM lparam ) -> BOOL;
		static auto WINAPI Procedure( HWND window , UINT message , WPARAM wparam , LPARAM lparam )->LRESULT;

	private:
		HWND m_window = nullptr;
		WNDPROC m_procedure = nullptr;
		vector< EventFn > m_event_array = { };
	};
}