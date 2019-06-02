#include "Client.hpp"
#include "Engine.hpp"
#include "Config.hpp"

#include "Engine/Render.hpp"
#include "Engine/Script.hpp"

#include "Feature/Gui.hpp"
#include "Feature/Menu.hpp"
#include "Feature/Notification.hpp"
#include "Feature/Utils.hpp"

namespace source
{
	int iScreenWidth = 0;
	int iScreenHeight = 0;

	bool ClientInit = false;

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

		auto& script = engine::ScriptManager::Instance();
		auto& notify = feature::Notification::Instance();
		auto& utils = feature::Utils::Instance();

		if ( !script.Create() )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #ScriptManager\n" ) );
			return false;
		}

		if ( !utils.Create() )
		{
			Andromeda::WriteDebugLog( XorStr( "[error] #Utils\n" ) );
			return false;
		}

		m_engine_client->GetScreenSize( iScreenWidth , iScreenHeight );
		
		// Загрузим настройки
		LoadConfig( XorStr( CONFIG_FILE ) , false );
		//SaveConfig( XorStr( CONFIG_FILE ) , false );

		// Выполняем Init во всех скриптах
		script.OnInit();

		feature::Menu::Instance().Create();
		feature::Gui::Instance().AddMenuRenderFn( OnRenderMenu );

		if ( config::settings::main::WelcomeMsg )
			notify.AddNotification( 10 , feature::nt_info , XorStr( "Welcome to Andromeda Hack !" ) );

		ClientInit = true;
		return true;
	}

	auto OnRenderMenu() -> void
	{
		auto& menu = feature::Menu::Instance();

		menu.RenderStartMenu();
		menu.RenderSettingsMenu();
		menu.RenderAboutMenu();
	}

	auto OnRender() -> void
	{
		if ( ClientInit )
		{
			auto& render = engine::Render::Instance();
			auto& gui = feature::Gui::Instance();

			gui.Begin();
			render.Begin();

			feature::Notification::Instance().RenderNotification();
			engine::ScriptManager::Instance().OnRender();

			render.End();
			gui.End();
		}
	}

	auto OnCreateMove( float flInputSampleTime , CUserCmd* pCmd ) -> void
	{
		engine::ScriptManager::Instance().OnCreateMove( pCmd );
	}
}
