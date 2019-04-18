#pragma once

#include "../../Valve/SDK.hpp"

namespace source
{
	namespace engine
	{
		class NetProp : public Singleton< NetProp >
		{
		public:
			auto Create( IBaseClientDLL* base_client ) -> bool;
			auto Destroy() -> void;

			auto Get( const string& array_name , const string& prop_name , RecvProp** prop_out = nullptr ) -> int;

		private:
			auto Get( RecvTable* recv_entry , const string& prop_name , RecvProp** prop_out = nullptr ) -> int;
			auto GetArrayEntry( const string& array_name ) -> RecvTable*;

		private:
			struct StoredPropData
			{
				//RecvProp* prop_ptr = nullptr;
				uint16_t class_relative_offset = 0;
			};

		private:
			vector< RecvTable* > m_recv_array = { };
			map<uint32_t , StoredPropData> m_props = { };
		};
	}
}