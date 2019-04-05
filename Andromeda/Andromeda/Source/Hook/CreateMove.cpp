#include "Hook.hpp"

#include "../Client.hpp"

namespace source
{
	namespace hook
	{
		auto WINAPI Hook_CreateMove( float flInputSampleTime , CUserCmd* cmd ) -> bool
		{
			source::ClientMode.Set( false );

			OnCreateMove( flInputSampleTime , cmd );

			bool ret = source::m_clientmode->CreateMove( flInputSampleTime , cmd );

			source::ClientMode.Set( true );

			return ret;
		}
	}
}