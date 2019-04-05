#pragma once

#include "../Valve/SDK.hpp"

#include "../Valve/Source/Entity.hpp"
#include "../Valve/Source/Player.hpp"
#include "../Valve/Source/Weapon.hpp"

namespace source
{
	extern int iScreenWidth;
	extern int iScreenHeight;

	auto OnInit() -> bool;
	auto OnRenderMenu() -> void;
	auto OnRender() -> void;
	auto OnCreateMove( float flInputSampleTime , CUserCmd* pCmd ) -> void;
}