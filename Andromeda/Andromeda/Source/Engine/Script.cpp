#include "Script.hpp"
#include "ScriptReg.hpp"

#include "../Feature/Notification.hpp"

namespace source
{
	namespace engine
	{
		void LineCallback( asIScriptContext* script_context , DWORD* time_out )
		{
			if ( *time_out < GetTickCount() )
			{
				Andromeda::WriteDebugLog( "[LineCallback] (%s) Timeout: %s\n" , 
										  script_context->GetFunction()->GetModuleName() ,
										  script_context->GetFunction()->GetDeclaration() );

				script_context->Abort();
			}
		}

		auto MessageCallback( const asSMessageInfo* msg , void* param ) -> void
		{
			//auto& notify = feature::Notification::Instance();

			if ( msg->type == asMSGTYPE_ERROR )
			{
				//notify.AddNotification( 10 , feature::nt_error , XorStr( "(%d, %d) : %s\n" ) , msg->row , msg->col , msg->message );
				Andromeda::WriteDebugLog( XorStr( "[error] (%d, %d) : %s\n" ) , msg->row , msg->col , msg->message );
			}
			else if ( msg->type == asMSGTYPE_WARNING )
			{
				//notify.AddNotification( 10 , feature::nt_warning , XorStr( "(%d, %d) : %s\n" ) , msg->row , msg->col , msg->message );
				Andromeda::WriteDebugLog( XorStr( "[warning] (%d, %d) : %s\n" ) , msg->row , msg->col , msg->message );
			}
			else if ( msg->type == asMSGTYPE_INFORMATION )
			{
				//notify.AddNotification( 10 , feature::nt_info , XorStr( "(%d, %d) : %s\n" ) , msg->row , msg->col , msg->message );
				Andromeda::WriteDebugLog( XorStr( "[info] (%d, %d) : %s\n" ) , msg->row , msg->col , msg->message );
			}
		}

		auto ScriptModule::ExecuteScriptContext( asIScriptContext* script_context ) -> bool
		{
			if ( !m_enable || !script_context )
				return true;

			auto module_name_notifi = Andromeda::str_wide_to_str_unicode( m_script_module->GetName() );

			auto& notify = feature::Notification::Instance();

			m_timeout = GetTickCount() + config::script::Timeout;
			
			script_context->SetLineCallback( asFUNCTION( LineCallback ) , &m_timeout , asCALL_CDECL );
			
			auto r = script_context->Execute();

			if ( r != asEXECUTION_FINISHED )
			{
				if ( r == asEXECUTION_EXCEPTION )
				{
					notify.AddNotification( 10 , feature::nt_error , XorStr( "(%s) An exception '%s' occurred. The script was forcibly disabled.\n" ) , module_name_notifi.c_str() , script_context->GetExceptionString() );
					
					Andromeda::WriteDebugLog( XorStr( "[error] (%s) An exception '%s' occurred. The script was forcibly disabled.\n" ) , module_name_notifi.c_str() , script_context->GetExceptionString() );

					asIScriptFunction* script_function_exception = script_context->GetExceptionFunction();

					if ( script_function_exception )
					{
						Andromeda::WriteDebugLog( XorStr( "declaration: %s\n" ) , script_function_exception->GetDeclaration() );
						Andromeda::WriteDebugLog( XorStr( "module: %s\n" ) , script_function_exception->GetModuleName() );
						Andromeda::WriteDebugLog( XorStr( "section: %s\n" ) , script_function_exception->GetScriptSectionName() );
						Andromeda::WriteDebugLog( XorStr( "line: %i\n" ) , script_context->GetExceptionLineNumber() );
					}
				}
				else if ( r == asEXECUTION_ERROR )
				{
					notify.AddNotification( 10 , feature::nt_error , XorStr( "(%s) An error '%s' occurred. The script was forcibly disabled.\n" ) , module_name_notifi.c_str() , script_context->GetExceptionString() );
					
					Andromeda::WriteDebugLog( XorStr( "[error] (%s) An error '%s' occurred. The script was forcibly disabled.\n" ) , module_name_notifi.c_str() , script_context->GetExceptionString() );
				}
				else if ( r == asEXECUTION_ABORTED )
				{
					notify.AddNotification( 10 , feature::nt_warning , XorStr( "Module (%s) context aborted. The script was forcibly disabled.\n" ) , module_name_notifi.c_str() );
					
					Andromeda::WriteDebugLog( XorStr( "[warning] Module (%s) context aborted. The script was forcibly disabled.\n" ) , module_name_notifi.c_str() );
				}

				m_enable = false;

				return false;
			}

			script_context->Unprepare();

			return true;
		}

