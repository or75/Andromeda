#include "Utils.hpp"

namespace source
{
	namespace feature
	{
		auto Utils::Create() -> bool
		{
			auto set_clan_tag = Andromeda::Memory::FindPattern( XorStr( "engine.dll" ) , XorStr( "53 56 57 8B DA 8B F9 FF 15" ) , 0 , 0 );

			if ( !set_clan_tag )
			{
				Andromeda::WriteDebugLog( XorStr( "[error] #SetClanTag\n" ) );
				return false;
			}

#if ENABLE_DEBUG_INTERFACE == 1
			Andromeda::WriteDebugLog( XorStr( "[set_clan_tag] %p\n" ) , set_clan_tag );
#endif

			SetClanTagFn = (SetClanTag_t)set_clan_tag;

			return true;
		}

		auto Utils::SetClanTag( string tag ) -> void
		{
			if ( SetClanTagFn )
				SetClanTagFn( tag.c_str() , 0 );
		}

		auto Utils::RandomInt( int iMinVal , int iMaxVal ) -> int
		{
			using RandomInt_t = int( __cdecl * )( int , int );
			static auto random_int_export = (RandomInt_t)GetProcAddress( GetModuleHandleA( XorStr( "vstdlib.dll" ) ) , XorStr( "RandomInt" ) );
			return random_int_export( iMinVal , iMaxVal );
		}

		auto Utils::RandomFloat( float flMinVal , float flMaxVal ) -> float
		{
			using RandomFloat_t = float( __cdecl * )( float , float );
			static auto random_float_export = (RandomFloat_t)GetProcAddress( GetModuleHandleA( XorStr( "vstdlib.dll" ) ) , XorStr( "RandomFloat" ) );
			return random_float_export( flMinVal , flMaxVal );
		}

		auto Utils::RandomFloatExp( float flMinVal , float flMaxVal , float flExponent ) -> float
		{
			using RandomFloatExp_t = float( __cdecl * )( float , float , float );
			static auto random_float_exp_export = (RandomFloatExp_t)GetProcAddress( GetModuleHandleA( XorStr( "vstdlib.dll" ) ) , XorStr( "RandomFloatExp" ) );
			return random_float_exp_export( flMinVal , flMaxVal , flExponent );
		}
	}
}