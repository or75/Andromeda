#include "Player.hpp"

auto ToBasePlayer( C_BaseEntity* base_entity ) -> C_BasePlayer*
{
	if ( !base_entity || !base_entity->IsPlayer() )
		return nullptr;

	return reinterpret_cast<C_BasePlayer*>( base_entity );
}

auto ToCSPlayer( C_BaseEntity* base_entity ) -> C_CSPlayer*
{
	return reinterpret_cast<C_CSPlayer*>( ToBasePlayer( base_entity ) );
}

auto C_BasePlayer::GetBasePlayer( int index ) -> C_BasePlayer*
{
	auto base_entity = GetBaseEntity( index );
	return ToBasePlayer( base_entity );
}

auto C_BasePlayer::EyePosition() -> Vector3
{
	return ( m_vecOrigin() + m_vecViewOffset() );
}

auto C_CSPlayer::GetCSPlayer( int index ) -> C_CSPlayer*
{
	auto base_entity = GetBaseEntity( index );
	return ToCSPlayer( base_entity );
}

auto C_CSPlayer::GetLocalCSPlayer() -> C_CSPlayer*
{
	auto index = source::m_engine_client->GetLocalPlayer();
	return GetCSPlayer( index );
}

auto C_CSPlayer::IsLocal() -> bool
{
	return ( entindex() == source::m_engine_client->GetLocalPlayer() );
}

auto C_CSPlayer::IsGood() -> bool
{
	return ( this && m_lifeState() == LIFE_ALIVE && m_iHealth() > 0 && !IsDormant() );
}