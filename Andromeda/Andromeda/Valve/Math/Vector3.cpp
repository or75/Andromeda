#include "Matrix.hpp"
#include "QAngle.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

Vector3::Vector3()
	: m_x( 0.f )
	, m_y( 0.f )
	, m_z( 0.f )
{ }

Vector3::Vector3( float x, float y, float z )
	: m_x( x )
	, m_y( y )
	, m_z( z )
{ }

Vector3::Vector3( const Vector3& vector )
	: m_x( vector[ 0 ] )
	, m_y( vector[ 1 ] )
	, m_z( vector[ 2 ] )
{ }

Vector3::Vector3( const float* vector )
	: m_x( vector[ 0 ] )
	, m_y( vector[ 1 ] )
	, m_z( vector[ 2 ] )
{ }

auto Vector3::Init( float x /* = 0.f */, float y /* = 0.f */, float z /* = 0.f */ ) -> void
{
	m_x = x;
	m_y = y;
	m_z = z;
}

auto Vector3::ToArray() -> float*
{
	return reinterpret_cast< float* >( this );
}

auto Vector3::ToArray() const -> const float*
{
	return reinterpret_cast< const float* >( this );
}

auto Vector3::At( unsigned int index ) -> float&
{
	auto data = ToArray();
	return data[ index ];
}

auto Vector3::At( unsigned int index ) const -> const float
{
	auto data = ToArray();
	return data[ index ];
}

auto Vector3::IsEqual( const Vector3& vector ) const -> bool
{
	return ( m_x == vector[ 0 ] &&
			 m_y == vector[ 1 ] &&
			 m_z == vector[ 2 ] );
}

auto Vector3::Dot( const Vector3& vector ) const -> float
{
	return ( m_x * vector[ 0 ] +
			 m_y * vector[ 1 ] +
			 m_z * vector[ 2 ] );
}

auto Vector3::LengthSquared() const -> float
{
	return Dot( *this );
}

auto Vector3::Length() const -> float
{
	return std::sqrt( LengthSquared() );
}

auto Vector3::Length2DSquared() const -> float
{
	return ( m_x * m_x +
			 m_y * m_y );
}

auto Vector3::Length2D() const -> float
{
	return std::sqrt( Length2DSquared() );
}

auto Vector3::Add( const Vector3& vector ) -> void
{
	m_x += vector[ 0 ];
	m_y += vector[ 1 ];
	m_z += vector[ 2 ];
}

auto Vector3::Subtract( const Vector3& vector ) -> void
{
	m_x -= vector[ 0 ];
	m_y -= vector[ 1 ];
	m_z -= vector[ 2 ];
}

auto Vector3::Multiply( const Vector3& vector ) -> void
{
	m_x *= vector[ 0 ];
	m_y *= vector[ 1 ];
	m_z *= vector[ 2 ];
}

auto Vector3::Divide( const Vector3& vector ) -> void
{
	m_x /= vector[ 0 ];
	m_y /= vector[ 1 ];
	m_z /= vector[ 2 ];
}

auto Vector3::Add( float value ) -> void
{
	m_x += value;
	m_y += value;
	m_z += value;
}

auto Vector3::Subtract( float value ) -> void
{
	m_x -= value;
	m_y -= value;
	m_z -= value;
}

auto Vector3::Multiply( float value ) -> void
{
	m_x *= value;
	m_y *= value;
	m_z *= value;
}

auto Vector3::Divide( float value ) -> void
{
	m_x /= value;
	m_y /= value;
	m_z /= value;
}

auto Vector3::DistanceSquared( const Vector3& vector ) const -> float
{
	auto data = *this;
	data.Subtract( vector );
	return data.LengthSquared();
}

auto Vector3::Distance( const Vector3& vector ) const -> float
{
	auto data = *this;
	data.Subtract( vector );
	return data.Length();
}

