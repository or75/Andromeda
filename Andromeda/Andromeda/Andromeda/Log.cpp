#include "Log.hpp"

namespace Andromeda
{
	auto Log::Create() -> bool
	{
		m_log_file.clear();
	
		auto& Loader = ImageLoader::Instance();

		m_log_file = Loader.GetDllDir() + XorStr( LOG_ERROR_FILE );

		return true;
	}

	auto Log::Destroy() -> void
	{
		CloseHandle( m_file_handle );
		
		m_log_file.clear();	
		m_file_handle = nullptr;
	}

	auto Log::AddLog( const char* data ) -> void
	{
		if ( !m_log_file.empty() )
		{
			if ( m_file_handle == INVALID_HANDLE_VALUE )
				m_file_handle = CreateFileA( m_log_file.c_str() , GENERIC_WRITE , FILE_SHARE_READ , 0 , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , 0 );

			if ( m_file_handle )
				WriteFile( m_file_handle , data , lstrlenA( data ) , 0 , 0 );
		}
	}

	auto ansi_to_unicode( const string& ansi ) -> wstring
	{
		wchar_t unicode[2048] = { 0 };
		MultiByteToWideChar( CP_UTF8 , 0u , ansi.c_str() , -1 , unicode , 2048 );
		return unicode;
	}

	auto unicode_to_ansi( const wstring& unicode ) -> string
	{
		char ansi[2048] = { 0 };
		WideCharToMultiByte( CP_UTF8 , 0u , unicode.c_str() , -1 , ansi , 2048 , nullptr , nullptr );
		return ansi;
	}

	auto str_wide_to_str_unicode( const string& ansi ) -> string
	{
		wchar_t unicode[2048] = { 0 };
		MultiByteToWideChar( CP_ACP , 0u , ansi.c_str() , -1 , unicode , 2048 );
		return unicode_to_ansi( wstring( unicode ) );
	}

	void WriteDebugLog( const char* format , ... )
	{
		auto& log = Log::Instance();

		char pszMessage[1024] = { 0 };

		va_list args;
		va_start( args , format );
		vsprintf_s( pszMessage , format , args );

		log.AddLog( pszMessage );

		va_end( args );
	}
}