		ScriptModule::ScriptModule( asIScriptModule* script_module )
		{
			auto m_script_engine = script_module->GetEngine();

			if ( m_script_engine )
			{
				m_enable = true;
				m_script_module = script_module;

				m_script_function_init = m_script_module->GetFunctionByDecl( XorStr( "void Init()" ) );
				m_script_function_render = m_script_module->GetFunctionByDecl( XorStr( "void Render()" ) );
				m_script_function_event = m_script_module->GetFunctionByDecl( XorStr( "void FireGameEvent(IGameEvent@)" ) );
				m_script_function_move = m_script_module->GetFunctionByDecl( XorStr( "void CreateMove(CUserCmd@)" ) );
			}
		}

		auto ScriptModule::CallInit( asIScriptContext* script_context ) -> bool
		{
			if ( !m_enable || !m_script_function_init )
				return false;

			if ( script_context )
			{
				script_context->Prepare( m_script_function_init );

				return ExecuteScriptContext( script_context );
			}

			return false;
		}

		auto ScriptModule::CallRender( asIScriptContext* script_context ) -> bool
		{
			if ( !m_enable || !m_script_function_render )
				return false;

			if ( script_context )
			{
				script_context->Prepare( m_script_function_render );

				return ExecuteScriptContext( script_context );
			}

			return false;
		}

		auto ScriptModule::CallFireGameEvent( asIScriptContext* script_context , IGameEvent* pEvent ) -> bool
		{
			if ( !m_enable || !m_script_function_event )
				return false;

			if ( script_context )
			{
				script_context->Prepare( m_script_function_event );
				script_context->SetArgObject( 0 , pEvent );
				
				return ExecuteScriptContext( script_context );
			}

			return false;
		}

		auto source::engine::ScriptModule::CallCreateMove( asIScriptContext* script_context , CUserCmd* pCmd ) -> bool
		{
			if ( !m_enable || !m_script_function_move )
				return false;

			if (  script_context )
			{
				script_context->Prepare( m_script_function_move );
				script_context->SetArgObject( 0 , pCmd );

				return ExecuteScriptContext( script_context );
			}

			return false;
		}

		auto ScriptModule::Unload() -> void
		{
			m_enable = false;
			m_script_module = nullptr;
		}

		auto ScriptSystem::Create() -> bool
		{
			m_script_engine = asCreateScriptEngine();

			if ( !m_script_engine )
				return false;

			m_script_engine->SetMessageCallback( asFUNCTION( MessageCallback ) , 0 , asCALL_CDECL );

			RegisterScriptDateTime( m_script_engine );
			RegisterScriptAny( m_script_engine );
			RegisterScriptArray( m_script_engine , true );
			RegisterStdString( m_script_engine );
			RegisterStdStringUtils( m_script_engine );
			RegisterScriptDictionary( m_script_engine );
			RegisterScriptMath( m_script_engine );
			RegisterExceptionRoutines( m_script_engine );

			RegisterScriptAndromeda( m_script_engine );

			return true;
		}

		auto ScriptSystem::Destroy() -> void
		{
			UnloadAll();

			m_script_engine->ClearMessageCallback();
			m_script_engine->ShutDownAndRelease();
		}

		auto ScriptSystem::BuildModule( string file_name , string module_name ) -> ScriptModule*
		{
			auto& notify = feature::Notification::Instance();

			auto module_name_notifi = Andromeda::str_wide_to_str_unicode( module_name );

			CScriptBuilder builder;

			int r = builder.StartNewModule( m_script_engine , module_name.c_str() );

			if ( r < 0 )
			{
				notify.AddNotification( 10 , feature::nt_error , XorStr( "#1 (%s) Unrecoverable error while starting a new module." ) , module_name_notifi.c_str() );
				Andromeda::WriteDebugLog( XorStr( "(%s) Unrecoverable error while starting a new module.\n" ) , module_name.c_str() );
				return nullptr;
			}

			r = builder.AddSectionFromFile( file_name.c_str() );

			if ( r < 0 )
			{
				notify.AddNotification( 10 , feature::nt_error , XorStr( "#2 (%s) Please correct the errors in the script and try again." ) , module_name_notifi.c_str() );
				Andromeda::WriteDebugLog( XorStr( "(%s) Please correct the errors in the script and try again.\n" ) , module_name.c_str() );
				return nullptr;
			}

			r = builder.BuildModule();

			if ( r < 0 )
			{
				notify.AddNotification( 10 , feature::nt_error , XorStr( "#4 (%s) Please correct the errors in the script and try again." ) , module_name_notifi.c_str() );
				Andromeda::WriteDebugLog( XorStr( "(%s) Please correct the errors in the script and try again.\n" ) , module_name.c_str() );
				return nullptr;
			}

			asIScriptModule* script_module = m_script_engine->GetModule( module_name.c_str() );

			if ( !script_module )
			{
				notify.AddNotification( 10 , feature::nt_error , XorStr( "#4 (%s) Can't find module" ) , module_name_notifi.c_str() );
				Andromeda::WriteDebugLog( XorStr( "can't find module from name (%s)\n" ) , module_name.c_str() );
				return nullptr;
			}

			return new ScriptModule( script_module );
		}

