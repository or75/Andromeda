#pragma once

#include "../Valve/SDK.hpp"

namespace source
{
	auto Create() -> bool;
	auto Destroy() -> void;

	extern Andromeda::Memory::SwapVmt ClientMode;
	extern Andromeda::Memory::SwapVmt Surface;

	extern IBaseClientDLL* m_base_client;
	extern IClientEntityList* m_client_entity_list;
	extern IVEngineClient* m_engine_client;
	extern IGameEventManager* m_game_event_manager;
	extern IEngineSound* m_engine_sound;
	extern IFileSystem* m_file_system;
	extern IEngineTrace* m_engine_trace;
	extern CGlobalVarsBase* m_globals;
	extern CBaseClientState* m_client_state;	
	extern ICvar* m_cvar;
	extern IClientMode* m_clientmode;
	extern IInputSystem* m_input_system;
	extern ISurface* m_surface;

	extern IDirect3DDevice9* m_direct_device;

	// Procedure

	bool WorldToScreen( const Vector3& vOrigin , Vector2& vScreen );
	bool WorldToScreen( const Vector3& vOrigin , int& x , int& y );
}