auto Vector3::Normalize() -> float
{
	auto length = Length();
	auto length_normal = 1.f / length;

	m_x *= length_normal;
	m_y *= length_normal;
	m_z *= length_normal;

	return length;
}

auto Vector3::NormalizeFast() -> void
{
	auto length = Length();
	auto length_normal = 1.f / length;

	m_x *= length_normal;
	m_y *= length_normal;
	m_z *= length_normal;
}

auto Vector3::Transform( const matrix3x4_t& transform ) const -> Vector3
{
	return { Dot( transform[ 0 ] ) + transform[ 0 ][ 3 ],
			 Dot( transform[ 1 ] ) + transform[ 1 ][ 3 ],
			 Dot( transform[ 2 ] ) + transform[ 2 ][ 3 ] };
}

auto Vector3::IsZero() const -> bool
{
	return IsEqual( Vector3::Zero );
}

auto Vector3::Cross( const Vector3& vector ) -> Vector3
{
	return { m_y * vector[ 2 ] - m_z * vector[ 1 ],
			 m_z * vector[ 0 ] - m_x * vector[ 2 ],
			 m_x * vector[ 1 ] - m_y * vector[ 0 ] };
}

auto Vector3::ToVector4D( float w /*= 0.f*/ ) const -> Vector4
{
	return { m_x, m_y, m_z, w };
}

auto Vector3::operator = ( const Vector3& vector ) -> Vector3&
{
	Init( vector[ 0 ], vector[ 1 ], vector[ 2 ] );
	return *this;
}

auto Vector3::operator [] ( unsigned int index ) -> float&
{
	return At( index );
}

auto Vector3::operator [] ( unsigned int index ) const -> const float
{
	return At( index );
}

auto Vector3::operator == ( const Vector3& vector ) const -> bool
{
	return IsEqual( vector );
}

auto Vector3::operator != ( const Vector3& vector ) const -> bool
{
	return !IsEqual( vector );
}

auto Vector3::operator += ( const Vector3& vector ) -> Vector3&
{
	Add( vector );
	return *this;
}

auto Vector3::operator -= ( const Vector3& vector ) -> Vector3&
{
	Subtract( vector );
	return *this;
}

auto Vector3::operator *= ( const Vector3& vector ) -> Vector3&
{
	Multiply( vector );
	return *this;
}

auto Vector3::operator /= ( const Vector3& vector ) -> Vector3&
{
	Divide( vector );
	return *this;
}

auto Vector3::operator += ( float value ) -> Vector3&
{
	Add( value );
	return *this;
}

auto Vector3::operator -= ( float value ) -> Vector3&
{
	Subtract( value );
	return *this;
}

auto Vector3::operator *= ( float value ) -> Vector3&
{
	Multiply( value );
	return *this;
}

auto Vector3::operator /= ( float value ) -> Vector3&
{
	Divide( value );
	return *this;
}

auto Vector3::operator + ( const Vector3& vector ) const -> Vector3
{
	auto data = *this;
	data.Add( vector );
	return data;
}

auto Vector3::operator - ( const Vector3& vector ) const -> Vector3
{
	auto data = *this;
	data.Subtract( vector );
	return data;
}

auto Vector3::operator * ( const Vector3& vector ) const -> Vector3
{
	auto data = *this;
	data.Multiply( vector );
	return data;
}

auto Vector3::operator / ( const Vector3& vector ) const -> Vector3
{
	auto data = *this;
	data.Divide( vector );
	return data;
}

auto Vector3::operator + ( float value ) const -> Vector3
{
	auto data = *this;
	data.Add( value );
	return data;
}

auto Vector3::operator - ( float value ) const -> Vector3
{
	auto data = *this;
	data.Subtract( value );
	return data;
}

auto Vector3::operator * ( float value ) const -> Vector3
{
	auto data = *this;
	data.Multiply( value );
	return data;
}

auto Vector3::operator / ( float value ) const -> Vector3
{
	auto data = *this;
	data.Divide( value );
	return data;
}


Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };