#include "Matrix.hpp"
#include "QAngle.hpp"
#include "Vector3.hpp"

auto matrix3x4_t::At( unsigned int index ) -> float*
{
	return m_data[ index ];
}

auto matrix3x4_t::At( unsigned int index ) const -> const float*
{
	return m_data[ index ];
}

auto matrix3x4_t::operator [] ( unsigned int index ) -> float*
{
	return At( index );
}

auto matrix3x4_t::operator [] ( unsigned int index ) const -> const float*
{
	return At( index );
}


auto VMatrix::At( unsigned int index ) -> float*
{
	return m_data[ index ];
}

auto VMatrix::At( unsigned int index ) const -> const float*
{
	return m_data[ index ];
}

auto VMatrix::operator [] ( unsigned int index ) -> float*
{
	return At( index );
}

auto VMatrix::operator [] ( unsigned int index ) const -> const float*
{
	return At( index );
}

auto AngleNormalize( float angle ) -> float
{
	angle = std::fmod( angle, 360.f );

	if( angle > 180.f )
		angle -= 360.f;
	else if( angle < -180.f )
		angle += 360.f;

	return angle;
}