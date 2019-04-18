#pragma once

#include "../SDK.hpp"

#include "../../Source/Engine.hpp"
#include "../../Source/Engine/NetProp.hpp"

#pragma region region_index

namespace index
{
	namespace IHandleEntity
	{
		constexpr auto SetRefEHandle = 1;
		constexpr auto GetRefEHandle = 2;
	}
	namespace IClientUnknown
	{
		constexpr auto GetCollideable = 3;
		constexpr auto GetClientNetworkable = 4;
		constexpr auto GetClientRenderable = 5;
		constexpr auto GetIClientEntity = 6;
		constexpr auto GetBaseEntity = 7;
	}
	namespace ICollideable
	{
		constexpr auto OBBMins = 1;
		constexpr auto OBBMaxs = 2;
	}
	namespace IClientNetworkable
	{
		constexpr auto GetClientClass = 2;
		constexpr auto IsDormant = 9;
		constexpr auto entindex = 10;
	}
	namespace IClientRenderable
	{
		constexpr auto GetModel = 8;
		constexpr auto DrawModel = 9;
		constexpr auto SetupBones = 13;
	}
	namespace IClientEntity
	{
		constexpr auto GetAbsOrigin = 10;
		constexpr auto GetAbsAngles = 11;
	}
	namespace C_BaseEntity
	{
		constexpr auto IsPlayer = 153;
	}
}

#pragma endregion

#pragma region region_netprop

#define netprop( type, name, array_name, prop_name ) \
auto name ## () const -> type& \
{ \
	static auto offset = source::engine::NetProp::Instance().Get( array_name, prop_name ); \
	return *( type* )( this + offset ); \
}

#define netprop_raw( type, name, array_name, prop_name ) \
auto name ## () const -> type \
{ \
	static auto offset = source::engine::NetProp::Instance().Get( array_name, prop_name ); \
	return ( type )( this + offset ); \
}

#define netprop_ex( type, name, array_name, prop_name, extra ) \
auto name ## () const -> type& \
{ \
	static auto offset = source::engine::NetProp::Instance().Get( array_name, prop_name ) + extra; \
	return *( type* )( this + offset ); \
}

#pragma endregion

class IHandleEntity
{
public:
	auto SetRefEHandle( const CBaseHandle& handle ) -> void
	{
		VirtualFn( void )( IHandleEntity* , const CBaseHandle& );
		return Andromeda::Memory::vget< Fn >( this , index::IHandleEntity::SetRefEHandle )( this , handle );
	}

	auto GetRefEHandle() -> const CBaseHandle &
	{
		VirtualFn( CBaseHandle& )( IHandleEntity* );
		return Andromeda::Memory::vget< Fn >( this , index::IHandleEntity::GetRefEHandle )( this );
	}
};

class IClientUnknown : public IHandleEntity
{
public:
	auto GetCollideable() -> ICollideable*
	{
		VirtualFn( ICollideable* )( IClientUnknown* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientUnknown::GetCollideable )( this );
	}

	auto GetClientNetworkable() -> IClientNetworkable*
	{
		VirtualFn( IClientNetworkable* )( IClientUnknown* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientUnknown::GetClientNetworkable )( this );
	}

	auto GetClientRenderable() -> IClientRenderable*
	{
		VirtualFn( IClientRenderable* )( IClientUnknown* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientUnknown::GetClientRenderable )( this );
	}

	auto GetIClientEntity() -> IClientEntity*
	{
		VirtualFn( IClientEntity* )( IClientUnknown* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientUnknown::GetIClientEntity )( this );
	}

	auto GetBaseEntity() -> C_BaseEntity*
	{
		VirtualFn( C_BaseEntity* )( IClientUnknown* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientUnknown::GetBaseEntity )( this );
	}
};

class ICollideable
{
public:
	auto OBBMins() -> Vector3&
	{
		VirtualFn( Vector3& )( ICollideable* );
		return Andromeda::Memory::vget< Fn >( this , index::ICollideable::OBBMins )( this );
	}

	auto OBBMaxs() -> Vector3&
	{
		VirtualFn( Vector3& )( ICollideable* );
		return Andromeda::Memory::vget< Fn >( this , index::ICollideable::OBBMaxs )( this );
	}
};

class IClientNetworkable
{
public:
	auto GetClientClass() -> ClientClass*
	{
		VirtualFn( ClientClass* )( IClientNetworkable* );
		return  Andromeda::Memory::vget< Fn >( this , index::IClientNetworkable::GetClientClass )( this );
	}

	auto IsDormant() -> bool
	{
		VirtualFn( bool )( IClientNetworkable* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientNetworkable::IsDormant )( this );
	}

	auto entindex() -> int
	{
		VirtualFn( int )( IClientNetworkable* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientNetworkable::entindex )( this );
	}
};

class IClientRenderable
{
public:
	auto GetModel() -> const model_t*
	{
		VirtualFn( model_t* )( IClientRenderable* );
		return  Andromeda::Memory::vget< Fn >( this , index::IClientRenderable::GetModel )( this );
	}

	auto DrawModel( int flags , const int& instasnce ) -> int
	{
		VirtualFn( int )( IClientRenderable* , int , const int& );
		return Andromeda::Memory::vget< Fn >( this , index::IClientRenderable::DrawModel )( this , flags , instasnce );
	}

