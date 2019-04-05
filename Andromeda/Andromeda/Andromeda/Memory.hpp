#pragma once

#include "../Include/Auto.hpp"

namespace Andromeda
{
	namespace Memory
	{
		template< typename T >
		inline auto vget( PVOID instance , unsigned int index ) -> T
		{
			auto procedure_array = *reinterpret_cast<PDWORD_PTR*>( instance );
			return (T)( procedure_array )[index];
		}

		class SwapVmt
		{
		public:
			SwapVmt( void* instance = nullptr );

		public:
			auto Create( void* instance ) -> bool;
			auto Destroy() -> void;

			auto Set( bool state ) -> bool;
			auto Hook( PVOID hooked , size_t index ) -> bool;

			template< typename T >
			inline auto Get( size_t index ) -> T
			{
				if ( !m_restore || index >= m_size )
					return ( T )nullptr;

				return (T)m_restore[index];
			}

		private:
			uintptr_t** m_instance = nullptr;
			uintptr_t* m_restore = nullptr;
			unique_ptr< uintptr_t[] > m_replace = nullptr;
			size_t m_size = 0u;
		};

		/* IDA Style 00 FF ?? */

		auto FindPattern( const char* pattern , DWORD_PTR start , DWORD_PTR end , DWORD_PTR offset , DWORD read = 0 )->PVOID;
		auto FindPattern( const char* module_name , const char* pattern , DWORD_PTR offset , DWORD read = 0 )->PVOID;
	}
}