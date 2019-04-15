#include "Engine.hpp"
#include "Client.hpp"

#include "Engine/Factory.hpp"
#include "Engine/NetProp.hpp"
#include "Engine/Render.hpp"
#include "Engine/Script.hpp"

#include "Feature/Gui.hpp"

#include "Hook/Hook.hpp"

namespace source
{
	DWORD_PTR* Present_o;
	DWORD_PTR* Reset_o;

	Andromeda::Memory::SwapVmt ClientMode;
	Andromeda::Memory::SwapVmt Surface;

	IBaseClientDLL* m_base_client = nullptr;
	IClientEntityList* m_client_entity_list = nullptr;
	IVEngineClient* m_engine_client = nullptr;
	IGameEventManager* m_game_event_manager = nullptr;
	IEngineSound* m_engine_sound = nullptr;
	IFileSystem* m_file_system = nullptr;
	IEngineTrace* m_engine_trace = nullptr;
	CGlobalVarsBase* m_globals = nullptr;
	CBaseClientState* m_client_state = nullptr;
	ICvar* m_cvar = nullptr;
	IClientMode* m_clientmode = nullptr;
	IInputSystem* m_input_system = nullptr;
	ISurface* m_surface = nullptr;

	IDirect3DDevice9* m_direct_device = nullptr;

	auto Create() -> bool
	{
		auto& loader = Andromeda::ImageLoader::Instance();
		auto& log = Andromeda::Log::Instance();
		auto& input = Andromeda::Input::Instance();
		auto& netprop = engine::NetProp::Instance();
		auto& render = engine::Render::Instance();
		auto& gui = feature::Gui::Instance();

		if ( !log.Create() )
			return false;

		auto TimeOut = 0;

		while ( !GetModuleHandleA( XorStr( "ServerBrowser.dll" ) ) )
		{
			if ( TimeOut >= 30 )
			{
				Andromeda::WriteDebugLog( XorStr( "[error] #TimeOut\n" ) );
				return false;
			}

			Sleep( 1000 );
			TimeOut++;
		}

		if ( !input.Create() )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #InputCreate\n" ) );
			return false;
		}

		engine::Factory factory_client( XorStr( "client_panorama.dll" ) );
		engine::Factory factory_engine( XorStr( "engine.dll" ) );
		engine::Factory factory_filesystem_stdio( XorStr( "filesystem_stdio.dll" ) );
		engine::Factory factory_vstdlib( XorStr( "vstdlib.dll" ) );
		engine::Factory factory_inputsystem( XorStr( "inputsystem.dll" ) );
		engine::Factory factory_vguimatsurface( XorStr( "vguimatsurface.dll" ) );

		m_base_client = factory_client.Get< IBaseClientDLL* >( XorStr( "VClient" ) );
		m_client_entity_list = factory_client.Get< IClientEntityList* >( XorStr( "VClientEntityList" ) );
		m_engine_client = factory_engine.Get< IVEngineClient* >( XorStr( "VEngineClient" ) );
		m_game_event_manager = factory_engine.Get< IGameEventManager* >( XorStr( "GAMEEVENTSMANAGER" ) );
		m_engine_sound = factory_engine.Get< IEngineSound* >( XorStr( "IEngineSoundClient" ) );
		m_file_system = factory_filesystem_stdio.Get< IFileSystem* >( XorStr( "VFileSystem" ) );
		m_engine_trace = factory_engine.Get< IEngineTrace* >( XorStr( "EngineTraceClient" ) );
		m_globals = (CGlobalVarsBase*)Andromeda::Memory::FindPattern( XorStr( "client_panorama.dll" ) , XorStr( "A1 ? ? ? ? 5E 8B 40 10" ) , 1 , 2 );
		m_client_state = **(CBaseClientState***)( ( *(PDWORD_PTR*)m_engine_client )[12] + 0x0010 );
		m_cvar = factory_vstdlib.Get< ICvar* >( XorStr( "VEngineCvar" ) );
		m_clientmode = (IClientMode*)Andromeda::Memory::FindPattern( XorStr( "client_panorama.dll" ) , XorStr( "B9 ? ? ? ? 83 38 02 75 0D" ) , 1 , 1 );
		m_input_system = (IInputSystem*)factory_inputsystem.Get< IInputSystem* >( XorStr( "InputSystemVersion" ) );
		m_surface = (ISurface*)factory_vguimatsurface.Get< ISurface* >( XorStr( "VGUI_Surface" ) );

