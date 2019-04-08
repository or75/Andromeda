#pragma once

#include "../Include/Auto.hpp"

namespace Andromeda
{
	class Log : public Singleton< Log >
	{
	public:
		auto Create() -> bool;
		auto Destroy() -> void;

		auto AddLog( const char* data ) -> void;

	private:
		string m_log_file = "";
		HANDLE m_file_handle = INVALID_HANDLE_VALUE;
	};

	auto ansi_to_unicode( const string& ansi ) -> wstring;
	auto unicode_to_ansi( const wstring& unicode )->string;
	auto str_wide_to_str_unicode( const string& ansi )->string;

	void WriteDebugLog( const char * format , ... );
}