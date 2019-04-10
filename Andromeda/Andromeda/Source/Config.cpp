#include "Config.hpp"

#include "../Include/Rapidjson/rapidjson.h"
#include "../Include/Rapidjson/prettywriter.h"
#include "../Include/Rapidjson/filewritestream.h"

namespace source
{
	using namespace rapidjson;

	void LoadConfig( const char* file_name )
	{
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();

		/*
		try
		{
			Json::Value json_config;

			ifstream cfg_file( dll_dir + file_name );

			cfg_file >> json_config;

			cfg_file.clear();
			cfg_file.close();

			json_config.clear();
		}
		catch ( exception& e )
		{
			Andromeda::WriteDebugLog( "[error] json parse error: %s\n" , e.what() );
		}
		*/
	}

	void SaveConfig( const char* file_name )
	{
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();

		StringBuffer s;
		PrettyWriter<StringBuffer> writer( s );

		writer.SetIndent( '\t' , 1 );
		writer.SetFormatOptions( kFormatSingleLineArray );

		writer.StartObject(); 
		writer.Key( "hello" );
		writer.String( "world" );
		writer.Key( "t" );
		writer.Bool( true );
		writer.Key( "f" );
		writer.Bool( false );
		writer.Key( "n" );
		writer.Null();
		writer.Key( "i" );
		writer.Uint( 123 );
		writer.Key( "pi" );
		writer.Double( 3.1416 );
		writer.Key( "a" );
		writer.StartArray();

		for ( unsigned i = 0; i < 4; i++ )
			writer.Uint( i );

		writer.EndArray();
		writer.EndObject();

		ofstream cfg_file( dll_dir + file_name );

		cfg_file << s.GetString();

		cfg_file.clear();
		cfg_file.close();
	}
}