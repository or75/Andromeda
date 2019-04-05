#include "Input.hpp"

namespace Andromeda
{
	auto Input::Create() -> bool
	{
		EnumWindows( &Input::ProcedureEnum , GetCurrentProcessId() );

		if ( !m_window )
			return false;

		SetLastError( 0u );
		m_procedure = (WNDPROC)SetWindowLongPtr( m_window , GWLP_WNDPROC , (LONG_PTR)&Input::Procedure );

		auto error_code = GetLastError();

		if ( !m_procedure && error_code )
			return false;

		return true;
	}

	auto Input::Destroy() -> void
	{
		if ( m_procedure )
			SetWindowLongPtr( m_window , GWLP_WNDPROC , (LONG_PTR)m_procedure );

		m_window = nullptr;
		m_procedure = nullptr;
		m_event_array.clear();
	}

	auto Input::SetWindow( HWND window ) -> void
	{
		m_window = window;
	}

	auto Input::GetWindow() const -> HWND
	{
		return m_window;
	}

	auto Input::AddEvent( EventFn procedure ) -> void
	{
		if ( !procedure )
		{
			return;
		}

		if ( !m_event_array.empty() )
		{
			for ( const auto& event : m_event_array )
			{
				if ( event == procedure )
					return;
			}
		}

		m_event_array.emplace_back( procedure );
	}

	auto Input::ExecuteEventArray( HWND window , UINT message , WPARAM wparam , LPARAM lparam ) -> LRESULT
	{
		auto code = false;

		if ( !m_event_array.empty() )
		{
			for ( auto& event : m_event_array )
				code = event( message , wparam , lparam );
		}

		if ( code )
			return TRUE;

		auto execute_procedure = [&]( HWND window , UINT message , WPARAM wparam , LPARAM lparam )
		{
			return ( m_procedure ?
					 CallWindowProc( m_procedure , window , message , wparam , lparam ) :
					 DefWindowProc( window , message , wparam , lparam ) );
		};

		return execute_procedure( window , message , wparam , lparam );
	}

	auto WINAPI Input::ProcedureEnum( HWND window , LPARAM lparam ) -> BOOL
	{
		DWORD dwProcessID = 0;

		GetWindowThreadProcessId( window , &dwProcessID );

		if ( dwProcessID == static_cast<DWORD>( lparam ) )
		{
			Input::Instance().SetWindow( window );
			return FALSE;
		}

		return TRUE;
	}

	auto WINAPI Input::Procedure( HWND window , UINT message , WPARAM wparam , LPARAM lparam ) -> LRESULT
	{
		return Input::Instance().ExecuteEventArray( window , message , wparam , lparam );
	}
}