#include "Memory.hpp"

#define INRANGE(x,a,b)	(x >= a && x <= b) 
#define getBits( x )	(INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )	(getBits(x[0]) << 4 | getBits(x[1]))

namespace Andromeda
{
	namespace Memory
	{
		SwapVmt::SwapVmt( void* instance )
		{
			if ( instance )
				Create( instance );
		}

		auto SwapVmt::Create( void* instance ) -> bool
		{
			m_instance = reinterpret_cast<uintptr_t**>( instance );

			if ( !m_instance )
				return false;

			m_restore = *m_instance;

			if ( !m_restore )
				return false;

			while ( m_restore[m_size] )
				m_size++;

			if ( !m_size )
				return false;

			m_replace = make_unique< uintptr_t[] >( m_size );
			memcpy( m_replace.get() , m_restore , m_size * sizeof( uintptr_t ) );

			if ( !Set( true ) )
				return false;

			return true;
		}

		auto SwapVmt::Destroy() -> void
		{
			Set( false );

			m_instance = nullptr;
			m_restore = nullptr;
			m_replace.reset();
			m_size = 0;
		}

		auto SwapVmt::Set( bool state ) -> bool
		{
			if ( !m_instance || !m_restore || !m_replace )
				return false;

			auto data = state ? m_replace.get() : m_restore;
			return ( *m_instance = data );
		}

		auto SwapVmt::Hook( PVOID hooked , size_t index ) -> bool
		{
			if ( !m_replace )
				return false;

			return ( m_replace[index] = (uintptr_t)hooked );
		}

		auto GetModuleInfo( const char* module_name ) -> MODULEINFO
		{
			MODULEINFO module_info = { 0 };

			auto h_module = GetModuleHandleA( module_name );

			if ( h_module == 0 )
				return module_info;

			GetModuleInformation( GetCurrentProcess() , h_module , &module_info , sizeof( MODULEINFO ) );
			return module_info;
		}

		auto FindPattern( const char* pattern , DWORD_PTR start , DWORD_PTR end , DWORD_PTR offset , DWORD read )->PVOID
		{
			DWORD_PTR found_address = 0;
			auto p_pattern = pattern;

			for ( DWORD_PTR dwPtr = start; dwPtr < end; dwPtr++ )
			{
				if ( !*p_pattern )
					return (PVOID)found_address;

				if ( *(PBYTE)p_pattern == '\?' || *(PBYTE)dwPtr == getByte( p_pattern ) )
				{
					if ( !found_address )
						found_address = (DWORD_PTR)dwPtr;

					if ( !p_pattern[1] || !p_pattern[2] )
						break;

					if ( *(PWORD)p_pattern == '\?\?' || *(PBYTE)p_pattern != '\?' )
						p_pattern += 3;
					else
						p_pattern += 2;
				}
				else
				{
					p_pattern = pattern;
					found_address = 0;
				}
			}

			if ( found_address )
			{
				found_address += (DWORD_PTR)offset;

				for ( DWORD i = 0; i < read; i++ )
					found_address = *(PDWORD_PTR)( found_address );
			}

			return (PVOID)found_address;
		}

		auto FindPattern( const char* module_name , const char* pattern , DWORD_PTR offset , DWORD read )->PVOID
		{
			auto Info = GetModuleInfo( module_name );
			auto Start = (DWORD_PTR)Info.lpBaseOfDll;
			auto Size = Info.SizeOfImage;

			return FindPattern( pattern , Start , Start + Size , offset , read );
		}
	}
}