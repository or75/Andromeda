#pragma once

#include "../../Valve/SDK.hpp"

namespace source
{
	namespace engine
	{
		class Factory
		{
			using factory_map_t = unordered_map< string , void* >;

		public:
			Factory( const string& image_name );

		public:
			template< typename T >
			inline auto Get( const string& name )
			{
				auto factory = m_factory_map[name];
				return (T)factory;
			}

		private:
			auto CreateFactoryMap() -> void;

		private:
			HMODULE m_image = nullptr;
			InterfaceReg* m_interface_array = nullptr;
			factory_map_t m_factory_map = { };
		};

	}
}