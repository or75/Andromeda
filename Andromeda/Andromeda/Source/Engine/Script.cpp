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

		auto ScriptModule::PrepateFunctionContext( asIScriptModule* script_module , const char* function_decl ) -> asIScriptFunction*
		{
			if ( !m_enable )
				return nullptr;

			auto& script_system = engine::ScriptManager::Instance().m_script_system;

			asIScriptFunction* script_function = script_module->GetFunctionByDecl( function_decl );

			if ( script_function )
			{
				m_script_context = script_system->PrepareContextFromPool( script_function );

				//script_context->Unprepare();
				//script_context->Prepare( script_function );

				return script_function;
			}

			return nullptr;
		}

		auto ScriptModule::ExecuteScriptContext( asIScriptModule* script_module ) -> bool
		{
			if ( !m_enable || !m_script_context )
				return true;

			auto& script_system = engine::ScriptManager::Instance().m_script_system;
			auto& notify = feature::Notification::Instance();

			m_timeout = GetTickCount() + config::script::Timeout;

			m_script_context->SetLineCallback( asFUNCTION( LineCallback ) , &m_timeout , asCALL_CDECL );

			auto r = m_script_context->Execute();

			if ( r != asEXECUTION_FINISHED )
			{
				if ( r == asEXECUTION_EXCEPTION )
				{
					notify.AddNotification( 10 , feature::nt_error , XorStr( "(%s) An exception '%s' occurred. The script was forcibly disabled.\n" ) , script_module->GetName() , m_script_context->GetExceptionString() );
					
					Andromeda::WriteDebugLog( XorStr( "[error] (%s) An exception '%s' occurred. The script was forcibly disabled.\n" ) , script_module->GetName() , m_script_context->GetExceptionString() );

					asIScriptFunction* script_function_exception = m_script_context->GetExceptionFunction();

					if ( script_function_exception )
					{
						Andromeda::WriteDebugLog( XorStr( "declaration: %s\n" ) , script_function_exception->GetDeclaration() );
						Andromeda::WriteDebugLog( XorStr( "module: %s\n" ) , script_function_exception->GetModuleName() );
						Andromeda::WriteDebugLog( XorStr( "section: %s\n" ) , script_function_exception->GetScriptSectionName() );
						Andromeda::WriteDebugLog( XorStr( "line: %i\n" ) , m_script_context->GetExceptionLineNumber() );
					}
				}
				else if ( r == asEXECUTION_ERROR )
				{
					notify.AddNotification( 10 , feature::nt_error , XorStr( "(%s) An error '%s' occurred. The script was forcibly disabled.\n" ) , script_module->GetName() , m_script_context->GetExceptionString() );
					
					Andromeda::WriteDebugLog( XorStr( "[error] (%s) An error '%s' occurred. The script was forcibly disabled.\n" ) , script_module->GetName() , m_script_context->GetExceptionString() );
				}
				else if ( r == asEXECUTION_ABORTED )
				{
					notify.AddNotification( 10 , feature::nt_warning , XorStr( "Module (%s) context aborted. The script was forcibly disabled.\n" ) , script_module->GetName() );
					
					Andromeda::WriteDebugLog( XorStr( "[warning] Module (%s) context aborted. The script was forcibly disabled.\n" ) , script_module->GetName() );
				}

				script_system->ReturnContextToPool( m_script_context );

				m_enable = false;

				return false;
			}

			script_system->ReturnContextToPool( m_script_context );

			return true;
		}

		ScriptModule::ScriptModule( asIScriptModule* script_module )
		{
			auto m_script_engine = script_module->GetEngine();

			if ( m_script_engine )
			{
				m_script_context = nullptr;
				m_script_module = script_module;

				m_enable = true;
			}
		}

		auto ScriptModule::CallInit() -> bool
		{
			if ( !m_script_module || !m_enable )
				return false;

			asIScriptFunction* script_function = PrepateFunctionContext( m_script_module , XorStr( "void Init()" ) );

			if ( script_function )
				return ExecuteScriptContext( m_script_module );

			return false;
		}

		auto ScriptModule::CallRender() -> bool
		{
			if ( !m_script_module || !m_enable )
				return false;

			asIScriptFunction* script_function = PrepateFunctionContext( m_script_module , XorStr( "void Render()" ) );

			if ( script_function )
				return ExecuteScriptContext( m_script_module );

			return false;
		}

		auto ScriptModule::CallFireGameEvent( IGameEvent* pEvent ) -> bool
		{
			if ( !m_script_module || !m_enable )
				return false;

			asIScriptFunction* script_function = PrepateFunctionContext( m_script_module , XorStr( "void FireGameEvent(IGameEvent@)" ) );

			if ( script_function )
			{		
				m_script_context->SetArgObject( 0 , pEvent );

				return ExecuteScriptContext( m_script_module );
			}

			return false;
		}

		auto source::engine::ScriptModule::CallCreateMove( float flInputSampleTime , CUserCmd* pCmd ) -> bool
		{
			if ( !m_script_module || !m_enable )
				return false;

			asIScriptFunction* script_function = PrepateFunctionContext( m_script_module , XorStr( "void CreateMove(float,CUserCmd@)" ) );

			if ( script_function )
			{
				m_script_context->SetArgFloat( 0 , flInputSampleTime );
				m_script_context->SetArgObject( 1 , pCmd );

				return ExecuteScriptContext( m_script_module );
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
			
			for ( auto& context : m_contexts_list )
			{
				context->Suspend();
				context->Release();
			}
			
			m_module_list.clear();
			m_contexts_list.clear();
		}

		auto ScriptSystem::PrepareContextFromPool( asIScriptFunction* script_function ) -> asIScriptContext*
		{
			asIScriptContext* m_script_context = nullptr;

			if ( m_contexts_list.size() )
			{
				m_script_context = *m_contexts_list.rbegin();
				m_contexts_list.pop_back();
			}
			else
				m_script_context = m_script_engine->CreateContext();

			m_script_context->Prepare( script_function );

			return m_script_context;
		}

		auto ScriptSystem::ReturnContextToPool( asIScriptContext* script_context ) -> void
		{
			script_context->Unprepare();

			m_contexts_list.push_back( script_context );
		}

		auto ScriptManager::Create() -> bool
		{
			m_script_system = new ScriptSystem();

			if ( m_script_system->Create() )
			{
				m_script_dir = Andromeda::ImageLoader::Instance().GetDllDir() + XorStr( "script\\" );
				
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
						auto call_ok = csgo_script.CallInit();
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
						auto call_ok = csgo_script.CallRender();
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
						auto call_ok = csgo_script.CallFireGameEvent( pEvent );
				}
			}
		}

		auto source::engine::ScriptManager::OnCreateMove( float flInputSampleTime , CUserCmd* pCmd ) -> void
		{
			if ( m_script_system )
			{
				for ( auto& csgo_script : m_script_system->m_module_list )
				{
					if ( csgo_script.m_enable )
						auto call_ok = csgo_script.CallCreateMove( flInputSampleTime , pCmd );
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
								m_script_system->AddModule( script_module );

								if ( call_new_init )
								{
									script_module->CallInit();
								}
							}
						}
					}
				} while ( FindNextFileA( hSearch , &wfd ) );

				FindClose( hSearch );
			}

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