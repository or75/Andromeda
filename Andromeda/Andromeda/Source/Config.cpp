#include "Config.hpp"

#include "../Include/Rapidjson/rapidjson.h"
#include "../Include/Rapidjson/prettywriter.h"
#include "../Include/Rapidjson/document.h"
#include "../Include/Rapidjson/istreamwrapper.h"
#include "../Include/Rapidjson/ostreamwrapper.h"
#include "../Include/Rapidjson/error/en.h"

namespace source
{
	using namespace rapidjson;

	void LoadConfig( const char* file_name )
	{
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();

		ifstream cfg_file( dll_dir + file_name );
		Document doc_config;
		IStreamWrapper isw( cfg_file );

		doc_config.ParseStream( isw );

		if ( doc_config.HasParseError() )
		{
			Andromeda::WriteDebugLog( "[error] load json: %s (%i)\n" , GetParseError_En( doc_config.GetParseError() ) , doc_config.GetErrorOffset() );
		}

		doc_config.Clear();

		cfg_file.clear();
		cfg_file.close();
	}

	void SaveConfig( const char* file_name )
	{
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();

		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;

		ofstream cfg_file( dll_dir + file_name );

		Document doc_config;
		Value array( rapidjson::kArrayType );
		Document::AllocatorType& allocator = doc_config.GetAllocator();

		doc_config.SetObject();

		doc_config.AddMember( "Name" , "XYZ" , allocator );
		doc_config.AddMember( "Rollnumer" , 2 , allocator );
		doc_config.AddMember( "array" , array , allocator );

		if ( doc_config.HasParseError() )
		{
			Andromeda::WriteDebugLog( "[error] save json: %s (%i)\n" , GetParseError_En( doc_config.GetParseError() ) , doc_config.GetErrorOffset() );
		}
		else
		{
			OStreamWrapper osw( cfg_file );
			PrettyWriter<OStreamWrapper> cfg_writer( osw );

			cfg_writer.SetIndent( '\t' , 1 );
			cfg_writer.SetFormatOptions( PrettyFormatOptions::kFormatSingleLineArray );

			doc_config.Accept( cfg_writer );
		}

		cfg_file.clear();
		cfg_file.close();
	}
}