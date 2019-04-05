#pragma once

#include "../Engine.hpp"

constexpr auto EXECUTE_TIMEOUT = 8000;

namespace source
{
	namespace engine
	{
		class ScriptModule
		{
		public:
			ScriptModule( asIScriptModule* script_module );

			auto PrepateFunctionContext( asIScriptContext* script_context , asIScriptModule* script_module , const char* function_decl ) -> asIScriptFunction*;
			auto ExecuteScriptContext( asIScriptContext* script_context , asIScriptModule* script_module ) -> bool;

			auto CallInit() -> bool;
			auto CallRender() -> bool;
			auto CallFireGameEvent( IGameEvent* pEvent ) -> bool;
			auto CallCreateMove( float flInputSampleTime , CUserCmd* pCmd ) -> bool;

			auto Unload() -> void;

		public:
			bool				m_enable;
			DWORD				m_timeout;
			asIScriptContext*	m_script_context;
			asIScriptModule*	m_script_module;
		};

		class ScriptSystem
		{
		public:
			auto Create() -> bool;
			auto Destroy() -> void;

			auto BuildModule( string file_name , string module_name ) -> ScriptModule*;
			auto AddModule( ScriptModule* script_module ) -> bool;

		public:
			vector<ScriptModule>	m_module_list;
			asIScriptEngine*		m_script_engine;
		};

		class ScriptManager : public Singleton<ScriptManager>
		{
		public:
			auto Create() -> bool;
			auto Destroy() -> void;

			auto OnInit() -> void;
			auto OnRender() -> void;
			auto OnFireGameEvent( IGameEvent* pEvent ) -> void;
			auto OnCreateMove( float flInputSampleTime , CUserCmd* pCmd ) -> void;

		public:
			auto UpdateScriptList( bool call_new_init = false ) -> void;

		public:
			ScriptSystem*	m_script_system;
			vector<string>	m_script_list;
			string			m_script_dir;
		};
	}
}