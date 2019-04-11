#pragma once

#include "../Include/Auto.hpp"

namespace config
{

}

namespace source
{
	void LoadConfig( string file_name , bool notifi = true );
	void SaveConfig( string file_name , bool notifi = true );
}