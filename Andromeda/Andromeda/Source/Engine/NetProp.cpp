#include "NetProp.hpp"

namespace source
{
	namespace engine
	{
		uint32_t crc32c( const char* s )
		{
			int i = 0;
			uint32_t crc = 0;
			do
			{
				crc ^= (uint8_t)( *s++ | 0x20 );
				for ( i = 0; i < 8; i++ )
					crc = ( crc >> 1 ) ^ ( 0x82F63B78 * ( crc & 1 ) );
			} while ( *( s - 1 ) != 0 );
			return crc;
		}

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

		auto NetProp::Get( const string& array_name , const string& prop_name , RecvProp** prop_out /*= nullptr*/ ) -> int
		{
			auto hash_name = array_name + "->" + prop_name;
			auto hash = crc32c( hash_name.c_str() );

			if ( m_props.find( hash ) != m_props.end() )
			{
				return m_props[hash].class_relative_offset;
			}
			else
			{
				RecvProp* local_prop = nullptr;

				auto array_entry = GetArrayEntry( array_name );
				auto offset = Get( array_entry , prop_name , &local_prop );
			
				if ( prop_out )
					*prop_out = local_prop;

				m_props[hash].class_relative_offset = offset;

				return offset;
			}

			return 0;
		}

		auto NetProp::Get( RecvTable* recv_entry , const string& prop_name , RecvProp** prop_out /*= nullptr*/ ) -> int
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
						*prop_out = prop;

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