		m_direct_device = (IDirect3DDevice9*)Andromeda::Memory::FindPattern( XorStr( "shaderapidx9.dll" ) , XorStr( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) , 1 , 2 );

#if ENABLE_DEBUG_INTERFACE == 1
		Andromeda::WriteDebugLog( XorStr( "[m_base_client] %p\n" ) , m_base_client );
		Andromeda::WriteDebugLog( XorStr( "[m_client_entity_list] %p\n" ) , m_client_entity_list );
		Andromeda::WriteDebugLog( XorStr( "[m_engine_client] %p\n" ) , m_engine_client );
		Andromeda::WriteDebugLog( XorStr( "[m_game_event_manager] %p\n" ) , m_game_event_manager );
		Andromeda::WriteDebugLog( XorStr( "[m_engine_sound] %p\n" ) , m_engine_sound );
		Andromeda::WriteDebugLog( XorStr( "[m_file_system] %p\n" ) , m_file_system );
		Andromeda::WriteDebugLog( XorStr( "[m_engine_trace] %p\n" ) , m_engine_trace );
		Andromeda::WriteDebugLog( XorStr( "[m_globals] %p\n" ) , m_globals );
		Andromeda::WriteDebugLog( XorStr( "[m_client_state] %p\n" ) , m_client_state );
		Andromeda::WriteDebugLog( XorStr( "[m_cvar] %p\n" ) , m_cvar );
		Andromeda::WriteDebugLog( XorStr( "[m_clientmode] %p\n" ) , m_clientmode );
		Andromeda::WriteDebugLog( XorStr( "[m_input_system] %p\n" ) , m_input_system );
		Andromeda::WriteDebugLog( XorStr( "[m_surface] %p\n" ) , m_surface );

		Andromeda::WriteDebugLog( XorStr( "[m_direct_device] %p\n" ) , m_direct_device );
#endif

