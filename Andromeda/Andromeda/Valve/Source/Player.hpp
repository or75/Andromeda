#pragma once

#include "Entity.hpp"

auto ToBasePlayer( C_BaseEntity* base_entity ) -> C_BasePlayer*;
auto ToCSPlayer( C_BaseEntity* base_entity ) -> C_CSPlayer*;

class C_BasePlayer : public C_BaseCombatCharacter
{
public:
	static auto GetBasePlayer( int index ) -> C_BasePlayer*;

public:
	auto EyePosition() -> Vector3;

public:
	netprop( char , m_lifeState , XorStr( "DT_BasePlayer" ) , XorStr( "m_lifeState" ) );
	netprop( int , m_fFlags , XorStr( "DT_BasePlayer" ) , XorStr( "m_fFlags" ) );
	netprop( int , m_iHealth , XorStr( "DT_BasePlayer" ) , XorStr( "m_iHealth" ) );
	netprop( int , m_nTickBase , XorStr( "DT_BasePlayer" ) , XorStr( "m_nTickBase" ) );
	netprop( Vector3 , m_vecVelocity , XorStr( "DT_BasePlayer" ) , XorStr( "m_vecVelocity[0]" ) );
	netprop( Vector3 , m_vecViewOffset , XorStr( "DT_BasePlayer" ) , XorStr( "m_vecViewOffset[0]" ) );
	netprop( QAngle , m_aimPunchAngle , XorStr( "DT_BasePlayer" ) , XorStr( "m_aimPunchAngle" ) );
	netprop( QAngle , m_viewPunchAngle , XorStr( "DT_BasePlayer" ) , XorStr( "m_viewPunchAngle" ) );
	netprop( int , m_iObserverMode , XorStr( "DT_BasePlayer" ) , XorStr( "m_iObserverMode" ) );
	netprop_raw( const char* , m_szLastPlaceName , XorStr( "DT_BasePlayer" ) , XorStr( "m_szLastPlaceName" ) );
	netprop_ex( QAngle , v_angle , XorStr( "DT_BasePlayer" ) , XorStr( "deadflag" ) , 0x04 );
};

class C_CSPlayer : public C_BasePlayer
{
public:
	static auto GetCSPlayer( int index ) -> C_CSPlayer*;
	static auto GetLocalCSPlayer() -> C_CSPlayer*;

public:
	auto IsLocal() -> bool;
	auto IsGood() -> bool;

public:
	netprop( bool , m_bIsDefusing , XorStr( "DT_CSPlayer" ) , XorStr( "m_bIsDefusing" ) );
	netprop( bool , m_bHasHelmet , XorStr( "DT_CSPlayer" ) , XorStr( "m_bHasHelmet" ) );
	netprop( bool , m_bHasHeavyArmor , XorStr( "DT_CSPlayer" ) , XorStr( "m_bHasHeavyArmor" ) );
	netprop( int , m_ArmorValue , XorStr( "DT_CSPlayer" ) , XorStr( "m_ArmorValue" ) );
	netprop( int , m_iShotsFired , XorStr( "DT_CSPlayer" ) , XorStr( "m_iShotsFired" ) );
	netprop( float , m_flFlashMaxAlpha , XorStr( "DT_CSPlayer" ) , XorStr( "m_flFlashMaxAlpha" ) );
	netprop( QAngle , m_angEyeAngles , XorStr( "DT_CSPlayer" ) , XorStr( "m_angEyeAngles[0]" ) );
};