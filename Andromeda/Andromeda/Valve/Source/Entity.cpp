#include "Entity.hpp"

auto C_BaseEntity::GetBaseEntity( int index ) -> C_BaseEntity*
{
	auto client_entity = source::m_client_entity_list->GetClientEntity( index );
	return ( client_entity ? client_entity->GetBaseEntity() : nullptr );
}