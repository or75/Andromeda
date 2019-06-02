#pragma once

#include "../Include/Auto.hpp"

namespace config
{
	namespace settings
	{
		namespace main
		{
			extern int WelcomeMsg;
			extern int Timeout;
			extern int MenuKey;
		}
	}
}

namespace source
{
	void LoadConfig( string file_name , bool notifi = true );
	void SaveConfig( string file_name , bool notifi = true );
}