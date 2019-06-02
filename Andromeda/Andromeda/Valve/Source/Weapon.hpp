#pragma once

#include "Entity.hpp"

namespace index
{
	namespace C_WeaponCSBase
	{
		constexpr auto GetCSWeaponData = 454;
	}
}

class C_BaseCombatWeapon : public C_BaseEntity
{
public:
	netprop( int , m_iClip1 , XorStr( "DT_BaseCombatWeapon" ) , XorStr( "m_iClip1" ) );
	netprop( float , m_flNextPrimaryAttack , XorStr( "DT_BaseCombatWeapon" ) , XorStr( "m_flNextPrimaryAttack" ) );
	netprop( float , m_flNextSecondaryAttack , XorStr( "DT_BaseCombatWeapon" ) , XorStr( "m_flNextSecondaryAttack" ) );
	netprop( int , m_iPrimaryReserveAmmoCount , XorStr( "DT_BaseCombatWeapon" ) , XorStr( "m_iPrimaryReserveAmmoCount" ) );
};

class C_WeaponCSBase : public C_BaseCombatWeapon
{
public:
	auto IsFireTime() -> bool;
	auto IsSecondaryFireTime() -> bool;

	auto GetCSWeaponData() -> CCSWeaponInfo*;
};