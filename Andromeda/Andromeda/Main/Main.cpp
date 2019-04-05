#include "../Source/Engine.hpp"

auto WINAPI CheatThread( PVOID lpThreadParam ) -> DWORD
{
	if ( !source::Create() )
	{
		source::Destroy();

#if ENABLE_SUPPORT_MANUAL_MAP == 0
		FreeLibraryAndExitThread( (HMODULE)lpThreadParam , 0 );
#endif
	}

	return 0;
}

auto OnLoadImage( HMODULE DllImage , PVOID pReserved ) -> void
{
	if ( Andromeda::ImageLoader::Instance().OnLoadImage( DllImage , pReserved ) )
		CreateThread( 0 , 0 , CheatThread , DllImage , 0 , 0 );
}

auto WINAPI DllMain( HINSTANCE hinstDLL , DWORD Reason , PVOID pReserved ) -> BOOL
{
	switch ( Reason )
	{
		case DLL_PROCESS_ATTACH:
		{
#if ENABLE_SUPPORT_MANUAL_MAP == 0
			DisableThreadLibraryCalls( hinstDLL );
#endif
			OnLoadImage( hinstDLL , pReserved );
			break;
		}
#if ENABLE_SUPPORT_MANUAL_MAP == 0
		case DLL_PROCESS_DETACH:
		{
			source::Destroy();
			break;
		}
#endif
	}
	
	return TRUE;
}