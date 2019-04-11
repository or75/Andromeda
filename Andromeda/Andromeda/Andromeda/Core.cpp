#include "Core.hpp"

namespace Andromeda
{
	auto ImageLoader::OnLoadImage( HMODULE DllImage , PVOID pReserved ) -> bool
	{
#if ENABLE_SUPPORT_MANUAL_MAP == 1 
		if ( pReserved )
		{
			auto pManualInject = (PMANUAL_INJECT)pReserved;

			if ( pManualInject->m_cheat_key == LOADER_LICENSE_KEY )
			{
				auto image_name = string( pManualInject->m_cheat_dir ) + XorStr( "\\" );

				m_dll_dir = image_name.substr( 0 , image_name.find_last_of( '\\' ) );
				m_dll_dir += '\\';

				return true;
			}
		}

		return false;
#else
		char image_file_name[MAX_PATH] = { 0 };

		if ( !GetModuleFileNameA( DllImage , image_file_name , MAX_PATH ) )
			return false;

		auto image_name = string( image_file_name );
			
		m_dll_dir = image_name.substr( 0 , image_name.find_last_of( '\\' ) );
		m_dll_dir += '\\';

		return true;
#endif
	}

	auto ImageLoader::Destroy() -> void
	{
		m_dll_dir.clear();
	}

	auto ImageLoader::GetDllDir() -> string
	{
		return m_dll_dir;
	}
}