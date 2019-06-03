#pragma once

#include "../Engine.hpp"

namespace source
{
	namespace engine
	{
		class ScriptModule;

		struct ScriptCategory
		{
			string name = "";
			vector<ScriptModule*> modules;
		};

		class ScriptModule
		{
		public:
			ScriptModule( asIScriptModule* script_module );

			auto ExecuteScriptContext( asIScriptContext* script_context ) -> bool;

			auto CallInit( asIScriptContext* script_context ) -> bool;
			auto CallRender( asIScriptContext* script_context ) -> bool;
			auto CallFireGameEvent( asIScriptContext* script_context , IGameEvent* pEvent ) -> bool;
			auto CallCreateMove( asIScriptContext* script_context , CUserCmd* pCmd ) -> bool;

			auto Unload() -> void;

		public:
			bool				m_enable;
			DWORD				m_timeout;
			asIScriptModule*	m_script_module;
			string				m_full_path;
			string				m_category;

			asIScriptFunction*	m_script_function_init;
			asIScriptFunction*	m_script_function_initex;
			asIScriptFunction*	m_script_function_render;
			asIScriptFunction*	m_script_function_event;
			asIScriptFunction*	m_script_function_move;
		};

		class ScriptSystem
		{
		public:
			auto Create() -> bool;
			auto Destroy() -> void;

			auto BuildModule( string file_name , string module_name ) -> ScriptModule*;
			auto AddModule( ScriptModule* script_module ) -> bool;

			auto UnloadAll() -> void;
			auto UnloadModule( ScriptModule* script_module ) -> void;

		public:
			vector<ScriptModule*>	m_module_list;
			vector<ScriptCategory>	m_module_cat_list;
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
			auto OnCreateMove( CUserCmd* pCmd ) -> void;

		public:
			auto UpdateScriptList( bool call_new_init = false ) -> void;

		public:
			ScriptSystem*	m_script_system;
			string			m_script_dir;

			asIScriptContext*	m_script_context_init;
			asIScriptContext*	m_script_context_render;
			asIScriptContext*	m_script_context_event;
			asIScriptContext*	m_script_context_move;
		};
	}
}