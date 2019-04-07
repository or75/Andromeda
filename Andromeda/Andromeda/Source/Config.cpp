#include "Config.hpp"

#include "../Include/Json/json.hpp"

using json = nlohmann::json;

namespace source
{
	void LoadConfig( const char* file_name )
	{

	}

	void SaveConfig( const char* file_name )
	{
		json jcfg = 
		{
		  {"pi", 3.141},
		  {"happy", true},
		  {"name", "Niels"},
		  {"nothing", nullptr},
		  {"answer", {
			{"everything", 42}
		  }},
		  {"list", {1, 0, 2}},
		  {"object", {
			{"currency", "USD"},
			{"value", 42.99}
		  }}
		};

		ofstream cfg_file( file_name );

		cfg_file << std::setw( 4 ) << jcfg << std::endl;

		//Andromeda::WriteDebugLog( "cfg:\n%s\n" , cfg.dump( 1 , '\t' ).c_str() );
	}
}