		auto ScriptSystem::AddModule( ScriptModule* script_module ) -> bool
		{
			if ( script_module )
			{
				m_module_list.push_back( *script_module );
				return true;
			}

			return false;
		}

		auto ScriptSystem::UnloadAll() -> void
		{
			for ( auto& module : m_module_list )
				module.Unload();

			m_module_list.clear();
		}

		auto ScriptManager::Create() -> bool
		{
			m_script_system = new ScriptSystem();

			if ( m_script_system->Create() )
			{
				m_script_dir = Andromeda::ImageLoader::Instance().GetDllDir() + XorStr( "script\\" );

				m_script_context_init = m_script_system->m_script_engine->CreateContext();
				m_script_context_render = m_script_system->m_script_engine->CreateContext();
				m_script_context_event = m_script_system->m_script_engine->CreateContext();
				m_script_context_move = m_script_system->m_script_engine->CreateContext();
				
				UpdateScriptList();

				return true;
			}

			return false;
		}

		auto ScriptManager::Destroy() -> void
		{
			m_script_dir.clear();

			if ( m_script_system )
			{
				m_script_context_init->Release();
				m_script_context_init = nullptr;

				m_script_context_render->Release();
				m_script_context_render = nullptr;

				m_script_context_event->Release();
				m_script_context_event = nullptr;

				m_script_context_move->Release();
				m_script_context_move = nullptr;

				m_script_system->Destroy();
				m_script_system = nullptr;
			}

			m_script_list.clear();
		}

		auto ScriptManager::OnInit() -> void
		{
			if ( m_script_system )
			{
				for ( auto& csgo_script : m_script_system->m_module_list )
				{
					if ( csgo_script.m_enable )
					{
						auto call_ok = csgo_script.CallInit( m_script_context_init );
					}
				}
			}
		}

		auto ScriptManager::OnRender() -> void
		{
			if ( m_script_system )
			{
				for ( auto& csgo_script : m_script_system->m_module_list )
				{
					if ( csgo_script.m_enable )
					{
						auto call_ok = csgo_script.CallRender( m_script_context_render );
					}
				}
			}
		}

		auto ScriptManager::OnFireGameEvent( IGameEvent* pEvent ) -> void
		{
			if ( m_script_system )
			{
				for ( auto& csgo_script : m_script_system->m_module_list )
				{
					if ( csgo_script.m_enable )
					{
						auto call_ok = csgo_script.CallFireGameEvent( m_script_context_event , pEvent );
					}
				}
			}
		}

		auto source::engine::ScriptManager::OnCreateMove( CUserCmd* pCmd ) -> void
		{
			if ( m_script_system )
			{
				for ( auto& csgo_script : m_script_system->m_module_list )
				{
					if ( csgo_script.m_enable )
					{
						auto call_ok = csgo_script.CallCreateMove( m_script_context_move , pCmd );
					}
				}
			}
		}

		auto ScriptManager::UpdateScriptList( bool call_new_init ) -> void
		{
			m_script_list.clear();

			HANDLE hSearch = 0;
			WIN32_FIND_DATA wfd = { 0 };

			auto script_dir_mask = m_script_dir + XorStr( "*.as" );

			if ( ( hSearch = FindFirstFileA( script_dir_mask.c_str() , &wfd ) ) != INVALID_HANDLE_VALUE )
			{
				do
				{
					if ( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
					{
						string file_name = wfd.cFileName;
						string full_path_file_name = m_script_dir + file_name;

						m_script_list.push_back( file_name );

						bool module_added = false;

						for ( auto& csgo_script : m_script_system->m_module_list )
						{
							string name = csgo_script.m_script_module->GetName();

							if ( file_name == name )
							{
								module_added = true;
								break;
							}
						}

						if ( !module_added )
						{
							auto script_module = m_script_system->BuildModule( full_path_file_name , file_name );

							if ( script_module )
							{
								script_module->m_full_path = full_path_file_name;
								
								m_script_system->AddModule( script_module );

								if ( call_new_init )
								{
									auto call_ok = script_module->CallInit( m_script_context_init );
								}
							}
						}
					}
				} while ( FindNextFileA( hSearch , &wfd ) );

				FindClose( hSearch );
			}

			//m_script_system->m_script_jit->finalizePages();

			// copy sounds
			string script_sound_dir = m_script_dir + XorStr( "sound\\" );
			string csgo_sound_dir = m_engine_client->GetGameDirectory() + XorStr( "sound\\andromeda\\" );

			if ( filesystem::exists( script_sound_dir ) )
			{
				filesystem::remove_all( csgo_sound_dir );
				filesystem::create_directory( csgo_sound_dir );
				filesystem::copy( script_sound_dir , csgo_sound_dir , filesystem::copy_options::recursive );
			}
		}
	}
}