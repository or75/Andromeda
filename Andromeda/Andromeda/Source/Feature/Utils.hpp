#pragma once

#include "../Client.hpp"

namespace source
{
	namespace feature
	{
		class Utils : public Singleton< Utils >
		{
		public:
			auto Create() -> bool;

			auto SetClanTag( string tag ) -> void;

			auto RandomInt( int iMinVal , int iMaxVal ) -> int;
			auto RandomFloat( float flMinVal = 0.0f , float flMaxVal = 1.0f ) -> float;
			auto RandomFloatExp( float flMinVal = 0.0f , float flMaxVal = 1.0f , float flExponent = 1.0f ) -> float;

		private:
			using SetClanTag_t = void( __fastcall* )( const char* , const char* );

		private:
			SetClanTag_t SetClanTagFn;
		};
	}
}