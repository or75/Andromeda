#include "Client.hpp"
#include "Engine.hpp"
#include "Config.hpp"

#include "Engine/Render.hpp"
#include "Engine/Script.hpp"

#include "Feature/Gui.hpp"
#include "Feature/Menu.hpp"
#include "Feature/Notification.hpp"

namespace source
{
	int iScreenWidth = 0;
	int iScreenHeight = 0;

	class ClientEventListener : IGameEventListener
	{
	public:
		ClientEventListener()
		{
			m_game_event_manager->AddListenerGlobal( this , false );
		}

		~ClientEventListener()
		{
			m_game_event_manager->RemoveListener( this );
		}

		auto FireGameEvent( IGameEvent* pEvent ) -> void
		{
			if ( pEvent && !pEvent->GetName().empty() )
			{
				engine::ScriptManager::Instance().OnFireGameEvent( pEvent );
			}
		}

		auto GetEventDebugID() -> int
		{
			return EVENT_DEBUG_ID_INIT;
		}
	};

	ClientEventListener* m_client_event_listener = nullptr;

	auto OnInit() -> bool
	{
		m_client_event_listener = new ClientEventListener();

		auto& loader = Andromeda::ImageLoader::Instance();
		auto& script = engine::ScriptManager::Instance();
		auto& notify = feature::Notification::Instance();

		if ( !script.Create() )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #ScriptManager\n" ) );
			return false;
		}

		m_engine_client->GetScreenSize( iScreenWidth , iScreenHeight );
		
		// Загрузим настройки
		LoadConfig( XorStr( CONFIG_FILE ) );
		//SaveConfig( XorStr( CONFIG_FILE ) );

		// Выполняем Init во всех скриптах
		script.OnInit();

		feature::Menu::Instance().Create();
		feature::Gui::Instance().AddMenuRenderFn( OnRenderMenu );

		notify.AddNotification( 10 , feature::nt_warning , XorStr( "Welcome to Andromeda Hack !" ) );

		return true;
	}

	auto OnRenderMenu() -> void
	{
		auto& gui = feature::Gui::Instance();
		auto& menu = feature::Menu::Instance();

		if ( gui.m_gui_open )
			menu.RenderStartMenu();
	}

	auto OnRender() -> void
	{
		auto& render = engine::Render::Instance();
		auto& gui = feature::Gui::Instance();
		auto& menu = feature::Menu::Instance();
		auto& notification = feature::Notification::Instance();

		gui.Begin();
		render.Begin();

		notification.RenderNotification();

		engine::ScriptManager::Instance().OnRender();

		render.End();
		gui.End();
	}

	auto OnCreateMove( float flInputSampleTime , CUserCmd* pCmd ) -> void
	{
		engine::ScriptManager::Instance().OnCreateMove( flInputSampleTime , pCmd );
	}
}
