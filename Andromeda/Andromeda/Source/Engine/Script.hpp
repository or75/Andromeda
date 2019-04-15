#pragma once

#include "../Engine.hpp"

//constexpr auto EXECUTE_TIMEOUT = 250;

namespace source
{
	namespace engine
	{
		class ScriptModule
		{
		public:
			ScriptModule( asIScriptModule* script_module );

			auto ExecuteScriptContext( asIScriptContext* script_context ) -> bool;

			auto CallInit() -> bool;
			auto CallRender() -> bool;
			auto CallFireGameEvent( IGameEvent* pEvent ) -> bool;
			auto CallCreateMove( float flInputSampleTime , CUserCmd* pCmd ) -> bool;

			auto PrepareContextFromPool( asIScriptFunction* script_function ) -> asIScriptContext*;
			auto ReturnContextToPool( asIScriptContext* script_context ) -> void;

			auto Unload() -> void;

		public:
			bool				m_enable;
			DWORD				m_timeout;
			asIScriptModule*	m_script_module;

			asIScriptContext*	m_script_context_init;
			asIScriptContext*	m_script_context_render;
			asIScriptContext*	m_script_context_event;
			asIScriptContext*	m_script_context_move;

			vector<asIScriptContext*>	m_contexts_list;
		};

		class ScriptSystem
		{
		public:
			auto Create() -> bool;
			auto Destroy() -> void;

			auto BuildModule( string file_name , string module_name ) -> ScriptModule*;
			auto AddModule( ScriptModule* script_module ) -> bool;

			auto UnloadAll() -> void;

		public:
			vector<ScriptModule>	m_module_list;
			asIScriptEngine*	m_script_engine;
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