	auto SetupBones( matrix3x4_t* pBoneToWorld , int nMaxBones , int boneMask , float currentTime ) -> bool
	{
		VirtualFn( bool )( IClientRenderable* , matrix3x4_t* , int , int , float );
		return Andromeda::Memory::vget< Fn >( this , index::IClientRenderable::SetupBones )( this , pBoneToWorld , nMaxBones , boneMask , currentTime );
	}
};

class IClientEntity : public IClientUnknown
{
public:
	auto OBBMins() -> const Vector3&
	{
		return GetCollideable()->OBBMins();
	}

	auto OBBMaxs() -> const Vector3&
	{
		return GetCollideable()->OBBMaxs();
	}

public:
	auto GetClientClass() -> ClientClass*
	{
		return GetClientNetworkable()->GetClientClass();
	}

	auto IsDormant() -> bool
	{
		return GetClientNetworkable()->IsDormant();
	}

	auto entindex() -> int
	{
		return GetClientNetworkable()->entindex();
	}

public:
	auto GetModel() -> const model_t*
	{
		return GetClientRenderable()->GetModel();
	}

	auto DrawModel( int flags , const int& instance ) -> int
	{
		return GetClientRenderable()->DrawModel( flags , instance );
	}

	auto SetupBones( matrix3x4_t* pBoneToWorld , int nMaxBones , int boneMask , float currentTime ) -> bool
	{
		return GetClientRenderable()->SetupBones( pBoneToWorld , nMaxBones , boneMask , currentTime );
	}

public:
	auto GetAbsOrigin() -> const Vector3&
	{
		VirtualFn( Vector3& )( IClientEntity* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientEntity::GetAbsOrigin )( this );
	}

	auto GetAbsAngles() -> const QAngle&
	{
		VirtualFn( QAngle& )( IClientEntity* );
		return Andromeda::Memory::vget< Fn >( this , index::IClientEntity::GetAbsAngles )( this );
	}

public:

	template<typename T>
	auto SetNetProp( T value , string dt_name , string prop ) -> void
	{
		auto offset = source::engine::NetProp::Instance().Get( dt_name , prop );

		if ( offset )
			*(T*)( this + offset ) = value;
	}

	template<typename T>
	auto GetNetProp( string dt_name , string prop , DWORD custom_offset = 0 ) -> T
	{
		if ( custom_offset )
			return *(T*)( this + custom_offset );
		else
		{
			auto offset = source::engine::NetProp::Instance().Get( dt_name , prop );

			//Andromeda::WriteDebugLog( "GetNetProp (%s) (0x%X)\n" , prop.c_str() , offset );

			if ( offset )
				return *(T*)( this + offset );
		}

		return (T)0;
	}

	string GetNetPropString( string dt_name , string prop , DWORD custom_offset = 0 )
	{
		if ( !custom_offset )
		{
			auto offset = source::engine::NetProp::Instance().Get( dt_name , prop );

			if ( offset )
			{
				string str = "";
				str = (const char*)( this + offset );
				return str;
			}
		}
		else
		{
			string str = "";
			str = (const char*)( this + custom_offset );
			return str;
		}

		return string( "" );
	}
};

class C_BaseEntity : public IClientEntity
{
public:
	static auto GetBaseEntity( int index ) -> C_BaseEntity*;

public:
	auto IsPlayer() -> bool
	{
		VirtualFn( bool )( C_BaseEntity* );
		return Andromeda::Memory::vget< Fn >( this , index::C_BaseEntity::IsPlayer )( this );
	}

public:
	netprop( int , m_iTeamNum , XorStr( "DT_BaseEntity" ) , XorStr( "m_iTeamNum" ) );
	netprop( Vector3 , m_vecOrigin , XorStr( "DT_BaseEntity" ) , XorStr( "m_vecOrigin" ) );

public:
	netprop_ex( const matrix3x4_t , m_rgflCoordinateFrame , XorStr( "DT_BaseEntity" ) , XorStr( "m_CollisionGroup" ) , -0x0030 );
};

class C_BaseAnimating : public C_BaseEntity
{
public:
	netprop( int , m_nHitboxSet , XorStr( "DT_BaseAnimating" ) , XorStr( "m_nHitboxSet" ) );
	netprop( int , m_nSequence , XorStr( "DT_BaseAnimating" ) , XorStr( "m_nSequence" ) );
	netprop( float , m_flCycle , XorStr( "DT_BaseAnimating" ) , XorStr( "m_flCycle" ) );
};

class C_BaseCombatCharacter : public C_BaseAnimating
{
public:
	auto GetActiveWeapon() -> C_WeaponCSBase*
	{
		auto handle = m_hActiveWeapon();
		return reinterpret_cast<C_WeaponCSBase*>( source::m_client_entity_list->GetClientEntityFromHandle( handle ) );
	}

public:
	netprop( CBaseHandle , m_hActiveWeapon , XorStr( "DT_BaseCombatCharacter" ) , XorStr( "m_hActiveWeapon" ) );
	netprop( float , m_flNextAttack , XorStr( "DT_BaseCombatCharacter" ) , XorStr( "m_flNextAttack" ) );
};