		if ( !m_globals )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #GlobalVars\n" ) );
			return false;
		}

		if ( !m_clientmode )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #ClientMode\n" ) );
			return false;
		}

		if ( !m_input_system )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #InputSystem\n" ) );
			return false;
		}

		if ( !m_surface )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #Surface\n" ) );
			return false;
		}

		if ( !netprop.Create( m_base_client ) )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #NetPropCreate\n" ) );
			return false;
		}

		if ( !m_direct_device )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #DirectDevice\n" ) );
			return false;
		}

		if ( !render.Create( m_direct_device ) )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #Renderer\n" ) );
			return false;
		}

		if ( !gui.Create( m_direct_device , input ) )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #Gui\n" ) );
			return false;
		}

		Present_o = (DWORD_PTR*)Andromeda::Memory::FindPattern( XorStr( "gameoverlayrenderer.dll" ) , XorStr( "FF 15 ? ? ? ? 8B F8 85 DB" ) , 2 , 1 );
		Reset_o = (DWORD_PTR*)Andromeda::Memory::FindPattern( XorStr( "gameoverlayrenderer.dll" ) , XorStr( "C7 45 ? ? ? ? ? FF 15 ? ? ? ? 8B F8" ) , 9 , 1 );

		if ( !Present_o || !Reset_o )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #RenderHook\n" ) );
			return false;
		}

		if ( !ClientMode.Create( m_clientmode ) )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #ClientModeHook\n" ) );
			return false;
		}

		if ( !Surface.Create( m_surface ) )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #SurfaceHook\n" ) );
			return false;
		}

		hook::Present_o = ( hook::Present_t )( *Present_o );
		hook::Reset_o = ( hook::Reset_t )( *Reset_o );

		*Present_o = (DWORD_PTR)&hook::Hook_Present;
		*Reset_o = (DWORD_PTR)&hook::Hook_Reset;

		ClientMode.Hook( hook::Hook_CreateMove , index::IClientMode::CreateMove );
		Surface.Hook( hook::Hook_LockCursor , index::ISurface::LockCursor );

		return OnInit();
	}

	auto Destroy() -> void
	{
		auto& log = Andromeda::Log::Instance();
		auto& input = Andromeda::Input::Instance();
		auto& netprop = engine::NetProp::Instance();
		auto& render = engine::Render::Instance();
		auto& script = engine::ScriptManager::Instance();

		if ( Present_o )
			*Present_o = (DWORD_PTR)&hook::Present_o;

		if ( Reset_o )
			*Reset_o = (DWORD_PTR)&hook::Reset_o;

		script.Destroy();	
		netprop.Destroy();
		input.Destroy();	
		render.Destroy();
		log.Destroy();
	}

	bool WorldToScreen( const Vector3& vOrigin , Vector2& vScreen )
	{
		//static auto p_view_matrix = (PVOID)( reinterpret_cast<DWORD>( &source::m_engine_client->WorldToScreenMatrix() ) + 0x40 );

		//if ( p_view_matrix && !IsBadReadPtr( p_view_matrix , sizeof( PVOID ) ) )
		//{
			//if ( IsBadReadPtr( (PVOID)( *(PDWORD)p_view_matrix ) , sizeof( PDWORD ) ) )
			//	return false;

			//VMatrix& ViewMatrix = *(VMatrix*)( ( *(PDWORD)p_view_matrix ) + 0x3DC );

			//static const VMatrix& ViewMatrix = *(VMatrix*)( ( *(PDWORD)p_view_matrix ) + 0x3DC ); //  //source::m_engine_client->WorldToScreenMatrix();
			static const VMatrix& ViewMatrix = source::m_engine_client->WorldToScreenMatrix();

			Vector2 vTempScreen;

			vTempScreen[0] = ViewMatrix[0][0] * vOrigin[0] + ViewMatrix[0][1] * vOrigin[1] + ViewMatrix[0][2] * vOrigin[2] + ViewMatrix[0][3];
			vTempScreen[1] = ViewMatrix[1][0] * vOrigin[0] + ViewMatrix[1][1] * vOrigin[1] + ViewMatrix[1][2] * vOrigin[2] + ViewMatrix[1][3];

			float w = ViewMatrix[3][0] * vOrigin[0] + ViewMatrix[3][1] * vOrigin[1] + ViewMatrix[3][2] * vOrigin[2] + ViewMatrix[3][3];

			if ( w < 0.001f )
				return false;

			float invw = 1.0f / w;

			vTempScreen.m_x *= invw;
			vTempScreen.m_y *= invw;

			int iScreenWidth = 0 , iScreenHeight = 0;

			m_engine_client->GetScreenSize( iScreenWidth , iScreenHeight );

			float x = static_cast<float>( iScreenWidth ) / 2.f;
			float y = static_cast<float>( iScreenHeight ) / 2.f;

			x += 0.5f * vTempScreen[0] * static_cast<float>( iScreenWidth ) + 0.5f;
			y -= 0.5f * vTempScreen[1] * static_cast<float>( iScreenHeight ) + 0.5f;

			vScreen[0] = x;
			vScreen[1] = y;

			return true;
		//}

		//return false;
	}

	bool WorldToScreen( const Vector3& vOrigin , int& x , int& y )
	{
		Vector2 vScreen;

		if ( WorldToScreen( vOrigin , vScreen ) )
		{
			x = static_cast<int>( vScreen[0] );
			y = static_cast<int>( vScreen[1] );

			return true;
		}

		x = 0;
		y = 0;

		return false;
	}
}