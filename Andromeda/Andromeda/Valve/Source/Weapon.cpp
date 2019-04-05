#include "Weapon.hpp"

auto C_WeaponCSBase::IsFireTime() -> bool
{
	return ( source::m_globals->curtime >= m_flNextPrimaryAttack() );
}

auto C_WeaponCSBase::IsSecondaryFireTime() -> bool
{
	return ( source::m_globals->curtime >= m_flNextSecondaryAttack() );
}

auto C_WeaponCSBase::GetCSWeaponData() -> CCSWeaponInfo*
{
	VirtualFn( CCSWeaponInfo* )( C_WeaponCSBase* );
	return Andromeda::Memory::vget< Fn >( this , index::C_WeaponCSBase::GetCSWeaponData )( this );
}
