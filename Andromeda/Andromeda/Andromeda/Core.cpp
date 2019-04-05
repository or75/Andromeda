#include "Core.hpp"

namespace Andromeda
{
	auto ImageLoader::OnLoadImage( HMODULE DllImage , PVOID pReserved ) -> bool
	{
		memset( &m_data , 0 , sizeof loader_data_s );

#if ENABLE_SUPPORT_MANUAL_MAP == 1 
		if ( pReserved )
		{
			auto pLoaderData = (loader_data_s*)pReserved;

			if ( pLoaderData->m_cheat_key == LOADER_LICENSE_KEY )
			{
				memcpy( &m_data , pLoaderData , sizeof loader_data_s );
				memset( pLoaderData , 0 , sizeof loader_data_s );

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
		memset( &m_data , 0 , sizeof loader_data_s );

		m_dll_dir.clear();
	}

	auto ImageLoader::GetDllDir() -> string
	{
		return m_dll_dir;
	}
}