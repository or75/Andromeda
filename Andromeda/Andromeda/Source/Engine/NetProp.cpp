#include "NetProp.hpp"

namespace source
{
	namespace engine
	{
		auto NetProp::Create( IBaseClientDLL* base_client ) -> bool
		{
			m_recv_array.clear();

			if ( !base_client )
				return false;

			for ( auto entry = base_client->GetAllClasses(); entry; entry = entry->m_pNext )
			{
				auto recv_entry = entry->m_pRecvTable;

				if ( recv_entry )
					m_recv_array.emplace_back( recv_entry );
			}

			return !m_recv_array.empty();
		}

		auto NetProp::Destroy() -> void
		{
			m_recv_array.clear();
		}

		auto NetProp::Get( const string & array_name , const string & prop_name , RecvProp * *prop_out /*= nullptr*/ ) -> int
		{
			auto array_entry = GetArrayEntry( array_name );
			return Get( array_entry , prop_name , prop_out );
		}

		auto NetProp::Get( RecvTable * recv_entry , const string & prop_name , RecvProp * *prop_out /*= nullptr*/ ) -> int
		{
			if ( !recv_entry )
				return 0;

			auto extra = 0;

			for ( auto index = 0; index < recv_entry->m_nProps; index++ )
			{
				auto prop = &recv_entry->m_pProps[index];
				auto child = prop->m_pDataTable;

				if ( child && child->m_nProps )
				{
					auto add = Get( child , prop_name , prop_out );

					if ( add )
						extra += ( prop->m_Offset + add );
				}

				if ( !prop_name.compare( prop->m_pVarName ) )
				{
					if ( prop_out )
						* prop_out = prop;

					return ( prop->m_Offset + extra );
				}
			}

			return extra;
		}

		auto NetProp::GetArrayEntry( const string & array_name ) -> RecvTable*
		{
			for ( auto& entry : m_recv_array )
			{
				if ( !array_name.compare( entry->m_pNetTableName ) )
					return entry;
			}

			return nullptr;
		}
	}
}