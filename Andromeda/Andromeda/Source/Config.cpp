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
		auto dll_dir = Andromeda::ImageLoader::Instance().GetDllDir();

		json json_config = 
		{
			{"pi", 3.141},
			{"happy", true},
			{"name", "Niels"},
			{"nothing", nullptr},
			{"answer",{
				{"everything", 42}
			}},
				{"list", {1, 0, 2}},
				{"object", {
					{"currency", "USD"},
					{"value", 42.99}
			}}
		};

		ofstream cfg_file( file_name );

		cfg_file << json_config.dump( 1 , '\t' ) << endl;

		cfg_file.close();
		cfg_file.clear();
	}
}